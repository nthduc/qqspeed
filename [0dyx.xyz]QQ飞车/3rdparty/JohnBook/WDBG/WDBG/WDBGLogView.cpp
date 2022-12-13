/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"

#include "WDBGProjDoc.h"
#include "WDBGLogView.h"
#include "WDBGChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CWDBGLogView, CEditView)

BEGIN_MESSAGE_MAP(CWDBGLogView, CEditView)
    //{{AFX_MSG_MAP(CWDBGLogView)
        ON_WM_CHAR()
    ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
    ON_MESSAGE(WM_DBG_OUTPUT,DebuggerOutputMsg)
END_MESSAGE_MAP()

CWDBGLogView::CWDBGLogView()
{
    // TODO: add construction code here
}

CWDBGLogView::~CWDBGLogView()
{
}

BOOL CWDBGLogView::PreCreateWindow(CREATESTRUCT& cs)
{
    BOOL bPreCreated = CEditView::PreCreateWindow(cs);

    return bPreCreated;
}

void CWDBGLogView::OnDraw(CDC* /*pDC*/)
{
#ifdef _DEBUG
    CWDBGProjDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
#endif
}

BOOL CWDBGLogView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default CEditView preparation
    return CEditView::OnPreparePrinting(pInfo);
}

void CWDBGLogView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Default CEditView begin printing.
    CEditView::OnBeginPrinting(pDC, pInfo);
}

void CWDBGLogView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
    // Default CEditView end printing
    CEditView::OnEndPrinting(pDC, pInfo);
}

#ifdef _DEBUG
void CWDBGLogView::AssertValid() const
{
    CEditView::AssertValid();
}

void CWDBGLogView::Dump(CDumpContext& dc) const
{
    CEditView::Dump(dc);
}

CWDBGProjDoc* CWDBGLogView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWDBGProjDoc)));
    return (CWDBGProjDoc*)m_pDocument;
}
#endif //_DEBUG

void CWDBGLogView :: Clear ( void )
{
    GetEditCtrl().SetWindowText ( NULL ) ;
    GetEditCtrl().SetModify ( FALSE ) ;
}

void CWDBGLogView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // I am eating the keystrokes because I want a read only edit
    // control that does not had a grey color associated with it.
    switch ( LOBYTE ( VkKeyScan ( (TCHAR)nChar ) ) )
    {
        case VK_LWIN    :
        case VK_RWIN    :
        case VK_APPS    :
        case VK_HELP    :
        case VK_PRIOR   :
        case VK_NEXT    :
        case VK_UP      :
        case VK_DOWN    :
        case VK_LEFT    :
        case VK_RIGHT   :
            CEditView::OnChar(nChar, nRepCnt, nFlags);
            break ;
        default         :
            break ;
    }
}

void CWDBGLogView::OnInitialUpdate()
{
    CEditView::OnInitialUpdate();

    // Set the text to this view's frame
    CString sTitle ;
    if ( FALSE == sTitle.LoadString ( IDS_LOGGINGWINTITLE ) )
    {
        ASSERT ( !"sTitle.LoadString ( IDS_LOGGINGWINTITLE ) failed!");
        sTitle = _T ( "Log Window" ) ;
    }
    SetViewText ( sTitle ) ;

    // Set the font for the edit control.
    m_cEditFont.CreatePointFont ( 100                  ,
                                  _T ( "Courier New" )  ) ;

    SetFont ( &m_cEditFont ) ;
}

void CWDBGLogView::SetViewText ( LPCTSTR szText , BOOL bDisableClose )
{
    GetParent()->SetWindowText ( szText ) ;
    CWDBGChildFrame * pFrame = (CWDBGChildFrame *)GetParent ( ) ;
    if ( TRUE == bDisableClose )
    {
        pFrame->DisableCloseButton ( ) ;
    }
    else
    {
        pFrame->EnableCloseButton ( ) ;
    }
}

void CWDBGLogView :: DeleteLine ( int iLine )
{
    // Deletes the specified line number.
    int iBegin = GetEditCtrl().LineIndex ( iLine ) ;
    if ( -1 == iBegin )
    {
        return ;
    }
    int iEnd = iBegin + GetEditCtrl().LineLength ( iBegin ) ;
    // The +2 accounts for "\r\n"
    GetEditCtrl().SetSel ( iBegin , iEnd + 2 , TRUE ) ;
    GetEditCtrl().ReplaceSel ( _T ( "" ) ) ;
}

afx_msg LRESULT CWDBGLogView::DebuggerOutputMsg ( WPARAM /*wParam*/ ,
                                                  LPARAM lParam      )
{
    LPCTSTR szData = (LPCTSTR)lParam ;

    // Append it to the end of the edit control.
    AppendText ( szData ) ;
    return ( 0 ) ;
}

void CWDBGLogView :: AppendText ( LPCTSTR szText , BOOL bNoScroll )
{
    int iLen = GetEditCtrl().GetWindowTextLength ( ) ;

    GetEditCtrl().SetSel ( iLen , iLen , bNoScroll ) ;
    GetEditCtrl().ReplaceSel ( szText ) ;
    if ( TRUE == bNoScroll )
    {
        GetEditCtrl().SetSel ( 0 , 0 , bNoScroll ) ;
    }
}

void CWDBGLogView :: AppendLast ( LPCTSTR szText    ,
                                  BOOL    bNoScroll  )
{
    MSG msg ;

    while ( ::PeekMessage ( &msg , NULL , NULL , NULL , PM_NOREMOVE ) )
    {
        AfxGetThread()->PumpMessage ( ) ;
    }

    AppendText ( szText , bNoScroll ) ;
}

void CWDBGLogView::OnChange()
{
    // Just eat it.
}

void CWDBGLogView::OnEditPaste()
{
    // Just eat it.

}

void CWDBGLogView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    // Keep it disabled.
    pCmdUI->Enable ( FALSE ) ;
}

void CWDBGLogView::OnRButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
    // Just eat it.
}

void CWDBGLogView::OnEditCut()
{
    // Just eat it.
}

void CWDBGLogView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ( FALSE ) ;
}


void CWDBGLogView::OnDestroy()
{
    // It would be nice if MFC told the document something other than
    // "Uhhh, the view list changed.  Even though I know what view was
    // added or removed, you need to go find out yourself."
    CWDBGProjDoc * pDoc = GetDocument ( ) ;
    ASSERT ( NULL != pDoc ) ;
    pDoc->ViewDestroyed ( this ) ;
    CEditView::OnDestroy();
}
