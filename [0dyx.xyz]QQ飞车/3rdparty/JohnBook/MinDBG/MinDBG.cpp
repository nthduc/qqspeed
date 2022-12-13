/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.

The world’s simplest debugger for Win32 programs
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
// The Usual Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"

/*//////////////////////////////////////////////////////////////////////
// Prototypes and Types.
//////////////////////////////////////////////////////////////////////*/
// Shows the minimal help.
void ShowHelp ( void ) ;

// The break handler.
BOOL WINAPI CtrlBreakHandler ( DWORD dwCtrlType ) ;

// Display functions
void DisplayCreateProcessEvent ( CREATE_PROCESS_DEBUG_INFO & stCPDI ) ;
void DisplayCreateThreadEvent ( DWORD                      dwTID  ,
                                CREATE_THREAD_DEBUG_INFO & stCTDI  ) ;
void DisplayExitThreadEvent ( DWORD                    dwTID  ,
                              EXIT_THREAD_DEBUG_INFO & stETDI  ) ;
void DisplayExitProcessEvent ( EXIT_PROCESS_DEBUG_INFO & stEPDI ) ;
void DisplayDllLoadEvent ( HANDLE                hProcess ,
                           LOAD_DLL_DEBUG_INFO & stLDDI    ) ;
void DisplayDllUnLoadEvent ( UNLOAD_DLL_DEBUG_INFO & stULDDI ) ;
void DisplayODSEvent ( HANDLE                     hProcess ,
                       OUTPUT_DEBUG_STRING_INFO & stODSI    ) ;
void DisplayExceptionEvent ( EXCEPTION_DEBUG_INFO & stEDI ) ;

// The typedef for DebugActiveProcessStop.
typedef BOOL (WINAPI *PFNDEBUGACTIVEPROCESSSTOP)(DWORD) ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The flag that indicates I'm supposed to detach.
static BOOL g_bDoTheDetach = FALSE ;

