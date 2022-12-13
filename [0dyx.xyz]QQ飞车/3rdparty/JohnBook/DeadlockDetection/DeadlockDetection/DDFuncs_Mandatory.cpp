/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The mandatory functions that must always be hooked.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "DeadlockDetection.h"
#include "Internal.h"
#include "BugslayerUtil.h"
#include "Tables.h"

// Turn off all optimizations to avoid registers being changed behind
// my back.
#pragma optimize( "", off )

HMODULE NAKEDDEF DD_LoadLibraryA ( LPCSTR lpLibFileName )
{
    // Any local variables must be declared before the HOOKFN_PROLOG
    // macro.  This sets up the actual function prolog and automatically
    // defines some key variables such as stEvtInfo (DDEVENTINFO).
    HOOKFN_PROLOG ( ) ;

    // Before doing the call to the real function, you need to specify
    // the REAL_FUNC_PRE_CALL macro to ensure the registers get set
    // up to reflect what they were when the function was called.
    REAL_FUNC_PRE_CALL ( ) ;
    // Make the real function call.
    LoadLibraryA ( lpLibFileName ) ;
    // The macro to save off the registers after the real call.  This
    // way I can change the registers before I leave so it appears
    // that this function wasn't even there.
    REAL_FUNC_POST_CALL ( ) ;

    // The work specific to the LoadLibraryA hook.
    if ( NULL != stEvtInfo.dwRetValue )
    {
        HookAllLoadedModules ( ) ;
    }

    // Takes care of the function epilog, restoring the registers as
    // well as the stack.  The number passed to the macro is the
    // number of parameters passed to the function.  Be very careful
    // and make sure you don't get "editor inheritance" errors when you
    // cut and past this macro into other functions!
    HOOKFN_EPILOG ( 1 ) ;
}

HMODULE NAKEDDEF DD_LoadLibraryW ( LPCWSTR lpLibFileName )
{
    HOOKFN_PROLOG ( ) ;

    REAL_FUNC_PRE_CALL ( ) ;
    LoadLibraryW ( lpLibFileName ) ;
    REAL_FUNC_POST_CALL ( ) ;

    if ( NULL != stEvtInfo.dwRetValue )
    {
        HookAllLoadedModules ( ) ;
    }

    HOOKFN_EPILOG ( 1 ) ;
}

HMODULE NAKEDDEF DD_LoadLibraryExA ( LPCSTR lpLibFileName ,
                                     HANDLE hFile         ,
                                     DWORD  dwFlags        )
{
    HOOKFN_PROLOG ( ) ;

    REAL_FUNC_PRE_CALL ( ) ;
    LoadLibraryExA ( lpLibFileName ,
                     hFile         ,
                     dwFlags        ) ;
    REAL_FUNC_POST_CALL ( ) ;

    if ( ( NULL != stEvtInfo.dwRetValue        ) &&
         ( LOAD_LIBRARY_AS_DATAFILE != dwFlags )    )
    {
        HookAllLoadedModules ( ) ;
    }

    HOOKFN_EPILOG ( 3 ) ;
}

HMODULE NAKEDDEF DD_LoadLibraryExW ( LPCWSTR lpLibFileName ,
                                     HANDLE  hFile        ,
                                     DWORD   dwFlags        )
{
    HOOKFN_PROLOG ( ) ;

    REAL_FUNC_PRE_CALL ( ) ;
    LoadLibraryExW ( lpLibFileName ,
                     hFile         ,
                     dwFlags        ) ;
    REAL_FUNC_POST_CALL ( ) ;

    if ( ( NULL != stEvtInfo.dwRetValue        ) &&
         ( LOAD_LIBRARY_AS_DATAFILE != dwFlags )    )
    {
        HookAllLoadedModules ( ) ;
    }

    HOOKFN_EPILOG ( 3 ) ;
}

