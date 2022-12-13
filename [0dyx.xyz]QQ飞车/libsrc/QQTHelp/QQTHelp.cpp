// QQTHelp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QQTHelp.h"
#include "ExceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CQQTHelpApp

BEGIN_MESSAGE_MAP(CQQTHelpApp, CWinApp)
	//{{AFX_MSG_MAP(CQQTHelpApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQTHelpApp construction

CQQTHelpApp::CQQTHelpApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQQTHelpApp object

CQQTHelpApp theApp;

extern "C"  LONG WINAPI QQExceptionFilter(_EXCEPTION_POINTERS *ExceptionInfo )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	MessageBeep(MB_ICONASTERISK);

	CExceptionDlg Dlg;
	Dlg.m_ExceptionInfo = ExceptionInfo;
	Dlg.DoModal();
	
	if (Dlg.m_bSendError) // 发送错误报告
	{
	}

	//if (Dlg.m_bRestartBQQ)
	{
		//因game一机只能启一个大厅，删除时间2003/05/14
		/*
		
		//重启QQ Game
		CHAR szModule[MAX_PATH];
		GetModuleFileName(NULL,szModule,MAX_PATH);
		ShellExecute(NULL, _T("OPEN"), szModule, NULL, NULL, SW_SHOWNORMAL);
		*/
	}	

	return EXCEPTION_EXECUTE_HANDLER;
}