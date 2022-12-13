#pragma once

/*
class CTimer : protected CWnd
{
public:
	CTimer(void);
	virtual ~CTimer(void);

	void SetTimer(UINT uElapse);
	void KillTimer();

	// overridable
	virtual void OnTimeOut();

protected:
	//{{AFX_MSG(CTimer)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	UINT_PTR	m_uTime;
	UINT		m_nIDEvent;
};*/

class CTimer
{
public:
	CTimer(void);
	virtual ~CTimer(void);

	void SetTimer(UINT uElapse);
	void KillTimer();
	void OnTimer();

	static VOID CALLBACK TimerProc(
		HWND hwnd,         // handle to window
		UINT uMsg,         // WM_TIMER message
		UINT_PTR idEvent,  // timer identifier
		DWORD dwTime       // current system time
		);

	// overridable
	virtual void OnTimeOut();

protected:
	HWND		m_hWnd;
	UINT_PTR	m_nIDEvent;
};
