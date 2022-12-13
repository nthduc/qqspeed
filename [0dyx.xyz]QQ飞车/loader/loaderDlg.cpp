// loaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "loader.h"
#include "loaderDlg.h"
#include "network/userdata.h"
#include "loaderdef.h"
#include ".\loaderdlg.h"
#include "bmprgn.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void MD5HashString(const BYTE* Src, int HashType, BYTE* Dest) //MD5_DIGEST_LENGTH
{
	switch(HashType)
	{
	case 0:
		{
			BYTE byHash1[MD5_DIGEST_LENGTH];
			Md5HashBuffer(byHash1, Src, byteslen(Src, MD5_DIGEST_LENGTH));
			Md5HashBuffer(Dest, byHash1, MD5_DIGEST_LENGTH);		
		}
		break;
	case 1:
		Md5HashBuffer(Dest, Src, MD5_DIGEST_LENGTH);
		break;
	case 2:
		::memcpy(Dest, Src, MD5_DIGEST_LENGTH);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLoaderDlg dialog

CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
{
	char strCurrentDir[MAX_PATH] = "";
	FullName("../", strCurrentDir);
	SetCurrentDirectory(strCurrentDir);
	char strClientProfile[MAX_PATH] = "";
	_fullpath(strClientProfile, "client.ini", MAX_PATH);
	BOOL nSaveUin = GetPrivateProfileInt("Settings", "SaveUin", 0, strClientProfile);
	m_bWindowed = !GetPrivateProfileInt("Settings", "FullScreen", 0, strClientProfile);
	m_nDisplayMode = GetPrivateProfileInt("Settings", "DisplayMode", 0, strClientProfile);
	int nWorld = GetPrivateProfileInt("Settings", "WorldID", 1, strClientProfile);

	if ( !m_bWindowed )
		WritePrivateProfileString("Settings", "FullScreen", "0", strClientProfile);

	char strAccount[40] = "";
	CUserDataProfile kProfile;
	int nCount = kProfile.GetUserCount();
	if ( nCount > 0 )
	{
		UINT Uin = kProfile.GetUserData(nCount - 1).Uin;
		_snprintf(strAccount, 40, "%d", Uin);
	}
	ZeroMemory(&m_pi, sizeof(m_pi));

	//{{AFX_DATA_INIT(CLoaderDlg)
	m_bSaveAccount = nSaveUin;
	m_strAccount = strAccount;
	m_strPwd = _T("");
	m_nWorldIndex = nWorld - 1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hBkBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
}

CLoaderDlg::~CLoaderDlg()
{
	char strClientProfile[MAX_PATH] = "";
	_fullpath(strClientProfile, "client.ini", MAX_PATH);
	char strSaveUin[10] = "";
	sprintf(strSaveUin, "%d", m_bSaveAccount);
	WritePrivateProfileString("Settings", "SaveUin", strSaveUin, strClientProfile);	
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoaderDlg)
	DDX_Control(pDX, IDC_BUTTON_OK, m_wndButtonLogin);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_wndButtonExit);
	DDX_Control(pDX, IDC_REPORT, m_wndButtonReport);
	DDX_Control(pDX, IDC_EXPLORER1, m_WebCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Check(pDX, IDC_CHECK1, m_bSaveAccount);
	DDX_Check(pDX, IDC_CHECK2, m_bWindowed);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strAccount);
	DDV_MaxChars(pDX, m_strAccount, 10);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDV_MaxChars(pDX, m_strPwd, 16);
	DDX_Radio(pDX, IDC_RADIO, m_nWorldIndex);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CLoaderDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_COPYDATA()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_REPORT, OnBnClickedReport)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnBnClickedButtonExit)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoaderDlg message handlers

