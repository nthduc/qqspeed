/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "PCH.h"
#include "DebugInterface.h"
#include "DebuggeeInfo.h"
#include "ProcessDebugEvents.h"

/*//////////////////////////////////////////////////////////////////////
                        File Scope Declarations
//////////////////////////////////////////////////////////////////////*/
// Handler for CREATE_PROCESS_DEBUG_EVENTs
static
DWORD CreateProcessEvent ( CDebugBaseUser *            pUserClass  ,
                           LPDEBUGGEEINFO              pData       ,
                           DWORD                       dwProcessId ,
                           DWORD                       dwThreadId  ,
                           CREATE_PROCESS_DEBUG_INFO & stCPDI       ) ;
                           
// Handler for LOAD_DLL_DEBUG_EVENTs
static
DWORD LoadDllDebugEvent ( CDebugBaseUser *          pUserClass  ,
                          LPDEBUGGEEINFO            pData       ,
                          LPDEBUG_EVENT             pDE          ) ;
                                                    
// Handler for OUTPUT_DEBUG_STRING_EVENTs
static
DWORD OutputDebugStringEvent ( CDebugBaseUser *           pUserClass  ,
                               LPDEBUGGEEINFO             pData       ,
                               DWORD                      dwProcessId ,
                               DWORD                      dwThreadId  ,
                               OUTPUT_DEBUG_STRING_INFO & stODSI     ) ;

// Handler for RIP_EVENTs
static
DWORD RipEvent ( CDebugBaseUser * pUserClass  ,
                 LPDEBUGGEEINFO   pData       ,
                 DWORD            dwProcessId ,
                 DWORD            dwThreadId  ,
                 RIP_INFO &       stRI         ) ;

static
DWORD ExceptionEvent ( CDebugBaseUser *       pUserClass  ,
                       LPDEBUGGEEINFO         pData       ,
                       DWORD                  dwProcessId ,
                       DWORD                  dwThreadId  ,
                       EXCEPTION_DEBUG_INFO & stEDI        ) ;
                       
// Helps with the symbol loading.
static
void FillModuleInfo ( LPDEBUGGEEINFO       pData       ,
                      LPCVOID              lpBaseAddr  ,
                      PIMAGEHLP_MODULE64   pModInfo     ) ;

/*//////////////////////////////////////////////////////////////////////
                     !!IMPLEMENTATION STARTS HERE!!
//////////////////////////////////////////////////////////////////////*/

