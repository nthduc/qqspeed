#include "StdAfx.h"
#include ".\gameappimpl.h"
#include "txtCtrlr.h"
#include "framework/GameApp_i.c"
#include "algorithm"
#include "common.h"
#include "fileop.h"

#define MAX_COM_RETRY_COUNT	100

CGameAppImpl::CGameAppImpl(void):
m_pActiveExt(NULL),
m_bIMEEnabled(TRUE),
m_hIMC(NULL)
{
}

CGameAppImpl::~CGameAppImpl(void)
{
	m_pActiveExt = 0;
	UnLoadExtensions();
	UnLoadServices();
}

BOOL CGameAppImpl::Init(HWND hWnd)
{
	assert(hWnd);
	m_hWnd = hWnd;
	m_bIMEEnabled = TRUE;
	if ( !LoadServices() )
		return FALSE;
	if ( !LoadExtensions() )
		return FALSE;
	return TRUE;
}

void CGameAppImpl::Update()
{
	static DWORD s_dwLastTime = GetTickCount();
	DWORD dwCurrentTime = GetTickCount();
	for ( UpdateContainer::iterator it = m_UpdateContainer.begin(); it != m_UpdateContainer.end(); it++ )
	{
		IGameUpdate* pUpdate = *it;
		pUpdate->Update(dwCurrentTime - s_dwLastTime);
	}
	s_dwLastTime = dwCurrentTime;
}

BOOL CGameAppImpl::LoadServices()
{
	CtxtCtrlr txtCtrl;
	if ( !txtCtrl.Open("Service.txt") )
	{
		XError("无法找到Service组件注册文件，默认情况下将退出程序");
		return FALSE;
	}

	while(1)
	{
		if ( !txtCtrl.Next() )
			break;
		CtxtRecordset* pRecordset = txtCtrl.GetRecordset();
		if ( pRecordset->GetFieldCount() < 2 )
			continue;
		LPCSTR pszCLSID = pRecordset->GetField(0);
		LPCSTR pszIID = pRecordset->GetField(1);
		CLSID clsid;
		WCHAR wszCLSID[MAX_PATH] = {0};
		MultiByteToWideChar(CP_ACP, 0, pszCLSID, strlen(pszCLSID), wszCLSID, MAX_PATH);
		IID iid;
		WCHAR wszIID[MAX_PATH] = {0};
		MultiByteToWideChar(CP_ACP, 0, pszIID, strlen(pszIID), wszIID, MAX_PATH);

		if ( NOERROR == CLSIDFromString(wszCLSID, &clsid) 
			&& NOERROR == IIDFromString(wszIID, &iid) )
		{
			BOOL bLoad = LoadService(clsid, iid);
		}
	}
	return TRUE;

}

HRESULT CGameAppImpl::LoadService(REFCLSID clsid, REFIID iid)
{
	CComPtr<IGameService> spService;
	HRESULT hr = E_FAIL;
	int nRetryCount = 0;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IGameService, (void**)&spService);
	LogCoCreateInstance(clsid, hr);

	if ( hr == S_OK )
	{
		hr = spService->AppInit(this);
		if ( hr == S_OK )
		{
			CComQIPtr<IGameUpdate> spUpdate = spService;
			if ( spUpdate )
			{
				m_UpdateContainer.push_back(spUpdate);
			}

			CComQIPtr<IGameWndMessage> spWndMsg = spService;
			if ( spWndMsg )
			{
				WndMsgContainer::iterator it = find(m_WndMsgContainer.begin(), m_WndMsgContainer.end(), spWndMsg);
				if ( it == m_WndMsgContainer.end() )
					m_WndMsgContainer.push_back(spWndMsg);
			}

			SvcPair aPair = { clsid, iid, spService.Detach() };
			m_SvcContainer.push_back(aPair);
		}
		else
		{
			spService->AppUnInit();
		}
	}
	assert(hr == S_OK);
	return hr;
}

