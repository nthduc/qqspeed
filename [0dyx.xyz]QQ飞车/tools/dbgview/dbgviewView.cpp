// dbgviewView.cpp : CdbgviewView 类的实现
//

#include "stdafx.h"
#include "dbgview.h"

#include "dbgviewDoc.h"
#include "dbgviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdbgviewView

IMPLEMENT_DYNCREATE(CdbgviewView, CListView)

BEGIN_MESSAGE_MAP(CdbgviewView, CListView)
END_MESSAGE_MAP()

// CdbgviewView 构造/析构

CdbgviewView::CdbgviewView()
{
	// TODO: 在此处添加构造代码

}

CdbgviewView::~CdbgviewView()
{
}

BOOL CdbgviewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CdbgviewView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	// 从而可以用项填充 ListView。
	CdbgviewDoc* pDoc = GetDocument();
	HICON hIcon = ExtractIcon(AfxGetInstanceHandle(), pDoc->GetPathName(), 0);
	{
		GetParent()->SetIcon(hIcon, TRUE);	
	}
	
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	CListCtrl& pCtrl = GetListCtrl();
	pCtrl.InsertColumn(0, "Debug Message", LVCFMT_LEFT, rcWindow.Width()*0.8);
	pCtrl.InsertColumn(1, "Time", LVCFMT_LEFT, rcWindow.Width()*0.2);
}


// CdbgviewView 诊断

#ifdef _DEBUG
void CdbgviewView::AssertValid() const
{
	CListView::AssertValid();
}

void CdbgviewView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CdbgviewDoc* CdbgviewView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdbgviewDoc)));
	return (CdbgviewDoc*)m_pDocument;
}
#endif //_DEBUG


// CdbgviewView 消息处理程序
void CdbgviewView::AddMessage(LPCSTR lpszMessage)
{
	GetDocument()->SetModifiedFlag();
	CListCtrl& pCtrl = GetListCtrl();
	int pos = pCtrl.InsertItem(pCtrl.GetItemCount(), lpszMessage);
	SYSTEMTIME t;
	memset(&t, 0, sizeof(t));
	GetLocalTime(&t);
	char szTime[MAX_PATH] = "";
	sprintf(szTime, "%02d:%02d:%02d:%03d", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	pCtrl.SetItemText(pos, 1, szTime);
	pCtrl.EnsureVisible(pos, FALSE);
}