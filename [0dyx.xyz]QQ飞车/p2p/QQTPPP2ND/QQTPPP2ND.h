// QQTPPP2ND.h : main header file for the QQTPPP2ND DLL
//

#if !defined(AFX_QQTPPP2ND_H__2A9B39AB_BCE9_431C_89FB_EB0B72F03C86__INCLUDED_)
#define AFX_QQTPPP2ND_H__2A9B39AB_BCE9_431C_89FB_EB0B72F03C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
//#define _ENABLE_QQT_LOG_
#include "QQTHelper.h"
#include "LogFile.h"

/////////////////////////////////////////////////////////////////////////////
// CQQTPPP2NDApp
// See QQTPPP2ND.cpp for the implementation of this class
//

extern int g_statInterval;
extern int g_outputstat;

void RecvDataLog(int buflen);
void SendDataLog(int buflen);

class CQQTPPP2NDApp : public CWinApp
{
public:
	CQQTPPP2NDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQTPPP2NDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CQQTPPP2NDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQTPPP2ND_H__2A9B39AB_BCE9_431C_89FB_EB0B72F03C86__INCLUDED_)
