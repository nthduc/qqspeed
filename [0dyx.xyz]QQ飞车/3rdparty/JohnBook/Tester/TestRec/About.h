/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _ABOUT_H
#define _ABOUT_H

#include "Resource.h"
#include "StatLink.h"
#include "BigIcon.h"

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
        // No message handlers
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    virtual BOOL OnInitDialog ( void ) ;

    // The hyper link.
    CStaticLink m_cWebLink ;
    // The MSDN Mag link.
    CStaticLink m_cMSDNLink ;
    // The big icon control.
    CBigIcon m_cBigIcon ;


};

#endif  // !_ABOUT_H