DWORD ProcessDebugEvents ( CDebugBaseUser * pUserClass ,
                           LPDEBUGGEEINFO   pData      ,
                           LPDEBUG_EVENT    pDE         )
{
    // Call me paranoid....
    ASSERT ( FALSE == IsBadReadPtr ( pUserClass ,
                                     sizeof ( CDebugBaseUser ) ) ) ;
    ASSERT ( FALSE == IsBadReadPtr ( pDE , sizeof ( LPDEBUG_EVENT ) ) );
    ASSERT ( NULL != pData ) ;

    // The return value from the handler.
    DWORD dwRet DBG_CONTINUE ;

    switch ( pDE->dwDebugEventCode )
    {
        case EXCEPTION_DEBUG_EVENT       :
            dwRet = ExceptionEvent ( pUserClass       ,
                                     pData            ,
                                     pDE->dwProcessId ,
                                     pDE->dwThreadId  ,
                                     pDE->u.Exception  ) ;
            break ;
        case CREATE_THREAD_DEBUG_EVENT   :
            {
                dwRet =
                    pUserClass->CreateThreadEvent( pDE->dwProcessId    ,
                                                   pDE->dwThreadId     ,
                                                   pDE->u.CreateThread);
            }
            break ;
        case CREATE_PROCESS_DEBUG_EVENT  :
            dwRet = CreateProcessEvent ( pUserClass               ,
                                         pData                    ,
                                         pDE->dwProcessId         ,
                                         pDE->dwThreadId          ,
                                         pDE->u.CreateProcessInfo  ) ;
            break ;
        case EXIT_THREAD_DEBUG_EVENT     :
            {
                dwRet =
                    pUserClass->ExitThreadEvent ( pDE->dwProcessId ,
                                                  pDE->dwThreadId  ,
                                          pDE->u.ExitThread.dwExitCode);
            }
            break ;
        case EXIT_PROCESS_DEBUG_EVENT    :
            {
                pUserClass->ExitProcessEvent ( pDE->dwProcessId       ,
                                               pDE->dwThreadId        ,
                                       pDE->u.ExitProcess.dwExitCode  );
            }
            break ;
        case LOAD_DLL_DEBUG_EVENT        :
            dwRet = LoadDllDebugEvent ( pUserClass , pData , pDE ) ;
            break ;
        case UNLOAD_DLL_DEBUG_EVENT      :
            {
                pUserClass->UnloadDllEvent ( pDE->dwProcessId          ,
                                             pDE->dwThreadId           ,
                                          pDE->u.UnloadDll.lpBaseOfDll);

                pData->SymUnloadModule ( pDE->u.UnloadDll.lpBaseOfDll );
            }
            break ;
        case OUTPUT_DEBUG_STRING_EVENT   :
            dwRet = OutputDebugStringEvent ( pUserClass         ,
                                             pData              ,
                                             pDE->dwProcessId   ,
                                             pDE->dwThreadId    ,
                                             pDE->u.DebugString   ) ;
            break ;
        case RIP_EVENT                   :
            dwRet = RipEvent ( pUserClass       ,
                               pData            ,
                               pDE->dwProcessId ,
                               pDE->dwThreadId  ,
                               pDE->u.RipInfo    ) ;
            break ;
        default                          :
            {
                ASSERT ( !"Unknown debug event!" ) ;

                pUserClass->Output ( _T ( "An unknown debugger event " )
                                     _T ( "occurred ->%i (0x%08X)" ) ,
                                     pDE->u.RipInfo.dwType           ,
                                     pDE->u.RipInfo.dwType            ) ;
                pUserClass->OutputCRLF ( ) ;
            }
            break ;
    }
    return ( dwRet ) ;
}


static
DWORD CreateProcessEvent ( CDebugBaseUser *            pUserClass  ,
                           LPDEBUGGEEINFO              pData       ,
                           DWORD                       dwProcessId ,
                           DWORD                       dwThreadId  ,
                           CREATE_PROCESS_DEBUG_INFO & stCPDI       )
{

    // Set the process handle, which is the first thing that has to
    // happen.
    pData->SetProcessHandle ( stCPDI.hProcess ) ;
    // The process ID.
    pData->SetProcessId ( dwProcessId ) ;

    // Initialize the symbol engine.
    pData->InitializeSymEng ( ) ;

    // Try to load this one.
    pData->SymLoadModule ( stCPDI.hFile               ,
                           pData->GetProcessName ( )  ,
                           stCPDI.lpBaseOfImage        ) ;
                           
    DWORD dwRet = DBG_CONTINUE ;

    // Send the initial create process event.
    dwRet = pUserClass->CreateProcessEvent ( dwProcessId ,
                                             dwThreadId  ,
                                             stCPDI       ) ;
    if ( DBG_CONTINUE != dwRet )
    {
        return ( dwRet ) ;
    }

    // Do the informational output.
    IMAGEHLP_MODULE64 stModInfo ;
    
    FillModuleInfo ( pData , stCPDI.lpBaseOfImage , &stModInfo );

    // Fake a LOAD_DLL_DEBUG_INFO!
    LOAD_DLL_DEBUG_INFO stLDDI ;
    stLDDI.dwDebugInfoFileOffset = 0 ;
    stLDDI.fUnicode              = stCPDI.fUnicode ;
    stLDDI.hFile                 = stCPDI.hFile ;
    stLDDI.lpBaseOfDll           = stCPDI.lpBaseOfImage ;
    stLDDI.lpImageName           = stCPDI.lpImageName ;
    stLDDI.nDebugInfoSize        = stCPDI.nDebugInfoSize ;
    
    dwRet = pUserClass->LoadDllEvent ( dwProcessId               ,
                                       dwThreadId                ,
                                       stLDDI                    ,
                                       pData->GetProcessName ( ) ,
                                       stModInfo                  ) ;
    if ( DBG_CONTINUE != dwRet )
    {
        return ( dwRet ) ;
    }
    
    // Finally, fake a little thread creation so that it is easier to
    // keep the thread information straight.
    CREATE_THREAD_DEBUG_INFO stCTI ;
    stCTI.hThread           = stCPDI.hThread ;
    stCTI.lpStartAddress    = stCPDI.lpStartAddress ;
    stCTI.lpThreadLocalBase = stCPDI.lpThreadLocalBase ;

    dwRet = pUserClass->CreateThreadEvent ( dwProcessId ,
                                            dwThreadId  ,
                                            stCTI        ) ;
    return ( dwRet ) ;
}

