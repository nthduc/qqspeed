// QQTUdpClientSocket.h: interface for the CQQTUdpClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QQTUDPCLIENTSOCKET_H__492475EC_686D_4C49_A3B4_9EC9A6808AD2__INCLUDED_)
#define AFX_QQTUDPCLIENTSOCKET_H__492475EC_686D_4C49_A3B4_9EC9A6808AD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "QQTUdpServerSocket.h"
#include "QQTUdpBufferMgr.h"
#include "QQTTempl.h"
#define RE_TRANSMIT_TIME_LENGTH						3
#define RE_SEND_CONNECT_DATA_LENGTH					3
#define TCP_CHECK_RESEND_TIMER						4
#define P2P_HELLO_TIMER								5
#define TCP_RESEND_TIME_LENGTH						3000

struct _qqt_cc_udp_head;
class CQQTUdpClientSocket : public CQQTUdpProtocolSocket::IQQTUdpProtocolSocketReceiver
{
public:
	class IUdpClientCallBack
	{
	public:
		virtual void OnClientClose(short shPlayerID, int nUin) PURE;
		virtual void OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort) PURE;
		virtual void OnRecvP2PGameDataFromClient(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, BYTE *pBody, int nBodyLen) PURE;
		virtual ULONG GetMyOutterIP() PURE;
		virtual byte GetRoundMark() PURE;
		virtual CQQTUdpBufferMgr::IMsgBuffer* GetPreBuffer(int iClientTag) PURE;
		virtual void SetLastBuffer(int iClientTag, CQQTUdpBufferMgr::IMsgBuffer* pMsgBuffer) PURE;
		virtual void SendTcpP2pData(int nPlayerCount, short *asPlayerID, DWORD *auiUin, int iLen, byte *pBuffer) PURE;
	};
public:
	CQQTUdpClientSocket();
	virtual ~CQQTUdpClientSocket();
	//////////////////////////////////////////////////////////////////////////
	// CQQTUdpClientSocket operation
	void Initialize();
	void SetMyInfo(DWORD dwUin, short sPlayerID)					{	m_ProtocolSocket.SetMyInfo(dwUin, sPlayerID);	}
	void SetDestPlayerInfo(DWORD dwUin, short sPlayerID)		{ m_dwDestUin = dwUin; m_sDestPlayerID = sPlayerID;	}
	void SetGameStatus(int iGameStatus);
	void SetConnect(IUdpClientCallBack *pIUdpClientCallBack, CQQTUdpServerSocket *pSvrSocket, int iTag)
		{m_pUdpSvrSocket = pSvrSocket; m_pUdpClientCallBack = pIUdpClientCallBack; m_iClientTag = iTag;}
	void SetServerAddr(ULONG ulSvrIP, USHORT usSvrPort)		{	m_ulSvrIP = ulSvrIP; m_usSvrPort = usSvrPort;	}
	void StartConnectPlayer();
	void SetTransmitOK();
	void OnTimer();
	void SetDestPlayerAddr(ULONG ulOuterIP, USHORT usOuterPort, ULONG ulInnerIP, USHORT usInnerPort);
	BOOL IsDestPlayer(DWORD dwUin)										{	return (m_dwDestUin == dwUin) ? TRUE : FALSE;	}
	void SetCanPass(ULONG ulIP, USHORT usPort);
	void SendP2PGameData(UINT uiLen, BYTE *pBuf, BOOL bKeyData, short* sPlayerIDs = NULL, UINT* uiUins = NULL, INT nNumOfPlayer = 0, BOOL bForceUseServer = FALSE);
	void SendTcpDataToServer(UINT uiLen, BYTE *pBuf, short *ashPlayerID, UINT *auiUin, int nPlayerCount);
	void GetDestPlayerInfo(DWORD &dwUin, short &shPlayerID){dwUin = m_dwDestUin; shPlayerID = m_sDestPlayerID;}
	bool CanPass()	{ return m_iConnectState == _connection_state_can_pass; }
	DWORD GetNetSpeed() 
	{ 
		if ( GetTickCount() - m_dwNetSpeedLastTime > 5000 )
			{ return 0xffffffff; }
		else
			{ return m_dwNetSpeed; }
	}
protected:
	//////////////////////////////////////////////////////////////////////////
	// implementation of CQQTUdpProtocolSocket::IQQTUdpProtocolSocketReceiver
	virtual void OnClose();
	virtual void OnLoginResponse(ULONG ulSvrIP, ULONG ulOuterIP){ASSERT(FALSE);}
	virtual void OnGetPlayerInfoFromSvr(ULONG ulSvrIP, short shSrcPlayerID, int nSrcUin, ULONG ulSrcOuterIP, USHORT usSrcOuterPort, ULONG ulSrcInerIP, USHORT usSrcInnerPort){ASSERT(FALSE);}
	virtual void OnRecvP2PTestConnectData(ULONG ulIP, USHORT usPort, ULONG ulTestMyIP, USHORT usTestMyPort, byte byCmd);
	virtual void OnRecvP2PTestNetSpeed(const _qqt_cc_udp_head*);
	virtual void OnRecvP2PTestNetSpeedReply(const _qqt_cc_udp_head*);

public:
	virtual void OnRecvP2PGameData(ULONG ulSrcIP, short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen);
	void OnGetTcpP2pGameData(short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen);
protected:
#ifdef _ENABLE_QQT_LOG_
	virtual void LogString(const char *pFormat, ...);
	virtual void LogBin(int nLen, const char *pBuf);
	virtual void Log2Win(char* pFormat, ...);
	char m_szLogPath[MAX_PATH];
