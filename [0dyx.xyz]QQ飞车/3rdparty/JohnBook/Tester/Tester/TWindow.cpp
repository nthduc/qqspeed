/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "Resource.h"
#include "TWindow.h"
#include "TWindows.h"

HRESULT TWindow :: Error ( UINT uiResID , HRESULT hRes )
{
    return ( AtlReportError ( __uuidof ( ITWindow ) ,
                              uiResID               ,
                              GUID_NULL             ,
                              hRes                   ) ) ;
}


STDMETHODIMP TWindow :: get_hWnd ( LONG_PTR * pVal )
{
    *pVal = (LONG_PTR)m_hWnd ;
    return ( S_OK ) ;
}

STDMETHODIMP TWindow :: put_hWnd ( LONG_PTR newVal )
{
    m_hWnd = (HWND)newVal ;
    return ( S_OK ) ;
}

STDMETHODIMP TWindow :: get_Caption ( BSTR * pVal )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        TCHAR szBuff [ MAX_PATH ] ;
        szBuff[ 0 ] = _T ( '\0' ) ;
        GetWindowText ( m_hWnd , szBuff , MAX_PATH ) ;
        CComBSTR cBStr ( szBuff ) ;
        *pVal = cBStr.Copy ( ) ;
        hr = S_OK ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Title ( BSTR * pVal )
{
    return ( get_Caption ( pVal ) ) ;
}

STDMETHODIMP TWindow :: get_ClassName (BSTR * pVal )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        TCHAR szBuff [ MAX_PATH ] ;
        int iRet = GetClassName ( m_hWnd , szBuff , MAX_PATH ) ;
        if ( 0 == iRet )
        {
            hr = Error ( IDS_UNEXPECTEDERROR , E_UNEXPECTED ) ;
        }
        else
        {
            CComBSTR cBStr ( szBuff ) ;
            *pVal = cBStr.Copy ( ) ;
            hr = S_OK ;
        }
    }
    return ( hr ) ;
}

