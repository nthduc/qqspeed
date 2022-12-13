/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _MEMORYDISPLAYDLG_H
#define _MEMORYDISPLAYDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMemoryDisplayDlg : public CDialog
{
// Construction
public:
    CMemoryDisplayDlg(CWnd* pParent = NULL);

    CWDBGProjDoc * m_pDoc ;
    LPCVOID        m_dwConvertedAddr ;
// Dialog Data
    //{{AFX_DATA(CMemoryDisplayDlg)
    enum { IDD = IDD_MEMTODISPLAY };
    CString m_sAddress;
    UINT    m_uiLen;
    //}}AFX_DATA
    int     m_iWidth ;


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMemoryDisplayDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMemoryDisplayDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnByte();
    afx_msg void OnDword();
    afx_msg void OnWord();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _MEMORYDISPLAYDLG_H