#endif
private:
	void SetDestPlayerP2PAddr(ULONG ulIP, USHORT usPort);
	void ProcessP2pData(short sPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen);
	void Transmit();
	void SendTestData(BYTE byCmd);
	short GetSeq()								{	return m_sSequence ++;	}
	void CheckTcpData();
protected:
	enum _connection_state
	{
		_connection_state_no_start = 0,
		_connection_state_transmit,
		_connection_state_transmit_ok,
		_connection_state_testing,
		_connection_state_can_not_pass,
		_connection_state_can_pass
	};


	#define FAST_MSG_PTR_ARRAY_SIZE								4096
	class CFastMsgPtrArray
	{
	public:
		CFastMsgPtrArray();
		~CFastMsgPtrArray();
		void Add(CQQTUdpBufferMgr::IMsgBuffer *p, BOOL bSetPre = TRUE);
		void Remove(int i);
		void FreeAll();
		int GetSize() const																								{		return m_iSize;		}
		void RefreshPtrArrayByRecvFlag(int iClientTag, int iMinUnRecv, int iMaxUnRecv, byte byFlag);
		CQQTUdpBufferMgr::IMsgBuffer* GetPre();
		CQQTUdpBufferMgr::IMsgBuffer* GetLongestNoSendPriority2(int iClientTag);
		CQQTUdpBufferMgr::IMsgBuffer* operator[](int i);
	private:
		CQQTUdpBufferMgr::IMsgBuffer * m_arPtr[FAST_MSG_PTR_ARRAY_SIZE];
		int m_iTagPre;
		int m_iSize;
	};
	class CBoolBitSet
	{
	#define BIT_SET_SIZE			1024
	public:
		CBoolBitSet();
		~CBoolBitSet();

		void Clear();
		void MarkBit(int iPos);
		void ClearBit(int iPos);
		BOOL GetBit(int iPos);
		void GetReceiveFlag(int iMinUnReceive, int iMaxUnReceive, byte &byFlag);
		short GetMaxUnRecv();
		short GetMinUnReceive();
	private:
		byte m_arByte[BIT_SET_SIZE];		
		int m_iSize;
		short m_sMaxUnReceive;
		short m_sMinUnReceive;
	};

	#pragma pack(push, 1)
	typedef struct _qqt_udp_app_head
	{
		byte byNumberOfPack;				//打包了多少个game msg
		byte byRoundMark;						//局号
		short sMinUnRecv;						//最小的没收到的包		
		short sMaxUnRecv;						//最大收到的包
		byte byRecvFlag;							//前４个bit表示是否收到：
																//				sMinUnRecv + 1
																//				sMinUnRecv + 2
																//				sMinUnRecv + 3
																//				sMinUnRecv + 4
																//后４个bit表示是否收到：
																//				sMaxUnRecv - 1
																//				sMaxUnRecv - 2
																//				sMaxUnRecv - 3
																//				sMaxUnRecv - 4
	};
	typedef struct _qqt_udp_app_pack
	{
		short sSeq;
		unsigned short usLen;
		byte byBuffer[BIG_BUFFER_SIZE];
	};
	#pragma pack(pop)
	BOOL EncodeAppPack(_qqt_udp_app_pack &appPack, byte *pOutBuffer, int &iOutBufferLen);
	BOOL EncodeAppHead(_qqt_udp_app_head &appHead, byte *pOutBuffer, int &iOutBufferLen);
	BOOL DecodeAppPack(_qqt_udp_app_pack &appPack, byte *pInBuffer, int &iInBufferLen);
	BOOL DecodeAppHead(_qqt_udp_app_head &appHead, byte *pInBuffer, int &iInBufferLen);
	int GetAppPackLength(_qqt_udp_app_pack &appPack);
private:
	CQQTUdpProtocolSocket m_ProtocolSocket;
	IUdpClientCallBack *m_pUdpClientCallBack;
	CQQTUdpServerSocket *m_pUdpSvrSocket;

	//////////////////////////////////////////////////////////////////////////
	int m_iTestConnectTimer;
	int m_iTransmitTimer;
	int m_iTcpSendTImer;
	int m_iHelloTimer;
	int m_iTestNetSpeedTimer;
	//////////////////////////////////////////////////////////////////////////		
	int m_iGameStatus;							// GameStatus
	ULONG m_ulSvrIP;							// Server IP
	USHORT m_usSvrPort;						// Server Port
	int m_iConnectState;							// Conncet State
	DWORD m_dwDestUin;						//Dest Player Uin
	short	m_sDestPlayerID;					//Dest Player ID
	ULONG m_ulDestPlayerOuterIP;
	USHORT m_usDesplaterOuterPort;
	ULONG m_ulDestPlayerInnerIP;
	USHORT m_usDestPlayerInnerPort;
	BOOL m_bFirstSendTcpRecv;
	int m_iClientTag;
	short m_sSequence;

	CFastMsgPtrArray m_arPtrMsgBuffer;
	CBoolBitSet m_RecvSet;
	BYTE m_byLastTestDataCmd;
	
	//static byte s_keyp2p[16];
	
	BOOL m_bLastPkg;
	short m_shSendSeq;
	_qqt_udp_app_pack m_stLastPkg;

	DWORD m_dwNetSpeed;
	DWORD m_dwNetSpeedLastTime;
};


#endif // !defined(AFX_QQTUDPCLIENTSOCKET_H__492475EC_686D_4C49_A3B4_9EC9A6808AD2__INCLUDED_)
