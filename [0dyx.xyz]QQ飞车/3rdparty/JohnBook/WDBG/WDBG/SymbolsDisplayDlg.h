/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _SYMBOLSDISPLAY_H
#define _SYMBOLSDISPLAY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSymbolsDisplayDlg : public CDialog
{
// Construction
public:
    CSymbolsDisplayDlg ( LOADEDMODVECT * pModVect ,
                         CWnd* pParent = NULL ) ;

// Dialog Data
    //{{AFX_DATA(CSymbolsDisplayDlg)
    enum { IDD = IDD_SYMTODISPLAY };
    CComboBox   m_cModCombo;
    CString m_sMod;
    //}}AFX_DATA
    LPCVOID m_dwSelectedBase ;


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSymbolsDisplayDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:

    LOADEDMODVECT * m_pModVect ;

    // Generated message map functions
    //{{AFX_MSG(CSymbolsDisplayDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _SYMBOLSDISPLAY_H
