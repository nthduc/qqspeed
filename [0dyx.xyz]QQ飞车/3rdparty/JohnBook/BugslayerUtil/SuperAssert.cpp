/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "PCH.h"
#include "BugslayerUtil.h"
#include "SuperAssert.h"
#include "AssertDlg.h"
#include "CriticalSection.h"
#include "resource.h"
#include "Internal.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Typedefs, Constants, & Defines
//////////////////////////////////////////////////////////////////////*/
// The maximum number of threads I can handle at once.
const int k_MAXTHREADS = 100 ;

// The GetProcessHandleCount typedef.
typedef BOOL (__stdcall *GETPROCESSHANDLECOUNT)(HANDLE , PDWORD) ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes
//////////////////////////////////////////////////////////////////////*/
// Does the actual work to pop the assertion dialog.
static INT_PTR PopTheFancyAssertion ( TCHAR * szBuffer      ,
                                      LPCSTR  szEmail       ,
                                      DWORD64 dwStack       ,
                                      DWORD64 dwStackFrame  ,
                                      DWORD64 dwIP          ,
                                      int *   piIgnoreCount  ) ;
                                      
// Tries to get the module causing the assertion.
static SIZE_T GetModuleWithAssert ( DWORD64 dwIP   ,
                                    TCHAR * szMod  ,
                                    DWORD   dwSize  ) ;
                                      
// Yes, this is the compiler intrinsic, but you have to prototype it in
// order to use it.
extern "C" void * _ReturnAddress ( void ) ;
#pragma intrinsic ( _ReturnAddress )

// A function to hide the machinations to get the open handles in the
// process.
static BOOL SafelyGetProcessHandleCount ( PDWORD pdwHandleCount ) ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The number of assertions to ignore on a global basis.
int g_iGlobalIgnoreCount = 0 ;
// The total number of assertions.
static int g_iTotalAssertions = 0 ;
// The critical section that protects everything.
static CCriticalSection g_cCS ;
// The pointer to the GetProcessHandleCount function.
static GETPROCESSHANDLECOUNT g_pfnGPH = NULL ;

