/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The prototypes for all the hook functions and the prolog/epilog code
----------------------------------------------------------------------*/

#ifndef _DD_FUNCS_H
#define _DD_FUNCS_H

/*//////////////////////////////////////////////////////////////////////
    All the hook functions are __declspec(naked) functions, so I must
provide the prolog and epilog. I need to provide a custom prolog and
epilog for several reasons:
1. Functions written in C have no control over which registers are used
   or when the compiler saves the original registers. Not having control
   over the registers means that getting the return address is nearly
   impossible. For the DeadlockDetection project, the return address is
   critical.
2. I also wanted to hand the parameters to the extension DLL processing
   function without having to copy massive amounts of data on
   each function call.
3. Because almost all the hook functions behave the same way, I set up
   the common variables needed in all functions.
4. Hook functions can't change any of the return values, including
   the value from GetLastError. By doing my own prolog and epilog, I
   can make it much easier to return the correct value. Also,
   I need to restore the register values to the state they were in
   following the real function call.

A basic hook function only requires two macros, HOOKFN_STARTUP and
HOOKFN_SHUTDOWN.

As you can see, doing a regular function is quite easy!

BOOL NAKEDDEF DD_InitializeCriticalSectionAndSpinCount (
                                   LPCRITICAL_SECTION lpCriticalSection,
                                   DWORD              dwSpinCount      )
{
    HOOKFN_STARTUP ( eInitializeCriticalSectionAndSpinCount ,
                     DDOPT_CRITSEC                          ,
                     0                                       ) ;
                     
    InitializeCriticalSectionAndSpinCount ( lpCriticalSection ,
                                            dwSpinCount        ) ;
                                            
    HOOKFN_SHUTDOWN ( 2 , DDOPT_CRITSEC ) ;
}

If you have to do special processing and don't want to do something the
normal macros don't do, you can use the following macros to do the
necessary behind the scenes work:

HOOKFN_PROLOG
REAL_FUNC_PRE_CALL
REAL_FUNC_POST_CALL
HOOKFN_EPILOG

Here's an example of a function using the macros:
HMODULE NAKEDDEF DD_LoadLibraryA ( LPCSTR lpLibFileName )
{
    // Any local variables must be declared before the HOOKFN_PROLOG
    // macro.  This sets up the actual function prolog and automatically
    // defines some key variables such as stEvtInfo (DDEVENTINFO).
    HOOKFN_PROLOG ( ) ;

    // Before doing the call to the real function, you need to specify
    // the REAL_FUNC_PRE_CALL macro to ensure the registers get set
    // up to reflect what they were when the function was called.
    REAL_FUNC_PRE_CALL ( ) ;
    // Make the real function call.
    LoadLibraryA ( lpLibFileName ) ;
    // The macro to save off the registers after the real call.  This
    // way I can change the registers before I leave so that it appears
    // that this function wasn't even there.
    REAL_FUNC_POST_CALL ( ) ;

    // The work specific to the LoadLibraryA hook.
    if ( NULL != stEvtInfo.dwRetValue )
    {
        HookAllLoadedModules ( ) ;
    }

    // Takes care of the function epilog, restoring the registers as
    // well as the stack.  The number passed to the macro is the
    // number of parameters passed to the function.  Be very careful
    // and make sure you don't get "editor inheritance" errors when you
    // cut and past this macro into other functions!
    HOOKFN_EPILOG ( 1 ) ;
}
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// The register state structure. I use this structure to ensure that
// ALL registers are returned exactly as they came from the real
// function. Notice that EBP and ESP are handled as part of the prolog.
//////////////////////////////////////////////////////////////////////*/
typedef struct tag_REGSTATE
{
    DWORD   dwEAX ;
    DWORD   dwEBX ;
    DWORD   dwECX ;
    DWORD   dwEDX ;
    DWORD   dwEDI ;
    DWORD   dwESI ;
    DWORD   dwEFL ;
} REGSTATE , * PREGSTATE ;

/*//////////////////////////////////////////////////////////////////////
// The macros to save and restore ESI across function calls in debug
// builds.
//////////////////////////////////////////////////////////////////////*/
#ifdef _DEBUG
#define SAVE_ESI()      __asm PUSH ESI
#define RESTORE_ESI()   __asm POP  ESI
#else
#define SAVE_ESI()
#define RESTORE_ESI()
#endif

