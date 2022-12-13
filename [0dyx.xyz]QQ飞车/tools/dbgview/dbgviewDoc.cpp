// dbgviewDoc.cpp :  CdbgviewDoc 类的实现
//

#include "stdafx.h"
#include "dbgview.h"

#include "dbgviewDoc.h"
#include ".\dbgviewdoc.h"
#include ".\dbgviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DWORD WINAPI PumpThread(
						LPVOID lpParameter   // thread data
						);
// CdbgviewDoc

IMPLEMENT_DYNCREATE(CdbgviewDoc, CDocument)

BEGIN_MESSAGE_MAP(CdbgviewDoc, CDocument)
	ON_COMMAND(ID_TERMINATE_CAPTURE, OnTerminateCapture)
END_MESSAGE_MAP()


// CdbgviewDoc 构造/析构

CdbgviewDoc::CdbgviewDoc():
m_hThread(INVALID_HANDLE_VALUE)
{
	// TODO: 在此添加一次性构造代码
	m_kProcessInfo.hProcess = INVALID_HANDLE_VALUE;
}

CdbgviewDoc::~CdbgviewDoc()
{
	if ( m_hThread != INVALID_HANDLE_VALUE )
	{
		TerminateThread(m_hThread, 0);
	}
	if ( m_kProcessInfo.hProcess != INVALID_HANDLE_VALUE )
	{
		TerminateProcess(m_kProcessInfo.hProcess, 0);
	}
}

BOOL CdbgviewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CdbgviewDoc 序列化

void CdbgviewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CdbgviewDoc 诊断

#ifdef _DEBUG
void CdbgviewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CdbgviewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CdbgviewDoc 命令

BOOL CdbgviewDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	char driver[4], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
	_splitpath(lpszPathName, driver, dir, fname, ext);
	char szPath[MAX_PATH];
	_makepath(szPath, driver, dir, NULL, NULL);

	if ( !CreateProcess(lpszPathName, NULL, NULL, NULL, FALSE, 0, NULL, szPath, &si, &pi) )
	{
		DWORD err = GetLastError();
		LPVOID pszErr;
		va_list va;
		
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			0, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pszErr, 0, NULL);
		MessageBox(AfxGetMainWnd()->m_hWnd, (LPSTR)pszErr, "错误", MB_OK|MB_ICONEXCLAMATION);
		LocalFree(pszErr);
		return FALSE;
	}
	m_kProcessInfo = pi;
	DWORD dwThreadID;
	m_hThread = CreateThread(NULL, 0, &PumpThread, (LPVOID)this, 0, &dwThreadID);

	return TRUE;
}

void CdbgviewDoc::OnMessage(LPCSTR lpszMessage)
{
	POSITION pos = GetFirstViewPosition();
	if ( pos == NULL )	
		return;
	CdbgviewView* pView = (CdbgviewView*)GetNextView(pos);
	if ( !pView )
		return;
	pView->AddMessage(lpszMessage);
}

#define PAGE_SIZE		4096
#define DBWIN_MAP		"DBWIN_BUFFER"
#define DBWIN_WRITEVENT "DBWIN_BUFFER_READY"
#define DBWIN_READEVENT "DBWIN_DATA_READY"

typedef struct _tag_DBMap
{
	DWORD dwProcessId;
	CHAR szString[PAGE_SIZE - sizeof(DWORD)];
} DB_MAP, *PDB_MAP;

DWORD WINAPI PumpThread(
						LPVOID lpParameter   // thread data
						)
{
	CdbgviewDoc* pDoc = (CdbgviewDoc*)lpParameter;
	DWORD dwProcessID = pDoc->m_kProcessInfo.dwProcessId;
	
	HANDLE hMap;
	PDB_MAP pDBMap;

	HANDLE hEventRead;
	HANDLE hEventWrite;

	hEventRead = CreateEvent(NULL, FALSE, FALSE, DBWIN_READEVENT);
	hEventWrite = CreateEvent(NULL, FALSE, FALSE, DBWIN_WRITEVENT);
	if(!hEventRead || !hEventWrite)
	{
		return 0;
	}

	hMap = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		PAGE_SIZE,
		DBWIN_MAP);
	if(!hMap)
		return 0;

	pDBMap = (PDB_MAP)MapViewOfFile(hMap,
		FILE_MAP_READ,
		0,
		0,
		0);
	if(!pDBMap)
	{
		CloseHandle(hMap);
		return 0;
	}

	while(TRUE)
	{
		SetEvent(hEventWrite);	
		DWORD dwWait = WaitForSingleObject(hEventRead, INFINITE);
		if ( dwWait == WAIT_OBJECT_0 && dwProcessID == pDBMap->dwProcessId )
		{
			pDoc->OnMessage(pDBMap->szString);
		}
	}
	return 0;
}
void CdbgviewDoc::OnTerminateCapture()
{
	// TODO: Add your command handler code here
	if ( m_hThread )
	{
		TerminateThread(m_hThread, 0);
	}
}

BOOL CdbgviewDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	CFileDialog fd(FALSE, "log", NULL, 0, "*.log|*.log||", AfxGetMainWnd());
	if ( IDOK != fd.DoModal() )
	{
		return FALSE;
	}
	CString strPathName = fd.GetPathName();

	FILE* fp = fopen(strPathName, "r");
	if ( fp )
	{
		if ( IDNO == MessageBox(AfxGetMainWnd()->m_hWnd, "文件已存在，是否替换？", "保存", MB_YESNO|MB_ICONEXCLAMATION) )
			return FALSE;
	}

	POSITION pos = GetFirstViewPosition();
	if ( pos == NULL )	
		return FALSE;
	CdbgviewView* pView = (CdbgviewView*)GetNextView(pos);
	if ( !pView )
		return FALSE;

	CListCtrl& pCtrl = pView->GetListCtrl();

	fp = fopen(strPathName, "w");
	if ( !fp )
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, "无法打保存文件(文件只读或者正在使用中)", "保存", MB_ICONEXCLAMATION);
		return FALSE;
	}

	char szText[MAX_PATH] = "";
	for ( int i = 0; i < pCtrl.GetItemCount(); i++ )
	{
		pCtrl.GetItemText(i, 0, szText, MAX_PATH);
		fputs(szText, fp);
	}
	fclose(fp);
	return TRUE;
	return CDocument::OnSaveDocument(lpszPathName);
}
