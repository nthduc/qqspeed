// dbgviewView.cpp : CdbgviewView ���ʵ��
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

// CdbgviewView ����/����

CdbgviewView::CdbgviewView()
{
	// TODO: �ڴ˴���ӹ������

}

CdbgviewView::~CdbgviewView()
{
}

BOOL CdbgviewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CdbgviewView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	// �Ӷ������������ ListView��
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


// CdbgviewView ���

#ifdef _DEBUG
void CdbgviewView::AssertValid() const
{
	CListView::AssertValid();
}

void CdbgviewView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CdbgviewDoc* CdbgviewView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdbgviewDoc)));
	return (CdbgviewDoc*)m_pDocument;
}
#endif //_DEBUG


// CdbgviewView ��Ϣ�������
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