/*//////////////////////////////////////////////////////////////////////
// Implementation!
//////////////////////////////////////////////////////////////////////*/
// Turn off "unreachable code" error from this function calling
// ExitProcess.
#pragma warning ( disable : 4702 )
BOOL RealSuperAssertion ( LPCWSTR  szType        ,
                          LPCWSTR  szExpression  ,
                          LPCWSTR  szFunction    ,
                          LPCWSTR  szFile        ,
                          int      iLine         ,
                          LPCSTR   szEmail       ,
                          DWORD64  dwStack       ,
                          DWORD64  dwStackFrame  ,
                          DWORD64  dwIP          ,
                          int *    piFailCount   ,
                          int *    piIgnoreCount  )
{
    // Always start by bumping up the total number of assertions seen
    // so far.
    g_iTotalAssertions++ ;

    // Bump up this particular instance failure count.
    if ( NULL != piFailCount )
    {
        *piFailCount = *piFailCount + 1 ;
    }

    // See if there is any way to short circuit doing the whole dialog.
    // A "-1" means ignore everything.
    if ( ( g_iGlobalIgnoreCount < 0                        ) ||
         ( ( NULL != piIgnoreCount ) && *piIgnoreCount < 0 )   )
    {
        return ( FALSE ) ;
    }

    // If I am in the middle of ignoring all assertions for a bit, I can
    // skip out early!
    if ( g_iGlobalIgnoreCount > 0 )
    {
        g_iGlobalIgnoreCount-- ;
        return ( FALSE ) ;
    }

    // Am I supposed to skip this local assertion?
    if ( ( NULL != piIgnoreCount ) && ( *piIgnoreCount > 0 ) )
    {
        *piIgnoreCount = *piIgnoreCount - 1 ;
        return ( FALSE ) ;
    }

    // Holds the return value of the string (STRSAFE) manipulation
    // functions.
    HRESULT hr = S_OK ;

    // Save off the last error value so I don't whack it doing the
    // assertion dialog.
    DWORD dwLastError = GetLastError ( ) ;

    TCHAR szFmtMsg[ MAX_PATH ] ;
    DWORD dwMsgRes = ConvertErrorToMessage ( dwLastError ,
                                             szFmtMsg    ,
                                             sizeof ( szFmtMsg ) /
                                                    sizeof ( TCHAR ) ) ;
    if ( 0 == dwMsgRes )
    {
        hr = StringCchCopy ( szFmtMsg                               ,
                             sizeof ( szFmtMsg ) / sizeof ( TCHAR ) ,
                  _T ( "Last error message text not available\r\n" ) ) ;
        ASSERT ( SUCCEEDED ( hr ) ) ;
    }

    // Get the module information.
    TCHAR szModuleName[ MAX_PATH ] ;
    if ( 0 == GetModuleWithAssert ( dwIP , szModuleName , MAX_PATH ))
    {
        hr = StringCchCopy ( szModuleName                             ,
                             sizeof ( szModuleName ) / sizeof (TCHAR) ,
                             _T ( "<unknown application>" )           );
        ASSERT ( SUCCEEDED ( hr ) ) ;
    }

    // Grab the synchronization object to block other threads from
    // getting to this point.
    EnterCriticalSection ( &g_cCS.m_CritSec ) ;

    // The buffer to hold the expression message.
    TCHAR szBuffer[ 2048 ] ;
#define BUFF_CHAR_SIZE ( sizeof ( szBuffer ) / sizeof ( TCHAR ) )

    if ( ( NULL != szFile ) && ( NULL != szFunction ) )
    {
        // Split out the base name from the whole filename.
        TCHAR szTempName[ MAX_PATH ] ;
        LPTSTR szFileName ;
        LPTSTR szDir = szTempName ;

        hr = StringCchCopy ( szDir                                    ,
                             sizeof ( szTempName ) / sizeof ( TCHAR ) ,
                             szFile                                   );
        ASSERT ( SUCCEEDED ( hr ) ) ;
        szFileName = _tcsrchr ( szDir , _T ( '\\' ) ) ;
        if ( NULL == szFileName )
        {
            szFileName = szTempName ;
            szDir = _T ( "" ) ;
        }
        else
        {
            *szFileName = _T ( '\0' ) ;
            szFileName++ ;
        }
        DWORD dwHandleCount = 0 ;
        if ( TRUE == SafelyGetProcessHandleCount ( &dwHandleCount ) )
        {
            // Use the new STRSAFE functions to ensure I don't run off
            // the end of the buffer.
            hr = StringCchPrintf (
                       szBuffer                                      ,
                       BUFF_CHAR_SIZE                                ,
                      _T ( "Type         : %s\r\n"                   )\
                      _T ( "Expression   : %s\r\n"                   )\
                      _T ( "Module       : %s\r\n"                   )\
                      _T ( "Location     : %s, Line %d in %s (%s)\r\n")\
                      _T ( "LastError    : 0x%08X (%d)\r\n"          )\
                      _T ( "               %s"                       )\
                      _T ( "Fail count   : %d\r\n"                   )\
                      _T ( "Handle count : %d"                       ),
                       szType                                         ,
                       szExpression                                   ,
                       szModuleName                                   ,
                       szFunction                                     ,
                       iLine                                          ,
                       szFileName                                     ,
                       szDir                                          ,
                       dwLastError                                    ,
                       dwLastError                                    ,
                       szFmtMsg                                       ,
                       *piFailCount                                   ,
                       dwHandleCount                                  );
            ASSERT ( SUCCEEDED ( hr ) ) ;
        }
        else
        {
            hr = StringCchPrintf (
                       szBuffer                                      ,
                       BUFF_CHAR_SIZE                                ,
                       _T ( "Type       : %s\r\n"                   ) \
                       _T ( "Expression : %s\r\n"                   ) \
                       _T ( "Module     : %s\r\n"                   ) \
                       _T ( "Location   : %s, Line %d in %s (%s)\r\n")\
                       _T ( "LastError  : 0x%08X (%d)\r\n"          ) \
                       _T ( "             %s"                       ) \
                       _T ( "Fail count : %d\r\n"                   ) ,
                       szType                                         ,
                       szExpression                                   ,
                       szModuleName                                   ,
                       szFunction                                     ,
                       iLine                                          ,
                       szFileName                                     ,
                       szDir                                          ,
                       dwLastError                                    ,
                       dwLastError                                    ,
                       szFmtMsg                                       ,
                       *piFailCount                                   );
            ASSERT ( SUCCEEDED ( hr ) ) ;
        }
    }
    else
    {
        if ( NULL == szFunction )
        {
            szFunction = _T ( "Unknown function" ) ;
        }
        hr = StringCchPrintf ( szBuffer                        ,
                          BUFF_CHAR_SIZE                       ,
                          _T ( "Type       : %s\r\n"           ) \
                          _T ( "Expression : %s\r\n"           ) \
                          _T ( "Function   : %s\r\n"           ) \
                          _T ( "Module     : %s\r\n"           ) \
                          _T ( "LastError  : 0x%08X (%d)\r\n"  )
                          _T ( "             %s"               ) ,
                          szType                                 ,
                          szExpression                           ,
                          szFunction                             ,
                          szModuleName                           ,
                          dwLastError                            ,
                          dwLastError                            ,
                          szFmtMsg                                ) ;
        ASSERT ( SUCCEEDED ( hr ) ) ;
    }

    if ( DA_SHOWODS == ( DA_SHOWODS & GetDiagAssertOptions ( ) ) )
    {
        OutputDebugString ( szBuffer ) ;
        OutputDebugString ( _T ( "\n" ) ) ;
    }

    if ( DA_SHOWEVENTLOG ==
                        ( DA_SHOWEVENTLOG & GetDiagAssertOptions ( ) ) )
    {
        // Only write to the event log if everything is really kosher.
        static BOOL bEventSuccessful = TRUE ;
        if ( TRUE == bEventSuccessful )
        {
            bEventSuccessful = OutputToEventLog ( szBuffer ) ;
        }
    }

    if ( INVALID_HANDLE_VALUE != GetDiagAssertFile ( ) )
    {
        static BOOL bWriteSuccessful = TRUE ;
        
        if ( TRUE == bWriteSuccessful )
        {
            DWORD dwWritten ;
            int    iLen = lstrlen ( szBuffer ) ;
            char * pToWrite = NULL ;

#ifdef UNICODE
            pToWrite = (char*)_alloca ( iLen + 1 ) ;

            BSUWide2Ansi ( szBuffer , pToWrite , iLen + 1 ) ;
#else
            pToWrite = szBuffer ;
#endif
            bWriteSuccessful = WriteFile ( GetDiagAssertFile ( )   ,
                                           pToWrite                ,
                                           iLen                    ,
                                           &dwWritten              ,
                                           NULL                     ) ;
            if ( FALSE == bWriteSuccessful )
            {
                OutputDebugString (
                  _T ( "\n\nWriting assertion to file failed.\n\n" ) ) ;
            }
        }
    }

    // By default, treat the return as an IGNORE.  This works best in
    // the case the user does not want the MessageBox.
    INT_PTR iRet = IDIGNORE ;

    // Only show the dialog if the process is running interactively and
    // the user wants to see it.
    if ( ( DA_SHOWMSGBOX == ( DA_SHOWMSGBOX & GetDiagAssertOptions()))&&
         ( TRUE == BSUIsInteractiveUser ( )                          )  )
    {
        iRet = PopTheFancyAssertion ( szBuffer      ,
                                      szEmail       ,
                                      dwStack       ,
                                      dwStackFrame  ,
                                      dwIP          ,
                                      piIgnoreCount  ) ;
    }

    // I'm done with the critical section!
    LeaveCriticalSection ( &g_cCS.m_CritSec ) ;

    SetLastError ( dwLastError ) ;

    // Does the user want to break into the debugger?
    if ( IDRETRY == iRet )
    {
        return ( TRUE ) ;
    }

    // Does the user want to abort the program?
    if ( IDABORT == iRet )
    {
        ExitProcess ( (UINT)-1 ) ;
        return ( TRUE ) ;
    }

    // The only option left is to ignore the assertion.
    return ( FALSE ) ;
}

