// loaderDlg.h : header file
//
//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES

#if !defined(AFX_LOADERDLG_H__8BF77CE5_B353_40B2_9155_6C57CF474869__INCLUDED_)
#define AFX_LOADERDLG_H__8BF77CE5_B353_40B2_9155_6C57CF474869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "hyperlink.h"
#include "FxButton.h"

/////////////////////////////////////////////////////////////////////////////
// CLoaderDlg dialog

class CLoaderDlg : public CDialog
{
// Construction
public:
	CLoaderDlg(CWnd* pParent = NULL);	// standard constructor
	~CLoaderDlg();
	
// Dialog Data
	//{{AFX_DATA(CLoaderDlg)
	enum { IDD = IDD_LOADER_DIALOG };
	CWebBrowser2	m_WebCtrl;
	CComboBox	m_ComboBox;
	BOOL	m_bWindowed;
	BOOL	m_bSaveAccount;
	CString	m_strAccount;
	CString	m_strPwd;
	int		m_nWorldIndex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HBITMAP m_hBkBitmap;
	HICON m_hIcon;
	CHyperLink	m_HyperLink1;
	CHyperLink	m_HyperLink2;
	CHyperLink	m_HyperLink3;
	int		m_nDisplayMode;

	CFxButton	m_wndButtonLogin;
	CFxButton	m_wndButtonExit;
	CFxButton	m_wndButtonReport;

	PROCESS_INFORMATION	m_pi;
	// Generated message map functions
	//{{AFX_MSG(CLoaderDlg)
	afx_msg void OnCheck1();
	afx_msg void OnButtonOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedReport();
	afx_msg void OnBnClickedButtonExit();
	afx_msg UINT OnNcHitTest(CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADERDLG_H__8BF77CE5_B353_40B2_9155_6C57CF474869__INCLUDED_)
