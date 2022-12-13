// SuperSaverOptionsShim.cpp : Implementation of CSuperSaverOptionsShim
#include "stdafx.h"
#include "SuperSaverOptionsShim.h"

// CSuperSaverOptionsShim

LPCTSTR k_HOSTCLSID =
   _T ( "CLSID\\{0B4F08D8-EA89-45dd-AF0A-431BB06DA2CD}\\InprocServer32" ) ;
    
LRESULT CSuperSaverOptionsShim :: OnShowWindow ( UINT   ,
                                                 WPARAM ,
                                                 LPARAM ,
                                                 BOOL&    )
{
	CComPtr<IUnknown> pUnk ;
	Reset ( ) ;
	FinalConstruct ( ) ;
	
	HKEY hKeyInProcServer32 = NULL;
	LONG lRes = RegOpenKeyEx ( HKEY_CLASSES_ROOT    ,
	                           k_HOSTCLSID          ,
	                           0                    ,
	                           KEY_READ             ,
	                           &hKeyInProcServer32   ) ;
    ATLASSERT ( ERROR_SUCCESS == lRes ) ;
                   
	if( ERROR_SUCCESS == lRes )
	{
		TCHAR szPath[ MAX_PATH ] ;
        TCHAR szClass[ MAX_PATH ] ;
		DWORD dwSize = MAX_PATH ;
		
		lRes = RegQueryValueEx ( hKeyInProcServer32  ,
		                         _T("Class")         ,
		                         NULL                ,
		                         NULL                ,
		                         (LPBYTE)szClass     ,
		                         &dwSize              ) ;
        ATLASSERT ( ERROR_SUCCESS == lRes ) ;

        if ( ERROR_SUCCESS == lRes )
        {
            dwSize = MAX_PATH ;
            
            lRes = RegQueryValueEx ( hKeyInProcServer32 ,
                                     _T ( "CodeBase" )  ,
                                     NULL               ,
                                     NULL               ,
                                     (LPBYTE)szPath     ,
                                     &dwSize             ) ;
            ATLASSERT ( ERROR_SUCCESS == lRes ) ;
                                                 
		    if ( ERROR_SUCCESS == lRes )
		    {
			    TCHAR *pszPath = szPath ;
			    while ( _T ( '\0' ) != *pszPath )
			    {
				    if ( isupper ( *pszPath ) )
				    {
					    TCHAR c = _tolower ( *pszPath ) ;
					    *pszPath = c ;
				    }
				    pszPath++ ;
			    }
			    pszPath = szPath ;
			    pszPath += _tcslen ( _T ( "file:///" ) ) ;

			    TCHAR *pszPath2 = pszPath ;
			    while ( _T ( '\0' ) != *pszPath2 )
			    {
				    if ( *pszPath2 == _T ( '/' ) )
				    {
					    *pszPath2 = _T ( '\\' ) ;
					}
				    pszPath2++ ;
			    }

			    HRESULT hr = HostUserControl ( CComBSTR ( pszPath ) ,
			                                   CComBSTR ( szClass ) ,
			                                   &pUnk                 ) ;
                ATLASSERT ( S_OK == hr ) ;
                UNREFERENCED_PARAMETER ( hr ) ;
		    }
        }
		RegCloseKey ( hKeyInProcServer32 ) ;
	}
	return ( 0 ) ;
}

LRESULT CSuperSaverOptionsShim::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	WORD wLength = LOWORD(lParam);
	WORD wHeight = HIWORD(lParam);
	::MoveWindow ( m_hWndForm , 0 , 0 , wLength , wHeight , TRUE ) ;
	return 0;
}

void CSuperSaverOptionsShim::Reset()
{
	m_pHost = NULL;
	m_pDefaultDomain = NULL;
	m_pObjHandle = NULL;
	m_varUnwrappedObject.Clear();
	m_hWndForm = 0;
}

HRESULT CSuperSaverOptionsShim::HostUserControl(BSTR Assembly, BSTR Class, IUnknown **ppControlObject)
{
	RECT rc;
	CComPtr<IWin32Window> pIWin32Window;
	HRESULT hr = m_pDefaultDomain->CreateInstance(Assembly, Class, &m_pObjHandle);
	if(FAILED(hr) || (!m_pObjHandle))
	{
		hr = m_pDefaultDomain->CreateInstanceFrom(Assembly, Class, &m_pObjHandle);
		if(FAILED(hr) || (!m_pObjHandle))
		{
			hr = E_FAIL;
			Reset();
			return hr;
		}
	}
	hr = m_pObjHandle->Unwrap(&m_varUnwrappedObject);
	if((m_varUnwrappedObject.vt != VT_DISPATCH) && (m_varUnwrappedObject.vt != VT_UNKNOWN) || (!m_varUnwrappedObject.punkVal))
	{
		hr = E_FAIL;
		Reset();
		return hr;
	}
	hr = m_varUnwrappedObject.pdispVal->QueryInterface(IID_IUnknown, (LPVOID*)ppControlObject);
	hr = m_varUnwrappedObject.pdispVal->QueryInterface(IID_IWin32Window, (LPVOID*)&pIWin32Window);
	if(FAILED(hr))
	{
		Reset();
		return hr;
	}
	long lHwnd = 0 ;
	hr = pIWin32Window->get_Handle ( &lHwnd ) ;
	// '' : conversion from '' to '' of greater size
#pragma warning ( disable : 4312 )
	m_hWndForm = reinterpret_cast<HWND> ( lHwnd ) ;
#pragma warning ( default : 4312 )
	if(FAILED(hr) || (!m_hWndForm))
	{
		Reset();
		return hr;
	}
	::SetParent ( m_hWndForm , m_hWnd ) ;
	::GetWindowRect ( m_hWnd , &rc ) ;
	::MoveWindow ( m_hWndForm           ,
	               0                    ,
	               0                    ,
	               rc.right - rc.left   ,
	               rc.bottom-rc.top     ,
	               TRUE                  ) ;
	return hr;
}

HRESULT CSuperSaverOptionsShim::FinalConstruct()
{
	CComPtr<IUnknown> pAppDomainPunk;
	HRESULT hr = CorBindToRuntimeEx(NULL, NULL, STARTUP_LOADER_OPTIMIZATION_SINGLE_DOMAIN | STARTUP_CONCURRENT_GC, __uuidof(CorRuntimeHost), __uuidof(ICorRuntimeHost), (LPVOID*)&m_pHost);
	if(FAILED(hr))
		return hr;
	hr = m_pHost->Start();
	if(FAILED(hr))
		return hr;
	hr = m_pHost->GetDefaultDomain(&pAppDomainPunk);
	if(FAILED(hr) || !pAppDomainPunk)
		return hr;
	hr = pAppDomainPunk->QueryInterface(__uuidof(mscorlib::_AppDomain), (LPVOID*)&m_pDefaultDomain);
	if(FAILED(hr) || !m_pDefaultDomain)
		return hr;

	return hr;
}