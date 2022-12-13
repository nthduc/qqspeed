/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "pch.h"
#include "BugslayerUtil.h"
#include "DeadlockDetection.h"
#include "DeadlockDetection_RC.h"
#include "DDExtHandler.h"
#include "PersistSettings.h"
#include "DD_Funcs.h"
#include "Internal.h"
#include "ModList.h"
#include "Tables.h"

/*//////////////////////////////////////////////////////////////////////
                             File Constants
//////////////////////////////////////////////////////////////////////*/
// The default extension DLL.
const LPCTSTR k_szDEFEXTDLLNAME = _T ( "DeadDetExt.DLL" ) ;
// The default for starting in DllMain.
const BOOL    k_bSTARTINDLLMAIN = FALSE ;
// The default logging options.
const DWORD   k_dwDEFAULTOPTS   = DDOPT_ALL ;
// The interesting module strings.
const LPCTSTR k_KERNEL32 = _T ( "KERNEL32.DLL" ) ;
const LPCTSTR k_USER32 = _T ( "USER32.DLL" ) ;
const LPCTSTR k_BUGSLAYERUTIL = _T ( "BUGSLAYERUTIL.DLL" ) ;

/*//////////////////////////////////////////////////////////////////////
                           File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The HINSTANCE for DeadlockDetection.DLL.
static HINSTANCE g_hInst = NULL ;
// The flag that indicates that the deadlock system is initialized.
static BOOL g_bInitialized = FALSE ;
// The private heap that everything is allocated from for this whole
// module.  By doing this, I do not interfere with the user's memory
// scheme.
static HANDLE g_hHeap ;
// The processed module list class.
static CModList g_cProcessedModList ;
// The ignore module list.
static CModList g_cIgnoreList ;
// The critical section that is used to protect everything that can be
// called from outside this file.
static CRITICAL_SECTION g_CritSec ;
// The DeadDet Extension DLL handler class.
static CDDExtHandler g_cDDExtHandler ;
// The current options.
static DWORD g_dwOptions ;
// The suspended state flag.
static BOOL  g_bSuspended = FALSE ;


/*//////////////////////////////////////////////////////////////////////
                          File Scope Functions
//////////////////////////////////////////////////////////////////////*/
static void InitTables ( void ) ;
static void InitIgnoreModList ( CModList & cList ) ;
static BOOL UnHookModuleForDeadlockDetection ( HINSTANCE hMod ) ;
static BOOL HookModuleForDeadlockDetection ( HINSTANCE hMod ) ;
static BOOL InitCrtTable ( HINSTANCE hMod , LPCTSTR szModName ) ;
static BOOL HookExitProcess ( HINSTANCE hMod ) ;
static BOOL UnhookExitProcess ( HINSTANCE hMod ) ;

/*//////////////////////////////////////////////////////////////////////
                    Exported Function Implementation
//////////////////////////////////////////////////////////////////////*/

