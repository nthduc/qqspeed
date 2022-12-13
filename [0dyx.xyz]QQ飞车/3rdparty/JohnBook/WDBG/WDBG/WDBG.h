/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _WDBG_H
#define _WDBG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "WDBGProjDoc.h"

class CWDBGApp : public CWinApp
{
public:
    CWDBGApp();

    void DocumentWasOpened ( CWDBGProjDoc * pDoc ) ;
    void DocumentIsClosed ( CWDBGProjDoc * pDoc ) ;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWDBGApp)
    public:
    virtual BOOL InitInstance();
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CWDBGApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileOpen();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()


protected   :
    // Only one document can be open at a time.
    CWDBGProjDoc * m_pDoc ;

private     :
    CWDBGApp ( CWDBGApp & ) { }
    CWDBGApp & operator = ( const CWDBGApp & ) { }
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _WDBG_H
