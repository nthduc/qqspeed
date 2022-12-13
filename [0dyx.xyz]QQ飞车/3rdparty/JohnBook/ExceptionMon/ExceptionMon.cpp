/*//////////////////////////////////////////////////////////////////////
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright (c) 1997-2003 John Robbins -- All rights reserved.

Exception Monitor will look for a few environment variables:
EXPMONBREAK
    - If set, will cause a DebugBreak call so you can debug the
      startup and run of Exception Monitor.
EXPMONPROC
    - If set, will only crank up the Exception Monitor profiler if the
      process name matches the one specified in the environment
      variable.  If the environment variable is not present, Exception
      Monitor will work in all processes it loads into.
EXPMONFLUSH
    - If set, all writes to the output file will be flushed.
EXPMONFILENAME
    - For ASP.NET monitoring, you'll need to specify a file name in a
      directory where the ASPNET_WP.EXE process as the ASPNET user has
      write permissions.

//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"

#ifdef _DEBUG
#ifndef ASSERT
#define ASSERT(x)   if (!(x)) DebugBreak ( )
#define VERIFY(x)   ASSERT(x)
#endif
#endif  // _DEBUG

// The largest character size I'll handle.
static const int k_WIDECHARSIZE = 2048 ;

// The environment variable string to force a DebugBreak as soon as
// the ExceptionMonitor code starts executing.
static const wchar_t * k_EXCEPTMONBREAK = _T ( "EXPMONBREAK" ) ;
// The environment variable indicating the process to monitor.
static const wchar_t * k_EXPMONPROC = _T ( "EXPMONPROC" ) ;
// The environment variable indicating all writes need to be flushed.
static const wchar_t * k_EXPMONFLUSH = _T ( "EXPMONFLUSH" ) ;
// The environment variable indicating the path and file to write to.
static const wchar_t * k_EXPMONFILENAME = _T ( "EXPMONFILENAME" ) ;

// The global that indicates shutdown has occured.
static BOOL g_ShutdownProcessed = FALSE ;

class CExceptionMon : public CBaseProfilerCallback
{
/*//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////*/
public      :
    CExceptionMon ( void )
    {
        m_pFile = NULL ;
        m_bFlushWrites = FALSE ;
        
        m_szNameBuff1 = new wchar_t [ k_WIDECHARSIZE ] ;
        m_szNameBuff2 = new wchar_t [ k_WIDECHARSIZE ] ;
        m_szNameBuff3 = new wchar_t [ k_WIDECHARSIZE ] ;
        
        m_bIsProfiling = FALSE ;
    }
    
    virtual ~CExceptionMon ( void )
    {
        delete [] m_szNameBuff1 ;
        delete [] m_szNameBuff2 ;
        delete [] m_szNameBuff3 ;
    }
    
    // The function called by DllMain to ensure that things get cleaned
    // up if ShutDown is not called.  See section 6.2 of PROFILING.DOC.
    friend void DllUnloadingCheck ( void ) ;

    STDMETHOD ( OnInitialize) ( ) ;
    STDMETHOD ( OnShutdown) (  ) ;

    // The methods that do all the work.
    STDMETHOD ( ExceptionThrown ) ( ObjectID thrownObjectId ) ;
    STDMETHOD ( ExceptionUnwindFinallyEnter ) ( FunctionID functionId ) ;
    STDMETHOD ( ExceptionCatcherEnter ) ( FunctionID functionId ,
                                          ObjectID   objectId    ) ;
                                          
    STDMETHOD ( ExceptionCLRCatcherFound ) ( void ) ;
    STDMETHOD ( ExceptionCLRCatcherExecute ) ( void ) ;
    
    STDMETHOD ( ExceptionOSHandlerEnter ) ( UINT_PTR unused ) ;
    STDMETHOD ( ExceptionOSHandlerLeave ) ( UINT_PTR unused ) ;
    
