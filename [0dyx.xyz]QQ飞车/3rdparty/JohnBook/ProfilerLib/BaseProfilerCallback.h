/*//////////////////////////////////////////////////////////////////////
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright (c) 1997-2003 John Robbins -- All rights reserved.
//////////////////////////////////////////////////////////////////////*/
#ifndef _BASEPROFILERCALLBACK_H
#define _BASEPROFILERCALLBACK_H

#pragma once

class CBaseProfilerCallback : public ICorProfilerCallback
{
public  :
    CBaseProfilerCallback ( void )
    {
        m_lRefCount = 0 ;
        m_pICorProfilerInfo = NULL ;
    }
    STDMETHOD_ ( ULONG , AddRef) (  ) ;
    STDMETHOD_ ( ULONG , Release) (  ) ;
    STDMETHOD  ( QueryInterface) (  REFIID riid , void **ppInterface  );
    
    // The ProfilerLib versions of Intitialize and Shutdown.  This way
    // you don't have to worry about getting calling this base class.
    STDMETHOD ( OnInitialize ) ( ) ;
    STDMETHOD ( OnShutdown ) ( ) ;

    // YOU DO NOT NEED TO OVERRIDE Initialize.  IF YOU DO, YOU'LL HAVE
    // TO CALL THIS BASE CLASS YOURSELF.  Implement the OnInitialize
    // method, and it will be called by the base class after
    // everything's already initialized.
    STDMETHOD ( Initialize) ( IUnknown * pICorProfilerInfoUnk ) ;
    // The same goes for Shutdown.  Implement OnShutdown and things will
    // be happy.  If you need to manually shut down this class, (if your
    // profiler is unloaded without Shutdown being called), call this
    // method directly.
    STDMETHOD ( Shutdown ) (  ) ;
    STDMETHOD ( AppDomainCreationStarted ) ( AppDomainID appDomainId ) ;
    STDMETHOD ( AppDomainCreationFinished ) ( AppDomainID appDomainId ,
                                              HRESULT     hrStatus    );
    STDMETHOD ( AppDomainShutdownStarted ) ( AppDomainID appDomainId ) ;
    STDMETHOD ( AppDomainShutdownFinished ) ( AppDomainID appDomainId ,
                                              HRESULT     hrStatus    );
    STDMETHOD ( AssemblyLoadStarted ) ( AssemblyID assemblyId ) ;
    STDMETHOD ( AssemblyLoadFinished ) ( AssemblyID assemblyId ,
                                         HRESULT    hrStatus    ) ;
    STDMETHOD ( AssemblyUnloadStarted ) ( AssemblyID assemblyId ) ;
    STDMETHOD ( AssemblyUnloadFinished ) ( AssemblyID assemblyId ,
                                           HRESULT    hrStatus    ) ;
    STDMETHOD ( ModuleLoadStarted ) ( ModuleID moduleId ) ;
    STDMETHOD ( ModuleLoadFinished ) ( ModuleID moduleId ,
                                       HRESULT  hrStatus  ) ;
    STDMETHOD ( ModuleUnloadStarted ) ( ModuleID moduleId ) ;
    STDMETHOD ( ModuleUnloadFinished ) ( ModuleID moduleId ,
                                         HRESULT  hrStatus  ) ;
    STDMETHOD ( ModuleAttachedToAssembly ) ( ModuleID moduleId   ,
                                             UINT     assemblyId  ) ;
    STDMETHOD ( ClassLoadStarted ) ( ClassID classId ) ;
    STDMETHOD ( ClassLoadFinished ) ( ClassID classId  ,
                                      HRESULT hrStatus  ) ;
    STDMETHOD ( ClassUnloadStarted ) ( ClassID classId ) ;
    STDMETHOD ( ClassUnloadFinished ) ( ClassID classId  ,
                                        HRESULT hrStatus  ) ;
    STDMETHOD ( FunctionUnloadStarted ) ( FunctionID functionId ) ;
    STDMETHOD ( JITCompilationStarted ) ( FunctionID functionId     ,
                                          BOOL       fIsSafeToBlock  ) ;
    STDMETHOD ( JITCompilationFinished ) ( FunctionID functionId     ,
                                           HRESULT    hrStatus       ,
                                           BOOL       fIsSafeToBlock ) ;
    STDMETHOD ( JITCachedFunctionSearchStarted )
                                        ( FunctionID functionId      ,
                                          BOOL * pbUseCachedFunction ) ;
    STDMETHOD ( JITCachedFunctionSearchFinished )
                                        ( FunctionID functionId    ,
                                          COR_PRF_JIT_CACHE result ) ;
    STDMETHOD ( JITFunctionPitched ) ( FunctionID functionId ) ;
    STDMETHOD ( JITInlining ) ( FunctionID callerId       ,
                                FunctionID calleeId       ,
                                BOOL *     pfShouldInline ) ;
    STDMETHOD ( ThreadCreated ) ( ThreadID threadId ) ;
    STDMETHOD ( ThreadDestroyed ) ( ThreadID threadId ) ;
    STDMETHOD ( ThreadAssignedToOSThread ) ( ThreadID managedThreadId ,
                                             ULONG    osThreadId      );
    STDMETHOD ( RemotingClientInvocationStarted ) (  ) ;
    STDMETHOD ( RemotingClientSendingMessage ) ( GUID * pCookie ,
                                                 BOOL fIsAsync ) ;
    STDMETHOD ( RemotingClientReceivingReply ) ( GUID * pCookie ,
                                                 BOOL fIsAsync ) ;
    STDMETHOD ( RemotingClientInvocationFinished ) (  ) ;
    STDMETHOD ( RemotingServerReceivingMessage ) ( GUID * pCookie ,
                                                   BOOL fIsAsync ) ;
    STDMETHOD ( RemotingServerInvocationStarted ) (  ) ;
    STDMETHOD ( RemotingServerInvocationReturned ) (  ) ;
    STDMETHOD ( RemotingServerSendingReply ) ( GUID * pCookie ,
                                               BOOL fIsAsync ) ;
    STDMETHOD ( UnmanagedToManagedTransition ) ( FunctionID functionId ,
                                    COR_PRF_TRANSITION_REASON reason ) ;
    STDMETHOD ( ManagedToUnmanagedTransition ) ( FunctionID functionId ,
                                    COR_PRF_TRANSITION_REASON reason ) ;
    STDMETHOD ( RuntimeSuspendStarted ) (
                                COR_PRF_SUSPEND_REASON suspendReason ) ;
    STDMETHOD ( RuntimeSuspendFinished ) (  ) ;
    STDMETHOD ( RuntimeSuspendAborted ) (  ) ;
    STDMETHOD ( RuntimeResumeStarted ) (  ) ;
    STDMETHOD ( RuntimeResumeFinished ) (  ) ;
    STDMETHOD ( RuntimeThreadSuspended ) ( ThreadID threadId ) ;
    STDMETHOD ( RuntimeThreadResumed ) ( ThreadID threadId ) ;
    STDMETHOD ( MovedReferences ) ( ULONG cMovedObjectIDRanges   ,
                                    UINT oldObjectIDRangeStart[] ,
                                    UINT newObjectIDRangeStart[] ,
                                    ULONG cObjectIDRangeLength[]  ) ;
    STDMETHOD ( ObjectAllocated ) ( ObjectID objectId ,
                                    ClassID  classId   ) ;
    STDMETHOD ( ObjectsAllocatedByClass ) ( ULONG   cClassCount ,
                                            ClassID classIds[]  ,
                                            ULONG   cObjects[]   ) ;
    STDMETHOD ( ObjectReferences ) ( ObjectID objectId       ,
                                     ClassID  classId        ,
                                     ULONG    ObjectRefs     ,
                                     ObjectID objectRefIds[]  ) ;
    STDMETHOD ( RootReferences ) ( ULONG cRootRefs       ,
                                   ObjectID rootRefIds[]  ) ;
    STDMETHOD ( ExceptionThrown ) ( ObjectID thrownObjectId ) ;
    STDMETHOD ( ExceptionSearchFunctionEnter ) ( FunctionID functionId);
    STDMETHOD ( ExceptionSearchFunctionLeave ) ( ) ;
    STDMETHOD ( ExceptionSearchFilterEnter ) ( FunctionID functionId ) ;
    STDMETHOD ( ExceptionSearchFilterLeave ) (  ) ;
    STDMETHOD ( ExceptionSearchCatcherFound ) ( FunctionID functionId );
    STDMETHOD ( ExceptionOSHandlerEnter ) ( UINT_PTR unused ) ;
    STDMETHOD ( ExceptionOSHandlerLeave ) ( UINT_PTR unused ) ;
    STDMETHOD ( ExceptionUnwindFunctionEnter ) ( FunctionID functionId);
    STDMETHOD ( ExceptionUnwindFunctionLeave ) (  ) ;
    STDMETHOD ( ExceptionUnwindFinallyEnter ) ( FunctionID functionId );
    STDMETHOD ( ExceptionUnwindFinallyLeave ) (  ) ;
    STDMETHOD ( ExceptionCatcherEnter ) ( FunctionID functionId ,
                                          ObjectID   objectId    ) ;
    STDMETHOD ( ExceptionCatcherLeave ) (  ) ;
    STDMETHOD ( COMClassicVTableCreated ) ( ClassID wrappedClassId ,
                                            REFGUID implementedIID ,
                                            void *pVTable ,
                                            ULONG cSlots ) ;
    STDMETHOD ( COMClassicVTableDestroyed ) ( ClassID wrappedClassId ,
                                              REFGUID implementedIID ,
                                              void *pVTable ) ;
    STDMETHOD ( ExceptionCLRCatcherFound ) ( void ) ;
    STDMETHOD ( ExceptionCLRCatcherExecute ) ( void ) ;
    
protected   :
    
