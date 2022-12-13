/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Windows Applications
Copyright © 2002-2003 John Robbins
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "PerThreadData.h"
#include "Settings.h"

/*//////////////////////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////////////////////*/
// Because the Profiler documentation says CLR threads can jump all over
// the place between real OS threads, I can't use the beauty of
// per-thread storage in Win32.  The first versions of the CLR do have
// a one-to-one mapping but it won't be that way in the future.
// The managed thread IDs are keys and the per thread data are the data
// elements.
typedef map<ThreadID, CPerThreadData *> ThreadDataMap ;

/*//////////////////////////////////////////////////////////////////////
// Constants and defines
//////////////////////////////////////////////////////////////////////*/
// The environment variable that contains only the process to profile.
static const TCHAR * k_FLOWTRACEPROC  = _T ( "FLOWTRACEPROC" ) ;
// The environment variable that indicates if I'm supposed to break
// into the profiler for debugging.
static const TCHAR * k_FLOWTRACEBREAK = _T ( "FLOWTRACEBREAK" ) ;
// The environment variable that indicates the directory you want files
// written to instead of putting them in the same directory as the
// executing process.  This will also be the location for the
// application's .FLS file as well.
static const TCHAR * k_FLOWTRACEFILEDIR = _T ( "FLOWTRACEFILEDIR" ) ;

// The format string used to build up the filename prefix.
static const TCHAR * k_FILEPREFIXFMT = _T ( "%s_%04X_" ) ;

// The thread start format to include the thread number.
static const TCHAR * k_THREADSTARTFMT = _T ( "Thread %d started at" ) ;
// The thread end message.
static const TCHAR * k_THREADENDMSG = _T ( "Thread ended at" ) ;
// The time message format.
static const TCHAR * k_TIMEMSGFMT = _T ( "%s : %02d:%02d:%02d:%04d" ) ;

// The class that indicates startup.
static const TCHAR * k_STARTUPCLASS = _T ( "System.AppDomain" ) ;
// The method that indicates startup.  For Framework V 1.0, it is
// SetupDomain.
static const TCHAR * k_STARTUPFUNC = _T ( "ResetBindingRedirects" ) ;

/*//////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////*/
// The global that indicates shutdown has occured.
static BOOL g_ShutdownProcessed = FALSE ;

// The instance of the interface.  This is used by the entry and exit
// hooks as well as the shutdown code.
class CFlowTrace ;
static CFlowTrace * g_pFlowTrace = NULL ;

/*//////////////////////////////////////////////////////////////////////
// Function Prototypes
//////////////////////////////////////////////////////////////////////*/
// The hook functions to record method entry and exits.
void NakedEnter ( FunctionID funcID ) ;
void NakedLeave ( FunctionID funcID ) ;
void NakedTailCall ( FunctionID funcID ) ;
// The ID mapper function.
UINT_PTR __stdcall IDMapper ( FunctionID functionId     ,
                              BOOL *     pbHookFunction  ) ;


/*//////////////////////////////////////////////////////////////////////
// The profiling class for doing the flowing.
//////////////////////////////////////////////////////////////////////*/
class CFlowTrace : public CBaseProfilerCallback
{
/*//////////////////////////////////////////////////////////////////////
// Ye Ol' Ctor and Dtor.
//////////////////////////////////////////////////////////////////////*/
public      :
    CFlowTrace ( void )
    {
        m_bIsProfiling = FALSE ;
        m_szFileNamePrefix[ 0 ] = _T ( '\0' ) ;
        m_iThreadCount = 0 ;
        
        m_FinalizerThreadID = 0 ;
        m_MainThreadID = 0 ;
        
        m_bIgnoreFinalizerThread = TRUE ;
        m_bSeenEndOfStartup = FALSE ;
    }
    
    virtual ~CFlowTrace ( void )
    {
    }

    // The function called by DllMain to ensure that things get cleaned
    // up if ShutDown is not called.  See section 6.2 of PROFILING.DOC.
    friend void DllUnloadingCheck ( void ) ;

    STDMETHOD ( OnInitialize) ( ) ;
    STDMETHOD ( OnShutdown) ( ) ;

    // I need these to create the per-thread storage.
    STDMETHOD ( ThreadCreated ) ( UINT threadId ) ;
    STDMETHOD ( ThreadDestroyed ) ( UINT threadId ) ;

