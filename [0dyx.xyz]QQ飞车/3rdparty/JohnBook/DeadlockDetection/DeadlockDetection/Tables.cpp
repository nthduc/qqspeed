/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The function tables.
----------------------------------------------------------------------*/
#include "pch.h"
#include "DeadlockDetection.h"
#include "BugslayerUtil.h"
#include "Internal.h"
#include "Tables.h"

CRTDLLDESC g_stCRTDLLInfo [ NUM_CRT_DLLS ] =
{
    {
        _T ( "MSVCRT.DLL" )                     ,
        0                                       ,
        (LPHOOKFUNCDESC)&g_stCRTHookFuncs       ,
        (LPHOOKFUNCDESC)&g_stMSVCRTRealFuncs
    } ,
    {
        _T ( "MSVCRTD.DLL" )                    ,
        0                                       ,
        (LPHOOKFUNCDESC)&g_stCRTHookFuncs       ,
        (LPHOOKFUNCDESC)&g_stMSVCRTDRealFuncs
    } ,
    {
        _T ( "MSVCR70.DLL" )                    ,
        0                                       ,
        (LPHOOKFUNCDESC)&g_stCRTHookFuncs       ,
        (LPHOOKFUNCDESC)&g_stMSVCR70RealFuncs
    } ,
    {
        _T ( "MSVCR70D.DLL" )                   ,
        0                                       ,
        (LPHOOKFUNCDESC)&g_stCRTHookFuncs       ,
        (LPHOOKFUNCDESC)&g_stMSVCR71DRealFuncs
    } ,
    {
        _T ( "MSVCR71.DLL" )                    ,
        0                                       ,
        (LPHOOKFUNCDESC)&g_stCRTHookFuncs       ,
        (LPHOOKFUNCDESC)&g_stMSVCR71RealFuncs
    } ,
    {
        _T ( "MSVCR71D.DLL" )                   ,
        0                                       ,
        (LPHOOKFUNCDESC)&g_stCRTHookFuncs       ,
        (LPHOOKFUNCDESC)&g_stMSVCR70DRealFuncs
    } ,
        
} ;

////////////////////////////////////////////////////////////////////////
// The special table for just ExitProcess.  I've got to hook that one
// everywhere so DeadlockDetection can shut down.
////////////////////////////////////////////////////////////////////////
HOOKFUNCDESC g_stSpecialExitProcessHook =
{
    "ExitProcess"    , (PROC)DD_ExitProcess
} ;

HOOKFUNCDESC g_stSpecialExitProcessReal =
{
    "ExitProcess"    , (PROC)ExitProcess
} ;

////////////////////////////////////////////////////////////////////////
// KEEP THESE TABLES SORTED IN ALPHABETICAL ORDER!!!!
// KEEP THESE TABLES SORTED IN ALPHABETICAL ORDER!!!!
// KEEP THESE TABLES SORTED IN ALPHABETICAL ORDER!!!!
////////////////////////////////////////////////////////////////////////

HOOKFUNCDESC g_stCRTHookFuncs[ NUMBER_CRTDLL_FUNCS ] =
{
    { "_beginthread"        , (PROC)DD_beginthread              } ,
    { "_beginthreadex"      , (PROC)DD_beginthreadex            } ,
    { "_endthread"          , (PROC)DD_endthread                } ,
    { "_endthreadex"        , (PROC)DD_endthreadex              } ,
} ;

// The table for MSVCRT.DLL
HOOKFUNCDESC g_stMSVCRTRealFuncs[ NUMBER_CRTDLL_FUNCS ] =
{
    { "_beginthread"        , (PROC)NULL                        } ,
    { "_beginthreadex"      , (PROC)NULL                        } ,
    { "_endthread"          , (PROC)NULL                        } ,
    { "_endthreadex"        , (PROC)NULL                        } ,
} ;

// The table for MSVCRTD.DLL
HOOKFUNCDESC g_stMSVCRTDRealFuncs[ NUMBER_CRTDLL_FUNCS ] =
{
    { "_beginthread"        , (PROC)NULL                        } ,
    { "_beginthreadex"      , (PROC)NULL                        } ,
    { "_endthread"          , (PROC)NULL                        } ,
    { "_endthreadex"        , (PROC)NULL                        } ,
} ;

// The table for MSVCR70.DLL.DLL
HOOKFUNCDESC g_stMSVCR70RealFuncs[ NUMBER_CRTDLL_FUNCS ] =
{
    { "_beginthread"        , (PROC)NULL                        } ,
    { "_beginthreadex"      , (PROC)NULL                        } ,
    { "_endthread"          , (PROC)NULL                        } ,
    { "_endthreadex"        , (PROC)NULL                        } ,
} ;

