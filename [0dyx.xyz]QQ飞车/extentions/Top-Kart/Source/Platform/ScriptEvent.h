////////////////////////////////////////////////
//     FileName:    ScriptEvent.h
//     Author:      Fish
//     Date:        2006-10-12 14:59:44
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "framework/GameApp_i.h"
#include "Network/network_i.h"
#include "QQTppp_i.h"
#include "script.h"

class CScriptEvent :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CScriptEvent>,
	public INetworkSink,
	public IQQTpppSink
{
public:
	CScriptEvent():
    m_pApp(NULL),
	m_dwCookie(0),
	m_pScriptState(NULL)
	{
	}

	~CScriptEvent()
	{
		AppUnInit();
	}

	HRESULT AppInit(IGameApp*);
	HRESULT AppUnInit();

	//INetworkSink
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult);
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead);
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode);
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode);
	STDMETHOD(OnCreate)(ServerEntity, int nErrorCode);
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode);

	//IQQTpppSink
	STDMETHOD(OnUdpLoginOk)();
	STDMETHOD(OnRecvP2PDataFromSvr)(short shPlayerID, int nBodyLen, BYTE *pBody);
	STDMETHOD(OnGetPlayerInfoFromSvr)(short shPlayerID, int nUin);
	STDMETHOD(OnRecvClientTry2ConnectData)(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort);
	STDMETHOD(OnRecvP2PGameDataFromClient)(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody);
	STDMETHOD(SendTcpP2pData)(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData);

	BEGIN_COM_MAP(CScriptEvent)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IQQTpppSink)
	END_COM_MAP()

protected:
	IGameApp*		m_pApp;
	INetwork*		m_pNetwork;
	DWORD			m_dwCookie;
	DWORD			m_dwP2PCookie;
	CScriptState*	m_pScriptState;
};

