#pragma once

#include "framework/GameApp_i.h"
#include "vector"
using namespace std;

typedef struct tagSvcPair
{
	CLSID			clsid;
	IID				iid;
	IGameService*	pService;
}SvcPair;

typedef vector<SvcPair>				SvcPairContainer;
typedef vector<IGameExtension*>		ExtContainer;
typedef vector<IGameUpdate*>		UpdateContainer;
typedef vector<IGameWndMessage*>	WndMsgContainer;

class CGameAppImpl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGameAppImpl, &CLSID_GameAppImpl>,
	public IGameApp,
	public IGameSettings,
	public IGameWndMessage
{
public:
	CGameAppImpl(void);
	virtual ~CGameAppImpl(void);

	//Public
	BOOL Init(HWND);
	void Update();

	//IGameApp
	STDMETHOD(GetService)(REFIID, void**);
	STDMETHOD(RegService)(REFIID, IGameService*);
	STDMETHOD(GetService)(REFIID, IGameService**);
	STDMETHOD(GetMainWnd)(HWND*);
	STDMETHOD(ActiveExtension)(LPCSTR);
	STDMETHOD(GetVersion)(DWORD*);
	STDMETHOD(GetLUVersion)(DWORD*, DWORD*);
	STDMETHOD(EnableIME)(BOOL);
	STDMETHOD(IsIMEEnabled)(BOOL*);

	//IGameSettings
	STDMETHOD(GetIntValue)(LPCSTR item, INT*);
	STDMETHOD(SetIntValue)(LPCSTR item, INT);

	//IGameWndMessage
	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/////////////////////////////////////////////////////
	DECLARE_REGISTRY_RESOURCEID(1)
	BEGIN_COM_MAP(CGameAppImpl)
		COM_INTERFACE_ENTRY(IGameApp)
		COM_INTERFACE_ENTRY(IGameSettings)
	END_COM_MAP()

private:
	BOOL LoadServices();
	HRESULT LoadService(REFCLSID, REFIID);
	BOOL LoadExtensions();
	HRESULT LoadExtension(REFCLSID);
	void UnLoadServices();
	void UnLoadExtensions();

	void LogCoCreateInstance(REFCLSID, HRESULT);

private:
	SvcPairContainer	m_SvcContainer;
	ExtContainer		m_ExtContainer;
	UpdateContainer		m_UpdateContainer;
	WndMsgContainer		m_WndMsgContainer;
	HWND				m_hWnd;
	IGameExtension*		m_pActiveExt;
	BOOL				m_bIMEEnabled;
	HIMC				m_hIMC;
};
