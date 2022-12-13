/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The File Logging Deadlock Detection Extension DLL.
----------------------------------------------------------------------*/
#include "PCH.h"
#include "DeadlockDetection.h"
#include "ParamDecode.h"

/*//////////////////////////////////////////////////////////////////////
                    Type Definitions and Prototypes
//////////////////////////////////////////////////////////////////////*/
// The definition for all parameter output functions.
typedef int (*PFNOUTPUTFUNC)( LPTSTR    szBuff   ,
                              DWORD     dwParams  ) ;

// The array of output items.
typedef struct tagOutputData
{
    LPCTSTR         szName ;
    PFNOUTPUTFUNC   pProc  ;
} OUTPUTDATA , *LPOUTPUTDATA ;

// The output items for each function enum.  Keep this in the same order
// as the eFuncEnum list in DeadlockDetection.h.
OUTPUTDATA g_stOutData [ ] =
{
    { _T ( "***UNINITIALIZED FUNCTION ENUM!***" ) , NULL } ,
// Special functions that are always reported.
    { _T ( "ExitProcess" ) ,
      Trans_ExitProcessParams
    } ,
// Waiting and special functions that are always reported.
    { _T ( "WaitForSingleObject" ) ,
      Trans_WaitForSingleObjectParams
    } ,
    { _T ( "WaitForSingleObjectEx" ) ,
      Trans_WaitForSingleObjectExParams
    } ,
    { _T ( "WaitForMultipleObjects" ) ,
      Trans_WaitForMultipleObjectsParams
    } ,
    { _T ( "WaitForMultipleObjectsEx" ) ,
      Trans_WaitForMultipleObjectsExParams
    } ,
    { _T ( "MsgWaitForMultipleObjects" ) ,
      Trans_MsgWaitForMultipleObjectsParams
    } ,
    { _T ( "MsgWaitForMultipleObjectsEx" ) ,
      Trans_MsgWaitForMultipleObjectsExParams
    } ,
    { _T ( "SignalObjectAndWait" ) ,
      Trans_SignalObjectAndWaitParams
    } ,
    { _T ( "CloseHandle" ) ,
      Trans_CloseHandleParams
    } ,
    { _T ( "GetProcAddress" ) ,
      Trans_GetProcAddressParams
    } ,
// Thread specific functions.
    { _T ( "CreateThread" ) ,
      Trans_CreateThreadParams
    } ,
    { _T ( "ExitThread" ) ,
      Trans_ExitThreadParams
    } ,
    { _T ( "SuspendThread" ) ,
      Trans_SuspendThreadParams
    } ,
    { _T ( "ResumeThread" ) ,
      Trans_ResumeThreadParams
    } ,
    { _T ( "TerminateThread" ) ,
      Trans_TerminateThreadParams
    } ,
    { _T ( "FreeLibraryAndExitThread" ) ,
      Trans_FreeLibraryAndExitThread
    } ,
    { _T ( "_beginthreadex" ) ,
      Trans_beginthreadex
    } ,
    { _T ( "_endthreadex" ) ,
      Trans_endthreadex
    } ,
    { _T ( "_beginthread" ) ,
      Trans_beginthread
    } ,
    { _T ( "_endthread" ) ,
      Trans_endthread
    } ,
// CriticalSection functions.
    { _T ( "InitializeCriticalSection" ) ,
      Trans_InitializeCriticalSectionParams
    } ,
    { _T ( "InitializeCriticalSectionAndSpinCount" ) ,
      Trans_InitializeCriticalSectionAndSpinCountParams
    } ,
    { _T ( "DeleteCriticalSection" ) ,
      Trans_DeleteCriticalSectionParams
    } ,
    { _T ( "EnterCriticalSection" ) ,
      Trans_EnterCriticalSectionParams
    } ,
    { _T ( "LeaveCriticalSection" ) ,
      Trans_LeaveCriticalSectionParams
    } ,
    { _T ( "SetCriticalSectionSpinCount" ) ,
      Trans_SetCriticalSectionSpinCountParams
    } ,
    { _T ( "TryEnterCriticalSection" ) ,
      Trans_TryEnterCriticalSectionParams
    } ,
// Mutex functions.
    { _T ( "CreateMutexA" ) ,
      Trans_CreateMutexAParams
    } ,
    { _T ( "CreateMutexW" ) ,
      Trans_CreateMutexWParams
    } ,
    { _T ( "OpenMutexA" ) ,
      Trans_OpenMutexAParams
    } ,
    { _T ( "OpenMutexW" ) ,
      Trans_OpenMutexWParams
    } ,
    { _T ( "ReleaseMutex" ) ,
      Trans_ReleaseMutexParams
    } ,
// Semaphore functions.
    { _T ( "CreateSemaphoreA" ) ,
      Trans_CreateSemaphoreAParams
    } ,
    { _T ( "CreateSemaphoreW" ) ,
      Trans_CreateSemaphoreWParams
    } ,
    { _T ( "OpenSemaphoreA" ) ,
      Trans_OpenSemaphoreAParams
    } ,
    { _T ( "OpenSemaphoreW" ) ,
      Trans_OpenSemaphoreWParams
    } ,
    { _T ( "ReleaseSemaphore" ) ,
      Trans_ReleaseSemaphoreParams
    } ,
// Event functions.
    { _T ( "CreateEventA" ) ,
      Trans_CreateEventAParams
    } ,
    { _T ( "CreateEventW" ) ,
      Trans_CreateEventWParams
    } ,
    { _T ( "OpenEventA" ) ,
      Trans_OpenEventAParams
    } ,
    { _T ( "OpenEventW" ) ,
      Trans_OpenEventWParams
    } ,
    { _T ( "PulseEvent" ) ,
      Trans_PulseEventParams
    } ,
    { _T ( "ResetEvent" ) ,
      Trans_ResetEventParams
    } ,
    { _T ( "SetEvent" ) ,
      Trans_SetEventParams
    } ,
} ;

