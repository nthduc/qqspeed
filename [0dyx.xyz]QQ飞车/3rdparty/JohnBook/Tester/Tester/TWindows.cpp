/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "TWindow.h"
#include "TWindows.h"

STDMETHODIMP TWindows :: get_Count ( LONG * pVal )
{
    HRESULT hr = S_OK ;
    if ( NULL != pVal )
    {
        *pVal = m_aTWinArray.GetSize ( ) ;
    }
    else
    {
        hr = E_INVALIDARG ;
    }
    return ( hr ) ;
}


STDMETHODIMP TWindows :: get_Item ( LONG        Index  ,
                                    ITWindow ** RetVal  )
{
    HRESULT hr = S_OK ;
    if ( NULL != RetVal )
    {
        *RetVal = NULL ;
        // Keep to the 1-based indexes like VB.
        if ( (Index < 1) || (Index > m_aTWinArray.GetSize ( ) ) )
        {
            hr = E_INVALIDARG ;
        }
        else
        {
            hr = m_aTWinArray[ Index - 1 ].CopyTo ( RetVal ) ;
        }
    }
    else
    {
        hr = E_INVALIDARG ;
    }
    return ( hr ) ;
}

typedef CComObject < CComEnum < IEnumVARIANT        ,
                                &IID_IEnumVARIANT   ,
                                VARIANT             ,
                                _Copy<VARIANT> > >   ENUMVAR ;
                                
STDMETHODIMP TWindows :: get__NewEnum ( IUnknown ** retval )
{
    HRESULT hr = S_OK ;
    
    if ( NULL == retval )
    {
        return ( E_POINTER ) ;
    }
    // Always initialize to a known value.
    *retval = NULL ;
    
    // Allocate the variant array.
    int iArraySize = m_aTWinArray.GetSize ( ) ;
    VARIANT * pVarArray = new VARIANT [ iArraySize + 1 ] ;
    
    // Copy over each value into the variant.
    for ( int iCurr = 0 ; iCurr < iArraySize ; iCurr++ )
    {
        pVarArray[ iCurr ].vt = VT_DISPATCH ;
        pVarArray[ iCurr ].pdispVal = m_aTWinArray[ iCurr ].p ;
    }
    if ( SUCCEEDED ( hr ) )
    {
        // Initialize the last variant to empty.
        pVarArray[ iArraySize ].vt = VT_EMPTY ;
        
        // Create the IEnumVARIANT interface
        ENUMVAR * p = new ENUMVAR ;
        hr = p->Init ( &pVarArray[ 0 ]          ,
                       &pVarArray[ iArraySize ] ,
                       NULL                     ,
                       AtlFlagCopy               ) ;
        if ( SUCCEEDED( hr ) )
        {
            hr = p->QueryInterface ( IID_IEnumVARIANT ,
                                     (LPVOID*)retval   ) ;
        }
    }
    
    // Delete the variant array.
    delete [] pVarArray ;
    
    return ( hr ) ;
}

STDMETHODIMP TWindows :: Add ( VARIANT *   item   ,
                               ITWindow ** RetVal  )
{
    HRESULT hr = S_OK ;
    if ( NULL != item )
    {
        // I need to look in the variant to see what type I'm being
        // passed.  If it's VT_DISPATCH, it's an object.  Anything else
        // I treat as an HWND.
        if ( VT_DISPATCH != item->vt )
        {
            // It's an HWND so make sure it's a real one.
            HWND hWndItem = (HWND)item->iVal ;

            CComObject<TWindow> * p = new CComObject<TWindow> ( ) ;
            hr = p->QueryInterface ( __uuidof (ITWindow ) ,
                                     (LPVOID*)RetVal       ) ;
            if ( SUCCEEDED ( hr ) )
            {
                hr = (*RetVal)->put_hWnd ( (LONG_PTR)hWndItem ) ;
                if ( SUCCEEDED ( hr ) )
                {
                    // Finally add this fella to the array.
                    m_aTWinArray.Add ( p ) ;
                }
                else
                {
                    delete p ;
                    hr = Error ( IDS_UNEXPECTEDERROR ) ;
                }
            }
            else
            {
                delete p ;
                hr = Error ( IDS_UNEXPECTEDERROR ) ;
            }
        }
        else
        {
            // This is a dispatch pointer.  First check to see if I can
            // get the TWindow off this puppy.  If so, I can add it.
            IDispatch * pDisp = item->pdispVal ;
            hr = pDisp->QueryInterface ( __uuidof ( ITWindow ) ,
                                         (LPVOID*)RetVal        ) ;
            if ( SUCCEEDED ( hr ) )
            {
                // It's a good type so plop it in.
                CComPtr<ITWindow> p ;
                p = *RetVal ;
                m_aTWinArray.Add ( p ) ;
                
            }
            else
            {
                hr = Error ( IDS_DISPATCHNOTVALID , E_INVALIDARG ) ;
            }
        }
    }
    else
    {
        hr = E_INVALIDARG ;
    }
    return ( hr ) ;
}

STDMETHODIMP TWindows :: Remove ( LONG        Index  ,
                                  ITWindow ** RetVal  )
{
    HRESULT hr = S_OK ;
    
    Index = Index - 1 ;
    if ( (Index < 0) || (Index > ( m_aTWinArray.GetSize ( ) - 1 ) ) )
    {
        hr = E_INVALIDARG ;
    }
    else
    {
        *RetVal = NULL ;

        // Copy off the value.
        hr = m_aTWinArray[ Index ].CopyTo ( RetVal ) ;
        if ( SUCCEEDED ( hr ) )
        {
            if ( FALSE == m_aTWinArray.RemoveAt ( Index ) )
            {
                hr = E_FAIL ;
            }
            else
            {
                hr = S_OK ;
            }
        }
    }
    return ( hr ) ;
}
