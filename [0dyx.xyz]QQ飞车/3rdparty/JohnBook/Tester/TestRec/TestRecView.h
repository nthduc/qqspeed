/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _TESTRECVIEW_H
#define _TESTRECVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTestRecView : public CEditView
{
protected: // create from serialization only
    CTestRecView();
    DECLARE_DYNCREATE(CTestRecView)

// Attributes
public:
    CTestRecDoc* GetDocument();

// Operations
public:


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTestRecView)
    public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual LRESULT SysMenuCommand(UINT nID, LPARAM lParam) ;
    protected:
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CTestRecView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    CFont m_cCourierFont ;

// Generated message map functions
protected:
    //{{AFX_MSG(CTestRecView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestRecView.cpp
inline CTestRecDoc* CTestRecView::GetDocument()
   { return (CTestRecDoc*)m_pDocument; }
#endif

////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !_TESTRECVIEW_H
