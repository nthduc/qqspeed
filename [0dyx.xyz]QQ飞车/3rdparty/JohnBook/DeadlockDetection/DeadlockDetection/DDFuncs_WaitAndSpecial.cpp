/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The wait and special functions.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "DeadlockDetection.h"
#include "Internal.h"

// Turn off all optimizations to avoid registers being changed behind
// my back.
#pragma optimize( "", off )

// Disable the "conditional expression is constant" in the
// HOOKFN_STARTUP macro.
#pragma warning ( disable : 4127 )

DWORD NAKEDDEF DD_WaitForSingleObject ( HANDLE hHandle        ,
                                        DWORD  dwMilliseconds  )
{
    HOOKFN_STARTUP ( eWaitForSingleObject , DDOPT_WAIT , TRUE ) ;

    WaitForSingleObject ( hHandle , dwMilliseconds ) ;
    
    HOOKFN_SHUTDOWN ( 2 , DDOPT_WAIT ) ;
}

DWORD NAKEDDEF DD_WaitForSingleObjectEx ( HANDLE hHandle        ,
                                          DWORD  dwMilliseconds ,
                                          BOOL   bAlertable      )
{
    HOOKFN_STARTUP ( eWaitForSingleObjectEx , DDOPT_WAIT , TRUE ) ;

    WaitForSingleObjectEx ( hHandle , dwMilliseconds , bAlertable ) ;
    
    HOOKFN_SHUTDOWN ( 3 , DDOPT_WAIT ) ;
}

DWORD NAKEDDEF DD_WaitForMultipleObjects( DWORD          nCount     ,
                                          CONST HANDLE * lpHandles  ,
                                          BOOL           bWaitAll   ,
                                          DWORD          dwMilliseconds)
{
    HOOKFN_STARTUP ( eWaitForMultipleObjects , DDOPT_WAIT , TRUE ) ;

    WaitForMultipleObjects ( nCount         ,
                             lpHandles      ,
                             bWaitAll       ,
                             dwMilliseconds  ) ;
    
    HOOKFN_SHUTDOWN ( 4 , DDOPT_WAIT ) ;
}

DWORD NAKEDDEF DD_WaitForMultipleObjectsEx( DWORD        nCount        ,
                                            CONST HANDLE *lpHandles    ,
                                            BOOL         bWaitAll      ,
                                            DWORD        dwMilliseconds,
                                            BOOL         bAlertable    )
{
    HOOKFN_STARTUP ( eWaitForMultipleObjectsEx , DDOPT_WAIT , TRUE ) ;

    WaitForMultipleObjectsEx ( nCount         ,
                               lpHandles      ,
                               bWaitAll       ,
                               dwMilliseconds ,
                               bAlertable      ) ;
    
    HOOKFN_SHUTDOWN ( 5 , DDOPT_WAIT ) ;
}

DWORD NAKEDDEF DD_MsgWaitForMultipleObjects ( DWORD    nCount         ,
                                              LPHANDLE pHandles       ,
                                              BOOL     fWaitAll       ,
                                              DWORD    dwMilliseconds ,
                                              DWORD    dwWakeMask      )
{
    HOOKFN_STARTUP ( eMsgWaitForMultipleObjects , DDOPT_WAIT , TRUE ) ;

    MsgWaitForMultipleObjects ( nCount         ,
                                pHandles       ,
                                fWaitAll       ,
                                dwMilliseconds ,
                                dwWakeMask      ) ;

    HOOKFN_SHUTDOWN ( 5 , DDOPT_WAIT ) ;
}

DWORD NAKEDDEF DD_MsgWaitForMultipleObjectsEx ( DWORD    nCount        ,
                                                LPHANDLE pHandles      ,
                                                DWORD    dwMilliseconds,
                                                DWORD    dwWakeMask    ,
                                                DWORD    dwFlags       )
{
    HOOKFN_STARTUP ( eMsgWaitForMultipleObjectsEx , DDOPT_WAIT , TRUE ) ;

    MsgWaitForMultipleObjectsEx ( nCount        ,
                                  pHandles      ,
                                  dwMilliseconds,
                                  dwWakeMask    ,
                                  dwFlags        ) ;

    HOOKFN_SHUTDOWN ( 5 , DDOPT_WAIT ) ;
}


DWORD NAKEDDEF DD_SignalObjectAndWait ( HANDLE hObjectToSignal,
                                        HANDLE hObjectToWaitOn,
                                        DWORD  dwMilliseconds,
                                        BOOL   bAlertable )
{

    HOOKFN_STARTUP ( eSignalObjectAndWait , DDOPT_WAIT , TRUE ) ;

    SignalObjectAndWait ( hObjectToSignal ,
                          hObjectToWaitOn ,
                          dwMilliseconds  ,
                          bAlertable       ) ;

    HOOKFN_SHUTDOWN ( 4 , DDOPT_WAIT ) ;
}

BOOL NAKEDDEF DD_CloseHandle ( HANDLE hObject )
{
    HOOKFN_STARTUP ( eCloseHandle , DDOPT_WAIT , TRUE ) ;

    CloseHandle ( hObject ) ;
    
    HOOKFN_SHUTDOWN ( 1 , DDOPT_WAIT ) ;
}

// Turn optimizations back on.
#pragma optimize( "", on )
