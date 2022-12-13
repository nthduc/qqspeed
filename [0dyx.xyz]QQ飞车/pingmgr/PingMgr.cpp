// PingMgr.cpp : implementation file
//

#include "stdafx.h"
#include "UnkHelper.h"
#include "QQTPing.h"
#include "PingMgr.h"
#include "IPingObject.h"
#include "IPingMgr_i.c"
#include "Winsock2.h"
#include "IcmpPingObject.h"
#include "UdpPingObject.h"
#include "IPingMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "Ws2_32.lib")


/////////////////////////////////////////////////////////////////////////////
// CPingMgr

#define PING_TIMER_ID 1

IMPLEMENT_DYNCREATE(CPingMgr, CCmdTarget)
IMPLEMENT_IUNKNOWN(CPingMgr, PingMgr)
IMPLEMENT_OLECREATE(
	CPingMgr,
	"CPingMgr",	
	0xEDCD6AE4, 0x0A21, 0x4d19, 0xA1, 0xDD, 0x87, 0x83, 0x60, 0x65, 0x43, 0x21, 
)

BEGIN_INTERFACE_MAP(CPingMgr, CCmdTarget)
	INTERFACE_PART(CPingMgr, IID_IPingMgr, PingMgr)
END_INTERFACE_MAP()

int CPingMgr::s_nIndex = 0;

CPingMgr::CPingMgr()
{
	TRACE("CPingMgr::CPingMgr()\n");
	m_PingArray.RemoveAll();
	m_pPingSink = NULL; 
	m_nLimitTime = DEFAULT_PING_LIMIT_TIME;

	m_bIsTimerRunning = FALSE;

	VERIFY( CreateEx(0,AfxRegisterWndClass(0,NULL,NULL,NULL),NULL,WS_POPUP,0,0,0,0,NULL,NULL) );
}

CPingMgr::~CPingMgr()
{
	TRACE("CPingMgr::~CPingMgr()\n");
	while (m_PingArray.GetSize())
	{
		IPingObject *pPingObject = m_PingArray.GetAt(0);
		if (NULL != pPingObject)
		{
			pPingObject->CancelPing();
			delete pPingObject;
			m_PingArray.RemoveAt(0);
		}
	}

	EndTimer();
	DestroyWindow();
}

void CPingMgr::StartTimer()
{
	if (!::IsWindow(GetSafeHwnd()))
	{
		return;
	}

	if (m_bIsTimerRunning)
	{
		EndTimer();
	}

	ASSERT(IsWindow(GetSafeHwnd()));

	SetTimer(PING_TIMER_ID, 250, NULL);
	m_bIsTimerRunning = TRUE;
}

void CPingMgr::EndTimer()
{
	if (m_bIsTimerRunning && IsWindow(GetSafeHwnd()))
	{
		KillTimer(PING_TIMER_ID);
		m_bIsTimerRunning = FALSE;
	}
}

STDMETHODIMP CPingMgr::XPingMgr::SetLimitTime(int nLimitTime)
{
	METHOD_PROLOGUE(CPingMgr, PingMgr);
	return pThis->SetLimitTime(nLimitTime);
}

STDMETHODIMP CPingMgr::XPingMgr::PingServer(unsigned long ulSvrIP, unsigned short usPort, int *pnIndex)
{
	METHOD_PROLOGUE(CPingMgr, PingMgr);
	return pThis->PingServer(ulSvrIP, usPort, pnIndex);
}

STDMETHODIMP CPingMgr::XPingMgr::CancelPing(int nIndex)
{
	METHOD_PROLOGUE(CPingMgr, PingMgr);
	return pThis->CancelPing(nIndex);
}

STDMETHODIMP CPingMgr::XPingMgr::Initialize(IPingSink *pPingSink)
{
	METHOD_PROLOGUE(CPingMgr, PingMgr);
	return pThis->Initialize(pPingSink);
}

STDMETHODIMP CPingMgr::SetLimitTime(int nLimitTime)
{	
	m_nLimitTime = nLimitTime;
	return S_OK;
}

