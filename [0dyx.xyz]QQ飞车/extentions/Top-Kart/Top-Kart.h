////////////////////////////////////////////////
//     FileName:    Top-Kart.h
//     Author:      Fish
//     Date:        2006-11-29 10:13:07
//     Description: []
////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "topkart_i.h"
#include "framework/GameApp_i.h"
#include "framework/vfs_i.h"
#include "common.h"
#include "Source/Platform/UIMiscMgr.h"
#include "Source/Platform/UIMiscCallback.h"

class CScriptEvent;
class CAntibotImpl;
class CMsgDataCache;
class CMessageHouse;
class CGPNetIntercommn;
class CAvatarNet;
class CTaskNet;

class CTopKart :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTopKart, &CLSID_TopKart>,
	public IGameExtension,
	public IGameUpdate,
	public IGameWndMessage
{
public:
	CTopKart();

	//IGameExtension
	STDMETHOD(Load)(IGameApp*);
	STDMETHOD(Active)();
	STDMETHOD(DeActive)();
	STDMETHOD(UnLoad)();
	STDMETHOD(GetName)(LPCSTR*);

	//IGameUpdate
	STDMETHOD(Update)(DWORD);

	//IGameWndMessage
	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY_MAIN)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CTopKart)
		COM_INTERFACE_ENTRY(IGameExtension)
		COM_INTERFACE_ENTRY(IGameUpdate)
		COM_INTERFACE_ENTRY(IGameWndMessage)
	END_COM_MAP()

protected:
	HRESULT InitPlatform();
	HRESULT InitEngine();
	HRESULT InitObjects();
	
private:
	IGameApp*		m_pApp;
	NiApplication*	m_pNiApp;

	CComObject<CScriptEvent>*		m_pScriptEvent;
	CComObject<CMsgDataCache>*		m_pDataCache;
	CComObject<CMessageHouse>*		m_pMessageHouse;
	CComObject<CGPNetIntercommn>*	m_pGPNetIntercommn;
	CComObject<CAvatarNet>*			m_pAvatarNet;
	CComObject<CTaskNet>*			m_pTaskNet;
	CComObject<CAntibotImpl>*		m_pAntibot;
	CUIMiscMgr						m_UIMiscMgr;
	CKartUIMiscCallback				m_UIMiscCallback;
};