// See description in DeadlockDetection.h.
DEADDET_DLLINTERFACE BOOL WINAPI OpenDeadlockDetection ( DWORD dwOpts )
{
    // If we have already been initialized, just kick out now.
    if ( TRUE == g_bInitialized )
    {
        SetLastError ( ERROR_ALREADY_INITIALIZED ) ;
        return ( FALSE ) ;
    }
    __try
    {
        __try
        {
            // Initialize the critical section that protects life as
            // the deadlock detection system knows it.
            InitializeCriticalSectionAndSpinCount ( &g_CritSec , 4000 );
            // Now grab the critical section.
            EnterCriticalSection ( &g_CritSec ) ;

            // Initialize the tables.
            InitTables ( ) ;

            // Set the options.
            g_dwOptions = dwOpts ;

            // Figure out the extension DLL to load.
            TCHAR szExtName [ MAX_PATH ] ;

            VERIFY ( GetExtDllFilename ( szExtName            ,
                                         sizeof ( szExtName ) ,
                                         k_szDEFEXTDLLNAME     ) ) ;

            // Load and verify the extension DLL.
            if ( FALSE == g_cDDExtHandler.LoadDDExtDll ( szExtName ) )
            {
                ShowFatalError ( g_hInst            ,
                                 IDS_NODDEXTDLLLOAD ,
                                 szExtName           ) ;
                g_bInitialized = FALSE ;
                __leave ;
            }

            if ( FALSE == g_cDDExtHandler.DeadDetExtOpen ( ) )
            {
                ShowFatalError ( g_hInst             ,
                                 IDS_DDEXTFAILEDINIT ,
                                 szExtName            ) ;
                g_bInitialized = FALSE ;
                __leave ;
            }

            // Create the heap that everything is allocated from.
            g_hHeap = HeapCreate ( 0 , 0 , 0 ) ;

            ASSERT ( NULL != g_hHeap ) ;
            if ( NULL == g_hHeap )
            {
                ShowFatalError ( g_hInst , IDS_NOHEAPCREATE ) ;
                g_bInitialized = FALSE ;
                __leave ;
            }

            // Set the module lists heap.
            g_cProcessedModList.SetHeap ( g_hHeap ) ;
            g_cIgnoreList.SetHeap ( g_hHeap ) ;

            InitIgnoreModList ( g_cIgnoreList ) ;

            // The basic initialization work is done.  At this point
            // start figuring out the DLLs that are in this address
            // space and patch them.
            HookAllLoadedModules ( ) ;

            // Everything worked so everything is initialized.
            g_bInitialized = TRUE ;
        }
        __except ( EXCEPTION_EXECUTE_HANDLER )
        {
            ASSERT ( FALSE ) ;
            // Make sure the initialization settings get set to FALSE.
            g_bInitialized = FALSE ;
        }
    }
    __finally
    {
        // Always release the critical section here.
        LeaveCriticalSection ( &g_CritSec ) ;

        // Was there a problem initializing?
        if ( FALSE == g_bInitialized )
        {
            // Since nothing should own the critical section, get rid
            // of it here.
            DeleteCriticalSection ( &g_CritSec ) ;
        }
    }
    // Always return the initialization state.
    return ( g_bInitialized ) ;
}

// See description in DeadlockDetection.h.
DEADDET_DLLINTERFACE void WINAPI CloseDeadlockDetection ( void )
{
    // If not initialize, kick out now.
    if ( FALSE == g_bInitialized )
    {
        SetLastError ( ERROR_ACCESS_DENIED ) ;
        return ;
    }
    __try
    {
        // Always grab the critical section.
        EnterCriticalSection ( &g_CritSec ) ;
        __try
        {
            // Force the mode to suspended.
            SuspendDeadlockDetection ( ) ;
            
            // Indicate were no longer initialize.
            g_bInitialized = FALSE ;

            // Do all the unhooking.
            for ( UINT i = 0                            ;
                  i < g_cProcessedModList.GetCount ( )  ;
                  i++                                    )
            {
                UnHookModuleForDeadlockDetection (
                                        g_cProcessedModList [ i ] ) ;
            }
            
            // Unhook ExitProcess in all the ignore list modules.
            for ( UINT i = 0                            ;
                  i < g_cIgnoreList.GetCount ( )        ;
                  i++                                    )
            {
                UnhookExitProcess ( g_cIgnoreList [ i ] ) ;
            }

            // Tell the extension DLL that it is done.
            g_cDDExtHandler.DeadDetExtClose ( ) ;

            // Unload the extension DLL.
            g_cDDExtHandler.FreeDDExtDll ( ) ;

            // Clear out the module lists.
            g_cProcessedModList.Clear ( ) ;
            g_cProcessedModList.SetHeap ( NULL ) ;
            g_cIgnoreList.Clear ( ) ;
            g_cIgnoreList.SetHeap ( NULL ) ;

            // Shut down the private heap.
            VERIFY ( HeapDestroy ( g_hHeap ) ) ;
            g_hHeap = NULL ;
        }
        __except ( EXCEPTION_EXECUTE_HANDLER )
        {
            ASSERT ( FALSE ) ;
        }
    }
    __finally
    {
        // Get rid of the critical section.
        LeaveCriticalSection ( &g_CritSec ) ;
        DeleteCriticalSection ( &g_CritSec ) ;
    }
}

