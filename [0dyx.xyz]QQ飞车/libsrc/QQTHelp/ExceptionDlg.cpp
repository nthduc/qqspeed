// ExceptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QQTHelp.h"
#include "ExceptionDlg.h"
#include "TraceStack.h"
#include "fileop.h"
#include "filever.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExceptionDlg dialog

CExceptionDlg::CExceptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExceptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExceptionDlg)
	m_strError = _T("");
	m_bRestartBQQ = TRUE;
	//}}AFX_DATA_INIT
	m_ExceptionInfo = NULL;
	m_bSendError = FALSE;

}


void CExceptionDlg::DoDataExchange(CDataExchange* pDX)
{
 
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExceptionDlg)
	DDX_Text(pDX, IDC_EDIT_ERROR_INFO, m_strError);
	DDX_Check(pDX, IDC_CHECK_RESTART, m_bRestartBQQ);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CExceptionDlg, CDialog)
	//{{AFX_MSG_MAP(CExceptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExceptionDlg message handlers

BOOL CExceptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (GetSafeHwnd())
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->SetWindowText("确 定");
	}

	ASSERT(m_ExceptionInfo);
	if(!m_ExceptionInfo)
		return FALSE;
	
	//设置ICON
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_SORRY_ICON);
	m_bmpIcon.LoadBitmap(IDB_QQSORRY_BMP);
	pStatic->SetBitmap(m_bmpIcon);

	CString strTemp;

	EXCEPTION_RECORD * pERec = m_ExceptionInfo->ExceptionRecord;
	switch(pERec->ExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		strTemp += "EXCEPTION_ACCESS_VIOLATION";
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		strTemp += "EXCEPTION_DATATYPE_MISALIGNMENT";
		break;
	case EXCEPTION_BREAKPOINT:
		strTemp += "EXCEPTION_BREAKPOINT";
		break;
	case EXCEPTION_SINGLE_STEP:
		strTemp += "EXCEPTION_SINGLE_STEP";
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		strTemp += "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		strTemp += "EXCEPTION_FLT_DENORMAL_OPERAND";
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		strTemp += "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		strTemp += "EXCEPTION_FLT_INEXACT_RESULT";
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		strTemp += "EXCEPTION_FLT_INVALID_OPERATION";
		break;
	case EXCEPTION_FLT_OVERFLOW:
		strTemp += "EXCEPTION_FLT_OVERFLOW";
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		strTemp += "EXCEPTION_FLT_STACK_CHECK";
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		strTemp += "EXCEPTION_FLT_UNDERFLOW";
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		strTemp += "EXCEPTION_INT_DIVIDE_BY_ZERO";
		break;
	case EXCEPTION_INT_OVERFLOW:
		strTemp += "EXCEPTION_INT_OVERFLOW";
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		strTemp += "EXCEPTION_PRIV_INSTRUCTION";
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		strTemp += "EXCEPTION_IN_PAGE_ERROR";
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		strTemp += "EXCEPTION_ILLEGAL_INSTRUCTION";
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		strTemp += "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		break;
	case EXCEPTION_STACK_OVERFLOW:
		strTemp += "EXCEPTION_STACK_OVERFLOW";
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		strTemp += "EXCEPTION_INVALID_DISPOSITION";
		break;
	case EXCEPTION_GUARD_PAGE:
		strTemp += "EXCEPTION_GUARD_PAGE";
		break;
	case EXCEPTION_INVALID_HANDLE:
		strTemp += "EXCEPTION_INVALID_HANDLE";
		break;
	}

	char szBuf[MAX_PATH] = "\0";
	char szProfile[MAX_PATH] = "\0";
	FullName("client.ini", szProfile);
	char szModule[MAX_PATH] = "\0";
	GetPrivateProfileString("App", "Cmd", "", szModule, MAX_PATH, szProfile);
	FormatVersion(szBuf, MAX_PATH, szModule);
	char szProduct[MAX_PATH] = "\0";
	GetPrivateProfileString("App", "Product", "", szProduct, MAX_PATH, szProfile);
	CString strVer = CString(szProduct) + szBuf;
	//End Add

	//出错类型:\r\n%s\r\n出错地址:\r\n 0x%08X\r\n
	m_strError.Format("-----------------------------------\r\n出错类型： %s\r\n出错地址： 0x%08X\r\n",strTemp,pERec->ExceptionAddress);
	m_strError = strVer + m_strError;

	if(pERec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION &&
		pERec->NumberParameters >1 &&
		pERec->ExceptionInformation )
	{
		CString str;
		if(pERec->ExceptionInformation[0]==0)
			str = "读取";
		else
			str = "写入";
		//出错原因:\r\n对 0x%08X 地址进行 %s 操作
		strTemp.Format("出错原因： 对 0x%08X 地址进行%s操作",pERec->ExceptionInformation[1],str);
		m_strError += strTemp;
	}	
	
	m_strError += "\r\n\r\n出错过程：\r\n";
	strTemp.Empty();
	GetCallStack(*(m_ExceptionInfo->ContextRecord), strTemp);
	m_strError += strTemp;

	//\r\n模块及其加载地址列表: \r\n===================================
	m_strError += "\r\n\r\n模块及其加载地址列表: \r\n-----------------------------------";

	PBYTE pb = NULL;
	MEMORY_BASIC_INFORMATION mbi;
	int nLen;
	char szModName[MAX_PATH];

	while (VirtualQuery(pb, &mbi, sizeof(mbi)) == sizeof(mbi))
	{
		if (mbi.State == MEM_FREE)
			mbi.AllocationBase = mbi.BaseAddress;

		if ( (mbi.AllocationBase != mbi.BaseAddress) ||
			(mbi.AllocationBase == NULL))
		{
			// Do not add the module name to the list
			// if any of the following is true:
			// 1. If this block is NOT the beginning of a region
			// 2. If the address is NULL
			nLen = 0;
		}
		else
		{
			nLen = GetModuleFileNameA((HINSTANCE) mbi.AllocationBase,
			szModName, MAX_PATH);
		}
		if (nLen > 0)
		{
			strTemp.Format("\r\n[ 0x%08X ] %s", mbi.AllocationBase, szModName);
			m_strError += strTemp;
		}
		pb += mbi.RegionSize;
	}
	m_strError += "\r\n___________________________________";
	
	

	// add by Hyson, 2003.08.07 1531
	// for write err msg to file

	char szInfoFilePath[MAX_PATH] = "\0";
	FullName("Info.ini", szInfoFilePath);

	BOOL bWriteLog = FALSE;

	//int iIfWriteLog = GetPrivateProfileInt("Public","IfDoLog",0,InfoFilePathName);
	int nWriteLog = GetPrivateProfileInt( "HelpDll" , "WriteLog" , 0 , szInfoFilePath );