STDMETHODIMP CPingMgr::PingServer(unsigned long ulSvrIP, unsigned short usPort, int *pnIndex)
{
	if (NULL == pnIndex)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	long ip = ::htonl(ulSvrIP);
	TRACE("PingServer::ping server ip = %s, port = %d\n", ::inet_ntoa(*(struct in_addr*)(&ip)), usPort);
	
	for (int i = 0; i < m_PingArray.GetSize(); ++i)
	{
		IPingObject *pPingObject = m_PingArray.GetAt(i);
		if (NULL != pPingObject)
		{
			if (pPingObject->IsSameDestServer(ulSvrIP, usPort))
			{
				// 不要ping相同的Server
				long ip = ::htonl(ulSvrIP);
				TRACE("ping same server ip = %s, port = %d\n", ::inet_ntoa(*(struct in_addr*)(&ip)), usPort);
				return FALSE;
			}
		}
	}

	++s_nIndex;
	*pnIndex = s_nIndex;

	// icmp
	IPingObject *pPingOjbect = (IPingObject *)new CIcmpPingObject;
	m_PingArray.Add(pPingOjbect);

	pPingOjbect->SetIndex(s_nIndex);
	pPingOjbect->Ping(ulSvrIP, usPort, DEFAULT_PING_COUNT, GetTickCount());

	// udp
	IPingObject *pUdpPingOjbect = (IPingObject *)new CUdpPingObject;
	m_UdpPingArray.Add(pUdpPingOjbect);
	pUdpPingOjbect->SetIndex(s_nIndex);
	pUdpPingOjbect->Ping(ulSvrIP, usPort, DEFAULT_PING_COUNT, GetTickCount());

	return S_OK;
}

STDMETHODIMP CPingMgr::Initialize(IPingSink *pPingSink)
{
	if (NULL == pPingSink)
	{
		ASSERT(FALSE);
		return S_FALSE;
	}

	m_pPingSink = pPingSink;

	StartTimer();

	return S_OK;
}

STDMETHODIMP CPingMgr::CancelPing(int nIndex)
{
	int i;

	for (i = 0; i < m_PingArray.GetSize(); ++i)
	{
		IPingObject *pPingOjbect = m_PingArray.GetAt(i);
		if (NULL != pPingOjbect && pPingOjbect->IsObject(nIndex))
		{
			pPingOjbect->CancelPing();

			delete pPingOjbect;
			m_PingArray.RemoveAt(i);
			break;
		}
	}

	for (i = 0; i < m_UdpPingArray.GetSize(); ++i)
	{
		IPingObject *pPingOjbect = m_UdpPingArray.GetAt(i);
		if (NULL != pPingOjbect && pPingOjbect->IsObject(nIndex))
		{
			pPingOjbect->CancelPing();

			delete pPingOjbect;
			m_UdpPingArray.RemoveAt(i);
			break;
		}
	}
	
	return S_OK;
}

#define MAX_PRIORITY 5000
#define FLOAT_EQUAL(aaa, bbb) \
	(((float)aaa - (float)bbb < 0.00001) && ((float)aaa - (float)bbb > -0.00001))


void CPingMgr::OnTimer(UINT nIDEvent)
{
	ASSERT(nIDEvent == PING_TIMER_ID);
	if (PING_TIMER_ID != nIDEvent)
	{
		return;
	}

	int nTTL1, nTTL2, nAverageRTT1, nAverageRTT2;
	float fLossRate1, fLossRate2;
	
	for (int i = 0; i < m_PingArray.GetSize(); ++i)
	{
		IPingObject *pPingObject1 = m_PingArray.GetAt(i);
		
		// 时间到了
		if (pPingObject1 && ((int)GetTickCount() - pPingObject1->GetStartTime() >= m_nLimitTime))
		{				
			TRACE("ping count = %d\n", m_PingArray.GetSize());
			pPingObject1->CancelPing();
			pPingObject1->GetPingResult(nTTL1, fLossRate1, nAverageRTT1);
			
			for (int j = 0; j < m_UdpPingArray.GetSize(); ++j)
			{
				IPingObject *pPingObject2 = m_UdpPingArray.GetAt(j);
				
				if (pPingObject2 && pPingObject2->IsObject(pPingObject1->GetIndex()))
				{				
					pPingObject2->GetPingResult(nTTL2, fLossRate2, nAverageRTT2);
					
					if (!FLOAT_EQUAL(1.0f, fLossRate1))
					{
						m_pPingSink->NotifyPingResult(pPingObject1->GetIndex(), nTTL1, fLossRate1, nAverageRTT1);
						TRACE("notify icmp ping result index = %d\n", pPingObject1->GetIndex());
					}
					else
					{
						m_pPingSink->NotifyPingResult(pPingObject2->GetIndex(), nTTL2, fLossRate2, nAverageRTT2);					 
						TRACE("notify udp ping result index = %d\n", pPingObject1->GetIndex());
					}
					
					delete pPingObject2;
					m_UdpPingArray.RemoveAt(j);
					
					break;
				}
			}
			
			delete pPingObject1;
			m_PingArray.RemoveAt(i);
		}
	}
}

BEGIN_MESSAGE_MAP(CPingMgr, CWnd)
	//{{AFX_MSG_MAP(CPingMgr)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPingMgr message handlers
