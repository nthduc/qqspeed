/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBGUser.h"

// The maximum amount of time I will let SendMessageTimeout wait.
#define DBG_TIMEOUT (1000*1000)

CWDBGUser :: CWDBGUser ( void )
{
    m_hMainWnd = NULL ;
    m_hOutputWnd = NULL ;
    m_hWaitEvent = NULL ;

    for ( int i = 0 ; i < k_NUMPOSTITEMS ; i++ )
    {
        m_aszOutBuffs[ i ] = new TCHAR [ k_BUFFSIZE ] ;
    }

    m_iCurrBuff = 0 ;
}

CWDBGUser :: ~CWDBGUser ( void )
{
    if ( NULL != m_hWaitEvent )
    {
        VERIFY ( CloseHandle ( m_hWaitEvent ) ) ;
    }

    for ( int i = 0 ; i < k_NUMPOSTITEMS ; i++ )
    {
        delete [] m_aszOutBuffs[ i ] ;
    }
}

// Initializes this class.
BOOL CWDBGUser :: Initialize ( HWND    hMainWnd    ,
                               HWND    hOutputWnd  ,
                               LPCTSTR szEventName  )
{
    // Normally I would check IsWindow against TRUE but on Win9x,
    // IsWindow returns something else.
    ASSERT ( ::IsWindow ( hMainWnd ) ) ;
    ASSERT ( ::IsWindow ( hOutputWnd ) ) ;
    ASSERT ( FALSE == IsBadStringPtr ( szEventName , MAX_PATH ) ) ;

    // Set the HWNDs.
    m_hMainWnd   = hMainWnd   ;
    m_hOutputWnd = hOutputWnd ;

    // Open the event.
    m_hWaitEvent = ::OpenEvent ( EVENT_ALL_ACCESS , 0 , szEventName ) ;
    ASSERT ( NULL != m_hWaitEvent ) ;

    return ( NULL != m_hWaitEvent ) ;
}

BOOL CWDBGUser :: IsInitialized ( void )
{
    return ( NULL != m_hWaitEvent ) ;
}

BOOL CWDBGUser :: SendMsgToUI ( UINT    uiMsg    ,
                                WPARAM  wParam   ,
                                LPARAM  lParam    )
{
// As I mentioned in WDGBUser.h, you could also use SendMessageTimeout
// here.  However, it makes it harder to debug because the API can
// timeout while you are stopped in the debugger.  While SendMessage
// works fine, it can obviously lead to deadlocks.
// Using SendMessageTimeout could be a little better because if the
// debug thread failed to get the message over, it could terminate
// itself.

//    DWORD dwSMTRes ;
//    DWORD dwMsgRes ;
//    dwSMTRes = ::SendMessageTimeout ( m_hMainWnd         ,
//                                      uiMsg              ,
//                                      wParam             ,
//                                      lParam             ,
//                                      SMTO_ABORTIFHUNG   ,
//                                      DBG_TIMEOUT        ,
//                                      &dwMsgRes           ) ;
//    SUPERASSERT ( ( 0 != dwSMTRes ) ) ;
//    // If the message timed out, I've got a serious problem.
//    if ( ( 0 == dwSMTRes ) && ( 0 == GetLastError ( ) ) )
//    {
//        // Return zero so that the debug loop bails out.
//        return ( FALSE ) ;
//    }
    ::SendMessage ( m_hMainWnd , uiMsg , wParam , lParam ) ;
    return ( TRUE ) ;
}

// A general textual output function.
void CWDBGUser :: Output ( LPCTSTR szFmt , ... )
{
    LPTSTR pOutBuff = m_aszOutBuffs [ m_iCurrBuff ] ;
    m_iCurrBuff++ ;
    if ( k_NUMPOSTITEMS == m_iCurrBuff )
    {
        m_iCurrBuff = 0 ;
    }

    va_list  args ;

    va_start ( args , szFmt ) ;

    wvsprintf ( pOutBuff , szFmt , args ) ;

    va_end ( args ) ;

    ::PostMessage ( m_hOutputWnd      ,
                    WM_DBG_OUTPUT     ,
                    0                 ,
                    (LPARAM)pOutBuff   ) ;
}

