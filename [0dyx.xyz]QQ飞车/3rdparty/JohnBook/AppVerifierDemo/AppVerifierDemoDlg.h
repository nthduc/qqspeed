// AppVerifierDemoDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CAppVerifierDemoDlg dialog
class CAppVerifierDemoDlg : public CDialog
{
// Construction
public:
	CAppVerifierDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_APPVERIFIERDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOverRunBoundary();
    afx_msg void OnBnClickedOverRunNotOnBoundary();
    afx_msg void OnBnClickedDoubleFree();
    afx_msg void OnBnClickedAccessAfterFree();
    afx_msg void OnBnClickedDblCritSecInit();
    afx_msg void OnBnClickedCorruptCritSec();
    afx_msg void OnBnClickedExtraCritSecRelease();
    afx_msg void OnBnClickedUseUninitCritSec();
    afx_msg void OnBnClickedGethandleInfo();
    afx_msg void OnBnClickedTlsgetvalueparam();
    CStatic CVersionEdit;
    afx_msg void OnBnClickedVersionInfo();
    afx_msg void OnBnClickedNullDACL();
    afx_msg void OnBnClickedCreateProc();
};
