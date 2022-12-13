/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
Parameter translation routines.  At this point, I am only doing just a
few that are needed for the DeadlockDetection system.
----------------------------------------------------------------------*/

#ifndef _PARAMDECODE_H
#define _PARAMDECODE_H

int Trans_DWORD ( LPTSTR     szBuff   ,
                  DWORD_PTR  dwParams  ) ;

int Trans_UINT ( LPTSTR    szBuff   ,
                 UINT_PTR  dwValue  ) ;

int Trans_int ( LPTSTR    szBuff   ,
                int       dwValue  ) ; ;

int Trans_LPSTR ( LPTSTR  szBuff  ,
                  LPCSTR  szValue  ) ;

int Trans_LPWSTR ( LPTSTR  szBuff  ,
                   LPCWSTR szValue  ) ;

int Trans_HandleArray ( LPTSTR   szBuff    ,
                        UINT     nCount    ,
                        HANDLE * lpHandles  ) ;

int Trans_ParamSeparator ( LPTSTR szBuff ) ;
int Trans_ExitProcessParams ( LPTSTR    szBuff   ,
                              DWORD_PTR dwParams  ) ;
int Trans_TerminateProcessParams ( LPTSTR     szBuff   ,
                                  DWORD_PTR  dwParams  ) ;
int Trans_WaitForSingleObjectParams ( LPTSTR    szBuff   ,
                                      DWORD_PTR dwParams  ) ;
int Trans_WaitForSingleObjectExParams ( LPTSTR    szBuff   ,
                                        DWORD_PTR dwParams  ) ;
int Trans_WaitForMultipleObjectsParams ( LPTSTR    szBuff   ,
                                         DWORD_PTR dwParams  ) ;
int Trans_WaitForMultipleObjectsExParams ( LPTSTR    szBuff   ,
                                           DWORD_PTR dwParams  ) ;
int Trans_MsgWaitForMultipleObjectsParams ( LPTSTR    szBuff   ,
                                            DWORD_PTR dwParams  ) ;
int Trans_MsgWaitForMultipleObjectsExParams ( LPTSTR    szBuff   ,
                                              DWORD_PTR dwParams  ) ;
int Trans_SignalObjectAndWaitParams ( LPTSTR    szBuff   ,
                                      DWORD_PTR dwParams  ) ;
int Trans_CloseHandleParams ( LPTSTR    szBuff   ,
                              DWORD_PTR dwParams  ) ;
int Trans_GetProcAddressParams ( LPTSTR    szBuff   ,
                                 DWORD_PTR dwParams  ) ;
int Trans_CreateThreadParams ( LPTSTR    szBuff   ,
                               DWORD_PTR dwParams  ) ;
int Trans_ExitThreadParams ( LPTSTR    szBuff   ,
                             DWORD_PTR dwParams  ) ;
int Trans_SuspendThreadParams ( LPTSTR    szBuff   ,
                                DWORD_PTR dwParams  ) ;
int Trans_ResumeThreadParams ( LPTSTR    szBuff   ,
                               DWORD_PTR dwParams  ) ;
int Trans_TerminateThreadParams ( LPTSTR    szBuff   ,
                                  DWORD_PTR dwParams  ) ;
int Trans_FreeLibraryAndExitThread ( LPTSTR    szBuff  ,
                                     DWORD_PTR dwParams ) ;
int Trans_beginthreadex ( LPTSTR    szBuff  ,
                          DWORD_PTR dwParams ) ;
int Trans_endthreadex ( LPTSTR    szBuff  ,
                        DWORD_PTR dwParams ) ;
int Trans_beginthread ( LPTSTR    szBuff  ,
                        DWORD_PTR dwParams ) ;
int Trans_endthread ( LPTSTR    szBuff  ,
                      DWORD_PTR dwParams ) ;
int Trans_InitializeCriticalSectionParams ( LPTSTR    szBuff   ,
                                            DWORD_PTR dwParams  ) ;
int Trans_InitializeCriticalSectionAndSpinCountParams (LPTSTR szBuff  ,
                                                    DWORD_PTR dwParams);
int Trans_DeleteCriticalSectionParams ( LPTSTR    szBuff   ,
                                        DWORD_PTR dwParams  ) ;
int Trans_EnterCriticalSectionParams ( LPTSTR    szBuff   ,
                                       DWORD_PTR dwParams  ) ;
int Trans_LeaveCriticalSectionParams ( LPTSTR    szBuff   ,
                                       DWORD_PTR dwParams  ) ;
int Trans_SetCriticalSectionSpinCountParams ( LPTSTR    szBuff   ,
                                              DWORD_PTR dwParams  ) ;
int Trans_TryEnterCriticalSectionParams ( LPTSTR    szBuff   ,
                                          DWORD_PTR dwParams  ) ;
int Trans_CreateMutexAParams ( LPTSTR    szBuff   ,
                               DWORD_PTR dwParams  ) ;
int Trans_CreateMutexWParams ( LPTSTR    szBuff   ,
                               DWORD_PTR dwParams  ) ;
int Trans_OpenMutexAParams ( LPTSTR    szBuff   ,
                             DWORD_PTR dwParams  ) ;
int Trans_OpenMutexWParams ( LPTSTR    szBuff   ,
                             DWORD_PTR dwParams  ) ;
int Trans_ReleaseMutexParams ( LPTSTR    szBuff   ,
                               DWORD_PTR dwParams  ) ;
int Trans_CreateSemaphoreAParams ( LPTSTR    szBuff   ,
                                   DWORD_PTR dwParams  ) ;
int Trans_CreateSemaphoreWParams ( LPTSTR    szBuff   ,
                                   DWORD_PTR dwParams  ) ;
int Trans_OpenSemaphoreAParams ( LPTSTR    szBuff   ,
                                 DWORD_PTR dwParams  ) ;
int Trans_OpenSemaphoreWParams ( LPTSTR    szBuff   ,
                                 DWORD_PTR dwParams  ) ;
int Trans_ReleaseSemaphoreParams ( LPTSTR    szBuff   ,
                                   DWORD_PTR dwParams  ) ;
int Trans_CreateEventAParams ( LPTSTR    szBuff   ,
                               DWORD_PTR dwParams  ) ;
int Trans_CreateEventWParams ( LPTSTR    szBuff   ,
                               DWORD_PTR dwParams  ) ;
int Trans_OpenEventAParams ( LPTSTR    szBuff   ,
                             DWORD_PTR dwParams  ) ;
int Trans_OpenEventWParams ( LPTSTR    szBuff   ,
                             DWORD_PTR dwParams  ) ;
int Trans_PulseEventParams ( LPTSTR    szBuff   ,
                             DWORD_PTR dwParams  ) ;
int Trans_ResetEventParams ( LPTSTR    szBuff   ,
                             DWORD_PTR dwParams  );
int Trans_SetEventParams ( LPTSTR    szBuff   ,
                           DWORD_PTR dwParams  ) ;

#endif // _PARAMDECODE_H


