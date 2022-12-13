/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "PCH.h"
#include "DeadlockDetection.h"
#include "ParamDecode.h"

int Trans_DWORDHEX ( LPTSTR     szBuff  ,
                     DWORD_PTR  dwValue  )
{
    return ( wsprintf ( szBuff  , _T ( "0x%08X" )  , dwValue ) ) ;
}


int Trans_UINT ( LPTSTR   szBuff   ,
                 UINT_PTR dwValue  )
{
    return ( wsprintf ( szBuff  , _T ( "%lu" )  , dwValue ) ) ;
}

int Trans_int ( LPTSTR szBuff   ,
                int    dwValue  )
{
    return ( wsprintf ( szBuff  , _T ( "%d" )  , dwValue ) ) ;
}

static int InternalStringTranslate ( LPTSTR  szBuff  ,
                                     LPCSTR  szValue ,
                                     LPCTSTR szFmt    )
{
    // Do the string pointer itself.
    int iRet = Trans_DWORDHEX ( szBuff , (DWORD_PTR)szValue ) ;

    // Now show the string.
    if ( NULL != szValue )
    {
        iRet += wsprintf ( szBuff + iRet ,
                           szFmt         ,
                           szValue        ) ;
    }
    else
    {
        iRet += wsprintf ( szBuff + iRet , _T ( " [NULL]" ) ) ;
    }
    return ( iRet ) ;
}

int Trans_LPSTR ( LPTSTR  szBuff  ,
                  LPCSTR  szValue  )
{
    return ( InternalStringTranslate ( szBuff         ,
                                       szValue        ,
                                       _T ( " [%S]" )  ) );
}

int Trans_LPWSTR ( LPTSTR  szBuff  ,
                   LPCWSTR szValue  )
{
    return ( InternalStringTranslate ( szBuff           ,
                                       (LPCSTR)szValue  ,
                                       _T ( " [%s]" )    ) ) ;
}

int Trans_HandleArray ( LPTSTR   szBuff     ,
                        UINT     nCount     ,
                        HANDLE * lpHandles   )
{
    // Do the lpHandle pointer.
    int iRet = wsprintf ( szBuff            ,
                          _T ( "0x%08X [" ) ,
                          lpHandles          ) ;
    // Put the handle values in the list.
    for ( UINT i = 0 ; i < nCount ; i++ )
    {
        iRet += wsprintf ( szBuff + iRet         ,
                           _T ( "0x%08x" )       ,
                           lpHandles[ i ]  ) ;
        if ( i + 1 < nCount )
        {
            iRet += wsprintf ( szBuff + iRet , _T ( " " ) ) ;
        }
    }
    iRet += wsprintf ( szBuff + iRet , _T ( "]" ) ) ;
    return ( iRet ) ;
}

int Trans_ParamSeparator ( LPTSTR szBuff )
{
    return ( wsprintf ( szBuff , _T ( ", " ) ) ) ;
}

int Trans_ExitProcessParams ( LPTSTR     szBuff   ,
                              DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR*)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_WaitForSingleObjectParams ( LPTSTR     szBuff   ,
                                      DWORD_PTR  dwParams  )
{
    LPWAITFORSINGLEOBJECT_PARAMS pData =
                        (LPWAITFORSINGLEOBJECT_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , (DWORD_PTR)pData->hHandle);
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    if ( INFINITE == pData->dwMilliseconds )
    {
        iRet += wsprintf ( szBuff + iRet , _T ( "INFINITE" ) ) ;
    }
    else
    {
        iRet += Trans_UINT ( szBuff + iRet , pData->dwMilliseconds ) ;
    }
    return ( iRet ) ;
}

int Trans_WaitForSingleObjectExParams ( LPTSTR    szBuff   ,
                                        DWORD_PTR dwParams  )
{
    LPWAITFORSINGLEOBJECTEX_PARAMS pData =
                        (LPWAITFORSINGLEOBJECTEX_PARAMS)dwParams ;
    // The first two params can be done through WaitForSingleObject.
    int iRet = 0 ;
    iRet += Trans_WaitForSingleObjectParams ( szBuff + iRet , dwParams);
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bAlertable ) ;
    return ( iRet ) ;
}

