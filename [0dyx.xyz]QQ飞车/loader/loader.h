// loader.h : main header file for the LOADER application
//

#if !defined(AFX_LOADER_H__A98C7758_D434_41E8_B237_2F9E5108B353__INCLUDED_)
#define AFX_LOADER_H__A98C7758_D434_41E8_B237_2F9E5108B353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "External.h"

/////////////////////////////////////////////////////////////////////////////
// CLoaderApp:
// See loader.cpp for the implementation of this class
//

class CLoaderApp : public CWinApp
{
public:
	CLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLoaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CExternal m_external;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADER_H__A98C7758_D434_41E8_B237_2F9E5108B353__INCLUDED_)
