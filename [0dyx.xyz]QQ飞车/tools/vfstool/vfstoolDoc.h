// vfstoolDoc.h : interface of the CVfstoolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VFSTOOLDOC_H__58F446CA_B789_48D7_BD59_72D231FD4978__INCLUDED_)
#define AFX_VFSTOOLDOC_H__58F446CA_B789_48D7_BD59_72D231FD4978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVfstoolView;
class CVfstoolDoc : public CDocument
{
protected: // create from serialization only
	CVfstoolDoc();
	DECLARE_DYNCREATE(CVfstoolDoc)

// Attributes
public:
	VROOT_h			m_hRoot;
	IFileSysMgr*	m_pFileSysMgr;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVfstoolDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Extract(LPCSTR lpszPath);
	CVfstoolView* GetActiveView();
	virtual ~CVfstoolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVfstoolDoc)
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VFSTOOLDOC_H__58F446CA_B789_48D7_BD59_72D231FD4978__INCLUDED_)
