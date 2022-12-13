#pragma once

/*-----------------------------------------------------------------------
Create By HoverZhao 2006.12
------------------------------------------------------------------------*/

#include <vector>
#include <map>
#include "singlet.h"
#include "QQCarLogicNet.h"

#define SEND_TCPPACK_TIME 4000
/*-----------------------------------------------------------------------
���˼·��

	NxMovePkgMgr ��Ҫ����Ϸ��ʼ��ʱ���ʼ������Ϸ������ʱ�����٣�
	������ҪCreate Destory����������Ҫ������Ϸ������Ϣ���ķ��ͺͽ���
	�Ĵ����߼� ��ͬʱ����ҵ���ĳЩCHECKPOINT ʱ Ҫ�ϱ���������Ϣ��

UDP���ķ���
	�ȴ���Ϸ��ʼ ˵��� ��  ��ʼ�� ��Ҫ���� UDP ������ң�Ҳ����Զ����ң���
	�����Ƿ��뵽һ�� map ��ά�� uin ��Ĭ�ϵ� frequence = 0 , ������� _MapUdpSendTable;
	
	����Ϸ���й����У����ݱ�����ң���Զ����ұ˴˼�����Զ������̬��������Ƶ�ʡ�
	����Ƶ�ʵľ���Ӱ�����Դӽű����л�ã���Ϸ�м�ʱ���� _MapUdpSendTable��ֵ��
	
	�������Ѿ��ִ��յ� ��ô����_MapUdpSendTable����Ӧ�� frequence = 0;
	�������뿪��Ϸ     ����ͬ��

	_MapUdpSendTable ��ʹ�� ����NxMovePkgMgr update() ʱ ������� ��_MapUdpSendTable

	�л�� ����Ƶ�ʣ����ֵҪ��֤����ȶ����� ��ʱ ֪�� ÿ��Զ����ҵ��ϴη���ʱ��
	���ѹ�ʱ�� �����ʽ lastsendtime + deltatime > 1/frequece  ��������UDP��������
	lastsendtime = gettickcount(); deltatime = 0; else deltatime + = ftime;

	�������Ѿ��ִ��յ� ���뿪������ɾ��Զ����ң�����Ϸ������һ��ɾ���ɣ�

	2005.12.20 �� ��Ҫ svr ת������� ����Ϸ������뿪�����:
	_MapUdpSendTable ����������ֶ� _byServer _disable.��ÿ�� Update ��ʱ��,����UpdateUdpSendTable
	ʵʱ���� ���͵Ĳ���.�����Ҫͨ��server ת��Udp�� ��ô_byServer true;

UDP���Ľ���
	����߼��Ƚϼ�
	ʹ�� Process��������һ�� �� check һ�£�����Ѿ��յ��� �Ͳ��ٴ�����
	Ȼ�� push  һ�� �������� ������ triger  ����kart���������

TCP���ķ���
	Ҳ�Ƚϼ� ֻ�Ա������������Ϳ����� ��ÿ��һ��ʱ�� �� ��һ����
	��Ϸ������һ��������ok��

TCP ���Ľ���
	û�����������˰�


	�ڽ�����Ϸ���� Ҳ����scenelevel update ��ʱ��

�ܽ᣺ 
	����������Ӧ�ý�
	
	NxGamePkgMgr;
------------------------------------------------------------------------*/


class NxGamePkgMgr : public CSingleT<NxGamePkgMgr>
{

public:
	bool Start();
	void Update(float fTime);
	bool Restart(){ return Start();}
	
	void UpdateUdpConnect();

	NxGamePkgMgr();
	~NxGamePkgMgr();

private:
	static NxGamePkgMgr * ms_pGamePkgMgr;


//udp ���մ������Լ���ر���

	struct ReciveNotebook
	{
		ULONG _uin;
		ULONG _index;
	};

public:
	void		ProcessUdpPkg(STUdpPack * );
protected:
	bool		CheckUdpPkg		(ULONG dwUin, ULONG index);	
	void		TrigerUdpPkg	(ULONG dwUin, ULONG index,STUDPPlayMove * );
	void        TriggerUdpCollisionPkg( STUDPPlayCollision * );


