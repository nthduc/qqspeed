#if !defined(AFX_QQTRAWSOCKET_H__898369AC_03B8_4CFD_AAFF_A89F39BBD3D6__INCLUDED_)
#define AFX_QQTRAWSOCKET_H__898369AC_03B8_4CFD_AAFF_A89F39BBD3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QQTRawSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQQTRawSocket command target
#include <afxsock.h>

class IRawSocketCallBack
{
public :
	virtual void OnReceive(int nErrorCode) = 0;
};

class CQQTRawSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CQQTRawSocket();
	virtual ~CQQTRawSocket();

	void Init(IRawSocketCallBack *pCallBack);

	IRawSocketCallBack *m_pCallBack;

	virtual void OnReceive(int nErrorCode);

	SOCKET m_RawSock;
	
	void Create();

	void Close();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQTRawSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CQQTRawSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQTRAWSOCKET_H__898369AC_03B8_4CFD_AAFF_A89F39BBD3D6__INCLUDED_)
