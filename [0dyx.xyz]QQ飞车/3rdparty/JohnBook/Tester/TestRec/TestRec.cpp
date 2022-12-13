/*----------------------------------------------------------------------
Debugging .NET and Windows Applications
Copyright (c) 1997-2002 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "TestRec.h"

#include "MainFrm.h"
#include "TestRecDoc.h"
#include "TestRecView.h"
#include "About.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////
// CTestRecApp

BEGIN_MESSAGE_MAP(CTestRecApp, CWinApp)
    //{{AFX_MSG_MAP(CTestRecApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////
// CTestRecApp construction

CTestRecApp::CTestRecApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

////////////////////////////////////////////////////////////////////////
// The one and only CTestRecApp object

CTestRecApp theApp;

////////////////////////////////////////////////////////////////////////
// CTestRecApp initialization

BOOL CTestRecApp::InitInstance()
{
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey ( _T ( "Bugslayer" ) ) ;

    // Load standard INI file options (including MRU)
    LoadStdProfileSettings ( 8 ) ;

    // Register the application's document templates.  Document
    // templates serve as the connection between documents, frame
    // windows and views.

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CTestRecDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CTestRecView));
    AddDocTemplate(pDocTemplate);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // The one and only window has been initialized, so show and update
    // it.
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}


////////////////////////////////////////////////////////////////////////
// CTestRecApp message handlers

// App command to run the dialog
void CTestRecApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

