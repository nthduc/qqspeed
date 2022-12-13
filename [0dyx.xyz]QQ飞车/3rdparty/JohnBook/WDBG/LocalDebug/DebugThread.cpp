/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "DebugInterface.h"
#include "Dll.h"
#include "Resource.h"
#include "DebuggeeInfo.h"
#include "ProcessDebugEvents.h"
#include "CreateDebugSynchEvents.h"

/*//////////////////////////////////////////////////////////////////////
                     Typedefs, Constants, & Defines
//////////////////////////////////////////////////////////////////////*/
// The package of parameters passed to the debugging thread.
typedef struct tag_THREADPARAMS
{
    LPCTSTR          szDebuggee ;
    LPCTSTR          szCmdLine  ;
    LPDWORD          lpPID      ;
    CDebugBaseUser * pUserClass ;
} THREADPARAMS , * PTHREADPARAMS ;

/*//////////////////////////////////////////////////////////////////////
                     File Scope Function Prototypes
//////////////////////////////////////////////////////////////////////*/
// The debug thread itself.
static UINT __stdcall DebugThread ( LPVOID lpVoid ) ;

/*//////////////////////////////////////////////////////////////////////
                     !!IMPLEMENTATION BEGINS HERE!!
//////////////////////////////////////////////////////////////////////*/
HANDLE DEBUGINTERFACE_DLLINTERFACE __stdcall
    StartDebugging ( LPCTSTR          szDebuggee        ,
                     LPCTSTR          szCmdLine         ,
                     LPDWORD          lpPID             ,
                     CDebugBaseUser * pUserClass        ,
                     LPHANDLE         lpDebugSyncEvents  )
{
    // Assert the parameters.
    ASSERT ( FALSE == IsBadStringPtr ( szDebuggee , MAX_PATH ) ) ;
    ASSERT ( FALSE == IsBadStringPtr ( szCmdLine , MAX_PATH ) ) ;
    ASSERT ( FALSE == IsBadWritePtr ( lpPID , sizeof ( DWORD ) ) ) ;
    ASSERT ( FALSE == IsBadReadPtr ( pUserClass ,
                                     sizeof ( CDebugBaseUser * ) ) ) ;
    ASSERT ( FALSE == IsBadWritePtr ( lpDebugSyncEvents ,
                                      sizeof ( HANDLE ) *
                                            NUM_DEBUGEVENTS ) ) ;
    // Check them all for real.
    if ( ( TRUE == IsBadStringPtr ( szDebuggee , MAX_PATH )     )  ||
         ( TRUE == IsBadStringPtr ( szCmdLine , MAX_PATH )      )  ||
         ( TRUE == IsBadWritePtr ( lpPID , sizeof ( DWORD )   ) )  ||
         ( TRUE == IsBadReadPtr ( pUserClass ,
                                  sizeof ( CDebugBaseUser * ) ) )  ||
         ( TRUE == IsBadWritePtr ( lpDebugSyncEvents ,
                                   sizeof ( HANDLE ) *
                                        NUM_DEBUGEVENTS )     )       )
    {
        SetLastError ( ERROR_INVALID_PARAMETER ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // The string used for the startup acknowledgment event
    TCHAR szStartAck [ MAX_PATH ] = _T ( "\0" ) ;

    // Load up the string for startup acknowledgment.
    if ( 0 == LoadString ( GetDllHandle ( )      ,
                           IDS_DBGEVENTINIT      ,
                           szStartAck            ,
                           MAX_PATH               ) )
    {
        ASSERT ( !"LoadString IDS_DBGEVENTINIT failed!" ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // The handle of the startup acknowledgment that this function
    // will wait on until the debug thread gets started
    HANDLE hStartAck = NULL ;
    
     // Create the startup acknowledgment event.
    hStartAck = CreateEvent ( NULL   ,     // Default security
                              TRUE   ,     // Manual-reset event
                              FALSE  ,     // Initial state=Not signaled
                              szStartAck ) ; // Event name
    ASSERT ( NULL != hStartAck ) ;
    if ( NULL == hStartAck )
    {
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // Bundle up the parameters.
    THREADPARAMS stParams ;
    stParams.lpPID = lpPID ;
    stParams.pUserClass = pUserClass ;
    stParams.szDebuggee = szDebuggee ;
    stParams.szCmdLine  = szCmdLine  ;

    // The handle to the debug thread
    HANDLE hDbgThread = INVALID_HANDLE_VALUE ;

    // Try to create the thread.
    UINT dwTID = 0 ;
    hDbgThread = (HANDLE)_beginthreadex ( NULL        ,
                                          0           ,
                                          DebugThread ,
                                          &stParams   ,
                                          0           ,
                                          &dwTID       ) ;
    ASSERT ( INVALID_HANDLE_VALUE != hDbgThread ) ;
    if (INVALID_HANDLE_VALUE == hDbgThread )
    {
        VERIFY ( CloseHandle ( hStartAck ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // Wait until the debug thread gets good and cranking.
    DWORD dwRet = ::WaitForSingleObject ( hStartAck , INFINITE ) ;
    ASSERT (WAIT_OBJECT_0 == dwRet ) ;
    if (WAIT_OBJECT_0 != dwRet )
    {
        VERIFY ( CloseHandle ( hStartAck ) ) ;
        VERIFY ( CloseHandle ( hDbgThread ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // Get rid of the acknowledgment handle.
    VERIFY ( CloseHandle ( hStartAck ) ) ;

    // Check that the debug thread is still running. If it isn’t,
    // the debuggee probably couldn’t get started.
    DWORD dwExitCode = ~STILL_ACTIVE ;
    if ( FALSE == GetExitCodeThread ( hDbgThread , &dwExitCode ) )
    {
        ASSERT ( !"GetExitCodeThread failed!" ) ;
        VERIFY ( CloseHandle ( hDbgThread ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }
     ASSERT ( STILL_ACTIVE == dwExitCode ) ;
    if ( STILL_ACTIVE != dwExitCode )
    {
        VERIFY ( CloseHandle ( hDbgThread ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }

    // Create the synchronization events so that the main thread can
    // tell the debug loop what to do.
    BOOL bCreateDbgSyncEvts =
                  CreateDebugSyncEvents ( lpDebugSyncEvents , *lpPID ) ;
    ASSERT ( TRUE == bCreateDbgSyncEvts ) ;
    if ( FALSE == bCreateDbgSyncEvts )
    {
        // This is a serious problem. I got the debug thread going, but
        // I was unable to create the synchronization events that the
        // user interface thread needs to control the debug thread. My
        // only option here is to punt. I’ll kill the
        // debug thread and just return. I can’t do much else.
        TRACE ( "StartDebugging : CreateDebugSyncEvents failed\n" ) ;
        VERIFY ( TerminateThread ( hDbgThread , (DWORD)-1 ) ) ;
        VERIFY ( CloseHandle ( hDbgThread ) ) ;
        return ( INVALID_HANDLE_VALUE ) ;
    }
    
    // Just in case someone modifies the function and fails to properly
    // initialize the returned value.
    ASSERT ( INVALID_HANDLE_VALUE != hDbgThread ) ;
    
    // Life is good!
    return ( hDbgThread ) ;
}

BOOL DEBUGINTERFACE_DLLINTERFACE __stdcall
    StopDebugging ( LPHANDLE lpDebugSyncEvents )
{
    ASSERT ( FALSE ==
                IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) );
    if ( TRUE == IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) )
    {
        SetLastError ( ERROR_INVALID_PARAMETER ) ;
        return ( FALSE ) ;
    }

    // Signal the debug thread with the event name to close.
    VERIFY ( SetEvent ( lpDebugSyncEvents[ CLOSEDEBUGGER ] ) ) ;
    return ( TRUE ) ;
}

BOOL DEBUGINTERFACE_DLLINTERFACE __stdcall
    SuspendDebugging ( LPHANDLE lpDebugSyncEvents )
{
    ASSERT ( FALSE ==
                IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) );
    if ( TRUE == IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) )
    {
        SetLastError ( ERROR_INVALID_PARAMETER ) ;
        return ( FALSE ) ;
    }
    BOOL bSE = SetEvent ( lpDebugSyncEvents [ SUSPENDDEBUGGER ] ) ;
    ASSERT ( TRUE == bSE ) ;
    return ( bSE ) ;
}

BOOL DEBUGINTERFACE_DLLINTERFACE __stdcall
    ResumeDebugging ( LPHANDLE lpDebugSyncEvents )
{
    ASSERT ( FALSE ==
                IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) );
    if ( TRUE == IsBadWritePtr ( lpDebugSyncEvents ,
                                sizeof ( HANDLE ) * NUM_DEBUGEVENTS ) )
    {
        SetLastError ( ERROR_INVALID_PARAMETER ) ;
        return ( FALSE ) ;
    }
    BOOL bSE = SetEvent ( lpDebugSyncEvents [ RESUMEDEBUGGER ] ) ;
    ASSERT ( TRUE == bSE ) ;
    return ( bSE ) ;
}


// The happy debug thread itself!
static UINT __stdcall DebugThread ( LPVOID lpVoid )
{
#ifdef _DEBUG
    BSUSetCurrentThreadName ( _T ( "WDBG Debug Thread" ) ) ;
#endif

    // Take care of the parameters. No need to check as this function
    // will not get called unless everything lines up.
    PTHREADPARAMS pParams = (PTHREADPARAMS)lpVoid ;

    // The main thread notification functions.
    CDebugBaseUser * pUserClass = pParams->pUserClass ;

    // A string buffer...
    TCHAR szBuff [ MAX_PATH ] ;
    // An event handle..
    HANDLE hEvent  ;
    // All the synchronization events for this thread.
    HANDLE hDbgEvents[ NUM_DEBUGEVENTS ] ;
    // The debuggee name.
    TCHAR  szName [ MAX_PATH ];

    // Get the sync event string.
    if ( FALSE == LoadString ( GetDllHandle ( )  ,
                               IDS_DBGEVENTINIT  ,
                               szBuff            ,
                               sizeof ( szBuff )   ) )
    {
        ASSERT ( !"LoadString IDS_DBGEVENTINIT failed!" ) ;
        return ( 0 ) ;
    }


    // Prepare to startup the debuggee.
    STARTUPINFO         si ;
    PROCESS_INFORMATION pi ;

    memset ( &si , NULL , sizeof ( STARTUPINFO ) ) ;
    memset ( &pi , NULL , sizeof ( PROCESS_INFORMATION ) ) ;

    // When starting, always show the app start cursor and show the
    // window normally.
    si.cb = sizeof ( STARTUPINFO ) ;
    si.dwFlags = STARTF_FORCEONFEEDBACK | STARTF_USESHOWWINDOW ;
    si.wShowWindow = SW_SHOWNORMAL ;
    
    // Package up the program and it's parameters so CreateProcess is
    // happy.
    TCHAR * szCmdLine = new TCHAR [ MAX_PATH * 2 ] ;
    
    wsprintf ( szCmdLine            ,
               _T ( "%s %s" )       ,
               pParams->szDebuggee  ,
               pParams->szCmdLine    ) ;
               
    // Crank up the debuggee!
    // DEBUG_ONLY_THIS_PROCESS is the magic flag that tells the OS
    // that this is going to be a debugger.  Also set CREATE_NEW_CONSOLE
    // so that any console applications get their own console window.
    BOOL bRetCreateProc =
        CreateProcess ( (LPTSTR)pParams->szDebuggee ,
                        (LPTSTR)szCmdLine           ,
                        NULL                        ,
                        NULL                        ,
                        FALSE                       ,
                        DEBUG_ONLY_THIS_PROCESS |
                            CREATE_NEW_CONSOLE      ,
                        NULL                        ,
                        NULL                        ,
                        &si                         ,
                        &pi                          ) ;
    ASSERT ( TRUE == bRetCreateProc ) ;
    
    // I no longer need the memory.
    delete [] szCmdLine ;

    // Set the process ID here as the main thread might get a boost
    // and look at it a little early.
    *pParams->lpPID = pi.dwProcessId ;

    // Copy the debuggee name as well.
    _tcscpy ( szName , pParams->szDebuggee ) ;

    // No matter if the debuggee was started or not, I still need to
    // clear out the initialization event.  The main thread is blocked
    // waiting on this.
    hEvent = OpenEvent ( EVENT_ALL_ACCESS ,
                         FALSE            ,
                         szBuff            ) ;
    ASSERT ( NULL != hEvent ) ;
    VERIFY ( SetEvent ( hEvent ) ) ;
    VERIFY ( CloseHandle ( hEvent ) ) ;

    // If the CreateProcess failed, leave as there is nothing to do.
    if ( FALSE == bRetCreateProc )
    {
        return ( 0 ) ;
    }

    //Close the thread handle from CreateProcess.
    VERIFY ( CloseHandle ( pi.hThread  ) ) ;

    // Create all the debug syncronization events for this thread.
    BOOL bCreateDbgEvts =
        CreateDebugSyncEvents ( hDbgEvents ,  pi.dwProcessId ) ;
    ASSERT ( TRUE == bCreateDbgEvts ) ;
    if ( TRUE != bCreateDbgEvts )
    {
        return ( 0 ) ;
    }

    // Reinitialize the debuggee info stuff.
    ReinitializeDebuggeeInfo ( ) ;

    // The actual debug events.
    DEBUG_EVENT de ;
    // Flag that tells us if the debuggee still running or not.
    BOOL bDebuggeeRunning = TRUE ;
    // Holds the index of the signaled event.
    int iIndex ;

    // Loop until the debuggee stops or I am told to stop.
    while ( TRUE == bDebuggeeRunning )
    {
        // Wait on my synchronization events.  Note that ACTIVEDEBUGGER
        // is always signaled so this function does not block in the
        // normal case.
        iIndex = WaitForMultipleObjects ( NUM_DEBUGEVENTS ,
                                          hDbgEvents      ,
                                          FALSE           ,
                                          INFINITE         ) ;
        switch ( iIndex )
        {
            // The highest priority item.
            case CLOSEDEBUGGER   :
                {
                    // The debuggee is still running, so shut it down.
                    VERIFY ( TerminateProcess ( pi.hProcess , 0 ) ) ;
                    VERIFY ( CloseHandle ( pi.hProcess ) ) ;
                    // This thread is finished.
                    bDebuggeeRunning = FALSE ;
                }
                break ;
            case SUSPENDDEBUGGER    :
                {
                    // Make the active debugger event non-signaled so I
                    // don't pop back in there.
                    VERIFY ( ResetEvent ( hDbgEvents [ACTIVEDEBUGGER]));
                    // Put the suspend event back to it's normal state.
                    // This causes the WaitForMultipleObjects at the
                    // top there to just block until resume is signaled.
                    VERIFY ( ResetEvent ( hDbgEvents[SUSPENDDEBUGGER]));
                }
                break ;
            case RESUMEDEBUGGER     :
                {
                    // Set the active debugger event to signaled, its
                    // normal state.
                    VERIFY ( SetEvent ( hDbgEvents[ACTIVEDEBUGGER] ) ) ;
                    // Turn off the reset event, its normal state.
                    VERIFY ( ResetEvent ( hDbgEvents[RESUMEDEBUGGER])) ;
                }
                break ;
            case ACTIVEDEBUGGER     :
                {
                    // Just a reminder.  Unless suspended, the active
                    // debugger state is always signaled so this part
                    // is always running.

                    // Wait a bit for a debugger event.  If none comes
                    // in, then I pop back to the WaitForMultipleObjects
                    // at the top of the loop.
                    if ( WaitForDebugEvent ( &de , (DWORD)100 ) )
                    {
                        // The debuggee info for this process.
                        LPDEBUGGEEINFO pData ;

                        // The create process is the first event ever
                        // seen so create a debuggee info class for it.
                        if ( CREATE_PROCESS_DEBUG_EVENT ==
                                                   de.dwDebugEventCode )
                        {
                            pData =
                                CreateDebuggeeInfo ( pUserClass ,
                                                     szName      ) ;
                        }
                        else
                        {
                            // The debuggee info has already been
                            // created.
                            pData = GetDebuggeeInfo ( de.dwProcessId ) ;
                        }

                        ASSERT ( NULL != pData ) ;

                        if ( WAIT_OBJECT_0 ==
                                WaitForSingleObject(
                                           hDbgEvents[CLOSEDEBUGGER],0))
                        {
                            continue ;
                        }

                        // Do the work to make everything fun.
                        DWORD dwContinueStatus =
                            ProcessDebugEvents ( pUserClass ,
                                                 pData      ,
                                                 &de         ) ;

                        // Do a quicky check of the close event.  If it
                        // has been signaled, I cannot pump load
                        // notifications because it will deadlock.
                        if ( WAIT_OBJECT_0 ==
                                WaitForSingleObject(
                                           hDbgEvents[CLOSEDEBUGGER],0))
                        {
                            continue ;
                        }

                        // If the processing returned zero something
                        // seriously bad has happened.  I need to
                        // manually signal the close event and stop the
                        // debug thread.
                        if ( 0 == dwContinueStatus )
                        {
                            VERIFY(SetEvent(hDbgEvents[CLOSEDEBUGGER]));
                            continue ;
                        }

                        // Let the thread continue.
                        VERIFY ( ContinueDebugEvent ( de.dwProcessId   ,
                                                      de.dwThreadId    ,
                                                     dwContinueStatus));

                        // If this event is an EXIT_PROCESS, then there
                        // is a little bit of stuff to do to keep this
                        // thread straight.
                        if ( EXIT_PROCESS_DEBUG_EVENT ==
                                                   de.dwDebugEventCode )
                        {
                            // Clear the active debugger event.
                            VERIFY ( ResetEvent (
                                           hDbgEvents[ACTIVEDEBUGGER]));

                            // Drop out of the thread.
                            bDebuggeeRunning = FALSE ;
                        }
                    }
                }
                break ;
            default :
                ASSERT ( !"Invalid wait index!" ) ;
                break ;
        }

    }

    // Close all of the debugging handles that belong to
    // this thread.
    for ( int i = 0 ; i < NUM_DEBUGEVENTS ; i++ )
    {
        VERIFY ( CloseHandle ( hDbgEvents[ i ] ) ) ;
    }

    // Get rid of the debuggee info for the process this thread is
    // debugging.
    LPDEBUGGEEINFO pData = GetDebuggeeInfo ( pi.dwProcessId ) ;
    ASSERT ( NULL != pData ) ;
    VERIFY ( DestroyDebuggeeInfo ( pData ) ) ;

    // Have a good day!
    TRACE ( "Debug Thread 0x%08X ending!\n" , GetCurrentThreadId ( ) );
    return ( 1 ) ;
}
