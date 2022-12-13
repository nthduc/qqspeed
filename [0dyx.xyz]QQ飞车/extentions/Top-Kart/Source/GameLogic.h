#pragma once


#include "../resource.h"
#include "framework/GameApp_i.h"
#include "Network/network_i.h"
#include "QQTppp_i.h"
#include "TopKart_i.h"
#include "MessageHouse.h"

interface IGUI;
class CGameLogic:
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGameLogic>,
	public IGameExtension, 
	public IGameUpdate,
	public INetworkSink,
	public IQQTpppSink
{

public:
	CGameLogic(void);
	~CGameLogic(void);

	//IGameUpdate
	STDMETHOD(Update)(DWORD);

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


	BEGIN_COM_MAP(CGameLogic)
		COM_INTERFACE_ENTRY(IGameExtension)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IQQTpppSink)
		COM_INTERFACE_ENTRY(IGameUpdate)
	END_COM_MAP()


public:
	void StartGameLogic(CNotifyGameBeginPara* pData);

protected:
	bool LoadScene();
	void PlacedPlayers();

private:
	CNotifyGameBeginPara* m_pGameBeginPara;
	unsigned int m_uiLoadtime;

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;
	IQQTppp*	m_pQQTppp;
	IGUI*		m_pGUI;
	DWORD		m_dwCookie;
	DWORD		m_dwP2PCookie;


};