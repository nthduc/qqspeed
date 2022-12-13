// QQTUdpClientSocket.cpp: implementation of the CQQTUdpClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QQTUdpClientSocket.h"
#include "IQQTPPP.h"
#include "QQTPPP2ND.h"
#include "Network/QQCrypt.h"
#include "common.h"
#include "zlib.h"

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

#define _ENABLE_SET_TRANSMIT_TYPE_
#ifdef _ENABLE_SET_TRANSMIT_TYPE_
enum
{
	_transmit_type_udp = 0,
	_transmit_type_tcp
};
int g_iTransmitType = _transmit_type_udp;
#endif/_ENABLE_SET_TRANSMIT_TYPE_

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

byte s_keyp2p[16] = 
{
	//qqt is the best!
	'q', 'q', 't', ' ',	'i', 's', ' ', 't',	'h', 'e', ' ', 'b',	'e', 's', 't', '!'
};

DWORD _p2p_GetTickCount()
{
	static DWORD dwLastTickCount = ::GetTickCount();
	DWORD dwReturnTickCount = ::GetTickCount();
	if(dwReturnTickCount == dwLastTickCount)
	{
		dwReturnTickCount ++;
	}

	return dwLastTickCount = dwReturnTickCount;
}

CQQTUdpClientSocket::CQQTUdpClientSocket()
{
#ifdef _ENABLE_QQT_LOG_
	_snprintf(m_szLogPath, MAX_PATH, "client_socket_%8X", this);
#endif

	m_ProtocolSocket.SetProtoolType(FALSE);
	m_ProtocolSocket.SetQQTUdpProtocolSocketReceiver(this);
	m_pUdpClientCallBack = NULL;
	m_pUdpSvrSocket = NULL;
	m_iClientTag = -1;
	Initialize();

	m_iTcpSendTImer = TCP_CHECK_RESEND_TIMER;
	m_iHelloTimer = P2P_HELLO_TIMER;

	#ifdef _ENABLE_SET_TRANSMIT_TYPE_
	static BOOL s_bLoad = TRUE;
	if(s_bLoad)
	{
		s_bLoad = FALSE;
		CString str = ::GetRootPath() + "config.ini";
		g_iTransmitType = ::GetPrivateProfileInt("QQTPPP", "TransmitType", 0, str);
	}
	#endif//_ENABLE_SET_TRANSMIT_TYPE_
}

CQQTUdpClientSocket::~CQQTUdpClientSocket()
{
	_logstring("CQQTUdpClientSocket::~CQQTUdpClientSocket()");
}
void CQQTUdpClientSocket::Initialize()
{
	_logstring("void CQQTUdpClientSocket::Initialize()");
	m_ProtocolSocket.Initialize();
	m_iGameStatus = PPPSetGameStatus_NotInGame;
	m_ulSvrIP = 0;
	m_usSvrPort = 0;
	m_iConnectState = _connection_state_no_start;
	m_dwDestUin = 0;
	m_sDestPlayerID = -1;
	m_iTransmitTimer = -1;
	m_iTestConnectTimer = -1;
	m_iTestNetSpeedTimer = 3;
	m_iHelloTimer = P2P_HELLO_TIMER;
	m_ulDestPlayerOuterIP = 0;
	m_usDesplaterOuterPort = 0;
	m_ulDestPlayerInnerIP = 0;
	m_usDestPlayerInnerPort = 0;
	m_sSequence = 0;
	m_byLastTestDataCmd = 0;
	
	m_bLastPkg = FALSE;
	m_shSendSeq = 0;
	memset(&m_stLastPkg, '\0', sizeof(m_stLastPkg));

	m_bFirstSendTcpRecv = TRUE;
	m_arPtrMsgBuffer.FreeAll();
	m_RecvSet.Clear();

	m_dwNetSpeed = 0xffffffff;
	m_dwNetSpeedLastTime = 0;
}
void CQQTUdpClientSocket::StartConnectPlayer()
{
	PPPTRACE("Start ConnectPlayer\n");
	ASSERT(m_iConnectState == _connection_state_no_start);
	ASSERT(m_ulSvrIP && m_usSvrPort);
	ASSERT(m_dwDestUin > 10000 && m_sDestPlayerID > -1);
	Transmit();
}
void CQQTUdpClientSocket::Transmit()
{
	PPPTRACE("Start Transmit\n");
	m_ProtocolSocket.SetServerAddr(m_ulSvrIP, m_usSvrPort);
	m_ProtocolSocket.cs_TransmitUDPAddr(m_sDestPlayerID, m_dwDestUin);
	m_iConnectState = _connection_state_transmit;
	m_iTransmitTimer = RE_TRANSMIT_TIME_LENGTH;
}
void CQQTUdpClientSocket::OnTimer()
{
	if(_connection_state_transmit == m_iConnectState)
	{
		if(m_iTransmitTimer > 0)
		{
			if(--m_iTransmitTimer == 0)
			{
				Transmit();
				return;
			}
		}
	}
	else if(_connection_state_testing == m_iConnectState)
	{
		if(m_iTestConnectTimer > 0)
		{
			if(-- m_iTestConnectTimer == 0)
			{
				SendTestData(m_byLastTestDataCmd);
				return;
			}
		}
	}
	else if(_connection_state_can_pass == m_iConnectState)
	{
		if(--m_iHelloTimer <= 0)
		{
			m_ProtocolSocket.cc_Hello();
			m_iHelloTimer = P2P_HELLO_TIMER;
		}
		//if ( --m_iTestNetSpeedTimer >= 0 )
		{
			m_ProtocolSocket.cc_TestNetSpeed();
		}
	}
}

