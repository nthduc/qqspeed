#if !defined(AFX_PINGMGR_H__18EEDAB8_7E9A_4560_805E_7EEB4003C951__INCLUDED_)
#define AFX_PINGMGR_H__18EEDAB8_7E9A_4560_805E_7EEB4003C951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PingMgr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPingMgr command target
#include "IPingMgr.h"
#include "IPingObject.h"
#include <afxtempl.h>

class CPingMgr : public CWnd
{
	DECLARE_DYNCREATE(CPingMgr)
	DECLARE_OLECREATE(CPingMgr)

	CPingMgr();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	afx_msg void OnTimer(UINT nIDEvent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPingMgr)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPingMgr();

	// Generated message map functions
	//{{AFX_MSG(CPingMgr)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(PingMgr, IPingMgr)
	INIT_INTERFACE_PART(CPingMgr, PingMgr)
	    STDMETHOD(PingServer)(unsigned long ulSvrIP, unsigned short usPort, int *pnIndex, int nLimitTime);
	    STDMETHOD(CancelPing)(int nIndex);
	    STDMETHOD(Initialize)(IPingSink *pPingSink);	
	END_INTERFACE_PART(PingMgr)
	DECLARE_INTERFACE_MAP()

	STDMETHOD(PingServer)(unsigned long ulSvrIP, unsigned short usPort, int *nIndex, int nLimitTime);
	STDMETHOD(CancelPing)(int nIndex);
	STDMETHOD(Initialize)(IPingSink *pPingSink);	

	virtual void OnFinalRelease()
	{
		delete this;
	}
public:
	static int s_nIndex;
	int m_nLimitTime;

	IPingSink *m_pPingSink;

	CArray<IPingObject *, IPingObject *> m_PingArray;     // icmp
	CArray<IPingObject *, IPingObject *> m_UdpPingArray;  // udp

	void CheckCanDeleteObjects();
	void CheckPingResult();
	void DeleteCanDeleteObjects();

	void StartTimer();
	void EndTimer();
	BOOL m_bIsTimerRunning;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINGMGR_H__18EEDAB8_7E9A_4560_805E_7EEB4003C951__INCLUDED_)
