/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "Helpers.h"

HRESULT BuildITWindowsFromHwndArray ( HWNDARRAY &  aHwnds ,
                                      ITWindows ** pVal    )
{
    HRESULT hr = S_OK ;
    
    // Create the ITWindows I'll end up passing back.
    CComObject<TWindows> * p = new CComObject<TWindows> ( ) ;
    hr = p->QueryInterface ( __uuidof ( ITWindows ) ,
                             (LPVOID*)pVal            ) ;
    if ( SUCCEEDED ( hr ) )
    {
        // Loop through all the HWNDS I found.
        int iCount = aHwnds.GetSize ( ) ;
        for ( int iCurr = 0 ; iCurr < iCount ; iCurr++ )
        {
            // Step one is to create the ITWindow interface and
            // set it's HWND.
            CComPtr<ITWindow> cWin ;
            hr = cWin.CoCreateInstance ( __uuidof ( TWindow ) );
            if ( SUCCEEDED ( hr ) )
            {
                hr = cWin->put_hWnd ( (UINT_PTR)aHwnds[ iCurr ] ) ;
                if ( SUCCEEDED ( hr ) )
                {
                    // Step two is to plunk this thing in the
                    // TWindows array.

                    VARIANT v ;
                    v.vt       = VT_DISPATCH ;
                    v.pdispVal = cWin.p ;

                    ITWindow * pTempWin = NULL ;

                    hr = (*pVal)->Add ( &v , &pTempWin ) ;

                    pTempWin->Release ( ) ;
                }
                else
                {
                    hr = E_UNEXPECTED ;
                }
            }
            else
            {
                hr = E_UNEXPECTED ;
            }
        }
    }
    else
    {
        hr = E_UNEXPECTED ;
    }
    return ( hr ) ;
}