// 为某玩家建立的和server的UDP连接成功 (每次与server建立1 + n个UDP连接, 1 个是进入小区时的UdpServerSocket, n 个是)
void CQQTUdpClientSocket::SetTransmitOK()
{
	PPPTRACE("SetTransmitOK Uin %d\n", m_dwDestUin);

	if(_connection_state_testing == m_iConnectState || _connection_state_can_pass == m_iConnectState)
	{
		//在收到TransmitOK之前已经收到了对方的探测包并开始探测，或已经建立好链接
		return;
	}

	m_iTransmitTimer = -1;
	m_iConnectState = _connection_state_transmit_ok;
	if(m_ulDestPlayerOuterIP)
	{
		SendTestData(UDP_CMD_TEST_CONNECTION_NO_RECEIVE);
	}
}
void CQQTUdpClientSocket::SendTestData(BYTE byCmd)
{
	if(!m_ulDestPlayerOuterIP)
	{
		PPPTRACE("SendTestData Ignored, DestPlayerOuterIP is not Set\n");
		return;
	}
	
	if(_connection_state_can_pass != m_iConnectState)
	{
		m_iConnectState = _connection_state_testing;
		m_iTestConnectTimer = RE_SEND_CONNECT_DATA_LENGTH;
		m_byLastTestDataCmd = byCmd;
	}
	
	m_ProtocolSocket.cc_SendConnectData(byCmd);
}

void CQQTUdpClientSocket::SetDestPlayerAddr(ULONG ulOuterIP, USHORT usOuterPort, ULONG ulInnerIP, USHORT usInnerPort)
{	
	PPPTRACE("SetDestPlayerAddr: Uin %d, OuterAddr %d.%d.%d.%d:%d, InnerAddr %d.%d.%d.%d:%d\n",
		m_dwDestUin, (ulOuterIP>>24)&0xFF, (ulOuterIP>>16)&0xFF, (ulOuterIP>>8)&0xFF, (ulOuterIP)&0xFF, usOuterPort,
		(ulInnerIP>>24)&0xFF, (ulInnerIP>>16)&0xFF, (ulInnerIP>>8)&0xFF, (ulInnerIP)&0xFF, usInnerPort);

	m_ulDestPlayerOuterIP = ulOuterIP;
	m_usDesplaterOuterPort = usOuterPort;
	m_ulDestPlayerInnerIP = ulInnerIP;
	m_usDestPlayerInnerPort = usInnerPort;

	ULONG ulMyOuterIP = m_pUdpClientCallBack ? m_pUdpClientCallBack->GetMyOutterIP() : 0;
	ASSERT(ulMyOuterIP);

	BOOL bMyNetLan = (m_ProtocolSocket.GetLocalIP() == ulMyOuterIP) ? FALSE : TRUE;
	BOOL bDestNetLan = (m_ulDestPlayerOuterIP == m_ulDestPlayerInnerIP) ? FALSE : TRUE;
		
	//简化这里的逻辑：当且仅当lan_2_same_lan使用内网ip
	if(bDestNetLan && bMyNetLan && ulMyOuterIP == m_ulDestPlayerOuterIP)
	{
		m_ProtocolSocket.SetCliendAddr(m_ulDestPlayerInnerIP, m_usDestPlayerInnerPort);
	}
	else
	{
		m_ProtocolSocket.SetCliendAddr(m_ulDestPlayerOuterIP, m_usDesplaterOuterPort);
	}

	if(_connection_state_transmit_ok == m_iConnectState)
	{
		SendTestData(UDP_CMD_TEST_CONNECTION_NO_RECEIVE);
	}
}
void CQQTUdpClientSocket::SetDestPlayerP2PAddr(ULONG ulIP, USHORT usPort)
{
	ULONG ulOuterIP = m_pUdpClientCallBack ? m_pUdpClientCallBack->GetMyOutterIP() : 0;
	ASSERT(ulOuterIP);

	BOOL bMyNetLan = (m_ProtocolSocket.GetLocalIP() == ulOuterIP) ? FALSE : TRUE;
	BOOL bDestNetLan = (m_ulDestPlayerOuterIP == m_ulDestPlayerInnerIP) ? FALSE : TRUE;
		
	if(bDestNetLan && bMyNetLan && ulOuterIP == m_ulDestPlayerOuterIP)
	{
		m_ulDestPlayerInnerIP = ulIP;
		m_usDestPlayerInnerPort = usPort;
	}
	else
	{
		m_ulDestPlayerOuterIP = ulIP;
		m_usDesplaterOuterPort = usPort;
	}

	PPPTRACE("SetDestPlayerP2PAddr: Uin %d, P2PAddr %d.%d.%d.%d:%d\n",
		m_dwDestUin, (ulIP>>24)&0xFF, (ulIP>>16)&0xFF, (ulIP>>8)&0xFF, (ulIP)&0xFF, usPort);

	m_ProtocolSocket.SetCliendAddr(ulIP, usPort);
}