// Unreachable code after ExitProcess.
#pragma warning ( disable :4702 )
VOID NAKEDDEF DD_ExitProcess ( UINT uExitCode )
{
    HOOKFN_PROLOG ( ) ;

    // The DeadlockDetection system must be shut down before the real
    // Exit process is called.  If I don't unhook everything now the
    // DD_* functions will still be called AFTER this DLL has been
    // unloaded.

    // Since I want to make the ExitProcess function the last thing
    // logged, I need to be a little creative here.

    if ( FALSE == IsSuspended ( ) )
    {
        // Use the normal entry.
        FILL_EVENTINFO ( eExitProcess ) ;

        stEvtInfo.ePrePost = ePreCall ;

        // The normal exit cannot be used so do it all manually.
        ProcessEvent ( &stEvtInfo ) ;

        // Now shut off all the other function logging so nothing will
        // appear after ExitProcess.
        SuspendDeadlockDetection ( ) ;
    }

    // It's cooked!
    CloseDeadlockDetection ( ) ;

    REAL_FUNC_PRE_CALL ( ) ;
    ExitProcess ( uExitCode ) ;
    REAL_FUNC_POST_CALL ( ) ;

    HOOKFN_EPILOG ( 1 ) ;
}
// Unreachable code after ExitProcess.
#pragma warning ( default :4702 )

BOOL NAKEDDEF DD_FreeLibrary ( HMODULE hModule )
{
    HOOKFN_PROLOG ( ) ;

    REAL_FUNC_PRE_CALL ( ) ;
    FreeLibrary ( hModule ) ;
    REAL_FUNC_POST_CALL ( ) ;

    if ( TRUE == stEvtInfo.dwRetValue )
    {
        ResetModuleListAfterModuleUnload ( ) ;
    }

    HOOKFN_EPILOG ( 1 ) ;
}

// Disable the "conditional expression is constant" in the
// HOOKFN_STARTUP macro.
#pragma warning ( disable : 4127 )

// Unreachable code after FreeLibraryAndExitThread.
#pragma warning ( disable :4702 )
VOID NAKEDDEF DD_FreeLibraryAndExitThread ( HMODULE hModule    ,
                                            DWORD   dwExitCode  )
{
    HOOKFN_STARTUP ( eFreeLibraryAndExitThread , DDOPT_THREADS , TRUE );

    // Since this function does not return, it puts me in a pickle about
    // what to do about the module(s) it will unload.  What I decided is
    // that there's not much I can do.
    // 1.  My first thought was to keep a global state flag that I'd set
    //     in this function and check in all the others.  That'd be
    //     ugly and slow down all the other functions for something
    //     that'll only happen once in a million years since almost
    //     no one uses this function.
    // 2.  What I decided was that since I'm updating the loads and
    //     normal frees, those calls will automatically account for
    //     anything unloaded because of this function.  That will take
    //     care of the case where a module comes back in at a different
    //     load address, which was my main concern.
    //     The main thing is that the call will get reported to the
    //     user.

    FreeLibraryAndExitThread  ( hModule , dwExitCode ) ;
    
    HOOKFN_SHUTDOWN ( 2 , DDOPT_THREADS ) ;
}
// Unreachable code after FreeLibraryAndExitThread.
#pragma warning ( default :4702 )


static HMODULE g_hKernel32 = NULL ;
static HMODULE g_hUser32 = NULL ;

static int Compare ( HOOKFUNCDESC * stOne , HOOKFUNCDESC * stTwo )
{
    return ( lstrcmpiA ( stOne->szFunc , stTwo->szFunc ) ) ;
}

