/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include "DeadlockDetection.h"

typedef unsigned ( __stdcall *PSTARTADDR)( void * ) ;


CRITICAL_SECTION g_stCS ;
CRITICAL_SECTION g_stCSTwo ;

void CriticalSectionTests ( )
{
    InitializeCriticalSection ( &g_stCS ) ;

    EnterCriticalSection ( &g_stCS ) ;

    LeaveCriticalSection ( &g_stCS ) ;

    DeleteCriticalSection ( &g_stCS ) ;

    typedef BOOL (WINAPI *PFNINITCRITSECANDSPINCOUNT) (
                                   LPCRITICAL_SECTION lpCriticalSection,
                                   DWORD              dwSpinCount     );

    PFNINITCRITSECANDSPINCOUNT pfnICSASP = NULL ;
    pfnICSASP = (PFNINITCRITSECANDSPINCOUNT)
        GetProcAddress(GetModuleHandle ( _T ( "kernel32.dll"  ) ) ,
                       "InitializeCriticalSectionAndSpinCount"     );
    if ( NULL != pfnICSASP )
    {
        pfnICSASP ( &g_stCSTwo , 400 ) ;
    }

    typedef BOOL (WINAPI *PFNSETCRITSECSPINCOUNT) (
                                   LPCRITICAL_SECTION lpCriticalSection,
                                   DWORD dwSpinCount ) ;

    PFNSETCRITSECSPINCOUNT pfnSCSSP = NULL ;
    pfnSCSSP = (PFNSETCRITSECSPINCOUNT)
        GetProcAddress ( GetModuleHandle ( _T ( "kernel32.dll" ) ) ,
                         "SetCriticalSectionSpinCount"              ) ;
    if ( NULL != pfnSCSSP )
    {
        pfnSCSSP ( &g_stCSTwo , 0x800 );
    }
    typedef BOOL (WINAPI *PFNTRYENTERCRITSEC)
                              ( LPCRITICAL_SECTION lpCriticalSection ) ;

    PFNTRYENTERCRITSEC pfnTECS = NULL ;
    pfnTECS = (PFNTRYENTERCRITSEC)
        GetProcAddress ( GetModuleHandle ( _T ( "kernel32.dll" ) ) ,
                         "TryEnterCriticalSection"                  ) ;
    if ( NULL != pfnTECS )
    {
        pfnTECS ( &g_stCSTwo ) ;
    }

}

void EventTests ( void )
{
    HANDLE hRet ;
    HANDLE hDup ;

    hRet = CreateEventA ( NULL , TRUE , FALSE , "Manual rest event" ) ;
    SetEvent ( hRet ) ;
    ResetEvent ( hRet ) ;
    hDup = OpenEventA ( EVENT_ALL_ACCESS ,
                        FALSE            ,
                        "Manual rest event" ) ;

    // Wide Version.
    hRet = CreateEventW ( NULL , TRUE , FALSE , L"Wide Char event" ) ;
    PulseEvent ( hRet ) ;
    hDup = OpenEventW ( EVENT_ALL_ACCESS , FALSE , L"Wide Char event" );
}

void MutexTests ( void )
{
    HANDLE hRet ;
    HANDLE hDup ;

    hRet = CreateMutexA ( NULL , TRUE , "ANSI Mutex String" ) ;
    hDup = OpenMutexA ( MUTEX_ALL_ACCESS , FALSE , "ANSI Mutex String" );
    ReleaseMutex ( hRet ) ;
    ReleaseMutex ( hDup ) ;

    hRet = CreateMutexW ( NULL , TRUE , L"Wide Mutex String" ) ;
    hDup = OpenMutexW ( MUTEX_ALL_ACCESS    , FALSE , L"Wide Mutex String");
    ReleaseMutex ( hRet ) ;
    ReleaseMutex ( hDup ) ;
}