int Trans_WaitForMultipleObjectsParams ( LPTSTR     szBuff   ,
                                         DWORD_PTR  dwParams  )
{
    LPWAITFORMULTIPLEOBJECTS_PARAMS pData =
                            (LPWAITFORMULTIPLEOBJECTS_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_UINT ( szBuff + iRet , pData->nCount ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_HandleArray ( szBuff + iRet             ,
                                pData->nCount             ,
                                (HANDLE*)pData->lpHandles  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    if ( INFINITE == pData->dwMilliseconds )
    {
        iRet += wsprintf ( szBuff + iRet , _T ( "INFINITE" ) ) ;
    }
    else
    {
        iRet += Trans_UINT ( szBuff + iRet , pData->dwMilliseconds ) ;
    }
    return ( iRet ) ;
}

int Trans_WaitForMultipleObjectsExParams ( LPTSTR     szBuff   ,
                                           DWORD_PTR  dwParams  )
{
    LPWAITFORMULTIPLEOBJECTSEX_PARAMS pData =
                           (LPWAITFORMULTIPLEOBJECTSEX_PARAMS)dwParams ;
    int iRet = 0 ;
    // First part done by WaitForMultipleObjects.
    iRet += Trans_WaitForMultipleObjectsParams ( szBuff + iRet ,
                                                  dwParams      ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bAlertable ) ;
    return ( iRet ) ;
}

int Trans_MsgWaitForMultipleObjectsParams ( LPTSTR    szBuff   ,
                                            DWORD_PTR dwParams  )
{
    LPMSGWAITFORMULTIPLEOBJECTS_PARAMS pData =
                          (LPMSGWAITFORMULTIPLEOBJECTS_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_UINT ( szBuff + iRet , pData->nCount ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_HandleArray ( szBuff + iRet   ,
                                pData->nCount   ,
                                pData->pHandles  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->fWaitAll ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    if ( INFINITE == pData->dwMilliseconds )
    {
        iRet += wsprintf ( szBuff + iRet , _T ( "INFINITE" ) ) ;
    }
    else
    {
        iRet += Trans_UINT ( szBuff + iRet , pData->dwMilliseconds ) ;
    }
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwWakeMask ) ;
    return ( iRet ) ;
}

int Trans_MsgWaitForMultipleObjectsExParams ( LPTSTR    szBuff   ,
                                              DWORD_PTR dwParams  )
{
    LPMSGWAITFORMULTIPLEOBJECTSEX_PARAMS pData =
                        (LPMSGWAITFORMULTIPLEOBJECTSEX_PARAMS)dwParams ;
    int iRet = 0 ;
    Trans_MsgWaitForMultipleObjectsParams ( szBuff + iRet , dwParams ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwFlags ) ;
    return ( iRet ) ;
}

int Trans_SignalObjectAndWaitParams ( LPTSTR    szBuff   ,
                                      DWORD_PTR dwParams  )
{
    LPSIGNALOBJECTANDWAIT_PARAMS pData =
                                (LPSIGNALOBJECTANDWAIT_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                     ,
                             (DWORD_PTR)pData->hObjectToSignal  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                     ,
                             (DWORD_PTR)pData->hObjectToWaitOn  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    if ( INFINITE == pData->dwMilliseconds )
    {
        iRet += wsprintf ( szBuff + iRet , _T ( "INFINITE" ) ) ;
    }
    else
    {
        iRet += Trans_UINT ( szBuff + iRet , pData->dwMilliseconds ) ;
    }
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bAlertable ) ;
    return ( iRet ) ;
}

int Trans_CloseHandleParams ( LPTSTR     szBuff   ,
                              DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_GetProcAddressParams ( LPTSTR     szBuff   ,
                                 DWORD_PTR  dwParams  )
{
    LPGETPROCADDRESS_PARAMS pData = (LPGETPROCADDRESS_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , (DWORD_PTR)pData->hModule);
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPSTR ( szBuff + iRet , (LPSTR)pData->lpProcName ) ;
    return ( iRet ) ;
}

int Trans_CreateThreadParams ( LPTSTR     szBuff   ,
                               DWORD_PTR  dwParams  )
{
    LPCREATETHREAD_PARAMS pData = (LPCREATETHREAD_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                        ,
                             (DWORD_PTR)pData->lpThreadAttributes  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwStackSize ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                    ,
                             (DWORD_PTR)pData->lpStartAddress  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                ,
                            (DWORD_PTR)pData->lpParameter  );
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwCreationFlags ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                ,
                             (DWORD_PTR)pData->lpThreadId  ) ;
    return ( iRet ) ;
}

int Trans_ExitThreadParams ( LPTSTR     szBuff   ,
                             DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_SuspendThreadParams ( LPTSTR szBuff   ,
                                DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_ResumeThreadParams ( LPTSTR szBuff   ,
                               DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_TerminateThreadParams ( LPTSTR    szBuff   ,
                                  DWORD_PTR dwParams  )
{
    LPTERMINATETHREAD_PARAMS pData = (LPTERMINATETHREAD_PARAMS)dwParams;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , (DWORD_PTR)pData->hThread);
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwExitCode ) ;
    return ( iRet ) ;
}

int Trans_FreeLibraryAndExitThread ( LPTSTR    szBuff  ,
                                     DWORD_PTR dwParams )
{
    LPFREELIBRARYANDEXITTHREAD_PARAMS pData =
                           (LPFREELIBRARYANDEXITTHREAD_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , (DWORD_PTR)pData->hModule);
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwExitCode ) ;
    return ( iRet ) ;
}

int Trans_beginthreadex ( LPTSTR    szBuff  ,
                          DWORD_PTR dwParams )
{
    LPCREATETHREAD_PARAMS pData = (LPCREATETHREAD_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                        ,
                             (DWORD_PTR)pData->lpThreadAttributes  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwStackSize ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                    ,
                             (DWORD_PTR)pData->lpStartAddress  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                ,
                            (DWORD_PTR)pData->lpParameter  );
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwCreationFlags ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                ,
                             (DWORD_PTR)pData->lpThreadId  ) ;
    return ( iRet ) ;
}
int Trans_endthreadex ( LPTSTR    szBuff  ,
                        DWORD_PTR dwParams )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}
                        
int Trans_beginthread ( LPTSTR    szBuff  ,
                        DWORD_PTR dwParams )
{
    LPBEGINTHREAD_PARAMS pData = (LPBEGINTHREAD_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                        ,
                             (DWORD_PTR)pData->start_address       ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->stack_size ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                    ,
                             (DWORD_PTR)pData->arglist ) ;
    return ( iRet ) ;
}

int Trans_endthread ( LPTSTR    /*szBuff*/  ,
                      DWORD_PTR /*dwParams*/ )
{
    return ( 0 ) ;
}

int Trans_InitializeCriticalSectionParams ( LPTSTR     szBuff   ,
                                            DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_InitializeCriticalSectionAndSpinCountParams (LPTSTR szBuff  ,
                                                   DWORD_PTR  dwParams )
{
    LPINITIALIZECRITICALSECTIONANDSPINCOUNT_PARAMS pData =
              (LPINITIALIZECRITICALSECTIONANDSPINCOUNT_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                   ,
                             (DWORD_PTR)pData->lpCriticalSection  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->dwSpinCount ) ;
    return ( iRet ) ;
}

int Trans_DeleteCriticalSectionParams ( LPTSTR     szBuff   ,
                                        DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_EnterCriticalSectionParams ( LPTSTR     szBuff   ,
                                       DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_LeaveCriticalSectionParams ( LPTSTR     szBuff   ,
                                       DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_SetCriticalSectionSpinCountParams ( LPTSTR     szBuff   ,
                                              DWORD_PTR  dwParams  )
{
    LPSETCRITICALSECTIONSPINCOUNT_PARAMS pData =
                        (LPSETCRITICALSECTIONSPINCOUNT_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                   ,
                             (DWORD_PTR)pData->lpCriticalSection  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->dwSpinCount ) ;
    return ( iRet ) ;
}

int Trans_TryEnterCriticalSectionParams ( LPTSTR     szBuff   ,
                                          DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_CreateMutexAParams ( LPTSTR     szBuff   ,
                               DWORD_PTR  dwParams  )
{
    LPCREATEMUTEXA_PARAMS pData = (LPCREATEMUTEXA_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                   ,
                             (DWORD_PTR)pData->lpMutexAttributes  );
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInitialOwner ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPSTR ( szBuff + iRet , (LPSTR)pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_CreateMutexWParams ( LPTSTR     szBuff   ,
                               DWORD_PTR  dwParams  )
{
    LPCREATEMUTEXW_PARAMS pData = (LPCREATEMUTEXW_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                   ,
                             (DWORD_PTR)pData->lpMutexAttributes  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInitialOwner ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPWSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_OpenMutexAParams ( LPTSTR     szBuff   ,
                             DWORD_PTR  dwParams  )
{
    LPOPENMUTEXA_PARAMS pData = (LPOPENMUTEXA_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwDesiredAccess ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInheritHandle ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_OpenMutexWParams ( LPTSTR     szBuff   ,
                             DWORD_PTR  dwParams  )
{
    LPOPENMUTEXW_PARAMS pData = (LPOPENMUTEXW_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwDesiredAccess ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInheritHandle ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPWSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_ReleaseMutexParams ( LPTSTR     szBuff   ,
                               DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_CreateSemaphoreAParams ( LPTSTR     szBuff   ,
                                   DWORD_PTR  dwParams  )
{
    LPCREATESEMAPHOREA_PARAMS pData =
                                   (LPCREATESEMAPHOREA_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                           ,
                             (DWORD_PTR)pData->lpSemaphoreAttributes  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->lInitialCount ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->lMaximumCount ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_CreateSemaphoreWParams ( LPTSTR     szBuff   ,
                                   DWORD_PTR  dwParams  )
{
    LPCREATESEMAPHOREW_PARAMS pData =
                                   (LPCREATESEMAPHOREW_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                           ,
                             (DWORD_PTR)pData->lpSemaphoreAttributes  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->lInitialCount ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->lMaximumCount ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPWSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_OpenSemaphoreAParams ( LPTSTR     szBuff   ,
                                 DWORD_PTR  dwParams  )
{
    LPOPENSEMAPHOREA_PARAMS pData = (LPOPENSEMAPHOREA_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                     ,
                             (DWORD_PTR)pData->dwDesiredAccess  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet                   ,
                         (DWORD_PTR)pData->bInheritHandle ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_OpenSemaphoreWParams ( LPTSTR     szBuff   ,
                                 DWORD_PTR  dwParams  )
{
    LPOPENSEMAPHOREW_PARAMS pData = (LPOPENSEMAPHOREW_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                     ,
                             (DWORD_PTR)pData->dwDesiredAccess  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet                   ,
                         (DWORD_PTR)pData->bInheritHandle ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPWSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_ReleaseSemaphoreParams ( LPTSTR     szBuff   ,
                                   DWORD_PTR  dwParams  )
{
    LPRELEASESEMAPHORE_PARAMS pData =
                                   (LPRELEASESEMAPHORE_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                ,
                             (DWORD_PTR)pData->hSemaphore  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->lReleaseCount ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                     ,
                             (DWORD_PTR)pData->lpPreviousCount  ) ;
    return ( iRet ) ;
}

int Trans_CreateEventAParams ( LPTSTR     szBuff   ,
                               DWORD_PTR  dwParams  )
{
    LPCREATEEVENTA_PARAMS pData = (LPCREATEEVENTA_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                       ,
                             (DWORD_PTR)pData->lpEventAttributes  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bManualReset ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInitialState ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_CreateEventWParams ( LPTSTR     szBuff   ,
                               DWORD_PTR  dwParams  )
{
    LPCREATEEVENTW_PARAMS pData = (LPCREATEEVENTW_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet                       ,
                             (DWORD_PTR)pData->lpEventAttributes  ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bManualReset ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInitialState ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPWSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_OpenEventAParams ( LPTSTR     szBuff   ,
                             DWORD_PTR  dwParams  )
{
    LPOPENMUTEXA_PARAMS pData = (LPOPENMUTEXA_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwDesiredAccess ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInheritHandle ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_OpenEventWParams ( LPTSTR     szBuff   ,
                             DWORD_PTR  dwParams  )
{
    LPOPENMUTEXW_PARAMS pData = (LPOPENMUTEXW_PARAMS)dwParams ;
    int iRet = 0 ;
    iRet += Trans_DWORDHEX ( szBuff + iRet , pData->dwDesiredAccess );
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_UINT ( szBuff + iRet , pData->bInheritHandle ) ;
    iRet += Trans_ParamSeparator ( szBuff + iRet ) ;
    iRet += Trans_LPWSTR ( szBuff + iRet , pData->lpName ) ;
    return ( iRet ) ;
}

int Trans_PulseEventParams ( LPTSTR     szBuff   ,
                             DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_ResetEventParams ( LPTSTR     szBuff   ,
                             DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}

int Trans_SetEventParams ( LPTSTR     szBuff   ,
                           DWORD_PTR  dwParams  )
{
    DWORD_PTR * pData = (DWORD_PTR *)dwParams ;
    return ( Trans_DWORDHEX ( szBuff , pData [ 0 ] ) ) ;
}