/*//////////////////////////////////////////////////////////////////////
// Class and method lookup functions
//////////////////////////////////////////////////////////////////////*/
protected   :

    // Call this method to get the thrown exception and call stack
    // information.
    BOOL OutputExceptionAndStack ( void ) ;
                                             
    HRESULT GetClassNameFromValueInterface
                                ( ICorDebugValue * pICorDebugValue ,
                                  LPTSTR           szBuffer        ,
                                  UINT             uiBuffLen        ) ;

    // A helper function for OutputExceptionAndStack.  Don't call this
    // directly.
    HRESULT OutputStack ( ICorDebugChainEnum * pICorDebugChainEnum ) ;
    // Another helper for OutputExceptionAndStack.
    HRESULT OutputFrame ( ICorDebugFrame * pFrame ) ;

    // Helper function to get the current thread ID. Anyone can call
    // this
    DWORD GetExecutingThreadId ( void ) ;
    

/*//////////////////////////////////////////////////////////////////////
// Helpers to do common grunt work.
//////////////////////////////////////////////////////////////////////*/
protected   :
    HRESULT BeginInprocDebugging( LPDWORD            pdwProfContext   ,
                                  ICorDebugThread ** pICorDebugThread ,
                            ICorDebugChainEnum ** pICorDebugChainEnum);
    HRESULT EndInprocDebugging ( DWORD dwProfContext ) ;
                                             

/*//////////////////////////////////////////////////////////////////////
// All the log file handling.
//////////////////////////////////////////////////////////////////////*/
protected   :
    BOOL OpenLog ( void ) ;
    void Log ( LPCTSTR szFmt , ... ) ;
    BOOL CloseLog ( void ) ;
    
/*//////////////////////////////////////////////////////////////////////
// Protected class member data.
//////////////////////////////////////////////////////////////////////*/
protected   :
    // Indicates if profiling is turned on for this instance of
    // Exception Monitor.
    BOOL m_bIsProfiling ;

    // The output file.
    FILE * m_pFile ;
    
    // The flush flag.  Set in the OpenLog method.
    BOOL   m_bFlushWrites ;
    
    // The critical section that protects the stack walking.
    CRITICAL_SECTION m_cCritSec ;
    
    // Buffers used to acquire name values when walking the stack.
    // These can only be accessed in the code protected by the
    // critical section.
    wchar_t * m_szNameBuff1 ;
    wchar_t * m_szNameBuff2 ;
    wchar_t * m_szNameBuff3 ;
    
} ;

/*//////////////////////////////////////////////////////////////////////
// Externally Called Methods
//////////////////////////////////////////////////////////////////////*/
HRESULT CExceptionMon :: OnInitialize ( void )
{
    HRESULT hr = S_FALSE ;
    
    // Is this a process I'm supposed to initialize in?
    m_bIsProfiling = SupposedToProfileThisProcess ( k_EXPMONPROC ) ;
    if ( FALSE == m_bIsProfiling )
    {
        // Since I don't want to anything in this process, I'll call the
        // SetEventMask with COR_PRF_NONE mask just to tell the CLR I
        // don't want any.
        hr = m_pICorProfilerInfo->SetEventMask ( COR_PRF_MONITOR_NONE );
        ASSERT ( SUCCEEDED ( hr ) ) ;
        return ( S_OK ) ;
    }
    
    
    // Indicate which events I'm interested in.
    hr = m_pICorProfilerInfo->
                    SetEventMask ( COR_PRF_MONITOR_EXCEPTIONS |
                                   COR_PRF_MONITOR_CLR_EXCEPTIONS |
                                   COR_PRF_ENABLE_INPROC_DEBUGGING ) ;
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( FAILED ( hr ) )
    {
        return ( E_INVALIDARG ) ;
    }
    
    if ( FALSE == InitializeCriticalSectionAndSpinCount ( &m_cCritSec ,
                                                          4000        ))
    {
        ASSERT ( !"Failed initializing critical section!!" ) ;
        return ( E_UNEXPECTED ) ;
    }
    
    if ( FALSE == OpenLog ( ) )
    {
        ASSERT ( !"Failed opening log!!" ) ;
        return ( E_UNEXPECTED ) ;
    }

    return ( S_OK ) ;
}

