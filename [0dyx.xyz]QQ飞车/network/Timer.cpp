#include "StdAfx.h"
#include ".\timer.h"
#include "common.h"
/*
BEGIN_MESSAGE_MAP(CTimer, CWnd)
	//{{AFX_MSG_MAP(CTimer)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTimer::CTimer(void):
m_uTime(0),
m_nIDEvent(100)
{
//	CreateEx(0, NULL, NULL, WS_CHILD | WS_POPUP, 0, 0, 0, 0, NULL, NULL);
	CreateEx(0,AfxRegisterWndClass(0,NULL,NULL,NULL),NULL,WS_POPUP,0,0,0,0,NULL,NULL);
}

CTimer::~CTimer(void)
{
	KillTimer();
}

void CTimer::SetTimer(UINT uElapse)
{
	KillTimer();
	m_uTime = CWnd::SetTimer(m_nIDEvent, uElapse, NULL);
	LogText("Time Set:%d.\n", uElapse);
}

void CTimer::KillTimer()
{
	if ( m_uTime )
	{
		CWnd::KillTimer(m_nIDEvent);
		m_uTime = 0;
		LogText("Time Killed.\n");
	}
}

void CTimer::OnTimer(UINT nIDEvent)
{
	KillTimer();
	OnTimeOut();
}

void CTimer::OnTimeOut()
{
}*/

CTimer::CTimer(void):
m_nIDEvent(0)
{
	m_hWnd = CreateWindow("Edit", "Timer", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
	ASSERT(m_hWnd);
}

CTimer::~CTimer(void)
{
	KillTimer();
	DestroyWindow(m_hWnd);
}

void CTimer::SetTimer(UINT uElapse)
{
	KillTimer();
	m_nIDEvent = (UINT_PTR)this;
	::SetTimer(m_hWnd, m_nIDEvent, uElapse, &CTimer::TimerProc);
	LogText("Time Set:%d.\n", uElapse);
}

void CTimer::KillTimer()
{
	if ( m_nIDEvent )
	{
		::KillTimer(m_hWnd, m_nIDEvent);
		m_nIDEvent = 0;
		LogText("Time Killed.\n");
	}
}

void CTimer::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CTimer* pThis = (CTimer*)idEvent;
	pThis->OnTimer();
}

void CTimer::OnTimer()
{
	KillTimer();
	OnTimeOut();
}

void CTimer::OnTimeOut()
{
}