    // For handling exception notifications that I need to keep the
    // indentation aligned when exceptions happen.m_bIgnoreFinalizerThread
    STDMETHOD ( ExceptionThrown ) ( ObjectID thrownObjectId ) ;
    STDMETHOD ( ExceptionUnwindFunctionLeave ) (  ) ;
    STDMETHOD ( ExceptionCatcherLeave ) (  ) ;

/*//////////////////////////////////////////////////////////////////////
// The static functions the enter and exit callbacks will call on each
// entry and exit.
//////////////////////////////////////////////////////////////////////*/
public      :
    void __stdcall FuncEnter ( FunctionID functionID ) ;
    void __stdcall FuncLeave ( FunctionID functionID ) ;
    void __stdcall FuncTailCall ( FunctionID functionID ) ;
    
    // The function mapper.
    UINT_PTR __stdcall FuncIDMapper ( FunctionID functionId     ,
                                      BOOL *     pbHookFunction  ) ;
                                                                                          
/*//////////////////////////////////////////////////////////////////////
// Helpers to do common grunt work.
//////////////////////////////////////////////////////////////////////*/
protected   :
    // Writes the time to the log with a message prefixed on it.
    void WriteTimeToLog ( CPerThreadData * pData ,
                          LPSYSTEMTIME     pLT   ,
                          LPCTSTR          szMsg  ) ;

/*//////////////////////////////////////////////////////////////////////
// Wrappers on top of the thread map to ensure thread synchronization.
//////////////////////////////////////////////////////////////////////*/
protected   :
    void AddThreadData ( ThreadID tID , CPerThreadData * pData ) ;
    CPerThreadData * GetThreadData ( ThreadID tID ) ;
    void RemoveThreadData ( ThreadID tID ) ;
    
    CPerThreadData * GetCurrentThreadData(ThreadID * pThreadID = NULL) ;
    
/*//////////////////////////////////////////////////////////////////////
// Protected data members
//////////////////////////////////////////////////////////////////////*/
protected   :
    // Flag to indicate profiling is active in this process so I can
    // shut down cleanly.
    BOOL m_bIsProfiling ;
    
    // The information necessary for the individual output filenames.
    // The base name of the process.  This is everything but the file
    // extension.
    wchar_t m_szFileNamePrefix[ MAX_PATH ] ;
    
    // The thread count.
    LONG m_iThreadCount ;
    
    // The main and finalizer thread IDs.  Since I do special processing
    // on these threads, I need to save them.
    ThreadID m_FinalizerThreadID ;
    ThreadID m_MainThreadID ;
    
    // Holds all the thread data.  Since CLR threads will jump between
    // OS threads in the future, I need to do my own TLS.
    ThreadDataMap     m_ThreadMap ;
    CRITICAL_SECTION  m_CS ;
    
    // The settings stuff.
    Settings m_cSettings ;
    
    // The flag that indicates if I'm supposed to pay attention to the
    // finalizer thread.
    BOOL m_bIgnoreFinalizerThread ;
    
    // Flag indicating that I've seen the end of the startup code.
    BOOL m_bSeenEndOfStartup ;
} ;

