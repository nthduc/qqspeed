////////////////////////////////////////////////
//     FileName:    TcpSocket.cpp
//     Author:      Fish
//     Date:        2006-9-18 16:12:59
//     Description: []
////////////////////////////////////////////////

#include "stdafx.h"
#include "TcpSocket.h"


#define MSG_WSAEADDRINUSE		"The specified address is already in use.\n"
#define MSG_WSAEADDRNOTAVAIL	"The specified address is not available from the local machine.\n"
#define MSG_WSAEAFNOSUPPORT		"Addresses in the specified family cannot be used with this socket.\n"
#define MSG_WSAECONNREFUSED		"The attempt to connect was forcefully rejected.\n"
#define MSG_WSAEDESTADDRREQ		"A destination address is required.\n"
#define MSG_WSAEFAULT			"The lpSockAddrLen argument is incorrect.\n"
#define MSG_WSAEINVAL			"The socket is already bound to an address.\n"
#define MSG_WSAEISCONN			"The socket is already connected.\n"
#define MSG_WSAEMFILE			"No more file descriptors are available.\n"
#define MSG_WSAENETUNREACH		"The network cannot be reached from this host at this time.\n"
#define MSG_WSAENOBUFS			"No buffer space is available. The socket cannot be connected.\n"
#define MSG_WSAENOTCONN			"The socket is not connected.\n"
#define MSG_WSAENOTSOCK			"The descriptor is a file, not a socket.\n"
#define MSG_WSAETIMEDOUT		"The attempt to connect timed out without establishing a connection. \n"
#define MSG_WSAUNKNOWNERROR		"Unknown WSAErrorCode.\n"

typedef struct tagCODE2MSG
{
	int nCode;
	char* szMsg;
}CODE2MSG;

CODE2MSG g_code2msg[] = {
	{ WSAEADDRINUSE, MSG_WSAEADDRINUSE },
	{ WSAEADDRNOTAVAIL, MSG_WSAEADDRNOTAVAIL },
	{ WSAEAFNOSUPPORT, MSG_WSAEAFNOSUPPORT },
	{ WSAECONNREFUSED, MSG_WSAECONNREFUSED },
	{ WSAEDESTADDRREQ,MSG_WSAEDESTADDRREQ },
	{ WSAEFAULT, MSG_WSAEFAULT },
	{ WSAEINVAL, MSG_WSAEINVAL },
	{ WSAEISCONN, MSG_WSAEISCONN },
	{ WSAEMFILE, MSG_WSAEMFILE },
	{ WSAENETUNREACH, MSG_WSAENETUNREACH },
	{ WSAENOBUFS, MSG_WSAENOBUFS },
	{ WSAENOTCONN, MSG_WSAENOTCONN },
	{ WSAENOTSOCK, MSG_WSAENOTSOCK },
	{ WSAETIMEDOUT, MSG_WSAETIMEDOUT }
};
char* FormatWSAErrorCode(int nErrorCode)
{
	for ( int i = 0; i < sizeof(g_code2msg) / sizeof(CODE2MSG); i++ )
	{
		if ( nErrorCode == g_code2msg[i].nCode )
			return g_code2msg[i].szMsg;
	}
	return MSG_WSAUNKNOWNERROR;
}
// CTcpSocket

CTcpSocket::CTcpSocket()
{
	Reset();
}

CTcpSocket::~CTcpSocket()
{
}

void CTcpSocket::OnReceive(int nErrorCode)
{
	LogText("OnReceive(ErrorCode=%d)\n", nErrorCode);
	CAsyncSocket::OnReceive(nErrorCode);
	if ( nErrorCode == 0 )
	{
		ReceiveData();
	}
}