// See description in DeadlockDetection.h.
DEADDET_DLLINTERFACE DWORD WINAPI GetDeadlockDetectionOptions ( void )
{
    return ( g_dwOptions ) ;
}

// See description in DeadlockDetection.h.
DEADDET_DLLINTERFACE DWORD WINAPI
                         SetDeadlockDetectionOptions ( DWORD dwNewOpts )
{
    // If not initialized, kick out.
    if ( FALSE == g_bInitialized )
    {
        SetLastError ( ERROR_ACCESS_DENIED ) ;
        return ( 0 ) ;
    }
    DWORD dwOldOpts = (DWORD)InterlockedExchange((LPLONG)&g_dwOptions ,
                                                  dwNewOpts           );
    return ( dwOldOpts ) ;
}

DEADDET_DLLINTERFACE void WINAPI SuspendDeadlockDetection ( void )
{
    if ( FALSE == g_bInitialized )
    {
        SetLastError ( ERROR_ACCESS_DENIED ) ;
        return ;
    }

    InterlockedExchange ( (LPLONG)&g_bSuspended , TRUE ) ;
}

DEADDET_DLLINTERFACE void WINAPI ResumeDeadlockDetection ( void )
{
    if ( FALSE == g_bInitialized )
    {
        SetLastError ( ERROR_ACCESS_DENIED ) ;
        return ;
    }

    InterlockedExchange ( (LPLONG)&g_bSuspended , FALSE ) ;
}

DEADDET_DLLINTERFACE BOOL WINAPI
                    WriteToDeadlockDetectionLog ( LPCTSTR szMsg )
{
    EnterCriticalSection ( &g_CritSec ) ;
    BOOL bRet = g_cDDExtHandler.DeadDetWriteToLog ( szMsg ) ;
    LeaveCriticalSection ( &g_CritSec ) ;
    return ( bRet ) ;
}


/*//////////////////////////////////////////////////////////////////////
                     Internal Function Definitions
//////////////////////////////////////////////////////////////////////*/

// Ye ol' standard DLL main.
BOOL WINAPI DllMain ( HINSTANCE hInst       ,
                      DWORD     dwReason    ,
                      LPVOID    /*lpReserved*/ )
{
    BOOL bRet = TRUE ;
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH :
            // Save off the DLL hInstance.
            g_hInst = hInst ;
            // I don't need the thread notifications.
            DisableThreadLibraryCalls ( g_hInst ) ;
            // Set the name of the settings file.
            SetPersistentFilename ( g_hInst ) ;
            // Get the default options.
            g_dwOptions = DefaultEventOptions ( k_dwDEFAULTOPTS ) ;
            // Do I start the whole thing on the load?
            if ( TRUE == StartInDllMain ( k_bSTARTINDLLMAIN ) )
            {
                bRet = OpenDeadlockDetection ( g_dwOptions ) ;
                ASSERT ( TRUE == bRet ) ;
            }
            break ;
        case DLL_PROCESS_DETACH :
            // If the library is initialized, then force the close down
            // just in case.
            if ( TRUE == g_bInitialized )
            {
                CloseDeadlockDetection ( ) ;
            }
            break ;
        default                 :
            break ;
    }
    return ( bRet ) ;
}

