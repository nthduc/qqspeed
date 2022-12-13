// AsyncGetHostByNameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AsyncGetHostByName.h"
#include "AsyncGetHostByNameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_GET_IP WM_USER + 1

/////////////////////////////////////////////////////////////////////////////
// CAsyncGetHostByNameWnd

CAsyncGetHostByNameWnd::CAsyncGetHostByNameWnd( IAsyncGetHostByNameWnd* pIAsyncGetHostByNameWnd )
{
	m_pIAsyncGetHostByNameWnd = pIAsyncGetHostByNameWnd;
}

CAsyncGetHostByNameWnd::~CAsyncGetHostByNameWnd()
{
}


BEGIN_MESSAGE_MAP(CAsyncGetHostByNameWnd, CWnd)
	//{{AFX_MSG_MAP(CAsyncGetHostByNameWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_GET_IP , OnGetIP )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAsyncGetHostByNameWnd message handlers
DWORD dwT1 = 0;
DWORD dwT2 = 0;

void CAsyncGetHostByNameWnd::GetIP( UINT unJobID , CString strIP )
{
	CWnd::CreateEx( 0 , AfxRegisterWndClass(0) , NULL , WS_POPUP , CRect( 0 , 0 , 1 , 1 ) , NULL , 0 );

	dwT1 = GetTickCount();
	if( ::IsWindow( m_hWnd ) )
	{
		m_unJobID	= unJobID;
		new CAsyncGetHostByName( m_hWnd , WM_GET_IP , strIP );
	}
	else
	{
//		AfxMessageBox( "创建域名解析窗口失败。" );
		if( m_pIAsyncGetHostByNameWnd ) m_pIAsyncGetHostByNameWnd->GetAsyncGetHostByNameResult( IAsyncGetHostByNameWnd::AsyncGetHostByNameResultValue::AGHBNRV_Failed , unJobID , 0 );
		DeleteSelf();
	}
}
void CAsyncGetHostByNameWnd::DeleteSelf()
{
	if( ::IsWindow( m_hWnd ) ) ::DestroyWindow( m_hWnd );
	delete this;
}

void CAsyncGetHostByNameWnd::OnGetIP( WPARAM wParam , LPARAM lParam )
{
	dwT2 = GetTickCount();
	CAsyncGetHostByName::TOnHostGet* p = (CAsyncGetHostByName::TOnHostGet*) lParam;
	
	if( p->bSuccess )
	{
		ULONG ulip = inet_addr( p->strIP );
		
		if( m_pIAsyncGetHostByNameWnd ) m_pIAsyncGetHostByNameWnd->GetAsyncGetHostByNameResult( IAsyncGetHostByNameWnd::AsyncGetHostByNameResultValue::AGHBNRV_Successed , m_unJobID , ulip );
	}
	else if( m_pIAsyncGetHostByNameWnd ) m_pIAsyncGetHostByNameWnd->GetAsyncGetHostByNameResult( IAsyncGetHostByNameWnd::AsyncGetHostByNameResultValue::AGHBNRV_Failed , m_unJobID , 0 );

	DeleteSelf();
}