/*//////////////////////////////////////////////////////////////////////
// CFlowTrace Implementation
//////////////////////////////////////////////////////////////////////*/
HRESULT CFlowTrace :: OnInitialize ( void )
{
    HRESULT hr = S_FALSE ;
    
    ////////////////////////////////////////////////////////////////////
    // Get the local time as soon as possible as I want the start time
    // logged to be as close to the real start as possible.
    ////////////////////////////////////////////////////////////////////
    SYSTEMTIME stLT ;
    GetLocalTime ( &stLT ) ;

    ////////////////////////////////////////////////////////////////////
    // Am I supposed to be loading in this process?
    ////////////////////////////////////////////////////////////////////
    
    if ( FALSE == SupposedToProfileThisProcess ( k_FLOWTRACEPROC ) )
    {
        // Set the flags to nothing.
        hr = m_pICorProfilerInfo->SetEventMask ( COR_PRF_MONITOR_NONE );
        ASSERT ( SUCCEEDED ( hr ) ) ;
        // Pop out now.
        return ( S_OK ) ;
    }
    
    ////////////////////////////////////////////////////////////////////
    // Get the filename pieces I'll need for later.
    ////////////////////////////////////////////////////////////////////
    TCHAR szProcessName[ MAX_PATH ] ;

    DWORD dwLen = GetModuleFileName ( NULL          ,
                                      szProcessName ,
                                      MAX_PATH       ) ;
    ASSERT ( dwLen != 0 ) ;
    if ( 0 == dwLen )
    {
        return ( E_UNEXPECTED ) ;
    }
    wchar_t * pEnd = _tcsrchr ( szProcessName , _T ( '.' ) ) ;
    if ( NULL == pEnd )
    {
        return ( E_UNEXPECTED ) ;
    }
    *pEnd = _T ( '\0' ) ;

    TCHAR szFileDir[ MAX_PATH ] ;
    DWORD dwRet = GetEnvironmentVariable ( k_FLOWTRACEFILEDIR ,
                                           szFileDir          ,
                                           MAX_PATH            ) ;
    if ( 0 != dwRet )
    {
        // The user wants to put the files someplace else instead of
        // the EXE directory.

        // Ensure there's a slash on the end of the file.
        wchar_t * pLastSlash = szFileDir + ( dwRet - 1 ) ;
        if ( *pLastSlash != _T ( '\\' ) )
        {
            pLastSlash++ ;
            *pLastSlash = _T ( '\\' ) ;
            pLastSlash++ ;
            *pLastSlash = _T ( '\0' ) ;
        }
        
        wchar_t * pJustProc = _tcsrchr ( szProcessName , _T ( '\\' ) ) ;
        ASSERT ( NULL != pJustProc ) ;
        if ( NULL == pJustProc )
        {
            return ( E_UNEXPECTED ) ;
        }
        pJustProc++ ;
        
        TCHAR szProcOnly [ MAX_PATH ] ;
        StringCchCopy ( szProcOnly , MAX_PATH , pJustProc ) ;
        
        StringCchPrintf ( szProcessName                               ,
                          sizeof ( szProcessName ) / sizeof ( TCHAR ) ,
                          _T ( "%s%s" )                               ,
                          szFileDir                                   ,
                          szProcOnly                                  );
    }
    StringCchPrintf ( m_szFileNamePrefix                               ,
                      sizeof ( m_szFileNamePrefix ) / sizeof ( TCHAR ) ,
                      k_FILEPREFIXFMT                                  ,
                      szProcessName                                    ,
                      GetCurrentProcessId ( )                         );
               
    ////////////////////////////////////////////////////////////////////
    // Initialize the settings and the thread map critical section.
    ////////////////////////////////////////////////////////////////////
    m_cSettings.Initialize ( szProcessName ) ;
    
    m_bIgnoreFinalizerThread = m_cSettings.IgnoreFinalizerThread ( ) ;
    
    if ( FALSE == m_cSettings.SkipStartupOnMainThread ( ) )
    {
        m_bSeenEndOfStartup = TRUE ;
    }
    else
    {
        m_bSeenEndOfStartup = FALSE ;
    }

    InitializeCriticalSectionAndSpinCount ( &m_CS , 4000 ) ;
    
    ////////////////////////////////////////////////////////////////////
    // Do the profiling API initialization so I get the notifications
    // I want.
    ////////////////////////////////////////////////////////////////////
    
    DWORD dwEvents = COR_PRF_MONITOR_THREADS        |
                     COR_PRF_MONITOR_ENTERLEAVE     |
                     COR_PRF_MONITOR_EXCEPTIONS     |
                     COR_PRF_MONITOR_CLR_EXCEPTIONS  ;
    
    if ( TRUE == m_cSettings.TurnOffInlining ( ) )
    {
        dwEvents |= COR_PRF_DISABLE_INLINING ;
    }
    
    // Indicate which events I'm interested in.
    hr = m_pICorProfilerInfo->SetEventMask ( dwEvents ) ;
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED( hr ) )
    {
        // Set the function hooks.
        hr = m_pICorProfilerInfo->
            SetEnterLeaveFunctionHooks((FunctionEnter*)&NakedEnter     ,
                                       (FunctionLeave*)&NakedLeave     ,
                                       (FunctionTailcall*)
                                                     &NakedTailCall   );
        ASSERT ( SUCCEEDED ( hr ) ) ;
        if ( SUCCEEDED ( hr ) )
        {
            // I only need to set the function ID mapper if I'm supposed
            // to ignore the finalizer thread.
            hr = m_pICorProfilerInfo->
                        SetFunctionIDMapper( (FunctionIDMapper*)
                                                             &IDMapper);
            ASSERT ( SUCCEEDED ( hr ) ) ;
        }
    }
    
    // Finally, indicate I'm profiling away....
    m_bIsProfiling = TRUE ;
    return ( hr ) ;
}