BOOL CLoaderDlg::OnInitDialog()
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
	HBITMAP hMaskBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if ( hMaskBmp )
	{
		BITMAP bm;
		GetObject(hMaskBmp, sizeof(bm), &bm);
		HRGN hRgn = BitmapToRegion(hMaskBmp, 0xff00ff);
		if ( hRgn )
			SetWindowRgn(hRgn, TRUE);
	}
	DeleteObject(hMaskBmp);

	char strProfile[MAX_PATH] = "";
	char strApp[20] = "";
	char strDesc[MAX_PATH] = "";
	_fullpath(strProfile, "WorldDef.ini", MAX_PATH);
	for ( int i = 0; i < 4; i++ )
	{
		sprintf(strApp, "World%d", i + 1);
		GetPrivateProfileString(strApp, "desc", "", strDesc, MAX_PATH, strProfile);
		if ( strlen(strDesc) == 0 )
			break;
		GetDlgItem(IDC_RADIO + i)->SetWindowText(strDesc);
		GetDlgItem(IDC_RADIO + i)->ShowWindow(SW_SHOW);
	}
		
	char strGetPwdPage[MAX_PATH] = "";
	char strRegistPage[MAX_PATH] = "";
	char strClientProfile[MAX_PATH] = "";
	_fullpath(strClientProfile, "client.ini", MAX_PATH);
	GetPrivateProfileString("app", "getpwdpage", "https://account.qq.com/", strGetPwdPage, MAX_PATH, strClientProfile);
	GetPrivateProfileString("app", "registpage", "http://freeqqm.qq.com/ ", strRegistPage, MAX_PATH, strClientProfile);

	m_HyperLink1.SubclassDlgItem(IDC_STATIC_GETPWD, this);
	m_HyperLink1.SetURL(strGetPwdPage);
	m_HyperLink1.SetUnderline(TRUE);
	m_HyperLink2.SubclassDlgItem(IDC_STATIC_REGIST, this);
	m_HyperLink2.SetURL(strRegistPage);
	m_HyperLink2.SetUnderline(TRUE);

	m_ComboBox.AddString("800*600(32位色)");
	m_ComboBox.AddString("1024*768(32位色)");
	m_ComboBox.SetCurSel(m_nDisplayMode);

	char strStartPage[MAX_PATH] = "";
	_fullpath(strStartPage, "home/index.htm", MAX_PATH);
	m_WebCtrl.Navigate2(&CComVariant(strStartPage), NULL, NULL, NULL, NULL);

	this->SetDefID(IDC_BUTTON_OK);
	m_wndButtonLogin.SetBkImage("loader/登录");
	m_wndButtonExit.SetBkImage("loader/退出");
	m_wndButtonReport.SetBkImage("loader/bug提交");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoaderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoaderDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();	
}

void CLoaderDlg::OnButtonOk()
{
	UpdateData();

	for ( int i = 0; i < m_strAccount.GetLength(); i++ )
	{
		char ch = m_strAccount[i];
		if ( ch < '0' || ch > '9' )
		{
			MessageBox("请输入正确的帐号和密码", NULL, MB_OK | MB_ICONHAND);
			return;
		}
	}
	UINT nAccount = atoi(m_strAccount);
	if ( nAccount == 0 || m_strPwd.GetLength() == 0 )
	{
		MessageBox("请输入正确的帐号和密码", NULL, MB_OK | MB_ICONHAND);
		return;
	}

	GetDlgItem(IDC_BUTTON_OK)->EnableWindow(FALSE);

	BYTE byPwd[MD5_DIGEST_LENGTH] = "";
	BYTE byPwdHash[MD5_DIGEST_LENGTH] = "";
	strncpy((char*)byPwd, (LPCSTR)m_strPwd, min(MD5_DIGEST_LENGTH, m_strPwd.GetLength()));
	MD5HashString(byPwd, 0, byPwdHash);

	SharedData kData;
	ZeroMemory(&kData, sizeof(kData));
	kData.hWnd = m_hWnd;
	kData.dwUin = nAccount;
	memcpy(kData.byPwd, byPwdHash, MD5_DIGEST_LENGTH);
	kData.dwWorldID = m_nWorldIndex + 1;

	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, PAGE_SIZE, KART_MAP);
	if ( !hFileMap )
	{
		MessageBox("共享内存建立失败，无法启动客户端", NULL, MB_ICONHAND | MB_OK);
		return;
	}
	LPVOID pMapData = MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if ( !pMapData )
	{
		CloseHandle(hFileMap);
		MessageBox("共享内存映射失败，无法启动客户端", NULL, MB_ICONHAND | MB_OK);
		return;
	}
	memcpy(pMapData, &kData, sizeof(kData));
	
	char strGameApp[MAX_PATH] = "";
	FullName("GameApp.exe", strGameApp);

	m_nDisplayMode = m_ComboBox.GetCurSel();
	char strProfile[MAX_PATH] = "";
	_fullpath(strProfile, "client.ini", MAX_PATH);
	char strDisplayMode[10] = "";
	sprintf(strDisplayMode, "%d", m_nDisplayMode);
	WritePrivateProfileString("Settings", "DisplayMode", strDisplayMode, strProfile);
	char strWorld[10] = "";
	sprintf(strWorld, "%d", m_nWorldIndex + 1);
	WritePrivateProfileString("Settings", "Worldid", strWorld, strProfile);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	BOOL bCreate = CreateProcess(NULL, strGameApp, NULL, NULL, TRUE, 0, NULL, NULL, &si, &m_pi);
	if ( !bCreate )
	{
		MessageBox("客户端文件损坏，无法启动！", NULL, MB_OK | MB_ICONHAND);
	}
}

