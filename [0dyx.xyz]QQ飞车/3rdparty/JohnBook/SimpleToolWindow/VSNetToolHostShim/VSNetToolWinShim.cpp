// VSNetToolWinShim.cpp : Implementation of CVSNetToolWinShim
#include "stdafx.h"
#include "VSNetToolWinShim.h"

// CVSNetToolWinShim
// 4278 '': identifier in type library '' is already a macro; use the 'rename' qualifier
#pragma warning ( disable : 4278 )
#import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" version("7.0") lcid("0") raw_interfaces_only named_guids
#pragma warning ( default : 4278 )

HPALETTE CreateDIBPalette (LPBITMAPINFO lpbmi, LPINT lpiNumColors) ;
HBITMAP LoadResourceBitmap(HINSTANCE hInstance, TCHAR *pszString, HPALETTE FAR* lphPalette)  ;

// CVSNetToolWinShim
LRESULT CVSNetToolWinShim :: OnSize ( UINT    /*uMsg*/    ,
                                      WPARAM  /*wParam*/  ,
                                      LPARAM  lParam      ,
                                      BOOL&   /*bHandled*/)
{
	WORD wLength = LOWORD ( lParam ) ;
	WORD wHeight = HIWORD ( lParam ) ;
	::MoveWindow ( m_hWndForm , 0 , 0 , wLength , wHeight , TRUE ) ;
	return ( 0 ) ;
}

void CVSNetToolWinShim :: Reset ( )
{
	m_pHost = NULL ;
	m_pDefaultDomain = NULL ;
	m_pObjHandle = NULL ;
	m_varUnwrappedObject.Clear ( ) ;
	m_hWndForm = 0 ;
}

HRESULT CVSNetToolWinShim::
            HostUserControl2 ( IUnknown * pToolWindow  ,
                               BSTR       Assembly     ,
                               BSTR       Class        ,
                               BSTR       SatelliteDLL ,
                               int        ResourceID   ,
                               IUnknown** ppControlObject )
{
	CComQIPtr<EnvDTE::Window> pWindow ( pToolWindow ) ;

	RECT rc ;
	CComPtr<IWin32Window> pIWin32Window ;
	
	// Try to create the assembly out of the GAC.
	HRESULT hr = m_pDefaultDomain->CreateInstance ( Assembly        ,
	                                                Class           ,
	                                                &m_pObjHandle    ) ;
	if ( FAILED ( hr ) || ( !m_pObjHandle ) )
	{
	    // Assume the complete path is passed in and try and create that
	    // one.
		hr = m_pDefaultDomain->CreateInstanceFrom ( Assembly      ,
		                                            Class         ,
		                                            &m_pObjHandle  ) ;
		if ( FAILED ( hr ) || ( !m_pObjHandle ) )
		{
		    // At least let the caller know something is bad in a
		    // debug build.
		    ATLASSERT ( FALSE ) ;
			hr = E_FAIL ;
			Reset ( ) ;
			return ( hr ) ;
		}
	}
	
	hr = m_pObjHandle->Unwrap ( &m_varUnwrappedObject ) ;
	if ( ( m_varUnwrappedObject.vt != VT_DISPATCH ) &&
	       ( m_varUnwrappedObject.vt != VT_UNKNOWN ) ||
	        ( !m_varUnwrappedObject.punkVal  ) )
	{
	    ATLASSERT ( FALSE ) ;
		hr = E_FAIL ;
		Reset ( ) ;
		return ( hr ) ;
	}
	hr = m_varUnwrappedObject.pdispVal->QueryInterface ( IID_IUnknown ,
	                                        (LPVOID*)ppControlObject ) ;
    ATLASSERT ( SUCCEEDED ( hr ) ) ;
    	                                        
	hr = m_varUnwrappedObject.pdispVal->
	                        QueryInterface ( IID_IWin32Window        ,
	                                         (LPVOID*)&pIWin32Window ) ;
    ATLASSERT ( SUCCEEDED ( hr ) ) ;
	if ( FAILED ( hr ) )
	{
		Reset ( ) ;
		return ( hr ) ;
	}
	
	long hWndTemp ;
	hr = pIWin32Window->get_Handle ( &hWndTemp ) ;
// 'type cast' : conversion from 'long' to 'HWND' of greater size
#pragma warning ( disable : 4312 )
	m_hWndForm = (HWND)hWndTemp ;
#pragma warning ( default : 4312 )
	if ( FAILED ( hr ) || ( !m_hWndForm ) )
	{
	    ATLASSERT ( FALSE ) ;
		Reset ( ) ;
		return ( hr ) ;
	}
	if ( pWindow.p )
	{
		USES_CONVERSION ;
		CComVariant varPic ;
		CComPtr<IPictureDisp> pPictureDisp ;
		PICTDESC pd ;
		pd.cbSizeofstruct = sizeof ( PICTDESC ) ;
		pd.picType = PICTYPE_BITMAP ;
		HMODULE hModule = LoadLibrary ( W2T ( SatelliteDLL ) ) ;
		pd.bmp.hbitmap = LoadResourceBitmap ( hModule ,
		                             MAKEINTRESOURCE ( ResourceID ) ,
		                                      &pd.bmp.hpal ) ;
		OleCreatePictureIndirect ( &pd              ,
		                           IID_IPictureDisp ,
		                           FALSE            ,
		                           (LPVOID*)&pPictureDisp ) ;
		pPictureDisp->QueryInterface ( IID_IUnknown ,
		                              (LPVOID*)&varPic.punkVal ) ;
		varPic.vt = VT_UNKNOWN ;
		pWindow->SetTabPicture ( varPic ) ;
		FreeLibrary ( hModule ) ;
	}

	::SetParent((HWND)m_hWndForm, m_hWnd);
	::GetWindowRect(m_hWnd, &rc);
	::MoveWindow((HWND)m_hWndForm, 0, 0, rc.right-rc.left, rc.bottom-rc.top, TRUE);
	::ShowWindow((HWND)m_hWndForm, SW_SHOW);
	return ( hr ) ;
}

