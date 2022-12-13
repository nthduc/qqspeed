/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
/*----------------------------------------------------------------------
DebugInterface.h
The debugger DLL interface.
----------------------------------------------------------------------*/

#ifndef _DEBUGINTERFACE_H
#define _DEBUGINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// The defines that set up how the functions or classes are exported or
// imported.
#ifndef DEBUGINTERFACE_DLLINTERFACE
#ifdef BUILDING_DEBUGINTERFACE_DLL
#define DEBUGINTERFACE_DLLINTERFACE __declspec ( dllexport )
#else
#define DEBUGINTERFACE_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_DEBUGINTERFACE_DLL
#endif  // DEBUGINTERFACE_DLLINTERFACE

/*----------------------------------------------------------------------
FUNCTION    :   SymToString
DISCUSSION  :
    Returns the appropriate string name for a SYM_TYPE returned by the
DBGHELP.DLL symbol engine.
PARAMETERS  :
    eType - The type to convert.
RETURNS     :
    The string represeting eType
----------------------------------------------------------------------*/
LPCTSTR DEBUGINTERFACE_DLLINTERFACE __stdcall
    SymToString ( SYM_TYPE eType ) ;

/*//////////////////////////////////////////////////////////////////////
The CDebugBaseUser class.

For the sake of keeping this sample relatively simple, I am using a C++
base class that the main thread creates and passes to the initialization
function.  In a real world debugger, this would be a COM interface.  The
idea with this sample is to give you an idea as to what happens in a
non-trivial debugger.  Going with full-blown COM stuff would have been
more involved, left much more room for error, and spewed all over your
registry.

//////////////////////////////////////////////////////////////////////*/
// Turn off the "'xxx' : unreferenced formal parameter" warning
#pragma warning ( disable : 4100 )
class CDebugBaseUser
{
public  :
// Constructors and destructors.
    CDebugBaseUser ( void ) {} ;
    virtual ~CDebugBaseUser ( void ) {} ;

// The individual functions you must override.

    // A general textual output function.
    virtual void Output ( LPCTSTR szFmt , ... ) = 0 ;

    // This function is called when the debuggee ends of its
    // own accord.  In other words, the debuggee exited.
    virtual void ExitProcessEvent ( DWORD dwProcessId ,
                                    DWORD dwThreadId  ,
                                    DWORD dwExitCode   ) = 0 ;

    // The exception handler.
    // Normally, this function returns either DBG_CONTINUE or
    // DBG_EXCEPTION_NOT_HANDLED.  However, if this function return
    // 0, it will force the debug loop to terminate.  Returning 0
    // should only happen when there is a serious error when processing
    // the exception.
    virtual
    DWORD ExceptionEvent ( DWORD                  dwProcessId ,
                           DWORD                  dwThreadId  ,
                           EXCEPTION_DEBUG_INFO & stEDI        ) = 0 ;


    ////////////////////////////////////////////////////////////////////
    // The following do not have to be overridden.  If your debugger
    // wants to see one of the events, it can do so.
    ////////////////////////////////////////////////////////////////////
    virtual void OutputCRLF ( void )
    {
        Output ( _T ( "\n" ) ) ;
    }

    // Returns an string allocated with new!!
    virtual LPCTSTR ConvertCRLF ( LPTSTR /*szBuff*/ , int /*iBuffLen*/ )
    {
        return ( NULL ) ;
    }

    // Handler for CREATE_THREAD_DEBUG_EVENTS.
    virtual
    DWORD CreateThreadEvent ( DWORD                      dwProcessId ,
                              DWORD                      dwThreadId  ,
                              CREATE_THREAD_DEBUG_INFO & stCTDI       )
    {
        Output ( _T ( "Thread 0x%08X created" ) , dwThreadId ) ;
        OutputCRLF ( ) ;
        return ( DBG_CONTINUE ) ;
    }

    // Handler for CREATE_PROCESS_DEBUG_EVENTs.  Called right when the
    // event happens.  The module is already loaded into the symbol
    // engine by the time this one is called.
    virtual
    DWORD CreateProcessEvent ( DWORD                       dwProcessId ,
                               DWORD                       dwThreadId  ,
                               CREATE_PROCESS_DEBUG_INFO & stCPDI      )
    {
        return ( DBG_CONTINUE ) ;
    }