void CTcpSocket::ReceiveData()
{
	static BYTE byBuf[MAX_DATA_LEN];
	int nRecvLen = m_nLastLeftSize;
	m_nLastLeftSize = 0;
	int nLen = Receive(byBuf + nRecvLen, MAX_DATA_LEN - nRecvLen);
	if ( nLen < 0 || nLen >= MAX_DATA_LEN - nRecvLen )
	{
		ASSERT(0);
		return;
	}
	if ( nLen == 0 )
		return;
	nRecvLen += nLen;

	ASSERT( nRecvLen <= MAX_DATA_LEN );
	LPBYTE byData = byBuf;
	LPBYTE byDataEnd = byData + nRecvLen;
	while(1)
	{
		int nPackLen = 0;
		switch(m_nHeadLenBytes)
		{
		case sizeof(long):
			nPackLen = ntohl(*(int*)byData);
			ASSERT(nPackLen<=MAX_DATA_LEN);
			break;
		case sizeof(short):
			nPackLen = ntohs(*(short*)byData);
			ASSERT(nPackLen<=MAX_DATA_LEN);
			break;
		default:
			nPackLen = byDataEnd-byData;
			break;
		}
		if ( nPackLen == 0 )
			break;
		if ( byData + nPackLen > byDataEnd  ) //如果接收的不是一个完整包，下次继续
		{
			LogText("Recv %d bytes, but PackLen=%d, wait next data.\n", nRecvLen, nPackLen);
			m_nLastLeftSize = ( byDataEnd - byData );
			break;
		}
		else //if ( byData + nPackLen <= byDataEnd )
		{
			LogText("Recv %d bytes from %s.\n", nPackLen, m_SocketCreateInfo.szHostAddr);
			OnMyReceive(byData, nPackLen);
			byData += nPackLen;
			if ( byData == byDataEnd )		//读包完成
				break;
		}
	}
	if ( m_nLastLeftSize != 0 && byData != byBuf )
	{
		LogText("Left some data, size=%d,offset=%d.\n", m_nLastLeftSize, byData-byBuf);
		memcpy(byBuf, byData, m_nLastLeftSize);
		//	//	for ( int i = 0; i < m_nLastLeftSize; i++ )
		//	byBuf[i] = byData[i];
		int nPackLen = ntohl(*(int*)byBuf);
		ASSERT(nPackLen < MAX_DATA_LEN);
	}
}

void CTcpSocket::OnConnect(int nErrorCode)
{
	LogText("OnConnect(ErrorCode=%d)\n", nErrorCode);
	CAsyncSocket::OnConnect(nErrorCode);
	if ( nErrorCode != 0 ) //重试最多3个端口之后才决定是否连接失败
	{
		m_nCurPortIdx++;
		m_nCurPortIdx %= MAX_PORTS_COUNT;
		m_nRetryCount++;
		if ( !Connect(FALSE) )	//超过3次连接，认定失败
		{
			LogText("Connect failed: %s.", FormatWSAErrorCode(nErrorCode));
			m_nConnectState = eConnectError;
			OnMyConnect(nErrorCode);
		}
		else
		{
			LogText("Connect failed: %s. Retrying...", FormatWSAErrorCode(nErrorCode));
			m_nConnectState = eConnectRetrying;
		}
	}
	else
	{
		m_nConnectState = eConnectOK;
		OnMyConnect(nErrorCode);
		while ( m_DataBuffer.size() > 0 )
		{
			CSendData& data = m_DataBuffer.back();
			Send(data.m_byData, data.m_nLen);
			OnMySend(GetLastError());
			LogText("Send %d bytes.(waited send)\n", data.m_nLen);
			m_DataBuffer.pop_back();
		}
	}
}

void CTcpSocket::OnClose(int nErrorCode)
{
	CAsyncSocket::OnClose(nErrorCode);
	if ( m_nConnectState != eConnectOK )
	{
		OnConnect(WSAECONNREFUSED);
	}
	else
	{
		ReceiveData();
		LogText("OnClose(ErrorCode=%d)\n", nErrorCode);
		Close();
		m_nConnectState = eConnectClosed;
		OnMyClose(nErrorCode);
	}
}

void CTcpSocket::OnSend(int nErrorCode)
{
	LogText("OnSend(ErrorCode=%d)\n", nErrorCode);
	CAsyncSocket::OnSend(nErrorCode);
	//	OnMySend(nErrorCode);
}

void CTcpSocket::OnMyReceive(BYTE* Data, int DataLen)
{
}

void CTcpSocket::OnMyConnect(int nErrorCode)
{
}

void CTcpSocket::OnMyClose(int nErrorCode)
{
}

void CTcpSocket::OnMySend(int nErrorCode)
{
}