LPCTSTR CWDBGUser :: ConvertCRLF ( LPTSTR szBuff , int iBuffLen )
{
    // Convert \n into \r\n, but don't screw up any \r\n sequences.
    TCHAR * szTempBuff = new TCHAR [ iBuffLen * 2 ] ;
    ZeroMemory ( szTempBuff , sizeof ( TCHAR ) * iBuffLen * 2 ) ;

    int iCurrBuff = 0 ;
    int iTempBuff = 0 ;

    while ( ( iCurrBuff < iBuffLen               ) &&
            ( _T ( '\0' ) != szBuff[ iCurrBuff ] )    )
    {
        if ( _T( '\n' ) == szBuff[ iCurrBuff ] )
        {
            szTempBuff[ iTempBuff ] = _T ( '\r' ) ;
            iTempBuff++ ;
        }
        else if ( ( _T ( '\r' ) == szBuff[ iCurrBuff ]     ) &&
                  ( _T ( '\n' ) == szBuff[ iCurrBuff + 1 ] )    )
        {
            szTempBuff[ iTempBuff ] = _T ( '\r' ) ;
            iTempBuff++ ;
            iCurrBuff++ ;
        }
        szTempBuff[ iTempBuff ] = szBuff[ iCurrBuff ] ;
        iTempBuff++ ;
        iCurrBuff++ ;
    }
    return ( szTempBuff ) ;
}

void CWDBGUser :: ExitProcessEvent ( DWORD dwProcessId ,
                                     DWORD dwThreadId  ,
                                     DWORD dwExitCode   )
{
    STEXITPROCESSEVENT stEPE ;
    stEPE.dwExitCode = dwExitCode ;
    stEPE.dwThreadId = dwThreadId ;

    Output ( _T ( "Process exited and returned 0x%08X" ) ,
             dwExitCode                                   ) ;
    OutputCRLF ( ) ;

    // It does not matter if SendMsgToUI fails.  The debug thread is
    // ending no matter what.
    VERIFY ( SendMsgToUI ( WM_DBG_EXITPROCESSEVENT ,
                           dwProcessId             ,
                           (LPARAM)&stEPE           ) ) ;

    // No need to return anything here as the debug loop is going away
    // on its own.
}

// The exception handler.
DWORD CWDBGUser :: ExceptionEvent ( DWORD                  dwProcessId ,
                                    DWORD                  dwThreadId  ,
                                    EXCEPTION_DEBUG_INFO & stEDI       )
{
    STEXCEPTIONEVENT stEE ;
    stEE.dwThreadId = dwThreadId ;
    stEE.pEDI = &stEDI ;

    DWORD dwRet = DBG_EXCEPTION_NOT_HANDLED ;
    stEE.pdwReturnVal = &dwRet ;

    BOOL bRet = SendMsgToUI ( WM_DBG_EXCEPTIONEVENT ,
                              dwProcessId           ,
                              (LPARAM)&stEE          ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }
    // Wait for the synchronization event to get signaled.
    ::WaitForSingleObject ( m_hWaitEvent , INFINITE ) ;
    VERIFY ( ResetEvent ( m_hWaitEvent ) ) ;

    return ( dwRet ) ;
}

DWORD CWDBGUser :: CreateProcessEvent ( DWORD               dwProcessId,
                                        DWORD               dwThreadId ,
                                     CREATE_PROCESS_DEBUG_INFO & stCPDI)
{
    STCREATEPROCESSEVENT stCPE ;
    stCPE.dwThreadId = dwThreadId ;
    stCPE.pCPDI = &stCPDI ;

    DWORD dwRet = DBG_CONTINUE ;
    stCPE.pdwReturnVal = &dwRet ;

    BOOL bRet = SendMsgToUI ( WM_DBG_CREATEPROCESSEVENT ,
                              dwProcessId               ,
                              (LPARAM)&stCPE            ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }
    ::WaitForSingleObject ( m_hWaitEvent , INFINITE ) ;
    VERIFY ( ResetEvent ( m_hWaitEvent ) ) ;
    return ( dwRet ) ;
}


