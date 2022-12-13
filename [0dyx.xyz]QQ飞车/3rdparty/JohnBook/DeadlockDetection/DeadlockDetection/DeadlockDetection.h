/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The TextFile DeadlockDetection Main Header
----------------------------------------------------------------------*/

#ifndef _DEADLOCKDETECTION_H
#define _DEADLOCKDETECTION_H

#ifdef __cplusplus
extern "C" {
#endif  // _cplusplus

/*//////////////////////////////////////////////////////////////////////
                            Special Defines
//////////////////////////////////////////////////////////////////////*/
// The defines that set up how the functions or classes are exported or
// imported.
#ifndef DEADDET_DLLINTERFACE
#ifdef BUILDING_DEADDET_DLL
#define DEADDET_DLLINTERFACE __declspec ( dllexport )
#else
#define DEADDET_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_DEADDET_DLL
#endif  // DEADDET_DLLINTERFACE


/*//////////////////////////////////////////////////////////////////////
                        Enumeration Definitions
//////////////////////////////////////////////////////////////////////*/
// The identifiers for the functions/directives for passed in the eFunc
// field of the DDEVENTINFO structure.
// IF YOU CHANGE THE ORDER OF THESE ENUMS YOU WILL BREAK THINGS.
typedef enum tag_eFuncEnum
{
    // The special uninitialized value.
    eUNINITIALIZEDFE = 0 ,

    // Waiting and special functions that are always reported.
    eExitProcess                            ,
    eWaitForSingleObject                    ,
    eWaitForSingleObjectEx                  ,
    eWaitForMultipleObjects                 ,
    eWaitForMultipleObjectsEx               ,
    eMsgWaitForMultipleObjects              ,
    eMsgWaitForMultipleObjectsEx            ,
    eSignalObjectAndWait                    ,
    eCloseHandle                            ,
    eGetProcAddress                         ,

    // Thread specific functions.
    eCreateThread                           ,
    eExitThread                             ,
    eSuspendThread                          ,
    eResumeThread                           ,
    eTerminateThread                        ,
    eFreeLibraryAndExitThread               ,
    e_beginthreadex                         ,
    e_endthreadex                           ,
    e_beginthread                           ,
    e_endthread                             ,

    // CriticalSection functions.
    eInitializeCriticalSection              ,
    eInitializeCriticalSectionAndSpinCount  ,
    eDeleteCriticalSection                  ,
    eEnterCriticalSection                   ,
    eLeaveCriticalSection                   ,
    eSetCriticalSectionSpinCount            ,
    eTryEnterCriticalSection                ,

    // Mutex functions.
    eCreateMutexA                           ,
    eCreateMutexW                           ,
    eOpenMutexA                             ,
    eOpenMutexW                             ,
    eReleaseMutex                           ,

    // Semaphore functions.
    eCreateSemaphoreA                       ,
    eCreateSemaphoreW                       ,
    eOpenSemaphoreA                         ,
    eOpenSemaphoreW                         ,
    eReleaseSemaphore                       ,

    // Event functions.
    eCreateEventA                           ,
    eCreateEventW                           ,
    eOpenEventA                             ,
    eOpenEventW                             ,
    ePulseEvent                             ,
    eResetEvent                             ,
    eSetEvent                               ,

    // The maximum event number.
    eMAXFUNCTIONENUM                        ,

} eFuncEnum ;

////////////////////////////////////////////////////////////////////////
// The identifier that indicates if the event information packet is
// for before the call or after the call completed.
typedef enum tag_ePrePostEnum
{
    eUNINITIALIZEDPP = 0    ,
    ePreCall                ,
    ePostCall               ,
    eMAXPREPOSTENUM
} ePrePostEnum ;

/*//////////////////////////////////////////////////////////////////////
                         Structure Definitions
//////////////////////////////////////////////////////////////////////*/
// The structure that is passed from the core DeadlockDetection.DLL to
// the attached extension DLL.  This describes the thread or
// synchronization function being executed.
typedef struct tagDDEVENTINFO
{
    // The identifier that specifies what is contained in the rest of
    // this structure.
    eFuncEnum    eFunc          ;
    // The pre-call or past-call indicator.
    ePrePostEnum ePrePost       ;
    // The return address.  This is so the calling function can be
    // found.
    DWORD        dwAddr         ;
    // The thread ID of the calling thread.
    DWORD        dwThreadId     ;
    // The return value for post calls.
    DWORD        dwRetValue     ;
    // The parameter information.  Cast this to the appropriate
    // structure as, described below, for the function.  When accessing
    // the parameters, treat them as read only.
    DWORD        dwParams       ;
} DDEVENTINFO , * LPDDEVENTINFO ;


////////////////////////////////////////////////////////////////////////
// To make processing the parameters for an individual easier here are
// the structures for each of the intercepted functions.  Just cast the
// dwParams structure like the following:
// LPCREATETHREAD_PARAMS pData = (LPCREATETHREAD_PARAMS)stEvt.dwParams ;

typedef struct tagEXITPROCESS_PARAMS
{
    UINT uExitCode ;
} EXITPROCESS_PARAMS , * LPEXITPROCESS_PARAMS ;

typedef struct tagCREATETHREAD_PARAMS
{
    LPSECURITY_ATTRIBUTES   lpThreadAttributes  ;
    DWORD                   dwStackSize         ;
    LPTHREAD_START_ROUTINE  lpStartAddress      ;
    LPVOID                  lpParameter         ;
    DWORD                   dwCreationFlags     ;
    LPDWORD                 lpThreadId          ;
} CREATETHREAD_PARAMS , * LPCREATETHREAD_PARAMS ;

typedef struct tagEXITTHREAD_PARAMS
{
    DWORD dwExitCode ;
} EXITTHREAD_PARAMS , * LPEXITTHREAD_PARAMS ;

typedef struct tagHANDLE_PARAMS
{
    HANDLE hHandle ;
} HANDLE_PARAMS , * LPHANDLE_PARAMS ;

typedef HANDLE_PARAMS SUSPENDTHREAD_PARAMS ;
typedef HANDLE_PARAMS * LPSUSPENDTHREAD_PARAMS ;

typedef HANDLE_PARAMS RESUMETHREAD_PARAMS ;
typedef HANDLE_PARAMS * LPRESUMETHREAD_PARAMS ;

typedef struct tagTERMINATETHREAD_PARAMS
{
    HANDLE hThread    ;
    DWORD  dwExitCode ;
} TERMINATETHREAD_PARAMS , * LPTERMINATETHREAD_PARAMS ;

typedef struct tagFREELIBRARYANDEXITTHREAD_PARAMS
{
    HMODULE hModule    ;
    DWORD   dwExitCode ;
} FREELIBRARYANDEXITTHREAD_PARAMS , * LPFREELIBRARYANDEXITTHREAD_PARAMS;

typedef CREATETHREAD_PARAMS BEGINTHREADEX_PARAMS ;
typedef LPCREATETHREAD_PARAMS * LPBEGINTHREADEX_PARAMS ;

typedef struct tag_BEGINTHREAD_PARAMS
{
    LPVOID   start_address ;
    unsigned stack_size    ;
    void *   arglist       ;
} BEGINTHREAD_PARAMS , * LPBEGINTHREAD_PARAMS ;

typedef EXITTHREAD_PARAMS ENDTHREADEX_PARAMS ;
typedef LPEXITTHREAD_PARAMS * LPENDTHREADEX_PARAMS ;

typedef struct tagWAITFORSINGLEOBJECT_PARAM
{
    HANDLE hHandle          ;
    DWORD  dwMilliseconds   ;
} WAITFORSINGLEOBJECT_PARAMS , * LPWAITFORSINGLEOBJECT_PARAMS ;

typedef struct tagWAITFORSINGLEOBJECTEX_PARAM
{
    HANDLE hHandle          ;
    DWORD  dwMilliseconds   ;
    BOOL   bAlertable       ;
} WAITFORSINGLEOBJECTEX_PARAMS , * LPWAITFORSINGLEOBJECTEX_PARAMS ;

typedef struct tagWAITFORMULTIPLEOBJECTS_PARAM
{
    DWORD           nCount          ;
    CONST HANDLE *  lpHandles       ;
    BOOL            bWaitAll        ;
    DWORD           dwMilliseconds  ;
} WAITFORMULTIPLEOBJECTS_PARAMS , * LPWAITFORMULTIPLEOBJECTS_PARAMS ;

typedef struct tagWAITFORMULTIPLEOBJECTSEX_PARAM
{
    DWORD           nCount          ;
    CONST HANDLE *  lpHandles       ;
    BOOL            bWaitAll        ;
    DWORD           dwMilliseconds  ;
    BOOL            bAlertable      ;
} WAITFORMULTIPLEOBJECTSEX_PARAMS , * LPWAITFORMULTIPLEOBJECTSEX_PARAMS;

typedef struct tagMSGWAITFORMULTIPLEOBJECTS_PARAM
{
    DWORD       nCount          ;
    LPHANDLE    pHandles        ;
    BOOL        fWaitAll        ;
    DWORD       dwMilliseconds  ;
    DWORD       dwWakeMask      ;
} MSGWAITFORMULTIPLEOBJECTS_PARAMS ,
    * LPMSGWAITFORMULTIPLEOBJECTS_PARAMS ;

typedef struct tagMSGWAITFORMULTIPLEOBJECTSEX_PARAM
{
    DWORD       nCount          ;
    LPHANDLE    pHandles        ;
    BOOL        fWaitAll        ;
    DWORD       dwMilliseconds  ;
    DWORD       dwWakeMask      ;
    DWORD       dwFlags         ;
} MSGWAITFORMULTIPLEOBJECTSEX_PARAMS ,
    * LPMSGWAITFORMULTIPLEOBJECTSEX_PARAMS ;

typedef struct tagSIGNALOBJECTANDWAIT_PARAM
{
    HANDLE hObjectToSignal  ;
    HANDLE hObjectToWaitOn  ;
    DWORD  dwMilliseconds   ;
    BOOL   bAlertable       ;
} SIGNALOBJECTANDWAIT_PARAMS , * LPSIGNALOBJECTANDWAIT_PARAMS ;

typedef HANDLE_PARAMS CLOSEHANDLE_PARAMS ;
typedef HANDLE_PARAMS * LPCLOSEHANDLE_PARAMS ;

typedef struct tagGETPROCADDRESS_PARAMS
{
    HMODULE hModule     ;
    LPCSTR  lpProcName  ;
} GETPROCADDRESS_PARAMS , * LPGETPROCADDRESS_PARAMS ;

typedef struct tagCRITSEC_PARAMS
{
    LPCRITICAL_SECTION lpCriticalSection ;
} CRITSEC_PARAMS , * LPCRITSEC_PARAMS ;

typedef CRITSEC_PARAMS INITIALIZECRITICALSECTION_PARAMS ;
typedef CRITSEC_PARAMS * LPINITIALIZECRITICALSECTION_PARAMS ;

typedef struct tagINITIALIZECRITICALSECTIONANDSPINCOUNT_PARAM
{
    LPCRITICAL_SECTION lpCriticalSection ;
    DWORD              dwSpinCount       ;
} INITIALIZECRITICALSECTIONANDSPINCOUNT_PARAMS ,
                      * LPINITIALIZECRITICALSECTIONANDSPINCOUNT_PARAMS ;

typedef CRITSEC_PARAMS DELETECRITICALSECTION_PARAMS ;
typedef CRITSEC_PARAMS * LPDELETECRITICALSECTION_PARAMS ;

typedef CRITSEC_PARAMS ENTERCRITICALSECTION_PARAMS ;
typedef CRITSEC_PARAMS * LPENTERCRITICALSECTION_PARAMS ;

typedef CRITSEC_PARAMS LEAVECRITICALSECTION_PARAMS ;
typedef CRITSEC_PARAMS * LPLEAVECRITICALSECTION_PARAMS ;

typedef INITIALIZECRITICALSECTIONANDSPINCOUNT_PARAMS
                        SETCRITICALSECTIONSPINCOUNT_PARAMS ;
typedef INITIALIZECRITICALSECTIONANDSPINCOUNT_PARAMS
                        * LPSETCRITICALSECTIONSPINCOUNT_PARAMS ;

typedef CRITSEC_PARAMS TRYENTERCRITICALSECTION_PARAMS ;
typedef CRITSEC_PARAMS * LPTRYENTERCRITICALSECTION_PARAMS ;

typedef struct tagCREATEMUTEXA_PARAM
{
    LPSECURITY_ATTRIBUTES lpMutexAttributes ;
    BOOL                  bInitialOwner     ;
    LPCSTR                lpName            ;
} CREATEMUTEXA_PARAMS , * LPCREATEMUTEXA_PARAMS ;

typedef struct tagCREATEMUTEXW_PARAM
{
    LPSECURITY_ATTRIBUTES lpMutexAttributes ;
    BOOL                  bInitialOwner     ;
    LPCWSTR               lpName            ;
} CREATEMUTEXW_PARAMS , * LPCREATEMUTEXW_PARAMS ;

typedef struct tagOPENMUTEXA_PARAM
{
    DWORD   dwDesiredAccess ;
    BOOL    bInheritHandle  ;
    LPCSTR  lpName          ;
} OPENMUTEXA_PARAMS , * LPOPENMUTEXA_PARAMS ;

typedef struct tagOPENMUTEXW_PARAM
{
    DWORD   dwDesiredAccess ;
    BOOL    bInheritHandle  ;
    LPCWSTR lpName          ;
} OPENMUTEXW_PARAMS , * LPOPENMUTEXW_PARAMS ;

typedef struct tagRELEASEMUTEX_PARAM
{
    HANDLE hMutex ;
} RELEASEMUTEX_PARAMS , * LPRELEASEMUTEX_PARAMS ;

typedef struct tagCREATESEMAPHOREA_PARAM
{
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes ;
    LONG                  lInitialCount         ;
    LONG                  lMaximumCount         ;
    LPCSTR                lpName                ;
} CREATESEMAPHOREA_PARAMS , * LPCREATESEMAPHOREA_PARAMS ;

typedef struct tagCREATESEMAPHOREW_PARAM
{
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes ;
    LONG                  lInitialCount         ;
    LONG                  lMaximumCount         ;
    LPCWSTR               lpName                ;
} CREATESEMAPHOREW_PARAMS , * LPCREATESEMAPHOREW_PARAMS ;

typedef OPENMUTEXA_PARAMS OPENSEMAPHOREA_PARAMS ;
typedef OPENMUTEXA_PARAMS * LPOPENSEMAPHOREA_PARAMS ;

typedef OPENMUTEXW_PARAMS OPENSEMAPHOREW_PARAMS ;
typedef OPENMUTEXW_PARAMS * LPOPENSEMAPHOREW_PARAMS ;

typedef struct tagRELEASESEMAPHORE_PARAM
{
    HANDLE hSemaphore       ;
    LONG   lReleaseCount    ;
    LPLONG lpPreviousCount  ;
} RELEASESEMAPHORE_PARAMS , * LPRELEASESEMAPHORE_PARAMS ;

typedef struct tagCREATEEVENTA_PARAM
{
    LPSECURITY_ATTRIBUTES lpEventAttributes ;
    BOOL                  bManualReset      ;
    BOOL                  bInitialState     ;
    LPCSTR                lpName            ;
} CREATEEVENTA_PARAMS , * LPCREATEEVENTA_PARAMS ;

typedef struct tagCREATEEVENTW_PARAM
{
    LPSECURITY_ATTRIBUTES lpEventAttributes ;
    BOOL                  bManualReset      ;
    BOOL                  bInitialState     ;
    LPCWSTR               lpName            ;
} CREATEEVENTW_PARAMS , * LPCREATEEVENTW_PARAMS ;

typedef OPENMUTEXA_PARAMS OPENEVENTA_PARAMS ;
typedef OPENMUTEXA_PARAMS * LPOPENEVENTA_PARAMS ;

typedef OPENMUTEXW_PARAMS OPENEVENTW_PARAMS ;
typedef OPENMUTEXW_PARAMS * LPOPENEVENTW_PARAMS ;

typedef struct tagPULSEEVENT_PARAM
{
    HANDLE hEvent ;
} PULSEEVENT_PARAMS , * LPPULSEEVENT_PARAMS ;


typedef PULSEEVENT_PARAMS RESETEVENT_PARAMS ;
typedef PULSEEVENT_PARAMS * LPRESETEVENT_PARAMS ;

typedef PULSEEVENT_PARAMS SETEVENT_PARAMS ;
typedef PULSEEVENT_PARAMS * LPSETEVENT_PARAMS ;

/*//////////////////////////////////////////////////////////////////////
                                Defines
//////////////////////////////////////////////////////////////////////*/
// The option defines that specify what classes of functions will be
// logged by DeadlockDetection.DLL.  These are specified to
// OpenDeadlockDetection as the initial settings and to
// ChangeDeadlockDetection to change settings on the fly.
// Alternatively, the starting options could be set in
// DeadlockDetection.ini

// Placebo flag for Wait* functions.  Yes, this is zero and thus will
// always be logged.  However, to make defining the hook macros easier,
// having the DDOPT_WAIT flag keeps things orthoginal!
#define DDOPT_WAIT          0x00000000
// Turns on Thread function logging.
#define DDOPT_THREADS       0x00000001
// Turns on CriticalSection function logging.
#define DDOPT_CRITSEC       0x00000002
// Turns on Mutex function logging.
#define DDOPT_MUTEX         0x00000004
// Turns on Semaphore function logging.
#define DDOPT_SEMAPHORE     0x00000010
// Turns on Event function logging.
#define DDOPT_EVENT         0x00000020

// Turns on all function logging.
#define DDOPT_ALL ( DDOPT_THREADS   | \
                    DDOPT_CRITSEC   | \
                    DDOPT_MUTEX     | \
                    DDOPT_SEMAPHORE | \
                    DDOPT_EVENT         )

