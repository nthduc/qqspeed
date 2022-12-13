// AVTimer.cpp : implementation file
//

#include "stdafx.h"
//#include "IAvatar_i.h"
#include "AVTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAVTimer

CAVTimer::CAVTimer()
{
	m_nInterval = 10;
	m_bRun = FALSE;
}

CAVTimer::~CAVTimer()
{
}

BEGIN_MESSAGE_MAP(CAVTimer, CWnd)
	//{{AFX_MSG_MAP(CAVTimer)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CAVTimer message handlers
BOOL CAVTimer::Create()
{
	Close();
	
	if( CWnd::CreateEx( 0 , AfxRegisterWndClass(0) , NULL , WS_POPUP , CRect( 0 , 0 , 1 , 1 ) , NULL , 0 ) )
	{
		return TRUE;
	}

	return FALSE;
}

void CAVTimer::Close()
{
	if( ::IsWindow(m_hWnd) )
	{
		if( m_bRun )
		{
			KillTimer(AVTIEMR_ID);
		}			
		
		DestroyWindow();
		m_bRun = FALSE;
	}
}

void CAVTimer::SetTimer(int nInterval)
{
	ASSERT( nInterval > 0 );
	m_nInterval = nInterval;

	if( m_bRun )
	{
		KillTimer( AVTIEMR_ID );

		CWnd::SetTimer( AVTIEMR_ID , 100 * m_nInterval , NULL );
	}
}

void CAVTimer::StartTimer()
{
	if( !m_bRun )
	{
		CWnd::SetTimer( AVTIEMR_ID , 100 * m_nInterval , NULL );
		m_bRun = TRUE;
	}
}

void CAVTimer::StopTimer()
{
	if( m_bRun )
	{
		KillTimer( AVTIEMR_ID );
		m_bRun = FALSE;
	}
}

void CAVTimer::OnTimer( UINT nIDEvent )
{
	CWnd::OnTimer( nIDEvent );

	if( AVTIEMR_ID == nIDEvent )
	{
		RaiseEvent(P2P_TIMER_EVENT());
	}
}

BOOL CAVTimer::IsRunning()
{
	return m_bRun;
}