BOOL CLoaderDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	switch(pCopyDataStruct->dwData)
	{
	case EDT_NotifyLoginResult:
		{
			NotifyLoginResult* pLoginResult = (NotifyLoginResult*)pCopyDataStruct->lpData;
			LPCSTR pstrMsg = NULL;
			switch(pLoginResult->nLoginResult)
			{
			case eLoginSuccessed:
				break;
			case eLoginFailed:
				pstrMsg = "登录失败，请确认用户名密码是否正确";
				break;
			case eLoginTimeout:
				pstrMsg = "对不起，登录超时，请确认网络状况";
				break;
			case eLoginRequireReg:
				break;
			case eLoginCanceled:
				pstrMsg = "登录被取消";
				break;
			case eLoginIdentityLimit:
				pstrMsg = "由于身份限制，你无法进行登录";
				break;
			case eLoginVersionLimit:
				pstrMsg = "由于版本限制，你无法进行登录";
				break;
			case eLoginVersionRecommend:
				break;
			case eLoginFileCracked:
				pstrMsg = "客户端文件损坏，无法登录";
				break;
			}
			if ( pLoginResult->dwEntity == eGameServer && strlen(pLoginResult->strErrorMessage) > 0 )
			{
				pstrMsg = pLoginResult->strErrorMessage;
			}
			if ( pstrMsg )
			{
				MessageBox(pstrMsg, NULL, MB_OK | MB_ICONHAND);
				TerminateProcess(m_pi.hProcess, 0);
				GetDlgItem(IDC_BUTTON_OK)->EnableWindow(TRUE);
			}
			else
			{
				if ( pLoginResult->dwEntity == eGameServer && pLoginResult->hWnd )
				{
					::ShowWindow(pLoginResult->hWnd, SW_SHOW);
					if ( !m_bWindowed )
					{
						char strClientProfile[MAX_PATH] = "";
						_fullpath(strClientProfile, "client.ini", MAX_PATH);
						WritePrivateProfileString("Settings", "FullScreen", "1", strClientProfile);
						::SendMessage(pLoginResult->hWnd, WM_KEYDOWN, (WPARAM)VK_F7, 0x00410001);
					}
					EndDialog(0);
				}
			}
		}
		break;
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CLoaderDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	TerminateProcess(m_pi.hProcess, 0);
	CDialog::OnClose();
}

void CLoaderDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CLoaderDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	RECT rc;
	pDC->GetWindow()->GetClientRect(&rc);

	HDC hMemDC = CreateCompatibleDC(pDC->m_hDC);
	SelectObject(hMemDC, m_hBkBitmap);

	BitBlt(pDC->m_hDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		hMemDC, 0, 0, SRCCOPY);

	DeleteObject(hMemDC);
	return TRUE;
	
//	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CLoaderDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if ( nCtlColor == CTLCOLOR_STATIC )  
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CLoaderDlg::OnBnClickedReport()
{
	// TODO: Add your control notification handler code here
	LPCSTR pszURL = "http://www.npc6.com/qqspeed_book/";
	ShellExecute(NULL, _T("open"), pszURL, NULL,NULL, SW_SHOW);
}

void CLoaderDlg::OnBnClickedButtonExit()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

UINT CLoaderDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialog::OnNcHitTest(point);

//	return CDialog::OnNcHitTest(point);
}
