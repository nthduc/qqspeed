// IcmpPingObject.cpp: implementation of the CIcmpPingObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IcmpPingObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIcmpPingObject::CIcmpPingObject()
{
	m_nSeq = 0;

	m_nIndex = 0;
	m_nStartTime = 0;

	m_nPingCount = 0;
	m_nReplyCount = 0;
	m_nTotalRTT = 0;
	m_nTotalTTL = 0;

	m_bAllThreadObjectHasReturn = FALSE;
}

CIcmpPingObject::~CIcmpPingObject()
{
	while (m_PingThreads.GetSize())
	{
		CPingThread *pPingThread = m_PingThreads.GetAt(0);
		// 强制结束线程
		if (pPingThread)
		{
			pPingThread->Terminal();
			delete pPingThread;	
		}

		m_PingThreads.RemoveAt(0);
	}
}

void CIcmpPingObject::Ping(unsigned int ulSvrIP, unsigned short usSvrPort, int nPingCount, int nStartTime, int nLimitTime)
{
	m_ulSvrIP = ulSvrIP;
	m_usSvrPort = usSvrPort;
	m_nStartTime = nStartTime;
	m_nPingCount = nPingCount;
	m_nLimitTime = nLimitTime;

	StartPings();
}

BOOL CIcmpPingObject::CanDelete()
{
	return m_bAllThreadObjectHasReturn;
}

// 检测是否所有的ping thread  都执行完了
void CIcmpPingObject::CheckCanDelete()
{
	BOOL bHasThread = FALSE;
	for (int i = 0; i < m_PingThreads.GetSize(); ++i)
	{
		CPingThread *pPingThread = m_PingThreads.GetAt(i);

		if (pPingThread->IsFinish())
		{
			m_nReplyCount ++;
			int nTTL; int nRTT;
			pPingThread->GetPingResult(nTTL, nRTT);
			m_nTotalTTL += nTTL;
			m_nTotalRTT += nRTT;
			delete pPingThread;
			m_PingThreads.RemoveAt(i);
			--i;
		}
		else
		{
			bHasThread = TRUE;
		}
	}
	m_bAllThreadObjectHasReturn = !bHasThread;
}

// ping 多次求丢包率
void CIcmpPingObject::StartPings()
{
	unsigned long ip = ::htonl(m_ulSvrIP);
	TRACE("startpings ip = %s\n", inet_ntoa(*(struct in_addr*)&ip));

	for (int i = 0; i < m_nPingCount; ++i)
	{
		TRACE("ping count = %d, index = %d\n", i, m_nIndex);
		CPingThread *pPingThread = new CPingThread;
		if (pPingThread)
		{
			pPingThread->Ping(m_ulSvrIP, m_nLimitTime);
			m_PingThreads.Add(pPingThread);
		}
	}
}

void CIcmpPingObject::CancelPing()
{
	m_bIsCanceled = TRUE;
}

void CIcmpPingObject::GetPingResult(int &nTTL, float &fLossRate, int &nAverageRTT)
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

/*
unsigned short CIcmpPingObject::CheckSum(unsigned short *pBuffer, int nBufferSize) 
{ 	
	unsigned long unCheckSum = 0; 

	while (nBufferSize > 1) 
	{ 
		unCheckSum += *pBuffer; 
		nBufferSize -= sizeof(unsigned short); 
		pBuffer ++;
	} 
		
	if (nBufferSize)
	{ 
		unCheckSum += *(unsigned char *)pBuffer; 
	} 
		
	unCheckSum = (unCheckSum >> 16) + (unCheckSum & 0xffff); 
	unCheckSum += (unCheckSum >> 16); 
	return (unsigned short)(~unCheckSum); 
}

  

#pragma  pack(4)
void CIcmpPingObject::EncodePingMsg(char *pBuffer)
{
	if (NULL == pBuffer)
	{
		ASSERT(FALSE);
		return;
	}

	QQT_PING_MSG *pPingMsg = (QQT_PING_MSG *)pBuffer;
	pPingMsg->m_ICMPHeader.i_code = 0;
	pPingMsg->m_ICMPHeader.i_id = (unsigned short) GetCurrentProcessId();
	pPingMsg->m_ICMPHeader.i_cksum = 0;
	pPingMsg->m_ICMPHeader.i_type = ICMP_ECHO;
	pPingMsg->m_ICMPHeader.i_seq = m_nSeq++;
	pPingMsg->m_unTimeStamp = GetTickCount();
	pPingMsg->m_nSocketIndex = m_nIndex;
	pPingMsg->m_ICMPHeader.i_cksum = CheckSum((unsigned short *)pPingMsg, sizeof(QQT_PING_MSG));

	TRACE("seq = %d, timestamp = %d, checksum = %d\n", pPingMsg->m_ICMPHeader.i_seq, pPingMsg->m_unTimeStamp, pPingMsg->m_ICMPHeader.i_cksum);
}

BOOL CIcmpPingObject::DecodePingMsg(char *pBuffer, int nBytes, int *pRTT, unsigned char *pTTL)
{
	IpHeader *pIPHeader; 
	QQT_PING_MSG *pPingMsg;

	if (NULL == pBuffer || NULL == pRTT)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (nBytes < sizeof(IpHeader))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pIPHeader = (IpHeader *)pBuffer;
	unsigned short nIPHeaderLength = pIPHeader->h_len * 4;

	if (int(nIPHeaderLength + sizeof(QQT_PING_MSG)) > nBytes)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pPingMsg = (QQT_PING_MSG *)(pBuffer + nIPHeaderLength);

	if (ICMP_ECHOREPLY != pPingMsg->m_ICMPHeader.i_type)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (pPingMsg->m_ICMPHeader.i_id != (unsigned short) GetCurrentProcessId())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if (pPingMsg->m_nSocketIndex != m_nIndex)
	{
		return FALSE;
	}

	if (m_nReplyCount >= m_nPingCount)
	{
		return FALSE;
	}

	*pRTT = GetTickCount() - pPingMsg->m_unTimeStamp;
	*pTTL = pIPHeader->ttl;

	TRACE("	reply ttl = %d, pretimestamp = %d, timestamp = %d, rtt = %d, source ip = %s, %d, dest ip = %s, %u \n",
		pIPHeader->ttl,
		pPingMsg->m_unTimeStamp,
		GetTickCount(),
		*pRTT,
		inet_ntoa( *((struct in_addr *)&pIPHeader->sourceIP)), 
		pIPHeader->sourceIP,
		inet_ntoa( *((struct in_addr *)&pIPHeader->destIP)),
		pIPHeader->destIP
		);

	return TRUE;
}

  
void CIcmpPingObject::OnReceive(int nErrorCode)
{
	sockaddr_in sockaddr;
	char buf[4096];
	int nlen = sizeof(sockaddr);
	int nRead = m_Socket.ReceiveFrom(buf, sizeof(buf), (SOCKADDR*)&sockaddr, &nlen);
	
	if (SOCKET_ERROR == nRead)
	{
		TRACE("OnReceive(), nRead = SOCKET_ERROR");
		
		if (WSAEWOULDBLOCK != nRead)
		{
			m_Socket.Close();
			return;
		}
	}

	int nRTT = 0;
	unsigned char cTTL = 0;
	if (DecodePingMsg(buf, nRead, &nRTT, &cTTL))
	{
		m_nTotalRTT += nRTT;
		m_nTotalTTL += cTTL;
		m_nReplyCount++;
		TRACE("++m_nReplyCount = %d\n, index = %d\n", m_nReplyCount, m_nIndex);
	}
}

*/
