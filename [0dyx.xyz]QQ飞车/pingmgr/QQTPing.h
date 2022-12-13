// QQTPing.h : main header file for the QQTPING DLL
//

#if !defined(AFX_QQTPING_H__A4B735CF_8A17_428F_A228_4D1084C8CEF6__INCLUDED_)
#define AFX_QQTPING_H__A4B735CF_8A17_428F_A228_4D1084C8CEF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQQTPingApp
// See QQTPing.cpp for the implementation of this class
//

class CQQTPingApp : public CWinApp
{
public:
	CQQTPingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQTPingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CQQTPingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQTPING_H__A4B735CF_8A17_428F_A228_4D1084C8CEF6__INCLUDED_)