// Takes care of the grunge to get the assertion dialog shown.
static INT_PTR PopTheFancyAssertion ( TCHAR * szBuffer      ,
                                      LPCSTR  szEmail       ,
                                      DWORD64 dwStack       ,
                                      DWORD64 dwStackFrame  ,
                                      DWORD64 dwIP          ,
                                      int *   piIgnoreCount  )
{

    // I don't do any memory allocation in this routine because I can
    // get into some weird problems.  I am about to boost this threads
    // priority pretty high, in an attempt to starve the other
    // threads so I can suspend them.  If I try to allocate memory at
    // that point, I can end up in a situation where a lower priority
    // thread has the CRT or OS heap synch object and this thread needs
    // it.  Consequently, you are looking at one fat, happy deadlock.
    // (Yes, I originally did this to myself, that's how I know about
    // it!)
    THREADINFO aThreadInfo [ k_MAXTHREADS ] ;
    DWORD aThreadIds [ k_MAXTHREADS ] ;

    // The first thread in the thread info array is ALWAYS the current
    // thread.  It's a zero based array, so the dialog code can treat
    // all threads as equals.  However, for this function, the array
    // is treated as a one-based array so I don't suspend the current
    // thread and such.
    UINT uiThreadHandleCount = 1 ;

    aThreadInfo[ 0 ].dwTID = GetCurrentThreadId ( ) ;
    aThreadInfo[ 0 ].hThread = GetCurrentThread ( ) ;
    aThreadInfo[ 0 ].szStackWalk = NULL ;

    // The first thing is to blast the priority for this thread up to
    // real time.  I don't want to have a thread created while I'm
    // preparing to suspend them.
    int iOldPriority = GetThreadPriority ( GetCurrentThread ( ) ) ;
    VERIFY ( SetThreadPriority ( GetCurrentThread ( )          ,
                                 THREAD_PRIORITY_TIME_CRITICAL  ) ) ;

    DWORD dwPID = GetCurrentProcessId ( ) ;

    DWORD dwIDCount = 0 ;
    if ( TRUE == GetProcessThreadIds ( dwPID                ,
                                       k_MAXTHREADS         ,
                                       (LPDWORD)&aThreadIds ,
                                        &dwIDCount            ) )
    {
        // There has to be at least one thread!!
        ASSERT ( 0 != dwIDCount ) ;
        ASSERT ( dwIDCount < k_MAXTHREADS ) ;

        // Calculate the number of handles.
        uiThreadHandleCount = dwIDCount ;
        // If the number of handles is 1, it's a single threaded app,
        // and I don't need to do anything!
        if ( ( uiThreadHandleCount > 1            ) &&
             ( uiThreadHandleCount < k_MAXTHREADS )   )
        {
            // Open each handle, suspend it, and store the
            // handle so I can resume them later.
            int iCurrHandle = 1 ;
            for ( DWORD i = 0 ; i < dwIDCount ; i++ )
            {
                // Of course, don't suspend this thread!!
                if ( GetCurrentThreadId ( ) != aThreadIds[ i ] )
                {
                    HANDLE hThread =
                            OpenThread ( THREAD_ALL_ACCESS ,
                                         FALSE             ,
                                         aThreadIds [ i ]   ) ;
                    if ( ( NULL != hThread                 ) &&
                         ( INVALID_HANDLE_VALUE != hThread )   )
                    {
                        // If SuspendThread returns -1, there no point
                        // and keeping that thread value around.
                        if ( (DWORD)-1 != SuspendThread ( hThread ) )
                        {
                            aThreadInfo[iCurrHandle].hThread = hThread ;
                            aThreadInfo[iCurrHandle].dwTID =
                                                       aThreadIds[ i ] ;
                            aThreadInfo[iCurrHandle].szStackWalk = NULL;
                            iCurrHandle++ ;
                        }
                        else
                        {
                            VERIFY ( CloseHandle ( hThread ) ) ;
                            uiThreadHandleCount-- ;
                        }
                    }
                    else
                    {
                        // Either this thread has some security set on
                        // it or it happened to end right after I
                        // collected the threads.   Consequently, I need
                        // to decrement the total thread handles or I
                        // will be one off.
                        TRACE( "Can't open thread: %08X\n" ,
                                aThreadIds [ i ]            ) ;
                        uiThreadHandleCount-- ;
                    }
                }
            }
        }
    }

    // Drop the thread priority back down!
    SetThreadPriority ( GetCurrentThread ( ) , iOldPriority ) ;

    // Ensure the application resources are set up.
    JfxGetApp()->m_hInstResources = GetBSUInstanceHandle ( ) ;

    // The assertion dialog its self.
    JAssertionDlg cAssertDlg ( szBuffer                     ,
                               szEmail                      ,
                               dwStack                      ,
                               dwStackFrame                 ,
                               dwIP                         ,
                               piIgnoreCount                ,
                               (LPTHREADINFO)&aThreadInfo   ,
                               uiThreadHandleCount           ) ;

    INT_PTR iRet = cAssertDlg.DoModal ( ) ;

    if ( ( 1 != uiThreadHandleCount           ) &&
         ( uiThreadHandleCount < k_MAXTHREADS )    )
    {
        // Crank up the thread priority again!
        int iOldPriority = GetThreadPriority ( GetCurrentThread ( ) ) ;
        VERIFY ( SetThreadPriority ( GetCurrentThread ( )          ,
                                     THREAD_PRIORITY_TIME_CRITICAL  ) );

        // If I've suspended the other threads in the process, I need to
        // resume them, close the handles and delete the array.
        for ( UINT i = 1 ; i < uiThreadHandleCount ; i++ )
        {
            VERIFY ( (DWORD)-1 !=
                        ResumeThread ( aThreadInfo[ i ].hThread ) ) ;
            VERIFY ( CloseHandle ( aThreadInfo[ i ].hThread ) ) ;
        }
        // Drop the thread priority back to what it was.
        VERIFY ( SetThreadPriority ( GetCurrentThread ( ) ,
                                     iOldPriority           ) ) ;
    }
    return ( iRet ) ;
}

