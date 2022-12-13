////////////////////////////////////////////////
//     FileName:    uiimpl.cpp
//     Author:      Fish
//     Date:        2006-12-18 14:10:30
//     Description: []
////////////////////////////////////////////////
#include "Top-KartPCH.h"
#include "uiimpl.h"
#include "framework/GameApp_i.h"
#include "common.h"
#include "uiproperty/propertyid.h"
#include "uilib/uiconfig_i.h"
#include "uilib/control_i.h"
#include "../../libsrc/UIEngine/gbUIRenderer.h"

#pragma comment(lib, "kernel")
#pragma comment(lib, "UIEngine")//, ÔÝÊ±ÆÁ±Î
#pragma comment(lib, "lualib")
#pragma comment(lib, "lualibx")
#pragma comment(lib, "tolualib")

CUIImpl::CUIImpl():
m_pApp(NULL),
m_pRender(NULL),
m_hWnd(NULL)
{
}

CUIImpl::~CUIImpl()
{
	AppUnInit();
}

HRESULT CUIImpl::AppInit(IGameApp* App)
{
	assert(App);
	m_pApp = App;
	HRESULT hr = m_pApp->GetMainWnd(&m_hWnd);
	KK_CHECK(hr);
KK_EXIT:
	return hr;
}

HRESULT CUIImpl::AppUnInit()
{
	DestroyDevice();
	return S_OK;
}

HRESULT CUIImpl::Update(DWORD dwElapse)
{
	DWORD t1 = GetTickCount();
	m_pRender->BeginUI();
	m_Canvas.OnDraw();

	BYTE* pData = NULL;
	long lSize = 0;
	m_Canvas.GetPixelBuffer(pData, lSize);
	TBitImage tb = {0};
	tb.attrib = 2;
	tb.h = m_rcClient.bottom - m_rcClient.top;
	tb.w = m_rcClient.right - m_rcClient.left;
	tb.pitch = tb.w * 4;
	memcpy(tb.data, &pData, 4);

	DWORD dwBegin = GetTickCount();
	char caTime[256] = {0};

	IJuneRegion* pRegion = m_Canvas.GetClipRegion();
	int nRectCount = pRegion->EnumRects(NULL, 0);	
	RECT* prcList = NULL;
	prcList = (RECT*)m_pMemoryAlloctor->Alloc(NULL, sizeof(RECT)*nRectCount);
	memset(prcList, 0, sizeof(RECT)*nRectCount);
	pRegion->EnumRects(prcList, nRectCount);

	m_pRender->UpdateUIScreen( prcList , nRectCount , &tb ) ;

	m_pMemoryAlloctor->Free((LPVOID)prcList);
	pRegion->ClearAll();
	m_pRender->EndUI();
	DWORD t2 = GetTickCount();
	DWORD t = t2 - t1;
	return S_OK;
}

HRESULT CUIImpl::WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT lRet = 0;
	TTenioPtr<IVariableParam> pParam;
	pParam->SetValue(PARAMID_MESSAGE, (int)msg);
	pParam->SetValue(PARAMID_WPARAM, (DWORD)wparam);
	pParam->SetValue(PARAMID_LPARAM, (DWORD)lparam);
	pParam->SetValue(PARAMID_RESULT, (DWORD)(lRet));

	BOOL bUserDo = m_pUILib->OnOutWindowMessage(pParam);
	lRet = pParam->GetDword(PARAMID_RESULT, FALSE);

	if(bUserDo)
	{
		return S_FALSE;
	}
	return S_OK;
}

STDMETHODIMP CUIImpl::CreateDevice()
{
	RELEASE(m_pRender);
	m_pRender = CreateUIRenderer(m_hWnd);
	assert(m_pRender);
	GetClientRect(m_hWnd, &m_rcClient);
	SIZE sz = { m_rcClient.right, m_rcClient.bottom };
	m_pRender->CreateUIScreenBlock(sz.cx, sz.cy);
	m_Canvas.Initialize(sz);
	m_Canvas.CreateScene(static_cast<IJuneCanvasUpdate*>(&m_Canvas));
	m_pUILib->SetRootScene(m_Canvas.m_pScene, m_Canvas.m_pSceneUpdate);
	m_pUILib->SetRootParentHandle(m_hWnd);
	IUIConfig* pConfig = m_pUILib->GetConfig();
	if ( pConfig )
	{
		char szCurDir[MAX_PATH] = "";
		GetCurrentDirectory(MAX_PATH, szCurDir);
		char szTmpPath[MAX_PATH] = "";
		_makepath(szTmpPath, NULL, szCurDir, "Data/ui/layout/", NULL);
		pConfig->SetLayoutRootPath(szTmpPath);
		_makepath(szTmpPath, NULL, szCurDir, "Data/ui/res/", NULL);
		pConfig->SetResRootPath(szTmpPath);
		_makepath(szTmpPath, NULL, szCurDir, "Data/ui/config/", NULL);
		pConfig->SetName2IDFileRootPath(szTmpPath);
		_makepath(szTmpPath, NULL, szCurDir, "Data/ui/script/", NULL);
		pConfig->SetScriptRootPath(szTmpPath);
		_makepath(szTmpPath, NULL, szCurDir, "Data/ui/style/", NULL);
		pConfig->SetStyleRootPath(szTmpPath);
	}
	EnterUILayout("login");
	return S_OK;
}

STDMETHODIMP CUIImpl::DestroyDevice()
{
	RELEASE(m_pRender);
	return S_OK;
}

STDMETHODIMP CUIImpl::EnterUILayout(LPCSTR lpszLayoutName)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(m_pUILib);
	{
		char szFileName[MAX_PATH] = "";
		_snprintf(szFileName, MAX_PATH, "%s%s", lpszLayoutName, ".xml");
		BOOL bRet = m_pUILib->RestoreFromXMLFile(szFileName);
		KK_CHECK_BOOL(bRet);
		IControl* pRootCtrl = (IControl*)m_pUILib->GetRootControlPtr();
		if ( pRootCtrl )
		{
			pRootCtrl->Show(TRUE);
			hr = S_OK;
		}
		else
		{
			hr = E_FAIL;
		}
	}
KK_EXIT:
	return hr;
}