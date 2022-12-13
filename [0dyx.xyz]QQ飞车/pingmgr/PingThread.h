// PingThread.h: interface for the CPingThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINGTHREAD_H__769ADA58_6C26_4F37_8E91_69D748D85729__INCLUDED_)
#define AFX_PINGTHREAD_H__769ADA58_6C26_4F37_8E91_69D748D85729__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CPing.h"

class CPingThread
{
public:
	CPingThread();
	virtual ~CPingThread();

	void Ping(unsigned long ulIP, int nLimitTime);
	virtual BOOL IsFinish();
	virtual void Terminal(); 

	static DWORD WINAPI PingThreadFun(LPVOID pParam);
	void GetPingResult(int &nTTL, int &nRTT);

	BOOL IsValid();
	
	HANDLE m_hThread;

	CPing m_Ping;

	unsigned long m_ulSvrIP;

	int m_nLimitTime;
};

#endif // !defined(AFX_PINGTHREAD_H__769ADA58_6C26_4F37_8E91_69D748D85729__INCLUDED_)