HRESULT CExceptionMon :: OnShutdown ( )
{
    // Only do the shutdown work if I'm actually profiling this process.
    if ( TRUE == m_bIsProfiling )
    {
        CloseLog ( ) ;
        
        DeleteCriticalSection ( &m_cCritSec ) ;
    }
    
    g_ShutdownProcessed = TRUE ;
    
    return ( S_OK ) ;
}

HRESULT CExceptionMon :: ExceptionThrown ( ObjectID thrownObjectId )
{
    // OK, there seems to be a oddity in the profiling interface.  If
    // you set the the event mask to request in-process debugging, you
    // will no longer get the object ID passed into ExceptionThrown.
    // All that's passed in is NULL.  Not very helpful when figuring out
    // which exception was thrown.  However, in the
    // ExceptionCatcherEnter method, the object is properly passed in.

    TRACE ( "ExceptionThrown : 0x%08X\n" , thrownObjectId ) ;

    // Grab the critical section.
    EnterCriticalSection ( &m_cCritSec ) ;
    
    if ( 0 != thrownObjectId )
    {
        DWORD dwTID = GetExecutingThreadId ( ) ;
        
        // Get the class name of the exception thrown.
        if ( TRUE == GetClassFromObjectId ( thrownObjectId  ,
                                            m_szNameBuff1   ,
                                            k_WIDECHARSIZE   )  )
        {
            Log ( _T ( "\nException thrown (%08X): %s\n" ) ,
                  dwTID                                    ,
                  m_szNameBuff1                             ) ;
        }
        else
        {
            Log ( _T ( "\nException thrown (%08X): 0x%p\n" ) ,
                  dwTID                                      ,
                  thrownObjectId                              ) ;
        }
    }
    else
    {
        if ( FALSE == OutputExceptionAndStack ( ) )
        {
            Log ( _T ( "Unable to get exception and stack!\n" ) ) ;
        }
    }
    
    // Release the critical section.
    LeaveCriticalSection ( &m_cCritSec ) ;
    
    return ( S_OK ) ;
}

HRESULT CExceptionMon ::
                ExceptionUnwindFinallyEnter ( FunctionID functionId )
{
    // Grab the critical section to keep the output straight.
    EnterCriticalSection ( &m_cCritSec ) ;
    
    DWORD dwTID = GetExecutingThreadId ( ) ;

    // m_szNameBuff1 will contain the class and m_szNameBuff2 will
    // contain the method.
    if ( GetClassAndMethodFromFunctionId ( functionId     ,
                                           m_szNameBuff1  ,
                                           k_WIDECHARSIZE ,
                                           m_szNameBuff2  ,
                                           k_WIDECHARSIZE  ) )
    {
        Log ( _T ( "Finally Executed (%08X): %s.%s\n" ) ,
              dwTID                                     ,
              m_szNameBuff1                             ,
              m_szNameBuff2                              ) ;
    }
    else
    {
        Log ( _T ( "Finally Executed (%08X): 0x%p\n" ) ,
              dwTID                                    ,
              functionId                                ) ;
    }

    // I'm done with the critical section.
    LeaveCriticalSection ( &m_cCritSec ) ;
    
    return ( S_OK ) ;
}

HRESULT CExceptionMon ::
                ExceptionCatcherEnter ( FunctionID functionId ,
                                        ObjectID   objectId    )
{
    // Grab the critical section.
    EnterCriticalSection ( &m_cCritSec ) ;

    DWORD dwTID = GetExecutingThreadId ( ) ;

    // m_szNameBuff1 will contain the class and m_szNameBuff2 will
    // contain the method.
    // m_szNameBuff3 will contain the class name.
    if ( ( GetClassAndMethodFromFunctionId ( functionId     ,
                                             m_szNameBuff1  ,
                                             k_WIDECHARSIZE ,
                                             m_szNameBuff2  ,
                                             k_WIDECHARSIZE  ) ) &&
         ( GetClassFromObjectId ( objectId       ,
                                  m_szNameBuff3  ,
                                  k_WIDECHARSIZE   )           )   )
    {
        Log ( _T ( "Exception Caught (%08X) : %s.%s %s\n" ) ,
              dwTID                                         ,
              m_szNameBuff1                                 ,
              m_szNameBuff2                                 ,
              m_szNameBuff3                                  ) ;
    }
    else
    {
        Log ( _T ( "Exception Caught (%08X): 0x%p 0x%p\n" ) ,
              dwTID                                         ,
              functionId                                    ,
              objectId                                       ) ;
    }
    
    // Release the critical section.
    LeaveCriticalSection ( &m_cCritSec ) ;
    
    return ( S_OK ) ;
}