BOOL BUGSUTIL_DLLINTERFACE
    SuperAssertionA ( LPCSTR  szType        ,
                      LPCSTR  szExpression  ,
                      LPCSTR  szFunction    ,
                      LPCSTR  szFile        ,
                      int     iLine         ,
                      LPCSTR  szEmail       ,
                      DWORD64 dwStack       ,
                      DWORD64 dwStackFrame  ,
                      int *   piFailCount   ,
                      int *   piIgnoreCount  )
{
    int iLenType = lstrlenA ( szType ) ;
    int iLenExp = lstrlenA ( szExpression ) ;
    int iLenFile = lstrlenA ( szFile ) ;
    int iLenFunc = lstrlenA ( szFunction ) ;

    wchar_t * pWideType = (wchar_t*)
                          HeapAlloc ( GetProcessHeap ( )       ,
                                      HEAP_GENERATE_EXCEPTIONS ,
                                      ( iLenType + 1 ) *
                                        sizeof ( wchar_t )     ) ;
    wchar_t * pWideExp = (wchar_t*)
                         HeapAlloc ( GetProcessHeap ( )       ,
                                     HEAP_GENERATE_EXCEPTIONS ,
                                     ( iLenExp + 1 ) *
                                       sizeof ( wchar_t )      ) ;
    wchar_t * pWideFile = (wchar_t*)
                          HeapAlloc ( GetProcessHeap ( )      ,
                                      HEAP_GENERATE_EXCEPTIONS ,
                                      ( iLenFile + 1 ) *
                                           sizeof ( wchar_t )   );
    wchar_t * pWideFunc = (wchar_t*)
                          HeapAlloc ( GetProcessHeap ( )       ,
                                      HEAP_GENERATE_EXCEPTIONS ,
                                      ( iLenFunc + 1 ) *
                                           sizeof ( wchar_t )   ) ;

    BSUAnsi2Wide ( szType , pWideType , iLenType + 1 ) ;
    BSUAnsi2Wide ( szExpression , pWideExp , iLenExp + 1 ) ;
    BSUAnsi2Wide ( szFile , pWideFile , iLenFile + 1 ) ;
    BSUAnsi2Wide ( szFunction , pWideFunc , iLenFunc + 1 ) ;

    BOOL bRet ;
    bRet = RealSuperAssertion ( pWideType                     ,
                                pWideExp                      ,
                                pWideFunc                     ,
                                pWideFile                     ,
                                iLine                         ,
                                szEmail                       ,
                                dwStack                       ,
                                dwStackFrame                  ,
                                (DWORD64)_ReturnAddress ( )   ,
                                piFailCount                   ,
                                piIgnoreCount                  ) ;

    VERIFY ( HeapFree ( GetProcessHeap ( ) , 0 , pWideType ) ) ;
    VERIFY ( HeapFree ( GetProcessHeap ( ) , 0 , pWideExp ) ) ;
    VERIFY ( HeapFree ( GetProcessHeap ( ) , 0 , pWideFile ) ) ;

    return ( bRet ) ;
}