static
DWORD LoadDllDebugEvent ( CDebugBaseUser *          pUserClass  ,
                          LPDEBUGGEEINFO            pData       ,
                          LPDEBUG_EVENT             pDE          )
{
    TCHAR szDLLName[ MAX_PATH ] ;
    szDLLName[ 0 ] = _T ( '\0' ) ;
    
    BOOL bRet = TRUE ;

    // If NTDLL.DLL has not come in, in essense this is the first
    // DLL into the process, I can't look into the debuggee to get
    // the full path.  NTDLL.DLL is special and on W2K, the value
    // passed in the lpImageName just points to zero.  On XP, it
    // points to just "ntdll.dll" so I need to hunt it down.
    if ( FALSE == pData->GetSeenNTDLLLoad ( ) )
    {
        // Mark that I've seen it.
        pData->SetSeenNTDLLLoad ( ) ;
        
        // As I am debugging locally, get the Windows directory.
        UINT uiLen = GetWindowsDirectory ( szDLLName , MAX_PATH ) ;
        ASSERT ( uiLen > 0 ) ;
        
        // Whack on the backslash.
        szDLLName[ uiLen ] = _T ( '\\' ) ;
        uiLen++ ;
        szDLLName[ uiLen ] = _T ( '\0' ) ;
        
        _tcscpy ( szDLLName + uiLen , _T ( "NTDLL.DLL" ) ) ;
    }
    else
    {
        // The value in lpImageName is a pointer to the full path
        // and name of the DLL being loaded.  The address is in the
        // debuggee address space.
        LPCVOID lpPtr = 0 ;
        DWORD dwBytesRead = 0 ;
        HANDLE hProcess = pData->GetProcessHandle ( ) ;
        
        bRet = DBG_ReadProcessMemory ( hProcess                   ,
                                       pDE->u.LoadDll.lpImageName ,
                                       &lpPtr                     ,
                                       sizeof ( LPCVOID )         ,
                                       &dwBytesRead                ) ;
        ASSERT ( TRUE == bRet ) ;
        if ( ( TRUE == bRet ) && ( 0 != lpPtr ) )
        {
            // If the name in the debuggee is UNICODE, I can read it
            // directly into the szDLLName variable as all this code
            // is UNICODE.
            if ( TRUE == pDE->u.LoadDll.fUnicode )
            {
                // Occasionally, you can't read the whole buffer that
                // contains the name so I need to step down until
                // I can be sure there's no name at all.
                DWORD dwSize = MAX_PATH  * sizeof ( TCHAR ) ;
                do
                {
                                    
                    bRet = DBG_ReadProcessMemory ( hProcess         ,
                                                   lpPtr            ,
                                                   szDLLName        ,
                                                   dwSize           ,
                                                   &dwBytesRead      ) ;
                    dwSize = dwSize - 20 ;
                }
                while ( ( FALSE == bRet ) && ( dwSize > 20 ) ) ;
            }
            else
            {
                // Read the ANSI string and convert it to UNICODE.
                char szAnsiName[ MAX_PATH ] ;
                bRet = DBG_ReadProcessMemory ( hProcess         ,
                                               lpPtr            ,
                                               szAnsiName       ,
                                               MAX_PATH         ,
                                               &dwBytesRead      ) ;
                ASSERT ( TRUE == bRet ) ;
                if ( TRUE == bRet )
                {
                    BSUAnsi2Wide ( szAnsiName , szDLLName , MAX_PATH ) ;
                }
            }
        }
    }
    // If I could not get the DLL name, I'll resort to seeing if get it
    // with GetModuleFileNameEx.  If that can't get it.  We've got
    // trouble in River City.  Interestingly, you can't use
    // GetModuleFileNameEx except if the above fails.  What you're
    // seeing if you can't get the filename is a rebased DLL.
    if ( _T ( '\0' ) == szDLLName[ 0 ] )
    {
        DWORD dwRet =
            GetModuleFileNameEx ( pData->GetProcessHandle ( )        ,
                                 (HMODULE)pDE->u.LoadDll.lpBaseOfDll ,
                                 szDLLName                           ,
                                 MAX_PATH                             ) ;
        if ( 0 == dwRet )
        {
            // Fill the final DLL name with something.
            _tcscpy ( szDLLName                             ,
                      _T ( "UNABLE TO EXTRACT DLL NAME!!" )  ) ;
        }
    }
    
    // Upper case the name as that's the way I like it (uuh, uuh).
    _tcsupr ( szDLLName ) ;

    IMAGEHLP_MODULE64 stModInfo ;

    // Load the module into the symbol engine.
    pData->SymLoadModule ( pDE->u.LoadDll.hFile       ,
                           szDLLName                  ,
                           pDE->u.LoadDll.lpBaseOfDll  ) ;

    FillModuleInfo ( pData , pDE->u.LoadDll.lpBaseOfDll , &stModInfo );

    DWORD dwRet = pUserClass->LoadDllEvent ( pDE->dwProcessId ,
                                             pDE->dwThreadId  ,
                                             pDE->u.LoadDll   ,
                                             szDLLName        ,
                                             stModInfo         ) ;

    return ( dwRet ) ;
}