void SemaphoreTests ( void )
{
    HANDLE hRet ;
    hRet = CreateSemaphoreA ( NULL , 1 , 1 , "FooSemaphore" ) ;

    HANDLE hRet2 = OpenSemaphoreA ( SEMAPHORE_ALL_ACCESS ,
                                    FALSE                ,
                                    "Non existant semaphore!" ) ;

    hRet = CreateSemaphoreW ( NULL , 1 , 1 , L"Wide Semaphore" ) ;

    hRet2 = OpenSemaphoreW ( SEMAPHORE_ALL_ACCESS ,
                             FALSE                ,
                             L"Wide Nonexistant" ) ;


    ReleaseSemaphore ( hRet , 1 , NULL ) ;
}

// Turn off unreachable code warning after ExitThread.
#pragma warning ( disable : 4702 )
DWORD __stdcall ThreadFunc ( DWORD dwSleepVal )
{
    Sleep ( dwSleepVal ) ;

    _endthreadex ( 3 ) ;
    return ( 5 ) ;
}

DWORD __stdcall CThreadFunc ( DWORD dwSleepVal )
{
    Sleep ( dwSleepVal ) ;

    ExitThread ( 5 ) ;
    return ( 5 ) ;
}


void ThreadTests ( void )
{
    DWORD dwThreadId ;

    HANDLE hThread = (HANDLE)CreateThread ( NULL        ,
                                            0           ,
                                   (LPTHREAD_START_ROUTINE)CThreadFunc ,
                                            (LPVOID)0   ,
                                            0           ,
                                            &dwThreadId  );

    HANDLE hThread2 = (HANDLE)_beginthreadex ( NULL                   ,
                                               0                      ,
                                               (PSTARTADDR)ThreadFunc ,
                                               (LPVOID)0              ,
                                               0                      ,
                                               (unsigned*)&dwThreadId );

    WaitForSingleObject ( hThread2 , INFINITE ) ;

    HANDLE hThread3 = (HANDLE)_beginthreadex ( NULL                   ,
                                               0                      ,
                                               (PSTARTADDR)ThreadFunc ,
                                               (LPVOID)0              ,
                                               CREATE_SUSPENDED       ,
                                               (unsigned*)&dwThreadId );

    SuspendThread ( hThread3 ) ;
    ResumeThread ( hThread3 ) ;
    TerminateThread ( hThread3 , 10 ) ;
    
    CloseHandle ( hThread ) ;
    CloseHandle ( hThread2 ) ;
    CloseHandle ( hThread3 ) ;
}

void WaitTests ( void )
{
    HANDLE hArray[ 2 ] ;

    hArray[0] = CreateEvent ( NULL  ,
                              TRUE  ,
                              FALSE ,
                              _T ( "Wait Tests Event Zero" ) ) ;
    hArray[1] = CreateEvent ( NULL  ,
                              TRUE  ,
                              TRUE  ,
                              _T ( "Wait Tests Event One" ) ) ;
    WaitForSingleObject ( hArray[1] , INFINITE ) ;
    WaitForSingleObjectEx ( hArray[1] , INFINITE , TRUE ) ;
    WaitForMultipleObjects ( 2 ,
                            (CONST HANDLE*)&hArray ,
                            FALSE ,
                            INFINITE ) ;
    WaitForMultipleObjectsEx ( 2 ,
                               (CONST HANDLE*)&hArray ,
                               FALSE ,
                               INFINITE ,
                               TRUE ) ;
}