// The extension DLL functions that are retrieved through
// GetProcAddress.
#define FN_DEADDETEXTOPEN           "DeadDetExtOpen"
#define FN_DEADDETEXTCLOSE          "DeadDetExtClose"
#define FN_DEADDETEXTPROCESSEVENT   "DeadDetProcessEvent"
#define FN_DEADDETWRITETOLOG        "DeadDetWriteToLog"

/*//////////////////////////////////////////////////////////////////////
                                Typedefs
//////////////////////////////////////////////////////////////////////*/
// The typedefs for the extension DLL required functions.
typedef BOOL (WINAPI *PFNDEADDETEXTOPEN) ( void ) ;
typedef void (WINAPI *PFNDEADDETEXTCLOSE) ( void ) ;
typedef BOOL (WINAPI *PFNDEADDETEXTPROCESSEVENT)
                                      ( const LPDDEVENTINFO pEvtInfo ) ;
typedef BOOL (WINAPI *PFNDEADDETWRITETOLOG) ( LPCTSTR szMsg ) ;

/*//////////////////////////////////////////////////////////////////////
               DeadLockDetection Main Function Prototypes
//////////////////////////////////////////////////////////////////////*/
/*----------------------------------------------------------------------
FUNCTION        :   OpenDeadlockDetection
DISCUSSION      :
    Starts the deadlock detection system running.  The extension DLL
will be loaded out of DeadLockDetection.ini, key: [Initialization],
value: ExtDll.  The INI file needs to be in the same directory as
DeadLockDetection.DLL.  If the extension DLL cannot be loaded, then the
file DeadDetEXT.DLL will be loaded.
    The dwOpts parameter can be set to DDOPT_SUSPEND in order to start
suspended so logging does not start until needed.
    If the INI key: [Initialization], value: StartInDllMain is set to 1
this function will be called in the DllMain for the
DeadLockDetection.DLL.  This can get the deadlock detection started a
bit sooner.  The startup options, in this case will be read out of the
INI file key [Initialization], value InitialOpts.
PARAMETERS      :
    dwOpts - One or more of the DDOPT_* values OR'd together to set the
             initial settings for the deadlock detection system.
RETURNS         :
    0 - There was a problem starting up.  If GetLastError returns
        ERROR_ALREADY_INITIALIZED then OpenDeadlockDetection has
        already been successfully called.  If not, then there was a
        fatal error and you will see message boxes indicating the
        error.
    1 - The deadlock detection system was fully initialized.
----------------------------------------------------------------------*/
DEADDET_DLLINTERFACE BOOL WINAPI OpenDeadlockDetection ( DWORD dwOpts );