HRESULT CExceptionMon :: ExceptionCLRCatcherFound ( void )
{
    // Grab the critical section.
    EnterCriticalSection ( &m_cCritSec ) ;

    DWORD dwTID = GetExecutingThreadId ( ) ;
    
    Log ( _T ( "CLR Catcher (%08X) found\n" ) , dwTID ) ;

    // Release the critical section.
    LeaveCriticalSection ( &m_cCritSec ) ;
    
    return ( S_OK ) ;
}

HRESULT CExceptionMon :: ExceptionCLRCatcherExecute ( void )
{
    // Grab the critical section.
    EnterCriticalSection ( &m_cCritSec ) ;

    DWORD dwTID = GetExecutingThreadId ( ) ;

    Log ( _T ( "CLR Catcher (%08X) executing\n" ) , dwTID ) ;

    // Release the critical section.
    LeaveCriticalSection ( &m_cCritSec ) ;

    return ( S_OK ) ;
}


HRESULT CExceptionMon :: ExceptionOSHandlerEnter ( UINT_PTR unused )
{
    // Grab the critical section.
    EnterCriticalSection ( &m_cCritSec ) ;

    DWORD dwTID = GetExecutingThreadId ( ) ;

    Log ( _T ( "OS Handler (%08X) enter %08X)\n" ) ,
          dwTID                                    ,
          unused                                    ) ;

    // Release the critical section.
    LeaveCriticalSection ( &m_cCritSec ) ;

    return ( S_OK ) ;
}

HRESULT CExceptionMon :: ExceptionOSHandlerLeave ( UINT_PTR unused )
{
    // Grab the critical section.
    EnterCriticalSection ( &m_cCritSec ) ;

    DWORD dwTID = GetExecutingThreadId ( ) ;

    Log ( _T ( "OS Handler (%08X) leave %08X)\n" ) ,
        dwTID                                      ,
        unused                                      ) ;

    // Release the critical section.
    LeaveCriticalSection ( &m_cCritSec ) ;

    return ( S_OK ) ;
}


/*/////////////////////////////////////////////////////////////////////
// Internal Only Methods
/////////////////////////////////////////////////////////////////////*/

HRESULT CExceptionMon ::
     BeginInprocDebugging ( LPDWORD               pdwProfContext     ,
                            ICorDebugThread **    pICorDebugThread   ,
                            ICorDebugChainEnum ** pICorDebugChainEnum )
{
    // Tell the profiling API I want to get the in-process debugging
    // stuff.
    HRESULT hr = m_pICorProfilerInfo->
                                BeginInprocDebugging ( TRUE          ,
                                                       pdwProfContext );
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        IUnknown * pIUnknown = NULL ;

        // Ask the profiling API for the IUnknown I can get the
        // ICorDebugThread interface from.
        hr = m_pICorProfilerInfo->
                        GetInprocInspectionIThisThread ( &pIUnknown ) ;
        ASSERT ( SUCCEEDED ( hr ) ) ;
        if ( SUCCEEDED ( hr ) )
        {
            hr = pIUnknown->
                    QueryInterface ( __uuidof ( ICorDebugThread ) ,
                                     (void**)pICorDebugThread       ) ;
            ASSERT ( SUCCEEDED ( hr ) ) ;
            
            // No matter what happens, I don't need the IUnknown any
            // more.
            pIUnknown->Release ( ) ;
            
            // I'm doing this as part of the normal processing because
            // if you don't call ICorDebugThread::EnumerateChains as
            // the first thing called off ICorDebugThread, many of the
            // other methods will fail.
            if ( SUCCEEDED ( hr ) )
            {
                hr = (*pICorDebugThread)->
                            EnumerateChains ( pICorDebugChainEnum ) ;
                ASSERT ( SUCCEEDED ( hr ) ) ;
                if ( FAILED ( hr ) )
                {
                    (*pICorDebugThread)->Release ( ) ;
                }
            }
        }
    }
    return ( hr ) ;
}

