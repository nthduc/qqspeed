/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/

#include "stdafx.h"
#include "DbgChooser.h"
#include "DbgChooserDlg.h"
#include "About.h"
#include "INIFile.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDbgChooserDlg::CDbgChooserDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDbgChooserDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDbgChooserDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pCmdLine = NULL ;
}

void CDbgChooserDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDbgChooserDlg)
	DDX_Control(pDX, IDC_DRWATSONDEBUGGER, m_cDrWatson);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDbgChooserDlg, CDialog)
    //{{AFX_MSG_MAP(CDbgChooserDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_DONOTDEBUG, OnDoNotDebug)
    ON_BN_CLICKED(IDC_DRWATSONDEBUGGER, OnDrWatsonDebugger)
    ON_BN_CLICKED(IDC_VCDEBUGGER, OnVCDebugger)
    ON_BN_CLICKED(IDC_WINDBGDEBUGGER, OnWinDBGDebugger)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_VCSEVENDEBUGGER, OnVCSevenDebugger)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDbgChooserDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDbgChooserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

void CDbgChooserDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDbgChooserDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CDbgChooserDlg::OnDoNotDebug()
{
    ASSERT ( FALSE == IsBadReadPtr ( m_pCmdLine                 ,
                                     sizeof ( CDbgCommandLine )  ) ) ;
    // If the user does not want to debug the crash, I need to set the
    // event handle value that was passed on the command line.  The
    // operating has paused the crashed app waiting for that handle to
    // get signaled.  Of course, only do this if the event value is 
    // something other than zero.
    if ( 0 != m_pCmdLine->m_iEventNum )
    {
        VERIFY ( ::SetEvent ( LongToHandle ( m_pCmdLine->m_iEventNum)));
    }
    EndDialog ( 0 ) ;
}

void CDbgChooserDlg::OnDrWatsonDebugger()
{
    CString sStr ;
    GetDrWatsonDebugger ( sStr.GetBuffer ( MAX_PATH ) , MAX_PATH ) ;
    sStr.ReleaseBuffer ( ) ;

    HANDLE hProc = StartDebugger ( sStr ) ;
    if ( NULL != hProc )
    {
        WaitForSingleObject ( hProc , INFINITE ) ;
        VERIFY ( CloseHandle ( hProc ) ) ;
    }
}

void CDbgChooserDlg::OnVCDebugger()
{
    CString sStr ;
    GetVCDebugger ( sStr.GetBuffer ( MAX_PATH ) , MAX_PATH ) ;
    sStr.ReleaseBuffer ( ) ;

    HANDLE hProc = StartDebugger ( sStr ) ;
    if ( NULL != hProc )
    {
        WaitForSingleObject ( hProc , INFINITE ) ;
        VERIFY ( CloseHandle ( hProc ) ) ;
    }
}

DWORD WINAPI SleepAndResetProc ( LPVOID )
{
    TRACE ( "SleepAndResetProc called.\n" ) ;
    int iSleepFor = SpawnPauseWait ( ) ;
    TRACE ( "Starting sleep.\n" ) ;
    Sleep ( iSleepFor * 1000 ) ; 
    TRACE ( "Finished sleep.\n" ) ;
    SetDbgChooserAsCrashDebugger ( ) ;   
    TRACE ( "SleepAndResetProc finished.\n" ) ;
    return ( 0 ) ;
}

void CDbgChooserDlg::OnVCSevenDebugger() 
{
    CString sStr ;
    GetVCSevenDebugger ( sStr.GetBuffer ( MAX_PATH ) , MAX_PATH ) ;
    sStr.ReleaseBuffer ( ) ;
    
    // This is really annoying.  For whatever strange reason, VS.NET 
    // actually checks the AeDebug key to see if it's set as the 
    // debugger.  If it's not, it won't debug the app.  So, I'll
    // fake it out here by putting it in, starting it, and swapping
    // DbgChooser back in!
    
    SetValueAsCrashDebugger ( sStr ) ;
    
    HANDLE hProc = StartDebugger ( sStr ) ;
    
    if ( NULL != hProc )
    {
        TRACE ( "Debugger started\n" ) ;
        DWORD dwTID ;
        HANDLE hThread = CreateThread ( NULL                ,
                                        0                   ,
                                        SleepAndResetProc   ,
                                        0                   ,
                                        0                   ,
                                        &dwTID               ) ;
        TRACE ( "Background thread started\n" ) ;
        TRACE ( "Waiting on process handle.\n" ) ;
        WaitForSingleObject ( hProc , INFINITE ) ;
        TRACE ( "Process handle wait finished.\n" ) ;
        VERIFY ( CloseHandle ( hProc ) ) ;

        // In case the VS7JIT.EXE bug occurs, where RPCSS can't start
        // the second instance of VS7JIT.EXE, the process gets signaled
        // almost immediately so I need to wait on the thread to ensure
        // it still runs.
        WaitForSingleObject ( hThread , INFINITE ) ;
        if ( NULL != hThread )
        {
            TRACE ( "Closing thread handle.\n" ) ;
            VERIFY ( CloseHandle ( hThread ) ) ;
        }                                        
    }
    
    SetDbgChooserAsCrashDebugger ( ) ;
}


