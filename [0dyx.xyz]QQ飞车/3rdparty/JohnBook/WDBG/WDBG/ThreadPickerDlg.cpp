/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "WDBGProjDoc.h"
#include "ThreadPickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThreadPickerDlg :: CThreadPickerDlg ( DBGTHREADVECT * pThreadVect ,
                                       LPCTSTR         szTitle     ,
                                       CWnd *          pParent      )
                  : CDialog ( CThreadPickerDlg::IDD , pParent )
{
    //{{AFX_DATA_INIT(CThreadPickerDlg)
    m_sThreadId = _T("");
    //}}AFX_DATA_INIT
    m_pThreadVect = pThreadVect ;
    ASSERT ( NULL != m_pThreadVect ) ;
    m_hSelectedThread = INVALID_HANDLE_VALUE ;
    m_sTitle = szTitle ;
}


void CThreadPickerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CThreadPickerDlg)
    DDX_Control(pDX, IDC_THREADCOMBO, m_cThreadCombo);
    DDX_CBString(pDX, IDC_THREADCOMBO, m_sThreadId);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThreadPickerDlg, CDialog)
    //{{AFX_MSG_MAP(CThreadPickerDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CThreadPickerDlg::OnOK()
{
    UpdateData ( ) ;
    int iCurrSel = m_cThreadCombo.GetCurSel ( ) ;
    ASSERT ( CB_ERR != iCurrSel ) ;

    m_hSelectedThread = (HANDLE)m_cThreadCombo.GetItemData ( iCurrSel );

    CDialog::OnOK();
}

BOOL CThreadPickerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString sStr ;

    // Fill the combo box.
    DBGTHREADVECT::iterator i ;
    for ( i  = m_pThreadVect->begin ( )  ;
          i != m_pThreadVect->end ( )    ;
          i++                             )
    {
        sStr.Format ( _T ( "0x%08X" ) , i->m_dwTID ) ;
        int iSlot = m_cThreadCombo.AddString ( sStr ) ;
        ASSERT ( CB_ERR != iSlot ) ;
        m_cThreadCombo.SetItemData ( iSlot , (DWORD_PTR)i->m_hThread ) ;
    }
    m_cThreadCombo.SetCurSel ( 0 ) ;

    // Set the dialog's title.
    SetWindowText ( m_sTitle ) ;

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