HRESULT CExceptionMon :: EndInprocDebugging ( DWORD dwProfContext )
{
    HRESULT hr ;
    hr = m_pICorProfilerInfo->EndInprocDebugging ( dwProfContext ) ;
    return ( hr ) ;
}

HRESULT CExceptionMon ::
    GetClassNameFromValueInterface ( ICorDebugValue * pICorDebugValue ,
                                     LPTSTR           szBuffer        ,
                                     UINT             uiBuffLen        )
{
    HRESULT hr = S_FALSE ;

    ICorDebugObjectValue * pObjVal = NULL ;
    
    ICorDebugReferenceValue * pRefVal = NULL ;
    
    // Get the reference to this value.  Exceptions should come in this
    // way.  If getting the ICorDebugReferenceValue fails, the type is
    // ICorDebugGenericValue.  There's nothing I can do with a
    // ICorDebugGenericValue as I need the class name.
    hr = pICorDebugValue->
                QueryInterface ( __uuidof ( ICorDebugReferenceValue ),
                                 (void**)&pRefVal                    );
    if ( SUCCEEDED ( hr ) )
    {
        // Deference the value.
        ICorDebugValue * pDeRef ;
        hr = pRefVal->Dereference ( &pDeRef ) ;
        
        if ( SUCCEEDED ( hr ) )
        {
            // Now that I dereferenced, I can ask for the object value.
            hr = pDeRef->
                    QueryInterface ( __uuidof ( ICorDebugObjectValue ),
                                    (void**)&pObjVal                 );
                                    
            // I no longer need the dereference.
            pDeRef->Release ( ) ;
        }
        // I no longer need the reference.
        pRefVal->Release ( ) ;
    }

    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        // Get the class interface for this object.
        ICorDebugClass * pClass ;
    
        hr = pObjVal->GetClass ( &pClass ) ;
        
        // I don't need the object reference any more.
        pObjVal->Release ( ) ;
    
        ASSERT ( SUCCEEDED ( hr ) ) ;
        if ( ( SUCCEEDED ( hr ) ) )
        {
            // Gotta have the class type def token value.
            mdTypeDef ClassDef ;
            hr = pClass->GetToken ( &ClassDef ) ;
            
            ASSERT ( SUCCEEDED ( hr ) ) ;
            if ( SUCCEEDED ( hr ) )
            {
                // In order to look up the class token, I need the
                // module so I can query for the meta data interface.
                ICorDebugModule * pMod ;
                hr = pClass->GetModule ( &pMod ) ;
        
                ASSERT ( SUCCEEDED ( hr ) ) ;
                if ( SUCCEEDED ( hr ) )
                {
                    // Get the metadata.
                    IMetaDataImport * pIMetaDataImport = NULL ;

                    hr = pMod->
                        GetMetaDataInterface ( IID_IMetaDataImport ,
                                    (IUnknown**)&pIMetaDataImport   ) ;
            
                    ASSERT ( SUCCEEDED ( hr ) ) ;
                    if ( SUCCEEDED ( hr ) )
                    {
                        // Finally, get the class name.
                        ULONG ulCopiedChars ;
                        
                        hr = pIMetaDataImport->
                                  GetTypeDefProps ( ClassDef       ,
                                                    szBuffer       ,
                                                    uiBuffLen      ,
                                                    &ulCopiedChars ,
                                                    NULL           ,
                                                    NULL            ) ;
                        ASSERT ( ulCopiedChars < uiBuffLen ) ;
                        if ( ulCopiedChars == uiBuffLen )
                        {
                            hr = S_FALSE ;
                        }
                                                         
                        pIMetaDataImport->Release ( ) ;
                    }
                    pMod->Release ( ) ;
                }
            }
            pClass->Release ( ) ;
        }
    }
    return ( hr ) ;
}

