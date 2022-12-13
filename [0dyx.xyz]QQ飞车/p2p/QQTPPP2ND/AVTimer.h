#if !defined(AFX_AVTIMER_H__5B439F55_E47F_4E4F_B09E_DCDE084F7D8A__INCLUDED_)
#define AFX_AVTIMER_H__5B439F55_E47F_4E4F_B09E_DCDE084F7D8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AVTimer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAVTimer window
#include <afxtempl.h>
#include "QQTppp2ND.h"
#include "EventSupport.h"

#define AVTIEMR_ID	1

class CAVTimer : public CWnd
{
	EVENT(P2P_TIMER_EVENT, (), ());
private:
	
public:
	CAVTimer();
	virtual ~CAVTimer();
// Attributes
public:
	int		m_nInterval;
	BOOL	m_bRun;
// Operations
public:
	BOOL	Create();
	void	Close();
	void	SetTimer(int nInterval);
	void	StartTimer();
	void	StopTimer();
	BOOL	IsRunning();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAVTimer)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAVTimer)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVTIMER_H__5B439F55_E47F_4E4F_B09E_DCDE084F7D8A__INCLUDED_)