// See INTERNAL.H.
BOOL HookAllLoadedModules ( void )
{
    // I'm much prefer not to have to grab this critical section for the
    // life of this function.  However, it is possible that one thread
    // could be building a list of newly hooked modules and another
    // thread could be unloading one of those modules and building it's
    // own list.  Consequently, I have to treat the list building
    // activity itself as an atomic operation because if I only protect
    // g_cProcessedModList, I could end up with a poorly built list.
    EnterCriticalSection ( &g_CritSec ) ;

    // Allocate and fill the array of loaded modules.
    DWORD dwModCount = 0 ;
    HMODULE * phMods =
                AllocAndFillProcessModuleList ( g_hHeap , &dwModCount );

    // Did we get it?
    ASSERT ( NULL != phMods ) ;
    ASSERT ( 0 != dwModCount ) ;
    if ( ( NULL == phMods ) || ( 0 == dwModCount ) )
    {
        LeaveCriticalSection ( &g_CritSec ) ;
        ShowFatalError ( g_hInst , IDS_NOGETLOADEDMODS ) ;
        return ( FALSE ) ;
    }

    // Keep the updated list in cTempList.
    CModList cTempList ;
    cTempList.SetHeap ( g_hHeap ) ;

    // Loop through the modules and hook them if needed.
    for ( DWORD i = 0 ; i < dwModCount ; i++ )
    {
        // Has this module already been processed?
        if ( FALSE == g_cProcessedModList.IsInList ( phMods [ i ] ) )
        {
            // Check to see if this module is in the ignore list.
            if ( FALSE == g_cIgnoreList.IsInList ( phMods [ i ] ) )
            {
                TCHAR szModName[ MAX_PATH ] ;
                DWORD dwRet = GetModuleBaseName ( GetCurrentProcess () ,
                                                  phMods[ i ]          ,
                                                  szModName            ,
                                                  MAX_PATH            );
                ASSERT ( 0 != dwRet ) ;
                if ( 0 != dwRet )
                {
                    BOOL bIsCrtModule = FALSE ;
                    // Is this one of the CRT modules?
                    if ( 0 == _tcsnicmp ( szModName                 ,
                                          _T ( "MSVCR" )            ,
                                          _tcslen ( _T ( "MSVCR" ) ) ) )
                    {
                        bIsCrtModule = InitCrtTable ( phMods[ i ] ,
                                                      szModName    ) ;
                    }
                    if ( FALSE == bIsCrtModule )
                    {
                        HookModuleForDeadlockDetection ( phMods [ i ] );
                    }
                    else
                    {
                        // It is a CRT module so I need to hook it's
                        // call to ExitProcess.
                        HookExitProcess ( phMods[ i ] ) ;
                    }
                    // The module has been processed, stick it in the
                    // temp list.
                    cTempList.Add ( phMods[ i ] ) ;
                }
            }
        }
    }
    
    // Copy the temp list into the real list.
    g_cProcessedModList = cTempList ;
    LeaveCriticalSection ( &g_CritSec ) ;

    // Get rid of the memory allocated earlier.
    VERIFY ( HeapFree ( g_hHeap , 0 , phMods ) ) ;

    // All OK, Jump Master!
    return ( TRUE ) ;
}

BOOL InitCrtTable ( HINSTANCE hMod , LPCTSTR szModName )
{
    LPCRTDLLDESC pCrtDesc = NULL ;
    BOOL bRet = TRUE ;
    // Loop through and find this module.
    for ( int i = 0 ; i < NUM_CRT_DLLS ; i++ )
    {
        if ( 0 == _tcsicmp ( szModName , g_stCRTDLLInfo[i].szDllName ) )
        {
            pCrtDesc = &g_stCRTDLLInfo[ i ] ;
            break ;
        }
    }
    if ( NULL == pCrtDesc )
    {
        // This means someone named a DLL starting with "MSVCR" (not a
        // good idea).
        bRet = FALSE ;
    }
    else
    {
        // Got a match.  Copy the current HINSTANCE into the slot.
        pCrtDesc->hMod = hMod ;
        // Loop through and get the particular exports for this
        // DLL.
        for ( int iFuncCnt = 0               ;
              iFuncCnt < NUMBER_CRTDLL_FUNCS ;
              iFuncCnt++                      )
        {
            pCrtDesc->pRealFuncDesc[ iFuncCnt ].pProc =
                    GetProcAddress ( hMod ,
                              pCrtDesc->pRealFuncDesc[iFuncCnt].szFunc);
            ASSERT ( NULL != pCrtDesc->pRealFuncDesc[ iFuncCnt ].pProc);
            if ( NULL == pCrtDesc->pRealFuncDesc[ iFuncCnt ].pProc )
            {
                bRet = FALSE ;
                ASSERT ( !"GetProcAddress failed!!" ) ;
                break ;
            }
        }
    }
    return ( bRet ) ;
}