    // Handler for EXIT_THREAD_DEBUG_EVENTs
    virtual
    DWORD ExitThreadEvent ( DWORD            dwProcessId ,
                            DWORD            dwThreadId  ,
                            DWORD            dwExitCode   )
    {
        Output ( _T ( "Thread 0x%08X exited and returned %d" ) ,
                 dwThreadId                                    ,
                 dwExitCode                                     ) ;
        OutputCRLF ( ) ;

        return ( DBG_CONTINUE ) ;
    }

    // Handler for LOAD_DLL_DEBUG_EVENTs.  This is called on the initial
    // module load.  Before this is called, the module's symbols are
    // loaded into the symbol engine.  This is also called for the EXE
    // so all loaded modules are handled the same way.
    virtual
    DWORD LoadDllEvent ( DWORD                 dwProcessId ,
                         DWORD                 dwThreadId  ,
                         LOAD_DLL_DEBUG_INFO & stLDDI      ,
                         LPCTSTR               szName      ,
                         IMAGEHLP_MODULE64 &   ModInfo      )
    {
        Output ( _T ( "'%s' loaded at 0x%08X with %s symbols" ) ,
                 szName                                         ,
                 stLDDI.lpBaseOfDll                             ,
                 SymToString ( ModInfo.SymType )                 ) ;
        OutputCRLF ( ) ;
        return ( DBG_CONTINUE ) ;
    }

    // Handler for UNLOAD_DLL_DEBUG_EVENTs
    virtual
    DWORD UnloadDllEvent ( DWORD            dwProcessId ,
                           DWORD            dwThreadId  ,
                           LPCVOID          lpBaseOfDll  )
    {
        Output ( _T ( "DLL Unloaded from 0x%08X" ) , lpBaseOfDll ) ;
        OutputCRLF ( ) ;
        return ( DBG_CONTINUE ) ;
    }

    // Handler for OUTPUT_DEBUG_STRING_EVENTs
    virtual
    DWORD OutputDebugStringEvent ( DWORD   dwProcessId ,
                                   DWORD   dwThreadId  ,
                                   LPCTSTR szString     )
    {
        Output ( szString ) ;
        return ( DBG_CONTINUE ) ;
    }

    // Handler for RIP_EVENTs
    virtual
    DWORD RipEvent ( DWORD            dwProcessId ,
                     DWORD            dwThreadId  ,
                     RIP_INFO &       stRI         )
    {
        return ( DBG_CONTINUE ) ;
    }

    // Handler for Unknown debug events.
    virtual
    DWORD UnknownEvent ( DWORD            dwEventCode ,
                         DWORD            dwProcessId ,
                         DWORD            dwThreadId   )
    {
        return ( DBG_CONTINUE ) ;
    }

} ;

// Turn back on the "'xxx' : unreferenced formal parameter" warning
#pragma warning ( default : 4100 )

/*//////////////////////////////////////////////////////////////////////
                   The Debugger Interface Starts Here
//////////////////////////////////////////////////////////////////////*/

// The debugger relies on event syncronization.  This is done through
// an array of event handles.  The following macros make for convenient
// indicies in that array so that we can easily see which event has
// been signaled.  To make these event strings unique, the event
// strings have the PID of the debuggee appended to them.

// The event handle ID that means the debugger is about to close because
// of a user request.  This is the highest priority event in the
// debugging loop.
#define CLOSEDEBUGGER      0

// The event handle ID to indicate that the debugger is suspending the
// current process.
#define SUSPENDDEBUGGER    1

// The event handle ID to indicate that the debugger is resuming after
// a suspension.
#define RESUMEDEBUGGER     2

// The event handle ID to indicate that the debugger is in the active
// state for the debuggee.
#define ACTIVEDEBUGGER     3

// The total number of debug events that are used by the system.  DO NOT
// assume that the number of events in the event array is always a fixed
// number, it is liable to change.
#define NUM_DEBUGEVENTS    4

