#ifndef GPNETINTERCOMMN_H
#define	GPNETINTERCOMMN_H

////////////////////////////////////////////////
//FileName:    GPNetIntercommn.h
//Author:      rayhuang
//Date:        2007-01-13
//Description: 网络交互
////////////////////////////////////////////////

#include "framework/GameApp_i.h"
#include "Network/Network_i.h"
#include "QQTppp_i.h"
#include "EventSupport.h"


class CGPNetIntercommn:
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGPNetIntercommn>,
	public INetworkSink,
	public IQQTpppSink,
	public IEventSupport
{

public:
	CGPNetIntercommn(void);
	~CGPNetIntercommn(void);

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

	BEGIN_COM_MAP(CGPNetIntercommn)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IQQTpppSink)
	END_COM_MAP()

	DECLARE_MESSAGE_MAP_QQKART


public:
	
	//上报服务器此次漂移累加的氮气量
	static void RequestDrift(unsigned int unInc);

	//请求服务器要获取位置为shPropPositionNO的道具, shPropPositionNO为位置索引
	static void RequsetGetProp(short shPropPositionNO);			


	//请求服务器使用道具， shProp为道具ID， iPropIndex为服务器生成的道具索引， unDstUin为目标的Uin
	static void RequestUseProp(short shPropID, int iPropIndex, unsigned int unDstUin, int nNewID);


	//上报服务器道具作用结果, iPropIndex为服务器生成的道具索引
	//iResult为作用结果，0表示被击中，1表示没被击中
	static void RequestPropEffectResult(short shPropID, int iPropIndex, int iResult, short sItemID);		


	//请求改变瞄准状态,unDstUin为目标的Uin, 
	//uchState为瞄准状态，0取消瞄准，1瞄准
	static void RequestChangeAimState(unsigned int unDstUin, unsigned char uchState);			

protected:

	//三维坐标点转换为三维数组
	static void Poin3DToArray3(NiPoint3 point, long* arr);

	void OnResponseDrift(void* pData);				//服务器回复因漂移取得加速卡的结果

	void OnNotifyReportDrift(void* pData);			//服务器通知组队赛中队伍氮气的积累量

	void OnResponseGetProp(void* pData);			//服务器答复获取位置为shPropPositionNO的道具

	void OnNotifyGetProp(void* pData);				//服务器通知道具的获取情况

	void OnResponseUseProp(void* pData);			//服务器答复使用请求

	void OnNotifyUseProp(void* pData);				//服务器通知道具使用情况

	void OnNotifyPropEffectResult(void* pData);		//服务器通知道具作用结果

	void OnNotifyChangeAimState(void* pData);		//服务器通知被瞄准玩家

	void OnNotifyReplaceProp(void* pData);			//服务器通知更换道具

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;		//INetworkSink
	IQQTppp*	m_pQQTppp;		//IQQTpppSink
	DWORD		m_dwCookie;
	DWORD		m_dwP2PCookie;

};
#endif