HRESULT CFlowTrace :: OnShutdown ( )
{
    // Only if this instance is profiling can I clear things up.
    if ( TRUE == m_bIsProfiling )
    {
        // Run through and remove any remaining per thread data
        // elements in the thread map.
        ThreadDataMap::iterator i ;
        for ( i = m_ThreadMap.begin ( ) ;
              i != m_ThreadMap.end ( )  ;
              i++                         )
        {
            delete i->second ;
        }
        // Clear the map.
        m_ThreadMap.clear ( ) ;
        
        DeleteCriticalSection ( &m_CS ) ;
    }
    // The shutdown has left the building.
    g_ShutdownProcessed = TRUE ;
    return ( S_OK ) ;
}

HRESULT CFlowTrace :: ThreadCreated ( UINT threadID )
{
    SYSTEMTIME stLT ;
    GetLocalTime ( &stLT ) ;
    
    // Is the thread data already there?
    CPerThreadData * pData = GetThreadData ( threadID ) ;
    if ( NULL == pData )
    {
        // Nope, gotta initialize it.
        // Store off the special two threads IDs.
        if ( 0 == m_MainThreadID )
        {
            InterlockedExchange ( (LONG*)&m_MainThreadID ,
                                  threadID                ) ;
        }
        // Is this the finalizer thread?
        else if ( 0 == m_FinalizerThreadID )
        {
            InterlockedExchange ( (LONG*)&m_FinalizerThreadID ,
                                  threadID                     ) ;
                                  
            // If I'm supposed to ignore the finalizer thread, kick out
            // here so I don't even create the thread data.  The
            // FunIDMapper method will prevent calls coming through on
            // direct finalizer thread calls.  It will not catch cross
            // thread calls through Invoke so I have to look for those
            // myself in the hook functions.
            if ( TRUE == m_bIgnoreFinalizerThread )
            {
                return ( S_OK ) ;
            }
        }

        pData = new CPerThreadData ( ) ;
        BOOL bRet = pData->Initialize ( m_szFileNamePrefix , threadID );
        
        TCHAR szThreadMsg[ 50 ] ;
        StringCchPrintf ( szThreadMsg                               ,
                          sizeof ( szThreadMsg ) / sizeof ( TCHAR ) ,
                          k_THREADSTARTFMT                          ,
                          m_iThreadCount                             ) ;
        InterlockedIncrement ( &m_iThreadCount ) ;

        WriteTimeToLog ( pData , &stLT , szThreadMsg ) ;

        ASSERT ( TRUE == bRet ) ;
        if ( FALSE == bRet )
        {
            return ( E_UNEXPECTED ) ;
        }

        AddThreadData ( threadID , pData ) ;
    }

    return ( S_OK ) ;
}

HRESULT CFlowTrace :: ThreadDestroyed ( UINT threadID )
{
    SYSTEMTIME stLT ;
    GetLocalTime ( &stLT ) ;

    CPerThreadData * pData = GetThreadData ( threadID ) ;
    ASSERT ( NULL != pData ) ;
    if ( NULL != pData )
    {
        WriteTimeToLog ( pData , &stLT , k_THREADENDMSG ) ;

        RemoveThreadData ( threadID ) ;
    }
    return ( S_OK ) ;
}

HRESULT CFlowTrace :: ExceptionThrown ( ObjectID thrownObjectId )
{
    TCHAR szExceptName[ MAX_PATH ] ;
    
    if ( TRUE == GetClassFromObjectId ( thrownObjectId ,
                                        szExceptName   ,
                                        MAX_PATH        ) )
    {
        CPerThreadData * pData = GetCurrentThreadData ( ) ;
        ASSERT ( NULL != pData ) ;
        if ( NULL != pData )
        {
            pData->SetLastExceptionName ( szExceptName ) ;
        }
    }
    
    return ( S_OK ) ;
}

