/*//////////////////////////////////////////////////////////////////////
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright (c) 1997-2003 John Robbins -- All rights reserved.
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "BaseProfilerCallback.h"

// Disable the unreference formal parameter error.
#pragma warning ( disable : 4100 )

ULONG CBaseProfilerCallback :: AddRef ( )
{
    InterlockedIncrement ( &m_lRefCount ) ;
    return ( m_lRefCount ) ;
}

ULONG CBaseProfilerCallback :: Release ( )
{
    if ( 0 == InterlockedDecrement ( &m_lRefCount ) )
    {
        delete this ;
    }
    return ( m_lRefCount ) ;
}

HRESULT CBaseProfilerCallback :: QueryInterface ( REFIID  riid        ,
                                                  void ** ppInterface  )
{
    return ( E_NOTIMPL ) ;
}

/*/////////////////////////////////////////////////////////////////////
// All the ICorProfilerCallback methods.
/////////////////////////////////////////////////////////////////////*/

HRESULT CBaseProfilerCallback :: OnInitialize ( )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                          Initialize ( IUnknown * pICorProfilerInfoUnk )
{
    HRESULT hr =
        pICorProfilerInfoUnk->
                QueryInterface ( IID_ICorProfilerInfo           ,
                                 (LPVOID *)&m_pICorProfilerInfo  ) ;
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( FAILED(hr) )
    {
        return ( E_INVALIDARG ) ;
    }
    
    m_pICorProfilerInfo->AddRef ( ) ;
    
    hr = OnInitialize ( ) ;

    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: OnShutdown ( )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: Shutdown ( )
{
    HRESULT hr = OnShutdown ( ) ;
    
    m_pICorProfilerInfo->Release ( ) ;
    
    return ( hr ) ;
}

HRESULT CBaseProfilerCallback ::
                    AppDomainCreationStarted ( AppDomainID appDomainId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    AppDomainCreationFinished ( AppDomainID appDomainId,
                                                HRESULT     hrStatus   )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    AppDomainShutdownStarted ( AppDomainID appDomainId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    AppDomainShutdownFinished ( AppDomainID appDomainId,
                                                HRESULT     hrStatus   )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    AssemblyLoadStarted ( AssemblyID assemblyId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    AssemblyLoadFinished ( AssemblyID assemblyId ,
                                           HRESULT    hrStatus    )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    AssemblyUnloadStarted ( AssemblyID assemblyId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    AssemblyUnloadFinished ( AssemblyID assemblyId ,
                                             HRESULT hrStatus      )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ModuleLoadStarted ( ModuleID moduleId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    ModuleLoadFinished ( ModuleID moduleId ,
                                         HRESULT  hrStatus  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    ModuleUnloadStarted ( ModuleID moduleId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    ModuleUnloadFinished ( ModuleID moduleId ,
                                           HRESULT  hrStatus  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    ModuleAttachedToAssembly ( ModuleID moduleId   ,
                                               UINT     assemblyId  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ClassLoadStarted ( ClassID classId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ClassLoadFinished ( ClassID classId  ,
                                                     HRESULT hrStatus  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ClassUnloadStarted ( ClassID classId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ClassUnloadFinished ( ClassID classId ,
                                                       HRESULT hrStatus)
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    FunctionUnloadStarted ( FunctionID functionId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    JITCompilationStarted ( FunctionID functionId,
                                            BOOL       fIsSafeToBlock )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    JITCompilationFinished ( FunctionID functionId ,
                                             HRESULT    hrStatus   ,
                                             BOOL       fIsSafeToBlock )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
        JITCachedFunctionSearchStarted ( FunctionID functionId,
                                         BOOL *     pbUseCachedFunction)
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
        JITCachedFunctionSearchFinished ( FunctionID        functionId ,
                                          COR_PRF_JIT_CACHE result     )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    JITFunctionPitched ( FunctionID functionId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    JITInlining ( FunctionID callerId       ,
                                  FunctionID calleeId       ,
                                  BOOL *     pfShouldInline  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ThreadCreated ( ThreadID threadId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ThreadDestroyed ( ThreadID threadId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    ThreadAssignedToOSThread ( ThreadID managedThreadId,
                                               ULONG    osThreadId     )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: RemotingClientInvocationStarted  ( )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    RemotingClientSendingMessage ( GUID * pCookie ,
                                                   BOOL fIsAsync   )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    RemotingClientReceivingReply ( GUID * pCookie  ,
                                                   BOOL   fIsAsync  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: RemotingClientInvocationFinished ( )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    RemotingServerReceivingMessage ( GUID * pCookie  ,
                                                     BOOL   fIsAsync  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: RemotingServerInvocationStarted ( )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: RemotingServerInvocationReturned ( )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    RemotingServerSendingReply ( GUID * pCookie  ,
                                                 BOOL   fIsAsync  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
    UnmanagedToManagedTransition ( FunctionID                functionId,
                                   COR_PRF_TRANSITION_REASON reason    )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
    ManagedToUnmanagedTransition ( FunctionID                functionId,
                                   COR_PRF_TRANSITION_REASON reason    )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
        RuntimeSuspendStarted ( COR_PRF_SUSPEND_REASON suspendReason )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: RuntimeSuspendFinished ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: RuntimeSuspendAborted ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: RuntimeResumeStarted ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: RuntimeResumeFinished ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                    RuntimeThreadSuspended ( ThreadID threadId )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                    RuntimeThreadResumed ( ThreadID threadId )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                MovedReferences ( ULONG cMovedObjectIDRanges    ,
                                  UINT  oldObjectIDRangeStart[] ,
                                  UINT  newObjectIDRangeStart[] ,
                                  ULONG cObjectIDRangeLength[]   )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: ObjectAllocated ( ObjectID objectId ,
                                                   ClassID  classId   )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                    ObjectsAllocatedByClass ( ULONG   cClassCount ,
                                              ClassID classIds[]  ,
                                              ULONG   cObjects[]   )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                    ObjectReferences ( ObjectID objectId       ,
                                       ClassID  classId        ,
                                       ULONG    cObjectRefs    ,
                                       ObjectID objectRefIds[]  )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                    RootReferences ( ULONG    cRootRefs    ,
                                     ObjectID rootRefIds[]  )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                    ExceptionThrown ( ObjectID thrownObjectId )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                ExceptionSearchFunctionEnter ( FunctionID functionId )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: ExceptionSearchFunctionLeave ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                ExceptionSearchFilterEnter ( FunctionID functionId )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: ExceptionSearchFilterLeave ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                ExceptionSearchCatcherFound ( FunctionID functionId )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                    ExceptionOSHandlerEnter ( UINT_PTR unused )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                    ExceptionOSHandlerLeave ( UINT_PTR unused )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                ExceptionUnwindFunctionEnter ( FunctionID functionId )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: ExceptionUnwindFunctionLeave ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                ExceptionUnwindFinallyEnter ( FunctionID functionId )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: ExceptionUnwindFinallyLeave ( )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback ::
                ExceptionCatcherEnter ( FunctionID functionId ,
                                        ObjectID   objectId    )
{
    return ( S_OK ) ;
}
HRESULT CBaseProfilerCallback :: ExceptionCatcherLeave ( )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                COMClassicVTableCreated ( ClassID wrappedClassId ,
                                          REFGUID implementedIID ,
                                          VOID *  pUnk           ,
                                          ULONG   cSlots          )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback ::
                COMClassicVTableDestroyed ( ClassID wrappedClassId ,
                                            REFGUID implementedIID ,
                                            VOID *  pUnk            )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ExceptionCLRCatcherFound ( void )
{
    return ( S_OK ) ;
}

HRESULT CBaseProfilerCallback :: ExceptionCLRCatcherExecute ( void )
{
    return ( S_OK ) ;
}

/*/////////////////////////////////////////////////////////////////////
// General helper methods common to all profilers.
/////////////////////////////////////////////////////////////////////*/

BOOL CBaseProfilerCallback ::
            GetClassAndMethodFromFunctionId ( FunctionID uiFunctionId ,
                                              LPWSTR     szClass      ,
                                              UINT       uiClassLen   ,
                                              LPWSTR     szMethod     ,
                                              UINT       uiMethodLen   )
{
    // The magic of metadata is how I'll find this information.

    // The return value.
    BOOL bRet = FALSE ;
    
    // The token for the function id.
    mdToken MethodMetaToken = 0 ;
    // The metadata interface.
    IMetaDataImport * pIMetaDataImport = NULL ;

    // Ask ICorProfilerInfo for the metadata interface for this
    // functionID
    HRESULT hr = m_pICorProfilerInfo->
                GetTokenAndMetaDataFromFunction ( uiFunctionId        ,
                                                  IID_IMetaDataImport ,
                                     (IUnknown**) &pIMetaDataImport   ,
                                                  &MethodMetaToken    );
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        // The token for the class.
        mdTypeDef ClassMetaToken ;
        // The total chars copies.
        ULONG ulCopiedChars ;
        
        // Look up the method information from the metadata.
        hr = pIMetaDataImport->GetMethodProps ( MethodMetaToken ,
                                                &ClassMetaToken ,
                                                szMethod        ,
                                                uiMethodLen     ,
                                                &ulCopiedChars  ,
                                                NULL            ,
                                                NULL            ,
                                                NULL            ,
                                                NULL            ,
                                                NULL             ) ;
        ASSERT ( SUCCEEDED ( hr ) ) ;
        ASSERT ( ulCopiedChars < uiMethodLen ) ;
        if ( ( SUCCEEDED ( hr )             ) &&
             ( ulCopiedChars < uiMethodLen )   )
        {
            // Armed with the class meta data token, I can look up the
            // class.
            hr = pIMetaDataImport->GetTypeDefProps ( ClassMetaToken ,
                                                     szClass        ,
                                                     uiClassLen     ,
                                                     &ulCopiedChars ,
                                                     NULL           ,
                                                     NULL            ) ;
            ASSERT ( SUCCEEDED ( hr ) ) ;
            ASSERT ( ulCopiedChars < uiClassLen ) ;
            if ( ( SUCCEEDED ( hr )           ) &&
                 ( ulCopiedChars < uiClassLen )   )
            {
                bRet = TRUE ;
            }
            else
            {
                bRet = FALSE ;
            }
        }
        else
        {
            bRet = FALSE ;
        }
        pIMetaDataImport->Release ( ) ;
    }
    else
    {
        bRet = FALSE ;
    }

    return ( bRet ) ;
}

BOOL CBaseProfilerCallback ::
                GetClassFromObjectId ( ObjectID  uiObjectId ,
                                       LPWSTR    szClass    ,
                                       UINT      uiClassLen  )
{
    // The return value.
    BOOL bRet = FALSE ;
    
    // Get the class meta data token.
    ClassID ClsID ;
    
    HRESULT hr = m_pICorProfilerInfo->
                            GetClassFromObject ( uiObjectId ,
                                                 &ClsID      ) ;
    ASSERT ( SUCCEEDED ( hr ) ) ;
    if ( SUCCEEDED ( hr ) )
    {
        
        // Get the class module ID and metadata token.
        ModuleID ModID ;
        mdTypeDef ClassMetaToken ;
        
        hr = m_pICorProfilerInfo->GetClassIDInfo ( ClsID           ,
                                                   &ModID          ,
                                                   &ClassMetaToken  ) ;

        ASSERT ( SUCCEEDED ( hr ) ) ;
        if ( SUCCEEDED ( hr ) )
        {
            // Now that I have the module ID, I can ask for the
            // metadata interface.
            IMetaDataImport * pIMetaDataImport = NULL ;

            hr = m_pICorProfilerInfo->
                    GetModuleMetaData ( ModID                         ,
                                        ofRead                        ,
                                        IID_IMetaDataImport           ,
                                        (IUnknown**)&pIMetaDataImport );
            ASSERT ( SUCCEEDED ( hr ) ) ;
            if ( SUCCEEDED ( hr ) )
            {

                ULONG ulCopiedChars ;
                hr = pIMetaDataImport->
                                    GetTypeDefProps ( ClassMetaToken ,
                                                      szClass        ,
                                                      uiClassLen     ,
                                                      &ulCopiedChars ,
                                                      NULL           ,
                                                      NULL           );
                ASSERT ( SUCCEEDED ( hr ) ) ;
                ASSERT ( ulCopiedChars < uiClassLen ) ;
                if ( ( SUCCEEDED ( hr )            ) &&
                     ( ulCopiedChars < uiClassLen )   )
                {
                    bRet = TRUE ;
                }
                else
                {
                    bRet = FALSE ;
                }
                
                pIMetaDataImport->Release ( ) ;
            }
            else
            {
                bRet = FALSE ;
            }
        }
        else
        {
            bRet = FALSE ;
        }
    }
    else
    {
        bRet = FALSE ;
    }
                                             
    return ( bRet ) ;
}

BOOL CBaseProfilerCallback ::
            SupposedToProfileThisProcess ( LPCWSTR szEnvVar )
{
    BOOL bRet = FALSE ;
    
    wchar_t szEnvProcName [ MAX_PATH ] ;
    
    if ( FALSE == ::GetEnvironmentVariable ( szEnvVar      ,
                                             szEnvProcName ,
                                             MAX_PATH       ) )
    {
        // The environment var does not exist so I'm supposed to do
        // everything.
        bRet = TRUE ;
    }
    else
    {
        // Gotta do the checking.
        
        // The first check will look to see if there's an exact match
        // with the process name with full path.  That way the user can
        // specify the whole path and directory in case there are
        // multiple instances they want to run.
        wchar_t szFullMod[ MAX_PATH ] ;
        DWORD dwRetVal = ::GetModuleFileName ( NULL       ,
                                               szFullMod  ,
                                               MAX_PATH    ) ;
        ASSERT ( 0 != dwRetVal ) ;
        if ( 0 != dwRetVal )
        {
            
            if ( 0 == _tcsicmp ( szEnvProcName , szFullMod ) )
            {
                // Got a match.
                bRet = TRUE ;
            }
            else
            {
                // Pull off the file and extension and check it.
                wchar_t * pNameStart = _tcsrchr ( szFullMod   ,
                                                  _T ( '\\' )  ) ;
                if ( NULL == pNameStart )
                {
                    pNameStart = szFullMod ;
                }
                else
                {
                    pNameStart++ ;
                }
                if ( 0 == _tcsicmp ( pNameStart , szEnvProcName ) )
                {
                    bRet = TRUE ;
                }
            }
            
        }
        
    }
    return ( bRet ) ;
}