static LPCTSTR k_HEADER =
  _T ( "TID        Ret Addr     C/R Ret Value  Function & Params\r\n" );
static LPCTSTR k_CRLF = _T ( "\r\n" ) ;

/*//////////////////////////////////////////////////////////////////////
                          Function Prototypes
//////////////////////////////////////////////////////////////////////*/
LPCTSTR ConvertFuncEnumToString ( eFuncEnum eFunc ) ;
LPCTSTR ConvertPrePostToString ( ePrePostEnum ePrePost ) ;

// The handle to the main file.
HANDLE g_hFile = INVALID_HANDLE_VALUE ;

BOOL WINAPI DeadDetProcessEvent ( const LPDDEVENTINFO pEvtInfo )
{
    // Double check that the file has not been closed on us.
    if ( INVALID_HANDLE_VALUE == g_hFile )
    {
        ASSERT ( !"DeadDetProcessEvent : file handle closed!" ) ;
        return ( FALSE ) ;
    }
    TCHAR szBuff[ MAX_PATH ] ;
    int iCurr = 0 ;

    // Put in the thread ID, the return address and if this is a pre or
    // post function call.
    iCurr += wsprintf ( szBuff + iCurr              ,
                        _T ( "0x%08X [0x%08X] %s" ) ,
                        pEvtInfo->dwThreadId        ,
                        pEvtInfo->dwAddr            ,
                        ConvertPrePostToString ( pEvtInfo->ePrePost ) );

    // If this is post call, then put in the returned value.
    if ( ePostCall == pEvtInfo->ePrePost )
    {
        iCurr += wsprintf ( szBuff + iCurr       ,
                            _T ( " 0x%08X" )     ,
                            pEvtInfo->dwRetValue  ) ;
    }
    else
    {
        iCurr += wsprintf ( szBuff + iCurr      ,
                            _T ( "           " )  ) ;

    }

    // Slap in the function name.
    if ( pEvtInfo->eFunc >= eMAXFUNCTIONENUM )
    {
        ASSERT ( !"**OUT OF RANGE FUNC ENUM*** " ) ;
        iCurr += wsprintf ( szBuff + iCurr ,
                            _T ( "**OUT OF RANGE FUNC ENUM*** " ) ) ;
    }
    else
    {
        iCurr += wsprintf ( szBuff + iCurr    ,
                            _T ( " %s " )     ,
                            g_stOutData[ pEvtInfo->eFunc ].szName ) ;
    }

    // Call the decoding function for the parameters.
    if ( NULL != g_stOutData[ pEvtInfo->eFunc ].pProc )
    {
        iCurr +=
            g_stOutData[pEvtInfo->eFunc].pProc( szBuff + iCurr     ,
                                                pEvtInfo->dwParams  ) ;
    }

    // Put on the CR/LF on the end.
    lstrcpy ( szBuff + iCurr , _T ( "\r\n" ) ) ;
    iCurr += 2 ;

    // Write it out to the file.
    DWORD dwWritten = 0 ;
    BOOL bRet = WriteFile ( g_hFile                  ,
                            szBuff                   ,
                            iCurr * sizeof ( TCHAR ) ,
                            &dwWritten               ,
                            NULL                      ) ;
    ASSERT ( TRUE == bRet ) ;

    // If this is any kind of wait function, flush the file in case
    // this is the one call that deadlocks.
    if ( TRUE == bRet )
    {
        switch ( pEvtInfo->eFunc )
        {
            case eWaitForSingleObject         :
            case eWaitForSingleObjectEx       :
            case eWaitForMultipleObjects      :
            case eWaitForMultipleObjectsEx    :
            case eMsgWaitForMultipleObjects   :
            case eMsgWaitForMultipleObjectsEx :
            case eSignalObjectAndWait         :
            case eEnterCriticalSection        :
            case eTryEnterCriticalSection     :
                bRet = FlushFileBuffers ( g_hFile ) ;
                ASSERT ( TRUE == bRet ) ;
                break ;
            default :
                break ;
        }
    }
    return ( bRet ) ;
}