HRESULT CFlowTrace :: ExceptionUnwindFunctionLeave (  )
{
    CPerThreadData * pData = GetCurrentThreadData ( ) ;
    ASSERT ( NULL != pData ) ;
    if ( NULL != pData )
    {
        pData->IncrementExceptionUnwind ( ) ;
    }
    return ( S_OK ) ;
}

HRESULT CFlowTrace :: ExceptionCatcherLeave (  )
{
    CPerThreadData * pData = GetCurrentThreadData ( ) ;
    ASSERT ( NULL != pData ) ;
    if ( NULL != pData )
    {
        // Only report the exception if it were an exception that was
        // unwound through at least one function.
        if ( pData->GetExceptionUnwinds ( ) > 0 )
        {
            pData->ReportExceptionAndUnwinds ( ) ;
        }
    }
    return ( S_OK ) ;
}


void __stdcall CFlowTrace :: FuncEnter ( FunctionID functionID )
{
    ThreadID threadID ;
    CPerThreadData * pData = GetCurrentThreadData ( &threadID ) ;
    
    // Since cross thread invoked calls can come in on the finalizer
    // thread as they are not sent through the Function ID Mapper, I
    // need to double check that this is not that case.
    if ( ( NULL == pData                    ) &&
         ( TRUE == m_bIgnoreFinalizerThread ) &&
         ( threadID == m_FinalizerThreadID  )   )
    {
        return ;
    }
    
    ASSERT ( NULL != pData ) ;
    if ( NULL != pData )
    {
        // Is this the the main thread, I'm supposed to skip startup
        // code, and I haven't seen it yet, don't do anything.
        if ( ( TRUE     == m_bSeenEndOfStartup ) ||
             ( threadID != m_MainThreadID      )   )
        {
            wchar_t szCls[ MAX_PATH ] ;
            wchar_t szFunc[ MAX_PATH ] ;

            BOOL bRet = g_pFlowTrace->
                GetClassAndMethodFromFunctionId ( functionID  ,
                                                  szCls       ,
                                                  MAX_PATH    ,
                                                  szFunc      ,
                                                  MAX_PATH     ) ;
            ASSERT ( TRUE == bRet ) ;
            if ( TRUE == bRet )
            {
                pData->WriteData ( szCls , szFunc ) ;
            }
        }
        pData->IncrementIndentLevel ( ) ;
    }
}

void __stdcall CFlowTrace :: FuncLeave ( FunctionID functionID )
{
    ThreadID threadID ;
    CPerThreadData * pData = GetCurrentThreadData ( &threadID ) ;
    
    // Since cross thread invoked calls can come in on the finalizer
    // thread as they are not sent through the Function ID Mapper, I
    // need to double check that this is not that case.
    if ( ( NULL == pData                    ) &&
        ( TRUE == m_bIgnoreFinalizerThread ) &&
        ( threadID == m_FinalizerThreadID  )   )
    {
        return ;
    }
    
    ASSERT ( NULL != pData ) ;
    if ( NULL != pData )
    {
        wchar_t szCls[ MAX_PATH ] ;
        wchar_t szFunc[ MAX_PATH ] ;

        BOOL bRet = g_pFlowTrace->
                      GetClassAndMethodFromFunctionId ( functionID  ,
                                                        szCls       ,
                                                        MAX_PATH    ,
                                                        szFunc      ,
                                                        MAX_PATH     ) ;
        ASSERT ( TRUE == bRet ) ;
        if ( TRUE == bRet )
        {
            // Always do the decrement to keep everything straight.
            pData->DecrementIndentLevel ( ) ;

            // If were executing the main thread and haven't seen the
            // end of the startup function, check that this one is it.
            if ( ( FALSE == m_bSeenEndOfStartup ) &&
                 ( threadID == m_MainThreadID   )    )
            {
                if ( ( 0 == _tcscmp ( k_STARTUPCLASS , szCls  ) ) &&
                     ( 0 == _tcscmp ( k_STARTUPFUNC  , szFunc ) )    )
                {
                    m_bSeenEndOfStartup = TRUE ;
                }

            }
            else
            {
                pData->WriteData ( szCls , szFunc ) ;
            }
        }
    }
}

