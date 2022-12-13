// QQTUdpProtocolSocket.h: interface for the CQQTUdpProtocolSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QQTUDPPROTOCOLSOCKET_H__67C3DC17_FEE3_4991_B116_03986E39F4AE__INCLUDED_)
#define AFX_QQTUDPPROTOCOLSOCKET_H__67C3DC17_FEE3_4991_B116_03986E39F4AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UdpSocket.h"


//udp cs cmds
#define UDP_CMD_LOGIN_SERVER			0x01
#define UDP_CMD_P2P_GAME_DATA			0x02
#define UDP_CMD_SHOW_MY_IP_PORT			0x03
#define UDP_CMD_RECV_OTHER_IP_PORT	0x04
#define UDP_CMD_HEART_BEAT				0x05

//udp cc cmds
#define UDP_CMD_TEST_CONNECTION_NO_RECEIVE			0x20
#define UDP_CMD_TEST_CONNECTION_RECEIVED			0x21
#define UDP_CMD_UDP_BIDIRECTIONAL_OK				0x22
#define UDP_CMD_CLIENT_HELLO						0x25
#define UDP_CMD_CLIENT_P2P_DATA						0x26

#define UDP_CMD_TEST_NETSPEED						0x27
#define UDP_CMD_TEST_NETSPEED_REPLY					0x28

#pragma pack(push, 1)
typedef struct _qqt_cc_udp_head
{
	short m_sTotalLength;
	char m_cCmdID;
	unsigned short m_usCheckSum;
	unsigned int m_uiUin;
	unsigned int m_uiTime;
} QQT_CC_UDP_HEAD;
#pragma pack(pop)

class CQQTUdpProtocolSocket : public CUdpSocket::IUdpSocketCallBack
{
public:

	class IQQTUdpProtocolSocketReceiver
	{
	public:
		virtual void OnClose() PURE;
		virtual void OnLoginResponse(ULONG ulSvrIP, ULONG ulOuterIP) PURE;
		virtual void OnGetPlayerInfoFromSvr(ULONG ulSvrIP, short shSrcPlayerID, int nSrcUin, ULONG ulSrcOuterIP, USHORT usSrcOuterPort, ULONG ulSrcInerIP, USHORT usSrcInnerPort) PURE;
		virtual void OnRecvP2PGameData(ULONG ulSrcIP, short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen) PURE;		
		virtual void OnRecvP2PTestConnectData(ULONG ulIP, USHORT usPort, ULONG ulTestMyIP, USHORT usTestMyPort, byte byCmd) PURE;
		//virtual void OnClientHello(short shPlayerID, UINT uiUin) PURE;
		virtual void OnRecvP2PTestNetSpeed(const _qqt_cc_udp_head*){};
		virtual void OnRecvP2PTestNetSpeedReply(const _qqt_cc_udp_head*){};
#ifdef _ENABLE_QQT_LOG_
		virtual void LogString(const char *pFormat, ...) PURE;
		virtual void LogBin(int nLen, const char *pBuf) PURE;
		virtual void Log2Win(char* pFormat, ...) PURE;
#endif
	};
protected:
	#define MAXOPTLENGTH 128	
	typedef struct _qqt_cs_udp_head
	{
		short	m_nTotalLength;
		BYTE	m_cVer;
		int		m_iSeq;
		short	m_nDialogID;
		unsigned int m_unUin;
		short	m_nTransferCmd;
		unsigned short m_nCheckSum;
		BYTE	m_byOptLength;
		BYTE	m_abyOption[MAXOPTLENGTH];
	} QQT_CS_UDP_HEAD;
	
	#pragma pack(push, 1)
	typedef struct _qqt_cs_udp_head_without_option
	{
		short	m_nTotalLength;
		BYTE	m_cVer;
		int		m_iSeq;
		short	m_nDialogID;
		unsigned int m_unUin;
		short	m_nTransferCmd;
		unsigned short m_nCheckSum;
		BYTE	m_byOptLength;		
	} QQT_CS_UDP_HEAD_WITHOUT_OPTION;	
	#pragma pack(pop)

	DWORD m_dwSeq;
	DWORD GetSeq()								{	return m_dwSeq ++;					}
	unsigned short CheckSum(const BYTE* pBuffer,int nBufLen);

