////////////////////////////////////////////////
//     FileName:    uiimpl.h
//     Author:      Fish
//     Date:        2006-12-18 14:10:27
//     Description: []
////////////////////////////////////////////////

#pragma once

#include "../../resource.h"       // main symbols
#include "framework/GameApp_i.h"
#include "uilib/uilib_i.h"
#include "uilib/D3DSceneWrap.h"
#include "IUIRenderer.h"
#include "framework/gui_i.h"
using namespace Tenio;

interface IGameApp;
class CUIImpl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUIImpl, &CLSID_GUI>,
	public IGUI,
	public IGameService,
	public IGameWndMessage
{
public:
	CUIImpl();
	~CUIImpl();

	//IGameService
	STDMETHOD(AppInit)(IGameApp*);
	STDMETHOD(AppUnInit)();
	
	//IGUI
	STDMETHOD(Update)(DWORD);
	STDMETHOD(CreateDevice)();
	STDMETHOD(DestroyDevice)();
	STDMETHOD(EnterUILayout)(LPCSTR);

	//IGameWndMessage
	STDMETHOD(WndProc)(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY_GUI)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CUIImpl)
		COM_INTERFACE_ENTRY(IGUI)
		COM_INTERFACE_ENTRY(IGameService)
		COM_INTERFACE_ENTRY(IGameWndMessage)
	END_COM_MAP()

private:
	IGameApp*		m_pApp;
	IUIRenderer*	m_pRender;
	HWND			m_hWnd;
	RECT			m_rcClient;
	CD3DCanvasWrap	m_Canvas;
	TTenioPtr<IUILib> m_pUILib;
	TTenioPtr<IMemoryAlloctor> m_pMemoryAlloctor;
};