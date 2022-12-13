/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "WDBGProjDoc.h"
#include "MemoryDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMemoryDisplayDlg::CMemoryDisplayDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMemoryDisplayDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMemoryDisplayDlg)
    m_sAddress = _T("");
    m_uiLen = 100;
    //}}AFX_DATA_INIT
    m_iWidth = 4 ;
    m_pDoc = NULL ;
}


void CMemoryDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMemoryDisplayDlg)
    DDX_Text(pDX, IDC_ADDRESS, m_sAddress);
    DDX_Text(pDX, IDC_LENGTH, m_uiLen);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemoryDisplayDlg, CDialog)
    //{{AFX_MSG_MAP(CMemoryDisplayDlg)
    ON_BN_CLICKED(IDC_BYTE, OnByte)
    ON_BN_CLICKED(IDC_DWORD, OnDword)
    ON_BN_CLICKED(IDC_WORD, OnWord)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMemoryDisplayDlg::OnOK()
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

BOOL CMemoryDisplayDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    UINT uiCtrl ;
    switch ( m_iWidth )
    {
        case 1  :
            uiCtrl = IDC_BYTE ;
            break ;
        case 2  :
            uiCtrl = IDC_WORD ;
            break ;
        case 4  :
            uiCtrl = IDC_DWORD ;
            break ;
        default :
            ASSERT ( !"Invalid width" ) ;
            uiCtrl = IDC_DWORD ;
            break ;
    }

    CButton * pButton = (CButton*)GetDlgItem ( uiCtrl ) ;
    ASSERT ( NULL != pButton ) ;
    pButton->SetCheck ( 1 ) ;
    return ( TRUE ) ;
}

void CMemoryDisplayDlg::OnByte()
{
    m_iWidth = 1 ;
}

void CMemoryDisplayDlg::OnDword()
{
    m_iWidth = 4 ;
}

void CMemoryDisplayDlg::OnWord()
{
    m_iWidth = 2 ;
}
