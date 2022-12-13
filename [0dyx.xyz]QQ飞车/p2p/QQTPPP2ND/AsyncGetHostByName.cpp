// AsyncGetHostByName.cpp : implementation file
//

#include "stdafx.h"
#include "AsyncGetHostByName.h"
//#include "resall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAsyncGetHostByName

void CAsyncGetHostByName::DoEvent(BOOL bSuccess, CString strFailReason, CString strIP)
{
	TOnHostGet v;
	v.bSuccess = bSuccess;
	v.strFailReason = strFailReason;
	v.strIP = strIP;
	v.nSvrType = m_nSvrType;
	v.nSvrIndex = m_nSvrIndex;

	//OnHostGet.m_lParam = (LPARAM)&v;
	LPARAM lParam = (LPARAM)&v;
	::SendMessage(m_hCallerWnd, m_nCallerMsg, 0, lParam);
	//OnHostGet.CallEvent(); //must use send message, v must hold before CallEvent() return;
}

#define WM_GETHOSTBYNAME_RETURN			WM_USER+1

CAsyncGetHostByName::CAsyncGetHostByName(HWND hWnd,  UINT nMsg, const CString strAddr,int nSvrType,int nSvrIndex)
{
	if (!CWnd::CreateEx(0, AfxRegisterWndClass(0), _T(""), WS_POPUP, 0,0,0,0, NULL, 0))
	{
		AfxMessageBox("system error! can not create window!");
	}

	m_hCallerWnd = hWnd;
	m_nCallerMsg = nMsg;
	//OnHostGet.Assign(hWnd, nMsg);
	m_nSvrType = nSvrType;
	m_nSvrIndex = nSvrIndex;

	m_strLastError = "";
	//m_straTemp.RemoveAll();
	m_straHostNeedToResolve.Add(strAddr);
	GetHostByName();
}


CAsyncGetHostByName::CAsyncGetHostByName(HWND hWnd,  UINT nMsg, const CStringArray* pstraAddr)
{
	if (!CWnd::CreateEx(0, AfxRegisterWndClass(0), _T(""), WS_POPUP, 0,0,0,0, NULL, 0))
	{
		AfxMessageBox("system error! can not create window!");
	}

	m_hCallerWnd = hWnd;
	m_nCallerMsg = nMsg;

	//OnHostGet.Assign(hWnd, nMsg);
	
	m_strLastError = "";
	m_straHostNeedToResolve.Copy(*pstraAddr);
	GetHostByName();
}


CAsyncGetHostByName::~CAsyncGetHostByName()
{

}


void CAsyncGetHostByName::GetHostByName()
{
	if(m_straHostNeedToResolve.GetSize()>0)
	{
		CString strAddr = m_straHostNeedToResolve[0];
		m_straHostNeedToResolve.RemoveAt(0);
		
		//如果IP地址不是x.x.x.x形式
		memset(m_arAsyncGetHostByNameBuf, 0, MAXGETHOSTSTRUCT);
		if(!WSAAsyncGetHostByName(m_hWnd, WM_GETHOSTBYNAME_RETURN, strAddr,
			m_arAsyncGetHostByNameBuf,MAXGETHOSTSTRUCT))
		{
			DoEvent(FALSE, "can not call api: WSAAsyncGetHostByName", "0.0.0.0");
			DestroyWindow();
			delete this;
		}

	}
	else
	{
		DoEvent(FALSE, m_strLastError, "0.0.0.0");
		DestroyWindow();
		delete this;
	}
}


void CAsyncGetHostByName::OnGetHostByNameReturn(WPARAM wParam, LPARAM lParam)
{
	BOOL bSuccess = FALSE;
	CString strError;
	switch(WSAGETASYNCERROR(lParam))
	{
	case 0:
		bSuccess = TRUE;
		break;
	case WSAENETDOWN:
		//LoadStr(strError, IDS_U_DID_NOT_CONNECT_TO_INTERNET_PLEASE_CHECK_NETWORK_CONFIG);
//		strError.LoadString(IDS_U_DID_NOT_CONNECT_TO_INTERNET_PLEASE_CHECK_NETWORK_CONFIG);
		bSuccess = FALSE;
		break;
	case WSAHOST_NOT_FOUND:
//		strError.LoadString(IDS_CANT_CONNECT_SERVER_PLEASE_CHECK_IF_CONNECT_INTERNET);
		bSuccess = FALSE;
		break;	
	default :
//		strError.LoadString(IDS_CANT_CONNECT_SERVER_PLEASE_CHECK_IF_CONNECT_INTERNET);
		bSuccess = FALSE;
		break;
	}
	

	if(bSuccess)
	{
		//IP地址已取得
		LPHOSTENT lphost = (LPHOSTENT)m_arAsyncGetHostByNameBuf;
		CString strIP;
		strIP.Format("%s",inet_ntoa(  *(LPIN_ADDR)lphost->h_addr  ));
		DoEvent(TRUE, "", strIP);
		DestroyWindow();
		delete this;
	}
	else
	{
		//IP地址还没取得
		m_strLastError = strError;
		GetHostByName();

		/*
		CInternetDetect::tagGetHostByNameResult* p = new CInternetDetect::tagGetHostByNameResult;
		p->bSuccess = FALSE;
		p->strFailReason = strError;
		OnHostGot.m_lParam = (LPARAM) p;
		OnHostGot.CallEvent(FALSE); //FALSE表示用PostMessage
		*/
	}
}


BEGIN_MESSAGE_MAP(CAsyncGetHostByName, CWnd)
	//{{AFX_MSG_MAP(CAsyncGetHostByName)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GETHOSTBYNAME_RETURN, OnGetHostByNameReturn)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAsyncGetHostByName message handlers