#define _MUST_LOG_TO_FILE_
#ifdef _MUST_LOG_TO_FILE_
	nWriteLog = 1;
#endif
	
	//只输出第一个错误
	static BOOL bFirstErrorLog = TRUE;
	if(nWriteLog && bFirstErrorLog)
	{
		bFirstErrorLog = FALSE;
		char szRootPath[MAX_PATH] = "\0";
		GetModulePath(NULL, szRootPath);
		CString strParh = szRootPath;
		strParh += "HelpLog\\";
		::CreateDirectory(strParh, NULL);
		strParh += "error.log";
				
		char x[10240];
		memset(x, 0, 10240);

		strncpy(x, m_strError, 900);
		FILE* file = fopen(strParh , "w");
		if(file)
		{
			int i = 0;			
			while(x[i] && i < 10240)
			{
				fputc(x[i], file);
				i ++;
			}
			FILE* fpException = fopen("exception.log", "r");
			if ( fpException )
			{
				fseek(fpException, 0, SEEK_END);
				long len = ftell(fpException) + 1;
				char* buf = new char[len];
				memset(buf, 0, len);
				fseek(fpException, 0, SEEK_SET);
				fread(buf, 1, len, fpException);
				fclose(fpException);

				fseek(file, 0, SEEK_END);
				fwrite(buf, 1, len, file);
			}
			fclose(file);
		}		
	}
	// end add  

	UpdateData(FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
// 发送错误报告
//
void CExceptionDlg::OnOK() 
{
	UpdateData();
	m_bSendError = TRUE;

	CDialog::OnOK();
}

////////////////////////////////////////////////////////////////////////////
//
// 不发送错误报告
//
void CExceptionDlg::OnCancel() 
{
	UpdateData();
	m_bSendError = FALSE;

	CDialog::OnCancel();
}