// See INTERNAL.H
void ResetModuleListAfterModuleUnload ( void )
{
    // See the comment in HookAllLoadedModules as to why I have to
    // protect even the temporary list creation.
    EnterCriticalSection ( &g_CritSec ) ;

    // Create a temporary list.
    // Allocate and fill the array of loaded modules.
    DWORD dwModCount = 0 ;
    HMODULE * phMods =
                AllocAndFillProcessModuleList ( g_hHeap , &dwModCount );

    // Did we get it?
    ASSERT ( NULL != phMods ) ;
    ASSERT ( 0 != dwModCount ) ;
    if ( ( NULL == phMods ) || ( 0 == dwModCount ) )
    {
        LeaveCriticalSection ( &g_CritSec ) ;
        ShowFatalError ( g_hInst , IDS_NOGETLOADEDMODS ) ;
    }
    
    // Empty the list but don't delete the memory.
    g_cProcessedModList.EmptyItems ( ) ;
    
    // Keeps track of which CRT modules are still loaded.
    BOOL bSeenCRT[NUM_CRT_DLLS] ;
    ZeroMemory ( &bSeenCRT , sizeof ( bSeenCRT ) ) ;
    
    for ( DWORD i = 0 ; i < dwModCount ; i++ )
    {
        if ( FALSE == g_cIgnoreList.IsInList ( phMods [ i ] ) )
        {
            // Look to see if this module is one in the CRT array.
            for ( DWORD k = 0 ; k < NUM_CRT_DLLS ; k++ )
            {
                if ( phMods[ i ] == g_stCRTDLLInfo[ k ].hMod )
                {
                    bSeenCRT[ k ] = TRUE ;
                }
            }
            // Add the module to the processed list.
            g_cProcessedModList.Add ( phMods[ i ] ) ;
        }
    }
    
    // Check to see if any CRT DLLs happened to get unloaded.
    for ( DWORD m = 0 ; m < NUM_CRT_DLLS; m++ )
    {
        if ( ( FALSE == bSeenCRT[ m ]        ) &&
             ( 0 != g_stCRTDLLInfo[ m ].hMod )    )
        {
            // Got an unloaded CRT module!
            for ( DWORD q = 0 ; q < NUMBER_CRTDLL_FUNCS ; q++ )
            {
                // Zero out all the real CRT functions for this DLL in
                // the unlikely case it comes back in at a different
                // address.
                g_stCRTDLLInfo[ m ].pRealFuncDesc[ q ].pProc  = NULL ;
            }
        }
    }
    
    LeaveCriticalSection ( &g_CritSec ) ;
}

