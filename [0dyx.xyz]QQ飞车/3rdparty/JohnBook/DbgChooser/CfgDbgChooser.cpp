/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/
#include "stdafx.h"
#include "DbgChooser.h"
#include "CfgDbgChooser.h"
#include "About.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCfgDbgChooser::CCfgDbgChooser(CWnd* pParent /*=NULL*/)
    : CDialog(CCfgDbgChooser::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCfgDbgChooser)
    m_sDrWatsonCfg = _T("");
    m_sVCDbgCfg = _T("");
    m_sWinDBGCfg = _T("");
    m_sVCSevenDbgCfg = _T ( "" ) ;
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CCfgDbgChooser::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCfgDbgChooser)
    DDX_Control(pDX, IDC_WINDBGCONFIG, m_cWinDBGCfg);
    DDX_Control(pDX, IDC_VCDBGCONFIG, m_cVCDbgCfg);
    DDX_Control(pDX, IDC_DRWATSONDBGCONFIG, m_cDrWatsonCfg);
    DDX_Text(pDX, IDC_DRWATSONDBGCONFIG, m_sDrWatsonCfg);
    DDX_Text(pDX, IDC_VCDBGCONFIG, m_sVCDbgCfg);
    DDX_Text(pDX, IDC_WINDBGCONFIG, m_sWinDBGCfg);
    DDX_Text ( pDX , IDC_VCSEVENDBGCONFIG, m_sVCSevenDbgCfg ) ;
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCfgDbgChooser, CDialog)
    //{{AFX_MSG_MAP(CCfgDbgChooser)
    ON_BN_CLICKED(IDHELP, OnHelp)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCfgDbgChooser::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCfgDbgChooser::OnPaint()
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
HCURSOR CCfgDbgChooser::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}


void CCfgDbgChooser::OnOK()
{
    // Crank the data exchange.
    UpdateData ( ) ;
    CDialog::OnOK();
}

void CCfgDbgChooser::OnHelp()
{
    CAboutDlg dlg ;
    dlg.DoModal ( ) ;
}