BOOL BUGSUTIL_DLLINTERFACE
    SuperAssertionW ( LPCWSTR szType        ,
                      LPCWSTR szExpression  ,
                      LPCWSTR szFunction    ,
                      LPCWSTR szFile        ,
                      int     iLine         ,
                      LPCSTR  szEmail       ,
                      DWORD64 dwStack       ,
                      DWORD64 dwStackFrame  ,
                      int *   piFailCount   ,
                      int *   piIgnoreCount  )
{
    return ( RealSuperAssertion ( szType                      ,
                                  szExpression                ,
                                  szFunction                  ,
                                  szFile                      ,
                                  iLine                       ,
                                  szEmail                     ,
                                  dwStack                     ,
                                  dwStackFrame                ,
                                  (DWORD64)_ReturnAddress ( ) ,
                                  piFailCount                 ,
                                  piIgnoreCount                ) ) ;
}

// Returns the number of times an assertion has been triggered in an
// application.  This number takes into account any way the assertion
// was ignored.
int BUGSUTIL_DLLINTERFACE GetSuperAssertionCount ( void )
{
    return ( g_iTotalAssertions ) ;
}

static BOOL SafelyGetProcessHandleCount ( PDWORD pdwHandleCount )
{
    static BOOL bAlreadyLooked = FALSE ;
    if ( FALSE == bAlreadyLooked )
    {
        HMODULE hKernel32 = ::LoadLibrary ( _T ( "kernel32.dll" ) ) ;
        g_pfnGPH = (GETPROCESSHANDLECOUNT)
                   ::GetProcAddress ( hKernel32               ,
                                      "GetProcessHandleCount"  ) ;
        FreeLibrary ( hKernel32 ) ;
        bAlreadyLooked = TRUE ;
    }
    if ( NULL != g_pfnGPH )
    {
        return ( g_pfnGPH ( GetCurrentProcess ( ) , pdwHandleCount ) );
    }
    else
    {
        return ( FALSE ) ;
    }
}

static SIZE_T GetModuleWithAssert ( DWORD64 dwIP   ,
                                    TCHAR * szMod  ,
                                    DWORD   dwSize  )
{
    // Attempt to get the memory base address for the value on the
    // stack.  From the base address, I'll try to get the module.
    MEMORY_BASIC_INFORMATION stMBI ;
    ZeroMemory ( &stMBI , sizeof ( MEMORY_BASIC_INFORMATION ) ) ;
    SIZE_T dwRet = VirtualQuery ( (LPCVOID)dwIP                      ,
                                  &stMBI                             ,
                                  sizeof ( MEMORY_BASIC_INFORMATION ) );
    if ( 0 != dwRet )
    {
        dwRet = GetModuleFileName ( (HMODULE)stMBI.AllocationBase ,
                                    szMod                         ,
                                    dwSize                         ) ;
        if ( 0 == dwRet )
        {
            // Punt and simply return the EXE.
            dwRet = GetModuleFileName ( NULL , szMod , dwSize ) ;
        }
    }
    return ( dwRet ) ;
}