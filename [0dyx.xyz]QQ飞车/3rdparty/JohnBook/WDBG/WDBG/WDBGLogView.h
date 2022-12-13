/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _WDBGLOGVIEW_H
#define _WDBGLOGVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWDBGLogView : public CEditView
{
protected: // create from serialization only
    CWDBGLogView();
    DECLARE_DYNCREATE(CWDBGLogView)

// Attributes
public:
    CWDBGProjDoc* GetDocument();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWDBGLogView)
    public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CWDBGLogView();

    // Clears the edit control.
    void Clear ( void ) ;

    void SetViewText ( LPCTSTR szText , BOOL bDisableClose = TRUE ) ;

    // Appends the text to the end of the edit control.
    void AppendText ( LPCTSTR szText , BOOL bNoScroll = FALSE ) ;
    void AppendLast ( LPCTSTR szText , BOOL bNoScroll = FALSE ) ;

    void DeleteLine ( int iLine ) ;

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CFont m_cEditFont ;

// Generated message map functions
protected:
    //{{AFX_MSG(CWDBGLogView)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChange();
    afx_msg void OnEditPaste();
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnEditCut();
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
    afx_msg void OnDestroy();
    //}}AFX_MSG
    afx_msg LRESULT DebuggerOutputMsg(WPARAM wParam, LPARAM lParam) ;
    DECLARE_MESSAGE_MAP()

private     :
    CWDBGLogView ( CWDBGLogView & ) { }
    CWDBGLogView & operator = ( const CWDBGLogView & )
        { return ( *this ) ; }

};

#ifndef _DEBUG  // debug version in WDBGLogView.cpp
inline CWDBGProjDoc* CWDBGLogView::GetDocument()
   { return (CWDBGProjDoc*)m_pDocument; }
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _WDBGLOGVIEW_H