/*----------------------------------------------------------------------
FUNCTION        :   CloseDeadlockDetection
DISCUSSION      :
    Shuts down the deadlock detection system.  Completely unhooks all
intercepted functions.
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
DEADDET_DLLINTERFACE void WINAPI CloseDeadlockDetection ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        :   GetDeadlockDetectionOptions
DISCUSSION      :
    Returns the currently selected deadlock system options.
PARAMETERS      :
    None.
RETURNS         :
    A DWORD value of the OR'd DDOPTS_* flags.
----------------------------------------------------------------------*/
DEADDET_DLLINTERFACE DWORD WINAPI GetDeadlockDetectionOptions ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        :   SetDeadlockDetectionOptions
DISCUSSION      :
    Sets the current deadlock system options.  The dwNewOpts parameter
must be a combination of DDOPT_* values.
PARAMETERS      :
    dwNewOpts - The new DDOPT_* value.
RETURNS         :
    A DWORD value of the old DDOPT_* flags.
----------------------------------------------------------------------*/
DEADDET_DLLINTERFACE DWORD WINAPI
                      SetDeadlockDetectionOptions ( DWORD dwNewOpts  ) ;

/*----------------------------------------------------------------------
FUNCTION        :   SuspendDeadlockDetection
DISCUSSION      :
    Suspends all deadlock detection logging.
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
DEADDET_DLLINTERFACE void WINAPI SuspendDeadlockDetection ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        :   ResumeDeadlockDetection
DISCUSSION      :
    Resumes all deadlock detection logging.
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
DEADDET_DLLINTERFACE void WINAPI ResumeDeadlockDetection ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        :   WriteToDeadlockDetectionLog
DISCUSSION      :
    Writes a message to the DeadlockDetection extension module.
PARAMETERS      :
    szMsg - The string message.
RETURNS         :
    0 - There's a problem.
    1 - Life is good!
----------------------------------------------------------------------*/
DEADDET_DLLINTERFACE BOOL WINAPI
                         WriteToDeadlockDetectionLog ( LPCTSTR szMsg ) ;

