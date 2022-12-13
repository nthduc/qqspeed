/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The thread functions.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "DeadlockDetection.h"
#include "Internal.h"

// Turn off all optimizations to avoid registers being changed behind
// my back.
#pragma optimize( "", off )

// Disable the "conditional expression is constant" in the
// HOOKFN_STARTUP macro.
#pragma warning ( disable : 4127 )

HANDLE NAKEDDEF DD_CreateThread (
                              LPSECURITY_ATTRIBUTES  lpThreadAttributes,
                              DWORD                  dwStackSize       ,
                              LPTHREAD_START_ROUTINE lpStartAddress    ,
                              LPVOID                 lpParameter       ,
                              DWORD                  dwCreationFlags   ,
                              LPDWORD                lpThreadId        )
{
    HOOKFN_STARTUP ( eCreateThread , DDOPT_THREADS , FALSE ) ;

    CreateThread ( lpThreadAttributes ,
                   dwStackSize        ,
                   lpStartAddress     ,
                   lpParameter        ,
                   dwCreationFlags    ,
                   lpThreadId          ) ;
                   
    HOOKFN_SHUTDOWN ( 6 , DDOPT_THREADS ) ;
}

// Unreachable code after ExitProcess.
#pragma warning ( disable :4702 )
VOID NAKEDDEF DD_ExitThread ( DWORD dwExitCode )
{
    // Once ExitThread is called, nothing after it will execute so
    // I log the call before the thread goes away.
    HOOKFN_STARTUP ( eExitThread , DDOPT_THREADS , TRUE ) ;

    ExitThread ( dwExitCode ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_THREADS ) ;
}
// Unreachable code after ExitProcess.
#pragma warning ( default :4702 )

DWORD NAKEDDEF DD_SuspendThread ( HANDLE hThread )
{
    HOOKFN_STARTUP ( eSuspendThread , DDOPT_THREADS , TRUE ) ;
    
    SuspendThread ( hThread ) ;

    HOOKFN_SHUTDOWN ( 1 , DDOPT_THREADS ) ;
}

DWORD NAKEDDEF DD_ResumeThread ( HANDLE hThread )
{
    HOOKFN_STARTUP ( eResumeThread , DDOPT_THREADS , TRUE ) ;
    
    ResumeThread ( hThread ) ;

    HOOKFN_SHUTDOWN ( 1 , DDOPT_THREADS ) ;
}

BOOL NAKEDDEF DD_TerminateThread ( HANDLE hThread , DWORD dwExitCode )
{
    HOOKFN_STARTUP ( eTerminateThread , DDOPT_THREADS , TRUE ) ;

    TerminateThread ( hThread , dwExitCode ) ;

    HOOKFN_SHUTDOWN ( 2 , DDOPT_THREADS ) ;
}

// The CRT versions of the above functions.  THESE ARE ALL CDECL!!!!!
uintptr_t NAKEDDEF
    DD_beginthreadex ( void *           security        ,
                       unsigned         stack_size      ,
                       unsigned ( __stdcall *start_address )( void * ) ,
                       void *           arglist         ,
                       unsigned         initflag        ,
                       unsigned *       thrdaddr         )
{
    HOOKFN_STARTUP ( e_beginthreadex , DDOPT_THREADS , FALSE ) ;

    _beginthreadex ( security       ,
                     stack_size     ,
                     start_address  ,
                     arglist        ,
                     initflag       ,
                     thrdaddr        ) ;

    // This is CDECL so use 0 as the number of parameters to remove!
    // The caller takes care of them.
    HOOKFN_SHUTDOWN ( 0 , DDOPT_THREADS ) ;
}

uintptr_t NAKEDDEF
    DD_beginthread ( void( __cdecl *start_address )( void * ) ,
                     unsigned   stack_size                    ,
                     void *     arglist                        )
{
    HOOKFN_STARTUP ( e_beginthread , DDOPT_THREADS , FALSE ) ;
    
    _beginthread ( start_address , stack_size , arglist ) ;
    
    // This is CDECL so use 0 as the number of parameters to remove!
    // The caller takes care of them.
    HOOKFN_SHUTDOWN ( 0 , DDOPT_THREADS ) ;
}

VOID NAKEDDEF DD_endthreadex ( unsigned retval )
{
    // Once ExitThread is called, nothing after it will execute so
    // I log the call before the thread goes away.
    HOOKFN_STARTUP ( e_endthreadex , DDOPT_THREADS , TRUE ) ;

    _endthreadex ( retval ) ;

    // This is CDECL so use 0 as the number of parameters to remove!
    // The caller takes care of them.
    HOOKFN_SHUTDOWN ( 0 , DDOPT_THREADS ) ;
}

VOID NAKEDDEF DD_endthread ( void )
{
    // Once ExitThread is called, nothing after it will execute so
    // I log the call before the thread goes away.
    HOOKFN_STARTUP ( e_endthread , DDOPT_THREADS , TRUE ) ;

    _endthread ( ) ;

    // This is CDECL so use 0 as the number of parameters to remove!
    // The caller takes care of them.
    HOOKFN_SHUTDOWN ( 0 , DDOPT_THREADS ) ;
}


// Turn optimizations back on.
#pragma optimize( "", on )

