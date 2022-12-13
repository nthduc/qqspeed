// QQTUdpServerSocket.cpp: implementation of the CQQTUdpServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QQTUdpServerSocket.h"

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

CQQTUdpServerSocket::CQQTUdpServerSocket()
{
	Initialize();
	m_ProtocolSocket.SetProtoolType(TRUE);
	m_ProtocolSocket.SetQQTUdpProtocolSocketReceiver(this);
	m_pSvrCallBack = NULL;
#ifdef _ENABLE_QQT_LOG_
	::_snprintf(m_szLogPath, MAX_PATH, "%s", "server_socket_log");
#endif
}

CQQTUdpServerSocket::~CQQTUdpServerSocket()
{
}

void CQQTUdpServerSocket::Initialize()
{
	m_ProtocolSocket.Initialize();
	m_QQTUdpLoginStatus = QQT_UDP_Login_Status_No_Start;
	m_iLoginRetryTimer = -1;
	m_iHelloTimer = -1;
}
void CQQTUdpServerSocket::SetServerAddr(ULONG ulSvrIP, USHORT usSvrPort)
{
	m_ProtocolSocket.SetServerAddr(ulSvrIP, usSvrPort);
}
void CQQTUdpServerSocket::SetMyInfo(DWORD dwUin, short sPlayerID)
{
	m_ProtocolSocket.SetMyInfo(dwUin, sPlayerID);
}
void CQQTUdpServerSocket::Login()
{
	if(QQT_UDP_Login_Status_Ok == m_QQTUdpLoginStatus)
	{
		ASSERT(FALSE);
	}
	m_ProtocolSocket.cs_UDPLogin();
	m_QQTUdpLoginStatus = QQT_UDP_Login_Status_Send_No_Receive;
	m_iLoginRetryTimer = UDP_LOGIN_RESEND_TIME;
}
void CQQTUdpServerSocket::OnClose()
{
	m_QQTUdpLoginStatus = QQT_UDP_Login_Status_No_Start;
	if(m_pSvrCallBack)
	{
		m_pSvrCallBack->OnSvrClose();
	}
	
	m_iLoginRetryTimer = UDP_LOGIN_RESEND_TIME;	
}

// 获得我的OutterIP
void CQQTUdpServerSocket::OnLoginResponse(ULONG ulSvrIP, ULONG ulOuterIP)
{
	if(ulSvrIP != m_ProtocolSocket.GetServerIP())
	{
		ASSERT(FALSE);
		return;
	}
	if(m_pSvrCallBack)
	{
		m_pSvrCallBack->OnLoginOk(ulOuterIP);
		m_iHelloTimer = UDP_SERVER_HELLO_TIME_LENGTH;
		m_QQTUdpLoginStatus = QQT_UDP_Login_Status_Ok;
	}
	else
	{
		ASSERT(FALSE);
	}
}
void CQQTUdpServerSocket::OnGetPlayerInfoFromSvr(ULONG ulSvrIP, short shSrcPlayerID, int nSrcUin, ULONG ulSrcOuterIP, USHORT usSrcOuterPort, ULONG ulSrcInerIP, USHORT usSrcInnerPort)
{
	if(ulSvrIP != m_ProtocolSocket.GetServerIP())
	{
		ASSERT(FALSE);
		return;
	}
	if(m_pSvrCallBack)
	{
		PPPTRACE("OnGetPlayerInfoFromSvr, Uin %d, OuterAddr: %d.%d.%d.%d:%d, InnerAddr: %d.%d.%d.%d:%d\n",
			nSrcUin, (ulSrcOuterIP>>24)&0xFF, (ulSrcOuterIP>>16)&0xFF, (ulSrcOuterIP>>8)&0xFF, (ulSrcOuterIP)&0xFF, usSrcOuterPort,
			(ulSrcInerIP>>24)&0xFF, (ulSrcInerIP>>16)&0xFF, (ulSrcInerIP>>8)&0xFF, (ulSrcInerIP)&0xFF, usSrcInnerPort);

		m_pSvrCallBack->OnGetPlayerInfoFromSvr(shSrcPlayerID, nSrcUin, ulSrcOuterIP, usSrcOuterPort, ulSrcInerIP, usSrcInnerPort);
	}
	else
	{
		ASSERT(FALSE);
	}
}
void CQQTUdpServerSocket::OnRecvP2PGameData(ULONG ulSrcIP, short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen)
{
	if(ulSrcIP != m_ProtocolSocket.GetServerIP())
	{
		ASSERT(FALSE);
		return;
	}
	if(m_pSvrCallBack)
	{
		m_pSvrCallBack->OnRecvP2PDataFromSvr(shPlayerID, uiUin, pBody, nBodyLen);
	}
	else
	{
		ASSERT(FALSE);
	}
}
void CQQTUdpServerSocket::OnTimer()
{
	if(m_iLoginRetryTimer > 0 || m_iHelloTimer > 0)
	{
		if(QQT_UDP_Login_Status_Ok != m_QQTUdpLoginStatus)
		{
			if(-- m_iLoginRetryTimer == 0)
			{
				Login();
			}
		}
		else if(-- m_iHelloTimer == 0)
		{
			m_ProtocolSocket.cs_Hello();
			m_iHelloTimer = UDP_SERVER_HELLO_TIME_LENGTH;
		}
	}	
}
void CQQTUdpServerSocket::BroadCastData(const byte *pBuffer, int nLen, short shPlayerID[8], unsigned int nUin[8], int nNumberOfPlayer)
{
	if(QQT_UDP_Login_Status_Ok == m_QQTUdpLoginStatus)
	{
		m_ProtocolSocket.cs_SendP2PGameData(pBuffer, nLen, shPlayerID, nUin, nNumberOfPlayer);
		m_iHelloTimer = UDP_SERVER_HELLO_TIME_LENGTH;
	}
	else
	{
		ASSERT(FALSE);
	}
}

#ifdef _ENABLE_QQT_LOG_
void CQQTUdpServerSocket::LogString(const char *pFormat, ...)
{
	CString str;
	va_list argList;
	va_start(argList, pFormat);
	str.FormatV(pFormat, argList);
	va_end(argList);
	LOGSTRING(TRUE, m_szLogPath, "%s", str);
}
void CQQTUdpServerSocket::LogBin(int nLen, const char *pBuf)
{
	LOGBIN(TRUE, m_szLogPath, nLen, pBuf);
}
void CQQTUdpServerSocket::Log2Win(char* pFormat, ...)
{
	CString str;
	va_list argList;
	va_start(argList, pFormat);
	str.FormatV(pFormat, argList);
	va_end(argList);
	DBWLOG("%s", str);
}
#endif