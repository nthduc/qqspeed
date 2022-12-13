/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _THREADPICKERDLG_H
#define _THREADPICKERDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CThreadPickerDlg : public CDialog
{
// Construction
public:
    CThreadPickerDlg ( DBGTHREADVECT * pThreadVect ,
                       LPCTSTR         szTitle     ,
                       CWnd *          pParent = NULL ) ;

// Dialog Data
    //{{AFX_DATA(CThreadPickerDlg)
    enum { IDD = IDD_THREADPICKER };
    CComboBox   m_cThreadCombo;
    CString     m_sThreadId;
    //}}AFX_DATA
    HANDLE m_hSelectedThread ;


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CThreadPickerDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:

    DBGTHREADVECT * m_pThreadVect ;
    CString         m_sTitle ;

    // Generated message map functions
    //{{AFX_MSG(CThreadPickerDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !_THREADPICKERDLG_H
