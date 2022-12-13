/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "TSystem.h"

STDMETHODIMP TSystem :: GetForegroundTWindow ( ITWindow ** pVal )
{
    return ( get_ForegroundTWindow ( pVal ) ) ;
}

STDMETHODIMP TSystem :: get_ForegroundTWindow ( ITWindow ** pVal )
{
    HRESULT hr = S_OK ;
    
    HWND hWndFore = ::GetForegroundWindow ( ) ;
    if ( ( NULL != hWndFore ) && ( TRUE == ::IsWindow ( hWndFore ) ) )
    {
        CComObject<TWindow> * p = new CComObject<TWindow> ( ) ;
        hr = p->QueryInterface ( __uuidof (ITWindow ) ,
                                 (LPVOID*)pVal         ) ;
        if ( SUCCEEDED ( hr ) )
        {
            hr = (*pVal)->put_hWnd ( (LONG_PTR)hWndFore ) ;
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
    }
    return ( hr ) ;
}

STDMETHODIMP TSystem :: EnumTopWindows ( ITWindows ** pVal )
{
    return ( get_TopTWindows ( pVal ) ) ;
}

BOOL CALLBACK EnumTopWindowProc ( HWND hWnd , LPARAM lParam )
{
    HWNDARRAY * pArray = (HWNDARRAY *)lParam ;
    pArray->Add ( hWnd ) ;
    return ( TRUE ) ;
}

STDMETHODIMP TSystem :: get_TopTWindows ( ITWindows ** pVal )
{
    HRESULT hr = S_OK ;
    HWNDARRAY aHwnds ;
    BOOL bRet = EnumWindows ( &EnumTopWindowProc ,
                              (LPARAM)&aHwnds     ) ;
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
    return ( hr ) ;
}

STDMETHODIMP TSystem :: FindTopWindow ( BSTR        szClass ,
                                        BSTR        szTitle ,
                                        ITWindow ** pVal     )
{
    return ( FindTopTWindow ( szClass , szTitle , pVal ) ) ;
}

STDMETHODIMP TSystem :: FindTopTWindow ( BSTR        szClass ,
                                         BSTR        szTitle ,
                                         ITWindow ** pVal     )
{
    HRESULT hr = S_OK ;
    
    CComBSTR cClass ( szClass ) ;
    CComBSTR cTitle ( szTitle ) ;
    HWND hWnd = FindWindow ( cClass , cTitle ) ;
    if ( NULL != hWnd )
    {
        CComObject<TWindow> * p = new CComObject<TWindow> ( ) ;
        hr = p->QueryInterface ( __uuidof (ITWindow ) ,
            (LPVOID*)pVal         ) ;
        if ( SUCCEEDED ( hr ) )
        {
            hr = (*pVal)->put_hWnd ( (LONG_PTR)hWnd ) ;
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
    return ( S_OK ) ;
}
STDMETHODIMP TSystem :: FindTopWindowByTitle ( BSTR        szTitle ,
                                               ITWindow ** pVal     )
{
    return ( FindTopTWindowByTitle ( szTitle , pVal ) ) ;
}

STDMETHODIMP TSystem :: FindTopTWindowByTitle ( BSTR        szTitle ,
                                                ITWindow ** pVal     )
{
    return ( FindTopTWindow ( NULL , szTitle , pVal ) ) ;
}

STDMETHODIMP TSystem :: FindTopWindowByClass ( BSTR        szClass ,
                                               ITWindow ** pVal     )
{
    return ( FindTopTWindowByClass ( szClass , pVal ) ) ;
}

STDMETHODIMP TSystem :: FindTopTWindowByClass ( BSTR        szClass ,
                                                ITWindow ** pVal     )
{
    return ( FindTopTWindow ( szClass , NULL , pVal ) ) ;
}

STDMETHODIMP TSystem :: Execute ( BSTR szProgram , VARIANT_BOOL* pVal )
{
    HRESULT hr = S_OK ;
    if ( NULL != pVal )
    {
        CComBSTR cProgram ( szProgram ) ;
        
        PROCESS_INFORMATION pi ;
        STARTUPINFO si ;

        ZeroMemory ( &si , sizeof ( STARTUPINFO ) ) ;
        ZeroMemory ( &pi , sizeof ( PROCESS_INFORMATION ) ) ;
        
        BOOL bRet = CreateProcess ( NULL     ,
                                    cProgram ,
                                    NULL     ,
                                    NULL     ,
                                    FALSE    ,
                                    0        ,
                                    NULL     ,
                                    NULL     ,
                                    &si      ,
                                    &pi       ) ;
        if ( TRUE == bRet )
        {
            *pVal = VARIANT_TRUE ;
            // Gotta close those handles!
            VERIFY ( CloseHandle ( pi.hProcess ) ) ;
            VERIFY ( CloseHandle ( pi.hThread ) ) ;
            hr = S_OK ;
        }
        else
        {
            *pVal = VARIANT_FALSE ;
            hr = E_FAIL ;
        }
    }
    else
    {
        hr = Error ( IDS_INVALIDARGUMENT , E_INVALIDARG ) ;
    }
    return ( hr ) ;
}

STDMETHODIMP TSystem :: Sleep ( DOUBLE Seconds )
{
    LONG lMillis = (LONG)Seconds * 1000 ;
    return ( Pause ( lMillis ) ) ;
}

STDMETHODIMP TSystem :: Pause ( LONG Milliseconds )
{
    ::Sleep ( Milliseconds ) ;
    return ( S_OK ) ;
}

STDMETHODIMP TSystem :: CheckResolution ( LONG           Width  ,
                                          LONG           Height ,
                                          VARIANT_BOOL * pVal    )
{
    HRESULT hr = S_OK ;
    
    LONG WidthRes = GetSystemMetrics ( SM_CXSCREEN ) ;
    LONG HeightRes = GetSystemMetrics ( SM_CYSCREEN ) ;
    
    if ( ( HeightRes != Height ) || ( WidthRes != Width ) )
    {
        *pVal = VARIANT_FALSE ;
        hr = Error ( IDS_SCREENMISMATCH , E_FAIL ) ;
    }
    else
    {
        *pVal = VARIANT_TRUE ;
        hr = S_OK ;
    }
    return ( hr ) ;
}

STDMETHODIMP TSystem :: CheckVirtualResolution ( LONG           Width  ,
                                                 LONG           Height ,
                                                 LONG           xOrigin,
                                                 LONG           yOrigin,
                                                 VARIANT_BOOL * pVal   )
{
    HRESULT hr = S_OK ;

    LONG WidthRes = GetSystemMetrics ( SM_CXVIRTUALSCREEN ) ;
    LONG HeightRes = GetSystemMetrics ( SM_CYVIRTUALSCREEN ) ;

    if ( ( HeightRes != Height ) || ( WidthRes != Width ) )
    {
        *pVal = VARIANT_FALSE ;
        hr = Error ( IDS_SCREENMISMATCH , E_FAIL ) ;
    }
    else
    {
        LONG xRealOrigin = GetSystemMetrics ( SM_XVIRTUALSCREEN ) ;
        LONG yRealOrigin = GetSystemMetrics ( SM_YVIRTUALSCREEN ) ;
        
        if ( ( xRealOrigin != xOrigin ) || ( yRealOrigin != yOrigin ) )
        {
            *pVal = VARIANT_FALSE ;
            hr = Error ( IDS_SCREENMISMATCH , E_FAIL ) ;
        }
        else
        {
            *pVal = VARIANT_TRUE ;
            hr = S_OK ;
        }
    }
    return ( hr ) ;
}

typedef struct tag_TITLEANDID
{
    HWND    hWndMatch ;
    LONG    Id ;
    TCHAR * szCaption ;
} TITLEANDID , * LPTITLEANDID ;
    
BOOL CALLBACK LookForChildTitleAndIdProc ( HWND hWnd , LPARAM lParam )
{
    LPTITLEANDID pData = (LPTITLEANDID)lParam ;
    
    TCHAR szCurrTitle [ MAX_PATH ] ;
    GetWindowText ( hWnd , szCurrTitle , MAX_PATH ) ;
    LONG lCurrId = GetWindowLong ( hWnd , GWL_ID ) ;
    
    if ( ( lCurrId == pData->Id                            ) &&
         ( 0 == _tcscmp ( pData->szCaption , szCurrTitle ) )    )
    {
        // Found it!
        pData->hWndMatch = hWnd ;
        return ( FALSE ) ;
    }
    return ( TRUE ) ;
}

STDMETHODIMP TSystem :: SetSpecificFocus ( BSTR           ForeTitle   ,
                                           BSTR           ParentTitle ,
                                           BSTR           ChildTitle  ,
                                           LONG           Id          ,
                                           VARIANT_BOOL * pVal         )
{
    HRESULT hr = S_OK ;

    // Step one is to find the window I'll force to the front by looking
    // for the foreground window.
    CComBSTR cForeTitle ( ForeTitle ) ;
    HWND hWnd = FindWindow ( NULL , cForeTitle ) ;
    if ( NULL == hWnd )
    {
        // Not much point in continuing if I can't find the foreground
        // window.
        *pVal = VARIANT_FALSE ;
        hr = Error ( IDS_NOTOPWIN , E_FAIL ) ;
        return ( hr ) ;
    }
    
    // If the parent title is not empty, then I am probably looking at
    // something like the "Save As" dialog.  In that case, there might
    // be multiple open so I need to look through all the top level
    // windows and ensure I find the correct one.
    CComBSTR cParentTitle ( ParentTitle ) ;
    if ( NULL != cParentTitle.m_str )
    {
        TCHAR szCaption[ MAX_PATH ] ;
        
        // Get the parent window for the foreground window as well as
        // it's caption.
        HWND hParent = GetParent ( hWnd ) ;
        GetWindowText ( hParent , szCaption , MAX_PATH ) ;
        
        // Does the parent window title match the one passed in?
        if ( cParentTitle != szCaption )
        {
            // I have to enumerate the windows as I found the wrong
            // main window.
            HWNDARRAY aHwnds ;
            BOOL bRet = EnumWindows ( &EnumTopWindowProc ,
                                      (LPARAM)&aHwnds     ) ;
            if ( TRUE == bRet )
            {
                // Forget the window I already thought I'd found.
                hWnd = NULL ;
                
                int iCurr = 0 ;
                for ( iCurr = 0 ; iCurr < aHwnds.GetSize ( ) ; iCurr++ )
                {
                    GetWindowText ( aHwnds[ iCurr ] ,
                                    szCaption       ,
                                    MAX_PATH         ) ;
                    if ( cParentTitle == szCaption )
                    {
                        // OK, I found the real fore window.
                        hWnd = aHwnds[ iCurr ] ;
                        break ;
                    }
                }
                if ( NULL == hWnd )
                {
                    hr = Error ( IDS_NOFINDPARENT , E_FAIL ) ;
                }
            }
            else
            {
                hr = Error ( IDS_UNEXPECTEDERROR ) ;
            }
        }
    }
    
    if ( SUCCEEDED ( hr ) )
    {
        // Now that I've found the appropriate top level window, I'll
        // scoot through all the child windows to find the one with the
        // specified ID.
        CComBSTR cChildTitle ( ChildTitle ) ;
        
        TITLEANDID stTID ;
        stTID.hWndMatch = NULL ;
        stTID.Id = Id ;
        stTID.szCaption = cChildTitle ;
        
        EnumChildWindows ( hWnd                        ,
                           &LookForChildTitleAndIdProc ,
                           (LPARAM)&stTID               ) ;
        if ( NULL != stTID.hWndMatch )
        {
            HWND hWndKid = stTID.hWndMatch ;
            
            // Attach this thread into the message queue of the top
            // level window I'll be moving to the front.
            DWORD dwTID = GetWindowThreadProcessId ( hWnd , NULL ) ;
            BOOL bRet = AttachThreadInput ( GetCurrentThreadId ( ) ,
                                            dwTID                  ,
                                            TRUE                    ) ;
            ASSERT ( bRet == TRUE ) ;
            if ( TRUE == bRet )
            {
                // I'm attached so move this window to the
                // forground.
                bRet = SetForegroundWindow ( hWnd ) ;
                ASSERT ( bRet == TRUE ) ;
                if ( TRUE == bRet )
                {
                    // Now that the parent has focus, set the
                    // keyboard focus to the child window.
                    HWND hFocus = SetFocus ( hWndKid ) ;
                    if ( NULL != hFocus )
                    {
                        // Everything worked!
                        hr = S_OK ;
                    }
                    else
                    {
                        ASSERT ( !"SetFocus failed!" ) ;
                        hr = Error ( IDS_SETFOCUSFAILED , E_FAIL ) ;
                    }
                }
                else
                {
                    hr = Error( IDS_NOSETFOREGROUNDWINDOW , E_FAIL);
                }
                // Since I've attached the thread input, I need to
                // release it.  This needs to get executed no matter
                // if anything else fails above.
                AttachThreadInput ( GetCurrentThreadId ( ) ,
                                    dwTID                  ,
                                    FALSE                   ) ;
            }
            else
            {
                hr = Error ( IDS_NOATTACHTHREADINPUT , E_FAIL ) ;
            }
        }
        else
        {
            hr = Error ( IDS_NOFINDKID , E_FAIL ) ;
        }
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