// Helper for getting window rects.
HRESULT TWindow :: GetWindowRect ( LONG * pVal , LPRECT lpRect )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        // Always set a default value.
        *pVal = 0 ;

        BOOL bRet = ::GetWindowRect ( m_hWnd , lpRect ) ;
        if ( FALSE == bRet )
        {
            hr = Error ( IDS_UNEXPECTEDERROR , E_UNEXPECTED ) ;
        }
        else
        {
            hr = S_OK ;
        }
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Top ( LONG * pVal )
{
    RECT r ;
    HRESULT hr = GetWindowRect ( pVal , &r ) ;
    if ( S_OK == hr )
    {
        *pVal = r.top ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Bottom ( LONG * pVal )
{
    RECT r ;
    HRESULT hr = GetWindowRect ( pVal , &r ) ;
    if ( S_OK == hr )
    {
        *pVal = r.bottom ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Left ( LONG * pVal )
{
    RECT r ;
    HRESULT hr = GetWindowRect ( pVal , &r ) ;
    if ( S_OK == hr )
    {
        *pVal = r.left ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Right ( LONG * pVal )
{
    RECT r ;
    HRESULT hr = GetWindowRect ( pVal , &r ) ;
    if ( S_OK == hr )
    {
        *pVal = r.right ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Valid ( VARIANT_BOOL * pVal )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        BOOL bRet = ::IsWindow ( m_hWnd ) ;
        if ( TRUE == bRet )
        {
            *pVal = VARIANT_TRUE ;
        }
        else
        {
            *pVal = VARIANT_FALSE ;
        }
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Enabled ( VARIANT_BOOL * pVal )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        BOOL bRet = ::IsWindowEnabled ( m_hWnd ) ;
        if ( TRUE == bRet )
        {
            *pVal = VARIANT_TRUE ;
        }
        else
        {
            *pVal = VARIANT_FALSE ;
        }
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow::get_Parent ( ITWindow ** pVal )
{
    HRESULT hr = S_OK ;
    // Try to get the parent.
    HWND hParent = GetParent ( m_hWnd ) ;
    if ( NULL == hParent )
    {
        *pVal = NULL ;
    }
    else
    {
        CComObject<TWindow> * p = new CComObject<TWindow> ( ) ;
        hr = p->QueryInterface ( __uuidof (ITWindow ) , (LPVOID*)pVal );
        if ( SUCCEEDED ( hr ) )
        {
            hr = (*pVal)->put_hWnd ( (LONG_PTR)hParent ) ;
        }
        else
        {
            delete p ;
            hr = Error ( IDS_UNEXPECTEDERROR , E_UNEXPECTED ) ;
        }
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_ProcessId ( LONG * pVal )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        DWORD dwPID = 0 ;
        DWORD dwTID = GetWindowThreadProcessId ( m_hWnd , &dwPID ) ;
        if ( 0 == dwTID )
        {
            *pVal = 0 ;
        }
        else
        {
            *pVal = dwPID ;
            hr = S_OK ;
        }
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_ThreadId ( LONG * pVal )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        DWORD dwTID = GetWindowThreadProcessId ( m_hWnd , NULL ) ;
        if ( 0 == dwTID )
        {
            *pVal = 0 ;
        }
        else
        {
            *pVal = dwTID ;
            hr = S_OK ;
        }
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Id ( LONG * pVal )
{
    HRESULT hr = S_OK ;
    ASSERT ( NULL != pVal ) ;
    if ( NULL == pVal )
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    else
    {
        *pVal = GetWindowLong ( m_hWnd , GWL_ID ) ;
        hr = S_OK ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: get_Visible ( VARIANT_BOOL * pVal )
{
    if ( TRUE == ::IsWindowVisible ( m_hWnd ) )
    {
        *pVal = VARIANT_TRUE ;
    }
    else
    {
        *pVal = VARIANT_FALSE ;
    }
    return ( S_OK ) ;
}

STDMETHODIMP TWindow :: SetWindowSize ( LONG X      ,
                                        LONG Y      ,
                                        LONG Width  ,
                                        LONG Height  )
{
    HRESULT hr = S_OK ;
    BOOL bRet = MoveWindow ( m_hWnd , X , Y , Width , Height , TRUE ) ;
    if ( FALSE == bRet )
    {
        hr = Error ( IDS_SIZEFAILED ) ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: SetForegroundTWindow ( void )
{
    HRESULT hr = S_OK ;
    
    // First check that the HWND is real.
    if ( TRUE == ::IsWindow ( m_hWnd ) )
    {
        // At least I've got a good window.
            
        DWORD dwWinPID = 0 ;
        DWORD dwWinTID = GetWindowThreadProcessId ( m_hWnd , &dwWinPID);
        if ( 0 != dwWinTID )
        {
            // Attach this thread to the message queue of the top
            // level window I'll be moving to the front.
            BOOL bRet = AttachThreadInput ( GetCurrentThreadId ( ) ,
                                            dwWinTID               ,
                                            TRUE                    ) ;
            ASSERT ( bRet == TRUE ) ;
            if ( TRUE == bRet )
            {
                // Force the window to the fore.
                bRet = SetForegroundWindow ( m_hWnd ) ;
                ASSERT ( bRet == TRUE ) ;
                
                BOOL bRet2 = AttachThreadInput ( GetCurrentThreadId () ,
                                                 dwWinTID              ,
                                                 FALSE                );
                                            
                if ( ( TRUE == bRet2 ) && ( TRUE == bRet ) )
                {
                    // It's cooked!
                    hr = S_OK ;
                }
                else
                {
                    hr = Error ( IDS_UNEXPECTEDERROR ) ;
                }
            }
            else
            {
                hr = Error ( IDS_ATTACHTHREADFAILED ) ;
            }
            
        }
        else
        {
            hr = Error ( IDS_UNEXPECTEDERROR ) ;
        }
    }
    else
    {
        hr = E_FAIL ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: SetFocusTWindow ( void )
{
    HRESULT hr = S_OK ;
    
    // First check that the HWND is real.
    if ( TRUE == ::IsWindow ( m_hWnd ) )
    {
        // At least I've got a good window.
            
        DWORD dwWinPID = 0 ;
        DWORD dwWinTID = GetWindowThreadProcessId ( m_hWnd , &dwWinPID);
        if ( 0 != dwWinTID )
        {
            BOOL bRet = AllowSetForegroundWindow ( dwWinPID ) ;
            if ( TRUE == bRet )
            {
                // Attach this thread to the message queue of the top
                // level window I'll be moving to the front.
                bRet = AttachThreadInput ( GetCurrentThreadId ( ) ,
                                           dwWinTID               ,
                                           TRUE                    ) ;
                if ( TRUE == bRet )
                {
                    // Force the window to the fore.
                    HWND hFocus = SetFocus ( m_hWnd ) ;
                    
                    bRet = AttachThreadInput ( GetCurrentThreadId ( ) ,
                                               dwWinTID               ,
                                               TRUE                   );
                    if ( ( TRUE == bRet ) && ( NULL != hFocus ) )
                    {
                        // It's cooked!
                        hr = S_OK ;
                    }
                    else
                    {
                        hr = Error ( IDS_ATTACHTHREADFAILED ) ;
                    }
                }
                else
                {
                    hr = Error ( IDS_ATTACHTHREADFAILED ) ;
                }
            }
            else
            {
                hr = Error ( IDS_UNEXPECTEDERROR ) ;
            }
        }
        else
        {
            hr = Error ( IDS_UNEXPECTEDERROR ) ;
        }
    }
    else
    {
        hr = E_FAIL ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: FindChildTWindow ( BSTR        szClass ,
                                           BSTR        szTitle ,
                                           ITWindow ** pVal     )
{
    HRESULT hr = S_OK ;
    // Do I have a good HWND?
    if ( TRUE == ::IsWindow ( m_hWnd ) )
    {
        CComBSTR cClass ( szClass ) ;
        CComBSTR cTitle ( szTitle ) ;
        HWND hChild = FindWindowEx ( m_hWnd , NULL , cClass , cTitle ) ;
        if ( NULL != hChild )
        {
            CComObject<TWindow> * p = new CComObject<TWindow> ( ) ;
            hr = p->QueryInterface ( __uuidof (ITWindow ) ,
                                     (LPVOID*)pVal         ) ;
            if ( SUCCEEDED ( hr ) )
            {
                hr = (*pVal)->put_hWnd ( (LONG_PTR)hChild ) ;
            }
            else
            {
                delete p ;
                hr = Error ( IDS_UNEXPECTEDERROR ) ;
            }
        }
        else
        {
            *pVal = NULL ;
            hr = E_FAIL ;
        }
    }
    else
    {
        *pVal = NULL ;
        hr = E_FAIL  ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: FindChildWindow ( BSTR        szClass ,
                                          BSTR        szTitle ,
                                          ITWindow ** pVal     )
{
    return ( FindChildTWindow ( szClass , szTitle , pVal ) ) ;
}

STDMETHODIMP TWindow :: FindChildTWindowByClass ( BSTR        szClass ,
                                                  ITWindow ** pVal     )
{
    return ( FindChildTWindow ( szClass , NULL , pVal ) ) ;
}

STDMETHODIMP TWindow :: FindChildWindowByClass ( BSTR        szClass ,
                                                 ITWindow ** pVal     )
{
    return ( FindChildTWindow ( szClass , NULL , pVal ) ) ;
}

STDMETHODIMP TWindow :: FindChildTWindowByTitle ( BSTR        szTitle ,
                                                  ITWindow ** pVal     )
{
    return ( FindChildTWindow ( NULL , szTitle , pVal ) ) ;
}

STDMETHODIMP TWindow :: FindChildWindowByTitle ( BSTR        szTitle ,
                                                 ITWindow ** pVal     )
{
    return ( FindChildTWindow ( NULL , szTitle , pVal ) ) ;
}

// The structure passed to the ChildWindowEnumProc.
typedef struct tag_FIND_ID
{
    // The ID to look for.
    LONG    lId ;
    // The HWND with the matching id.
    HWND    hMatchWnd ;
} FIND_ID , * LPFIND_ID ;

// The child window enumerator function.
BOOL CALLBACK ChildWindowIDEnumProc ( HWND hWnd , LPARAM lParam )
{
    LPFIND_ID pFind = (LPFIND_ID)lParam ;
    BOOL bRet = TRUE ;
    
    LONG lCurr = GetWindowLong ( hWnd , GWL_ID ) ;
    if ( lCurr == pFind->lId )
    {
        pFind->hMatchWnd = hWnd ;
        bRet = FALSE ;
    }
    return ( bRet ) ;
}

STDMETHODIMP TWindow :: FindChildTWindowWithID ( LONG        lID  ,
                                                 ITWindow ** pVal  )
{
    HRESULT hr = S_OK ;
    
    // No sense doing anything if the wrapped window is bad.
    if ( TRUE == ::IsWindow ( m_hWnd ) )
    {
        // Go forth and find.
        FIND_ID stFind ;
        stFind.hMatchWnd = NULL ;
        stFind.lId       = lID ;

        EnumChildWindows ( m_hWnd                 ,
                           &ChildWindowIDEnumProc ,
                           (LPARAM)&stFind         ) ;
                           
        if ( NULL != stFind.hMatchWnd )
        {
            CComObject<TWindow> * p = new CComObject<TWindow> ( ) ;
            hr = p->QueryInterface ( __uuidof (ITWindow ) ,
                                    (LPVOID*)pVal          ) ;
            if ( SUCCEEDED ( hr ) )
            {
                hr = (*pVal)->put_hWnd ( (LONG_PTR)stFind.hMatchWnd ) ;
            }
            else
            {
                delete p ;
                hr = Error ( IDS_UNEXPECTEDERROR ) ;
            }
        }
        else
        {
            hr = E_FAIL ;
            *pVal = NULL ;
        }
    }
    else
    {
        *pVal = NULL ;
        hr = E_FAIL ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindow :: FindChildWindowWithID ( LONG        lID  ,
                                                ITWindow ** pVal  )
{
    return ( FindChildTWindowWithID ( lID , pVal ) ) ;
}

STDMETHODIMP TWindow :: EnumChildren ( ITWindows ** pVal )
{
    return ( get_Children ( pVal ) ) ;
}

BOOL CALLBACK EnumChildWindowProc ( HWND hWnd , LPARAM lParam )
{
    HWNDARRAY * pArray = (HWNDARRAY *)lParam ;
    pArray->Add ( hWnd ) ;
    return ( TRUE ) ;
}

STDMETHODIMP TWindow :: get_Children ( ITWindows ** pVal )
{
    HRESULT hr = S_OK ;
    
    // No sense doing anything if this window is bad.
    if ( TRUE == ::IsWindow ( m_hWnd ) )
    {
        HWNDARRAY aHwnds ;
        BOOL bRet = EnumChildWindows ( m_hWnd               ,
                                       &EnumChildWindowProc ,
                                       (LPARAM)&aHwnds       ) ;
        if ( TRUE == bRet )
        {
            hr = BuildITWindowsFromHwndArray ( aHwnds , pVal ) ;
            if ( FAILED ( hr ) )
            {
                hr = Error ( IDS_UNEXPECTEDERROR ) ;
            }
        }
        else
        {
            hr = Error ( IDS_UNEXPECTEDERROR ) ;
        }
    }
    else
    {
        hr = E_FAIL ;
    }
    return ( hr ) ;
}