/*----------------------------------------------------------------------
FUNCTION        : HookModuleForDeadlockDetection
DISCUSSION      :
    Hooks the specified module's thread and synchronization functions.
PARAMETERS      :
    hMod - The module to hook.
RETURNS         :
    TRUE  - Life is good.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
static BOOL HookModuleForDeadlockDetection ( HINSTANCE hMod )
{
    // Note that all the functions for the module are always hooked.
    // It is up to the actual DD* hookee functions to determine if
    // they are active through the options or not.
    // By hooking all the functions, the user can toggle various sets
    // on and off on the fly!
    BOOL bRet = TRUE ;
#ifdef _DEBUG
    {
        TCHAR szName[ MAX_PATH ] ;
        VERIFY ( GetModuleFileName ( hMod , szName , MAX_PATH ) ) ;
        TRACE ( "Hooking module -> %S\n" , szName ) ;
    }
#endif

    // Do KERNEL32.DLL and USER32.DLL.
    if (  ( FALSE ==
                HookImportedFunctionsByName ( hMod                ,
                                              k_KERNEL32          ,
                                              NUMBER_KERNEL_FUNCS ,
                                              g_stDDKernelFuncs   ,
                                              NULL                ,
                                              NULL                 )) ||
          ( FALSE ==
                HookImportedFunctionsByName ( hMod              ,
                                              k_USER32          ,
                                              NUMBER_USER_FUNCS ,
                                              g_stDDUserFuncs   ,
                                              NULL              ,
                                              NULL                 ))  )
    {
        ASSERT ( !"HookModuleForDeadlockDetection failed!!") ;
        bRet = FALSE ;
    }
    // Rip through the CRT DLLs.
    for ( int i = 0 ; i < NUM_CRT_DLLS ; i++ )
    {
        // All the CRT functions share g_stCRTHookFuncs as it's the same
        // thing.
        bRet = HookImportedFunctionsByName(hMod                        ,
                                           g_stCRTDLLInfo[i].szDllName ,
                                           NUMBER_CRTDLL_FUNCS         ,
                                           g_stCRTHookFuncs            ,
                                           NULL                        ,
                                           NULL                       );
        ASSERT ( TRUE == bRet ) ;
    }
    return ( bRet ) ;
}

/*----------------------------------------------------------------------
FUNCTION        : UnHookModuleForDeadlockDetection
DISCUSSION      :
    Puts the original thread and synchronization functions back.
PARAMETERS      :
    hMod - The module to unhook.
RETURNS         :
    TRUE  - Life is good.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
static BOOL UnHookModuleForDeadlockDetection ( HINSTANCE hMod )
{
    // Check to see if this module is still in memory.  The only way
    // this condition could happen is if the user called
    // FreeLibraryAndExitThread and immediately called
    // CloseDeadlockDetection with no intervining LoadLibrary* or a call
    // to FreeLibrary.  Admittedly, this is a pretty rare case.
    TCHAR szName[ MAX_PATH ] ;
    BOOL bRet = TRUE ;
    
    if ( 0 != GetModuleFileName ( hMod , szName , MAX_PATH ) )
    {
        TRACE ( "Unhooking module -> %S\n" , szName ) ;
        

        HookImportedFunctionsByName ( hMod                  ,
                                      k_KERNEL32            ,
                                      NUMBER_KERNEL_FUNCS   ,
                                      g_stRealKernelFuncs   ,
                                      NULL                  ,
                                      NULL                   ) ;

        HookImportedFunctionsByName ( hMod              ,
                                      k_USER32          ,
                                      NUMBER_USER_FUNCS ,
                                      g_stRealUserFuncs ,
                                      NULL              ,
                                      NULL               ) ;

        // Do each of the CRT functions.
        for ( int i = 0 ; i < NUM_CRT_DLLS ; i++ )
        {
            if ( 0 != g_stCRTDLLInfo[i].hMod )
            {
                HookImportedFunctionsByName ( hMod ,
                                       g_stCRTDLLInfo[i].szDllName     ,
                                       NUMBER_CRTDLL_FUNCS             ,
                                       g_stCRTDLLInfo[i].pRealFuncDesc ,
                                       NULL                            ,
                                       NULL                           );
            }
        }
    }
    else
    {
        bRet = FALSE ;
    }
    return ( bRet ) ;
}

// See INTERNAL.H.
void ProcessEvent ( LPDDEVENTINFO lpEvtInfo )
{
    // Grab the critical section.
    //EnterCriticalSection ( &g_CritSec ) ;
    // Post the data.
    if ( FALSE == g_cDDExtHandler.DeadDetProcessEvent ( lpEvtInfo ) )
    {
        // Since there was a problem, shift to suspended mode.
        SuspendDeadlockDetection ( ) ;
    }
    // Release the critical section.
    //LeaveCriticalSection ( &g_CritSec ) ;
}

// See INTERNAL.H.
BOOL DoLogging ( DWORD dwItem )
{
    if ( ( dwItem == ( g_dwOptions & dwItem ) ) &&
         ( FALSE == g_bSuspended              )    )
    {
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

// See INTERNAL.H.
BOOL IsSuspended ( void )
{
    return ( g_bSuspended ) ;
}

/*----------------------------------------------------------------------
FUNCTION        :   InitTables
DISCUSSION      :
    Initializes those real function tables with NULL values.  Those are
the functions that are either new or not supported on multiple operating
systems.
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
static void InitTables ( void )
{
    HMODULE hKernel = GetModuleHandle ( k_KERNEL32 ) ;
    HMODULE hUser   = GetModuleHandle ( k_USER32 ) ;

    ASSERT ( NULL != hKernel ) ;
    ASSERT ( NULL != hUser ) ;

    if ( ( NULL != hKernel ) && ( NULL != hUser ) )
    {
        // Plow though the KERNEL32 functions.
        for ( int i = 0 ; i < NUMBER_KERNEL_FUNCS ; i++ )
        {
            if ( NULL == g_stRealKernelFuncs[ i ].pProc )
            {
                g_stRealKernelFuncs[ i ].pProc
                    = (PROC)GetProcAddress ( hKernel ,
                                         g_stRealKernelFuncs[i].szFunc);
            }
        }
        // Plow through the USER32 functions.
        for ( i = 0 ; i < NUMBER_USER_FUNCS ; i++ )
        {
            if ( NULL == g_stRealUserFuncs[ i ].pProc )
            {
                g_stRealUserFuncs[ i ].pProc
                    = (PROC)GetProcAddress ( hUser ,
                                           g_stRealUserFuncs[i].szFunc);
            }
        }
    }
}

/*----------------------------------------------------------------------
FUNCTION        : InitIgnoreModList
DISCUSSION      :
    Initializes the ignore module list.
PARAMETERS      :
    cList - The particular list to initialize.
RETURNS         :
    None.
----------------------------------------------------------------------*/
static void InitIgnoreModList ( CModList & cList )
{
    // Clear out any modules that were there.
    cList.Clear ( ) ;

    // Always add the deadlock detection module, the DDExt module, and
    // any other modules I load to the module list so I don't patch
    // them.  These are the only two modules where I don't hook
    // ExitProcess.
    cList.Add ( g_hInst ) ;
    HMODULE hTemp = g_cDDExtHandler.GetDDExtHandle ( ) ;
    cList.Add ( hTemp ) ;
    
    // Keep BugslayerUtil.DLL out of the mix.
    hTemp = GetModuleHandle ( _T ( "BUGSLAYERUTIL.DLL" ) ) ;
    if ( NULL != hTemp )
    {
        cList.Add ( hTemp ) ;
        HookExitProcess ( hTemp ) ;
    }
    
    // Keep USER32.DLL and KERNEL32.DLL out of the mix.  I can't monitor
    // anything I'm hooking.
    hTemp = GetModuleHandle ( _T ( "USER32.DLL" ) ) ;
    if ( NULL != hTemp )
    {
        cList.Add ( hTemp ) ;
        HookExitProcess ( hTemp ) ;
    }
    hTemp = GetModuleHandle ( _T ( "KERNEL32.DLL" ) ) ;
    if ( NULL != hTemp )
    {
        cList.Add ( hTemp ) ;
        HookExitProcess ( hTemp ) ;
    }
    
    int iCurrMod = 1 ;
    TCHAR szMod[ MAX_PATH ] ;
    while ( GetIgnoreModule ( iCurrMod , szMod ) )
    {
        iCurrMod++ ;
        hTemp = GetModuleHandle ( szMod ) ;
        if ( NULL != hTemp )
        {
            cList.Add ( hTemp ) ;
            HookExitProcess ( hTemp ) ;
        }
    }
}

BOOL HookExitProcess ( HINSTANCE hMod )
{
    BOOL bRet =
            HookImportedFunctionsByName ( hMod                        ,
                                          k_KERNEL32                  ,
                                          1                           ,
                                          &g_stSpecialExitProcessHook ,
                                          NULL                        ,
                                          NULL                        );
    return ( bRet ) ;
}

BOOL UnhookExitProcess ( HINSTANCE hMod )
{
    BOOL bRet =
            HookImportedFunctionsByName ( hMod                        ,
                                          k_KERNEL32                  ,
                                          1                           ,
                                          &g_stSpecialExitProcessReal ,
                                          NULL                        ,
                                          NULL                        );
    return ( bRet ) ;
}