// The table for MSVCR70D.DLL.DLL
HOOKFUNCDESC g_stMSVCR70DRealFuncs[ NUMBER_CRTDLL_FUNCS ] =
{
    { "_beginthread"        , (PROC)NULL                        } ,
    { "_beginthreadex"      , (PROC)NULL                        } ,
    { "_endthread"          , (PROC)NULL                        } ,
    { "_endthreadex"        , (PROC)NULL                        } ,
} ;

// The table for MSVCR71.DLL.DLL
HOOKFUNCDESC g_stMSVCR71RealFuncs[ NUMBER_CRTDLL_FUNCS ] =
{
    { "_beginthread"        , (PROC)NULL                        } ,
    { "_beginthreadex"      , (PROC)NULL                        } ,
    { "_endthread"          , (PROC)NULL                        } ,
    { "_endthreadex"        , (PROC)NULL                        } ,
} ;

// The table for MSVCR71D.DLL.DLL
HOOKFUNCDESC g_stMSVCR71DRealFuncs[ NUMBER_CRTDLL_FUNCS ] =
{
    { "_beginthread"        , (PROC)NULL                        } ,
    { "_beginthreadex"      , (PROC)NULL                        } ,
    { "_endthread"          , (PROC)NULL                        } ,
    { "_endthreadex"        , (PROC)NULL                        } ,
} ;

// The tables for the KERNEL32.DLL functions to hook.
HOOKFUNCDESC g_stDDKernelFuncs[ NUMBER_KERNEL_FUNCS ] =
{
    { "CloseHandle"                 , (PROC)DD_CloseHandle           } ,
    { "CreateEventA"                , (PROC)DD_CreateEventA          } ,
    { "CreateEventW"                , (PROC)DD_CreateEventW          } ,
    { "CreateMutexA"                , (PROC)DD_CreateMutexA          } ,
    { "CreateMutexW"                , (PROC)DD_CreateMutexW          } ,
    { "CreateSemaphoreA"            , (PROC)DD_CreateSemaphoreA      } ,
    { "CreateSemaphoreW"            , (PROC)DD_CreateSemaphoreW      } ,
    { "CreateThread"                , (PROC)DD_CreateThread          } ,
    { "DeleteCriticalSection"       , (PROC)DD_DeleteCriticalSection } ,
    { "EnterCriticalSection"        , (PROC)DD_EnterCriticalSection  } ,
    { "ExitProcess"                 , (PROC)DD_ExitProcess           } ,
    { "ExitThread"                  , (PROC)DD_ExitThread            } ,
    { "FreeLibrary"                 , (PROC)DD_FreeLibrary           } ,
    { "FreeLibraryAndExitThread"    ,
                                  (PROC)DD_FreeLibraryAndExitThread  } ,
    { "GetProcAddress"              , (PROC)DD_GetProcAddress        } ,
    { "InitializeCriticalSection"   ,
                                  (PROC)DD_InitializeCriticalSection } ,
    { "InitializeCriticalSectionAndSpinCount" ,
                      (PROC)DD_InitializeCriticalSectionAndSpinCount } ,
    {"LeaveCriticalSection"         , (PROC)DD_LeaveCriticalSection  } ,
    { "LoadLibraryA"                , (PROC)DD_LoadLibraryA          } ,
    { "LoadLibraryExA"              , (PROC)DD_LoadLibraryExA        } ,
    { "LoadLibraryExW"              , (PROC)DD_LoadLibraryExW        } ,
    { "LoadLibraryW"                , (PROC)DD_LoadLibraryW          } ,
    { "OpenEventA"                  , (PROC)DD_OpenEventA            } ,
    { "OpenEventW"                  , (PROC)DD_OpenEventW            } ,
    { "OpenMutexA"                  , (PROC)DD_OpenMutexA            } ,
    { "OpenMutexW"                  , (PROC)DD_OpenMutexW            } ,
    { "OpenSemaphoreA"              , (PROC)DD_OpenSemaphoreA        } ,
    { "OpenSemaphoreW"              , (PROC)DD_OpenSemaphoreW        } ,
    { "PulseEvent"                  , (PROC)DD_PulseEvent            } ,
    { "ReleaseMutex"                , (PROC)DD_ReleaseMutex          } ,
    { "ReleaseSemaphore"            , (PROC)DD_ReleaseSemaphore      } ,
    { "ResetEvent"                  , (PROC)DD_ResetEvent            } ,
    { "ResumeThread"                , (PROC)DD_ResumeThread          } ,
    { "SetCriticalSectionSpinCount" ,
                            (PROC)DD_SetCriticalSectionSpinCount     } ,
    { "SetEvent"                    , (PROC)DD_SetEvent              } ,
    { "SignalObjectAndWait"         , (PROC)DD_SignalObjectAndWait   } ,
    { "SuspendThread"               , (PROC)DD_SuspendThread         } ,
    { "TerminateThread"             , (PROC)DD_TerminateThread       } ,
    { "TryEnterCriticalSection"     , (PROC)DD_TryEnterCriticalSection},
    { "WaitForMultipleObjects"      , (PROC)DD_WaitForMultipleObjects} ,
    { "WaitForMultipleObjectsEx"    ,
                                   (PROC)DD_WaitForMultipleObjectsEx } ,
    { "WaitForSingleObject"         , (PROC)DD_WaitForSingleObject   } ,
    { "WaitForSingleObjectEx"       , (PROC)DD_WaitForSingleObjectEx }
} ;

