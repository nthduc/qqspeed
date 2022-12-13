#pragma once
#include "../../resource.h"
#include "framework/GameApp_i.h"
#include "Network/network_i.h"
#include "QQTppp_i.h"
#include "TopKart_i.h"

class CRoomLogic :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRoomLogic, &CLSID_RoomLogic>,
	public IGameExtension,
	public INetworkSink,
	public IQQTpppSink
{
public:
	CRoomLogic(void);
	~CRoomLogic(void);

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

	//IQQTpppSink
	STDMETHOD(OnUdpLoginOk)();
	STDMETHOD(OnRecvP2PDataFromSvr)(short shPlayerID, int nBodyLen, BYTE *pBody);
	STDMETHOD(OnGetPlayerInfoFromSvr)(short shPlayerID, int nUin);
	STDMETHOD(OnRecvClientTry2ConnectData)(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort);
	STDMETHOD(OnRecvP2PGameDataFromClient)(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody);
	STDMETHOD(SendTcpP2pData)(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData);

	DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY_ROOM)
	BEGIN_COM_MAP(CRoomLogic)
		COM_INTERFACE_ENTRY(IGameExtension)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IQQTpppSink)
	END_COM_MAP()

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;
	IQQTppp*	m_pQQTppp;
	DWORD		m_dwCookie;
	DWORD		m_dwP2PCookie;
};
