// GUIImpl.cpp : Implementation of CGUIImpl
#include "Top-KartPCH.h"
#include "GUIImpl.h"
#include "Display.h"
#include "ImageLib.h"
#include "GameUI.h"
#include "common.h"
#include "framework/vfs_i.h"

extern IDisplay* g_pDisplay;

#pragma comment(lib, "gameui")
#pragma comment(lib, "kernel")
#pragma comment(lib, "imagelib")
#pragma comment(lib, "ximage")
#pragma comment(lib, "UIEngine")//, ÔÝÊ±ÆÁ±Î
#pragma comment(lib, "lualib")
#pragma comment(lib, "lualibx")
#pragma comment(lib, "tolualib")
/////////////////////////////////////////////////////////////////////////////
// CGUIImpl
CGUIImpl::CGUIImpl():
m_pApp(NULL),
m_pBubbleMgr(NULL)
{
}

STDMETHODIMP CGUIImpl::AppInit(IGameApp* App)
{
	if ( g_pDisplay )
	{
		return S_FALSE;
	}
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	hr = m_pApp->GetMainWnd(&m_hWnd);
	KK_CHECK(hr);

	g_pDisplay = CreateDisplay3D();
	KK_CHECK_POINTER(g_pDisplay);

	m_pBubbleMgr = new CBubbleMgr;

KK_EXIT:
	return hr;
}

STDMETHODIMP CGUIImpl::AppUnInit()
{
	DestroyDevice();
	return S_OK;
}

STDMETHODIMP CGUIImpl::CreateDevice()
{
	HRESULT hr = E_FAIL;
	{
		hr = g_pDisplay->CreateWindowedDisplay(m_hWnd, 0);
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CGUIImpl::DestroyDevice()
{
	SAFE_DELETE(m_pBubbleMgr);
	if ( g_pDisplay )
	{
		UnloadUI();
		DestroyDisplay3D(g_pDisplay);
		g_pDisplay = NULL;
	}
	return S_OK;
}

STDMETHODIMP CGUIImpl::LoadUI(LPCSTR uiFile)
{
	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);
	int w = rcClient.right - rcClient.left;
	int h = rcClient.bottom - rcClient.top;
	bool bCreate = CreateGameUI(uiFile, w, h, &m_ss);
	return bCreate ? S_OK : E_FAIL;
}

STDMETHODIMP CGUIImpl::UnloadUI()
{
	DestroyGameUI();
	return S_OK;
}

STDMETHODIMP CGUIImpl::EnterUILayout(LPCSTR Name)
{
	string strLastLayout = GameUIGetCurScreen();
	if ( stricmp(strLastLayout.c_str(), Name) == 0 )
		return S_OK;
	m_strLastLayout = strLastLayout;
	GameUIGotoScreen(Name);
	return S_OK;
}

LPCSTR CGUIImpl::GetCurLayout()
{	
	return GameUIGetCurScreen();
}

LPCSTR CGUIImpl::GetLastLayout()
{
	if (m_strLastLayout.empty() || m_strLastLayout.compare("") == 0)
	{
		return NULL;
	}
	else
	{
		return m_strLastLayout.c_str();
	}
}

CScriptState* CGUIImpl::GetScriptState()
{
	return &m_ss;
}

IBubbleMgr* CGUIImpl::GetBubbleMgr()
{
	return m_pBubbleMgr;
}

IDisplay* CGUIImpl::GetDisplay()
{
	return g_pDisplay;
}
#include "../Profiling.h"

STDMETHODIMP CGUIImpl::Update(DWORD Elapse)
{
	PROFILE("Pure UpdateUI");
	static t0 = GetTickCount();
	DWORD t1 = GetTickCount();
	UpdateGameUI(t1-t0);	
	t0 = t1;	
	m_pBubbleMgr->Update(Elapse);

	return S_OK;
}

STDMETHODIMP CGUIImpl::RenderUI()
{
	PROFILE("Pure RenderUI");
	g_pDisplay->ClearScreen(0);
	::RenderGameUI();
	m_pBubbleMgr->Render();
	g_pDisplay->UpdateScreen();

	return S_OK;
}


STDMETHODIMP CGUIImpl::RegisterUICallBack(UICallbackEvent eventName,CallBackFunc pFunc)
{
	::RegisterUICallBack(eventName, pFunc);
	return S_OK;
}

bool IsKeyDownAsyn( int vkey )
{
	short state = GetAsyncKeyState( vkey );
	return (state&0xff00) != 0;
}

STDMETHODIMP CGUIImpl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			UINT nMode = 0;
			if( IsKeyDownAsyn(VK_CONTROL) ) nMode |= MK_CONTROL;
			if( IsKeyDownAsyn(VK_SHIFT) ) nMode |= MK_SHIFT;
			if( IsKeyDownAsyn(VK_MENU) ) nMode |= MK_ALT;

			lParam |= nMode;
		}
		break;
	}
	GameUIMessage(message, wParam, lParam);
	return S_OK;
}