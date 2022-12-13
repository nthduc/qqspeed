/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _WDBGCHILDFRAME_H
#define _WDBGCHILDFRAME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWDBGChildFrame : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CWDBGChildFrame)
protected:
    CWDBGChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:
    void DisableCloseButton ( void ) ;
    void EnableCloseButton ( void ) ;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWDBGChildFrame)
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CWDBGChildFrame();

    // Generated message map functions
    //{{AFX_MSG(CWDBGChildFrame)
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _WDBGCHILDFRAME_H