	void		PushUdpPkg		(STUDPPlayMove * );
	float		GetSwooshFactor	(ULONG dwUin );

private:
	std::vector<ReciveNotebook> _MapUdpReciveTable;
	std::vector<STUDPPlayMove>  _UdpPkgDatabase;

// udp ���ʹ����� ������Ƚϸ���

	struct SenderNotebook
	{
		ULONG _uin;
		UINT  _lastTime;
		float _deltaTime;
		ULONG _frequence;
		bool  _disable;			// ����뿪
		bool  _byserver;		// �Ƿ�ͨ��svrת��
		ULONG _pkgid;
		INT _udpSpeed;
	};
	enum eAnalaysis
	{
		ANA_DISTANCE,
		ANA_VIEW
	};

	

public:
	typedef enum enmPackType
	{
		EPT_NORMAL,
		EPT_PLAYCOLLISON,

	}ENMPACKTYPE;

	// �⼸������Ӧ����UPDATE ��������
public:
	void	SendUdp(float fTime);

protected:
	void	Analaysis(eAnalaysis );
	ULONG	ComputeDistance(Play * pPlay1, Play * pPlay2);  
	ULONG	GetFrequence  (ULONG nDistance);
	void	SendUdpPkg(float fTime);

private:
	std::vector< SenderNotebook > _MapUdpSendTable; 

// tcp ���ʹ����� Ҳ�ó�����UDDATE ����������
//	�����߼�Ӧ��д�������������

public: 
	void    SendTcp(float fTime);

protected:
	void SendTcpPkg(float fTime);
	INT	m_nTcpLastSendTime;

	//float   m_nTcpDeltaTime;
	bool    m_bSendLastTcp;


protected:
	UINT 	m_nCurTime;


public:
	UINT m_nRcvUdpPkgStatic;
	UINT m_nIncUdpPkg;
	DWORD m_nStaticTime;
	
public:
	static void SendUdpPkgImmt( void * pData,ENMPACKTYPE eType = EPT_NORMAL);

	static int  RegistLisener			( IQQCarLogicNet * pLisener );
	static void UnRegistLisener			( int iCookie ); 

	static std::map<int , IQQCarLogicNet*> ms_arrLisener;
	static int	m_nRegistCookie;

public:
	virtual void OnNetMessage(UINT MsgID, void * pData, int MsgType = 0); // 0 Udp 1 Tcp
	

};














/*

class NxMovePkgMgr
{
public:
	// UIN	��ID
	typedef std::map<ULONG, LONG> MapType;
	typedef MapType::iterator MapIterator;
	typedef MapType::const_iterator ConstMapIterator;

	// �ƶ���
	typedef std::vector<STUDPPlayMove> DataType;
	typedef DataType::iterator DataIterator;
	typedef DataType::const_iterator ConstDataIterator;

	//Uin  ����Ƶ��
	typedef std::map<ULONG,ULONG> PlayType;
	typedef PlayType::iterator PlayIterator;
	typedef PlayType::const_iterator ConstPlayIterator;

	enum eFrequency
	{
		NOSEND = 0,
		LOW_SEND = 5,
		MID_SEND = 15,
		HIGHT_SEND = 30
	};

	enum eDistance
	{
	};

	enum eAnalaysis
	{
		ANA_DISTANCE,
		ANA_VIEW
	};

private:
	static DataType _mapData;
	static MapType	_mapUin;
	static PlayType _needSendplay;
	


public:
	static bool 	init();
	static bool     restart() { return init();}
	static void		process(ULONG dwUin, ULONG index , STUDPPlayMove *);
	static void     update();


protected:
	static bool		check_pkg	(ULONG dwUin, ULONG index);	
	static void		push_pkg	(ULONG dwUin, ULONG index,STUDPPlayMove * );
	static void		triger_pkg	(ULONG dwUin, ULONG index,STUDPPlayMove * );
	static void		analaysis(eAnalaysis AnaType);
	static void		sendudp();
	static void     sendtcp();

protected:
	static ULONG computedistance(Play * pPlay1, Play * pPlay2);
	static ULONG getfrequece(ULONG nDistance);


protected:

};
*/


