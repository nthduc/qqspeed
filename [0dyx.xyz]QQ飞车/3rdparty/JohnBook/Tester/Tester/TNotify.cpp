/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "TNotify.h"


// TNotify

STDMETHODIMP TNotify :: AddNotification ( AddNotifyType   NotifType  ,
                                          AddNotifySearch SearchType ,
                                          BSTR            WinStr     ,
                                          VARIANT_BOOL *  pVal        )
{
    NotifyType = NotifyType ;
    SearchType = SearchType ;

    HRESULT hr = S_OK ;

    if ( NULL != WinStr )
    {
        hItem = AddNotify ( this , NotifType , SearchType , WinStr ) ;
        if ( INVALID_HANDLE_VALUE != hItem )
        {
            NotifyType = NotifType ;
        }
        else
        {
            hr = E_FAIL ;
        }
    }
    else
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    
    if ( SUCCEEDED ( hr ) )
    {
        *pVal = VARIANT_TRUE ;
    }
    else
    {
        *pVal = VARIANT_FALSE ;
    }
    return ( hr ) ;
}

STDMETHODIMP TNotify :: CheckNotification ( void )
{
    SpinTheTimerLoop ( 5 ) ;
    return ( S_OK ) ;
}

STDMETHODIMP TNotify :: ClearNotification ( void )
{
    RemoveNotify ( this , hItem ) ;
    hItem = INVALID_HANDLE_VALUE ;
    return ( S_OK ) ;
}

typedef CSimpleArray < TNotify * > TNotifyArray ;

// The array of TNotify objects.
TNotifyArray g_tnArray ;
// The timer ID.
UINT_PTR g_iTimerID = 0 ;

VOID CALLBACK CheckWinTimerProc ( HWND     /*hWnd   */  ,
                                  UINT     /*uMsg   */  ,
                                  UINT_PTR /*idEvent*/  ,
                                  DWORD    /*dwTime */   )
{
    for ( int i = 0 ; i < g_tnArray.GetSize ( ) ; i++ )
    {
        TNotify * pCurr = g_tnArray[ i ] ;
        HWND hRet = CheckNotifyCreateTitle ( pCurr->hItem ) ;
        BOOL bRet = CheckNotifyDestroyTitle ( pCurr->hItem ) ;
        
        if ( TRUE == bRet )
        {
            // Does this particular notification want to hear about
            // destroys?
            if ( ( antDestroyWindow    == pCurr->NotifyType ) ||
                 ( antCreateAndDestroy == pCurr->NotifyType )   )
            {
                pCurr->RaiseDestroyEvent ( ) ;
            }
        }
        if ( NULL != hRet )
        {
            // Am I supposed to pass on creates?
            if ( ( antCreateWindow     == pCurr->NotifyType ) ||
                 ( antCreateAndDestroy == pCurr->NotifyType )   )
            {
                // Gotta create the TWindow.
                ITWindow * pWin = NULL ;
                CComObject<TWindow> * p = new CComObject<TWindow> ( ) ;
                HRESULT hr = p->QueryInterface ( __uuidof (ITWindow ) ,
                                                (LPVOID*)&pWin        );
                if ( SUCCEEDED ( hr ) )
                {
                    pWin->put_hWnd ( (LONG_PTR)hRet ) ;
                    pCurr->RaiseCreateEvent ( pWin ) ;
                }
                else
                {
                    delete p ;
                }
            }
        }
    }
}


HANDLE AddNotify ( TNotify *       tNot         ,
                   AddNotifyType   NotifyType   ,
                   AddNotifySearch NotifySearch ,
                   BSTR            Title         )
{
    HANDLE hRet = INVALID_HANDLE_VALUE ;
    
    CComBSTR cTitle ( Title ) ;
    // Call into the notify helper DLL.
    hRet = AddNotifyTitle ( NotifyType , NotifySearch , cTitle ) ;
    if ( INVALID_HANDLE_VALUE != hRet )
    {
        // Got a slot so bump up the count on the object as I'll poink
        // it into my array.
        tNot->AddRef ( ) ;
        g_tnArray.Add ( tNot ) ;
        
        // If this is the first item, crank up the timer loop.
        if ( 1 == g_tnArray.GetSize ( ) )
        {
            g_iTimerID = SetTimer ( NULL               ,
                                    0                  ,
                                    100                ,
                                    &CheckWinTimerProc  ) ;
            if ( NULL == g_iTimerID )
            {
                tNot->Release ( ) ;
                RemoveNotifyTitle ( hRet ) ;
                hRet = INVALID_HANDLE_VALUE ;
            }
        }
    }
    
    return ( hRet ) ;
}
void RemoveNotify ( TNotify * tNot , HANDLE hItem )
{
    RemoveNotifyTitle ( hItem ) ;
    
    BOOL bRet = g_tnArray.Remove ( tNot ) ;
    ASSERT ( TRUE == bRet ) ;
    // Optimized out, but makes the release build happy.
    bRet = bRet ;
    
    // If this is the last one, kill the timer.
    if ( 0 == g_tnArray.GetSize ( ) )
    {
        KillTimer ( NULL , g_iTimerID ) ;
        g_iTimerID = 0 ;
    }
}

void SpinTheTimerLoop ( LONG lTimes )
{
    for ( LONG i = 0 ; i < lTimes ; i++ )
    {
        // Spin the timer loop.
        CheckWinTimerProc ( NULL , 0 , 0 , 0 ) ;
    }
}