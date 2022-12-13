/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _BREAKPOINTSDLG_H
#define _BREAKPOINTSDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBreakpointsDlg : public CDialog
{
// Construction
public:
    CBreakpointsDlg ( BPARRAY *      pBpArray ,
                      CWDBGProjDoc * pDoc     ,
                      CWnd*          pParent = NULL ) ;

// Dialog Data
    //{{AFX_DATA(CBreakpointsDlg)
    enum { IDD = IDD_BREAKPOINTS };
    CCheckListBox    m_cBPList;
    CButton          m_cEditBtn;
    CButton          m_cRemoveBtn;
    //}}AFX_DATA
    BOOL             m_bModifiedList ;


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBreakpointsDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // The mast breakpoint array.
    BPARRAY * m_pBpArray ;

    // The document.
    CWDBGProjDoc * m_pDoc ;

    // Generated message map functions
    //{{AFX_MSG(CBreakpointsDlg)
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnEdit();
    afx_msg void OnNew();
    afx_msg void OnRemove();
    virtual BOOL OnInitDialog();
        afx_msg void OnSelchangeBplist();
        //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !_BREAKPOINTSDLG_H