int main ( void )
{
    BOOL bRet = OpenDeadlockDetection ( DDOPT_ALL ) ;
    if ( FALSE == bRet )
    {
        _tprintf ( _T ( "OpenDeadlockDetection failed!" ) ) ;
        return ( 0 ) ;
    }

    DWORD dwOpts = GetDeadlockDetectionOptions ( ) ;
    dwOpts = dwOpts ;

    SetDeadlockDetectionOptions ( DDOPT_SEMAPHORE ) ;
    WriteToDeadlockDetectionLog ( _T ( "Test 1 : Should not see  " ) \
                                  _T ( "anything between this line" ));
    // Since the options are only semaphores the first set should
    // not be seen.
    CriticalSectionTests ( ) ;
    WriteToDeadlockDetectionLog(_T("Test 1 : ...and this line from DD")) ;

//////////////////////////////////////////////

    SetDeadlockDetectionOptions ( DDOPT_ALL ) ;
    
    // Do a little GetProcAddress check.
    HINSTANCE hUser32 = GetModuleHandle ( _T ( "USER32.DLL" ) ) ;
    PROC pProc = GetProcAddress ( hUser32 , "MessageBoxA" ) ;
    if ( NULL != pProc )
    {
        HINSTANCE hCrt = GetModuleHandle ( _T ( "MSVCRT.DLL" ) ) ;
        pProc = GetProcAddress ( hCrt , "_beginthreadex" ) ;
    }
    
    
    WriteToDeadlockDetectionLog
                    ( _T ( "Test 2 : Should see 7 crit sec (R) and 1 (C)" ) \
                      _T ( " between this line..." ) ) ;

    CriticalSectionTests ( ) ;

    WriteToDeadlockDetectionLog ( _T ("Test 2 : ...and this line"));

//////////////////////////////////////////////

    WriteToDeadlockDetectionLog
                       ( _T ( "Test 3  : Should see 7 Event (R) " ) \
                         _T ( "between this line..." ) ) ;

    EventTests ( ) ;

    WriteToDeadlockDetectionLog
                        ( _T ( "Test 3  : ...and this line" ) ) ;

//////////////////////////////////////////////
    WriteToDeadlockDetectionLog
                ( _T ( "Test 4  : Should see 8 Mutex (R) between " ) \
                  _T ( " this line..." ) ) ;

    MutexTests ( ) ;

    WriteToDeadlockDetectionLog
                           ( _T ( "Test 4  : ...and this line" ) ) ;

//////////////////////////////////////////////

    WriteToDeadlockDetectionLog
                ( _T ( "Test 5  : Should see 5 Semaphore (R) " ) \
                  _T ( "between this line..." ) ) ;

    SemaphoreTests ( ) ;

    WriteToDeadlockDetectionLog
                ( _T ( "Test 5  : ...and this line" ) ) ;

//////////////////////////////////////////////

    WriteToDeadlockDetectionLog
               ( _T ( "Test 6  : Should see 11 *thread* ONLY related ")\
                 _T ( "(C) and (R) {ignore Wait* and CloseHandle " ) \
                 _T ( "calls} between this line..." ) ) ;

    ThreadTests ( ) ;

    WriteToDeadlockDetectionLog
                ( _T ( "Test 6  : ...and this line" ) ) ;


//////////////////////////////////////////////
    WriteToDeadlockDetectionLog
                ( _T ( "Test 7  : Should see 8 Wait* ONLY related " ) \
                  _T ( "(C) and (R) {ignore CreateEventW calls} " ) \
                  _T ( "between this line..." ) ) ;
    WaitTests ( ) ;

    WriteToDeadlockDetectionLog
                 ( _T ( "Test 7  : ...and this line" ) ) ;

    HMODULE hMod = LoadLibrary ( _T ( "KERBEROS.DLL" ) ) ;
    FreeLibrary ( hMod ) ;
    
    HMODULE hMod2 = LoadLibraryEx ( _T ( "ADVAPI32.DLL"  ) ,
                                    NULL                   ,
                                    LOAD_WITH_ALTERED_SEARCH_PATH ) ;

    FreeLibrary ( hMod2 ) ;
    
    hMod2 = LoadLibraryA ( "CARDS.DLL" ) ;
    FreeLibrary ( hMod2 ) ;
    
    // No more output.
    SuspendDeadlockDetection ( ) ;
    return ( 3 ) ;
}
