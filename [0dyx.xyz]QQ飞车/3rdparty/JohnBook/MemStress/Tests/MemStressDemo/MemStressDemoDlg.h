/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#pragma once
#include "afxwin.h"


// CMemStressDemoDlg dialog
class CMemStressDemoDlg : public CDialog
{
// Construction
public:
	CMemStressDemoDlg(CWnd* pParent = NULL);	// standard constructor
	
	virtual ~CMemStressDemoDlg ( void ) ;

// Dialog Data
	enum { IDD = IDD_MEMSTRESSDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	
    BOOL m_bMemInit ;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedInitialize();
    CButton m_btnInit;
    CButton m_btnShutdown;
    CButton m_btnGenAlloc;
    CButton m_btn100Alloc;
    CButton m_btnFileAndLine;
    afx_msg void OnBnClickedShutdown();
    afx_msg void OnBnClickedGenalloc();
    afx_msg void OnBnClicked100bytealloc();
    afx_msg void OnBnClickedFileandline();
};