HRESULT CExceptionMon :: OutputFrame ( ICorDebugFrame * pFrame )
{
    ICorDebugFunction * pFunc = NULL ;
    
    // Start out by getting the function.
    HRESULT hr = pFrame->GetFunction ( &pFunc ) ;
    
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        // Get the method token so I can grovel for it in the
        // meta data.
        mdMethodDef FuncDefToken ;
        
        hr = pFrame->GetFunctionToken ( &FuncDefToken ) ;
        
        ASSERT ( SUCCEEDED ( hr ) ) ;
        if ( SUCCEEDED ( hr ) )
        {
            // Get the module for this function.
            ICorDebugModule * pMod ;
            hr = pFunc->GetModule ( &pMod ) ;
            
            ASSERT ( SUCCEEDED ( hr ) ) ;
            if ( SUCCEEDED ( hr ) )
            {
                // Get the meta data interface.
                IMetaDataImport * pIMetaDataImport = NULL ;

                hr = pMod->
                    GetMetaDataInterface ( IID_IMetaDataImport ,
                                        (IUnknown**)&pIMetaDataImport ) ;
                                        
                ASSERT ( SUCCEEDED ( hr ) ) ;
                if ( SUCCEEDED ( hr ) )
                {
                    mdTypeDef ClassDefToken ;
                    ULONG chOutCount = 0 ;

                    // Get the method.
                    hr = pIMetaDataImport->
                                    GetMethodProps ( FuncDefToken   ,
                                                     &ClassDefToken ,
                                                     m_szNameBuff2  ,
                                                     k_WIDECHARSIZE ,
                                                     &chOutCount    ,
                                                     NULL           ,
                                                     NULL           ,
                                                     NULL           ,
                                                     NULL           ,
                                                     NULL            ) ;
                    ASSERT ( SUCCEEDED ( hr ) ) ;
                    if ( SUCCEEDED ( hr ) )
                    {
                        // Now that I have the method, get the class
                        // name.
                        hr = pIMetaDataImport->
                                  GetTypeDefProps ( ClassDefToken  ,
                                                    m_szNameBuff1  ,
                                                    k_WIDECHARSIZE ,
                                                    &chOutCount    ,
                                                    NULL           ,
                                                    NULL            ) ;
                                                    
                        ASSERT ( SUCCEEDED ( hr ) ) ;
                        if ( SUCCEEDED ( hr ) )
                        {
                            Log ( _T ( "  %s.%s\n" ) ,
                                  m_szNameBuff1      ,
                                  m_szNameBuff2       ) ;
                        }
                    }
                    
                    pIMetaDataImport->Release ( ) ;
                }
                pMod->Release ( ) ;
            }
        }
        pFunc->Release ( ) ;
    }
    
    return ( hr ) ;
}