	BOOL cs_EncodeQQTUdpHead(BYTE byOutBuffer[256], int &nOutBufferLen, _qqt_cs_udp_head &udphead, int nBodyLen);
	BOOL cs_DecodeQQTUdpHead(_qqt_cs_udp_head &udphead, int &nHeadLen, BYTE* pBuffer, int nBufferLen);	
	BOOL cs_AddQQTUdpCheckSum(BYTE *pBuffer, int nLen);
	BOOL cs_CheckQQTUdpPkg(BYTE *pBuffer, int nLen);

	BOOL cc_CheckQQTUdpPkg(BYTE *pBuffer, int nLen);
	BOOL cc_DecodeQQTUdpHead(_qqt_cc_udp_head &udphead, int &nHeadLen, BYTE* pBuffer, int nBufferLen);
	/*
	BOOL cc_EncodeQQTUdpHead(BYTE byOutBuffer[256], int &nOutBufferLen, _qqt_cc_udp_head &udphead, int nBodyLen);	
	BOOL cc_AddQQTUdpCheckSum(BYTE *pBuffer, int nLen);	
	*/
public:
	CQQTUdpProtocolSocket();
	virtual ~CQQTUdpProtocolSocket();

	//////////////////////////////////////////////////////////////////////////	
	//get socket property
	ULONG GetLocalIP()							{	return m_UdpSocket.GetLocalIP();	}
	USHORT GetPort() const						{	return m_UdpSocket.GetLocalPort();	}
	ULONG GetClientIP() const						{	return m_UdpSocket.GetClientIP();		}
	USHORT GetClientPort() const					{	return m_UdpSocket.GetClientPort();	}
	ULONG GetServerIP() const						{	return m_UdpSocket.GetServerIP();		}
	USHORT GetServerPort() const					{	return m_UdpSocket.GetServerPort();	}

	//////////////////////////////////////////////////////////////////////////	
	//set socket property
	void SetServerAddr(ULONG ulServerIP, USHORT ulServerPort)	{m_UdpSocket.SetServerAddr(ulServerIP, ulServerPort);}
	void SetCliendAddr(ULONG ulClientIP, USHORT ulClientPort)	{m_UdpSocket.SetClientAddr(ulClientIP, ulClientPort);}

	//////////////////////////////////////////////////////////////////////////	
	//set protocol info
	void SetMyInfo(DWORD dwUin, short sPlayerID);
	void SetQQTUdpProtocolSocketReceiver(IQQTUdpProtocolSocketReceiver *pReceiver) {m_pQQTUdpProtocolSocketReceiver = pReceiver;}
	void SetProtoolType(BOOL bIsServer)			{	m_bIsServerProtocol = bIsServer;	}

	//////////////////////////////////////////////////////////////////////////	
	//protocol operation
	void Initialize();	
	BOOL CreateSocket();

	//////////////////////////////////////////////////////////////////////////
	// cs protocols
	BOOL cs_UDPLogin();//login
	BOOL cs_TransmitUDPAddr(short shPlayerID, int nUin);//请求转发udp端口
	BOOL cs_Hello();//hello
	BOOL cs_SendP2PGameData(const BYTE *pBuffer, int nLen, short shPlayerID[8], unsigned int nUin[8], int nNumberOfPlayer);//send p2p game data

	//////////////////////////////////////////////////////////////////////////
	// cc protocols
	BOOL cc_SendConnectData(byte byCmd);//Send Connect Data
	BOOL cc_Hello();//hello
	BOOL cc_TestNetSpeed();
	BOOL cc_TestNetSpeedReply(DWORD dwTime);
	BOOL cc_SendP2PGameData(const BYTE *pBuffer, int iLen);//send p2p game data
protected:
	//implementation of CUdpSocket::IUdpSocketCallBack
	virtual void OnReceive(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen);
	virtual void OnClose();
#ifdef _ENABLE_QQT_LOG_
	virtual void LogString(const char *pFormat, ...);
	virtual void LogBin(int nLen, const char *pBuf);
	virtual void Log2Win(char* pFormat, ...);
#endif

	void OnReceiveServerData(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen);
	void OnReceiveClientData(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen);
private:
	CUdpSocket m_UdpSocket;
	IQQTUdpProtocolSocketReceiver *m_pQQTUdpProtocolSocketReceiver;
	BOOL m_bIsServerProtocol;

	//my info
	DWORD m_dwMyUin;
	short m_sMyPlayerID;
	BYTE	m_byFirstTransmit;
};

#endif // !defined(AFX_QQTUDPPROTOCOLSOCKET_H__67C3DC17_FEE3_4991_B116_03986E39F4AE__INCLUDED_)