HRESULT CVSNetToolWinShim ::
            HostUserControl ( BSTR Assembly ,
                              BSTR Class    ,
                              IDispatch **ppControlObject )
{
	RECT rc;
	CComPtr<IWin32Window> pIWin32Window;
	HRESULT hr = m_pDefaultDomain->CreateInstance(Assembly, Class, &m_pObjHandle);
	if(FAILED(hr) || (!m_pObjHandle))
	{
		hr = m_pDefaultDomain->CreateInstanceFrom(Assembly, Class, &m_pObjHandle);
		if(FAILED(hr) || (!m_pObjHandle))
		{
		    ATLASSERT ( FALSE ) ;
			hr = E_FAIL;
			Reset();
			return hr;
		}
	}
	hr = m_pObjHandle->Unwrap(&m_varUnwrappedObject);
	if((m_varUnwrappedObject.vt != VT_DISPATCH) && (m_varUnwrappedObject.vt != VT_UNKNOWN) || (!m_varUnwrappedObject.punkVal))
	{
	    ATLASSERT ( FALSE ) ;
		hr = E_FAIL;
		Reset();
		return hr;
	}
	hr = m_varUnwrappedObject.pdispVal->QueryInterface(IID_IDispatch, (LPVOID*)ppControlObject);
	hr = m_varUnwrappedObject.pdispVal->QueryInterface(IID_IWin32Window, (LPVOID*)&pIWin32Window);
	if(FAILED(hr))
	{
	    ATLASSERT ( FALSE ) ;
		Reset();
		return hr;
	}
	long hWndTemp ;
	hr = pIWin32Window->get_Handle ( &hWndTemp ) ;
// 'type cast' : conversion from 'long' to 'HWND' of greater size
#pragma warning ( disable : 4312 )
	m_hWndForm = (HWND)hWndTemp ;
#pragma warning ( default : 4312 )
	if(FAILED(hr) || (!m_hWndForm))
	{
	    ATLASSERT ( FALSE ) ;
		Reset();
		return hr;
	}
	::SetParent((HWND)m_hWndForm, m_hWnd);
	::GetWindowRect(m_hWnd, &rc);
	::MoveWindow((HWND)m_hWndForm, 0, 0, rc.right-rc.left, rc.bottom-rc.top, TRUE);
	::ShowWindow((HWND)m_hWndForm, SW_SHOW);
	return hr;
}

