#include "stdafx.h"
#include "multisvrtcpsocket.h"
#include "utils.h"

#define SOCKET_TIMEOUT	3000

CMultiSvrTcpSocket::CMultiSvrTcpSocket()
{
	Reset();
}

CMultiSvrTcpSocket::~CMultiSvrTcpSocket()
{
	Reset();
}

void CMultiSvrTcpSocket::Create(int nHeadLenBytes)
{
	m_nHeadLenBytes = nHeadLenBytes;
}

void CMultiSvrTcpSocket::Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( m_dwTimerHandle )
	{
		CTimerMgr::Get()->UnRegist(m_dwTimerHandle);
		m_dwTimerHandle = 0;
	}
	CAsyncSocket::ShutDown();
	CAsyncSocket::Close();
}

void CMultiSvrTcpSocket::AddSocketInfo(SOCKET_CREATE_INFO* pSocketInfo)
{
	if ( pSocketInfo )
	{
		m_listSocketInfo.push_back(*pSocketInfo);
	}
}

void CMultiSvrTcpSocket::AddSocketInfoList(SOCKET_CREATE_INFO_LIST& listSocketInfo)
{
	if ( listSocketInfo.size() > 0 )
	{
		m_listSocketInfo.insert(m_listSocketInfo.end(), listSocketInfo.begin(), listSocketInfo.end());
	}
}

void CMultiSvrTcpSocket::SetSocketInfoList(SOCKET_CREATE_INFO_LIST& listSocketInfo)
{
	m_listSocketInfo = listSocketInfo;
}

void CMultiSvrTcpSocket::ClearSocketInfoList()
{
	m_listSocketInfo.clear();
}

SOCKET_CREATE_INFO_LIST& CMultiSvrTcpSocket::GetSocketInfoList()
{
	return m_listSocketInfo;
}

void CMultiSvrTcpSocket::Reset()
{
	m_nCurSvrIndex = 0;
	m_nCurPortIdx = 0;
	m_nConnectState = eConnectReady;
	m_nLastLeftSize = 0;
	m_bResponse = false;
	if ( m_hSocket != INVALID_SOCKET )
	{
		Close();
	}
}

bool CMultiSvrTcpSocket::SendData(const void* lpBuf, int nBufLen, int nFlags)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch(m_nConnectState)
	{
	case eConnectReady:
	case eConnectClosed:
	case eConnectError:
		m_DataBuffer.push_back(CSendData(nBufLen, lpBuf));
		Connect();
		break;
	case eConnecting:
	case eConnectRetrying:
		m_DataBuffer.push_back(CSendData(nBufLen, lpBuf));
		break;
	case eConnectOK:
		Send(lpBuf, nBufLen, nFlags);
		break;
	}
	return true;
}

bool CMultiSvrTcpSocket::Connect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch(m_nConnectState)
	{
	case eConnectClosed:
	case eConnectError:
	case eConnectReady:
		{
			SOCKET_CREATE_INFO* pSocketInfo = GetSocketInfo();
			if ( !pSocketInfo )
			{
				XTrace("no more socketinfo can be use to connect");
				m_pSinkProxy->OnResponseLogin(GetEntity(), eLoginTimeout);
				return false;
			}
			if ( m_hSocket != INVALID_SOCKET )
			{
				CAsyncSocket::Close();
			}
			if ( !CAsyncSocket::Create() )
			{
				XTrace(FormatErrorMessage(GetLastError()).c_str());
				return false;
			}
			//设置缓存大小
			int nOptLen = sizeof(int);
			if ( pSocketInfo->nSndBufSize != 0 )
				SetSockOpt(SO_SNDBUF, &pSocketInfo->nSndBufSize, nOptLen);
			if ( pSocketInfo->nRcvBufSize != 0 )
				SetSockOpt(SO_RCVBUF, &pSocketInfo->nRcvBufSize, nOptLen);

			CAsyncSocket::Connect(pSocketInfo->szHostAddr, pSocketInfo->uPorts[m_nCurPortIdx]);
			XTrace("Connect to %s:%d", pSocketInfo->szHostAddr, pSocketInfo->uPorts[m_nCurPortIdx]);
			if ( GetLastError() != WSAEWOULDBLOCK )
			{
				{
					XTrace(FormatErrorMessage(GetLastError()).c_str());
					return false;
				}
			}
		}
		break;
	case eConnecting:
	case eConnectRetrying:
		XTrace("Socket is connecting or retrying, connect command ignored");
		break;
	case eConnectOK:
		XTrace("Socket is connected already, connect command ignored");
		break;
	}
	return true;
}

//第一次发送数据（＝登陆）超时会走到这里
void CMultiSvrTcpSocket::OnTimer(DWORD dwHandle)
{
	//目的是转发到OnClose处理流程
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	XTrace("Socket send some data, but recv no response, time out, now close socket");
	CAsyncSocket::Close();
	OnConnect(WSAETIMEDOUT);
}

