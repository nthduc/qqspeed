// vfstool.h : main header file for the VFSTOOL application
//

#if !defined(AFX_VFSTOOL_H__1BEF7B5C_9BF8_45E5_B989_32205DA71809__INCLUDED_)
#define AFX_VFSTOOL_H__1BEF7B5C_9BF8_45E5_B989_32205DA71809__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVfstoolApp:
// See vfstool.cpp for the implementation of this class
//

class CVfstoolApp : public CWinApp
{
public:
	CVfstoolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVfstoolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVfstoolApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenFolder();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VFSTOOL_H__1BEF7B5C_9BF8_45E5_B989_32205DA71809__INCLUDED_)