/*//////////////////////////////////////////////////////////////////////
// The common prolog code for all DD_* functions
//////////////////////////////////////////////////////////////////////*/
#define HOOKFN_PROLOG()                                                \
/* All hook functions automatically get the same three local         */\
/* variables.                                                        */\
DDEVENTINFO stEvtInfo   ;   /* The event information for the function*/\
DWORD       dwLastError ;   /* The last error value                  */\
REGSTATE    stRegState  ;   /* The register state to ensure that I   */\
                            /* restore the registers correctly       */\
{                                                                      \
__asm PUSH  EBP             /* Always save EBP explicitly.           */\
__asm MOV   EBP , ESP       /* Move the stack.                       */\
__asm MOV   EAX , ESP       /* Get the stack pointer to calculate the*/\
                            /* return address and the parameters.    */\
SAVE_ESI ( )                /* Save ESI in debug builds.             */\
__asm SUB   ESP , __LOCAL_SIZE /* Save space for the local variables.*/\
__asm ADD   EAX , 04h + 04h /* Account for PUSH EBP and the          */\
                            /* return address.                       */\
                            /* Save start of parameters on the stack.*/\
__asm MOV   [stEvtInfo.dwParams] , EAX                                 \
__asm SUB   EAX , 04h       /* Get back to the return address.       */\
__asm MOV   EAX , [EAX]     /* EAX now holds the return address.     */\
                            /* Save the return address.              */\
__asm MOV   [stEvtInfo.dwAddr] , EAX                                   \
__asm MOV   dwLastError , 0 /* Initialize dwLastError.               */\
                            /* Initialize the event information.     */\
__asm MOV   [stEvtInfo.eFunc] , eUNINITIALIZEDFE                       \
__asm MOV   [stRegState.dwEDI] , EDI /* Save the two registers that  */\
__asm MOV   [stRegState.dwESI] , ESI /* need to be saved across      */\
                                     /* function calls.              */\
}

/*//////////////////////////////////////////////////////////////////////
// The common epilog code for all DD_* functions. iNumParams is the
// number of parameters to the function that is used to restore the
// stack to the proper place after the hook call.
//////////////////////////////////////////////////////////////////////*/
#define HOOKFN_EPILOG(iNumParams)                                      \
{                                                                      \
SetLastError ( dwLastError ) ;      /* Set the real function's last  */\
                                    /* error value.                  */\
__asm ADD   ESP , __LOCAL_SIZE      /* Add back the local variables. */\
__asm MOV   EBX , [stRegState.dwEBX]/* Restore all the registers so  */\
__asm MOV   ECX , [stRegState.dwECX]/* that this call looks          */\
__asm MOV   EDX , [stRegState.dwEDX]/* identical to the intercepted  */\
__asm MOV   EDI , [stRegState.dwEDI]/* function.                     */\
__asm MOV   ESI , [stRegState.dwESI]                                   \
__asm MOV   EAX , [stRegState.dwEFL]                                   \
__asm SAHF                                                             \
__asm MOV   EAX , [stRegState.dwEAX]                                   \
RESTORE_ESI ( )                     /* Restore ESI in debug builds.  */\
__asm MOV   ESP , EBP               /* Put back ESP.                 */\
__asm POP   EBP                     /* Restore the saved EBP.        */\
__asm RET   iNumParams * 4          /* stdcall restore of the stack  */\
}

/*//////////////////////////////////////////////////////////////////////
// The REAL_FUNC_PRE_CALL macro needs to be placed IMMEDIATELY *BEFORE*
// ANY call to the real function the hook function is handling. The
// macro ensures that EDI and ESI are returned in the same condition
// they were in when passed to the hook function.
//////////////////////////////////////////////////////////////////////*/
#define REAL_FUNC_PRE_CALL()                                           \
{                                                                      \
__asm MOV   EDI , [stRegState.dwEDI]    /* Restore the real EDI.     */\
__asm MOV   ESI , [stRegState.dwESI]    /* Restore the real ESI.     */\
}

