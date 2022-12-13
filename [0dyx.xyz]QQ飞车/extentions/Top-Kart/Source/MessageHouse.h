#pragma once
//-----------------------------------------------------------------------
// create by hoverzhao
// process game net message 
//-----------------------------------------------------------------------


#include "framework/GameApp_i.h"
#include "Network/Network_i.h"
#include "QQTppp_i.h"
#include "EventSupport.h"
//#include "IMessageHouse.h"

class CMessageHouse:
	//public IMessageHouse,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMessageHouse>,
	public INetworkSink,
	public IQQTpppSink,
	public IEventSupport
{

public:
	CMessageHouse(void);
	~CMessageHouse(void);

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

	BEGIN_COM_MAP(CMessageHouse)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IQQTpppSink)
	END_COM_MAP()

	DECLARE_MESSAGE_MAP_QQKART


protected:
	void OnNotifyGameBegin( void *);
	void OnNotifyRaceBegin( void *);
	void OnNotifyRaceOver ( void *);
	void OnNotifyGameOver ( void *);
	void OnNotifyKickFromServer( void *);
	void OnNotifyWaitUnReadyPlayers ( void * );
	void OnNotifyRaceShow ( void *);
	void OnNotifyRaceCountDown( void *);

	/*
		修改人：	ray
		日期：	6/2/2007
		描叙：	增加玩家某一play完成比赛时通知
	*/
	void OnNotifyPlayerFinishRace( void * );		

	void TerminateGame();

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;
	IQQTppp*	m_pQQTppp;
	DWORD		m_dwCookie;
	DWORD		m_dwP2PCookie;

};





