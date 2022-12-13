/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "MemStressDemo.h"
#include "MemStressDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMemStressDemoDlg dialog



CMemStressDemoDlg::CMemStressDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemStressDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMemStressDemoDlg :: ~CMemStressDemoDlg ( )
{
    if ( TRUE == m_bMemInit )
    {
        MEMSTRESSTERMINATE ( ) ;
    }
}


void CMemStressDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_INITIALIZE, m_btnInit);
    DDX_Control(pDX, IDC_SHUTDOWN, m_btnShutdown);
    DDX_Control(pDX, IDC_GENALLOC, m_btnGenAlloc);
    DDX_Control(pDX, IDC_100BYTEALLOC, m_btn100Alloc);
    DDX_Control(pDX, IDC_FILEANDLINE, m_btnFileAndLine);
}

BEGIN_MESSAGE_MAP(CMemStressDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_INITIALIZE, OnBnClickedInitialize)
    ON_BN_CLICKED(IDC_SHUTDOWN, OnBnClickedShutdown)
    ON_BN_CLICKED(IDC_GENALLOC, OnBnClickedGenalloc)
    ON_BN_CLICKED(IDC_100BYTEALLOC, OnBnClicked100bytealloc)
    ON_BN_CLICKED(IDC_FILEANDLINE, OnBnClickedFileandline)
END_MESSAGE_MAP()


// CMemStressDemoDlg message handlers

BOOL CMemStressDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMemStressDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMemStressDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMemStressDemoDlg::OnBnClickedInitialize()
{
    m_bMemInit = TRUE ;
    MEMSTRESSINIT ( _T ( "MemStressDemo" ) ) ;
    m_btnInit.EnableWindow ( FALSE ) ;
    m_btnShutdown.EnableWindow ( TRUE ) ;

    m_btnGenAlloc.EnableWindow ( TRUE ) ;
    m_btnFileAndLine.EnableWindow ( TRUE ) ;
    m_btn100Alloc.EnableWindow ( TRUE ) ;
}

void CMemStressDemoDlg::OnBnClickedShutdown()
{
    m_bMemInit = FALSE ;
    MEMSTRESSTERMINATE ( ) ;

    m_btnInit.EnableWindow ( TRUE ) ;
    m_btnShutdown.EnableWindow ( FALSE ) ;

    m_btnGenAlloc.EnableWindow ( FALSE ) ;
    m_btnFileAndLine.EnableWindow ( FALSE ) ;
    m_btn100Alloc.EnableWindow ( FALSE ) ;
}

void CMemStressDemoDlg::OnBnClickedGenalloc()
{
    char * szBuff = (char*)malloc ( 10 ) ;
    if ( NULL == szBuff )
    {
        AfxMessageBox ( _T ( "10 byte allocation failed!" ) ) ;
    }
    if ( NULL != szBuff )
    {
        free ( szBuff ) ;
    }
}

void CMemStressDemoDlg::OnBnClicked100bytealloc()
{
    char * szBuff = (char*)malloc ( 100 ) ;
    if ( NULL == szBuff )
    {
        AfxMessageBox ( _T ( "100 byte allocation failed!" ) ) ;
    }
    if ( NULL != szBuff )
    {
        free ( szBuff ) ;
    }
}

void SpecificLine ( void ) ;
void CMemStressDemoDlg::OnBnClickedFileandline()
{
    SpecificLine ( ) ;
}
