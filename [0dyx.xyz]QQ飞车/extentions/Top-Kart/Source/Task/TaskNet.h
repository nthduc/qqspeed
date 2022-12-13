////////////////////////////////////////////////
//     FileName:    TaskNet.h
//     Author:      rayhuang
//     Date:        2007-5-23
//     Description: ����ϵͳ���粿��
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

	//�ӷ�������ȡ�����б�
	static void RequestGetTaskList(DWORD dwUin);

	//�����������(��npc�Ự����Option��������
	static void RequestTakeTask(DWORD dwUin, DWORD dwTaskID, DWORD dwOption);	
	
	//����ɾ��idΪdwTaskID������
	static void RequestDeleteTask(DWORD dwUin, DWORD dwTaskID);

	//����ʼ�������񣨵�������ͼ�꣩
	static void RequestStartSingleTask(DWORD dwUin, DWORD dwTaskID, DWORD dwMapId);

	//�ϱ������еĹؼ��㣬��������
	static void RequestReportCurrentOpt(DWORD dwUin, DWORD dwOpt);

	//�ϱ�������׼����ϣ����Կ�ʼ������
	static void RequestPrepareReady(DWORD dwUin);

	//�ϱ�������checkpoint��
	static void RequestReportCheckPoint();

	//�����������������
	static void RequestSkipTask(DWORD dwUin, DWORD dwID);

protected:

	//������������ȡ�����б�Ľ��
	void OnResponseGetTaskList(void* pData);	

	//��������������ʼ����Ľ��
	void OnResponseTakeTask(void* pData);

	//��������������ɾ������Ľ��
	void OnResponseDeleteTask(void* pData);

	//��������������ʼ����Ľ��������RequestStartSingleTaskʱ������ʱ�������������
	void OnResponseStartSingleTask(void* pData);

	//��������������ʼ����Ľ��������RequestStartSingleTaskʱ����ȷʱ�������������
	void OnNotifySingleGameBegin(void* pData);

	//������֪ͨ��ʼ������
	void OnNotifySingleRaceBegin(void* pData);

	//������֪ͨ��������(�ͻ�����ʾ�Ƿ���ɵȣ�
	void OnNotifySingleRaceFinish(void* pData);

	//�������·������������ʾ�����
	void OnNotifySingleRaceOver(void* pData);

	//������֪ͨ��Ϸ�������˳���ǰ��Ϸ��
	void OnNotifySingleGameOver(void* pData);

	//�������ظ���������
	void OnResponseSkipTask(void* pData);

	//������֪ͨ����
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