/*//////////////////////////////////////////////////////////////////////
// The REAL_FUNC_POST_CALL macro needs to be placed IMMEDIATELY *AFTER*
// ANY call to the real function the hook function is handling. All the
// register values after the real call are saved so that the hook
// function epilog can return the same register values as the real
// function call.
//////////////////////////////////////////////////////////////////////*/
#define REAL_FUNC_POST_CALL()                                          \
{                                                                      \
__asm MOV   [stRegState.dwEAX] , EAX  /* Save the EAX value.         */\
__asm MOV   [stRegState.dwEBX] , EBX  /* Save the EBX value.         */\
__asm MOV   [stRegState.dwECX] , ECX  /* Save the ECX value.         */\
__asm MOV   [stRegState.dwEDX] , EDX  /* Save the EDX value.         */\
__asm MOV   [stRegState.dwEDI] , EDI  /* Save the EDI value.         */\
__asm MOV   [stRegState.dwESI] , ESI  /* Save the ESI value.         */\
__asm XOR   EAX , EAX                 /* Zero out EAX.               */\
__asm LAHF                            /* Load the flag values into AH*/\
__asm MOV   [stRegState.dwEFL] , EAX  /* Save the flag values.       */\
}                                                                      \
dwLastError = GetLastError ( ) ;      /* Save the last error value.  */\
{                                                                      \
__asm MOV   EAX , [stRegState.dwEAX]  /* Restore EAX to its original */\
                                      /* value.                      */\
                                      /* Set the return value for    */\
                                      /* the information.            */\
__asm MOV   [stEvtInfo.dwRetValue] , EAX                               \
}

/*//////////////////////////////////////////////////////////////////////
// A convenient macro to fill out the event information structure
//////////////////////////////////////////////////////////////////////*/
#define FILL_EVENTINFO(eFn)                         \
    stEvtInfo.eFunc      = eFn       ;              \
    stEvtInfo.ePrePost   = ePostCall ;              \
    stEvtInfo.dwThreadId = GetCurrentThreadId ( )
    
/*//////////////////////////////////////////////////////////////////////
// The version 2 macros to make life MUCH, MUCH simpler to define
// hook functions.
//////////////////////////////////////////////////////////////////////*/
// Declare this at the beginning of any hook function.
// eFunc          - The function enumeration value.
// SynchClassType - The DDOPT_* flag value for the function class you
//                  are processing.
// bRecordPreCall - Record the entry into this function.
#define HOOKFN_STARTUP(eFunc,SynchClassType,bRecordPreCall)            \
    HOOKFN_PROLOG ( ) ;                                                \
    if ( TRUE == DoLogging ( SynchClassType ) )                        \
    {                                                                  \
        FILL_EVENTINFO ( eFunc ) ;                                     \
        if ( TRUE == (int)bRecordPreCall )                             \
        {                                                              \
            stEvtInfo.ePrePost = ePreCall ;                            \
            ProcessEvent ( &stEvtInfo ) ;                              \
        }                                                              \
    }                                                                  \
    REAL_FUNC_PRE_CALL ( ) ;
    
/*//////////////////////////////////////////////////////////////////////
// The ending macro for a hook function.
// iNuMParams     - The number of parameters passed to the function.
// SynchClassType - The synchronization class for the function.
//////////////////////////////////////////////////////////////////////*/
#define HOOKFN_SHUTDOWN(iNumParams,SynchClass)                         \
    REAL_FUNC_POST_CALL ( ) ;                                          \
    if ( TRUE == DoLogging ( SynchClass ) )                            \
    {                                                                  \
        stEvtInfo.ePrePost = ePostCall ;                               \
        ProcessEvent ( &stEvtInfo ) ;                                  \
    }                                                                  \
    HOOKFN_EPILOG ( iNumParams ) ;

/*//////////////////////////////////////////////////////////////////////
// The calling convention declaration for all DD_* definitions.
//////////////////////////////////////////////////////////////////////*/
#define NAKEDDEF __declspec(naked)