void CMultiSvrTcpSocket::OnReceive(int nErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LogText("OnReceive(ErrorCode=%d, %s)\n", nErrorCode, FormatErrorMessage(nErrorCode).c_str());
	m_bFirstSend = false;
	m_bResponse = true;
	CTimerMgr::Get()->UnRegist(m_dwTimerHandle);
	if ( nErrorCode == 0 )
	{
		ReceiveData();
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CMultiSvrTcpSocket::OnConnect(int nErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LogText("OnConnect(%s, ErrorCode=%d, %s)\n", GetSocketInfo()->szHostAddr, nErrorCode, FormatErrorMessage(nErrorCode).c_str());
	if ( nErrorCode != 0 ) //重试最多3个端口之后才决定是否连接失败
	{
		XTrace("OnConnect error, try next...");
		SocketInfoMoveNext();
		m_nConnectState = eConnectError;
		if ( !Connect() )	
		{
			OnMyConnect(nErrorCode);
		}
		else
		{
			m_nConnectState = eConnectRetrying;
		}
	}
	else
	{
		m_nConnectState = eConnectOK;
		OnMyConnect(nErrorCode);
		//连接成功，启动登陆
		Login();
		//如果有缓存的数据，发送之
		while ( m_DataBuffer.size() > 0 )
		{
			CSendData& data = m_DataBuffer.back();
			Send(data.m_byData, data.m_nLen);
			OnMySend(nErrorCode);
			XTrace("Send %d bytes.(waited send)\n", data.m_nLen);
			m_DataBuffer.pop_back();
		}
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMultiSvrTcpSocket::OnClose(int nErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( m_nConnectState != eConnectOK )
	{
		OnConnect(WSAECONNREFUSED);
	}
	else
	{
		ReceiveData();
		XTrace("OnClose(ErrorCode=%d, %s)\n", nErrorCode, FormatErrorMessage(nErrorCode).c_str());
		Close();
		m_nConnectState = eConnectClosed;
		OnMyClose(nErrorCode);
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void CMultiSvrTcpSocket::OnSend(int nErrorCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	XTrace("OnSend(ErrorCode=%d, %s)\n", nErrorCode, FormatErrorMessage(nErrorCode).c_str());
	if ( m_bFirstSend )
	{
		m_dwTimerHandle = CTimerMgr::Get()->Regist(SOCKET_TIMEOUT, this, true);
	}
	CAsyncSocket::OnSend(nErrorCode);
}

void CMultiSvrTcpSocket::SetSinkProxy(ISocketSinkProxy* pSink)
{
	m_pSinkProxy = pSink;
}

SOCKET_CREATE_INFO*	CMultiSvrTcpSocket::GetSocketInfo()
{
	int nSize = m_listSocketInfo.size();
	if ( m_nCurSvrIndex >= nSize )
		return NULL;
	SOCKET_CREATE_INFO_LIST::iterator itBegin = m_listSocketInfo.begin();
	for ( int i = 0; i < m_nCurSvrIndex; i++ )
	{
		++itBegin;
	}
	SOCKET_CREATE_INFO* pSocketInfo = &*itBegin;
	if ( strlen(pSocketInfo->szHostAddr) == 0 )
	{
		LPCSTR pstrHostAddr = GetAddrByName(pSocketInfo->szHostName);
		if ( pstrHostAddr )
		{
			strncpy(pSocketInfo->szHostAddr, pstrHostAddr, MAX_HOSTADDR - 1);
		}
	}
	return pSocketInfo;
}

bool CMultiSvrTcpSocket::SocketInfoMoveNext()
{
	int nSize = m_listSocketInfo.size();
	if ( m_nCurPortIdx < MAX_PORTS_COUNT - 1 )
	{
		m_nCurPortIdx++;
		SOCKET_CREATE_INFO* pSocketInfo = GetSocketInfo();
		if ( pSocketInfo && pSocketInfo->uPorts[m_nCurPortIdx] == 0 )
		{
			SocketInfoMoveNext();
		}
	}
	else
	{
		m_nCurSvrIndex++;
		m_nCurPortIdx = 0;
	}
	return true;
}

void CMultiSvrTcpSocket::ReceiveData()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	static BYTE byBuf[MAX_DATA_LEN];
	int nRecvLen = m_nLastLeftSize;
	m_nLastLeftSize = 0;
	//while(1)
	{
		int nLen = Receive(byBuf + nRecvLen, MAX_DATA_LEN - nRecvLen);
		if ( nLen <= 0 )
		{
			return;
		}
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
				LogText("Recv %d bytes from %s.\n", nPackLen, GetSocketInfo() ? GetSocketInfo()->szHostAddr : "");
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
}