void CQQTUdpClientSocket::OnRecvP2PTestConnectData(ULONG ulIP, USHORT usPort, ULONG ulTestMyIP, USHORT usTestMyPort, byte byCmd)
{
	PPPTRACE("RecvTestData From Addr: %d.%d.%d.%d:%d, TestMyAddr: %d.%d.%d.%d:%d, cmd=%s\n",
		ulIP >> 24, (ulIP >> 16) & 0xff, (ulIP >> 8) & 0xff, ulIP & 0xff, usPort,
		ulTestMyIP >> 24, (ulTestMyIP >> 16) & 0xff, (ulTestMyIP >> 8) & 0xff, ulTestMyIP & 0xff, usTestMyPort,
		(UDP_CMD_TEST_CONNECTION_NO_RECEIVE == byCmd) ? "Send" : "Reply");

	if(UDP_CMD_TEST_CONNECTION_NO_RECEIVE == byCmd)
	{
		if(_connection_state_can_pass != m_iConnectState)
		{
			SetDestPlayerP2PAddr(ulIP, usPort);
		}

		SendTestData(UDP_CMD_TEST_CONNECTION_RECEIVED);
	}
	
	m_pUdpClientCallBack->OnRecvClientTry2ConnectData(m_sDestPlayerID, m_dwDestUin, ulTestMyIP, usTestMyPort);
}

void CQQTUdpClientSocket::OnRecvP2PTestNetSpeed(const _qqt_cc_udp_head* phead)
{
	if ( phead )
		m_ProtocolSocket.cc_TestNetSpeedReply(ntohl(phead->m_uiTime));
}

void CQQTUdpClientSocket::OnRecvP2PTestNetSpeedReply(const _qqt_cc_udp_head* phead)
{
	if ( phead )
	{
		DWORD t = GetTickCount();
		m_dwNetSpeedLastTime = t;
		t = ( t - ntohl(phead->m_uiTime) ) / 2;
		if ( m_dwNetSpeed == 0xffffffff )
			m_dwNetSpeed = t;
		else
			m_dwNetSpeed = (m_dwNetSpeed + t) / 2;
		LogText("NetSpeed to %d is %d tick, av=%d tick\n", phead->m_uiUin, t, m_dwNetSpeed);
	}
}

void CQQTUdpClientSocket::SetCanPass(ULONG ulIP, USHORT usPort)
{
	if(_connection_state_testing == m_iConnectState)
	{
		PPPTRACE("SetCanPass UIN %d, OuterAddr: %d.%d.%d.%d:%d, InnerAddr:%d.%d.%d.%d:%d\n",
			m_dwDestUin, (m_ulDestPlayerOuterIP>>24)&0xFF, (m_ulDestPlayerOuterIP>>16)&0xFF,
			(m_ulDestPlayerOuterIP>>8)&0xFF, (m_ulDestPlayerOuterIP)&0xFF, m_usDesplaterOuterPort,
			(m_ulDestPlayerInnerIP>>24)&0XFF, (m_ulDestPlayerInnerIP>>16)&0XFF,
			(m_ulDestPlayerInnerIP>>8)&0XFF, (m_ulDestPlayerInnerIP&0XFF), m_usDestPlayerInnerPort);

		SetDestPlayerP2PAddr(ulIP, usPort);

		m_iTestConnectTimer = -1;
		m_iConnectState = _connection_state_can_pass;
	}
}

#ifdef _ENABLE_QQT_LOG_
void CQQTUdpClientSocket::LogString(const char *pFormat, ...)
{
	CString str;
	va_list argList;
	va_start(argList, pFormat);
	str.FormatV(pFormat, argList);
	va_end(argList);
	LOGSTRING(FALSE, m_szLogPath, "%s", str);
}
void CQQTUdpClientSocket::LogBin(int nLen, const char *pBuf)
{
	LOGBIN(TRUE, m_szLogPath, nLen, pBuf);
}
void CQQTUdpClientSocket::Log2Win(char* pFormat, ...)
{
	CString str;
	va_list argList;
	va_start(argList, pFormat);
	str.FormatV(pFormat, argList);
	va_end(argList);
	DBWLOG("%s", str);
}
#endif

#define MAX_UDP_PACKS_LENGTH		900

void CQQTUdpClientSocket::SendTcpDataToServer(UINT uiLen, BYTE *pBuf, short *ashPlayerID, UINT *auiUin, int nPlayerCount)
{
	if(NULL == pBuf || uiLen > BIG_BUFFER_SIZE || nPlayerCount > 8)
	{
		ASSERT(FALSE);
		return;
	}
	
	_qqt_udp_app_head stHead;
	_qqt_udp_app_pack stPack;
	_qqt_udp_app_pack *pstPack;
	BYTE byBuffer[1024];
	int iBufferLength;
	int iEncodeLength;
	
	PPPTRACE("SendTcpDataToServer\n");
	
	stHead.byNumberOfPack = 1;
	stHead.byRecvFlag = 0;
	stHead.byRoundMark = m_pUdpClientCallBack ? m_pUdpClientCallBack->GetRoundMark() : 0;
	stHead.sMaxUnRecv = 0;
	stHead.sMinUnRecv = 0;
		
	pstPack = &stPack;
	pstPack->sSeq = 0;
	pstPack->usLen = uiLen;
	memcpy(pstPack->byBuffer, pBuf, uiLen);

	iBufferLength = 0;
	iEncodeLength = MAX_UDP_PACKS_LENGTH;
	VERIFY(EncodeAppHead(stHead, byBuffer, iEncodeLength));
	iBufferLength += iEncodeLength;

	iEncodeLength = MAX_UDP_PACKS_LENGTH - iBufferLength;
	VERIFY(EncodeAppPack(stPack, &byBuffer[iBufferLength], iEncodeLength));
	iBufferLength += iEncodeLength;
		
	if(m_pUdpClientCallBack)
	{
		m_pUdpClientCallBack->SendTcpP2pData(nPlayerCount, ashPlayerID, (DWORD*)auiUin,iBufferLength, byBuffer);
	}
}

