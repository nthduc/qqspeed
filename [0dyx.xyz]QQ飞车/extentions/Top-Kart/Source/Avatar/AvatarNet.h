////////////////////////////////////////////////
//FileName:    ItemDataMgr.h
//Author:      PigHuang
//Date:        2007-03-20
//Description: Avatar网络处理类
////////////////////////////////////////////////

#pragma once
#include "framework/GameApp_i.h"
#include "Network/Network_i.h"
#include "QQTppp_i.h"
#include "EventSupport.h"

class CAvatarNet :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAvatarNet>,
	public INetworkSink,
	public IQQTpppSink,
	public IEventSupport
{
public:
	CAvatarNet(void);
	virtual ~CAvatarNet(void);

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

	BEGIN_COM_MAP(CAvatarNet)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IQQTpppSink)
	END_COM_MAP()

	DECLARE_MESSAGE_MAP_QQKART

public:
	///上报改变装备状态
	static void	RequestChangeItemStatus(CRequestChangeItemStatus* pReq);
	static void RequestBuyCommodity(CRequestBuyCommodityPara* pReq);
	static void RequestGetCommodityListPara(CRequestGetCommodityListPara* pReq);

protected:
	///服务器回复改变装备状态结果
	void OnResponseChangeItemStatus(void* pData);
	void OnResponseBuyCommodity(void* pData);
	void OnResponseGetCommodityListPara(void* pData);

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;		//INetworkSink
	IQQTppp*	m_pQQTppp;		//IQQTpppSink
	DWORD		m_dwCookie;
	DWORD		m_dwP2PCookie;
};