HRESULT CExceptionMon :: OutputStack ( ICorDebugChainEnum *
                                                   pICorDebugChainEnum )
{
    ICorDebugChain * pCurrChain = NULL ;
    ULONG ulChainFetch = 0 ;
    
    HRESULT hrNextChain = pICorDebugChainEnum->Next ( 1             ,
                                                      &pCurrChain   ,
                                                      &ulChainFetch  ) ;
                                                      
    while ( ( SUCCEEDED ( hrNextChain ) ) && ( 1 == ulChainFetch ) )
    {
        // According to DEBUGREF.DOC, if the chain is not managed,
        // you'll never see the frames so I will skip the unmanaged
        // chains and only show managed code.
        BOOL bIsManaged = FALSE ;
        
        HRESULT hrIsManaged = pCurrChain->IsManaged ( &bIsManaged ) ;
        
        ASSERT ( SUCCEEDED ( hrIsManaged ) ) ;
        if ( ( SUCCEEDED ( hrIsManaged ) ) && ( TRUE == bIsManaged ) )
        {
            // Get the frame enumerator for this chain.
            ICorDebugFrameEnum * pFrameEnum = NULL ;
            
            HRESULT hrFrameEnum =
                           pCurrChain->EnumerateFrames ( &pFrameEnum ) ;
                           
            ASSERT ( SUCCEEDED ( hrFrameEnum ) ) ;
            if ( SUCCEEDED ( hrFrameEnum ) )
            {
                // Do all the frames.
                ICorDebugFrame * pCurrFrame = NULL ;
                ULONG ulFrameFetch = 0 ;
                
                HRESULT hrNextFrame = pFrameEnum->Next(1            ,
                                                       &pCurrFrame  ,
                                                       &ulFrameFetch );
                while ( ( SUCCEEDED ( hrNextFrame ) ) &&
                        ( 1 == ulFrameFetch         )    )
                {
                
                    OutputFrame ( pCurrFrame ) ;
                    
                    pCurrFrame->Release ( ) ;
                    pCurrFrame = NULL ;
                    hrNextFrame = pFrameEnum->Next ( 1             ,
                                                     &pCurrFrame   ,
                                                     &ulFrameFetch  ) ;
                }
            }
        }
        
        pCurrChain->Release ( ) ;
        pCurrChain = NULL ;
        hrNextChain = pICorDebugChainEnum->Next ( 1             ,
                                                  &pCurrChain   ,
                                                  &ulChainFetch  ) ;
    }
    
    return ( S_OK ) ;
}
                                   
BOOL CExceptionMon :: OutputExceptionAndStack ( void )
{
    DWORD dwProfContext = 0 ;
    ICorDebugThread * pICorDebugThread = NULL ;
    ICorDebugChainEnum * pICorDebugChainEnum = NULL ;
    
    HRESULT hr = BeginInprocDebugging ( &dwProfContext       ,
                                        &pICorDebugThread    ,
                                        &pICorDebugChainEnum  ) ;
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        ICorDebugValue * pICorDebugValue = NULL ;
        hr = pICorDebugThread->
                        GetCurrentException ( &pICorDebugValue ) ;
        if ( SUCCEEDED ( hr ) )
        {
            hr = GetClassNameFromValueInterface ( pICorDebugValue ,
                                                  m_szNameBuff1   ,
                                                  k_WIDECHARSIZE   ) ;
            ASSERT ( SUCCEEDED ( hr ) ) ;
            if ( SUCCEEDED ( hr ) )
            {
            
                DWORD dwTID = 0 ;
                
                hr = pICorDebugThread->GetID ( &dwTID ) ;
                ASSERT ( SUCCEEDED ( hr ) ) ;
                if ( SUCCEEDED ( hr ) )
                {
                    Log ( _T ( "\nException thrown (%08X): %s\n" ) ,
                          dwTID                                    ,
                          m_szNameBuff1                             ) ;
                          
                    //OutputExceptionProperties ( pICorDebugValue ) ;
                    //OutputExceptionArgsAndVars ( pICorDebugThread ) ;
                    OutputStack ( pICorDebugChainEnum ) ;
                }
                else
                {
                    Log(_T("\nException: Getting TID failed!\n"));
                }
            }
            else
            {
                Log(_T("\nException: Getting class name failed!\n"));
            }
            
            pICorDebugValue->Release ( ) ;
        }
        
        pICorDebugThread->Release ( ) ;
        pICorDebugChainEnum->Release ( ) ;
        
        EndInprocDebugging ( dwProfContext );
    }
    
    return ( SUCCEEDED ( hr ) ) ;
}

