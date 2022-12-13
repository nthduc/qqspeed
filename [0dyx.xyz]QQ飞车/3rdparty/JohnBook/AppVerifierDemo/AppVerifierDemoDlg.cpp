// AppVerifierDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AppVerifierDemo.h"
#include "AppVerifierDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAppVerifierDemoDlg dialog



CAppVerifierDemoDlg::CAppVerifierDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppVerifierDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppVerifierDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_VERSIONSTATIC, CVersionEdit);
}

BEGIN_MESSAGE_MAP(CAppVerifierDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_OVERRUNBOUNDARY, OnBnClickedOverRunBoundary)
    ON_BN_CLICKED(IDC_OVERRUNNOTONBOUNDARY, OnBnClickedOverRunNotOnBoundary)
    ON_BN_CLICKED(IDC_DOUBLEFREE, OnBnClickedDoubleFree)
    ON_BN_CLICKED(IDC_ACCESSAFTERFREE, OnBnClickedAccessAfterFree)
    ON_BN_CLICKED(IDC_DBLCRITSECINIT, OnBnClickedDblCritSecInit)
    ON_BN_CLICKED(IDC_CORRUPTCRITSEC, OnBnClickedCorruptCritSec)
    ON_BN_CLICKED(IDC_EXTRACRITSECRELEASE, OnBnClickedExtraCritSecRelease)
    ON_BN_CLICKED(IDC_USEUNINITCRITSEC, OnBnClickedUseUninitCritSec)
    ON_BN_CLICKED(IDC_GETHANDLEINFO, OnBnClickedGethandleInfo)
    ON_BN_CLICKED(IDC_TLSGETVALUEPARAM, OnBnClickedTlsgetvalueparam)
    ON_BN_CLICKED(IDC_VERSIONINFO, OnBnClickedVersionInfo)
    ON_BN_CLICKED(IDC_NULLDACL, OnBnClickedNullDACL)
    ON_BN_CLICKED(IDCREATEPROC, OnBnClickedCreateProc)
END_MESSAGE_MAP()


// CAppVerifierDemoDlg message handlers

BOOL CAppVerifierDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAppVerifierDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAppVerifierDemoDlg::OnPaint()
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
HCURSOR CAppVerifierDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAppVerifierDemoDlg::OnBnClickedOverRunBoundary ( )
{
    // Allocate on a sixteen byte boundary and overrun by one byte.
    char * pChar = (char*)malloc ( 16 ) ;
    pChar[ 17 ] = '\0' ;
}

void CAppVerifierDemoDlg::OnBnClickedOverRunNotOnBoundary()
{
    char * pChar = (char*)malloc ( 10 ) ;
    pChar[ 11 ] = '\0' ;
}

void CAppVerifierDemoDlg::OnBnClickedDoubleFree()
{
    char * pChar = new char [ 10 ] ;
    delete [] pChar ;
    delete [] pChar ;
}

void CAppVerifierDemoDlg::OnBnClickedAccessAfterFree()
{
    char * pChar = new char [ 10 ] ;
    delete [] pChar ;
    strcpy ( pChar , "Ouch!" ) ;
}

static CRITICAL_SECTION cs ;
void CAppVerifierDemoDlg::OnBnClickedDblCritSecInit()
{
    InitializeCriticalSectionAndSpinCount ( &cs , 4000 ) ;
    InitializeCriticalSectionAndSpinCount ( &cs , 4000 ) ;
}

static CRITICAL_SECTION cs2 ;
void CAppVerifierDemoDlg::OnBnClickedCorruptCritSec()
{
    DeleteCriticalSection ( &cs2 ) ;
}

static CRITICAL_SECTION cs3 ;
void CAppVerifierDemoDlg::OnBnClickedExtraCritSecRelease()
{
    InitializeCriticalSectionAndSpinCount ( &cs3 , 4000 ) ;
    EnterCriticalSection ( &cs3 ) ;
    LeaveCriticalSection ( &cs3 ) ;
    LeaveCriticalSection ( &cs3 ) ;
}
void CAppVerifierDemoDlg::OnBnClickedUseUninitCritSec()
{
    CRITICAL_SECTION cs3 ;
    ZeroMemory ( &cs3 , sizeof ( CRITICAL_SECTION ) ) ;
    EnterCriticalSection ( &cs3 ) ;
    LeaveCriticalSection ( &cs3 ) ;
}

void CAppVerifierDemoDlg::OnBnClickedGethandleInfo()
{
    DWORD dwFlags ;
    GetHandleInformation ( NULL , &dwFlags ) ;
}

void CAppVerifierDemoDlg::OnBnClickedTlsgetvalueparam()
{
    TlsGetValue ( 0x666 ) ;
}

void CAppVerifierDemoDlg::OnBnClickedVersionInfo()
{
    OSVERSIONINFO stOS ;
    ZeroMemory ( &stOS , sizeof ( OSVERSIONINFO ) ) ;
    stOS.dwOSVersionInfoSize = sizeof ( OSVERSIONINFO ) ;

    GetVersionEx ( &stOS ) ;
    CString cVal ;
    
    cVal.Format ( _T ( "Version : %d.%d.%d\n" ) ,
                  stOS.dwMajorVersion           ,
                  stOS.dwMinorVersion           ,
                  stOS.dwBuildNumber               ) ;
                  
    CVersionEdit.SetWindowText ( cVal ) ;
}


void CAppVerifierDemoDlg::OnBnClickedNullDACL()
{
    SECURITY_DESCRIPTOR sd = { SECURITY_DESCRIPTOR_REVISION ,
                               0                            ,
                               SE_DACL_PRESENT              ,
                               0                            ,
                               0                            ,
                               0                            ,
                               0                             } ;
    SECURITY_ATTRIBUTES sa = { sizeof  ( SECURITY_ATTRIBUTES ) ,
                               &sd                             ,
                               TRUE                             } ;
    HANDLE hEvent = CreateEvent ( &sa              ,
                                  TRUE             ,
                                  TRUE             ,
                                  _T ( "EventO"  )  ) ;
    CloseHandle ( hEvent ) ;
                               
}

void CAppVerifierDemoDlg::OnBnClickedCreateProc()
{

    PROCESS_INFORMATION pi ;
    STARTUPINFO si ;
    memset ( &si , NULL , sizeof ( STARTUPINFO ) ) ;
    memset ( &pi , NULL , sizeof ( PROCESS_INFORMATION ) ) ;

    si.cb = sizeof ( STARTUPINFO ) ;
    TCHAR szProg [ MAX_PATH ] ;
    _tcscpy ( szProg , _T ( "notepad.exe" ) ) ;
    BOOL bRet = CreateProcess ( NULL    ,
                                szProg  ,
                                NULL    ,
                                NULL    ,
                                FALSE   ,
                                0       ,
                                NULL    ,
                                NULL    ,
                                &si     ,
                                &pi      ) ;
    if ( TRUE == bRet )
    {
        CloseHandle ( pi.hProcess ) ;
        CloseHandle ( pi.hThread ) ;
    }
}
