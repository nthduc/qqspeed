/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _PROJECTSETTINGS_H
#define _PROJECTSETTINGS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProjectSettings : public CDialog
{
// Construction
public:
    CProjectSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CProjectSettings)
    enum { IDD = IDD_PROJECTSETTINGS };
    CString m_sCmdLineOpts;
    CString m_sEXEProgram;
    BOOL    m_bStopOnLdrBP;
    //}}AFX_DATA
    CString m_sShortName ;


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProjectSettings)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CProjectSettings)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !_PROJECTSETTINGS_H