BOOL WINAPI DeadDetWriteToLog ( LPCTSTR szMsg )
{
    // Double check that the file has not been closed on us.
    if ( INVALID_HANDLE_VALUE == g_hFile )
    {
        ASSERT ( !"DeadDetProcessEvent : file handle closed!" ) ;
        return ( FALSE ) ;
    }
    if ( TRUE == IsBadStringPtr ( szMsg , MAX_PATH ) )
    {
        ASSERT ( !"szMsg parameter is bad!!" ) ;
        return ( FALSE ) ;
    }
    // Write it out to the file.
    DWORD dwWritten = 0 ;
    size_t iLen = _tcslen ( szMsg ) ;
    BOOL bRet = WriteFile ( g_hFile                         ,
        szMsg                           ,
        (DWORD)iLen * sizeof ( TCHAR )  ,
        &dwWritten                      ,
        NULL                             ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( TRUE == bRet )
    {
        bRet = WriteFile ( g_hFile                                   ,
                           k_CRLF                                    ,
                           (DWORD)_tcslen ( k_CRLF ) * sizeof(TCHAR) ,
                           &dwWritten                                ,
                           NULL                                       );
        ASSERT ( TRUE == bRet ) ;
    }
    return ( bRet ) ;
}

////////////////////////////////////////////////////////////////////////
//                        Necessary Functions
////////////////////////////////////////////////////////////////////////
extern "C" BOOL WINAPI DllMain ( HINSTANCE hInst       ,
                                 DWORD     dwReason    ,
                                 LPVOID    /*lpReserved*/ )
{
    BOOL bRet = TRUE ;
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH :
            DisableThreadLibraryCalls ( hInst ) ;
            break ;
        case DLL_PROCESS_DETACH :
            DeadDetExtClose ( ) ;
            break ;
        default                 :
            break ;
    }
    return ( bRet ) ;
}

BOOL WINAPI DeadDetExtOpen ( void )
{
    // Is the file already open?
    if ( INVALID_HANDLE_VALUE != g_hFile )
    {
        ASSERT ( !"DeadDetExtOpen called multiple times!" ) ;
        return ( FALSE ) ;
    }

    BOOL bRet = FALSE ;

    // Create the file in the same directory as the EXE.
    TCHAR szFile [ MAX_PATH ] ;
    GetModuleFileName ( NULL , szFile , MAX_PATH ) ;

    LPTSTR szPeriod = _tcsrchr ( szFile , _T ( '.' ) ) ;
    if ( NULL != szPeriod )
    {
        szPeriod++ ;
        *szPeriod = _T ( '\0' ) ;
        _tcscpy ( szPeriod , _T ( "DD" ) ) ;

        g_hFile = CreateFile ( szFile                           ,
                               GENERIC_READ | GENERIC_WRITE     ,
                               FILE_SHARE_READ                  ,
                               NULL                             ,
                               CREATE_ALWAYS                    ,
                               FILE_ATTRIBUTE_ARCHIVE           ,
                               NULL                              ) ;
        ASSERT ( INVALID_HANDLE_VALUE != g_hFile ) ;
        if ( INVALID_HANDLE_VALUE != g_hFile )
        {
            DWORD dwWritten = 0 ;
            size_t iBytes = ( _tcslen ( k_HEADER ) ) *
                                        sizeof ( TCHAR ) ;
            bRet = WriteFile ( g_hFile          ,
                               k_HEADER         ,
                               (DWORD)iBytes    ,
                               &dwWritten       ,
                               NULL              ) ;
            ASSERT ( TRUE == bRet ) ;
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

void WINAPI DeadDetExtClose ( void )
{
    if ( INVALID_HANDLE_VALUE != g_hFile )
    {
        VERIFY ( CloseHandle ( g_hFile ) ) ;
    }
    g_hFile = INVALID_HANDLE_VALUE ;
}

LPCTSTR ConvertFuncEnumToString ( eFuncEnum eFunc )
{
    if ( eFunc >= eMAXFUNCTIONENUM )
    {
        return ( _T ( "***OUT OF RANGE FUNC ENUM***" ) ) ;
    }
    return ( g_stOutData[ eFunc ].szName ) ;
}

static LPCTSTR g_szPrePostString [ ] =
{
    _T ( "***UNINITIALIZED PRE POST***" ) ,
    _T ( "(C)" ) ,
    _T ( "(R)" ) ,
} ;

LPCTSTR ConvertPrePostToString ( ePrePostEnum ePrePost )
{
    if ( ePrePost >= eMAXPREPOSTENUM )
    {
        return ( _T ( "***OUT OF RANGE PREPOST ENUM***" ) ) ;
    }
    return ( g_szPrePostString [ ePrePost ] ) ;
}

