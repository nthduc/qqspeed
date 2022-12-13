/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _DOCNOTIFYWND_H
#define _DOCNOTIFYWND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Used to fake a "Go" press to continue after stopping for an
// exception.
#define WM_FORCE_A_GO   (WM_USER + 313)

class CDocNotifyWnd : public CWnd
{
// Construction
public:
    CDocNotifyWnd ( );

// Attributes
public:

// Operations
public:
    BOOL Create ( CWDBGProjDoc * pDoc ) ;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDocNotifyWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CDocNotifyWnd();

    // Generated message map functions
protected:
    //{{AFX_MSG(CDocNotifyWnd)
    afx_msg LRESULT HandleExitProcessEvent ( WPARAM , LPARAM );
    afx_msg LRESULT HandleExceptionEvent ( WPARAM , LPARAM ) ;
    afx_msg LRESULT HandleCreateProcessEvent ( WPARAM , LPARAM ) ;
    afx_msg LRESULT ForceAGo ( WPARAM , LPARAM ) ;
    afx_msg LRESULT HandleThreadCreateEvent ( WPARAM , LPARAM ) ;
    afx_msg LRESULT HandleThreadExitEvent ( WPARAM , LPARAM ) ;
    afx_msg LRESULT HandleLoadDllEvent ( WPARAM , LPARAM ) ;
    afx_msg LRESULT HandleUnloadDllEvent ( WPARAM , LPARAM ) ;
    afx_msg LRESULT HandleOutputDebugStringEvent ( WPARAM , LPARAM ) ;
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected   :
    // The class name.
    CString m_sClassName ;
    // The associated document.
    CWDBGProjDoc * m_pDoc ;
};

//{{AFX_INSERT_LOCATION}}

#endif // _DOCNOTIFYWND_H