DWORD CWDBGUser :: CreateThreadEvent ( DWORD                dwProcessId,
                                       DWORD                dwThreadId ,
                                      CREATE_THREAD_DEBUG_INFO & stCTDI)
{
    STCREATETHREADEVENT stCTE ;
    stCTE.dwThreadId = dwThreadId ;
    stCTE.pCTDI = &stCTDI ;

    DWORD dwRet = DBG_CONTINUE ;
    stCTE.pdwReturnVal = &dwRet ;

    BOOL bRet = SendMsgToUI ( WM_DBG_CREATETHREADEVENT ,
                              dwProcessId              ,
                              (LPARAM)&stCTE            ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }
    ::WaitForSingleObject ( m_hWaitEvent , INFINITE ) ;
    VERIFY ( ResetEvent ( m_hWaitEvent ) ) ;
    return ( dwRet ) ;
}

DWORD CWDBGUser :: ExitThreadEvent ( DWORD            dwProcessId ,
                                     DWORD            dwThreadId  ,
                                     DWORD            dwExitCode   )
{
    STEXITTHREADEVENT stETE ;
    stETE.dwThreadId = dwThreadId ;
    stETE.dwExitCode = dwExitCode ;

    DWORD dwRet = DBG_CONTINUE ;
    stETE.pdwReturnVal = &dwRet ;

    BOOL bRet = SendMsgToUI ( WM_DBG_EXITTHREADEVENT ,
                              dwProcessId            ,
                              (LPARAM)&stETE          ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }
    ::WaitForSingleObject ( m_hWaitEvent , INFINITE ) ;
    VERIFY ( ResetEvent ( m_hWaitEvent ) ) ;
    return ( dwRet ) ;
}

DWORD CWDBGUser :: LoadDllEvent ( DWORD                 dwProcessId ,
                                  DWORD                 dwThreadId  ,
                                  LOAD_DLL_DEBUG_INFO & stLDDI      ,
                                  LPCTSTR               szName      ,
                                  IMAGEHLP_MODULE64 &   ModInfo      )
{
    STLOADDLLEVENT stLDE ;
    stLDE.dwThreadId = dwThreadId ;
    stLDE.pLDDI = &stLDDI ;
    _tcscpy ( stLDE.szName , szName ) ;
    stLDE.pModInfo = &ModInfo ;

    DWORD dwRet = DBG_CONTINUE ;
    stLDE.pdwReturnVal = &dwRet ;

    BOOL bRet = SendMsgToUI ( WM_DBG_LOADDLLEVENT ,
                              dwProcessId         ,
                              (LPARAM)&stLDE       ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }
    ::WaitForSingleObject ( m_hWaitEvent , INFINITE ) ;
    VERIFY ( ResetEvent ( m_hWaitEvent ) ) ;
    return ( dwRet ) ;
}

DWORD CWDBGUser :: UnloadDllEvent ( DWORD   dwProcessId ,
                                    DWORD   dwThreadId  ,
                                    LPCVOID lpBaseOfDll  )
{
    STUNLOADDLLEVENT stUDE ;
    stUDE.dwBaseAddr = lpBaseOfDll ;
    stUDE.dwThreadId = dwThreadId ;

    DWORD dwRet = DBG_CONTINUE ;
    stUDE.pdwReturnVal = &dwRet ;

    BOOL bRet = SendMsgToUI ( WM_DBG_UNLOADDLLEVENT ,
                              dwProcessId           ,
                              (LPARAM)&stUDE         ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }
    ::WaitForSingleObject ( m_hWaitEvent , INFINITE ) ;
    VERIFY ( ResetEvent ( m_hWaitEvent ) ) ;
    return ( dwRet ) ;
}

DWORD CWDBGUser :: OutputDebugStringEvent ( DWORD   dwProcessId ,
                                            DWORD   dwThreadId  ,
                                            LPCTSTR szString     )
{
    STOUTPUTDEBUGSTRINGEVENT stODSE ;
    
    stODSE.dwThreadId = dwThreadId ;
    stODSE.szString = szString ;

    DWORD dwRet = DBG_CONTINUE ;
    stODSE.pdwReturnVal = &dwRet ;
    
    BOOL bRet = SendMsgToUI ( WM_DBG_OUTPUTDEBUGSTRINGEVENT ,
                              dwProcessId                   ,
                              (LPARAM)&stODSE                ) ;
    if ( FALSE == bRet )
    {
        return ( FALSE ) ;
    }
    ::WaitForSingleObject ( m_hWaitEvent , INFINITE ) ;
    VERIFY ( ResetEvent ( m_hWaitEvent ) ) ;
    return ( dwRet ) ;
}
                                            
                                            