static
DWORD OutputDebugStringEvent ( CDebugBaseUser *           pUserClass  ,
                               LPDEBUGGEEINFO             pData       ,
                               DWORD                      dwProcessId ,
                               DWORD                      dwThreadId  ,
                               OUTPUT_DEBUG_STRING_INFO & stODSI     )
{
    // OutputDebugString can dump huge numbers of characters so I'll
    // just allocate each time.
    DWORD dwTotalBuffSize = stODSI.nDebugStringLength ;
    
    if ( TRUE == stODSI.fUnicode )
    {
        dwTotalBuffSize *= 2 ;
    }
    
    PBYTE pODSData = new BYTE [ dwTotalBuffSize ] ;
    
    DWORD dwRead ;
    // Read the memory.
    BOOL bRet = DBG_ReadProcessMemory( pData->GetProcessHandle ( ) ,
                                       stODSI.lpDebugStringData    ,
                                       pODSData                    ,
                                       dwTotalBuffSize             ,
                                       &dwRead                      ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( TRUE == bRet )
    {
        TCHAR * szUnicode = NULL ;
        TCHAR * szSelected = NULL ;
        if ( TRUE == stODSI.fUnicode )
        {
            szSelected = (TCHAR*)pODSData ;
        }
        else
        {
            szUnicode = new TCHAR [ stODSI.nDebugStringLength ] ;
            BSUAnsi2Wide ( (const char*)pODSData     ,
                           szUnicode                 ,
                           stODSI.nDebugStringLength  ) ;

            int iLen = (int)strlen ( (const char*)pODSData ) ;
            iLen = MultiByteToWideChar ( CP_THREAD_ACP      ,
                                         0                  ,
                                         (LPCSTR)pODSData   ,
                                         iLen               ,
                                         szUnicode          ,
                                         stODSI.nDebugStringLength ) ;
                           
            szSelected = szUnicode ;
        }
        
        LPCTSTR szTemp =
                pUserClass->ConvertCRLF ( szSelected           ,
                                          stODSI.nDebugStringLength  );
        if ( NULL != szUnicode )
        {
            delete [] szUnicode ;
        }
                                                            
        // Send the converted string on to the user class.
        pUserClass->OutputDebugStringEvent ( dwProcessId ,
                                             dwThreadId  ,
                                             szTemp       ) ;
        delete [] szTemp ;
    }
    delete [] pODSData ;
    return ( DBG_CONTINUE ) ;
}

static
DWORD RipEvent ( CDebugBaseUser * pUserClass  ,
                 LPDEBUGGEEINFO   /*pData*/       ,
                 DWORD            /*dwProcessId*/ ,
                 DWORD            /*dwThreadId*/  ,
                 RIP_INFO &       stRI         )
{
    // Has anyone ever seen one of these?

    TCHAR szLevel [ 25 ] ;
    switch ( stRI.dwType )
    {
        case SLE_ERROR       :
            _tcscpy ( szLevel , _T ( "SLE_ERROR"  ) ) ;
            break ;
        case SLE_MINORERROR  :
            _tcscpy ( szLevel , _T ( "SLE_MINORERROR" ) ) ;
            break ;
        case SLE_WARNING     :
            _tcscpy ( szLevel , _T ( "SLE_WARNING" ) ) ;
            break ;
        case 0               :
            _tcscpy ( szLevel , _T ( "None" ) ) ;
            break ;
        default              :
            wsprintf ( szLevel , _T ( "Unknown %i" ) , stRI.dwType ) ;
            break ;
    }
    pUserClass->Output ( _T ( "RIP Event, level %s" ) , szLevel ) ;
    // Get the message from the system.  We ask it to allocate the
    // message buffer so we must make sure to delete it.
    LPTSTR szMsg = NULL ;
    BOOL bAllocated = TRUE ;
    if ( 0 == FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                              FORMAT_MESSAGE_FROM_SYSTEM      ,
                              NULL                            ,
                              stRI.dwError                    ,
                              MAKELONG ( MAKELANGID ( LANG_NEUTRAL ,
                                                  SUBLANG_NEUTRAL ) ,
                                         0                        ) ,
                              szMsg                           ,
                              0                               ,
                              NULL                             ) )
    {
        szMsg = _T ( "FormatMessage failed" ) ;
        bAllocated = FALSE ;
    }
    pUserClass->Output ( _T ( "   %s" ) , szMsg ) ;
    pUserClass->OutputCRLF ( ) ;
    if ( TRUE == bAllocated )
    {
        LocalFree ( szMsg ) ;
    }

    return ( DBG_CONTINUE ) ;
}


