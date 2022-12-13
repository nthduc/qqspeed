/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "WDBGProjDoc.h"
#include "SymbolsDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSymbolsDisplayDlg :: CSymbolsDisplayDlg ( LOADEDMODVECT * pModVect ,
                                           CWnd* pParent /*=NULL*/   )
    : CDialog ( CSymbolsDisplayDlg::IDD , pParent )
{
    //{{AFX_DATA_INIT(CSymbolsDisplayDlg)
        m_sMod = _T("");
        //}}AFX_DATA_INIT
    m_pModVect = pModVect ;
    m_dwSelectedBase = 0 ;
}


void CSymbolsDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSymbolsDisplayDlg)
    DDX_Control(pDX, IDC_MODCOMBO, m_cModCombo);
        DDX_CBString(pDX, IDC_MODCOMBO, m_sMod);
        //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSymbolsDisplayDlg, CDialog)
    //{{AFX_MSG_MAP(CSymbolsDisplayDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSymbolsDisplayDlg :: OnOK ( )
{
    UpdateData ( ) ;

    int iCurrSel = m_cModCombo.GetCurSel ( ) ;
    ASSERT ( CB_ERR != iCurrSel ) ;

    m_dwSelectedBase = (LPCVOID)m_cModCombo.GetItemData ( iCurrSel ) ;

    CDialog::OnOK ( ) ;
}

BOOL CSymbolsDisplayDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Fill the combo box.
    LOADEDMODVECT::iterator i ;

    for ( i =  m_pModVect->begin ( ) ;
          i != m_pModVect->end ( )   ;
          i++                         )
    {
        int iSlot = m_cModCombo.AddString ( i->m_szBaseName ) ;
        ASSERT ( iSlot != CB_ERR ) ;
        m_cModCombo.SetItemData ( iSlot , (DWORD_PTR)i->m_dwBaseAddr ) ;
    }

    m_cModCombo.SetCurSel ( 0 ) ;

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
