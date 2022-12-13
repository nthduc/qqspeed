#if !defined(AFX_EXCEPTIONDLG_H__8E4BDDD9_CDDE_4B54_923D_2431EE04DFF7__INCLUDED_)
#define AFX_EXCEPTIONDLG_H__8E4BDDD9_CDDE_4B54_923D_2431EE04DFF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExceptionDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CExceptionDlg dialog

class CExceptionDlg : public CDialog
{
// Construction
public:
	CExceptionDlg(CWnd* pParent = NULL);   // standard constructor
	_EXCEPTION_POINTERS* m_ExceptionInfo;

	BOOL m_bSendError;
	
// Dialog Data
	//{{AFX_DATA(CExceptionDlg)
	enum { IDD = IDD_EXCEPTION };
	CString	m_strError;
	BOOL m_bRestartBQQ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExceptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CBitmap	m_bmpIcon;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExceptionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCEPTIONDLG_H__8E4BDDD9_CDDE_4B54_923D_2431EE04DFF7__INCLUDED_)
