/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "ProjectSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProjectSettings::CProjectSettings(CWnd* pParent /*=NULL*/)
    : CDialog(CProjectSettings::IDD, pParent)
{
    //{{AFX_DATA_INIT(CProjectSettings)
    m_sCmdLineOpts = _T("");
    m_sEXEProgram = _T("");
    m_bStopOnLdrBP = FALSE;
    //}}AFX_DATA_INIT
}


void CProjectSettings::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProjectSettings)
    DDX_Text(pDX, IDC_CMDLINEOPTS, m_sCmdLineOpts);
    DDX_Text(pDX, IDC_EXEPROGRAM, m_sEXEProgram);
    DDX_Check(pDX, IDC_STOPONLDRBP, m_bStopOnLdrBP);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectSettings, CDialog)
    //{{AFX_MSG_MAP(CProjectSettings)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CProjectSettings::OnOK()
{
    // TODO: Add extra validation here

    CDialog::OnOK();
}

BOOL CProjectSettings::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString sTitle ;
    sTitle.Format ( IDS_PROJSETTINGSDLGTITLE , m_sShortName ) ;
    SetWindowText ( sTitle ) ;

    return TRUE;

}
