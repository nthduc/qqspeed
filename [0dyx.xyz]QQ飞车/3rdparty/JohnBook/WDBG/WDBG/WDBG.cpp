/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"

#include "MainFrm.h"
#include "WDBGChildFrame.h"
#include "WDBGProjDoc.h"
#include "WDBGLogView.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CWDBGApp, CWinApp)
    //{{AFX_MSG_MAP(CWDBGApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    // Standard print setup command
    ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////
// CWDBGApp construction

CWDBGApp::CWDBGApp()
{
    m_pDoc = NULL ;

}

////////////////////////////////////////////////////////////////////////
// The one and only CWDBGApp object

CWDBGApp theApp;

////////////////////////////////////////////////////////////////////////
// CWDBGApp initialization

BOOL CWDBGApp::InitInstance()
{
#ifdef _DEBUG
    BSUSetCurrentThreadName ( _T ( "WDBG UI Thread" ) ) ;
#endif

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey(_T("Bugslayer"));

    // Load standard INI file options (including MRU)
    LoadStdProfileSettings();

    // Register the application's document templates.  Document
    // templates serve as the connection between documents, frame
    // windows and views.

    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(
        IDR_WDBGTYPE,
        RUNTIME_CLASS(CWDBGProjDoc),
        RUNTIME_CLASS(CWDBGChildFrame), // custom MDI child frame
        RUNTIME_CLASS(CWDBGLogView));
    AddDocTemplate(pDocTemplate);

    // create main MDI Frame window
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
        return FALSE;
    m_pMainWnd = pMainFrame;

    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;

    // I don't want an empty project showing up.
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing ;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // The main window has been initialized, so show and update it.
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

    return TRUE;
}

void CWDBGApp :: DocumentWasOpened ( CWDBGProjDoc * pDoc )
{
    // If you wanted to handle multiple debuggee's at once, you could
    // associate the document with the process ID.  Since I set this
    // version up to only allow a single document at a time, this is
    // good enough.
    m_pDoc = pDoc ;
}

void CWDBGApp :: DocumentIsClosed ( CWDBGProjDoc * pDoc )
{
    pDoc = pDoc ;
    m_pDoc = NULL ;
}

// App command to run the dialog
void CWDBGApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}


void CWDBGApp::OnFileOpen()
{
    // This handles the case where the CTRL+O accelerator opens a file.
    // I want the CTRL+O when no files are open.
    if ( NULL != m_pDoc )
    {
        return ;
    }


    // My fancy implementation of File|Open that allows the user to
    // open up EXE's, DLL's, OCX's, as well as .WPJ's.

    // This is an open dialog.
    CFileDialog cDlg ( TRUE ) ;

    // Load the language title.
    CString sTitle ;
    if ( FALSE == sTitle.LoadString ( AFX_IDS_OPENFILE ) )
    {
        ASSERT ( !"sTitle.LoadString ( AFX_IDS_OPENFILE ) failed!" ) ;
        sTitle = _T ( "Open" ) ;
    }
    cDlg.m_ofn.lpstrTitle = sTitle ;

    // Start setting options.
    cDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST ;

    // Set the executable files as first in the list followed by the
    // .WPJ files.
    CString sFilter ;
    CString sTemp ;

    if ( FALSE == sTemp.LoadString ( IDS_EXEFILTER ) )
    {
        ASSERT ( !"sTemp.LoadString ( IDS_EXEFILTER ) failed!" ) ;
        sTemp = _T ( "Executable Files (*.exe;*.dll;*.ocx)" ) ;
    }
    sFilter = sTemp ;
    sFilter += _T ( '\0' ) ;

    if ( FALSE == sTemp.LoadString ( IDS_EXEEXT ) )
    {
        ASSERT ( !"sTemp.LoadString ( IDS_EXEEXT ) failed!" ) ;
        sTemp = _T ( "*.exe;*.dll;*.ocx" ) ;
    }
    sFilter += sTemp ;
    sFilter += _T ( '\0' ) ;

    // Get the document template strings.
    CDocTemplate * pTemplate ;
    CString sTemplateFilt ;
    CString sTemplateExt ;

    POSITION pos = GetFirstDocTemplatePosition ( ) ;
    while ( NULL != pos )
    {
        pTemplate = GetNextDocTemplate ( pos ) ;

        BOOL bHasAll =
            pTemplate->GetDocString ( sTemplateFilt ,
                                      CDocTemplate::filterName) &&
            pTemplate->GetDocString ( sTemplateExt ,
                                      CDocTemplate::filterExt ) &&
            !sTemplateFilt.IsEmpty ( )                          &&
            !sTemplateExt.IsEmpty ( ) ;
        if ( TRUE == bHasAll )
        {
            sFilter += sTemplateFilt ;
            sFilter += _T ( '\0' ) ;
            sFilter += _T ( '*' ) ;
            sFilter += sTemplateExt ;
            sFilter += _T ( '\0' ) ;
        }
    }

    // Now add the *.* filter.
    CString allFilter;
    if ( FALSE == allFilter.LoadString ( AFX_IDS_ALLFILTER ) )
    {
        ASSERT ( !"allFilter.LoadString ( AFX_IDS_ALLFILTER ) failed");
        allFilter = _T ( "All files" ) ;
    }
    sFilter += allFilter ;
    sFilter += _T ( '\0' ) ;
    sFilter += _T ( "*.*" ) ;
    sFilter += _T ( '\0' ) ;

    // Set the filters.
    cDlg.m_ofn.lpstrFilter = sFilter ;

    CString sFilename ;
    cDlg.m_ofn.lpstrFile = sFilename.GetBuffer ( MAX_PATH ) ;

    INT_PTR iResult = cDlg.DoModal ( ) ;
    iResult = iResult ;
    sFilename.ReleaseBuffer ( ) ;

    if ( IDOK == iResult )
    {
        OpenDocumentFile ( sFilename ) ;
    }
}

CDocument* CWDBGApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
    CDocument * pRet = CWinApp::OpenDocumentFile(lpszFileName);

    // Tile the initial windows.
    CMDIFrameWnd * pMDIFrame = (CMDIFrameWnd *)m_pMainWnd ;
    ASSERT ( NULL != pMDIFrame ) ;
    pMDIFrame->MDITile ( MDITILE_HORIZONTAL ) ;
    return ( pRet ) ;
}