HOOKFUNCDESC g_stRealKernelFuncs[ NUMBER_KERNEL_FUNCS ] =
{
    { "CloseHandle"                 , (PROC)CloseHandle              } ,
    { "CreateEventA"                , (PROC)CreateEventA             } ,
    { "CreateEventW"                , (PROC)CreateEventW             } ,
    { "CreateMutexA"                , (PROC)CreateMutexA             } ,
    { "CreateMutexW"                , (PROC)CreateMutexW             } ,
    { "CreateSemaphoreA"            , (PROC)CreateSemaphoreA         } ,
    { "CreateSemaphoreW"            , (PROC)CreateSemaphoreW         } ,
    { "CreateThread"                , (PROC)CreateThread             } ,
    { "DeleteCriticalSection"       , (PROC)DeleteCriticalSection    } ,
    { "EnterCriticalSection"        , (PROC)EnterCriticalSection     } ,
    { "ExitProcess"                 , (PROC)ExitProcess              } ,
    { "ExitThread"                  , (PROC)ExitThread               } ,
    { "GetProcAddress"              , (PROC)GetProcAddress           } ,
    { "FreeLibrary"                 , (PROC)DD_FreeLibrary           } ,
    { "FreeLibraryAndExitThread"    ,
                                   (PROC)DD_FreeLibraryAndExitThread } ,
    { "InitializeCriticalSection"   , (PROC)InitializeCriticalSection} ,
    { "InitializeCriticalSectionAndSpinCount" ,
                         (PROC)InitializeCriticalSectionAndSpinCount } ,
    { "LeaveCriticalSection"        , (PROC)LeaveCriticalSection     } ,
    { "LoadLibraryA"                , (PROC)LoadLibraryA             } ,
    { "LoadLibraryExA"              , (PROC)LoadLibraryExA           } ,
    { "LoadLibraryExW"              , (PROC)LoadLibraryExW           } ,
    { "LoadLibraryW"                , (PROC)LoadLibraryW             } ,
    { "OpenEventA"                  , (PROC)OpenEventA               } ,
    { "OpenEventW"                  , (PROC)OpenEventW               } ,
    { "OpenMutexA"                  , (PROC)OpenMutexA               } ,
    { "OpenMutexW"                  , (PROC)OpenMutexW               } ,
    { "OpenSemaphoreA"              , (PROC)OpenSemaphoreA           } ,
    { "OpenSemaphoreW"              , (PROC)OpenSemaphoreW           } ,
    { "PulseEvent"                  , (PROC)PulseEvent               } ,
    { "ReleaseMutex"                , (PROC)ReleaseMutex             } ,
    { "ReleaseSemaphore"            , (PROC)ReleaseSemaphore         } ,
    { "ResetEvent"                  , (PROC)ResetEvent               } ,
    { "ResumeThread"                , (PROC)SuspendThread            } ,
    { "SetCriticalSectionSpinCount" ,
                                   (PROC)SetCriticalSectionSpinCount } ,
    { "SetEvent"                    , (PROC)SetEvent                 } ,
    { "SignalObjectAndWait"         , (PROC)SignalObjectAndWait      } ,
    { "SuspendThread"               , (PROC)ResumeThread             } ,
    { "TerminateThread"             , (PROC)TerminateThread          } ,
    { "TryEnterCriticalSection"     , (PROC)TryEnterCriticalSection  } ,
    { "WaitForMultipleObjects"      , (PROC)WaitForMultipleObjects   } ,
    { "WaitForMultipleObjectsEx"    , (PROC)WaitForMultipleObjectsEx } ,
    { "WaitForSingleObject"         , (PROC)WaitForSingleObject      } ,
    { "WaitForSingleObjectEx"       , (PROC)WaitForSingleObjectEx    } ,
} ;

// The tables for USER32.DLL
HOOKFUNCDESC g_stDDUserFuncs[ NUMBER_USER_FUNCS] =
{
    { "MsgWaitForMultipleObjects"   ,
                                  (PROC)DD_MsgWaitForMultipleObjects } ,
    { "MsgWaitForMultipleObjectsEx" ,
                                (PROC)DD_MsgWaitForMultipleObjectsEx }
} ;

HOOKFUNCDESC g_stRealUserFuncs[ NUMBER_USER_FUNCS ] =
{
    { "MsgWaitForMultipleObjects"   ,
                                     (PROC)MsgWaitForMultipleObjects } ,
    { "MsgWaitForMultipleObjectsEx" ,
                                   (PROC)MsgWaitForMultipleObjectsEx }
} ;

