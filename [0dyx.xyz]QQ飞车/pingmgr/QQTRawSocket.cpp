// QQTRawSocket.cpp : implementation file
//

#include "stdafx.h"
#include "QQTRawSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQQTRawSocket

void CQQTRawSocket::Create()
{
	m_RawSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	if(INVALID_SOCKET == m_RawSock)
	{
		ASSERT(FALSE);

		// 等待超时然后退出
		return;
	}
	
	CAsyncSocket::Attach(m_RawSock);
}

CQQTRawSocket::CQQTRawSocket()
{
	m_pCallBack = NULL;
	m_RawSock = -1;
}

CQQTRawSocket::~CQQTRawSocket()
{
	if (-1 != m_RawSock)
	{
		CAsyncSocket::Close();
		m_RawSock = -1;
	}
}

void CQQTRawSocket::Init(IRawSocketCallBack *pCallBack)
{
	m_pCallBack = pCallBack;
}

void CQQTRawSocket::OnReceive(int nErrorCode)
{
	m_pCallBack->OnReceive(nErrorCode);
}

void CQQTRawSocket::Close()
{
	if (-1 != m_RawSock)
	{
		CAsyncSocket::Close();
		m_RawSock = -1;
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CQQTRawSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CQQTRawSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CQQTRawSocket member functions
