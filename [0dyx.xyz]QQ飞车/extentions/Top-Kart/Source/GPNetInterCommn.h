#ifndef GPNETINTERCOMMN_H
#define	GPNETINTERCOMMN_H

////////////////////////////////////////////////
//FileName:    GPNetIntercommn.h
//Author:      rayhuang
//Date:        2007-01-13
//Description: ���罻��
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
	
	//�ϱ��������˴�Ư���ۼӵĵ�����
	static void RequestDrift(unsigned int unInc);

	//���������Ҫ��ȡλ��ΪshPropPositionNO�ĵ���, shPropPositionNOΪλ������
	static void RequsetGetProp(short shPropPositionNO);			


	//���������ʹ�õ��ߣ� shPropΪ����ID�� iPropIndexΪ���������ɵĵ��������� unDstUinΪĿ���Uin
	static void RequestUseProp(short shPropID, int iPropIndex, unsigned int unDstUin, int nNewID);


	//�ϱ��������������ý��, iPropIndexΪ���������ɵĵ�������
	//iResultΪ���ý����0��ʾ�����У�1��ʾû������
	static void RequestPropEffectResult(short shPropID, int iPropIndex, int iResult, short sItemID);		


	//����ı���׼״̬,unDstUinΪĿ���Uin, 
	//uchStateΪ��׼״̬��0ȡ����׼��1��׼
	static void RequestChangeAimState(unsigned int unDstUin, unsigned char uchState);			

protected:

	//��ά�����ת��Ϊ��ά����
	static void Poin3DToArray3(NiPoint3 point, long* arr);

	void OnResponseDrift(void* pData);				//�������ظ���Ư��ȡ�ü��ٿ��Ľ��

	void OnNotifyReportDrift(void* pData);			//������֪ͨ������ж��鵪���Ļ�����

	void OnResponseGetProp(void* pData);			//�������𸴻�ȡλ��ΪshPropPositionNO�ĵ���

	void OnNotifyGetProp(void* pData);				//������֪ͨ���ߵĻ�ȡ���

	void OnResponseUseProp(void* pData);			//��������ʹ������

	void OnNotifyUseProp(void* pData);				//������֪ͨ����ʹ�����

	void OnNotifyPropEffectResult(void* pData);		//������֪ͨ�������ý��

	void OnNotifyChangeAimState(void* pData);		//������֪ͨ����׼���

	void OnNotifyReplaceProp(void* pData);			//������֪ͨ��������

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;		//INetworkSink
	IQQTppp*	m_pQQTppp;		//IQQTpppSink
	DWORD		m_dwCookie;
	DWORD		m_dwP2PCookie;

};
#endif