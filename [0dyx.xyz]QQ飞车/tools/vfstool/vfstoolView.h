// vfstoolView.h : interface of the CVfstoolView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VFSTOOLVIEW_H__32A8C19E_934A_4F49_8429_F49A30D5F291__INCLUDED_)
#define AFX_VFSTOOLVIEW_H__32A8C19E_934A_4F49_8429_F49A30D5F291__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxole.h"

class CVfstoolView : public CTreeView
{
protected: // create from serialization only
	CVfstoolView();
	DECLARE_DYNCREATE(CVfstoolView)

// Attributes
public:
	CVfstoolDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVfstoolView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Clear();
	HTREEITEM AddItem(LPCSTR);
	virtual ~CVfstoolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	COleDataSource m_DataSource;
// Generated message map functions
protected:
	//{{AFX_MSG(CVfstoolView)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in vfstoolView.cpp
inline CVfstoolDoc* CVfstoolView::GetDocument()
   { return (CVfstoolDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VFSTOOLVIEW_H__32A8C19E_934A_4F49_8429_F49A30D5F291__INCLUDED_)
