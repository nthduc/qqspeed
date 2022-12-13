#if !defined(AFX_ASYNCGETHOSTBYNAMEWND_H__FE50839F_BECA_4CEC_BDEB_F36679C5FDAF__INCLUDED_)
#define AFX_ASYNCGETHOSTBYNAMEWND_H__FE50839F_BECA_4CEC_BDEB_F36679C5FDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AsyncGetHostByNameWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAsyncGetHostByNameWnd window

class CAsyncGetHostByNameWnd : public CWnd
{
public:
	class IAsyncGetHostByNameWnd
	{
	public:
		enum AsyncGetHostByNameResultValue
		{
			AGHBNRV_Successed = 0,
			AGHBNRV_Failed
		};
		virtual void GetAsyncGetHostByNameResult( AsyncGetHostByNameResultValue , UINT , ULONG ) = 0;
	};
// Construction
public:
	CAsyncGetHostByNameWnd( IAsyncGetHostByNameWnd* );

// Attributes
public:

// Operations
public:
	void GetIP( UINT , CString );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsyncGetHostByNameWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAsyncGetHostByNameWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAsyncGetHostByNameWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnGetIP( WPARAM , LPARAM );
	DECLARE_MESSAGE_MAP()


	void DeleteSelf();
private:
	IAsyncGetHostByNameWnd* m_pIAsyncGetHostByNameWnd;
	UINT m_unJobID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNCGETHOSTBYNAMEWND_H__FE50839F_BECA_4CEC_BDEB_F36679C5FDAF__INCLUDED_)
