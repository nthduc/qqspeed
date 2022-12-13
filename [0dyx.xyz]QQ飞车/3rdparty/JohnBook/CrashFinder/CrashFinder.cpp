/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "CrashFinder.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "CrashFinderDoc.h"
#include "CrashFinderTreeView.h"
#include "About.h"
#include "AppOptionsDlg.h"

#include "SymbolEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

BEGIN_MESSAGE_MAP ( CCrashFinderApp , CWinApp )
  ON_COMMAND ( ID_APP_ABOUT     , OnAppAbout    )
  ON_COMMAND ( ID_VIEW_OPTIONS  , OnViewOptions )
  ON_COMMAND ( ID_FILE_NEW      , CWinApp::OnFileNew )
  ON_COMMAND ( ID_FILE_OPEN     , CWinApp::OnFileOpen )
END_MESSAGE_MAP ( )

CCrashFinderApp :: CCrashFinderApp ( )
                 : CWinApp ( )
{
    m_bShowFullPaths    = FALSE ;
    m_bConfirmDeletions = TRUE  ;
}

CCrashFinderApp theApp ;

BOOL CCrashFinderApp :: InitInstance ( )
{

#if _MSC_VER < 1300
#ifdef _AFXDLL
    // Call this when using MFC in a shared DLL
    Enable3dControls ( ) ;
#else
    // Call this when linking to MFC statically
    Enable3dControlsStatic ( ) ;
#endif
#endif

    SetRegistryKey ( _T ( "MSJ Bugslayer Column" ) ) ;

    // Load standard INI file options (including MRU)
    LoadStdProfileSettings ( ) ;

    // Load the global state.
    RestoreGlobalState ( ) ;

    // Register the application's document templates.  Document
    // templates serve as the connection between documents, frame
    // windows and views.

    CMultiDocTemplate * pDocTemplate ;
    pDocTemplate =
        new CMultiDocTemplate ( IDR_CRASHFTYPE                        ,
                                RUNTIME_CLASS ( CCrashFinderDoc     ) ,
                                RUNTIME_CLASS ( CChildFrame         ) ,
                                RUNTIME_CLASS ( CCrashFinderTreeView) );
    AddDocTemplate ( pDocTemplate ) ;

    // Hook up to handle the filetypes!
    RegisterShellFileTypes ( ) ;
    EnableShellOpen ( ) ;

    // Create main MDI Frame window
    CMainFrame * pMainFrame = new CMainFrame ;
    if ( !pMainFrame->LoadFrame ( IDR_MAINFRAME ) )
    {
        return ( FALSE ) ;
    }
    m_pMainWnd = pMainFrame ;

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo ;
    ParseCommandLine ( cmdInfo ) ;

    // Knock off the empty file at startup if there was not one passed
    // on the command line.
    if ( CCommandLineInfo::FileNew == cmdInfo.m_nShellCommand )
    {
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing ;
    }

    // Dispatch commands specified on the command line
    if ( !ProcessShellCommand ( cmdInfo ) )
    {
        return ( FALSE ) ;
    }

    // The main window has been initialized, so show and update it.
    pMainFrame->ShowWindow ( m_nCmdShow ) ;
    pMainFrame->UpdateWindow ( ) ;

    return ( TRUE ) ;
}

int CCrashFinderApp :: ExitInstance ( )
{
    ASSERT ( this ) ;
    SaveGlobalState ( ) ;
    return ( CWinApp::ExitInstance ( ) ) ;
}


// App command to run the dialog
void CCrashFinderApp :: OnAppAbout ( )
{
    CAboutDlg aboutDlg ;
    aboutDlg.DoModal ( ) ;
}