void CDbgChooserDlg::OnWinDBGDebugger()
{
    CString sStr ;
    GetWinDBGDebugger ( sStr.GetBuffer ( MAX_PATH ) , MAX_PATH ) ;
    sStr.ReleaseBuffer ( ) ;

    HANDLE hProc = StartDebugger ( sStr ) ;
    if ( NULL != hProc )
    {
        WaitForSingleObject ( hProc , INFINITE ) ;
        VERIFY ( CloseHandle ( hProc ) ) ;
    }
}

HANDLE CDbgChooserDlg :: StartDebugger ( LPCTSTR szFmt )
{
    ASSERT ( FALSE == IsBadReadPtr ( m_pCmdLine                 ,
                                     sizeof ( CDbgCommandLine )  ) ) ;

    HANDLE hRet = NULL ;
    
    // Fixing the bug reported by Karl Proese (great catch Karl!)
    // The bug: From TaskManager, right click on a process and choose 
    //          Debug.
    // The task manager just passes -p on the command line and does not
    // pass the -e (event.)  I was blindly passing -e 0 to MSDEV, which 
    // did not like it so MSDEV would never start debugging.  To work 
    // around the problem, I will strip the -e from the format string
    // if m_pCmdLine->m_iEventNum is 0.
    TCHAR szFinalFmt [ MAX_PATH ] ;
    if ( 0 != m_pCmdLine->m_iEventNum )
    {
        // It's a real crash, use the whole format string.
        _tcscpy ( szFinalFmt , szFmt ) ;
    }
    else
    {
        // Strip the "-e %ld" from the format.
        BOOL bWaitForDash = FALSE ;
        int iCurrReal = 0 ;
        for ( int iCurr = 0 ; _T ( '\0' ) != szFmt[ iCurr ] ; iCurr++ )
        {
            if ( ( _T ( '-' ) == szFmt[ iCurr     ] ) &&
                 ( _T ( 'e' ) == szFmt[ iCurr + 1 ] )    )
            {
                bWaitForDash = TRUE ;
                continue ;
            }
            if ( ( TRUE == bWaitForDash         ) && 
                 ( _T ( '-' ) == szFmt[ iCurr ] )    )
            {
                bWaitForDash = FALSE ;
            }
            if ( FALSE == bWaitForDash )
            {
                szFinalFmt[ iCurrReal ] = szFmt[ iCurr ] ;
                iCurrReal++ ;
            }
        }
        szFinalFmt[ iCurrReal ] = _T ( '\0' ) ;
    }

    CString sCmdLine ;
    sCmdLine.Format ( szFinalFmt                ,
                      m_pCmdLine->m_iProcessNum ,
                      m_pCmdLine->m_iEventNum    ) ;

    STARTUPINFO si ;
    memset ( &si , NULL , sizeof ( STARTUPINFO ) ) ;
    si.cb = sizeof ( STARTUPINFO ) ;

    PROCESS_INFORMATION pi ;
    memset ( &pi , NULL , sizeof ( PROCESS_INFORMATION ) ) ;

    // The only interesting thing here is that I need to let the
    // debuggers inherit my handles.  If I don't, the debugger does not
    // properly attach.
    TRACE ( "Starting debugger : %S\n" , (LPCTSTR)sCmdLine ) ;
    BOOL bRet = CreateProcess ( NULL                        ,
                                (LPTSTR)((LPCTSTR)sCmdLine) ,
                                NULL                        ,
                                NULL                        ,
                                TRUE                        ,
                                0                           ,
                                NULL                        ,
                                NULL                        ,
                                &si                         ,
                                &pi                          ) ;
    ASSERT ( TRUE == bRet ) ;
    if ( TRUE == bRet )
    {
        // Close the handles I no longer need.
        hRet = pi.hProcess ;
        VERIFY ( CloseHandle ( pi.hThread ) ) ;
        // Goodbye!
        EndDialog ( 0 ) ;
        TRACE ( "DBGCHOOSER : Debugger started according to CP\n" ) ;
    }
    else
    {
        // There was a problem spawning the debugger.  Tell the user
        // about it.
        CString sProblem ;
        sProblem.Format ( IDS_NOSPAWN , szFmt ) ;

        AfxMessageBox ( sProblem ) ;

        // Treat this just like the user decided not to debug the crash.
        OnDoNotDebug ( ) ;
    }
    return ( hRet ) ;
}

void CDbgChooserDlg::OnClose() 
{
    // Closing the dialog means I need to treat it like the user 
    // decided not to debug the problem.
    OnDoNotDebug ( ) ;
	CDialog::OnClose();
}