/*//////////////////////////////////////////////////////////////////////
// BIG NOTE BIG NOTE BIG NOTE BIG NOTE
// The following prototypes look like cdecl functions. They are
// not--they are all stdcall! The custom prolog and epilog ensure that
// the correct calling convention is used!
//////////////////////////////////////////////////////////////////////*/

////////////////////////////////////////////////////////////////////////
// The mandatory functions that have to be intercepted to make the
// system work
BOOL DD_FreeLibrary ( HMODULE hModule ) ;
VOID DD_FreeLibraryAndExitThread ( HMODULE hModule    ,
                                   DWORD   dwExitCode  ) ;
HMODULE DD_LoadLibraryA ( LPCSTR lpLibFileName ) ;
HMODULE DD_LoadLibraryW ( LPCWSTR lpLibFileName ) ;
HMODULE DD_LoadLibraryExA ( LPCSTR lpLibFileName ,
                            HANDLE hFile         ,
                            DWORD  dwFlags        ) ;
HMODULE DD_LoadLibraryExW ( LPCWSTR lpLibFileName ,
                            HANDLE  hFile         ,
                            DWORD   dwFlags        ) ;

VOID DD_ExitProcess ( UINT uExitCode ) ;

FARPROC DD_GetProcAddress ( HMODULE hModule , LPCSTR lpProcName ) ;

////////////////////////////////////////////////////////////////////////
// The thread-specific functions
HANDLE DD_CreateThread (LPSECURITY_ATTRIBUTES  lpThreadAttributes   ,
                        DWORD                  dwStackSize          ,
                        LPTHREAD_START_ROUTINE lpStartAddress       ,
                        LPVOID                 lpParameter          ,
                        DWORD                  dwCreationFlags      ,
                        LPDWORD                lpThreadId            ) ;
VOID DD_ExitThread ( DWORD dwExitCode ) ;
DWORD DD_SuspendThread ( HANDLE hThread ) ;
DWORD DD_ResumeThread ( HANDLE hThread ) ;
BOOL DD_TerminateThread ( HANDLE hThread , DWORD dwExitCode ) ;

// The following are the CRT thread function and are handled properly as
// they are __cdecl.
uintptr_t
    DD_beginthreadex ( void *           security        ,
                       unsigned         stack_size      ,
                       unsigned ( __stdcall *start_address )( void * ) ,
                       void *           arglist         ,
                       unsigned         initflag        ,
                       unsigned *       thrdaddr         ) ;
uintptr_t
    DD_beginthread ( void( __cdecl *start_address )( void * ) ,
                     unsigned   stack_size                    ,
                     void *     arglist                        ) ;
VOID DD_endthreadex ( unsigned retval ) ;
VOID DD_endthread ( void ) ;

////////////////////////////////////////////////////////////////////////
// Waiting and special functions
DWORD DD_WaitForSingleObject ( HANDLE hHandle        ,
                               DWORD  dwMilliseconds  ) ;
DWORD DD_WaitForSingleObjectEx ( HANDLE hHandle        ,
                                 DWORD  dwMilliseconds ,
                                 BOOL   bAlertable      ) ;
DWORD DD_WaitForMultipleObjects( DWORD          nCount          ,
                                 CONST HANDLE * lpHandles       ,
                                 BOOL           bWaitAll        ,
                                 DWORD          dwMilliseconds   ) ;
DWORD DD_WaitForMultipleObjectsEx( DWORD          nCount         ,
                                   CONST HANDLE * lpHandles      ,
                                   BOOL           bWaitAll       ,
                                   DWORD          dwMilliseconds ,
                                   BOOL           bAlertable      ) ;
DWORD DD_MsgWaitForMultipleObjects ( DWORD    nCount        ,
                                     LPHANDLE pHandles      ,
                                     BOOL     fWaitAll      ,
                                     DWORD    dwMilliseconds,
                                     DWORD    dwWakeMask     ) ;
DWORD DD_MsgWaitForMultipleObjectsEx ( DWORD    nCount          ,
                                       LPHANDLE pHandles        ,
                                       DWORD    dwMilliseconds  ,
                                       DWORD    dwWakeMask      ,
                                       DWORD    dwFlags          ) ;