static
DWORD ExceptionEvent ( CDebugBaseUser *       pUserClass  ,
                       LPDEBUGGEEINFO         pData       ,
                       DWORD                  dwProcessId ,
                       DWORD                  dwThreadId  ,
                       EXCEPTION_DEBUG_INFO & stEDI        )
{
    if ( ( EXCEPTION_BREAKPOINT ==
                stEDI.ExceptionRecord.ExceptionCode ) &&
         ( FALSE == pData->GetLoaderBreakpoint ( ) )     )
    {
        pData->SetLoaderBreakpoint ( ) ;
    }
    return ( pUserClass->ExceptionEvent ( dwProcessId ,
                                          dwThreadId  ,
                                          stEDI        ) ) ;
}

static
void FillModuleInfo ( LPDEBUGGEEINFO       pData       ,
                      LPCVOID              lpBaseAddr  ,
                      PIMAGEHLP_MODULE64   pModInfo     )
{
    memset ( pModInfo , NULL , sizeof ( IMAGEHLP_MODULE64 ) ) ;
    pModInfo->SizeOfStruct = sizeof ( IMAGEHLP_MODULE64 ) ;

    BOOL bSymGetMod = pData->SymGetModuleInfo ( pModInfo    ,
                                                lpBaseAddr   ) ;
    if ( FALSE == bSymGetMod )
    {
        pModInfo->SymType = SymNone ;

        // I gotta go find the module size on my own.
        IMAGE_DOS_HEADER stDOSHdrs ;
        DWORD dwBytesRead = 0 ;
        HANDLE hProcess = pData->GetProcessHandle ( ) ;
        
        BOOL bRet =
            DBG_ReadProcessMemory ( hProcess                    ,
                                    lpBaseAddr                  ,
                                    &stDOSHdrs                  ,
                                    sizeof ( IMAGE_DOS_HEADER ) ,
                                    &dwBytesRead                 ) ;
        if ( TRUE == bRet )
        {
            IMAGE_NT_HEADERS stPEHdrs ;
            LPCVOID lpPtr = ULongToPtr ( PtrToUlong ( lpBaseAddr ) +
                                         stDOSHdrs.e_lfanew         ) ;
     
            bRet =
                DBG_ReadProcessMemory ( hProcess                    ,
                                        lpPtr                       ,
                                        &stPEHdrs                   ,
                                        sizeof ( IMAGE_NT_HEADERS ) ,
                                        &dwBytesRead                 ) ;
            if ( TRUE == bRet )
            {
                pModInfo->ImageSize =
                            stPEHdrs.OptionalHeader.SizeOfImage ;
            }
        }
    }
}

