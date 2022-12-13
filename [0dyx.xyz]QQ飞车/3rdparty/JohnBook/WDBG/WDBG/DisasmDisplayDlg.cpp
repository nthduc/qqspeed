/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "WDBGProjDoc.h"
#include "DisasmDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDisasmDisplayDlg::CDisasmDisplayDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDisasmDisplayDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDisasmDisplayDlg)
    m_sAddress = _T("");
    m_uiCount = 100;
    //}}AFX_DATA_INIT
    m_pDoc = NULL ;
    m_dwConvertedAddr = 0 ;
}

void CDisasmDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDisasmDisplayDlg)
    DDX_Text(pDX, IDC_ADDRESS, m_sAddress);
    DDX_Text(pDX, IDC_INSTRUCTIONCOUNT, m_uiCount);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDisasmDisplayDlg, CDialog)
    //{{AFX_MSG_MAP(CDisasmDisplayDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDisasmDisplayDlg::OnOK()
{
    ASSERT ( NULL != m_pDoc ) ;
    // Force the update.
    UpdateData ( ) ;
    // I need to check the address.
    if ( FALSE == m_pDoc->ConvertStringToAddr ( m_sAddress ,
                                                &m_dwConvertedAddr ) )
    {
        AfxMessageBox ( IDS_INVALIDMEMADDRESS ) ;
    }
    else
    {
        CDialog::OnOK();
    }
}