/*//////////////////////////////////////////////////////////////////////
               DeadlockDetection Extension DLL Prototypes
//////////////////////////////////////////////////////////////////////*/
/*----------------------------------------------------------------------
FUNCTION        :   DeadDetExtOpen
DISCUSSION      :
    This function will be called as part of the OpenDeadlockDetection
processing so that the particular needs of the processing engine can
get initialized.
PARAMETERS      :
    None.
RETURNS         :
    0 - There was a problem so the whole OpenDeadlockDetection function
        will fail.
    1 - The initialization succeeded.
----------------------------------------------------------------------*/
BOOL WINAPI DeadDetExtOpen ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        :   DeadDetExtClose
DISCUSSION      :
    This function is called as part of the CloseDeadLockDetection
processing so that the processing engine can clean up and shutdown.
PARAMETERS      :
    None.
RETURNS         :
    None.
----------------------------------------------------------------------*/
void WINAPI DeadDetExtClose ( void ) ;

/*----------------------------------------------------------------------
FUNCTION        :   DeadDetProcessEvent
DISCUSSION      :
    The function that is called whenever one of the intercepted
functions is called.  The entire information is contained in the
pEvtInfo pointer.  The extension DLL can do what ever processing is
needed as long as no part of pEvtInfo is changed.
PARAMETERS      :
    pEvtInfo - The description of data going into the intercepted
               function.
RETURNS         :
    0 - There was a problem.  The deadlock detection system will shift
        to suspended mode.
    1 - The information was processed.
----------------------------------------------------------------------*/
BOOL WINAPI DeadDetProcessEvent ( const LPDDEVENTINFO pEvtInfo ) ;

/*----------------------------------------------------------------------
FUNCTION        :   DeadDetWriteToLog
DISCUSSION      :
    Called when the user wants to write a message to the logging
extension output
PARAMETERS      :
    szMsg - The message to write.
RETURNS         :
    0 - There was a problem.  The deadlock detection system will shift
        to suspended mode.
    1 - The information was processed.
----------------------------------------------------------------------*/
BOOL WINAPI DeadDetWriteToLog ( LPCTSTR szMsg ) ;

#ifdef __cplusplus
}
#endif  // _cplusplus


#endif  // _DEADLOCKDETECTION_H


