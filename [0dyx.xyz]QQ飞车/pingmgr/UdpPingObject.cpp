// UpdPingOjbect.cpp : implementation file
//

#include "stdafx.h"
#include "UdpPingObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUdpPingObject

CUdpPingObject::CUdpPingObject()
{
	m_nSeq = 0;

	m_nIndex = 0;
	m_nStartTime = 0;

	m_nPingCount = 0;
	m_nReplyCount = 0;
	m_nTotalRTT = 0;
	m_nTotalTTL = 0;

	Close();
}

CUdpPingObject::~CUdpPingObject()
{
}

void CUdpPingObject::CancelPing()
{
	m_bIsCanceled = TRUE;
}

void CUdpPingObject::GetPingResult(int &nTTL, float &fLossRate, int &nAverageRTT)
{
	if (0 == m_nPingCount || 0 == m_nReplyCount)
	{
		nTTL = 0;
		fLossRate = 1.0;
		nAverageRTT = 0;
		return;
	}

	nTTL = m_nTotalTTL / m_nReplyCount;
	nAverageRTT = m_nTotalRTT / m_nReplyCount;
	fLossRate = 1.0f - (float)m_nReplyCount / m_nPingCount;
}

void CUdpPingObject::Ping(unsigned int ulSvrIP, unsigned short usSvrPort, int nPingCount, int nStartTime, int nLimitTime)
{
	m_ulSvrIP = ulSvrIP;
	m_usSvrPort = usSvrPort;

	m_nStartTime = nStartTime;
	m_nPingCount = nPingCount;

	StartPings();	
}


void CUdpPingObject::StartPings()
{
	struct sockaddr_in stAddr;

 	memset(&stAddr, 0, sizeof(stAddr));
 	stAddr.sin_family = AF_INET;
	stAddr.sin_addr.s_addr = ::htonl(m_ulSvrIP);
	stAddr.sin_port = ntohs(m_usSvrPort);

	// GameServer 目前只使用icmp 所以屏蔽掉所有的Udp ping
/*	for (int i = 0; i < m_nPingCount; ++i)
	{
		//无需做网络字序转换，数据只用于本机
		UDPPING_DATA stPing;
		stPing.dwCmd = UDPPING_CMD;
		stPing.dwTimeStamp = GetTickCount();
		stPing.ushSeq = ++m_nSeq;
		stPing.dwFillData = 0;

		if(SOCKET_ERROR == SendTo(&stPing, sizeof(stPing), (struct sockaddr*)&stAddr, sizeof(stAddr)) )
		{
			if (GetLastError() == WSAEWOULDBLOCK)
				return;
			TRACE("Send Udp Ping Fail :%d\n", GetLastError());
		}
	}
*/
}

void CUdpPingObject::OnReceive(int nErrorCode)
{
	sockaddr_in sockaddr;
	char buf[4096];
	int nRead = ReceiveFrom(buf, sizeof(buf), (SOCKADDR*)&sockaddr, 0);

	if (SOCKET_ERROR == nRead)
	{
		if ( (WSAEWOULDBLOCK != GetLastError()) && (WSAECONNRESET != GetLastError()) )
		{
			Close();
		}
		return;
	}

	if (sizeof(UDPPING_DATA) != nRead)
	{
		return;
	}

	UDPPING_DATA *pData = (UDPPING_DATA *)buf;

	m_nTotalRTT += timeGetTime() - pData->dwTimeStamp;
	m_nTotalTTL += 0;
	m_nReplyCount++;
}

BOOL CUdpPingObject::CanDelete()
{
	// udp 超时就可以删除
	return timeGetTime() - m_nStartTime >= m_nLimitTime;
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUdpPingObject, CAsyncSocket)
	//{{AFX_MSG_MAP(CUdpPingObject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUdpPingObject member functions
