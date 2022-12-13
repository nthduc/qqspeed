#ifndef __TIMERMGR_H__
#define __TIMERMGR_H__

#include "singleT.h"
#include "idgen.h"
#include "map"
using namespace std;

#define MIN_TIMER_INTERVAL	1000
interface ITimerEvent
{
	virtual void OnTimer(DWORD dwHandle) = NULL;
};

typedef struct tagTIMERDATA
{
	DWORD	dwOriginTime;
	DWORD	dwInterval;
	ITimerEvent*	pTimerEvent;
	bool	bSmartTimer;
	bool	bExpired;
}TIMERDATA;
typedef map<DWORD, TIMERDATA>	TIMERDATA_LIST;

class CTimerMgr : public CSingleT<CTimerMgr>, public CIDGen
{
public:
	CTimerMgr():
	m_dwTimerCount(0),
	m_bTimerRunning(false)
	{
		m_hWnd = CreateWindow("Edit", "Timer", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
		m_nIDEvent = (DWORD)this;
	}

	~CTimerMgr()
	{
		if ( m_hWnd )
		{
			StopTimer();
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
	}

	DWORD Regist(DWORD dwMilliseconds, ITimerEvent* pTimerEvent, bool bSmartTimer = false)
	{
		TIMERDATA kData;
		kData.dwOriginTime = m_dwTimerCount;
		kData.dwInterval = dwMilliseconds / MIN_TIMER_INTERVAL;
		kData.pTimerEvent = pTimerEvent;
		kData.bSmartTimer = bSmartTimer;
		kData.bExpired = false;
		DWORD dwHandle = Alloc();
		m_listTimerData[dwHandle] = kData;
		StartTimer();
		return dwHandle;
	}
	void UnRegist(DWORD dwHandle)
	{
		TIMERDATA_LIST::iterator it = m_listTimerData.find(dwHandle);
		if ( it != m_listTimerData.end() )
		{
			m_listTimerData.erase(it);
		}
		if ( m_listTimerData.size() == 0 )
		{
			StopTimer();
		}
	}

	void OnTimer()
	{
		m_dwTimerCount++;
		TIMERDATA_LIST::iterator it = m_listTimerData.begin();
		for ( ; it != m_listTimerData.end(); ++it )
		{
			TIMERDATA& kData = it->second;
			if ( !kData.bExpired && kData.dwOriginTime + kData.dwInterval < m_dwTimerCount )
			{
				kData.bExpired = true;
				if ( kData.pTimerEvent )
				{
					kData.pTimerEvent->OnTimer(it->first);
				}
			}
		}
	}

	static VOID CALLBACK TimerProc(
		HWND hwnd,         // handle to window
		UINT uMsg,         // WM_TIMER message
		UINT_PTR idEvent,  // timer identifier
		DWORD dwTime       // current system time
		)
	{
		CTimerMgr* pThis = (CTimerMgr*)idEvent;
		pThis->OnTimer();
	}

protected:
	bool StartTimer()
	{
		if ( m_bTimerRunning )
			return false;
		::SetTimer(m_hWnd, m_nIDEvent, MIN_TIMER_INTERVAL, &CTimerMgr::TimerProc);
		m_bTimerRunning = true;
	}

	bool StopTimer()
	{
		if ( !m_bTimerRunning )
			return false;
		::KillTimer(m_hWnd, m_nIDEvent);
		m_bTimerRunning = false;
		return true;
	}

	void SweepGarbage()
	{
		TIMERDATA_LIST::iterator it = m_listTimerData.begin();
		for ( ; it != m_listTimerData.end(); ++it )
		{
			TIMERDATA& kData = it->second;
			if ( kData.bExpired )
			{
				m_listTimerData.erase(it);
				break;
			}
		}
	}

private:
	HWND	m_hWnd;
	UINT	m_nIDEvent;
	DWORD	m_dwTimerCount;
	bool	m_bTimerRunning;
	TIMERDATA_LIST	m_listTimerData;
};

class CTimerEventBase : public ITimerEvent
{
public:
	CTimerEventBase():
    m_dwTimerHandle(0)
	{
	}

protected:
	DWORD	m_dwTimerHandle;
};
#endif