STDMETHODIMP CGameAppImpl::RegService(REFIID iid, IGameService* pv)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(pv);
	{
		SvcPair aPair = { CLSID_NULL, iid, pv };
		hr = pv->AppInit(this);
		KK_CHECK(hr);
		pv->AddRef();
		m_SvcContainer.push_back(aPair);
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CGameAppImpl::GetService(REFIID iid, void** ppv)
{
	assert(ppv);
	for ( SvcPairContainer::iterator it = m_SvcContainer.begin(); it != m_SvcContainer.end(); it++ )
	{
		SvcPair& refPair = *it;
		if ( IsEqualGUID(refPair.iid, iid) )
		{
			HRESULT hr = refPair.pService->QueryInterface(iid, ppv);
			return hr;
		}
	}
	return E_FAIL;
}

STDMETHODIMP CGameAppImpl::GetService(REFIID iid, IGameService** ppv)
{
	assert(ppv);
	for ( SvcPairContainer::iterator it = m_SvcContainer.begin(); it != m_SvcContainer.end(); it++ )
	{
		SvcPair& refPair = *it;
		if ( IsEqualGUID(refPair.iid, iid) )
		{
			*ppv = refPair.pService;
			(*ppv)->AddRef();
			return S_OK;
		}
	}
	return E_FAIL;
}

BOOL CGameAppImpl::LoadExtensions()
{
	CtxtCtrlr txtCtrl;
	if ( !txtCtrl.Open("Exts.txt") )
		return FALSE;

	while(1)
	{
		if ( !txtCtrl.Next() )
			break;
		CtxtRecordset* pRecordset = txtCtrl.GetRecordset();
		LPCSTR pszCLSID = pRecordset->GetField(0);
		WCHAR wszCLSID[MAX_PATH] = {0};
		MultiByteToWideChar(CP_ACP, 0, pszCLSID, strlen(pszCLSID), wszCLSID, MAX_PATH);
		CLSID clsid;
		if ( NOERROR == CLSIDFromString(wszCLSID, &clsid) )
		{
			HRESULT hr = LoadExtension(clsid);
			assert(hr == S_OK);
		}
	}
	return TRUE;
}

HRESULT CGameAppImpl::LoadExtension(REFCLSID clsid)
{
	CComPtr<IGameExtension> spExtension;
	HRESULT hr = E_FAIL;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IGameExtension, (void**)&spExtension);
	LogCoCreateInstance(clsid, hr);

	if ( hr == S_OK )
	{
		CComQIPtr<IGameUpdate> spUpdate = spExtension;
		if ( spUpdate )
		{
			m_UpdateContainer.push_back(spUpdate);
		}

		CComQIPtr<IGameWndMessage> spWndMsg = spExtension;
		if ( spWndMsg )
		{
			WndMsgContainer::iterator it = find(m_WndMsgContainer.begin(), m_WndMsgContainer.end(), spWndMsg);
			if ( it == m_WndMsgContainer.end() )
				m_WndMsgContainer.push_back(spWndMsg);
		}

		hr = spExtension->Load(this);
		if ( hr == S_OK )
		{
			m_ExtContainer.push_back(spExtension.Detach());
		}
		else
		{
			spExtension->UnLoad();
		}
	}
	assert(hr == S_OK);
	return hr;
}

void CGameAppImpl::UnLoadServices()
{
	for ( SvcPairContainer::reverse_iterator it = m_SvcContainer.rbegin(); it != m_SvcContainer.rend(); it++ )
	{
		SvcPair& refPair = *it;
		refPair.pService->AppUnInit();
		refPair.pService->Release();
	}
	m_SvcContainer.clear();
}

void CGameAppImpl::UnLoadExtensions()
{
	for ( ExtContainer::reverse_iterator it = m_ExtContainer.rbegin(); it != m_ExtContainer.rend(); it++ )
	{
		IGameExtension* pExt = *it;
		pExt->UnLoad();
		pExt->Release();
	}
	m_ExtContainer.clear();
}

