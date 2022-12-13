/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/
#include "stdafx.h"
#include "DbgChooser.h"
#include "DbgChooserDlg.h"
#include "CfgDbgChooser.h"
#include "DbgCommandLine.h"
#include "INIFile.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CDbgChooserApp, CWinApp)
    //{{AFX_MSG_MAP(CDbgChooserApp)
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


CDbgChooserApp::CDbgChooserApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

CDbgChooserApp theApp;

BOOL CDbgChooserApp::InitInstance()
{
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    // Parse up the command line.
    CDbgCommandLine cmdInfo ;
    ParseCommandLine(cmdInfo);

    // If the command line was empty (no -e, -p), allow the user to
    // configure the options.
    if ( ( 0 == cmdInfo.m_iEventNum   ) &&
         ( 0 == cmdInfo.m_iProcessNum )    )
    {
        CCfgDbgChooser dlg ;
        m_pMainWnd = &dlg ;

        // Get the current settings.

        GetVCDebugger ( dlg.m_sVCDbgCfg.GetBuffer ( MAX_PATH + 1 ) ,
                        MAX_PATH                                    ) ;
        GetWinDBGDebugger ( dlg.m_sWinDBGCfg.GetBuffer ( MAX_PATH + 1 ),
                            MAX_PATH                                  );
        GetDrWatsonDebugger ( dlg.m_sDrWatsonCfg.GetBuffer (MAX_PATH+1),
                              MAX_PATH                                );
        GetVCSevenDebugger ( dlg.m_sVCSevenDbgCfg.GetBuffer(MAX_PATH+1),
                             MAX_PATH                                 );                            

        dlg.m_sVCDbgCfg.ReleaseBuffer ( ) ;
        dlg.m_sWinDBGCfg.ReleaseBuffer ( ) ;
        dlg.m_sDrWatsonCfg.ReleaseBuffer ( ) ;
        dlg.m_sVCSevenDbgCfg.ReleaseBuffer ( ) ;
        if ( IDOK == dlg.DoModal ( ) )
        {
            SetVCDebugger ( dlg.m_sVCDbgCfg ) ;
            SetWinDBGDebugger ( dlg.m_sWinDBGCfg ) ;
            SetDrWatsonDebugger ( dlg.m_sDrWatsonCfg ) ;
            SetVCSevenDebugger ( dlg.m_sVCSevenDbgCfg ) ;

            // Set the AeDebug key to point to DbgChooser.exe.
            SetDbgChooserAsCrashDebugger ( ) ;
        }
    }
    else
    {
        // This is a crash, show the chooser dialog.
        CDbgChooserDlg dlg ;
        m_pMainWnd = &dlg ;
        dlg.m_pCmdLine = &cmdInfo ;

        dlg.DoModal ( ) ;
    }
    return FALSE;
}
