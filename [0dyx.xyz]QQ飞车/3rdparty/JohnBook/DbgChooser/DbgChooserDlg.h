/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/
#ifndef _DBGCHOOSERDLG_H
#define _DBGCHOOSERDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DbgCommandLine.h"

class CDbgChooserDlg : public CDialog
{
// Construction
public:
    CDbgChooserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDbgChooserDlg)
	enum { IDD = IDD_DBGCHOOSER_DIALOG };
	CButton	m_cDrWatson;
	//}}AFX_DATA

    CDbgCommandLine * m_pCmdLine ;
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDbgChooserDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CDbgChooserDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnDoNotDebug();
    afx_msg void OnDrWatsonDebugger();
    afx_msg void OnVCDebugger();
    afx_msg void OnWinDBGDebugger();
	afx_msg void OnClose();
	afx_msg void OnVCSevenDebugger();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    HANDLE StartDebugger ( LPCTSTR szFmt ) ;
};

//{{AFX_INSERT_LOCATION}}

#endif // _DBGCHOOSERDLG_H
