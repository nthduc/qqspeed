/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "TestRec.h"

#include "TestRecDoc.h"
#include "TestRecView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////
// CTestRecView

IMPLEMENT_DYNCREATE(CTestRecView, CEditView)

BEGIN_MESSAGE_MAP(CTestRecView, CEditView)
    //{{AFX_MSG_MAP(CTestRecView)
    ON_WM_CREATE()
    ON_MESSAGE ( WM_REDIRECTED_SYSCOMMAND , SysMenuCommand)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////
// CTestRecView construction/destruction

CTestRecView::CTestRecView()
{
    m_cCourierFont.CreatePointFont ( 100 , _T ( "Courier New" ) ) ;
}

CTestRecView::~CTestRecView()
{
}

////////////////////////////////////////////////////////////////////////
// CTestRecView drawing

void CTestRecView::OnDraw(CDC* /*pDC*/)
{
    CTestRecDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: add draw code for native data here
}

////////////////////////////////////////////////////////////////////////
// CTestRecView diagnostics

#ifdef _DEBUG
void CTestRecView::AssertValid() const
{
    CEditView::AssertValid();
}

void CTestRecView::Dump(CDumpContext& dc) const
{
    CEditView::Dump(dc);
}

CTestRecDoc* CTestRecView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestRecDoc)));
    return (CTestRecDoc*)m_pDocument;
}
#endif //_DEBUG


LRESULT CTestRecView::SysMenuCommand(UINT nID, LPARAM /*lParam*/)
{
    if ( ( SC_RESTORE == nID ) || ( SC_CLOSE == nID ) )
    {
        // The user is restoring or closing the app so stop recording.
        GetDocument()->StopRecording ( ) ;
    }
    return ( 0 ) ;
}

int CTestRecView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CEditView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: Add your specialized creation code here
    SetFont ( &m_cCourierFont ) ;
    return 0;
}
