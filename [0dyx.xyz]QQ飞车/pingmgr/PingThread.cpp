// PingThread.cpp: implementation of the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PingThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPingThread::CPingThread()
{
	m_hThread = 0;
}

CPingThread::~CPingThread()
{

}

void CPingThread::GetPingResult(int &nTTL, int &nRTT)
{
	m_Ping.GetPingResult(nTTL, nRTT);
}

void CPingThread::Ping(unsigned long ulIP, int nLimitTime)
{
	DWORD dwThreadID;
	m_ulSvrIP = ulIP;
	m_nLimitTime = nLimitTime;
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PingThreadFun, (LPVOID)this, 0, &dwThreadID);
}

void CPingThread::Terminal()
{
	if (m_hThread)
	{
		TerminateThread(m_hThread, 0);
		m_hThread = 0;
	}
}

BOOL CPingThread::IsFinish()
{
	if (0 == m_hThread || WaitForSingleObject(m_hThread, 0) != WAIT_TIMEOUT)
	{
		CloseHandle(m_hThread); 
		m_hThread = 0;
		return TRUE;
	}

	return FALSE;
}

BOOL CPingThread::IsValid()
{
	return 0 != m_hThread;
}

DWORD WINAPI CPingThread::PingThreadFun(LPVOID pParam)
{
	if (NULL == pParam)
	{
		ASSERT(FALSE);
		return 0;
	}
	CPingThread *pPingThread = (CPingThread *)pParam;

//	TRACE("\nstart thread");
	unsigned long hostIP = ::htonl(pPingThread->m_ulSvrIP);
	char *pIp = inet_ntoa( *(struct in_addr*)&hostIP);
	pPingThread->m_Ping.Ping(pIp, pPingThread->m_nLimitTime);
//	TRACE("\nend thread");

	return 0;
}