    // Helper methods useful for all profilers.
    
    // Given a function id, looks up the class name and method name.
    BOOL GetClassAndMethodFromFunctionId ( FunctionID uiFunctionId ,
                                           LPWSTR     szClass      ,
                                           UINT       uiClassLen   ,
                                           LPWSTR     szMethod     ,
                                           UINT       uiMethodLen   ) ;
                                           
    // Given an object id, returns the class name.
    BOOL GetClassFromObjectId ( ObjectID  uiObjectId ,
                                LPWSTR    szClass    ,
                                UINT      uiClassLen  ) ;
         
    // Looks for the environment variable string passed in and:
    // 1.  If environment var does not exist, returns TRUE as this
    //     implies you're willing to do all processes.
    // 2   If the environment var exists
    //      A.  If the value matches the complete path and filename
    //          of the currently executing process, returns TRUE
    //      B.  If the value matches just the filename and extension
    //          of the currently executing process, returns TRUE
    // 3.  All other cases return FALSE.
    // This function only looks for a single process name.  Override
    // it to allow multiple process names or other special processing.
    virtual BOOL SupposedToProfileThisProcess ( LPCWSTR szEnvVar ) ;
                                
                                               
protected   :
    LONG m_lRefCount ;

    ICorProfilerInfo * m_pICorProfilerInfo ;

} ;

#endif  // _BASEPROFILERCALLBACK_H

