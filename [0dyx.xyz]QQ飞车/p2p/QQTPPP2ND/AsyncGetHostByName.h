#if !defined(AFX_ASYNCGETHOSTBYNAME_H__4C14A846_85DE_11D3_9A52_0080C8E1743A__INCLUDED_)
#define AFX_ASYNCGETHOSTBYNAME_H__4C14A846_85DE_11D3_9A52_0080C8E1743A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// AsyncGetHostByName.h : header file
//

#include "winsock.h"

//#include "CallBackEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CAsyncGetHostByName window

class CAsyncGetHostByName : public CWnd
{

// Construction
public:
	//typedef void (CObject::*TOnHostGet)(BOOL bSuccess, CString strFailReason, CString strIP);
	//CAsyncGetHostByName(CObject* pobj,  TOnHostGet pfunc, const CString strAddr);
	//CAsyncGetHostByName(CObject* pobj,  TOnHostGet pfunc, const CStringArray* pstraAddr);
	CAsyncGetHostByName(HWND hWnd,  UINT nMsg, const CString strAddr,int nSvrType = -1,int nSvrIndex = -1);
	CAsyncGetHostByName(HWND hWnd,  UINT nMsg, const CStringArray* pstraAddr);
	virtual ~CAsyncGetHostByName();

	typedef struct 
	{
		BOOL bSuccess; 
    	CString strFailReason;
    	CString strIP;
		int  nSvrType;
		int  nSvrIndex;
	} TOnHostGet;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsyncGetHostByName)
	//}}AFX_VIRTUAL

// Implementation
public:


	// Generated message map functions
protected:
	//{{AFX_MSG(CAsyncGetHostByName)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	afx_msg void OnGetHostByNameReturn(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


	HWND m_hCallerWnd;
	UINT m_nCallerMsg;

	//Add by Link:
	int m_nSvrType;
	int m_nSvrIndex;

	

	//CCallBackEvent OnHostGot;
	char m_arAsyncGetHostByNameBuf[MAXGETHOSTSTRUCT]; //WSAAsyncGetHostByNameµƒ¡Ÿ ±ª∫≥Â

	CStringArray m_straHostNeedToResolve; 
	void GetHostByName();

	CString m_strLastError;
	void DoEvent(BOOL bSuccess, CString strFailReason, CString strIP);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNCGETHOSTBYNAME_H__4C14A846_85DE_11D3_9A52_0080C8E1743A__INCLUDED_)
