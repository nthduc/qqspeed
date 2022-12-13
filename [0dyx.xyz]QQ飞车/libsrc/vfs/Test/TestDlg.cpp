// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//using namespace VFS;

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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_LIST1, m_lstOutput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TEST1, OnButtonTest1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
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

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestDlg::OnPaint() 
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
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/*
#if defined(_DEBUG)
#pragma comment(lib,"..\\Debug\\vfs_d.lib")
#else
#pragma comment(lib,"..\\Release\\vfs.lib")
#endif
*/

void CTestDlg::OnButtonTest1() 
{
	// TODO: Add your control notification handler code here

//	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr(NULL);

	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr();
//	GetTenioComponent(&pFileSysMgr, TENIO_INI);
	pFileSysMgr->Init();

	if (pFileSysMgr)
	{
	//	pFileSysMgr->MountRoot("../","data");
	//	FILE_h hfile = pFileSysMgr->OpenFile("data\\test.txt");
	//	pFileSysMgr->CloseFile(hfile);
		{
	//	ROOT_h hsrc = pFileSysMgr->OpenRoot("data",VFS_OPENROOT_MODE_WRITE_ALWAYS);
	//	pFileSysMgr->CopyFile(hsrc,"hellovfs.txt",hsrc,"hello.txt");
	//	pFileSysMgr->CloseRoot(hsrc);
		}

		ROOT_h htrg = pFileSysMgr->OpenRoot("pet2res.vfs",VFS_OPENROOT_MODE_WRITE_ALWAYS);
		if (htrg==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("test1.vfs open error");
			return ;
		}
		ROOT_h hsrc = pFileSysMgr->OpenRoot("data");
		if (hsrc==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("data dir open error!");
			pFileSysMgr->CloseRoot(htrg);
			return ;
		}

		FIND_h hfind = pFileSysMgr->FindFirstFile(hsrc,"",true);
		if (hfind!=VFS_INVALID_HANDLE_VALUE)
		{
			bool bfind;
			do 
			{
				ErrorMsg("copy %s",pFileSysMgr->GetFindPathName(hfind));
				pFileSysMgr->CopyFile(pFileSysMgr->GetFindRoot(hfind), pFileSysMgr->GetFindPathName(hfind),htrg,pFileSysMgr->GetFindPathName(hfind));
				bfind = pFileSysMgr->FindNextFile(hfind);
			}while(bfind);
		}

		pFileSysMgr->CloseRoot(htrg);
		pFileSysMgr->CloseRoot(hsrc);

	}

	//pFileSysMgr->Release();
}

void CTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here

	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr(NULL);
	pFileSysMgr->Init();

	if (pFileSysMgr)
	{
		ROOT_h htrg = pFileSysMgr->OpenRoot("data",VFS_OPENROOT_MODE_WRITE_ALWAYS);
		if (htrg==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("data dir open error!");
			return ;
		}

		ROOT_h hsrc = pFileSysMgr->OpenRoot("pet2res.vfs");
		if (hsrc==VFS_INVALID_HANDLE_VALUE)
		{
			pFileSysMgr->CloseRoot(htrg);
			ErrorMsg("test1.vfs open error!");
			return;
		}


		FIND_h hfind = pFileSysMgr->FindFirstFile(hsrc,"",true);
		if (hfind!=VFS_INVALID_HANDLE_VALUE)
		{
			bool bfind;
			do 
			{
				ErrorMsg("copy %s",pFileSysMgr->GetFindPathName(hfind));
				pFileSysMgr->CopyFile(pFileSysMgr->GetFindRoot(hfind), pFileSysMgr->GetFindPathName(hfind),htrg, pFileSysMgr->GetFindPathName(hfind));
				bfind = pFileSysMgr->FindNextFile(hfind);
			}while(bfind);
		}

		pFileSysMgr->CloseRoot(htrg);
		pFileSysMgr->CloseRoot(hsrc);

	}

	//pFileSysMgr->Release();	
}

void CTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr(NULL);
	pFileSysMgr->Init();

	if (pFileSysMgr)
	{

		ROOT_h htrg = pFileSysMgr->OpenRoot("test2.vfs",VFS_OPENROOT_MODE_WRITE_ALWAYS);
		if (htrg==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("test2.vfs open error");
			return;
		}

		ROOT_h hsrc = pFileSysMgr->OpenRoot("data",VFS_OPENROOT_MODE_WRITE_EXIST);
		if (hsrc==VFS_INVALID_HANDLE_VALUE)
		{
			pFileSysMgr->CloseRoot(htrg);
			ErrorMsg("data dir open error");
			return;
		}

		FIND_h hfind = pFileSysMgr->FindFirstFile(hsrc,"",true);
		if (hfind!=VFS_INVALID_HANDLE_VALUE)
		{
			bool bfind;
			do 
			{
				ErrorMsg("copy %s",pFileSysMgr->GetFindPathName(hfind));
				pFileSysMgr->CopyFile(pFileSysMgr->GetFindRoot(hfind), pFileSysMgr->GetFindPathName(hfind),htrg,"");
				bfind = pFileSysMgr->FindNextFile(hfind);
			}while(bfind);
		}

		pFileSysMgr->DelTree(hsrc,"");

		pFileSysMgr->CloseRoot(htrg);
		pFileSysMgr->CloseRoot(hsrc);

	}

	//pFileSysMgr->Release();	
}

void CTestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr(NULL);
	pFileSysMgr->Init();

	if (pFileSysMgr)
	{
		ROOT_h htrg = pFileSysMgr->OpenRoot("data2",VFS_OPENROOT_MODE_WRITE_ALWAYS);
		if (htrg==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("data2 dir open error");
			return;
		}
		ROOT_h hsrc = pFileSysMgr->OpenRoot("test2.vfs",VFS_OPENROOT_MODE_WRITE_EXIST);
		if (hsrc==VFS_INVALID_HANDLE_VALUE)
		{
			pFileSysMgr->CloseRoot(htrg);
			ErrorMsg("test2.vfs open error!");
			return;
		}

		FIND_h hfind = pFileSysMgr->FindFirstFile(hsrc,"",true);
		if (hfind!=VFS_INVALID_HANDLE_VALUE)
		{
			bool bfind;
			do 
			{
				ErrorMsg("copy %s",pFileSysMgr->GetFindPathName(hfind));
				pFileSysMgr->CopyFile(pFileSysMgr->GetFindRoot(hfind), pFileSysMgr->GetFindPathName(hfind),htrg,"");
				bfind = pFileSysMgr->FindNextFile(hfind);
			}while(bfind);
		}
		
		pFileSysMgr->DelTree(hsrc,"");

		pFileSysMgr->CloseRoot(htrg);
		pFileSysMgr->CloseRoot(hsrc);

	}

	//pFileSysMgr->Release();		
}

void CTestDlg::OnButton4() 
{

	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr(NULL);
	pFileSysMgr->Init();

	if (pFileSysMgr)
	{
		IFileSys* pFileSys = pFileSysMgr->GetVFileSysByExt("vfs");
		pFileSys->RegisterRWFilter("?fdt",pFileSys->GetFileRWFilter(""));
		ROOT_h htrg = pFileSysMgr->OpenRoot("testfdt.vfs",VFS_OPENROOT_MODE_WRITE_ALWAYS);
		if (htrg==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("testfdt.vfs 打开出错!");
			return ;
		}

		ROOT_h hsrc = pFileSysMgr->OpenRoot("data");
		if (hsrc==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("data 目录打开出错!");
			pFileSysMgr->CloseRoot(htrg);
			return ;
		}


		FIND_h hfind = pFileSysMgr->FindFirstFile(hsrc,"",true);
		if (hfind!=VFS_INVALID_HANDLE_VALUE)
		{
			bool bfind;
			do 
			{
				ErrorMsg("copy %s",pFileSysMgr->GetFindPathName(hfind));
				pFileSysMgr->CopyFile(pFileSysMgr->GetFindRoot(hfind), pFileSysMgr->GetFindPathName(hfind),htrg,"");
				bfind = pFileSysMgr->FindNextFile(hfind);
			}while(bfind);
		}

		pFileSysMgr->CloseRoot(htrg);
		pFileSysMgr->CloseRoot(hsrc);

		ErrorMsg("success");
	}

	//pFileSysMgr->Release();
}

