/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _DISASMDISPLAYDLG_H
#define _DISASMDISPLAYDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisasmDisplayDlg.h : header file
//

class CDisasmDisplayDlg : public CDialog
{
// Construction
public:
    CDisasmDisplayDlg(CWnd* pParent = NULL);

    CWDBGProjDoc * m_pDoc ;
    LPCVOID        m_dwConvertedAddr ;

// Dialog Data
    //{{AFX_DATA(CDisasmDisplayDlg)
    enum { IDD = IDD_DISASMTODISPLAY };
    CString m_sAddress;
    UINT    m_uiCount;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDisasmDisplayDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDisasmDisplayDlg)
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _DISASMDISPLAYDLG_H