DWORD DD_SignalObjectAndWait ( HANDLE hObjectToSignal ,
                               HANDLE hObjectToWaitOn ,
                               DWORD  dwMilliseconds  ,
                               BOOL   bAlertable       ) ;
BOOL DD_CloseHandle ( HANDLE hObject ) ;

////////////////////////////////////////////////////////////////////////
// Critical-section functions
VOID DD_InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL DD_InitializeCriticalSectionAndSpinCount (
                                   LPCRITICAL_SECTION lpCriticalSection,
                                   DWORD              dwSpinCount     );
VOID DD_DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection ) ;
VOID DD_EnterCriticalSection ( LPCRITICAL_SECTION lpCriticalSection ) ;
VOID DD_LeaveCriticalSection ( LPCRITICAL_SECTION lpCriticalSection ) ;
DWORD DD_SetCriticalSectionSpinCount (
                                   LPCRITICAL_SECTION lpCriticalSection,
                                   DWORD              dwSpinCount     );
BOOL DD_TryEnterCriticalSection ( LPCRITICAL_SECTION lpCriticalSection);

////////////////////////////////////////////////////////////////////////
// Mutex functions
HANDLE DD_CreateMutexA ( LPSECURITY_ATTRIBUTES lpMutexAttributes ,
                         BOOL                  bInitialOwner     ,
                         LPCSTR                lpName             ) ;
HANDLE DD_CreateMutexW ( LPSECURITY_ATTRIBUTES lpMutexAttributes ,
                         BOOL                  bInitialOwner     ,
                         LPCWSTR               lpName             ) ;
HANDLE DD_OpenMutexA ( DWORD  dwDesiredAccess ,
                       BOOL   bInheritHandle  ,
                       LPCSTR lpName           ) ;
HANDLE DD_OpenMutexW ( DWORD   dwDesiredAccess ,
                       BOOL    bInheritHandle  ,
                       LPCWSTR lpName           ) ;
BOOL DD_ReleaseMutex ( HANDLE hMutex ) ;

////////////////////////////////////////////////////////////////////////
// Semaphore functions
HANDLE
    DD_CreateSemaphoreA ( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes ,
                          LONG                  lInitialCount         ,
                          LONG                  lMaximumCount         ,
                          LPCSTR                lpName                );
HANDLE
    DD_CreateSemaphoreW ( LPSECURITY_ATTRIBUTES lpSemaphoreAttributes ,
                          LONG                  lInitialCount         ,
                          LONG                  lMaximumCount         ,
                          LPCWSTR               lpName                );
HANDLE DD_OpenSemaphoreA ( DWORD  dwDesiredAccess ,
                           BOOL   bInheritHandle  ,
                           LPCSTR lpName           ) ;
HANDLE DD_OpenSemaphoreW ( DWORD   dwDesiredAccess ,
                           BOOL    bInheritHandle  ,
                           LPCWSTR lpName           ) ;
BOOL DD_ReleaseSemaphore ( HANDLE hSemaphore      ,
                           LONG   lReleaseCount   ,
                           LPLONG lpPreviousCount  ) ;

////////////////////////////////////////////////////////////////////////
// Event functions
HANDLE DD_CreateEventA ( LPSECURITY_ATTRIBUTES lpEventAttributes ,
                         BOOL                  bManualReset      ,
                         BOOL                  bInitialState     ,
                         LPCSTR                lpName             ) ;
HANDLE DD_CreateEventW ( LPSECURITY_ATTRIBUTES lpEventAttributes ,
                         BOOL                  bManualReset      ,
                         BOOL                  bInitialState     ,
                         LPCWSTR               lpName             ) ;
HANDLE DD_OpenEventA ( DWORD  dwDesiredAccess ,
                       BOOL   bInheritHandle  ,
                       LPCSTR lpName           ) ;
HANDLE DD_OpenEventW ( DWORD   dwDesiredAccess ,
                       BOOL    bInheritHandle  ,
                       LPCWSTR lpName           ) ;
BOOL DD_PulseEvent ( HANDLE hEvent ) ;
BOOL DD_ResetEvent ( HANDLE hEvent ) ;
BOOL DD_SetEvent ( HANDLE hEvent ) ;

#endif  // _DD_FUNCS_H

