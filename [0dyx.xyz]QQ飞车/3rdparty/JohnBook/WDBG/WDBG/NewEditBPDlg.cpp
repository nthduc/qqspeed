/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "WDBGProjDoc.h"
#include "NewEditBPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNewEditBPDlg :: CNewEditBPDlg ( BOOL           bNew ,
                                 CWDBGProjDoc * pDoc ,
                                 CWnd * pParent /*=NULL*/)
               : CDialog ( CNewEditBPDlg::IDD , pParent )
{
    //{{AFX_DATA_INIT(CNewEditBPDlg)
    m_sLocation = _T("");
    m_uiPassCount = 0;
    //}}AFX_DATA_INIT
    m_bNewBP = bNew ;
    m_eType = eLocation ;
    m_pDoc = pDoc ;
    m_dwAddress = 0 ;
    m_bIsUnresolved = FALSE ;
}


void CNewEditBPDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewEditBPDlg)
    DDX_Control(pDX, IDC_BPTYPE, m_cBpTypeCombo);
    DDX_Control(pDX, IDC_PASSCOUNTEDIT, m_cPassCountEdit);
    DDX_Control(pDX, IDC_PASSCOUNTLABEL, m_cPassCountLabel);
    DDX_Text(pDX, IDC_BPLOCATION, m_sLocation);
    DDX_Text(pDX, IDC_PASSCOUNTEDIT, m_uiPassCount);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewEditBPDlg, CDialog)
    //{{AFX_MSG_MAP(CNewEditBPDlg)
        ON_CBN_SELCHANGE(IDC_BPTYPE, OnSelchangeBptype)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CNewEditBPDlg :: OnInitDialog ( )
{
    CDialog::OnInitDialog ( ) ;

    // Fill in the combo box with the supported breakpoint types.
    CString sData ;
    if ( FALSE == sData.LoadString ( IDS_LOCATIONBPNAME ) )
    {
        ASSERT ( !"sData.LoadString ( IDS_LOCATIONBPNAME ) failed!" ) ;
        sData = _T ( "Location" ) ;
    }
    int iVal = m_cBpTypeCombo.AddString ( sData ) ;
    m_cBpTypeCombo.SetItemData ( iVal , eLocation ) ;


    CString sTitle ;
    if ( TRUE == m_bNewBP )
    {
        if ( FALSE == sTitle.LoadString ( IDS_NEWBPTITLE ) )
        {
            ASSERT ( !"sTitle.LoadString ( IDS_NEWBPTITLE ) failed" ) ;
            sTitle = _T ( "New" ) ;
        }
    }
    else
    {
        if ( FALSE == sTitle.LoadString ( IDS_EDITBPTITLE ) )
        {
            ASSERT ( !"sTitle.LoadString ( IDS_EDITBPTITLE ) failed" ) ;
            sTitle = _T ( "Edit" ) ;
        }
    }
    SetWindowText ( sTitle ) ;

    if ( eLocation == m_eType )
    {
        m_cPassCountLabel.EnableWindow ( FALSE ) ;
        m_cPassCountEdit.EnableWindow ( FALSE ) ;
        m_cBpTypeCombo.SetCurSel ( 0 ) ;
    }
    else
    {
        // Put any additional BP types here.
    }

    return ( TRUE ) ;
}

void CNewEditBPDlg::OnSelchangeBptype()
{
    int iSel = m_cBpTypeCombo.GetCurSel ( ) ;
    eBreakpointType eType =
            (eBreakpointType)m_cBpTypeCombo.GetItemData ( iSel ) ;
    if ( eLocation == eType )
    {
        m_cPassCountLabel.EnableWindow ( FALSE ) ;
        m_cPassCountEdit.EnableWindow ( FALSE ) ;
    }
    else if ( eCounted == eType )
    {
        m_cPassCountLabel.EnableWindow ( TRUE ) ;
        m_cPassCountEdit.EnableWindow ( TRUE ) ;
    }

}

void CNewEditBPDlg::OnOK()
{
    UpdateData ( ) ;

    if ( m_sLocation.IsEmpty ( ) )
    {
        AfxMessageBox ( IDS_NOLOCATIONFORBP ) ;
        return ;
    }
    ASSERT ( NULL != m_pDoc ) ;
    if ( TRUE == m_pDoc->IsDebuggeeRunning ( ) )
    {
        if ( FALSE == m_pDoc->ConvertStringToAddr ( m_sLocation ,
                                                    &m_dwAddress ) )
        {
            AfxMessageBox ( IDS_BPADDRINVALID ) ;
            return ;
        }
    }
    else
    {
        m_bIsUnresolved = TRUE ;
    }

    int iSel = m_cBpTypeCombo.GetCurSel ( ) ;
    m_eType = (eBreakpointType)m_cBpTypeCombo.GetItemData ( iSel ) ;


    CDialog::OnOK();
}