void CQQTUdpClientSocket::SendP2PGameData(UINT uiLen, BYTE *pBuf, BOOL bKeyData, short* sPlayerIDs, UINT* uiUins, INT nNumOfPlayer, BOOL bForceUseServer)
{
	LOG_TIME(a1, "CQQTUdpClientSocket::SendP2PGameData", 30);
	if(NULL == pBuf || uiLen > BIG_BUFFER_SIZE)
	{
		ASSERT(FALSE);
		return;
	}

	enum
	{
		_send_data_method_c2c_udp = 0,
		_send_data_method_csc_udp,
		_send_data_method_csc_tcp
	};
	
	_qqt_udp_app_head stHead;
	_qqt_udp_app_pack astPack[2];
	_qqt_udp_app_pack *pstPack = NULL;
	BYTE byBuffer[1024];
	int iBufferLength;
	int iEncodeLength;
/*	
	if(bKeyData)
	{
		PPPTRACE("SendP2PGameData: Find Key Data, Send By Tcp\n");
		//如果是关键数据，通过server进行TCP转发
		stHead.byNumberOfPack = 1;
		stHead.byRecvFlag = 0;
		stHead.byRoundMark = m_pUdpClientCallBack ? m_pUdpClientCallBack->GetRoundMark() : 0;
		stHead.sMaxUnRecv = 0;
		stHead.sMinUnRecv = 0;
		
		pstPack = &astPack[0];
		pstPack->sSeq = 0;
		pstPack->usLen = uiLen;
		memcpy(pstPack->byBuffer, pBuf, uiLen);

		iBufferLength = 0;
		iEncodeLength = MAX_UDP_PACKS_LENGTH;
		VERIFY(EncodeAppHead(stHead, byBuffer, iEncodeLength));
		iBufferLength += iEncodeLength;

		iEncodeLength = MAX_UDP_PACKS_LENGTH - iBufferLength;
		VERIFY(EncodeAppPack(astPack[0], &byBuffer[iBufferLength], iEncodeLength));
		iBufferLength += iEncodeLength;

		// remove it by JohnZhou
		if(m_pUdpClientCallBack)
		{
			LOG_TIME(a2, "CQQTUdpClientSocket::SendP2PGameData: SendTcpP2pData()", 30);
			m_pUdpClientCallBack->SendTcpP2pData(1, &m_sDestPlayerID, &m_dwDestUin, iBufferLength, byBuffer);
		}
	}
*/

	m_shSendSeq++;
	int iNumberOfPack = 0;

	//当前包
	pstPack = &astPack[iNumberOfPack];
	pstPack->sSeq = m_shSendSeq;
	pstPack->usLen = uiLen;
	memcpy(pstPack->byBuffer, pBuf, uiLen);
	iNumberOfPack++;

	//是否能加上上一个包
	if(m_bLastPkg && (GetAppPackLength(m_stLastPkg) + GetAppPackLength(astPack[0]) < MAX_UDP_PACKS_LENGTH) )
	{
		astPack[iNumberOfPack] = m_stLastPkg;
		iNumberOfPack++;
	}
	
	stHead.byNumberOfPack = iNumberOfPack;
	stHead.byRecvFlag = 0;
	stHead.byRoundMark = m_pUdpClientCallBack ? m_pUdpClientCallBack->GetRoundMark() : 0;
	stHead.sMaxUnRecv = 0;
	stHead.sMinUnRecv = 0;
	
	iBufferLength = 0;
	iEncodeLength = MAX_UDP_PACKS_LENGTH;
	VERIFY(EncodeAppHead(stHead, byBuffer, iEncodeLength));
	iBufferLength += iEncodeLength;

	for(int i = iNumberOfPack - 1; i >= 0; i--)
	{
		pstPack = &astPack[i];
		iEncodeLength = MAX_UDP_PACKS_LENGTH - iBufferLength;
		VERIFY(EncodeAppPack(*pstPack, &byBuffer[iBufferLength], iEncodeLength));
		iBufferLength += iEncodeLength;
	}

	byte byEncryptBuffer[2048];			//加密后的消息流
	int iEncryptBufferLength = 2048;
	::oi_symmetry_encrypt2(byBuffer, iBufferLength, s_keyp2p, byEncryptBuffer,&iEncryptBufferLength);

	//如果p2p不通则转发
	if(_connection_state_can_pass != m_iConnectState || bForceUseServer )
	{
		short sArPlayerID[8];
		unsigned int iArUin[8];
		int numop =1;
		if ( nNumOfPlayer == 0 || !sPlayerIDs || !uiUins )
		{
			sArPlayerID[0] = m_sDestPlayerID;
			iArUin[0] = m_dwDestUin;
		}
		else
		{
			numop = nNumOfPlayer;
			assert(nNumOfPlayer <= 8);
			for ( int i = 0; i < nNumOfPlayer; i++ )
			{
				sArPlayerID[i] = sPlayerIDs[i];
				iArUin[i] = uiUins[i];
			}
		}
		m_pUdpSvrSocket->BroadCastData(byEncryptBuffer, iEncryptBufferLength, sArPlayerID, iArUin, numop);
	}
	else
	{
		LOG_TIME(A4, "CQQTUdpClientSocket::SendP2PGameData: cc_SendP2PGameData", 30);
		m_ProtocolSocket.cc_SendP2PGameData(byEncryptBuffer, iEncryptBufferLength);
	}



	m_stLastPkg = astPack[0];
	//m_bLastPkg = TRUE;
}
void CQQTUdpClientSocket::CheckTcpData()
{	
	//LOGSTRING(false, "ctd", "void CQQTUdpClientSocket::CheckTcpData()");

	DWORD dwCurrentTime = ::_p2p_GetTickCount();
	_qqt_udp_app_head apphead;
	::ZeroMemory(&apphead, sizeof(_qqt_udp_app_head));
	_qqt_udp_app_pack pack[10];
	
	CQQTUdpBufferMgr::IMsgBuffer *pMsgBuffer = NULL;
	byte *pBuffer;
	int iLen;
	int iCurrentSize = 0;

	for(int i = 0; i < m_arPtrMsgBuffer.GetSize(); i ++)
	{
		if(pMsgBuffer = m_arPtrMsgBuffer[i])
		{
			if(pMsgBuffer->GetRightLevel() > 2 && dwCurrentTime - pMsgBuffer->GetBufferFirstSendTime(m_iClientTag) >= TCP_RESEND_TIME_LENGTH)
			{
				ASSERT(pMsgBuffer->GetRightLevel() > 0);
				pMsgBuffer->GetBuffer(pBuffer, iLen);
				pack[apphead.byNumberOfPack].usLen = (unsigned short)iLen;
				pack[apphead.byNumberOfPack].sSeq = pMsgBuffer->GetSequence(m_iClientTag);					
				if(iCurrentSize + GetAppPackLength(pack[apphead.byNumberOfPack]) > MAX_UDP_PACKS_LENGTH)
				{
					continue;
				}
				::memcpy(pack[apphead.byNumberOfPack].byBuffer, pBuffer, iLen);
				iCurrentSize += GetAppPackLength(pack[apphead.byNumberOfPack]);

				//LOGSTRING(false, "ctd", "pack info:  seq = %d, len = %d", pack[apphead.byNumberOfPack].sSeq, pack[apphead.byNumberOfPack].sLen);
				//LOGBIN(false, "ctd", pack[apphead.byNumberOfPack].sLen, (const char*)pack[apphead.byNumberOfPack].byBuffer);

				apphead.byNumberOfPack ++;
				m_arPtrMsgBuffer.Remove(i --);
				
				if(10 == apphead.byNumberOfPack)
				{
					break;
				}
			}
		}
	}

	if(0 == apphead.byNumberOfPack)
	{
		return;
	}

	byte byBuffer[1024];
	//打包
	ASSERT(m_pUdpClientCallBack);
	apphead.byRoundMark = m_pUdpClientCallBack ? m_pUdpClientCallBack->GetRoundMark() : 0;
	apphead.sMinUnRecv = m_RecvSet.GetMinUnReceive();	
	apphead.sMaxUnRecv = m_RecvSet.GetMaxUnRecv();
	m_RecvSet.GetReceiveFlag(apphead.sMinUnRecv, apphead.sMaxUnRecv, apphead.byRecvFlag);

	int iBufferLength = 0;
	int iEncodeLength = MAX_UDP_PACKS_LENGTH;
	VERIFY(EncodeAppHead(apphead, (byte*)byBuffer, iEncodeLength));
	iBufferLength += iEncodeLength;

	for(i = 0; i < apphead.byNumberOfPack; i ++)
	{
		//LOGSTRING(false, "ctd", "pack info:  seq = %d, len = %d", pack[i].sSeq, pack[i].sLen);
		//LOGBIN(false, "ctd", pack[i].sLen, (const char*)pack[i].byBuffer);
		iEncodeLength = MAX_UDP_PACKS_LENGTH - iBufferLength;
		VERIFY(EncodeAppPack(pack[i], (byte*)&byBuffer[iBufferLength], iEncodeLength));
		iBufferLength += iEncodeLength;
	}

	//LOGSTRING(false, "ctd", "full pack info: len = %d", iBufferLength);
	//LOGBIN(false, "ctd", iBufferLength, (const char*)byBuffer);
	m_pUdpClientCallBack->SendTcpP2pData(1, &m_sDestPlayerID, &m_dwDestUin, iBufferLength, (byte*)byBuffer);
}
void CQQTUdpClientSocket::ProcessP2pData(short sPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen)
{
	//decode app head
	int iPos = 0;
	int iLeftBufferLen = nBodyLen;
	_qqt_udp_app_head appHead;
	_qqt_udp_app_pack appPack[10];
	if(!DecodeAppHead(appHead, pBody, iLeftBufferLen))
	{
		ASSERT(FALSE);
		return;
	}
	if(appHead.byNumberOfPack > 10 || 0 == appHead.byNumberOfPack)
	{
		ASSERT(FALSE);
		return;
	}
	if(appHead.byRoundMark != m_pUdpClientCallBack->GetRoundMark())
	{
		return;
	}
	iPos += iLeftBufferLen;
	iLeftBufferLen = nBodyLen - iPos;
	for(int i = 0; i < appHead.byNumberOfPack; i ++)
	{
		if(!DecodeAppPack(appPack[i], &pBody[iPos], iLeftBufferLen))
		{
			ASSERT(FALSE);
			return;
		}
		iPos += iLeftBufferLen;
		iLeftBufferLen = nBodyLen - iPos;
	}
	if(iLeftBufferLen)
	{
		ASSERT(FALSE);
		return;
	}

	#ifdef _ENABLE_QQT_LOG_
	CString strPackNumber, strTempPackNumber;
	for(i = 0; i < appHead.byNumberOfPack; i ++)
	{
		strTempPackNumber.Format("%d, ", appPack[i].sSeq);
		strPackNumber += strTempPackNumber;
	}
	_logstring("receive package: %s", strPackNumber);
	#endif

	//至此解包完毕
	for(i = 0; i < appHead.byNumberOfPack; i ++)
	{
		m_pUdpClientCallBack->OnRecvP2PGameDataFromClient(sPlayerID, uiUin, m_sDestPlayerID, appPack[i].byBuffer, appPack[i].usLen);
//		m_RecvSet.MarkBit(appPack[i].sSeq);
	}
/*
	//删除最小没有收到buffer以下所有buffer
	for(i = 0; i < m_arPtrMsgBuffer.GetSize(); i ++)
	{
		if(m_arPtrMsgBuffer[i]->GetSequence(m_iClientTag) < appHead.sMinUnRecv)
		{
			m_arPtrMsgBuffer.Remove(i --);			
		}
	}
	//如果最大最小未收到ｓｅｑ相等，则不进行进一步处理
	if(appHead.sMaxUnRecv == appHead.sMinUnRecv)
	{
		return;
	}
	m_arPtrMsgBuffer.RefreshPtrArrayByRecvFlag(m_iClientTag, appHead.sMinUnRecv, appHead.sMaxUnRecv, appHead.byRecvFlag);
	
	#ifdef _ENABLE_QQT_LOG_
	_logstring("对方没有受到包的信息：");

	strPackNumber = "";
	if(!(appHead.byRecvFlag & 0x01))
	{
		strTempPackNumber.Format("%d, ", appHead.sMinUnRecv + 1);
		strPackNumber += strTempPackNumber;
	}
	if(!(appHead.byRecvFlag & 0x02))
	{
		strTempPackNumber.Format("%d, ", appHead.sMinUnRecv + 2);
		strPackNumber += strTempPackNumber;
	}
	if(!(appHead.byRecvFlag & 0x04))
	{
		strTempPackNumber.Format("%d, ", appHead.sMinUnRecv + 3);
		strPackNumber += strTempPackNumber;
	}
	if(!(appHead.byRecvFlag & 0x08))
	{
		strTempPackNumber.Format("%d, ", appHead.sMinUnRecv + 4);
		strPackNumber += strTempPackNumber;
	}
	if(!(appHead.byRecvFlag & 0x10))
	{
		strTempPackNumber.Format("%d, ", appHead.sMaxUnRecv - 4);
		strPackNumber += strTempPackNumber;
	}
	if(!(appHead.byRecvFlag & 0x20))
	{
		strTempPackNumber.Format("%d, ", appHead.sMaxUnRecv - 3);
		strPackNumber += strTempPackNumber;
	}
	if(!(appHead.byRecvFlag & 0x40))
	{
		strTempPackNumber.Format("%d, ", appHead.sMaxUnRecv - 2);
		strPackNumber += strTempPackNumber;
	}
	if(!(appHead.byRecvFlag & 0x80))
	{
		strTempPackNumber.Format("%d, ", appHead.sMaxUnRecv - 1);
		strPackNumber += strTempPackNumber;
	}
	_logstring("	%s", strPackNumber);
	#endif
*/
}
void CQQTUdpClientSocket::OnGetTcpP2pGameData(short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen)
{
	PPPTRACE("OnGetTcpP2pGameData\n");
	ProcessP2pData(shPlayerID, uiUin, pBody, nBodyLen);
}
void CQQTUdpClientSocket::OnRecvP2PGameData(ULONG ulSrcIP, short shPlayerID, unsigned int uiUin, BYTE *pEncryptBuffer, int iEncryptBufferLen)
{
	//解密
	byte pBody[2048];			//解密后的消息流
	int nBodyLen = 2048;
	if(!::oi_symmetry_decrypt2(pEncryptBuffer, iEncryptBufferLen, s_keyp2p, pBody, &nBodyLen))
	{
		ASSERT(FALSE);
		return;
	}
	//解密 end

	ProcessP2pData(shPlayerID, uiUin, pBody, nBodyLen);
}

