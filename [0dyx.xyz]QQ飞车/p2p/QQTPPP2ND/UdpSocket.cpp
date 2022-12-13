// UdpSocket.cpp : implementation file
//

#include "stdafx.h"
#include "UdpSocket.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define _ENABLE_LOST_PACKAGE_
#ifdef _ENABLE_LOST_PACKAGE_
int g_iLostRate = 0;
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

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CUdpSocket
ULONG CUdpSocket::m_ulLocalIP = 0;
CUdpSocket::CUdpSocket()
{
	m_pUdpSocketCallBack = NULL;
	m_usLocalPort = 0;
	m_ulServerIP = 0;
	m_usServerPort = 0;
	m_ulClientIP = 0;
	m_usClientPort = 0;
	m_sUdpSendBufferLength = 0;
#ifdef _ENABLE_LOST_PACKAGE_
	
	static BOOL s_bLoad = TRUE;
	if(s_bLoad)
	{
		s_bLoad = FALSE;
		CString str = ::GetRootPath() + "config.ini";
		g_iLostRate = ((::GetPrivateProfileInt("QQTPPP", "LostRate", 0, str) % 100) + 100) % 100;		
	}
#endif
}

CUdpSocket::~CUdpSocket()
{
	CloseSocket();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUdpSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CUdpSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUdpSocket member functions
ULONG CUdpSocket::GetLocalIP()
{
	if(m_ulLocalIP)
	{
		return m_ulLocalIP;
	}
	::GetLocalIP(&m_ulLocalIP);
	m_ulLocalIP = ::ntohl(m_ulLocalIP);
	return m_ulLocalIP;
}

BOOL CUdpSocket::BindPort(USHORT usPort)
{
	ASSERT(0 == m_usLocalPort);
	if(m_usLocalPort && (0 == usPort || usPort == m_usLocalPort))
	{//已经创建不需再创建 
		return TRUE;
	}
	if(m_usLocalPort)
	{//需要close原来的socket
		_internal_socket_close();
	}
	
	if(!Create(usPort, SOCK_DGRAM))
	{
		//LOGSTRING(TRUE, LOGPATH, "ERROR, Create Port Failed!");
		ASSERT(FALSE);
		return FALSE;
	}

	//Set UDP RCVBUF Size
	int nRecvBuf = 256 * 1024;
	int nOptLen = sizeof(int);	
	if(SetSockOpt(SO_RCVBUF, &nRecvBuf, nOptLen))
	{
		TRACE("Set RCVBUF SUCCESS\n");
	}

	struct sockaddr_in stAddr;
	int nLen = sizeof(stAddr);
	if(!GetSockName((SOCKADDR*)&stAddr, &nLen))
	{
		//LOGSTRING(TRUE, LOGPATH, "ERROR, GetSockName Failed!");
		ASSERT(FALSE);
		return FALSE;
	}

	m_usLocalPort = ::ntohs(stAddr.sin_port);
	return TRUE;
}
void CUdpSocket::SetUdpSocketCallBack(IUdpSocketCallBack* pQQTUdpSocketCallBack)
{
	m_pUdpSocketCallBack = pQQTUdpSocketCallBack;
}

void CUdpSocket::SetServerAddr(ULONG ulIP, USHORT usPort)
{
	m_ulServerIP = ulIP;
	m_usServerPort = usPort;
}

void CUdpSocket::SetClientAddr(ULONG ulIP, USHORT usPort)
{
	m_ulClientIP = ulIP;
	m_usClientPort = usPort;
}

void CUdpSocket::SendUdpDataToServer(int nLen, const byte *pBuffer)
{
	SendUdpData(nLen, pBuffer, m_ulServerIP, m_usServerPort);
}

void CUdpSocket::SendUdpDataToClient(int nLen, const byte *pBuffer)
{
	SendUdpData(nLen, pBuffer, m_ulClientIP, m_usClientPort);
}

void CUdpSocket::SendUdpData(int nLen, const byte *pBuffer, ULONG ulIP, USHORT usPort)
{
#ifdef _ENABLE_LOST_PACKAGE_
	int iRand = ((rand() % 100) + 100) % 100;
	if(iRand < g_iLostRate)
	{
		return;
	}	
#endif

	if(!m_usLocalPort)
	{
		ASSERT(FALSE);
		return;
	}
	if(nLen <= 0 || NULL == pBuffer || nLen >= MAX_UDP_PACKAGE)
	{
		//LOGSTRING(TRUE, LOGPATH, "ERROR, Cant Send Null Buffer");
		ASSERT(FALSE);
		return;
	}

	ASSERT(ulIP && usPort);
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;	
	sockAddr.sin_addr.s_addr = ::htonl(ulIP);
	sockAddr.sin_port = ::htons(usPort);
	
	PPPTRACE("SendUdpData to %d.%d.%d.%d:%d\n", (ulIP>>24)&0xFF, 
		(ulIP>>16)&0xFF, (ulIP>>8)&0xFF, ulIP&0xFF, usPort);

	if(SOCKET_ERROR == SendTo(pBuffer, nLen, (SOCKADDR*)&sockAddr, sizeof(sockAddr)))
	{
		int iErrorCode = ::GetLastError();
		if(WSAEWOULDBLOCK == iErrorCode)
		{
			if(nLen <= MAX_UDP_PACKAGE)
			{
				::memcpy(m_byUdpSendBuffer, pBuffer, nLen);
				m_sUdpSendBufferLength = nLen;
			}
		}
		else
		{
			//LOGSTRING(TRUE, LOGPATH, "ERROR, Send Udp Data Failed! error code = %d", iErrorCode);
			ASSERT(FALSE);
			_internal_socket_close();
		}
	}
}

void CUdpSocket::OnReceive(int nErrorCode) 
{
	SOCKADDR sockAddr;
	int nSockLen = sizeof(SOCKADDR);

	#define UDP_ACCEPT_BUFFER_SIZE	4096
	byte byBuffer[UDP_ACCEPT_BUFFER_SIZE];

	int nRecvBufferLen = ReceiveFrom(byBuffer, UDP_ACCEPT_BUFFER_SIZE, (SOCKADDR*)&sockAddr, &nSockLen, 0);
	if(SOCKET_ERROR == nRecvBufferLen)
	{
		int nLastError = ::GetLastError();
		if(WSAEWOULDBLOCK != nLastError && WSAECONNRESET != nLastError)
		{
			_internal_socket_close();
		}		
	}
	else
	{
		#ifdef _ENABLE_LOST_PACKAGE_
		int iRand = ((rand() % 100) + 100) % 100;
		if(iRand < g_iLostRate)
		{
			return;
		}	
		#endif

		if(m_pUdpSocketCallBack)
		{
			ULONG ulIP = ::ntohl(((sockaddr_in*)&sockAddr)->sin_addr.s_addr);
			USHORT usPort = ::ntohs(((sockaddr_in*)&sockAddr)->sin_port);
			m_pUdpSocketCallBack->OnReceive(ulIP, usPort, byBuffer, nRecvBufferLen);
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CUdpSocket::OnSend(int nErrorCode) 
{	/*
	if(m_sUdpSendBufferLength)
	{
		ASSERT(m_ulDestIP && m_usDestPort);
		sockaddr_in sockAddr;
		sockAddr.sin_family = AF_INET;		
		sockAddr.sin_addr.s_addr = ::htonl(m_ulDestIP);
		sockAddr.sin_port = ::htons(m_usDestPort);
		if(SOCKET_ERROR == SendTo(m_byUdpSendBuffer, m_sUdpSendBufferLength, (SOCKADDR*)&sockAddr, sizeof(sockAddr)))
		{
			if(WSAEWOULDBLOCK != ::GetLastError())
			{
				CAsyncSocket::OnSend(nErrorCode);
				_internal_socket_close();
				return;
			}
		}
		else
		{
			m_sUdpSendBufferLength = 0;
		}
	}	*/
	CAsyncSocket::OnSend(nErrorCode);
}

void CUdpSocket::OnClose(int nErrorCode) 
{
	if(m_usLocalPort)
	{
		m_usLocalPort = 0;
		if(m_pUdpSocketCallBack)
		{
			m_pUdpSocketCallBack->OnClose();
		}
	}
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CUdpSocket::_internal_socket_close()
{
	if(m_usLocalPort)
	{
		CloseSocket();
		if(m_pUdpSocketCallBack)
		{
			m_pUdpSocketCallBack->OnClose();
		}
	}
}
void CUdpSocket::CloseSocket()
{
	m_sUdpSendBufferLength = 0;
	if(m_usLocalPort)
	{
		Close();
		m_usLocalPort = 0;
	}	
}