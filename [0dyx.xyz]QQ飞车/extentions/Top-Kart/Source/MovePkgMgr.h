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
设计思路：

	NxMovePkgMgr 需要在游戏开始的时候初始化，游戏结束的时候销毁，
	所以需要Create Destory函数。它主要负责游戏当中消息包的发送和接收
	的处理逻辑 ，同时当玩家到达某些CHECKPOINT 时 要上报服务器消息。

UDP包的发送
	先从游戏开始 说起吧 ，  初始化 需要发送 UDP 包的玩家（也就是远程玩家），
	把它们放入到一个 map 中维护 uin 和默认的 frequence = 0 , 起个名字 _MapUdpSendTable;
	
	在游戏进行过程中，根据本地玩家，与远程玩家彼此间距离的远近，动态调整发包频率。
	发包频率的距离影射表可以从脚本当中获得，游戏中及时更新 _MapUdpSendTable的值。
	
	如果玩家已经抵达终点 那么它在_MapUdpSendTable中相应的 frequence = 0;
	如果玩家离开游戏     处理同上

	_MapUdpSendTable 的使用 ：在NxMovePkgMgr update() 时 本地玩家 从_MapUdpSendTable

	中获得 发送频率（这个值要保证相对稳定）， 这时 知道 每个远程玩家的上次发包时间
	和已过时间 当表达式 lastsendtime + deltatime > 1/frequece  成立则发送UDP包，更新
	lastsendtime = gettickcount(); deltatime = 0; else deltatime + = ftime;

	如果玩家已经抵达终点 或离开（不必删除远程玩家，在游戏结束后一起删除吧）

	2005.12.20 号 需要 svr 转发的情况 和游戏中玩家离开的情况:
	_MapUdpSendTable 中添加两个字段 _byServer _disable.在每次 Update 的时候,调用UpdateUdpSendTable
	实时调整 发送的策略.如果需要通过server 转发Udp包 那么_byServer true;

UDP包的接收
	这个逻辑比较简单
	使用 Process函数处理一下 先 check 一下，如果已经收到过 就不再处理了
	然后 push  一下 保存起来 最后进行 triger  设置kart的物理参数

TCP包的发送
	也比较简单 只对本地玩家做处理就可以了 ，每隔一段时间 就 发一个包
	游戏结束发一个包，就ok了

TCP 包的接受
	没这个处理的咚咚吧


	在进行游戏当中 也就是scenelevel update 的时候

总结： 
	这个类的名字应该叫
	
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


//udp 接收处理函数以及相关变量

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

// udp 发送处理函数 这块儿会比较复杂

	struct SenderNotebook
	{
		ULONG _uin;
		UINT  _lastTime;
		float _deltaTime;
		ULONG _frequence;
		bool  _disable;			// 玩家离开
		bool  _byserver;		// 是否通过svr转发
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

	// 这几个函数应该在UPDATE 函数调用
public:
	void	SendUdp(float fTime);

protected:
	void	Analaysis(eAnalaysis );
	ULONG	ComputeDistance(Play * pPlay1, Play * pPlay2);  
	ULONG	GetFrequence  (ULONG nDistance);
	void	SendUdpPkg(float fTime);

private:
	std::vector< SenderNotebook > _MapUdpSendTable; 

// tcp 发送处理函数 也该出现在UDDATE 函数调用中
//	条件逻辑应该写在这个函数里面

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
	// UIN	包ID
	typedef std::map<ULONG, LONG> MapType;
	typedef MapType::iterator MapIterator;
	typedef MapType::const_iterator ConstMapIterator;

	// 移动包
	typedef std::vector<STUDPPlayMove> DataType;
	typedef DataType::iterator DataIterator;
	typedef DataType::const_iterator ConstDataIterator;

	//Uin  发包频率
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


