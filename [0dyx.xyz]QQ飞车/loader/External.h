#if !defined(AFX_EXTERNAL_H__0859966D_DDDF_4837_BB7E_695C0B05C639__INCLUDED_)
#define AFX_EXTERNAL_H__0859966D_DDDF_4837_BB7E_695C0B05C639__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// External.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CExternal command target

class CExternal : public CCmdTarget
{
	DECLARE_DYNCREATE(CExternal)

	CExternal();           // protected constructor used by dynamic creation
	virtual ~CExternal();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExternal)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExternal)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CExternal)
	afx_msg void ShowDlg();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTERNAL_H__0859966D_DDDF_4837_BB7E_695C0B05C639__INCLUDED_)
