// QQTUdpProtocolSocket.cpp: implementation of the CQQTUdpProtocolSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QQTUdpProtocolSocket.h"
#include "QQTPPP2ND.h"
#include "DownloadLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef _logstring
#undef _logstring
#endif
#ifdef _logbin
#undef _logbin
#endif
#ifdef _log2win
#undef _log2win
#endif

#ifdef _ENABLE_QQT_LOG_
#define _logstring	LogString
#define _logbin	LogBin
#define _log2win	Log2Win
#else
#define _logstring	/##/
#define _logbin	/##/
#define _log2win	/##/
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CQQTUdpProtocolSocket::cs_EncodeQQTUdpHead(BYTE byOutBuffer[256], int &nOutBufferLen, _qqt_cs_udp_head &udphead, int nBodyLen)
{
	if(nBodyLen < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(udphead.m_byOptLength > MAXOPTLENGTH)
	{
		udphead.m_byOptLength = MAXOPTLENGTH;
	}
	
	_qqt_cs_udp_head_without_option noOptionHead;
	::ZeroMemory(&noOptionHead, sizeof(_qqt_cs_udp_head_without_option));

	noOptionHead.m_nTotalLength = ::htons(nBodyLen + sizeof(_qqt_cs_udp_head_without_option) + udphead.m_byOptLength);
	noOptionHead.m_cVer = udphead.m_cVer;
	noOptionHead.m_iSeq = ::htonl(udphead.m_iSeq);
	noOptionHead.m_nDialogID = ::htons(udphead.m_nDialogID);
	noOptionHead.m_unUin = ::htonl(udphead.m_unUin);
	noOptionHead.m_nTransferCmd = ::htons(udphead.m_nTransferCmd);
	noOptionHead.m_byOptLength = udphead.m_byOptLength;

	memcpy(byOutBuffer, &noOptionHead, sizeof(_qqt_cs_udp_head_without_option));
	if(udphead.m_byOptLength)
	{
		memcpy(&byOutBuffer[sizeof(_qqt_cs_udp_head_without_option)], udphead.m_abyOption, udphead.m_byOptLength);
	}

	nOutBufferLen = udphead.m_byOptLength + sizeof(_qqt_cs_udp_head_without_option);

	return TRUE;
}
BOOL CQQTUdpProtocolSocket::cs_DecodeQQTUdpHead(_qqt_cs_udp_head &udphead, int &nHeadLen, BYTE* pBuffer, int nBufferLen)
{
	if(nBufferLen < sizeof(_qqt_cs_udp_head_without_option))
	{
		return FALSE;
	}

	_qqt_cs_udp_head_without_option noOptionHead;
	memcpy(&noOptionHead, pBuffer, sizeof(_qqt_cs_udp_head_without_option));

	udphead.m_nTotalLength= ::ntohs(noOptionHead.m_nTotalLength);
	udphead.m_cVer = noOptionHead.m_cVer;
	udphead.m_iSeq = ::ntohl(noOptionHead.m_iSeq);
	udphead.m_nDialogID = ::ntohs(noOptionHead.m_nDialogID);
	udphead.m_unUin = ::ntohl(noOptionHead.m_unUin);
	udphead.m_nTransferCmd = ::ntohs(noOptionHead.m_nTransferCmd);
	udphead.m_byOptLength = noOptionHead.m_byOptLength;

	if(udphead.m_byOptLength > MAXOPTLENGTH)
	{
		udphead.m_byOptLength = MAXOPTLENGTH;
	}
	if(udphead.m_byOptLength > 0)
	{
		::memcpy(udphead.m_abyOption, &pBuffer[sizeof(_qqt_cs_udp_head_without_option)], udphead.m_byOptLength);
	}

	nHeadLen = udphead.m_byOptLength + sizeof(_qqt_cs_udp_head_without_option);

	return TRUE;
}
BOOL CQQTUdpProtocolSocket::cc_DecodeQQTUdpHead(_qqt_cc_udp_head &udphead, int &nHeadLen, BYTE* pBuffer, int nBufferLen)
{
	if(nBufferLen < sizeof(_qqt_cc_udp_head))
	{
		return FALSE;
	}
	nHeadLen = sizeof(_qqt_cc_udp_head);
	::memcpy(&udphead, pBuffer, nHeadLen);
	udphead.m_sTotalLength = ::ntohs(udphead.m_sTotalLength);
	udphead.m_uiUin = ::ntohl(udphead.m_uiUin);
	return TRUE;
}

unsigned short CQQTUdpProtocolSocket::CheckSum(const BYTE* pBuffer,int nBufLen)
{
	if(NULL == pBuffer || nBufLen < 1)
	{
		ASSERT(FALSE);
		return 0;
	}

	unsigned long nSum = 0;
	unsigned short *pData = (unsigned short*)pBuffer;
	
	int nLen = nBufLen / 2;
	int nMod = nBufLen % 2;
	
	for(int i = 0; i < nLen; i ++)
	{
		nSum += pData[i];
	}

	unsigned short nShort = 0;
	
	if(1 == nMod)
	{
		nShort = (unsigned short)pBuffer[nBufLen - 1];
		nSum += nShort;
	}
	nSum = (nSum >> 16) + (nSum & 0xffff);
	nSum += (nSum >> 16);
	nShort = ~((unsigned short)nSum);
	return nShort;
}
BOOL CQQTUdpProtocolSocket::cs_CheckQQTUdpPkg(BYTE *pBuffer, int nLen)
{
	if(nLen < sizeof(_qqt_cs_udp_head_without_option))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	_qqt_cs_udp_head_without_option *pHead = (_qqt_cs_udp_head_without_option*)pBuffer;
	unsigned short usCheckSum = ::ntohs(pHead->m_nCheckSum);
	pHead->m_nCheckSum = 0;
	return (CheckSum(pBuffer, nLen) == usCheckSum) ? TRUE : FALSE;
}
BOOL CQQTUdpProtocolSocket::cc_CheckQQTUdpPkg(BYTE *pBuffer, int nLen)
{
	if(nLen < sizeof(_qqt_cc_udp_head))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	_qqt_cc_udp_head *pHead = (_qqt_cc_udp_head*)pBuffer;
	unsigned short usCheckSum = ::ntohs(pHead->m_usCheckSum);
	pHead->m_usCheckSum = 0;
	return (CheckSum(pBuffer, nLen) == usCheckSum) ? TRUE : FALSE;
}
BOOL CQQTUdpProtocolSocket::cs_AddQQTUdpCheckSum(BYTE *pBuffer, int nLen)
{
	if(nLen < sizeof(_qqt_cs_udp_head_without_option))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	_qqt_cs_udp_head_without_option *pHead = (_qqt_cs_udp_head_without_option*)pBuffer;
	pHead->m_nCheckSum = 0;
	unsigned short usCheckSum = CheckSum(pBuffer, nLen);
	pHead->m_nCheckSum = ::htons(usCheckSum);
	return TRUE;
}


CQQTUdpProtocolSocket::CQQTUdpProtocolSocket()
{
	m_UdpSocket.SetUdpSocketCallBack(this);
	m_pQQTUdpProtocolSocketReceiver = NULL;
	m_bIsServerProtocol = TRUE;
	m_byFirstTransmit = 0;
	Initialize();
}

CQQTUdpProtocolSocket::~CQQTUdpProtocolSocket()
{
	_logstring("CQQTUdpProtocolSocket Destructor~~");
}

void CQQTUdpProtocolSocket::SetMyInfo(DWORD dwUin, short sPlayerID)
{
	_logstring("CQQTUdpProtocolSocket::SetMyInfo(DWORD dwUin = %d, short sPlayerID = %d)", dwUin, sPlayerID);
	ASSERT(dwUin > 10000 && sPlayerID > -1);
	m_dwMyUin = dwUin;
	m_sMyPlayerID = sPlayerID;
}
void CQQTUdpProtocolSocket::Initialize()
{
	_logstring("CQQTUdpProtocolSocket::Initialize()");
	m_dwMyUin = 0;
	m_sMyPlayerID = -1;
	m_UdpSocket.CloseSocket();
	m_dwSeq = 0;
	m_byFirstTransmit = 0;
}
BOOL CQQTUdpProtocolSocket::CreateSocket()
{
	if(m_UdpSocket.GetLocalPort())
	{
		return TRUE;
	}
	return m_UdpSocket.BindPort();
}

BOOL CQQTUdpProtocolSocket::cs_UDPLogin()
{
	_logstring("CQQTUdpProtocolSocket::cs_UDPLogin()");
	if(!m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}	
	
	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	_qqt_cs_udp_head udphead;
	udphead.m_cVer = 0x00;
	udphead.m_iSeq = (int)GetSeq();
	udphead.m_nDialogID = m_sMyPlayerID;
	udphead.m_unUin = m_dwMyUin;
	udphead.m_nTransferCmd = UDP_CMD_LOGIN_SERVER;
	udphead.m_nCheckSum = 0;
	udphead.m_byOptLength = 6;
	ULONG ulNLocalIP = ::htonl(m_UdpSocket.GetLocalIP());
	USHORT usNLocalPort = ::htons(m_UdpSocket.GetLocalPort());
	::memcpy(udphead.m_abyOption, &ulNLocalIP, 4);
	::memcpy(&udphead.m_abyOption[4], &usNLocalPort, 2);
	
	BYTE byOutBuffer[256];
	int nOutBufferLen;
	cs_EncodeQQTUdpHead(byOutBuffer, nOutBufferLen, udphead, 0);
	cs_AddQQTUdpCheckSum(byOutBuffer, nOutBufferLen);
	
	m_UdpSocket.SendUdpDataToServer(nOutBufferLen, byOutBuffer);	
	return TRUE;
}
BOOL CQQTUdpProtocolSocket::cs_TransmitUDPAddr(short shPlayerID, int nUin)
{
	_logstring("CQQTUdpProtocolSocket::cs_TransmitUDPAddr(short shPlayerID, int nUin)");
	if(m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	PPPTRACE("TransmitUDPAddr To Uin %d, MyInnerAddr: %d.%d.%d.%d:%d\n",
		nUin, (m_UdpSocket.GetLocalIP()>>24)&0xFF, (m_UdpSocket.GetLocalIP()>>16)&0xFF,
		(m_UdpSocket.GetLocalIP()>>8)&0xFF, (m_UdpSocket.GetLocalIP())&0xFF, m_UdpSocket.GetLocalPort());

	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	_qqt_cs_udp_head udphead;
	udphead.m_cVer = 0x00;
	udphead.m_iSeq = (int)GetSeq();
	udphead.m_nDialogID = m_sMyPlayerID;
	udphead.m_unUin = m_dwMyUin;
	udphead.m_nTransferCmd = UDP_CMD_SHOW_MY_IP_PORT;
	udphead.m_nCheckSum = 0;	
	udphead.m_byOptLength = 13;	
	::ZeroMemory(udphead.m_abyOption, MAXOPTLENGTH);	
	*((unsigned short*)(&udphead.m_abyOption[0])) = ::htons(shPlayerID);//0~1 src player id	
	*((int*)(&udphead.m_abyOption[2])) = ::htonl(nUin);//2~5 src player uin	
	*((unsigned int*)(&udphead.m_abyOption[6])) = ::htonl(m_UdpSocket.GetLocalIP());//6~9 my local ip	
	*((unsigned short*)(&udphead.m_abyOption[10])) = ::htons(m_UdpSocket.GetLocalPort());//10~11 my local port
	*((BYTE*)(&udphead.m_abyOption[12])) = m_byFirstTransmit++;

	BYTE byOutBuffer[256];
	int nOutBufferLen;
	cs_EncodeQQTUdpHead(byOutBuffer, nOutBufferLen, udphead, 0);
	cs_AddQQTUdpCheckSum(byOutBuffer, nOutBufferLen);
	
	m_UdpSocket.SendUdpDataToServer(nOutBufferLen, byOutBuffer);
	
	return TRUE;
}
BOOL CQQTUdpProtocolSocket::cs_Hello()
{
	_logstring("CQQTUdpProtocolSocket::cs_Hello()");
	if(!m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	_qqt_cs_udp_head udphead;
	udphead.m_cVer = 0x00;
	udphead.m_iSeq = GetSeq();
	udphead.m_nDialogID = m_sMyPlayerID;
	udphead.m_unUin = m_dwMyUin;
	udphead.m_nTransferCmd = UDP_CMD_HEART_BEAT;
	udphead.m_nCheckSum = 0;
	udphead.m_byOptLength = 0;
				
	BYTE byOutBuffer[256];
	int nOutBufferLen;
	cs_EncodeQQTUdpHead(byOutBuffer, nOutBufferLen, udphead, 0);
	cs_AddQQTUdpCheckSum(byOutBuffer, nOutBufferLen);
	
	m_UdpSocket.SendUdpDataToServer(nOutBufferLen, byOutBuffer);	
	return TRUE;
}
BOOL CQQTUdpProtocolSocket::cs_SendP2PGameData(const BYTE *pBuffer, int nLen, short shPlayerID[8], unsigned int nUin[8], int nNumberOfPlayer)
{
	_logstring("CQQTUdpProtocolSocket::cs_SendP2PGameData(const BYTE *pBuffer, int nLen, short shPlayerID[8], int nUin[8], int nNumberOfPlayer)");
	if(!m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if(NULL == pBuffer || 0 >= nLen)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(nNumberOfPlayer < 1 || nNumberOfPlayer > 8)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(nLen > MAX_UDP_PACKAGE)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	_qqt_cs_udp_head udphead;
	udphead.m_cVer = 0x00;
	udphead.m_iSeq = GetSeq();
	udphead.m_nDialogID = m_sMyPlayerID;
	udphead.m_unUin = m_dwMyUin;
	udphead.m_nTransferCmd = UDP_CMD_P2P_GAME_DATA;
	udphead.m_nCheckSum = 0;
	udphead.m_byOptLength = 6 * nNumberOfPlayer;
	for(int i = 0; i < nNumberOfPlayer; i ++)
	{
		ASSERT(shPlayerID[i] > -1);
		ASSERT(nUin[i] > 10000);
		*((unsigned short *)(&udphead.m_abyOption[i * 6])) = ::htons(shPlayerID[i]);
		*((unsigned int *)(&udphead.m_abyOption[i * 6 + 2])) = ::htonl(nUin[i]);
	}
	
	BYTE byOutBuffer[MAX_UDP_PACKAGE];
	int nOutBufferLen;
	cs_EncodeQQTUdpHead(byOutBuffer, nOutBufferLen, udphead, nLen);
	if(nOutBufferLen + nLen > MAX_UDP_PACKAGE)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	::memcpy(&byOutBuffer[nOutBufferLen], pBuffer, nLen);
	nOutBufferLen += nLen;
	cs_AddQQTUdpCheckSum(byOutBuffer, nOutBufferLen);
	
	m_UdpSocket.SendUdpDataToServer(nOutBufferLen, byOutBuffer);
	return TRUE;
}
BOOL CQQTUdpProtocolSocket::cc_SendConnectData(byte byCmd)
{
	_logstring("CQQTUdpProtocolSocket::cc_SendConnectData(byte byCmd)");
	if(m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	ASSERT(UDP_CMD_TEST_CONNECTION_NO_RECEIVE == byCmd
		|| UDP_CMD_TEST_CONNECTION_RECEIVED == byCmd);
   
	BYTE buf[64];
	::ZeroMemory(buf, sizeof(buf));
	_qqt_cc_udp_head *pstccUdpHead = (_qqt_cc_udp_head *)buf;
	BYTE *ptr = buf + sizeof(_qqt_cc_udp_head);
	*(ULONG*)ptr = htonl(m_UdpSocket.GetClientIP());
	*(USHORT*)(ptr+4) = htons(m_UdpSocket.GetClientPort());

	int nPkgLen = sizeof(_qqt_cc_udp_head) + 6;

	pstccUdpHead->m_cCmdID = byCmd;
	pstccUdpHead->m_sTotalLength = ::htons(nPkgLen); //只有消息头
	pstccUdpHead->m_uiUin = ::htonl(m_dwMyUin);
	pstccUdpHead->m_uiTime = ::htonl(GetTickCount());
	pstccUdpHead->m_usCheckSum = ::htons(CheckSum(buf, nPkgLen));
	m_UdpSocket.SendUdpDataToClient(nPkgLen, buf);

	PPPTRACE("SendTestData To DestAddr:%d.%d.%d.%d:%d, Cmd %s\n",
		(m_UdpSocket.GetClientIP()>>24)&0xFF, (m_UdpSocket.GetClientIP()>>16)&0xFF,
		(m_UdpSocket.GetClientIP()>>8)&0xFF, (m_UdpSocket.GetClientIP())&0xFF, 
		m_UdpSocket.GetClientPort(), (UDP_CMD_TEST_CONNECTION_NO_RECEIVE == byCmd) ? "Send" : "Reply");

	return TRUE;
}

BOOL CQQTUdpProtocolSocket::cc_Hello()
{
	PPPTRACE("Say Hello to Addr Addr: %d.%d.%d.%d:%d\n",
		(m_UdpSocket.GetClientIP()>>24)&0xFF, (m_UdpSocket.GetClientIP()>>16)&0xFF,
		(m_UdpSocket.GetClientIP()>>8)&0xFF, (m_UdpSocket.GetClientIP())&0xFF, 
		m_UdpSocket.GetClientPort());
	
	if(m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	_qqt_cc_udp_head ccUdpHead;
	::ZeroMemory(&ccUdpHead, sizeof(_qqt_cc_udp_head));
	ccUdpHead.m_cCmdID = UDP_CMD_CLIENT_HELLO;	
	ccUdpHead.m_sTotalLength = ::htons(sizeof(_qqt_cc_udp_head));
	ccUdpHead.m_uiUin = ::htonl(m_dwMyUin);
	ccUdpHead.m_uiTime = ::htonl(GetTickCount());
	ccUdpHead.m_usCheckSum = ::htons(CheckSum((const BYTE*)&ccUdpHead, sizeof(_qqt_cc_udp_head)));
	m_UdpSocket.SendUdpDataToClient(sizeof(_qqt_cc_udp_head), (const byte*)&ccUdpHead);
	return TRUE;
}


BOOL CQQTUdpProtocolSocket::cc_TestNetSpeed()
{
	PPPTRACE("TestNetSpeed to Addr Addr: %d.%d.%d.%d:%d\n",
		(m_UdpSocket.GetClientIP()>>24)&0xFF, (m_UdpSocket.GetClientIP()>>16)&0xFF,
		(m_UdpSocket.GetClientIP()>>8)&0xFF, (m_UdpSocket.GetClientIP())&0xFF, 
		m_UdpSocket.GetClientPort());

	if(m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	_qqt_cc_udp_head ccUdpHead;
	::ZeroMemory(&ccUdpHead, sizeof(_qqt_cc_udp_head));
	ccUdpHead.m_cCmdID = UDP_CMD_TEST_NETSPEED;	
	ccUdpHead.m_sTotalLength = ::htons(sizeof(_qqt_cc_udp_head));
	ccUdpHead.m_uiUin = ::htonl(m_dwMyUin);
	ccUdpHead.m_uiTime = ::htonl(GetTickCount());
	ccUdpHead.m_usCheckSum = ::htons(CheckSum((const BYTE*)&ccUdpHead, sizeof(_qqt_cc_udp_head)));
	m_UdpSocket.SendUdpDataToClient(sizeof(_qqt_cc_udp_head), (const byte*)&ccUdpHead);
	return TRUE;
}

BOOL CQQTUdpProtocolSocket::cc_TestNetSpeedReply(DWORD dwTime)
{
	PPPTRACE("TestNetSpeedReply to Addr Addr: %d.%d.%d.%d:%d\n",
		(m_UdpSocket.GetClientIP()>>24)&0xFF, (m_UdpSocket.GetClientIP()>>16)&0xFF,
		(m_UdpSocket.GetClientIP()>>8)&0xFF, (m_UdpSocket.GetClientIP())&0xFF, 
		m_UdpSocket.GetClientPort());

	if(m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	_qqt_cc_udp_head ccUdpHead;
	::ZeroMemory(&ccUdpHead, sizeof(_qqt_cc_udp_head));
	ccUdpHead.m_cCmdID = UDP_CMD_TEST_NETSPEED_REPLY;	
	ccUdpHead.m_sTotalLength = ::htons(sizeof(_qqt_cc_udp_head));
	ccUdpHead.m_uiUin = ::htonl(m_dwMyUin);
	ccUdpHead.m_uiTime = htonl(dwTime);
	ccUdpHead.m_usCheckSum = ::htons(CheckSum((const BYTE*)&ccUdpHead, sizeof(_qqt_cc_udp_head)));
	m_UdpSocket.SendUdpDataToClient(sizeof(_qqt_cc_udp_head), (const byte*)&ccUdpHead);
	return TRUE;
}

BOOL CQQTUdpProtocolSocket::cc_SendP2PGameData(const BYTE *pBuffer, int iLen)
{
	LOG_TIME(a1, "cc_SendP2PGameData", 30);
	_logstring("CQQTUdpProtocolSocket::cc_SendP2PGameData(const BYTE *pBuffer, int iLen)");
	if(m_bIsServerProtocol)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(!CreateSocket())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if(NULL == pBuffer || iLen < 1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	ASSERT(m_sMyPlayerID > -1);
	ASSERT(m_dwMyUin > 10000);
	int iBufferLen = sizeof(_qqt_cc_udp_head) + iLen;
	if(iBufferLen > MAX_UDP_PACKAGE)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	byte byBuffer[MAX_UDP_PACKAGE];
	_qqt_cc_udp_head ccUdpHead;
	::ZeroMemory(&ccUdpHead, sizeof(_qqt_cc_udp_head));
	ccUdpHead.m_cCmdID = UDP_CMD_CLIENT_P2P_DATA;	
	ccUdpHead.m_sTotalLength = ::htons(sizeof(_qqt_cc_udp_head) + iLen);
	ccUdpHead.m_uiUin = ::htonl(m_dwMyUin);
	ccUdpHead.m_uiTime = ::htonl(GetTickCount());
	::memcpy(byBuffer, &ccUdpHead, sizeof(_qqt_cc_udp_head));
	::memcpy(&byBuffer[sizeof(_qqt_cc_udp_head)], pBuffer, iLen);
	_qqt_cc_udp_head *pHead = (_qqt_cc_udp_head*)byBuffer;
	pHead->m_usCheckSum = ::htons(CheckSum(byBuffer, iBufferLen));

	SendDataLog(iBufferLen);
	m_UdpSocket.SendUdpDataToClient(iBufferLen, byBuffer);
	return TRUE;
}
void CQQTUdpProtocolSocket::OnReceive(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen)
{
	_logstring("CQQTUdpProtocolSocket::OnReceive(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen)");
	if(m_bIsServerProtocol)
	{
		OnReceiveServerData(ulIP, usPort, pBuffer, nBufferLen);
	}
	else
	{
		OnReceiveClientData(ulIP, usPort, pBuffer, nBufferLen);
	}
}
void CQQTUdpProtocolSocket::OnReceiveServerData(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen)
{
	LOG_TIME(a1, "CQQTUdpProtocolSocket::OnReceiveServerData", 30);
	_logstring("CQQTUdpProtocolSocket::OnReceiveServerData(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen)");
	if(!cs_CheckQQTUdpPkg(pBuffer, nBufferLen))
	{
		_logstring("	ERROR, check sum wrong!");
		ASSERT(FALSE);
		return;
	}	
	
	_qqt_cs_udp_head udphead;
	int nHeadLen = 0;
	if(!cs_DecodeQQTUdpHead(udphead, nHeadLen, pBuffer, nBufferLen))
	{
		_logstring("	ERROR, failed ot decode cs udp head!");
		ASSERT(FALSE);
		return;
	}
	if(nBufferLen != udphead.m_nTotalLength)
	{
		_logstring("	ERROR, data with error length");
		ASSERT(FALSE);
		return;
	}

	BYTE byBody[MAX_UDP_PACKAGE];
	int nBodyLen = nBufferLen - nHeadLen;	
	if(nBodyLen < 0 || nBodyLen > MAX_UDP_PACKAGE)
	{
		_logstring("	ERROR, udp body length is invailable.");
		ASSERT(FALSE);
		return;
	}
	if(nBodyLen)
	{
		::memcpy(byBody, &pBuffer[nHeadLen], nBodyLen);
	}

	_logstring("	pass normal data check");
	
	switch(udphead.m_nTransferCmd)
	{
	case UDP_CMD_LOGIN_SERVER:
		_logstring("	UDP_CMD_LOGIN_SERVER, my outer ip = %d.%d.%d.%d", ::ntohl(*((unsigned int *)(&udphead.m_abyOption[0]))) >> 24, (::ntohl(*((unsigned int *)(&udphead.m_abyOption[0]))) >> 16) & 0xff, (::ntohl(*((unsigned int *)(&udphead.m_abyOption[0]))) >> 8) & 0xff, ::ntohl(*((unsigned int *)(&udphead.m_abyOption[0]))) &0xff);
		if(udphead.m_byOptLength < 4)
		{
			ASSERT(FALSE);				
			return;
		}
		if(m_pQQTUdpProtocolSocketReceiver)
		{
			m_pQQTUdpProtocolSocketReceiver->OnLoginResponse(ulIP, ::ntohl(::ntohl(*((unsigned int *)(&udphead.m_abyOption[0])))));
		}
		break;
	case UDP_CMD_P2P_GAME_DATA:
		_logstring("	UDP_CMD_P2P_GAME_DATA");
		ASSERT(nBodyLen);
		if(m_pQQTUdpProtocolSocketReceiver && nBodyLen)
		{
			PPPTRACE("Recv GameData from Server, Uin %d, Addr: %d.%d.%d.%d:%d\n",
				udphead.m_unUin, (ulIP>>24)&0xFF, (ulIP>>16)&0xFF, (ulIP>>8)&0xFF,
				(ulIP)&0xFF, usPort);

			m_pQQTUdpProtocolSocketReceiver->OnRecvP2PGameData(ulIP, udphead.m_nDialogID, udphead.m_unUin, byBody, nBodyLen);
		}		
		break;

	// 其他玩家为和此玩家建立连接跟server开通了一个新的UDP连接
	case UDP_CMD_RECV_OTHER_IP_PORT:
		if(m_pQQTUdpProtocolSocketReceiver)
		{
			m_pQQTUdpProtocolSocketReceiver->OnGetPlayerInfoFromSvr(
				ulIP,
				::ntohs(*((unsigned short*)(&udphead.m_abyOption[0]))),
				::ntohl(*((unsigned int*)(&udphead.m_abyOption[2]))),
				::ntohl(::ntohl(*((unsigned int*)(&udphead.m_abyOption[6])))),//dest player outer ip, in net seq
				::ntohs(::ntohs(*((unsigned short*)(&udphead.m_abyOption[10])))),//dest player outer port, in net seq
				::ntohl(*((unsigned int*)(&udphead.m_abyOption[12]))),
				::ntohs(*((unsigned short*)(&udphead.m_abyOption[16])))
			);
		}
		break;
	case UDP_CMD_HEART_BEAT:
		break;
	default:
		_logstring("	ERROR, unknown cmd!");
		ASSERT(FALSE);		
		break;
	}
}

void CQQTUdpProtocolSocket::OnReceiveClientData(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen)
{
	_logstring("CQQTUdpProtocolSocket::OnReceiveClientData(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen)");

	RecvDataLog(nBufferLen);  // LOG FOR NET STATISTIC
	
	// 检查CheckSum
	if(!cc_CheckQQTUdpPkg(pBuffer, nBufferLen))
	{
		_logstring("	ERROR, failed to check cc sum!");
		ASSERT(FALSE);
		return;
	}
	_qqt_cc_udp_head udphead;
	int nHeadLen = 0;
	if(!cc_DecodeQQTUdpHead(udphead, nHeadLen, pBuffer, nBufferLen))
	{
		_logstring("	ERROR, failed to decode cc head!");
		ASSERT(FALSE);
		return;
	}
	if(nBufferLen != udphead.m_sTotalLength)
	{
		_logstring("	ERROR, errror length!");
		ASSERT(FALSE);
		return;
	}

	BYTE byBody[MAX_UDP_PACKAGE];
	int nBodyLen = nBufferLen - nHeadLen;	
	if(nBodyLen < 0 || nBodyLen > MAX_UDP_PACKAGE)
	{
		_logstring("	ERROR, cc package length is wrong!");
		ASSERT(FALSE);
		return;
	}
	if(nBodyLen)
	{
		::memcpy(byBody, &pBuffer[nHeadLen], nBodyLen);
	}

	_logstring("	cc package check ok!");

	switch(udphead.m_cCmdID)
	{
	case UDP_CMD_TEST_CONNECTION_NO_RECEIVE:
	case UDP_CMD_TEST_CONNECTION_RECEIVED:
//	case UDP_CMD_UDP_BIDIRECTIONAL_OK:
#ifdef _ENABLE_QQT_LOG_
		if(UDP_CMD_TEST_CONNECTION_NO_RECEIVE == udphead.m_cCmdID)
			_logstring("		UDP_CMD_TEST_CONNECTION_NO_RECEIVE");
		if(UDP_CMD_TEST_CONNECTION_RECEIVED == udphead.m_cCmdID)
			_logstring("		UDP_CMD_TEST_CONNECTION_RECEIVED");
//		if(UDP_CMD_UDP_BIDIRECTIONAL_OK == udphead.m_cCmdID)
//			_logstring("		UDP_CMD_UDP_BIDIRECTIONAL_OK");
#endif
		if(m_pQQTUdpProtocolSocketReceiver)
		{
			ULONG ulTestMyIP = ntohl(*(ULONG*)byBody);
			USHORT usTestMyPort = ntohs(*(USHORT*)(byBody+4));

			m_pQQTUdpProtocolSocketReceiver->OnRecvP2PTestConnectData(
				ulIP, usPort, ulTestMyIP, usTestMyPort,
				udphead.m_cCmdID
			);
		}
		break;	
	case UDP_CMD_TEST_NETSPEED:
		if ( m_pQQTUdpProtocolSocketReceiver )
		{
			m_pQQTUdpProtocolSocketReceiver->OnRecvP2PTestNetSpeed(&udphead);
		}
		break;
	case UDP_CMD_TEST_NETSPEED_REPLY:
		if ( m_pQQTUdpProtocolSocketReceiver )
		{
			m_pQQTUdpProtocolSocketReceiver->OnRecvP2PTestNetSpeedReply(&udphead);
		}
		break;

	case UDP_CMD_CLIENT_P2P_DATA:
		_logstring("		UDP_CMD_CLIENT_P2P_DATA");
		if(m_pQQTUdpProtocolSocketReceiver)
		{
			PPPTRACE("Recv GameData from Client, Uin %d, Addr: %d.%d.%d.%d:%d\n",
				udphead.m_uiUin, (ulIP>>24)&0xFF, (ulIP>>16)&0xFF, (ulIP>>8)&0xFF,
				(ulIP)&0xFF, usPort);

			m_pQQTUdpProtocolSocketReceiver->OnRecvP2PGameData(ulIP, -1, udphead.m_uiUin, byBody, nBodyLen);
		}
		break;
	case UDP_CMD_CLIENT_HELLO:
		PPPTRACE("Recv Hello From Uin %d, Addr: %d.%d.%d.%d:%d\n",
				udphead.m_uiUin, (ulIP>>24)&0xFF, (ulIP>>16)&0xFF, (ulIP>>8)&0xFF,
				(ulIP)&0xFF, usPort);
		break;
	default:
		ASSERT(FALSE);
		break;;
	}
}
void CQQTUdpProtocolSocket::OnClose()
{
	_logstring("CQQTUdpProtocolSocket::OnClose()");
	if(m_pQQTUdpProtocolSocketReceiver)
	{
		m_pQQTUdpProtocolSocketReceiver->OnClose();
	}
}
#ifdef _ENABLE_QQT_LOG_
void CQQTUdpProtocolSocket::LogString(const char *pFormat, ...)
{
	if(m_pQQTUdpProtocolSocketReceiver)
	{
		CString str;
		va_list argList;
		va_start(argList, pFormat);
		str.FormatV(pFormat, argList);
		va_end(argList);
		m_pQQTUdpProtocolSocketReceiver->LogString("%s", str);
	}
}
void CQQTUdpProtocolSocket::LogBin(int nLen, const char *pBuf)
{
	if(m_pQQTUdpProtocolSocketReceiver)
	{
		m_pQQTUdpProtocolSocketReceiver->LogBin(nLen, pBuf);
	}
}
void CQQTUdpProtocolSocket::Log2Win(char* pFormat, ...)
{
	if(m_pQQTUdpProtocolSocketReceiver)
	{
		CString str;
		va_list argList;
		va_start(argList, pFormat);
		str.FormatV(pFormat, argList);
		va_end(argList);
		m_pQQTUdpProtocolSocketReceiver->Log2Win("%s", str);
	}
}
#endif