void __stdcall CFlowTrace :: FuncTailCall ( FunctionID functionID )
{
    ThreadID threadID ;
    CPerThreadData * pData = GetCurrentThreadData ( &threadID ) ;
    
    // Since cross thread invoked calls can come in on the finalizer
    // thread as they are not sent through the Function ID Mapper, I
    // need to double check that this is not that case.
    if ( ( NULL == pData                    ) &&
        ( TRUE == m_bIgnoreFinalizerThread ) &&
        ( threadID == m_FinalizerThreadID  )   )
    {
        return ;
    }
    
    ASSERT ( NULL != pData ) ;
    if ( NULL != pData )
    {
        wchar_t szCls[ MAX_PATH ] ;
        wchar_t szFunc[ MAX_PATH ] ;

        BOOL bRet = g_pFlowTrace->
                        GetClassAndMethodFromFunctionId ( functionID  ,
                                                          szCls       ,
                                                          MAX_PATH    ,
                                                          szFunc      ,
                                                          MAX_PATH    );
        ASSERT ( TRUE == bRet ) ;
        if ( TRUE == bRet )
        {
            pData->DecrementIndentLevel ( ) ;
            pData->WriteData ( szCls , szFunc ) ;
        }
    }
}

UINT_PTR __stdcall CFlowTrace ::
            FuncIDMapper ( FunctionID functionId     ,
                           BOOL *     pbHookFunction  )
{
    // Set the flag to the default value.
    *pbHookFunction = TRUE ;
    
    ThreadID CurrThreadID = 0 ;
    HRESULT hr =
             m_pICorProfilerInfo->GetCurrentThreadID ( &CurrThreadID ) ;
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        // If this is the finalizer thread, and I'm supposed to ignore
        // it, do so.
        if ( ( TRUE == m_bIgnoreFinalizerThread    ) &&
             ( CurrThreadID == m_FinalizerThreadID )    )
        {
            *pbHookFunction = FALSE ;
        }
    }
    return ( (UINT_PTR)functionId ) ;
}


void CFlowTrace :: WriteTimeToLog ( CPerThreadData * pData ,
                                    LPSYSTEMTIME     pLT   ,
                                    LPCTSTR          szMsg  )
{
    ASSERT ( NULL != pData ) ;
    TCHAR szBuff[ 100 ] ;

    StringCchPrintf ( szBuff                               ,
                      sizeof ( szBuff ) / sizeof ( TCHAR ) ,
                      k_TIMEMSGFMT                         ,
                      szMsg                                ,
                      pLT->wHour                           ,
                      pLT->wMinute                         ,
                      pLT->wMinute                         ,
                      pLT->wMilliseconds                    ) ;
    pData->WriteMessage ( szBuff ) ;
}

void CFlowTrace :: AddThreadData ( ThreadID         tID   ,
                                   CPerThreadData * pData  )
{
    EnterCriticalSection ( &m_CS ) ;
#ifdef _DEBUG
    {
        ThreadDataMap::const_iterator iterTemp ;
        
        iterTemp = m_ThreadMap.find ( tID ) ;
        ASSERT ( iterTemp == m_ThreadMap.end ( ) ) ;
    }
#endif
    m_ThreadMap[ tID ] = pData ;
    LeaveCriticalSection ( &m_CS ) ;
}
                                   
CPerThreadData * CFlowTrace :: GetThreadData ( ThreadID tID )
{
    EnterCriticalSection ( &m_CS ) ;
    
    ThreadDataMap::const_iterator iterVal ;
    
    iterVal = m_ThreadMap.find ( tID ) ;
    
    CPerThreadData * pRet = NULL ;
    
    if ( iterVal != m_ThreadMap.end ( ) )
    {
        pRet = iterVal->second ;
    }

    LeaveCriticalSection ( &m_CS ) ;
    
    return ( pRet ) ;
}

void CFlowTrace :: RemoveThreadData ( ThreadID tID )
{
    EnterCriticalSection ( &m_CS ) ;
    
    CPerThreadData * pData = GetThreadData ( tID ) ;
    ASSERT ( NULL != pData ) ;
    
#ifdef _DEBUG
    size_t iRem =
#endif
        m_ThreadMap.erase ( tID ) ;
    ASSERT ( 1 == iRem ) ;
    
    LeaveCriticalSection ( &m_CS ) ;
    
    // Free the memory associated with the thread data.
    delete pData ;
}

