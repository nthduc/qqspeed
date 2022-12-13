////////////////////////////////////////////////
//     FileName:    TaskNet.h
//     Author:      rayhuang
//     Date:        2007-5-23
//     Description: 任务系统网络部分
////////////////////////////////////////////////


#pragma once 
#include "framework/GameApp_i.h"
#include "Network/Network_i.h"
#include "QQTppp_i.h"
#include "EventSupport.h"


class CTaskNet:
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTaskNet>,
	public INetworkSink,
	public IQQTpppSink,
	public IEventSupport
{

public:
	CTaskNet(void);
	~CTaskNet(void);

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

	BEGIN_COM_MAP(CTaskNet)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IQQTpppSink)
	END_COM_MAP()

	DECLARE_MESSAGE_MAP_QQKART


public:

	//从服务器拉取任务列表
	static void RequestGetTaskList(DWORD dwUin);

	//请求接受任务(跟npc会话接受Option主被动）
	static void RequestTakeTask(DWORD dwUin, DWORD dwTaskID, DWORD dwOption);	
	
	//请求删除id为dwTaskID的任务
	static void RequestDeleteTask(DWORD dwUin, DWORD dwTaskID);

	//请求开始单局任务（单击任务图标）
	static void RequestStartSingleTask(DWORD dwUin, DWORD dwTaskID, DWORD dwMapId);

	//上报任务中的关键点，操作序列
	static void RequestReportCurrentOpt(DWORD dwUin, DWORD dwOpt);

	//上报服务器准备完毕，可以开始比赛了
	static void RequestPrepareReady(DWORD dwUin);

	//上报服务器checkpoint点
	static void RequestReportCheckPoint();

	//请求服务器跳过任务
	static void RequestSkipTask(DWORD dwUin, DWORD dwID);

protected:

	//服务器返回拉取任务列表的结果
	void OnResponseGetTaskList(void* pData);	

	//服务器返回请求开始任务的结果
	void OnResponseTakeTask(void* pData);

	//服务器返回请求删除任务的结果
	void OnResponseDeleteTask(void* pData);

	//服务器返回请求开始任务的结果（调用RequestStartSingleTask时，错误时服务器发这个）
	void OnResponseStartSingleTask(void* pData);

	//服务器返回请求开始任务的结果（调用RequestStartSingleTask时，正确时服务器发这个）
	void OnNotifySingleGameBegin(void* pData);

	//服务器通知开始比赛了
	void OnNotifySingleRaceBegin(void* pData);

	//服务器通知比赛结束(客户端显示是否完成等）
	void OnNotifySingleRaceFinish(void* pData);

	//服务器下发比赛结果（显示结果）
	void OnNotifySingleRaceOver(void* pData);

	//服务器通知游戏结束（退出当前游戏）
	void OnNotifySingleGameOver(void* pData);

	//服务器回复跳过任务
	void OnResponseSkipTask(void* pData);

	//服务器通知奖励
	void OnNotifyTaskAward(void* pData);

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;		//INetworkSink
	IQQTppp*	m_pQQTppp;		//IQQTpppSink
	DWORD		m_dwCookie;
	DWORD		m_dwP2PCookie;
	//for test
	bool		m_bFirst;

};