CQQTUdpClientSocket::CFastMsgPtrArray::CFastMsgPtrArray()
{
	FreeAll();	
}
CQQTUdpClientSocket::CFastMsgPtrArray::~CFastMsgPtrArray()
{}
void CQQTUdpClientSocket::CFastMsgPtrArray::Add(CQQTUdpBufferMgr::IMsgBuffer *p, BOOL bSetPre)
{
	if(m_iSize < FAST_MSG_PTR_ARRAY_SIZE)
	{
		m_arPtr[m_iSize ++] = p;
		if(bSetPre)
		{
			m_iTagPre = m_iSize - 1;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}
void CQQTUdpClientSocket::CFastMsgPtrArray::Remove(int i)
{
	if(i >= 0 && i < m_iSize)
	{
		m_arPtr[i]->Release();
		m_arPtr[i] = m_arPtr[-- m_iSize];
		if(m_iTagPre == m_iSize)
		{
			m_iTagPre = i;
		}
	}
}
void CQQTUdpClientSocket::CFastMsgPtrArray::FreeAll()
{
	m_iSize = 0;
	m_iTagPre = -1;
}
CQQTUdpBufferMgr::IMsgBuffer* CQQTUdpClientSocket::CFastMsgPtrArray::GetPre()
{
	return (-1 == m_iTagPre) ? NULL : m_arPtr[m_iTagPre];	
}
CQQTUdpBufferMgr::IMsgBuffer* CQQTUdpClientSocket::CFastMsgPtrArray::operator[](int i)
{
	if(i >= 0 && i < m_iSize)
	{
		return m_arPtr[i];
	}
	ASSERT(FALSE);
	return NULL;
}
int CQQTUdpClientSocket::GetAppPackLength(_qqt_udp_app_pack &appPack)
{
	ASSERT(appPack.usLen > 0 && appPack.usLen < BIG_BUFFER_SIZE);
	return sizeof(_qqt_udp_app_pack) - BIG_BUFFER_SIZE + appPack.usLen;
}
CQQTUdpClientSocket::CBoolBitSet::CBoolBitSet()
{
	Clear();
}
CQQTUdpClientSocket::CBoolBitSet::~CBoolBitSet()
{}
void CQQTUdpClientSocket::CBoolBitSet::Clear()
{
	::ZeroMemory(m_arByte, BIT_SET_SIZE);		
	m_iSize = 0;
	m_sMaxUnReceive = 0;
	m_sMinUnReceive = 0;
}
void CQQTUdpClientSocket::CBoolBitSet::MarkBit(int iPos)
{
	if(-1 == iPos)
	{
		return;
	}
	if(iPos >= 0 && iPos < BIT_SET_SIZE * 8)
	{
		m_arByte[iPos >> 3] |= 1 << (iPos & 0x7);
	}
	else
	{
		ASSERT(FALSE);
	}
	if(iPos == m_sMinUnReceive)
	{
		while(GetBit(m_sMinUnReceive))
		{
			m_sMinUnReceive ++;
		}
	}
	if(m_sMaxUnReceive <= iPos)
	{
		m_sMaxUnReceive = iPos + 1;
	}
}
void CQQTUdpClientSocket::CBoolBitSet::ClearBit(int iPos)
{
	ASSERT(iPos >= 0 && iPos < BIT_SET_SIZE * 8);
	m_arByte[iPos >> 3] &= ~(1 << (iPos & 0x7));
}
BOOL CQQTUdpClientSocket::CBoolBitSet::GetBit(int iPos)
{
	ASSERT(iPos >= 0 && iPos < BIT_SET_SIZE * 8);
	return (m_arByte[iPos >> 3] & (1 << (iPos & 0x7))) ? TRUE : FALSE;
}
void CQQTUdpClientSocket::CBoolBitSet::GetReceiveFlag(int iMinUnReceive, int iMaxUnReceive, byte &byFlag)
{
	byFlag = 0;
	if(iMinUnReceive >= 0 && iMinUnReceive < BIT_SET_SIZE * 8 && iMaxUnReceive >= 0 && iMaxUnReceive < BIT_SET_SIZE * 8)
	{
		int i, iPos;
		for(i = 0; i < 4; i ++)
		{
			iPos = iMinUnReceive + 1 + i;
			byFlag <<= 1;
			if(iPos < BIT_SET_SIZE * 8 && GetBit(iPos))
			{
				byFlag |= 1;
			}
		}
		for(i = 0; i < 4; i ++)
		{
			iPos = iMaxUnReceive - 4 + i;
			byFlag <<= 1;
			if(iPos >= 0 && GetBit(iPos))
			{
				byFlag |= 1;
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}
short CQQTUdpClientSocket::CBoolBitSet::GetMaxUnRecv()
{
	return m_sMaxUnReceive;		
}
short CQQTUdpClientSocket::CBoolBitSet::GetMinUnReceive()
{
	return m_sMinUnReceive;
}


BOOL CQQTUdpClientSocket::EncodeAppPack(_qqt_udp_app_pack &appPack, byte *pOutBuffer, int &iOutBufferLen)
{
	if(iOutBufferLen >= GetAppPackLength(appPack))
	{
		unsigned long nSrcLen = appPack.usLen;
		unsigned long nDstLen;
		nDstLen = nSrcLen;
		unsigned char* pBufferDst = new unsigned char[nDstLen];
		if(0 != compress(pBufferDst , &nDstLen, appPack.byBuffer, nSrcLen))
		{
			assert(0);
			delete pBufferDst;
			return FALSE;
		}

		if ( nDstLen > nSrcLen )
		{
			assert(0);
			delete pBufferDst;
			return FALSE;
		}

		*((unsigned short*)pOutBuffer) = ::htons(appPack.sSeq);
		*((unsigned short*)&pOutBuffer[2]) = ::htons(nDstLen);
		::memcpy(&pOutBuffer[4], pBufferDst, nDstLen);
		iOutBufferLen = 4 + nDstLen;
		delete pBufferDst;
		return TRUE;
	}
	return FALSE;
}
BOOL CQQTUdpClientSocket::EncodeAppHead(_qqt_udp_app_head &appHead, byte *pOutBuffer, int &iOutBufferLen)
{
	if(iOutBufferLen >= sizeof(_qqt_udp_app_head))
	{
		_qqt_udp_app_head nshead;
		nshead = appHead;

		nshead.sMinUnRecv = ::htons(appHead.sMinUnRecv);
		nshead.sMaxUnRecv = ::htons(appHead.sMaxUnRecv);

		::memcpy(pOutBuffer, &nshead, sizeof(_qqt_udp_app_head));
		iOutBufferLen = sizeof(_qqt_udp_app_head);
		return TRUE;
	}
	return FALSE;
}
BOOL CQQTUdpClientSocket::DecodeAppPack(_qqt_udp_app_pack &appPack, byte *pInBuffer, int &iInBufferLen)
{
	if(iInBufferLen < 5 || !pInBuffer)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	appPack.sSeq = ::ntohs(*((unsigned short*)pInBuffer));
	appPack.usLen =  ::ntohs(*((unsigned short*)&pInBuffer[2]));
	if(appPack.usLen + 4 > iInBufferLen || appPack.usLen > BIG_BUFFER_SIZE)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	unsigned long nSrcLen = appPack.usLen;
	unsigned long nDstLen = 1024;
	BYTE DstBuffer[1024];
	if( uncompress((unsigned char *)DstBuffer, &nDstLen, (const unsigned char *)(pInBuffer + 4), nSrcLen) != 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	::memcpy(appPack.byBuffer, DstBuffer, nDstLen);
	iInBufferLen = appPack.usLen + 4;
	return TRUE;
}
BOOL CQQTUdpClientSocket::DecodeAppHead(_qqt_udp_app_head &appHead, byte *pInBuffer, int &iInBufferLen)
{
	if(iInBufferLen < sizeof(_qqt_udp_app_head) || !pInBuffer)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	_qqt_udp_app_head tempHead;
	::memcpy(&tempHead, pInBuffer, sizeof(_qqt_udp_app_head));
	appHead = tempHead;
	appHead.sMinUnRecv = ::ntohs(tempHead.sMinUnRecv);
	appHead.sMaxUnRecv = ::ntohs(tempHead.sMaxUnRecv);
	iInBufferLen = sizeof(_qqt_udp_app_head);
	return TRUE;
}
void CQQTUdpClientSocket::CFastMsgPtrArray::RefreshPtrArrayByRecvFlag(int iClientTag, int iMinUnRecv, int iMaxUnRecv, byte byFlag)
{
	BOOL bMinFlag[4], bMaxFlag[4];
	int i;
	for(i = 0; i < 4; i ++)
	{
		bMaxFlag[3 - i] = (byFlag & 0x01) ? TRUE : FALSE;
		byFlag >>= 1;
	}
	for(i = 0; i < 4; i ++)
	{
		bMinFlag[3 - i] = (byFlag & 0x01) ? TRUE : FALSE;
		byFlag >>= 1;
	}
	short sSeq;
	for(i = 0; i < m_iSize; i ++)
	{
		sSeq = m_arPtr[i]->GetSequence(iClientTag);
		if(sSeq > iMinUnRecv && sSeq < iMinUnRecv + 5)
		{			
			if(bMinFlag[sSeq - iMinUnRecv - 1])
			{				
				Remove(i --);
			}
			else
			{
				m_arPtr[i]->SetBufferState(iClientTag, CQQTUdpBufferMgr::_buffer_state_need_resend);
			}
		}
		else if(sSeq > iMaxUnRecv - 5 && sSeq < iMaxUnRecv)
		{
			if(bMaxFlag[sSeq + 4 - iMaxUnRecv])
			{				
				Remove(i --);
			}
			else
			{
				m_arPtr[i]->SetBufferState(iClientTag, CQQTUdpBufferMgr::_buffer_state_need_resend);
			}
		}
		else if(sSeq == iMinUnRecv)
		{
			m_arPtr[i]->SetBufferState(iClientTag, CQQTUdpBufferMgr::_buffer_state_need_resend);
		}
	}
}
CQQTUdpBufferMgr::IMsgBuffer* CQQTUdpClientSocket::CFastMsgPtrArray::GetLongestNoSendPriority2(int iClientTag)
{
	DWORD dwTime = 0;
	CQQTUdpBufferMgr::IMsgBuffer *pMsgBuffer = NULL;
	for(int i = 0; i < m_iSize; i ++)
	{
		if(2 == m_arPtr[i]->GetRightLevel())
		{
			if(0 == dwTime || m_arPtr[i]->GetBufferTimeStamp(iClientTag) < dwTime)
			{
				pMsgBuffer = m_arPtr[i];
				dwTime = m_arPtr[i]->GetBufferTimeStamp(iClientTag);
			}
		}
	}

	return pMsgBuffer;
}
void CQQTUdpClientSocket::SetGameStatus(int iGameStatus)
{
	if(IsDestPlayer(0))
	{
		ASSERT(FALSE);
		return;
	}
	m_iGameStatus = iGameStatus;

	m_arPtrMsgBuffer.FreeAll();
	m_RecvSet.Clear();
	m_sSequence = 0;
	m_bLastPkg = FALSE;
	m_shSendSeq = 0;
	memset(&m_stLastPkg, '\0', sizeof(m_stLastPkg));
}
void CQQTUdpClientSocket::OnClose()
{
	m_ProtocolSocket.Initialize();
	m_iConnectState = _connection_state_no_start;
	m_iTransmitTimer = RE_TRANSMIT_TIME_LENGTH;
}