HRESULT CVSNetToolWinShim::FinalConstruct()
{
	CComPtr<IUnknown> pAppDomainPunk;
	HRESULT hr = CorBindToRuntimeEx(NULL, NULL, STARTUP_LOADER_OPTIMIZATION_SINGLE_DOMAIN | STARTUP_CONCURRENT_GC, __uuidof(CorRuntimeHost), __uuidof(ICorRuntimeHost), (LPVOID*)&m_pHost);
	if(FAILED(hr))
	{
	    ATLASSERT ( FALSE ) ;
		return hr;
	}
	hr = m_pHost->Start();
	if(FAILED(hr))
	{
	    ATLASSERT ( FALSE ) ;
		return hr;
	}
	hr = m_pHost->GetDefaultDomain(&pAppDomainPunk);
	if(FAILED(hr) || !pAppDomainPunk)
	{
	    ATLASSERT ( FALSE ) ;
		return hr;
	}
	hr = pAppDomainPunk->QueryInterface(__uuidof(mscorlib::_AppDomain), (LPVOID*)&m_pDefaultDomain);
	if(FAILED(hr) || !m_pDefaultDomain)
	{
	    ATLASSERT ( FALSE ) ;
		return hr;
	}

	return hr;
}

HPALETTE CreateDIBPalette (LPBITMAPINFO lpbmi, LPINT lpiNumColors)
{
	LPBITMAPINFOHEADER  lpbi;
	LPLOGPALETTE     lpPal;
	HANDLE           hLogPal;
	HPALETTE         hPal = NULL;
	int              i;

	lpbi = (LPBITMAPINFOHEADER)lpbmi;
	if (lpbi->biBitCount <= 8)
		*lpiNumColors = (1 << lpbi->biBitCount);
	else
		*lpiNumColors = 0;  // No palette needed for 24 BPP DIB

	if (lpbi->biClrUsed > 0)
		*lpiNumColors = lpbi->biClrUsed;  // Use biClrUsed

	if (*lpiNumColors)
	{
		hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) + sizeof (PALETTEENTRY) * (*lpiNumColors));
		lpPal = (LPLOGPALETTE) GlobalLock (hLogPal);
		lpPal->palVersion    = 0x300;
		lpPal->palNumEntries = (WORD)*lpiNumColors;

		for (i = 0;  i < *lpiNumColors;  i++)
		{
			lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}
		hPal = CreatePalette (lpPal);
		GlobalUnlock (hLogPal);
		GlobalFree   (hLogPal);
	}
	return hPal;
}

HBITMAP LoadResourceBitmap(HINSTANCE hInstance, TCHAR *pszString, HPALETTE FAR* lphPalette)
{
	HGLOBAL hGlobal;
	HBITMAP hBitmapFinal = NULL;
	LPBITMAPINFOHEADER  lpbi;
	HDC hdc;
	int iNumColors;
	HRSRC hRsrc = FindResource(hInstance, pszString, RT_BITMAP);
	if (hRsrc)
	{
		hGlobal = LoadResource(hInstance, hRsrc);
		lpbi = (LPBITMAPINFOHEADER)LockResource(hGlobal);

		hdc = GetDC(NULL);
		*lphPalette =  CreateDIBPalette ((LPBITMAPINFO)lpbi, &iNumColors);
		if (*lphPalette)
		{
			SelectPalette(hdc,*lphPalette,FALSE);
			RealizePalette(hdc);
		}

		hBitmapFinal = CreateDIBitmap(hdc, (LPBITMAPINFOHEADER)lpbi, (LONG)CBM_INIT, (LPSTR)lpbi + lpbi->biSize + iNumColors * sizeof(RGBQUAD), (LPBITMAPINFO)lpbi, DIB_RGB_COLORS );

		ReleaseDC(NULL,hdc);
		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}
	return (hBitmapFinal);
}