/*----------------------------------------------------------------------
FUNCTION    :   StartDebugging
DISCUSSION  :
    Does the work to crank up the debug loop.
PARAMETERS  :
    szDebuggee - The debuggee program to execute.  This should be the
                 full path to the debuggee.
    szCmdLine  - The command line to pass to the debuggee.  This can
                 be NULL if it is not needed.
    lpPID      - If the function succeeds, the PID of the debuggee.
                 This is the PID to pass to CreateDebugSyncEvents
    pUserClass - The main thread notification class.
    lpDebugEvents - The array of handles.  This must be NUM_DEBUGEVENTS
                    items long.
RETURNS     :
    INVALID_HANDLE_VALUE - There was a problem.
    Anything else - The handle to the debug thread.
----------------------------------------------------------------------*/
HANDLE DEBUGINTERFACE_DLLINTERFACE __stdcall
    StartDebugging ( LPCTSTR          szDebuggee        ,
                     LPCTSTR          szCmdLine         ,
                     LPDWORD          lpPID             ,
                     CDebugBaseUser * pUserClass        ,
                     LPHANDLE         lpDebugSyncEvents  ) ;


/*----------------------------------------------------------------------
FUNCTION    :   StopDebugging
DISCUSSION  :
    Stops the debugging loop and terminates the debuggee.
PARAMETERS  :
    lpDebugEvents - The array of initialized handles.
RETURNS     :
    TRUE  - The debug thread was stopped.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL DEBUGINTERFACE_DLLINTERFACE __stdcall
    StopDebugging ( LPHANDLE lpDebugSyncEvents ) ;

/*----------------------------------------------------------------------
FUNCTION    :   SuspendDebugging
DISCUSSION  :
    Suspends the debugging thread.
PARAMETERS  :
    lpDebugEvents - The array of initialized handles.
RETURNS     :
    TRUE  - The debug thread was suspended.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL DEBUGINTERFACE_DLLINTERFACE __stdcall
    SuspendDebugging ( LPHANDLE lpDebugSyncEvents ) ;

/*----------------------------------------------------------------------
FUNCTION    :   ResumeDebugging
DISCUSSION  :
    Resumes the debugging thread.
PARAMETERS  :
    lpDebugEvents - The array of initialized handles.
RETURNS     :
    TRUE  - The debug thread was resumed.
    FALSE - There was a problem.
----------------------------------------------------------------------*/
BOOL DEBUGINTERFACE_DLLINTERFACE __stdcall
    ResumeDebugging ( LPHANDLE lpDebugSyncEvents ) ;

/*----------------------------------------------------------------------
FUNCTION    :   GetDebuggeeProcessHandle
DISCUSSION  :
    Returns the debuggee's process handle.
PARAMETERS  :
    None.
RETURNS     :
    The handle
----------------------------------------------------------------------*/
HANDLE DEBUGINTERFACE_DLLINTERFACE __stdcall
    GetDebuggeeProcessHandle ( void ) ;

/*----------------------------------------------------------------------
FUNCTION    :   GetDebuggeeProcessId
DISCUSSION  :
    Returns the process id for the debuggee.
PARAMETERS  :
    None.
RETURNS     :
    The ID.
----------------------------------------------------------------------*/
DWORD DEBUGINTERFACE_DLLINTERFACE __stdcall
    GetDebuggeeProcessId ( void ) ;

/*----------------------------------------------------------------------
FUNCTION    :   GetDebuggeeModuleName
DISCUSSION  :
    Returns the full path and name to a module loaded by the debuggee.
PARAMETERS  :
    dwAddr - The module base address.
    szName - The buffer to hold the name.
    iLen   - The size of the buffer.
RETURNS     :
    The number of characters copied into szName.
----------------------------------------------------------------------*/
DWORD DEBUGINTERFACE_DLLINTERFACE __stdcall
    GetDebuggeeModuleName ( LPCVOID dwAddr ,
                            LPTSTR  szName ,
                            int     iLen    ) ;

#ifdef __cplusplus
}
#endif

#endif  // _DEBUGINTERFACE_H