/*//////////////////////////////////////////////////////////////////////
// The Entry Point.
//////////////////////////////////////////////////////////////////////*/
void _tmain ( int argc , TCHAR * argv[ ] )
{
    // Check that there is a command-line argument.
    if ( 1 == argc )
    {
        ShowHelp ( ) ;
        return ;
    }

    // Have a buffer big enough for the command and command line
    // parameters.
    TCHAR szCmdLine[ MAX_PATH + MAX_PATH ] ;
    // The possible PID if attaching.
    DWORD dwPID = 0 ;

    szCmdLine[ 0 ] = _T ( '\0' ) ;
    
    // Check if the command line starts with "-" as that indicates a
    // process ID I will attach to.
    if ( _T ( '-' ) == argv[1][0] )
    {
        // Attempt to strip off the PID from the command line option.
        
        // Move past the '-' in the string.
        TCHAR * pPID = argv[1] + 1 ;
        dwPID = _tstol ( pPID ) ;
        if ( 0 == dwPID )
        {
            _tprintf ( _T ( "Invalid PID value : %s\n" ) , pPID ) ;
            return ;
        }
    }
    else
    {
        dwPID = 0 ;
        
        // I'm going to start up the process.
        for ( int i = 1 ; i < argc ; i++ )
        {
            _tcscat ( szCmdLine , argv[ i ] ) ;
            if ( i < argc )
            {
                _tcscat ( szCmdLine , _T ( " " ) ) ;
            }
        }
    }

    // The return value holder.
    BOOL bRet = FALSE ;
    
    // Set the CTRL+BREAK handler.
    bRet = SetConsoleCtrlHandler ( CtrlBreakHandler , TRUE ) ;
    if ( FALSE == bRet )
    {
        _tprintf ( _T ( "Unable to set CTRL+BREAK handler!\n" ) ) ;
        return ;
    }
    
    // If the PID is zero, I'm starting the process.
    if ( 0 == dwPID )
    {
        // Try to start the debuggee process.  The function call looks
        // like a normal CreateProcess call except for the special start
        // option flag DEBUG_ONLY_THIS_PROCESS.
        STARTUPINFO         stStartInfo     ;
        PROCESS_INFORMATION stProcessInfo   ;

        memset ( &stStartInfo   , NULL , sizeof ( STARTUPINFO        ));
        memset ( &stProcessInfo , NULL , sizeof ( PROCESS_INFORMATION));

        stStartInfo.cb = sizeof ( STARTUPINFO ) ;

        bRet = CreateProcess ( NULL                      ,
                               szCmdLine                 ,
                               NULL                      ,
                               NULL                      ,
                               FALSE                     ,
                               CREATE_NEW_CONSOLE |
                               DEBUG_ONLY_THIS_PROCESS   ,
                               NULL                      ,
                               NULL                      ,
                               &stStartInfo              ,
                               &stProcessInfo             ) ;
        
        // Don't forget to close process and thread handles returned
        // from CreateOricess.
        VERIFY ( CloseHandle ( stProcessInfo.hProcess ) ) ;
        VERIFY ( CloseHandle ( stProcessInfo.hThread ) ) ;

        // See whether the debuggee process started.
        if ( FALSE == bRet )
        {
            _tprintf ( _T ( "Unable to start %s\n" ) , szCmdLine ) ;
            return ;
        }
        
        // Save the process ID in case there's a detach.
        dwPID = stProcessInfo.dwProcessId ;
    }
    else
    {
        bRet = DebugActiveProcess ( dwPID ) ;
        if ( FALSE == bRet )
        {
            _tprintf ( _T ( "Unable to attach to %u\n" ) , dwPID ) ;
            return ;
        }
    }
    
    // The debuggee started, so let's enter the debug loop.
    DEBUG_EVENT stDE                      ;
    BOOL        bSeenInitialBP   = FALSE  ;
    BOOL        bContinue        = TRUE   ;
    HANDLE      hProcess         = INVALID_HANDLE_VALUE ;
    DWORD       dwContinueStatus          ;

    // Loop until told to stop.
    while ( TRUE == bContinue )
    {
        // Pause until a debug event notification happens.
        BOOL bProcessDbgEvent = WaitForDebugEvent ( &stDE , 100 ) ;
        
        if ( TRUE == bProcessDbgEvent )
        {
            // Handle the particular debug events.  Because MinDBG is
            // only a minimal debugger, it handles only a few events.
            switch ( stDE.dwDebugEventCode )
            {
                case CREATE_PROCESS_DEBUG_EVENT   :
                {
                    DisplayCreateProcessEvent(stDE.u.CreateProcessInfo);
                    // Save the handle information needed for later.
                    // Note that you can't close this handle.  If you
                    // do, CloseHandle fails.
                    hProcess = stDE.u.CreateProcessInfo.hProcess ;

                    // You can safely close the file handle.  If you
                    // close the thread, CloseHandle fails deep in
                    // ContinueDebugEvent when you end the application.
                    VERIFY(CloseHandle(stDE.u.CreateProcessInfo.hFile));

                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;
                case EXIT_PROCESS_DEBUG_EVENT   :
                {
                    DisplayExitProcessEvent ( stDE.u.ExitProcess ) ;
                    bContinue = FALSE ;
                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;

                case LOAD_DLL_DEBUG_EVENT    :
                {
                    DisplayDllLoadEvent ( hProcess , stDE.u.LoadDll ) ;
                    
                    // Don't forget to close the corresponding file
                    // handle.
                    VERIFY ( CloseHandle( stDE.u.LoadDll.hFile ) ) ;
                    
                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;
                case UNLOAD_DLL_DEBUG_EVENT  :
                {
                    DisplayDllUnLoadEvent ( stDE.u.UnloadDll ) ;
                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;

                case CREATE_THREAD_DEBUG_EVENT  :
                {
                    DisplayCreateThreadEvent ( stDE.dwThreadId     ,
                                            stDE.u.CreateThread  ) ;
                    // Note that you can't close the thread handle.  If
                    // you do, CloseHandle fails deep inside
                    // ContinueDebugEvent.

                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;
                case EXIT_THREAD_DEBUG_EVENT    :
                {
                    DisplayExitThreadEvent ( stDE.dwThreadId   ,
                                            stDE.u.ExitThread  ) ;
                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;

                case OUTPUT_DEBUG_STRING_EVENT  :
                {
                    DisplayODSEvent ( hProcess , stDE.u.DebugString ) ;
                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;

                case EXCEPTION_DEBUG_EVENT      :
                {
                    DisplayExceptionEvent ( stDE.u.Exception ) ;

                    // The only exception that I have to treat specially
                    // is the initial breakpoint the loader provides.
                    switch(stDE.u.Exception.ExceptionRecord.ExceptionCode)
                    {
                        case EXCEPTION_BREAKPOINT :
                        {
                            // If a breakpoint exception occurs and it's
                            // the first seen, I continue on my merry
                            // way; otherwise, I pass the exception on
                            // to the debuggee.
                            if ( FALSE == bSeenInitialBP )
                            {
                                bSeenInitialBP = TRUE ;
                                dwContinueStatus = DBG_CONTINUE ;
                            }
                            else
                            {
                                // Houston, we have a problem!
                                dwContinueStatus =
                                             DBG_EXCEPTION_NOT_HANDLED ;
                            }
                        }
                        break ;

                        // Just pass on any other exceptions to the
                        // debuggee.
                        default                         :
                        {
                            dwContinueStatus =
                                             DBG_EXCEPTION_NOT_HANDLED ;
                        }
                        break ;
                    }
                }
                break ;

                // For any other events, just continue on.
                default                         :
                {
                    dwContinueStatus = DBG_CONTINUE ;
                }
                break ;
            }

            // Pass on to the operating system.
    #ifdef _DEBUG
            BOOL bCntDbg =
    #endif
                ContinueDebugEvent ( stDE.dwProcessId ,
                                    stDE.dwThreadId  ,
                                    dwContinueStatus  ) ;
                                     
            ASSERT ( TRUE == bCntDbg ) ;
        }
        // Check to see if the detach is supposed to happen.
        if ( TRUE == g_bDoTheDetach )
        {
            // Detaching only works on XP and higher so I'll have to
            // do the GetProcAddress to look up the
            // DebugActiveProcessStop.
            bContinue = FALSE ;
            
            HINSTANCE hKernel32 =
                             GetModuleHandle ( _T ( "KERNEL32.DLL" ) ) ;
            if ( 0 != hKernel32 )
            {
                PFNDEBUGACTIVEPROCESSSTOP pfnDAPS =
                            (PFNDEBUGACTIVEPROCESSSTOP)
                          GetProcAddress ( hKernel32                ,
                                           "DebugActiveProcessStop"  ) ;
                if ( NULL != pfnDAPS )
                {
#ifdef _DEBUG
                    BOOL bTemp =
#endif
                    pfnDAPS ( dwPID ) ;
            
                    ASSERT ( TRUE == bTemp ) ;
                }
            }
        }
    }
}

/*//////////////////////////////////////////////////////////////////////
// Monitors CTRL+BREAK processing.
//////////////////////////////////////////////////////////////////////*/
BOOL WINAPI CtrlBreakHandler ( DWORD dwCtrlType )
{
    // I'll only handle CTRL+BREAK.  Anything else kills the debuggee.
    if ( CTRL_BREAK_EVENT == dwCtrlType )
    {
        g_bDoTheDetach = TRUE ;
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Display's program help.
//////////////////////////////////////////////////////////////////////*/
void ShowHelp ( void )
{
    _tprintf ( _T ( "Start a program to debug:\n" )
               _T ( "   MinDBG <program to debug> " )
               _T ( "<program's command-line options>\n" )
               _T ( "Attach to an existing program:\n" )
               _T ( "   MinDBG -PID\n" )
               _T ( "       PID is the decimal process ID\n" ) ) ;
}


/*//////////////////////////////////////////////////////////////////////
// Display's create process events.
//////////////////////////////////////////////////////////////////////*/
void DisplayCreateProcessEvent ( CREATE_PROCESS_DEBUG_INFO & stCPDI )
{
    _tprintf ( _T ( "Create Process Event      :\n" ) ) ;
    _tprintf ( _T ( "   hFile                  : 0x%08X\n" ) ,
               stCPDI.hFile                                   ) ;
    _tprintf ( _T ( "   hProcess               : 0x%08X\n" ) ,
               stCPDI.hProcess                                ) ;
    _tprintf ( _T ( "   hThread                : 0x%08X\n" ) ,
               stCPDI.hThread                                 ) ;
    _tprintf ( _T ( "   lpBaseOfImage          : 0x%08X\n" ) ,
               stCPDI.lpBaseOfImage                           ) ;
    _tprintf ( _T ( "   dwDebugInfoFileOffset  : 0x%08X\n" ) ,
               stCPDI.dwDebugInfoFileOffset                   ) ;
    _tprintf ( _T ( "   nDebugInfoSize         : 0x%08X\n" ) ,
               stCPDI.nDebugInfoSize                          ) ;
    _tprintf ( _T ( "   lpThreadLocalBase      : 0x%08X\n" ) ,
               stCPDI.lpThreadLocalBase                       ) ;
    _tprintf ( _T ( "   lpStartAddress         : 0x%08X\n" ) ,
               stCPDI.lpStartAddress                          ) ;
    _tprintf ( _T ( "   lpImageName            : 0x%08X\n" ) ,
               stCPDI.lpImageName                             ) ;
    _tprintf ( _T ( "   fUnicode               : 0x%08X\n" ) ,
               stCPDI.fUnicode                                ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Display's create thread events.
//////////////////////////////////////////////////////////////////////*/
void DisplayCreateThreadEvent ( DWORD                      dwTID  ,
                                CREATE_THREAD_DEBUG_INFO & stCTDI  )
{
    _tprintf ( _T ( "Create Thread Event       :\n" ) ) ;
    _tprintf ( _T ( "   TID                    : 0x%08X\n" ) ,
               dwTID                                          ) ;
    _tprintf ( _T ( "   hThread                : 0x%08X\n" ) ,
               stCTDI.hThread                                 ) ;
    _tprintf ( _T ( "   lpThreadLocalBase      : 0x%08X\n" ) ,
             stCTDI.lpThreadLocalBase                         ) ;
    _tprintf ( _T ( "   lpStartAddress         : 0x%08X\n" ) ,
               stCTDI.lpStartAddress                          ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Display's exit thread events.
//////////////////////////////////////////////////////////////////////*/
void DisplayExitThreadEvent ( DWORD                    dwTID  ,
                              EXIT_THREAD_DEBUG_INFO & stETDI  )
{
    _tprintf ( _T ( "Exit Thread Event         :\n" ) ) ;
    _tprintf ( _T ( "   TID                    : 0x%08X\n" ) ,
               dwTID                                          ) ;
    _tprintf ( _T ( "   dwExitCode             : 0x%08X\n" ) ,
               stETDI.dwExitCode                              ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Display's exit process events.
//////////////////////////////////////////////////////////////////////*/
void DisplayExitProcessEvent ( EXIT_PROCESS_DEBUG_INFO & stEPDI )
{
    _tprintf ( _T ( "Exit Process Event        :\n" ) ) ;
    _tprintf ( _T ( "   dwExitCode             : 0x%08X\n" ) ,
               stEPDI.dwExitCode                              ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Display's DLL load events.
//////////////////////////////////////////////////////////////////////*/
void DisplayDllLoadEvent ( HANDLE                hProcess ,
                           LOAD_DLL_DEBUG_INFO & stLDDI    )
{
    _tprintf ( _T ( "DLL Load Event            :\n" ) ) ;
    _tprintf ( _T ( "   hFile                  : 0x%08X\n" ) ,
               stLDDI.hFile                                   ) ;
    _tprintf ( _T ( "   lpBaseOfDll            : 0x%08X\n" ) ,
               stLDDI.lpBaseOfDll                             ) ;
    _tprintf ( _T ( "   dwDebugInfoFileOffset  : 0x%08X\n" ) ,
               stLDDI.dwDebugInfoFileOffset                   ) ;
    _tprintf ( _T ( "   nDebugInfoSize         : 0x%08X\n" ) ,
               stLDDI.nDebugInfoSize                          ) ;
    _tprintf ( _T ( "   lpImageName            : 0x%08X\n" ) ,
               stLDDI.lpImageName                             ) ;
    _tprintf ( _T ( "   fUnicode               : 0x%08X\n" ) ,
               stLDDI.fUnicode                                ) ;
             
    static bool bSeenNTDLL = false ;
    TCHAR szDLLName[ MAX_PATH ] ;
    
    // NTDLL.DLL is special.  On W2K, the lpImageName is NULL and
    // on XP, the name just points to 'ntdll.dll' so I will fake the
    // load information.
    if ( false == bSeenNTDLL )
    {
        bSeenNTDLL = true ;
        UINT uiLen = GetWindowsDirectory ( szDLLName , MAX_PATH ) ;
        ASSERT ( uiLen > 0 ) ;
        if ( uiLen > 0 )
        {
            _tcscpy ( szDLLName + uiLen , _T ( "\\NTDLL.DLL" ) ) ;
        }
        else
        {
            _tcscpy ( szDLLName , _T ( "GetWindowsDirectory FAILED!" ));
        }
    }
    else
    {
        szDLLName[ 0 ] = _T ( '\0' ) ;
        
        // The value in lpImageName is a pointer to the full path
        // and name of the DLL being loaded.  The address is in the
        // debuggee address space.
        LPCVOID lpPtr = 0 ;
        DWORD dwBytesRead = 0 ;
        BOOL bRet = FALSE ;

        bRet = ReadProcessMemory ( hProcess           ,
                                   stLDDI.lpImageName ,
                                   &lpPtr             ,
                                   sizeof ( LPCVOID ) ,
                                   &dwBytesRead        ) ;
        if ( TRUE == bRet )
        {
            // If the name in the debuggee is UNICODE, I can read it
            // directly into the szDLLName variable as all this code
            // is UNICODE.
            if ( TRUE == stLDDI.fUnicode )
            {
                // Occasionally, you can't read the whole buffer that
                // contains the name so I need to step down until
                // I can be sure there's no name at all.
                DWORD dwSize = MAX_PATH  * sizeof ( TCHAR ) ;
                do
                {

                    bRet = ReadProcessMemory ( hProcess         ,
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
                DWORD dwAnsiSize = MAX_PATH ;
                
                do
                {
                    bRet = ReadProcessMemory ( hProcess         ,
                                               lpPtr            ,
                                               szAnsiName       ,
                                               dwAnsiSize       ,
                                               &dwBytesRead      ) ;
                    dwAnsiSize = dwAnsiSize - 20 ;
                } while ( ( FALSE == bRet ) && ( dwAnsiSize > 20 ) ) ;
                if ( TRUE == bRet )
                {
                    MultiByteToWideChar ( CP_THREAD_ACP     ,
                                          0                 ,
                                          szAnsiName        ,
                                          -1                ,
                                          szDLLName         ,
                                          MAX_PATH           ) ;
                }
            }
        }
    }
    if ( _T ( '\0' ) == szDLLName[ 0 ] )
    {
        // This DLL has some issues.  Try to read it with
        // GetModuleHandleEx.  While you'd think this would work for all
        // it only seems to work if the module can't be retrieved by
        // the above means.  If you can't retrieve the DLL name with the
        // code above, you're actually looking at a rebased DLL.
        DWORD dwRet = GetModuleFileNameEx ( hProcess                 ,
                                            (HMODULE)stLDDI.
                                                        lpBaseOfDll  ,
                                            szDLLName                ,
                                            MAX_PATH                  );
        ASSERT ( dwRet > 0 ) ;
        if ( 0 == dwRet )
        {
            szDLLName[ 0 ] = _T ( '\0' ) ;
        }
    }
    
    if ( _T ( '\0' ) != szDLLName[ 0 ] )
    {
        _tcsupr ( szDLLName ) ;
        _tprintf ( _T ( "   DLL name               : %s\n" ) ,
                   szDLLName                                  ) ;
    }
    else
    {
        _tprintf ( _T ( "UNABLE TO READ DLL NAME!!\n" ) ) ;
    }
}

/*//////////////////////////////////////////////////////////////////////
// Display's DLL unload events.
//////////////////////////////////////////////////////////////////////*/
void DisplayDllUnLoadEvent ( UNLOAD_DLL_DEBUG_INFO & stULDDI )
{
    _tprintf ( _T ( "DLL Unload Event          :\n" ) ) ;
    _tprintf ( _T ( "   lpBaseOfDll            : 0x%08X\n" ) ,
               stULDDI.lpBaseOfDll                            ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Display's OutputDebugString events.
//////////////////////////////////////////////////////////////////////*/
void DisplayODSEvent ( HANDLE                     hProcess ,
                       OUTPUT_DEBUG_STRING_INFO & stODSI    )
{
    _tprintf ( _T ( "OutputDebugString Event   :\n" ) ) ;
    _tprintf ( _T ( "   lpDebugStringData      : 0x%08X\n" ) ,
               stODSI.lpDebugStringData                       ) ;
    _tprintf ( _T ( "   fUnicode               : 0x%08X\n" ) ,
               stODSI.fUnicode                                ) ;
    _tprintf ( _T ( "   nDebugStringLength     : %d\n"     ) ,
               stODSI.nDebugStringLength                      ) ;
    _tprintf ( _T ( "   String                 : " ) ) ;

    TCHAR szFinalBuff[ 512 ] ;
    if ( stODSI.nDebugStringLength > 512 )
    {
        _tprintf ( _T ( "String to large!!\n" ) ) ;
        return ;
    }
    
    DWORD dwRead ;
    BOOL bRet ;
    
    // Interestingly enough, all OutputDebugString calls, no matter if
    // the application is full UNICODE or not always come in as ANSI
    // strings.
    if ( false == stODSI.fUnicode )
    {
        // Read the ANSI string.
        char szAnsiBuff[ 512 ] ;
        bRet = ReadProcessMemory ( hProcess                  ,
                                   stODSI.lpDebugStringData  ,
                                   szAnsiBuff                ,
                                   stODSI.nDebugStringLength ,
                                   &dwRead                    ) ;
        if ( TRUE == bRet )
        {
            MultiByteToWideChar ( CP_THREAD_ACP ,
                                  0             ,
                                  szAnsiBuff    ,
                                  -1            ,
                                  szFinalBuff   ,
                                  512            ) ;
        }
        else
        {
            szFinalBuff[ 0 ] = _T ( '\0' ) ;
        }
    }
    else
    {
        // Read the UNICODE string.
        bRet = ReadProcessMemory ( hProcess                     ,
                                   stODSI.lpDebugStringData     ,
                                   szFinalBuff                  ,
                                   stODSI.nDebugStringLength *
                                            sizeof ( TCHAR )    ,
                                   &dwRead                       ) ;
        if ( FALSE == bRet )
        {
            szFinalBuff[ 0 ] = _T ( '\0' ) ;
        }
    }

    if ( _T ( '\0' ) != szFinalBuff[ 0 ] )
    {
        _tprintf ( _T ( "%s\n" ) , szFinalBuff ) ;
    }
    else
    {
        _tprintf ( _T ( "UNABLE TO READ ODS STRING!!\n" ) ) ;
    }
}

/*//////////////////////////////////////////////////////////////////////
// Display's exception events.
//////////////////////////////////////////////////////////////////////*/
void DisplayExceptionEvent ( EXCEPTION_DEBUG_INFO & stEDI )
{
    _tprintf ( _T ( "Exception Event           :\n" ) ) ;
    _tprintf ( _T ( "   dwFirstChance          : 0x%08X\n" ) ,
               stEDI.dwFirstChance                            ) ;
    _tprintf ( _T ( "   ExceptionCode          : 0x%08X\n" ) ,
               stEDI.ExceptionRecord.ExceptionCode            ) ;
    _tprintf ( _T ( "   ExceptionFlags         : 0x%08X\n" ) ,
               stEDI.ExceptionRecord.ExceptionFlags           ) ;
    _tprintf ( _T ( "   ExceptionRecord        : 0x%08X\n" ) ,
               stEDI.ExceptionRecord.ExceptionRecord          ) ;
    _tprintf ( _T ( "   ExceptionAddress       : 0x%08X\n" ) ,
               stEDI.ExceptionRecord.ExceptionAddress         ) ;
    _tprintf ( _T ( "   NumberParameters       : 0x%08X\n" ) ,
               stEDI.ExceptionRecord.NumberParameters         ) ;
}