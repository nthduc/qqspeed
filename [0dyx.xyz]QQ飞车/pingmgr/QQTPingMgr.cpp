// PingMgr.cpp : implementation file
//

#include "stdafx.h"
#include "UnkHelper.h"
#include "QQTPing.h"
#include "QQTPingMgr.h"
#include "IPingObject.h"
#include "IPingMgr_i.c"
#include "Winsock2.h"
#include "IcmpPing.h"
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
		}
		m_PingArray.RemoveAt(0);
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

STDMETHODIMP CPingMgr::XPingMgr::PingServer(unsigned long ulSvrIP, unsigned short usPort, int *pnIndex, int nLimitTime)
{
	METHOD_PROLOGUE(CPingMgr, PingMgr);
	return pThis->PingServer(ulSvrIP, usPort, pnIndex, nLimitTime);
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

STDMETHODIMP CPingMgr::PingServer(unsigned long ulSvrIP, unsigned short usPort, int *pnIndex, int nLimitTime)
{
	if (NULL == pnIndex)
	{
		ASSERT(FALSE);
		return S_FALSE;
	}

	DeleteCanDeleteObjects();

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
				return S_FALSE;
			}
		}
	}

	++s_nIndex;
	*pnIndex = s_nIndex;

	// icmp
	IPingObject *pPingOjbect = (IPingObject *)new CIcmpPingObject;
	m_PingArray.Add(pPingOjbect);

	pPingOjbect->SetIndex(s_nIndex);
	pPingOjbect->Ping(ulSvrIP, usPort, DEFAULT_PING_COUNT, timeGetTime(), nLimitTime);

	// udp
	IPingObject *pUdpPingOjbect = (IPingObject *)new CUdpPingObject;
	m_UdpPingArray.Add(pUdpPingOjbect);
	pUdpPingOjbect->SetIndex(s_nIndex);
	pUdpPingOjbect->Ping(ulSvrIP, usPort, DEFAULT_PING_COUNT, timeGetTime(), nLimitTime);

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
			break;
		}
	}

	for (i = 0; i < m_UdpPingArray.GetSize(); ++i)
	{
		IPingObject *pPingOjbect = m_UdpPingArray.GetAt(i);
		if (NULL != pPingOjbect && pPingOjbect->IsObject(nIndex))
		{
			pPingOjbect->CancelPing();
			break;
		}
	}
	
	return S_OK;
}

#define MAX_PRIORITY 5000
#define FLOAT_EQUAL(aaa, bbb) \
	(((float)aaa - (float)bbb < 0.00001) && ((float)aaa - (float)bbb > -0.00001))


void CPingMgr::CheckPingResult()
{
	int nTTL1, nTTL2, nAverageRTT1, nAverageRTT2;
	float fLossRate1, fLossRate2;
	
	for (int i = 0; i < m_PingArray.GetSize(); ++i)
	{
		IPingObject *pPingObject1 = m_PingArray.GetAt(i);

		if (!pPingObject1 || pPingObject1->IsCanceled())
		{
			continue;
		}

		TRACE("\n***ping count*** = %d\n", m_PingArray.GetSize());
		// 时间到了 或已经可以删除（同一地址的所有的icmp ping都返回了）
		if (pPingObject1 
			&& ((pPingObject1->IsTimeOut() || pPingObject1->CanDelete())))
		{				
			IPingObject *pPingObject2 = NULL;
			pPingObject1->CancelPing();

			// 找到与Icmp 对应的udp
			for (int j = 0; j < m_UdpPingArray.GetSize(); ++j)
			{
				IPingObject *pTemp = m_UdpPingArray.GetAt(j);
				if (pTemp && pTemp->IsObject(pPingObject1->GetIndex()))
				{				
					pPingObject2 = pTemp;
					pTemp->GetPingResult(nTTL2, fLossRate2, nAverageRTT2);	
					break;
				}
			}

			TRACE("is time out = %d", pPingObject1->IsTimeOut());
			TRACE("is can delete = %d", pPingObject1->CanDelete());

			pPingObject1->GetPingResult(nTTL1, fLossRate1, nAverageRTT1);
			TRACE("\nget ping result : ttl = %d, lossRate1 = %f , averagertt1 = %d\n", nTTL1, fLossRate1, nAverageRTT1);

			// 先检测icmp的ping， 如果有返回且不是全部丢包, 就使用icmp的结果
			if (FLOAT_EQUAL(1.0f, fLossRate1) && pPingObject2)
			{
				TRACE("\nnotify udp ping result index = %d\n", pPingObject1->GetIndex());
				m_pPingSink->NotifyPingResult(pPingObject2->GetIndex(), nTTL2, fLossRate2, nAverageRTT2);					 
			}
			else
			{
				TRACE("\nnotify icmp ping result index = %d\n", pPingObject1->GetIndex());
				m_pPingSink->NotifyPingResult(pPingObject1->GetIndex(), nTTL1, fLossRate1, nAverageRTT1);
			}
		}
	}
}

void CPingMgr::CheckCanDeleteObjects()
{
	for (int i = 0; i < m_PingArray.GetSize(); ++i)
	{
		IPingObject *pPingObject1 = m_PingArray.GetAt(i);
		if (pPingObject1) 
		{
			pPingObject1->CheckCanDelete();
		}
	}
	
	for (i = 0; i < m_UdpPingArray.GetSize(); ++i)
	{
		IPingObject *pPingObject2 = m_UdpPingArray.GetAt(i);
		if (pPingObject2)
		{
			pPingObject2->CheckCanDelete();
		}
	}
}

void CPingMgr::OnTimer(UINT nIDEvent)
{
	ASSERT(nIDEvent == PING_TIMER_ID);
	if (PING_TIMER_ID != nIDEvent)
	{
		return;
	}

	CheckCanDeleteObjects();
	CheckPingResult();
	DeleteCanDeleteObjects();
}


void CPingMgr::DeleteCanDeleteObjects()
{
	int i = 0;
//	TRACE("del ping count = %d", m_PingArray.GetSize());
	while (i < m_PingArray.GetSize())
	{
		IPingObject *pPingObject1 = m_PingArray.GetAt(i);
		if (pPingObject1 && pPingObject1->CanDelete())
		{
//			TRACE("delete ping object");
			delete pPingObject1;
			//m_PingArray.RemoveAt(i);
			m_PingArray.SetAt(i, NULL);
		}
		else
		{
			++i;
		}
	}

	i = 0;
	while (i < m_UdpPingArray.GetSize())
	{
		IPingObject *pPingObject2 = m_UdpPingArray.GetAt(i);
		if (pPingObject2 && pPingObject2->CanDelete())
		{
			delete pPingObject2;
			//m_UdpPingArray.RemoveAt(i);
			m_UdpPingArray.SetAt(i, NULL);
		}
		else
		{ 
			++i;
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