void CCrashFinderApp :: OnViewOptions ( )
{
    CAppOptionsDlg cOptDlg ;

    // Set up to show the current state.
    cOptDlg.m_bConfirmDeletions = m_bConfirmDeletions ;
    cOptDlg.m_bShowFullPaths    = m_bShowFullPaths    ;

    if ( IDOK == cOptDlg.DoModal ( ) )
    {
        // The confirmation option is not one that needs to have
        // all the documents notified so just change it here.  The
        // documents look at this when needed.
        m_bConfirmDeletions = cOptDlg.m_bConfirmDeletions ;

        // Only do the full notifications of all open documents if the
        // full paths really did change.
        if ( m_bShowFullPaths != cOptDlg.m_bShowFullPaths )
        {
            m_bShowFullPaths    = cOptDlg.m_bShowFullPaths    ;

            // Loop through the document templates and have them loop
            // through their specific documents.
            CDocTemplate * pDocTemplate ;
            POSITION posTemplate = GetFirstDocTemplatePosition ( ) ;
            while ( NULL != posTemplate )
            {
                pDocTemplate = GetNextDocTemplate ( posTemplate ) ;
                ASSERT ( NULL != pDocTemplate ) ;

                POSITION posDoc = pDocTemplate->GetFirstDocPosition ( );
                CCrashFinderDoc * pCFDoc ;
                while ( NULL != posDoc )
                {
                    pCFDoc =
                       (CCrashFinderDoc *)pDocTemplate->
                                                    GetNextDoc(posDoc) ;
                    ASSERT ( NULL != pCFDoc ) ;

                    pCFDoc->ShowFullPaths ( m_bShowFullPaths ) ;
                }
            }
        }
    }
}

BOOL CCrashFinderApp :: ConfirmDeletions ( void ) const
{
    ASSERT ( this ) ;
    return ( m_bConfirmDeletions ) ;
}

BOOL CCrashFinderApp :: ShowFullPaths ( void ) const
{
    ASSERT ( this ) ;
    return ( m_bShowFullPaths ) ;
}

// Saves the global application state.
void CCrashFinderApp :: SaveGlobalState ( void )
{
    CString sVersion ;

    VERIFY ( sVersion.LoadString ( IDS_VERSIONREGKEY ) ) ;

    WriteProfileInt ( sVersion                  ,
                      _T ( "ShowFullPaths" )    ,
                      m_bShowFullPaths           ) ;

    WriteProfileInt ( sVersion                  ,
                      _T ( "ConfirmDeletions" ) ,
                      m_bConfirmDeletions        ) ;

    WriteProfileInt ( sVersion                     ,
                      _T ("IncludeAddress")        ,
                      m_bAddress                 );
    WriteProfileInt ( sVersion                     ,
                      _T ("IncludeModule")         ,
                      m_bModule                  );
    WriteProfileInt ( sVersion                     ,
                      _T ("IncludeFunction")       ,
                      m_bFunction                );
    WriteProfileInt ( sVersion                     ,
                      _T ("IncludeFnDisplacement") ,
                      m_bFnDisplacement          );
    WriteProfileInt ( sVersion                     ,
                      _T ("IncludeSourceFile")     ,
                      m_bSourceFile              );
    WriteProfileInt ( sVersion                     ,
                      _T ("IncludeSourceLine")     ,
                      m_bSourceLine              );
    WriteProfileInt ( sVersion                     ,
                      _T ("IncludeSrcDisplacement"),
                      m_bSrcDisplacement         );
    WriteProfileInt ( sVersion                     ,
                      _T ("OneLine"),
                      m_bOneLine         );
}

// Restored the global application state.
void CCrashFinderApp :: RestoreGlobalState ( void )
{
    CString sVersion ;

    VERIFY ( sVersion.LoadString ( IDS_VERSIONREGKEY ) ) ;

    m_bShowFullPaths = (BOOL)GetProfileInt ( sVersion               ,
                                             _T ( "ShowFullPaths" ) ,
                                             FALSE                   ) ;
    m_bConfirmDeletions = (BOOL)GetProfileInt ( sVersion               ,
                                                _T ("ConfirmDeletions"),
                                                TRUE                    );

    m_bAddress = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("IncludeAddress"),
                                                TRUE                  );
    m_bModule = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("IncludeModule"),
                                                TRUE                 );
    m_bFunction = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("IncludeFunction"),
                                                TRUE                   );
    m_bFnDisplacement = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("IncludeFnDisplacement"),
                                                TRUE                         );
    m_bSourceFile = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("IncludeSourceFile"),
                                                TRUE                     );
    m_bSourceLine = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("IncludeSourceLine"),
                                                TRUE                     );
    m_bSrcDisplacement = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("IncludeSrcDisplacement"),
                                                TRUE                          );
    m_bOneLine = (BOOL)GetProfileInt ( sVersion                   ,
                                                _T ("OneLine"),
                                                FALSE                          );
}

