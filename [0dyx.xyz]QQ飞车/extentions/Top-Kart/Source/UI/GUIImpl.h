// GUIImpl.h : Declaration of the CGUIImpl

#ifndef __GUI_H_
#define __GUI_H_

#include "../../resource.h"       // main symbols
#include "framework/GameApp_i.h"
#include "framework/gui_i.h"
#include "script.h"
#include "BubbleMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CGUIImpl
class ATL_NO_VTABLE CGUIImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGUIImpl, &CLSID_GUI>,
	public IGUI,
	public IGameService,
	public IGameWndMessage
{
public:
	CGUIImpl();
	
	//IGUI
	STDMETHOD(CreateDevice)();
	STDMETHOD(DestroyDevice)();
	STDMETHOD(LoadUI)(LPCSTR);
	STDMETHOD(UnloadUI)();
	STDMETHOD(RenderUI)();
	STDMETHOD(Update)(DWORD);
	STDMETHOD(EnterUILayout)(LPCSTR);
	STDMETHOD_(LPCSTR,			GetCurLayout)();
	STDMETHOD_(LPCSTR,			GetLastLayout)();
	STDMETHOD_(IDisplay*,		GetDisplay)();
	STDMETHOD_(IBubbleMgr*,		GetBubbleMgr)();
	STDMETHOD_(CScriptState*,	GetScriptState)();
	STDMETHOD(RegisterUICallBack)(UICallbackEvent eventName,CallBackFunc pFunc);

	//IGameService
	STDMETHOD(AppInit)(IGameApp*);
	STDMETHOD(AppUnInit)();

	//IGameWndMessage
	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//IGameUpdate
	
	DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY_GUI)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CGUIImpl)
		COM_INTERFACE_ENTRY(IGUI)
		COM_INTERFACE_ENTRY(IGameService)
		COM_INTERFACE_ENTRY(IGameWndMessage)
	END_COM_MAP()

// IGUI
public:

private:
	IGameApp*	m_pApp;
	HWND		m_hWnd;
	CScriptState	m_ss;
	CBubbleMgr		*m_pBubbleMgr;
	std::string     m_strLastLayout;
};

#endif //__GUI_H_