void CTestDlg::OnButton5() 
{
	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr();
	pFileSysMgr->Init();

	// TODO: Add your control notification handler code here

	//IFileSysMgr* pFileSysMgr = ICreateFileSysMgr(NULL);

	if (pFileSysMgr)
	{
		IFileSys* pFileSys = pFileSysMgr->GetVFileSysByExt("vfs");
		pFileSys->RegisterRWFilter("?fdt",pFileSys->GetFileRWFilter(""));
		ROOT_h htrg = pFileSysMgr->OpenRoot("testfdt",VFS_OPENROOT_MODE_WRITE_ALWAYS);
		if (htrg==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("testfdt 目录打开出错!");
			return ;
		}

		ROOT_h hsrc = pFileSysMgr->OpenRoot("testfdt.vfs",VFS_OPENROOT_MODE_WRITE_EXIST);
		if (hsrc==VFS_INVALID_HANDLE_VALUE)
		{
			pFileSysMgr->CloseRoot(htrg);
			ErrorMsg("testfdt.vfs 打开出错!");
			return ;
		}

		FIND_h hfind = pFileSysMgr->FindFirstFile(hsrc,"",true);
		if (hfind!=VFS_INVALID_HANDLE_VALUE)
		{
			bool bfind;
			do 
			{
				ErrorMsg("copy %s",pFileSysMgr->GetFindPathName(hfind));
				pFileSysMgr->CopyFile(pFileSysMgr->GetFindRoot(hfind), pFileSysMgr->GetFindPathName(hfind),htrg,"");
				bfind = pFileSysMgr->FindNextFile(hfind);
			}while(bfind);
		}

		pFileSysMgr->CloseRoot(htrg);
		pFileSysMgr->CloseRoot(hsrc);
	
		ErrorMsg("success");
	}

//	pFileSysMgr->Release();	
}

const char* GetLine(const char* buf, int buflen)
{
#	define BUFLEN 255
	static char line[BUFLEN];
	int i;
	if (buflen>BUFLEN-1) buflen=BUFLEN-1;
	char ch;
	for (i=0;i<buflen && (ch=buf[i]);i++)
	{
		if (!(ch==0x0d || ch==0x0a))
		{
			line[i] = ch;
		}
		else
		{
			break;
		}		
	}
	line[i]=0;
	return line;
}

void CTestDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	IFileSysMgr* pFileSysMgr = ICreateFileSysMgr();
	pFileSysMgr->Init();

	pFileSysMgr->Init();

	// TODO: Add your control notification handler code here

	//IFileSysMgr* pFileSysMgr = ICreateFileSysMgr(NULL);

	if (pFileSysMgr)
	{
		{

			ROOT_h hsrc = pFileSysMgr->OpenRoot("data\\",VFS_OPENROOT_MODE_WRITE_ALWAYS);
			if (hsrc==VFS_INVALID_HANDLE_VALUE)
			{
				ErrorMsg("open root fail");
				return;
			}
			if (0==pFileSysMgr->CopyFile(hsrc,"hellofs.txt",hsrc,"hello.txt"))
			{
				ErrorMsg("copy fail");
			}
			if (0==pFileSysMgr->CloseRoot(hsrc))
			{
				ErrorMsg("close root fail");
			}
		}

		pFileSysMgr->MountRoot("test1.vfs","data\\"); 
		pFileSysMgr->MountRoot(".\\data","data\\");

		static int tag = 0;
		if (tag==0) { pFileSysMgr->SetVFSFirst(); ErrorMsg("read hello.txt from vfs!"); }
		else { pFileSysMgr->SetVFSFirst(false); ErrorMsg("read hello.txt from winfs!"); }
		tag^=1;

		FILE_h hfile = pFileSysMgr->OpenFile("data\\hello.txt");
		if (hfile==VFS_INVALID_HANDLE_VALUE)
		{
			ErrorMsg("not found hello.txt");
			return;
		}

		IBuffer* pbuffer = pFileSysMgr->GetFileBuffer(hfile);

		if (pbuffer==NULL)
		{
			ErrorMsg("error buffer");
			return;
		}

		char *pbuf = (char*)pbuffer->GetBuffer();

		ErrorMsg("%s",GetLine(pbuf,pbuffer->GetUsedSize()));		

		pFileSysMgr->CloseFile(hfile);
		pFileSysMgr->UnMountAll();
	}

}


void CTestDlg::ErrorMsg(const char* fmt,...)
{
	va_list vl;
	va_start(vl,fmt);
	char msgbuf[256];
	vsprintf(msgbuf,fmt,vl);
	int idx = m_lstOutput.GetCount();
	m_lstOutput.InsertString(idx,msgbuf);
	m_lstOutput.SetCaretIndex(idx);
	va_end(vl);
}