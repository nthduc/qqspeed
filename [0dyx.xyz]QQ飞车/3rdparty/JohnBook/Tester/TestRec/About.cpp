/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "About.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CAboutDlg :: OnInitDialog ( void )
{
    // Let the normal processing happen.
    CDialog::OnInitDialog ( ) ;

    m_cWebLink.SubclassDlgItem ( IDC_HYPERLINK , this ) ;

    m_cMSDNLink.SubclassDlgItem ( IDC_MSDNLINK , this ) ;
    m_cMSDNLink.m_link = _T ( "http:\\\\mspress.microsoft.com" ) ;

    // Do the big icon.
    m_cBigIcon.SubclassDlgItem ( IDC_BIGICON , this ) ;
    m_cBigIcon.SizeToContent ( ) ;


    return ( TRUE ) ;
}
