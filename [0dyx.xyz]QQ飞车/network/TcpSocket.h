////////////////////////////////////////////////
//     FileName:    TcpSocket.h
//     Author:      Fish
//     Date:        2006-9-18 16:12:42
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "Network/Network_i.h"
#include "typedef.h"
#include "common.h"
#include "senddata.h"
#include "Timer.h"
#include "list"
using namespace std;

interface ISocketSinkProxy;

// CTcpSocket 命令目标
//特性：
//1、支持最多三个端口的Connect
class CTcpSocket : public CAsyncSocket, public CTimer
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();

	void Create(SOCKET_CREATE_INFO* SocketInfo, int nHeadLenBytes = sizeof(int))
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		Reset();
		if ( m_hSocket != INVALID_SOCKET )
		{
			Close();
		}
		if ( SocketInfo )
		{
			ASSERT(strlen(SocketInfo->szHostAddr) > 0 && typelen(SocketInfo->uPorts) > 0 );
			memcpy(&m_SocketCreateInfo, SocketInfo, sizeof(SOCKET_CREATE_INFO));
			m_nHeadLenBytes = nHeadLenBytes;
		}
		if ( m_hSocket == INVALID_SOCKET )
		{
			CAsyncSocket::Create();
			LogText(FormatErrorMessage(GetLastError()).c_str());
			int nOptLen = sizeof(int);
			if ( m_SocketCreateInfo.nSndBufSize != 0 )
				SetSockOpt(SO_SNDBUF, &m_SocketCreateInfo.nSndBufSize, nOptLen);
			if ( m_SocketCreateInfo.nRcvBufSize != 0 )
				SetSockOpt(SO_RCVBUF, &m_SocketCreateInfo.nRcvBufSize, nOptLen);
		}
	}

	void Reset()
	{
		KillTimer();
		m_DataBuffer.clear();
		//		memset(&m_SocketCreateInfo, 0, sizeof(SOCKET_CREATE_INFO));
		m_nCurPortIdx = rand() % MAX_PORTS_COUNT;
		m_nRetryCount = 0;
		m_nConnectState = eConnectReady;
		m_nLastLeftSize = 0;
	}

	void SetSinkProxy(ISocketSinkProxy* sink)
	{
		m_pSinkProxy = sink;
	}

	BOOL Connect(BOOL bInitial = TRUE)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if ( bInitial )
		{
			m_nCurPortIdx = rand() % MAX_PORTS_COUNT;
			m_nRetryCount = 0;
		}
		//else if ( m_nCurPortIdx >= typelen(m_SocketCreateInfo.uPorts) )//重试完所有的端口了
		else if ( m_nRetryCount > MAX_PORTS_COUNT )
		{
			return FALSE;
		}
		if ( m_nConnectState == eConnectOK )
			//|| m_nConnectState == eConnecting 
			//|| m_nConnectState == eConnectRetrying ) //已经连接成功或者正在连接
		{
			LogText("Already Connected.\n");
			return TRUE;
		}
		LogText("Connecting %s:%d\n", m_SocketCreateInfo.szHostAddr, m_SocketCreateInfo.uPorts[m_nCurPortIdx]);
		CAsyncSocket::Connect(m_SocketCreateInfo.szHostAddr, m_SocketCreateInfo.uPorts[m_nCurPortIdx]);
		DWORD dwError = GetLastError();
		switch(dwError)
		{
		case WSAEWOULDBLOCK:
			m_nConnectState = eConnecting;
			return TRUE;
		case WSAEISCONN: //如果已经连接了，需要先关闭
			Close();
		case WSAENOTSOCK://如果没有创建Socket，需要创建
			CAsyncSocket::Create();
			CAsyncSocket::Connect(m_SocketCreateInfo.szHostAddr, m_SocketCreateInfo.uPorts[m_nCurPortIdx]);
			ASSERT(GetLastError() == WSAEWOULDBLOCK);
			return TRUE;
		}
		return FALSE;
	}

	BOOL SendData(const void* lpBuf, int nBufLen, int nFlags = 0)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		switch(m_nConnectState)
		{
		case eConnectClosed:
		case eConnectError:
			//assert(0); // you can not send data while not connected
			Create(NULL);
			SendData(lpBuf, nBufLen, nFlags);
			return FALSE;

		case eConnectReady:
			if ( !Connect() )
				return FALSE;
		case eConnecting:
		case eConnectRetrying:
			m_DataBuffer.push_back(CSendData(nBufLen, lpBuf));
			return TRUE;

		case eConnectOK:
			Send(lpBuf, nBufLen, nFlags);
			OnMySend(GetLastError());
			LogText("Send %d bytes.\n", nBufLen);
			return TRUE;
		default:
			ASSERT(0);
			return FALSE;
		}
	}

	LPSOCKET_CREATE_INFO GetSocketInfo()
	{
		return &m_SocketCreateInfo;
	}

public:
	//CAsyncSocket
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	//CTimer
	virtual void OnTimeOut(){}
protected: 
	//overidables
	virtual void OnMyReceive(BYTE* Data, int DataLen);
	virtual void OnMyConnect(int nErrorCode);
	virtual void OnMyClose(int nErrorCode);
	virtual void OnMySend(int nErrorCode);

private:
	void ReceiveData();

protected:
	list<CSendData> m_DataBuffer;

	//服务器地址/端口,支持最多3个端口的重试连接
	SOCKET_CREATE_INFO	m_SocketCreateInfo;
	int				m_nCurPortIdx;
	int				m_nRetryCount;
	ConnectState	m_nConnectState;
	int				m_nHeadLenBytes;
	//包边界记录
	int				m_nLastLeftSize;

	ISocketSinkProxy* m_pSinkProxy;
};


