/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "About.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

BEGIN_MESSAGE_MAP ( CAboutDlg , CDialog )
    ON_MESSAGE ( WM_CTLCOLORSTATIC , OnCtrlColorStatic )
END_MESSAGE_MAP ( )

CAboutDlg :: CAboutDlg ( void )
           : CDialog ( CAboutDlg::IDD ) ,
             m_cBigIcon ( )

{
    ASSERT ( this ) ;
    m_pcFont = NULL ;
}

CAboutDlg :: ~CAboutDlg ( void )
{
    ASSERT ( this ) ;
    if ( NULL != m_pcFont )
    {
        delete m_pcFont ;
        m_pcFont = NULL ;
    }
}

void CAboutDlg :: DoDataExchange ( CDataExchange * pDX )
{
    ASSERT ( this ) ;

    CDialog::DoDataExchange ( pDX ) ;
}

BOOL CAboutDlg :: OnInitDialog ( void )
{
    ASSERT ( this ) ;

    // Let the normal processing happen.
    CDialog::OnInitDialog ( ) ;

    // Do the big icon.
    m_cBigIcon.SubclassDlgItem ( IDC_BIGICON , this ) ;
    m_cBigIcon.SizeToContent ( ) ;

    // Create and set the name font.
    m_pcFont = new CFont ;
    BOOL bRet = m_pcFont->CreatePointFont ( 300 , _T ( "Arial" ) ) ;
    ASSERT ( bRet ) ;
    if ( TRUE == bRet )
    {
        CWnd * pWnd = GetDlgItem ( IDC_APPNAME ) ;
        ASSERT ( NULL != pWnd ) ;
        if ( NULL != pWnd )
        {
            pWnd->SetFont ( m_pcFont ) ;
        }
    }


    // Stick the hint text into the edit control.
    HRSRC hRes = ::FindResource ( NULL                             ,
                                  MAKEINTRESOURCE ( IDR_HINTDATA ) ,
                                  _T ( "HINTDATA" )                 ) ;
    ASSERT ( NULL != hRes ) ;
    if ( NULL != hRes )
    {
        LPCTSTR szData = (LPCTSTR)::LoadResource ( NULL , hRes ) ;
        ASSERT ( NULL != szData ) ;

        CWnd * pEdit = GetDlgItem ( IDC_HINTS ) ;
        ASSERT ( NULL != pEdit ) ;

        pEdit->SetWindowText ( szData ) ;

    }

    // Put the focus on the OK button.
    CWnd * pOK = GetDlgItem ( IDOK ) ;
    ASSERT ( NULL != pOK ) ;

    pOK->SetFocus ( ) ;

    return ( FALSE ) ;
}

LRESULT CAboutDlg :: OnCtrlColorStatic ( WPARAM wParam , LPARAM lParam )
{
    CWnd * pAppName = GetDlgItem ( IDC_APPNAME ) ;
    ASSERT ( NULL != pAppName ) ;

    LRESULT lRet = Default ( ) ;

    if ( (HWND)lParam == pAppName->m_hWnd )
    {
        // Set the text color to red just to be interesting.
        ::SetTextColor ( (HDC)wParam , (RGB ( 255 , 0 , 0 ) ) ) ;
    }
    return ( lRet ) ;
}

