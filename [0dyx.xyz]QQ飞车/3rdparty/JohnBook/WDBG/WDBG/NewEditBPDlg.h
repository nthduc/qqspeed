/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _NEWEDITBPDLG_H
#define _NEWEDITBPDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNewEditBPDlg : public CDialog
{
// Construction
public:
    CNewEditBPDlg ( BOOL           bNewBP ,
                    CWDBGProjDoc * pDoc ,
                    CWnd *         pParent = NULL ) ;

// Dialog Data
    //{{AFX_DATA(CNewEditBPDlg)
    enum { IDD = IDD_NEWEDITBP };
    CComboBox   m_cBpTypeCombo;
    CEdit   m_cPassCountEdit;
    CStatic m_cPassCountLabel;
    CString m_sLocation;
    UINT    m_uiPassCount;
    //}}AFX_DATA
    eBreakpointType m_eType ;
    LPCVOID m_dwAddress ;
    BOOL m_bIsUnresolved ;


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewEditBPDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // The type of BP.
    BOOL m_bNewBP ;

    // The document.
    CWDBGProjDoc * m_pDoc ;

    // Generated message map functions
    //{{AFX_MSG(CNewEditBPDlg)
    virtual BOOL OnInitDialog();
        afx_msg void OnSelchangeBptype();
        virtual void OnOK();
        //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !_NEWEDITBPDLG_H