FARPROC NAKEDDEF DD_GetProcAddress( HMODULE hModule , LPCSTR lpProcName)
{
    HOOKFUNCDESC   stKey ;
    LPHOOKFUNCDESC lpRet ;
    BOOL           bOneOfMine ;

    HOOKFN_PROLOG ( ) ;

    bOneOfMine = FALSE ;
    // Initialize the statics if need be.
    if ( NULL == g_hKernel32 )
    {
        g_hKernel32 = GetModuleHandle ( _T ( "KERNEL32.DLL" ) ) ;
        g_hUser32 = GetModuleHandle ( _T ( "USER32.DLL" ) ) ;
    }

    // Is this being called with KERNEL32.DLL?
    if ( g_hKernel32 == hModule )
    {
        // Is the function one of the ones that we hooked?
        stKey.szFunc = lpProcName ;
        lpRet = (LPHOOKFUNCDESC)bsearch ( (void*)&stKey             ,
                                          (void*)&g_stDDKernelFuncs ,
                                          NUMBER_KERNEL_FUNCS       ,
                                          sizeof ( HOOKFUNCDESC )   ,
                                       (int(*)(const void*,const void*))
                                                 Compare             ) ;
        if ( NULL != lpRet )
        {
            bOneOfMine = TRUE ;
            // Return my function in place of the real one.
            // Pointer truncation.
            #pragma warning ( disable : 4311 )
            stEvtInfo.dwRetValue = (DWORD)(lpRet->pProc) ;
            #pragma warning ( default : 4311 )
            dwLastError = ERROR_SUCCESS ;
            // Set the register state to something normal.
            __asm MOV   EAX , [stEvtInfo.dwRetValue]
            __asm MOV   [stRegState.dwEAX] , EAX
            __asm XOR   EAX , EAX
            __asm LAHF
            __asm MOV   [stRegState.dwEFL] , EAX
            __asm MOV   [stRegState.dwEBX] , EBX
            __asm MOV   [stRegState.dwECX] , ECX
            __asm MOV   [stRegState.dwEDX] , EDX
            __asm MOV   [stRegState.dwEDI] , EDI
            __asm MOV   [stRegState.dwESI] , ESI
        }
    }
    // Is this being called with USER32.DLL?
    else if ( g_hUser32 == hModule )
    {
        // This is a little bit of overkill for the first version since
        // there are only two functions that are hooked out of
        // USER32.DLL.  What's really important here is that this is
        // now extensible in case more USER32.DLL functions are added.

        // Is the function one of the ones that we hooked?
        stKey.szFunc = lpProcName ;
        lpRet = (LPHOOKFUNCDESC)bsearch ( (void*)&stKey             ,
                                          (void*)&g_stDDUserFuncs   ,
                                          NUMBER_USER_FUNCS         ,
                                          sizeof ( HOOKFUNCDESC )   ,
                                       (int(*)(const void*,const void*))
                                                 Compare             ) ;
        if ( NULL != lpRet )
        {
            bOneOfMine = TRUE ;

            // Return my function in place of the real one.
            // Pointer truncation.
            #pragma warning ( disable : 4311 )
            stEvtInfo.dwRetValue = (DWORD)lpRet->pProc ;
            #pragma warning ( default : 4311 )
            
            dwLastError = ERROR_SUCCESS ;
            __asm MOV   EAX , [stEvtInfo.dwRetValue]
            __asm MOV   [stRegState.dwEAX] , EAX
            __asm XOR   EAX , EAX
            __asm LAHF
            __asm MOV   [stRegState.dwEFL] , EAX
            __asm MOV   [stRegState.dwEBX] , EBX
            __asm MOV   [stRegState.dwECX] , ECX
            __asm MOV   [stRegState.dwEDX] , EDX
            __asm MOV   [stRegState.dwEDI] , EDI
            __asm MOV   [stRegState.dwESI] , ESI
        }
    }
    else
    {
        // Lemme grind through the CRT modules.
        for ( int i = 0 ; i < NUM_CRT_DLLS ; i++ )
        {
            if ( hModule == g_stCRTDLLInfo[i].hMod )
            {
                stKey.szFunc = lpProcName ;
                lpRet = (LPHOOKFUNCDESC)bsearch ( (void*)&stKey       ,
                               (void*)g_stCRTDLLInfo[i].pHookFuncDesc ,
                                            NUMBER_CRTDLL_FUNCS       ,
                                            sizeof ( HOOKFUNCDESC )   ,
                                    (int(*)(const void*,const void*))
                                                    Compare           );
                if ( NULL != lpRet )
                {
                    bOneOfMine = TRUE ;

                    // Return my function in place of the real one.
                    // Pointer truncation.
                    #pragma warning ( disable : 4311 )
                    stEvtInfo.dwRetValue = (DWORD)lpRet->pProc ;
                    #pragma warning ( default : 4311 )
                    
                    dwLastError = ERROR_SUCCESS ;
                    __asm MOV   EAX , [stEvtInfo.dwRetValue]
                    __asm MOV   [stRegState.dwEAX] , EAX
                    __asm XOR   EAX , EAX
                    __asm LAHF
                    __asm MOV   [stRegState.dwEFL] , EAX
                    __asm MOV   [stRegState.dwEBX] , EBX
                    __asm MOV   [stRegState.dwECX] , ECX
                    __asm MOV   [stRegState.dwEDX] , EDX
                    __asm MOV   [stRegState.dwEDI] , EDI
                    __asm MOV   [stRegState.dwESI] , ESI
                    
                }
                break ;
            }
        }
    }
    
    if ( FALSE == bOneOfMine )
    {
        REAL_FUNC_PRE_CALL ( ) ;
        GetProcAddress ( hModule , lpProcName );
        REAL_FUNC_POST_CALL ( ) ;
    }
    
    HOOKFN_EPILOG ( 2 ) ;
}

// Turn optimizations back on.
#pragma optimize( "", on )