DWORD CExceptionMon :: GetExecutingThreadId ( void )
{
    ThreadID CurrManagedThread = 0 ;
    DWORD dwWin32ID = 0 ;
    HRESULT hr = S_FALSE ;
    
    hr = m_pICorProfilerInfo->GetCurrentThreadID ( &CurrManagedThread );
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        hr = m_pICorProfilerInfo->GetThreadInfo ( CurrManagedThread ,
                                                  &dwWin32ID         ) ;
        ASSERT ( SUCCEEDED ( hr ) ) ;
        if ( FAILED ( hr ) )
        {
            dwWin32ID = 0 ;
        }
    }
    return ( dwWin32ID ) ;
}

BOOL CExceptionMon :: OpenLog ( void )
{
    ASSERT ( NULL == m_pFile ) ;
    
    // Look to see if the flush environment variable is set.
    if ( 0 != GetEnvironmentVariable ( k_EXPMONFLUSH , NULL , 0 ) )
    {
        m_bFlushWrites = TRUE ;
    }
    
    TCHAR szOutName[ MAX_PATH ] = _T ( "\0" ) ;
    
    // Try to get the filename from the environment variable first.
    if ( 0 == GetEnvironmentVariable ( k_EXPMONFILENAME ,
                                       szOutName        ,
                                       MAX_PATH          ) )
    {
        if ( 0 != GetModuleFileName ( NULL , szOutName , MAX_PATH ) )
        {
            TCHAR * pPeriod = _tcsrchr ( szOutName , _T ( '.' ) ) ;
            if ( NULL == pPeriod )
            {
                _tcscat ( szOutName , _T ( ".LOG" ) ) ;
            }
            else
            {
                _tcscpy ( pPeriod , _T ( ".LOG" ) ) ;
            }
        }
        else
        {
            ASSERT ( ! "GetModuleFileName failed" ) ;
            _tcscpy ( szOutName , _T ( "ExcepMon.LOG" ) ) ;
        }
    }
    
    m_pFile = _tfopen ( szOutName , _T ( "wt" ) ) ;
    ASSERT ( NULL != m_pFile ) ;
    return ( NULL != m_pFile ) ;
}

void CExceptionMon :: Log ( LPCTSTR szFmt , ... )
{
    ASSERT ( NULL != m_pFile ) ;
    
    TCHAR szOutBuff [ 1024 ] ;

    va_list  args ;

    va_start ( args , szFmt ) ;

    wvsprintf ( szOutBuff , szFmt , args ) ;

    va_end ( args ) ;

    _fputts ( szOutBuff, m_pFile );
    
    if ( TRUE == m_bFlushWrites )
    {
        fflush ( m_pFile ) ;
    }
}

BOOL CExceptionMon :: CloseLog ( void )
{
    if ( NULL != m_pFile )
    {
        fclose ( m_pFile ) ;
        m_pFile = NULL ;
        return ( TRUE ) ;
    }
    return ( FALSE ) ;
}

/*//////////////////////////////////////////////////////////////////////
// The instance of the interface.
//////////////////////////////////////////////////////////////////////*/
static CExceptionMon * g_pExceptMon = NULL ;

/*//////////////////////////////////////////////////////////////////////
// The function ProfileLib.LIB needs to create the profiler instance.
//////////////////////////////////////////////////////////////////////*/
ICorProfilerCallback * AllocateProfilerCallback ( )
{
    if ( 0 != GetEnvironmentVariable ( k_EXCEPTMONBREAK ,
                                        NULL             ,
                                        0                 ) )
    {
        DebugBreak ( ) ;
    }
    g_pExceptMon = new CExceptionMon ( ) ;
    return ( g_pExceptMon ) ;
}

/*//////////////////////////////////////////////////////////////////////
// Called by DllMain when this DLL is unloading.
//////////////////////////////////////////////////////////////////////*/
void DllUnloadingCheck ( void )
{
    // Call shutdown in case I'm loaded in the interop case.
    if ( ( FALSE == g_ShutdownProcessed ) && ( NULL != g_pExceptMon ) )
    {
        g_pExceptMon->Shutdown ( ) ;
    }
    if ( NULL != g_pExceptMon )
    {
        delete g_pExceptMon ;
        g_pExceptMon = NULL ;
    }
}