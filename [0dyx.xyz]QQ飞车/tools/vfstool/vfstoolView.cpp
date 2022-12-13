// vfstoolView.cpp : implementation of the CVfstoolView class
//

#include "stdafx.h"
#include "vfstool.h"

#include "vfstoolDoc.h"
#include "vfstoolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVfstoolView

IMPLEMENT_DYNCREATE(CVfstoolView, CTreeView)

BEGIN_MESSAGE_MAP(CVfstoolView, CTreeView)
	//{{AFX_MSG_MAP(CVfstoolView)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVfstoolView construction/destruction

CVfstoolView::CVfstoolView()
{
	// TODO: add construction code here
}

CVfstoolView::~CVfstoolView()
{
}

BOOL CVfstoolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVfstoolView drawing

void CVfstoolView::OnDraw(CDC* pDC)
{
	CVfstoolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CVfstoolView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();


	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	DragAcceptFiles();
	
}

/////////////////////////////////////////////////////////////////////////////
// CVfstoolView printing

BOOL CVfstoolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVfstoolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVfstoolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CVfstoolView diagnostics

#ifdef _DEBUG
void CVfstoolView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CVfstoolView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CVfstoolDoc* CVfstoolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVfstoolDoc)));
	return (CVfstoolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVfstoolView message handlers

HTREEITEM CVfstoolView::AddItem(LPCSTR lpszFileName)
{
	CTreeCtrl& tree = GetTreeCtrl();
	return tree.InsertItem(lpszFileName);
}

void CVfstoolView::Clear()
{
	CTreeCtrl& tree = GetTreeCtrl();
	tree.DeleteAllItems();
}

void CVfstoolView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
/*
	HGLOBAL hText = GlobalAlloc(GMEM_FIXED, 100);
	LPVOID pText = GlobalLock(hText);
	strcpy((char*)pText, "abcd");
	GlobalUnlock(hText);

	m_DataSource.CacheGlobalData(CF_TEXT, hText);
	m_DataSource.DoDragDrop();
	*/
//	*pResult = 0;
	CTreeCtrl& tree = GetTreeCtrl();
	CImageList* pImageList = tree.CreateDragImage(pNMTreeView->itemNew.hItem);
	
	*pResult = 1;
}

BOOL CVfstoolView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTreeView::OnDrop(pDataObject, dropEffect, point);
}

void CVfstoolView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTreeView::OnDropFiles(hDropInfo);
}
