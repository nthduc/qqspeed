/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _TESTREC_H
#define _TESTREC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// The redirected WM_SYSCOMMAND from the frame to the active view.  This
// allows me to keep all the coordination in one place instead of
// scattering it hither and yon across the application.
#define WM_REDIRECTED_SYSCOMMAND    (WM_USER+0x73)

////////////////////////////////////////////////////////////////////////
// CTestRecApp:
// See TestRec.cpp for the implementation of this class
//

class CTestRecApp : public CWinApp
{
public:
    CTestRecApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTestRecApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CTestRecApp)
    afx_msg void OnAppAbout();
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !_TESTREC_H

