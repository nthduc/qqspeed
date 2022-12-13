/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "BinaryImageOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

BEGIN_MESSAGE_MAP ( CBinaryImageOptionsDlg , CDialog )
END_MESSAGE_MAP ( )

CBinaryImageOptionsDlg :: CBinaryImageOptionsDlg ( )
           : CDialog ( CBinaryImageOptionsDlg::IDD ) ,
             m_cHexCtrl ( )
{
    ASSERT ( this ) ;
    m_dwAddr = 0 ;
}

void CBinaryImageOptionsDlg :: DoDataExchange ( CDataExchange * pDX )
{
    ASSERT ( this ) ;

    CDialog::DoDataExchange ( pDX ) ;
}

BOOL CBinaryImageOptionsDlg :: OnInitDialog ( void )
{
    ASSERT ( this ) ;
    ASSERT ( 0 != m_dwAddr ) ;

    // Subclass the edit control with my cheezy hex only edit control
    // class.
    m_cHexCtrl.SubclassDlgItem ( IDC_LOADADDREDIT , this ) ;

    CEdit * pEdit = (CEdit*)GetDlgItem ( IDC_LOADADDREDIT ) ;
    ASSERT ( NULL != pEdit ) ;

    // Format the load address into a string and stick it into the edit
    // control.
    CString sTemp ;
    sTemp.Format ( _T ( "%08X" ) , m_dwAddr ) ;

    pEdit->SetWindowText ( sTemp ) ;

    return ( TRUE ) ;
}

void CBinaryImageOptionsDlg :: OnOK ( void )
{

    ASSERT ( this ) ;

    // Get the text out of the edit control.
    TCHAR szTemp[ 25 ] ;

    CEdit * pEdit = (CEdit*)GetDlgItem ( IDC_LOADADDREDIT ) ;
    ASSERT ( NULL != pEdit ) ;

    pEdit->GetWindowText ( szTemp , sizeof ( szTemp ) ) ;

    // Try and convert it to an value.  I'm taking the easy way out.
    DWORD dwTempAddr ;
    int iRet = _stscanf ( szTemp         ,
                           _T ( "%x" )   ,
                           &dwTempAddr    ) ;

    if ( 1 != iRet )
    {
        CString sMsg ;
        sMsg.FormatMessage ( IDS_LOADADDRBADCONVERSION , szTemp ) ;
        AfxMessageBox ( sMsg ) ;
    }
    else
    {
        m_dwAddr = dwTempAddr ;
        CDialog::OnOK ( ) ;
    }
}