CPerThreadData * CFlowTrace ::
                    GetCurrentThreadData ( ThreadID * pThreadID )
{
    ThreadID CurrThreadID = 0 ;
    HRESULT hr =
             m_pICorProfilerInfo->GetCurrentThreadID ( &CurrThreadID ) ;
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( FAILED ( hr ) )
    {
        return ( NULL ) ;
    }

    CPerThreadData * pData = GetThreadData ( CurrThreadID ) ;
    
    if ( NULL != pThreadID )
    {
        *pThreadID = CurrThreadID ;
    }
    
    return ( pData ) ;
}


/*//////////////////////////////////////////////////////////////////////
// The provocatively naked functions necessary to hook the entry and
// exit.
//////////////////////////////////////////////////////////////////////*/
void __declspec ( naked ) NakedEnter ( FunctionID /*funcID*/ )
{
    __asm
    {
        PUSH EAX                        // Save off whacked registers.
        PUSH ECX
        PUSH EDX
        
        PUSH [ESP + 10h]                // Push the function ID as the
                                        //  parameter.
        MOV  ECX , g_pFlowTrace         // Push the instance data on the
        PUSH ECX                        //  stack.
        CALL CFlowTrace::FuncEnter      // Call the FuncEnter method.
        
        POP  EDX                        // Restore saved registers.
        POP  ECX
        POP  EAX
        
        RET 4                           // Return and clear the function
                                        //  ID off the stack as it was
                                        //  passed to this function.
    }
}

void __declspec ( naked ) NakedLeave ( FunctionID /*funcID*/ )
{
    __asm
    {
        PUSH EAX                        // Save off whacked registers.
        PUSH ECX
        PUSH EDX
        
        PUSH [ESP + 10h]                // Push the function ID as the
                                        //  parameter.
        MOV  ECX , g_pFlowTrace         // Push the instance data on the
        PUSH ECX                        //  stack.
        CALL CFlowTrace::FuncLeave      // Call the FuncEnter method.
        
        POP  EDX                        // Restore saved registers.
        POP  ECX
        POP  EAX
        
        RET 4                           // Return and clear the function
                                        //  ID off the stack as it was
                                        //  passed to this function.
    }
}

void __declspec ( naked ) NakedTailCall ( FunctionID /*funcID*/ )
{
    __asm
    {
        PUSH EAX                        // Save off whacked registers.
        PUSH ECX
        PUSH EDX
        
        PUSH [ESP + 10h]                // Push the function ID as the
                                        //  parameter.
        MOV  ECX , g_pFlowTrace         // Push the instance data on the
        PUSH ECX                        //  stack.
        CALL CFlowTrace::FuncTailCall   // Call the FuncEnter method.
        
        POP  EDX                        // Restore saved registers.
        POP  ECX
        POP  EAX
        
        RET 4                           // Return and clear the function
                                        //  ID off the stack as it was
                                        //  passed to this function.
    }
}

// The ID mapper function.
UINT_PTR __stdcall IDMapper ( FunctionID functionId     ,
                              BOOL *     pbHookFunction  )
{
    return ( g_pFlowTrace->FuncIDMapper ( functionId     ,
                                          pbHookFunction  ) ) ;
}



/*//////////////////////////////////////////////////////////////////////
// The function ProfileLib.LIB needs to create the profiler instance.
//////////////////////////////////////////////////////////////////////*/
ICorProfilerCallback * AllocateProfilerCallback ( )
{
    if ( 0 != GetEnvironmentVariable ( k_FLOWTRACEBREAK , NULL , 0 ) )
    {
        DebugBreak ( ) ;
    }
    g_pFlowTrace = new CFlowTrace ( ) ;
    return ( g_pFlowTrace ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Called by DllMain when this DLL is unloading.
//////////////////////////////////////////////////////////////////////*/
void DllUnloadingCheck ( void )
{
    // Call shutdown in case the code did not go through previously.
    if ( ( FALSE == g_ShutdownProcessed ) &&
         ( NULL != g_pFlowTrace         )   )
    {
        g_pFlowTrace->Shutdown ( ) ;
    }
    // Release the memory.
    if ( NULL != g_pFlowTrace )
    {
        delete g_pFlowTrace ;
        g_pFlowTrace = NULL ;
    }
}