HRESULT CGameAppImpl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	for ( WndMsgContainer::iterator it = m_WndMsgContainer.begin(); it != m_WndMsgContainer.end(); it++ )
	{
		IGameWndMessage* p = *it;
		hr = p->WndProc(hWnd, message, wParam, lParam);
		if ( hr != S_OK )
		{
			break;
		}
	}
	return hr;
}

STDMETHODIMP CGameAppImpl::GetMainWnd(HWND* hWnd)
{
	assert(hWnd);
	*hWnd = m_hWnd;
	return S_OK;
}

STDMETHODIMP CGameAppImpl::ActiveExtension(LPCSTR Name)
{
	HRESULT hr = E_FAIL;
	LPCSTR pszName = NULL;
	if ( m_pActiveExt )
	{
		hr = m_pActiveExt->GetName(&pszName);
	
		if ( S_OK == hr && stricmp(Name, pszName) == 0 )
		{
			return S_OK;
		}
	}
	for ( ExtContainer::const_iterator it = m_ExtContainer.begin(); 
		it != m_ExtContainer.end(); it++ )
	{
		IGameExtension* p = *it;
		hr = p->GetName(&pszName);
		if ( S_OK == hr && stricmp(pszName, Name) == 0 )
		{
			if ( m_pActiveExt )
				m_pActiveExt->DeActive();
			hr = p->Active();
			m_pActiveExt = p;
			break;
		}
	}
	return hr;
}

STDMETHODIMP CGameAppImpl::GetVersion(DWORD* Version)
{
	assert(Version);
	*Version = 0;
	return S_OK;
}

STDMETHODIMP CGameAppImpl::GetLUVersion(DWORD* Version, DWORD* OptVersion)
{
	assert(Version);
	assert(OptVersion);
	*Version = 0;
	*OptVersion = 0;
	return S_OK;
}

STDMETHODIMP CGameAppImpl::EnableIME(BOOL bEnable)
{
	if ( !bEnable && m_bIMEEnabled )
	{
		m_hIMC = ImmAssociateContext(m_hWnd, NULL);   // It makes IME disable for hWnd window.
	}
	else if ( bEnable && !m_bIMEEnabled )
	{
		ImmAssociateContext(m_hWnd, m_hIMC);
	}
	m_bIMEEnabled = bEnable;
	return S_OK;
}

STDMETHODIMP CGameAppImpl::IsIMEEnabled(BOOL* Value)
{
	*Value = m_bIMEEnabled;
	return S_OK;
}

STDMETHODIMP CGameAppImpl::GetIntValue(LPCSTR item, INT* Value)
{
	assert(Value);
	LPCSTR szIniFile = "client.ini";
	char szIniFileFull[MAX_PATH] = "";
	_fullpath(szIniFileFull, szIniFile, MAX_PATH);
	*Value = GetPrivateProfileInt("Settings", item, 0, szIniFileFull);
	return S_OK;
}

STDMETHODIMP CGameAppImpl::SetIntValue(LPCSTR item, INT Value)
{
	LPCSTR szIniFile = "client.ini";
	char szIniFileFull[MAX_PATH] = "";
	_fullpath(szIniFileFull, szIniFile, MAX_PATH);
	char szValue[MAX_PATH] = "";
	_snprintf(szValue, MAX_PATH, "%d", Value);
	WritePrivateProfileString("Settings", item, szValue, szIniFileFull);
	return S_OK;
}

void CGameAppImpl::LogCoCreateInstance(REFCLSID clsid, HRESULT hr)
{
	LPOLESTR str;
	StringFromCLSID(clsid, &str);
	char szCLSID[MAX_PATH] = "";
	BOOL bUseDefault = FALSE;
	WideCharToMultiByte(CP_ACP, 0, str, wcslen(str), szCLSID, MAX_PATH, "", &bUseDefault);

	if ( FAILED(hr) )
	{
		XError("创建组件失败,hr=0x%X", hr);
		LogText("CoCreateInstance(%s) Failed with hr=%X\n", szCLSID, hr);
	}
	else
	{
		LogText("CoCreateInstance(%s) Succeeded with hr=%X\n", szCLSID, hr);
	}
}