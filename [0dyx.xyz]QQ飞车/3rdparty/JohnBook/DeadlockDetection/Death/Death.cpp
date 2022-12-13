/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <process.h>
#include "DeadlockDetection.h"

#define EVT_NAME _T ( "The Event Name" )

CRITICAL_SECTION g_stCS ;

DWORD __stdcall HappyGoLuckyThread ( DWORD )
{
    _tprintf ( _T ( "I'm HappyGoLuckyThread and I would like the CS please!\n" ) ) ;
    EnterCriticalSection ( &g_stCS ) ;
    _tprintf ( _T ( "HappyGoLuckyThread has the CS, waiting on the event\n" ) ) ;
    HANDLE hEvent = OpenEvent ( EVENT_ALL_ACCESS , FALSE , EVT_NAME ) ;
    WaitForSingleObject ( hEvent , INFINITE ) ;
    CloseHandle ( hEvent ) ;
    LeaveCriticalSection (  &g_stCS ) ;
    return  ( 1 ) ;
}

DWORD __stdcall GrumpyThread ( DWORD )
{
    _tprintf ( _T ( "I'm GrumpyThread opening the event, NOW!\n" ) ) ;
    HANDLE hEvent = OpenEvent ( EVENT_ALL_ACCESS , FALSE , EVT_NAME ) ;
    _tprintf ( _T ( "GrumpyThread give me the CS, NOW!\n" ) ) ;
    EnterCriticalSection ( &g_stCS ) ;
    SetEvent ( hEvent ) ;
    CloseHandle ( hEvent ) ;
    LeaveCriticalSection (  &g_stCS ) ;
    return  ( 1 ) ;
}

typedef unsigned ( __stdcall *PSTARTADDR)( void * ) ;

void _tmain ( void )
{
    HANDLE hEvent ;
    DWORD dwThreadId ;

    OpenDeadlockDetection ( DDOPT_ALL ) ;

    InitializeCriticalSection ( &g_stCS ) ;
    hEvent = CreateEvent ( NULL , TRUE , FALSE , EVT_NAME ) ;


    HANDLE hThread1 = (HANDLE)
                      _beginthreadex ( NULL                           ,
                                       0                              ,
                                       (PSTARTADDR)HappyGoLuckyThread ,
                                       (LPVOID)0                      ,
                                       0                              ,
                                       (unsigned*)&dwThreadId         );

    HANDLE hThread2 = (HANDLE)
                      _beginthreadex ( NULL                     ,
                                       0                        ,
                                       (PSTARTADDR)GrumpyThread ,
                                       (LPVOID)0                ,
                                       0                        ,
                                       (unsigned*)&dwThreadId    ) ;

    EnterCriticalSection ( &g_stCS ) ;
    WaitForSingleObject ( hEvent , INFINITE ) ;
    LeaveCriticalSection ( &g_stCS ) ;
    CloseHandle ( hEvent ) ;
    CloseHandle ( hThread1 ) ;
    CloseHandle ( hThread2 ) ;
}
