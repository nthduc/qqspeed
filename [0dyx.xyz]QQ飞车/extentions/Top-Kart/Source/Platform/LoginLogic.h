////////////////////////////////////////////////
//     FileName:    LoginLogic.h
//     Author:      Fish
//     Date:        2006-10-12 9:45:30
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "../../resource.h"
#include "framework/GameApp_i.h"
#include "Network/network_i.h"
#include "TopKart_i.h"

class CLoginLogic :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLoginLogic, &CLSID_LoginLogic>,
	public IGameExtension,
	public INetworkSink	
{
public:
	CLoginLogic(void);
	~CLoginLogic(void);

	//IGameExtension
	STDMETHOD(Load)(IGameApp*);
	STDMETHOD(Active)();
	STDMETHOD(DeActive)();
	STDMETHOD(UnLoad)();
	STDMETHOD(GetName)(LPCSTR*);

	//INetworkSink
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult);
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead);
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode);
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode);
	STDMETHOD(OnCreate)(ServerEntity, int nErrorCode);
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode);

	DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY_LOGIN)
	BEGIN_COM_MAP(CLoginLogic)
		COM_INTERFACE_ENTRY(IGameExtension)
		COM_INTERFACE_ENTRY(INetworkSink)
	END_COM_MAP()

protected:
	void CheckAutoLogin();

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;
	DWORD		m_dwCookie;
};
