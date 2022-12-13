/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "TInput.h"
#include "PlayInput.h"

// TInput


STDMETHODIMP TInput :: get_MultiMonitor ( VARIANT_BOOL * pVal )
{
    if ( NULL == pVal )
    {
        return ( E_INVALIDARG ) ;
    }
    
    if ( TRUE == GetMultiMonitor ( ) )
    {
        *pVal = VARIANT_TRUE ;
    }
    else
    {
        *pVal = VARIANT_FALSE ;
    }
    return ( S_OK ) ;
}

STDMETHODIMP TInput :: put_MultiMonitor ( VARIANT_BOOL newVal )
{
    if ( VARIANT_TRUE == newVal )
    {
        SetMultiMonitor ( TRUE ) ;
    }
    else
    {
        SetMultiMonitor ( FALSE ) ;
    }
    return ( S_OK ) ;
}

STDMETHODIMP TInput ::
            get_AbsoluteScreenCoordinates ( VARIANT_BOOL * pVal )
{
    if ( NULL == pVal )
    {
        return ( E_POINTER ) ;
    }
    *pVal = VARIANT_TRUE ;
    return ( S_OK ) ;
}

STDMETHODIMP TInput ::
            put_AbsoluteScreenCoordinates ( VARIANT_BOOL newVal )
{
    if ( VARIANT_FALSE == newVal )
    {
        MessageBox ( GetForegroundWindow ( )                           ,
                     _T ( "Virtual screen coordinates not supported.") ,
                     _T ( "Tester" )                                   ,
                     MB_OK                                            );
    }
    return ( S_OK ) ;
}

STDMETHODIMP TInput ::
            get_VirtualScreenCoordinates ( VARIANT_BOOL * pVal )
{
    if ( NULL == pVal )
    {
        return ( E_POINTER ) ;
    }
    *pVal = VARIANT_TRUE ;
    return ( S_OK ) ;
}

STDMETHODIMP TInput ::
            put_VirtualScreenCoordinates ( VARIANT_BOOL newVal )
{
    if ( VARIANT_TRUE == newVal )
    {
        MessageBox ( GetForegroundWindow ( )                           ,
                     _T ( "Virtual screen coordinates not supported.") ,
                     _T ( "Tester" )                                   ,
                     MB_OK                                            );
    }
    return ( S_OK ) ;
}

STDMETHODIMP TInput :: PlayInput ( BSTR Str )
{
    HRESULT hr = S_OK ;
    CComBSTR cStr ( Str ) ;
    UINT uiErrPos = 0 ;
    
    UINT uiRet = ::PlayInput ( cStr , &uiErrPos ) ;
    if ( uiRet != PI_SUCCESS )
    {
        UINT uiResID = 0 ;
        switch ( uiRet )
        {
            case PI_INVALIDPARAMS :
                uiResID = IDS_PIINVALIDPARAMS ;
        	    break ;
            case PI_PARSEERROR :
                uiResID = IDS_PIPARSEERROR ;
        	    break ;
            case PI_ERROR :
                uiResID = IDS_PIUNKERROR ;
                break ;
            case PI_CRASH :
                uiResID = IDS_PICRASH ;
        	    break ;
            default:
                ASSERT ( !"invalid return value from PlayInput!!" ) ;
                break ;
        }
        hr = Error ( uiResID , E_FAIL ) ;
    }
    return ( hr ) ;
}

STDMETHODIMP TInput :: PlayKeys ( BSTR Str )
{
    return ( PlayInput ( Str ) ) ;
}
