// QQTUdpBufferMgr.cpp: implementation of the CQQTUdpBufferMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QQTPPP2ND.h"
#include "QQTUdpBufferMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQQTUdpBufferMgr::CQQTUdpBufferMgr()
{
	Initialize();	
}
void CQQTUdpBufferMgr::RollTagForward(int &iTag, int iSizeMax)
{
	iTag ++;
	if(iTag == iSizeMax)
	{
		iTag = 0;
	}
}

CQQTUdpBufferMgr::~CQQTUdpBufferMgr()
{

}
CQQTUdpBufferMgr::IMsgBuffer* CQQTUdpBufferMgr::AddMsgBuffer(byte *pBuffer, int iBufferLength, int iBufferRightLevel)
{
	IMsgBuffer *pRetBuffer = NULL;
	if(NULL == pBuffer || iBufferLength < 1 || iBufferLength > BIG_BUFFER_SIZE)
	{
		ASSERT(FALSE);
		return NULL;
	}
	if(iBufferLength <= SMALL_BUFFER_SIZE)
	{
		for(int i = 0; i < SMALL_BUFFER_COUNT; i ++)
		{
			if(m_arSmallBuffer[m_iSmallBufferTag].IsFreeBuffer())
			{
				m_arSmallBuffer[m_iSmallBufferTag].SetBuffer(pBuffer, iBufferLength, iBufferRightLevel);
				pRetBuffer = (IMsgBuffer*)(&m_arSmallBuffer[m_iSmallBufferTag]);
				RollTagForward(m_iSmallBufferTag, SMALL_BUFFER_COUNT);
				return pRetBuffer;
			}
			RollTagForward(m_iSmallBufferTag, SMALL_BUFFER_COUNT);
		}
		ASSERT(FALSE);
		//所有buffer已经全部使用,这时 iTag恰好转了一圈，故直接替换这个就可以
		m_arSmallBuffer[m_iSmallBufferTag].ClearRef();
		m_arSmallBuffer[m_iSmallBufferTag].SetBuffer(pBuffer, iBufferLength, iBufferRightLevel);
		pRetBuffer = (IMsgBuffer*)(&m_arSmallBuffer[m_iSmallBufferTag]);
		RollTagForward(m_iSmallBufferTag, SMALL_BUFFER_COUNT);
		return pRetBuffer;	
	}
	else
	{
		for(int i = 0; i < BIG_BUFFER_COUNT; i ++)
		{
			if(m_arBigBuffer[m_iBigBufferTag].IsFreeBuffer())
			{
				m_arBigBuffer[m_iBigBufferTag].SetBuffer(pBuffer, iBufferLength, iBufferRightLevel);
				pRetBuffer = (IMsgBuffer*)(&m_arBigBuffer[m_iBigBufferTag]);
				RollTagForward(m_iBigBufferTag, BIG_BUFFER_COUNT);
				return pRetBuffer;
			}
			RollTagForward(m_iBigBufferTag, BIG_BUFFER_COUNT);
		}
		ASSERT(FALSE);
		//所有buffer已经全部使用,这时 iTag恰好转了一圈，故直接替换这个就可以
		m_arBigBuffer[m_iBigBufferTag].ClearRef();
		m_arBigBuffer[m_iBigBufferTag].SetBuffer(pBuffer, iBufferLength, iBufferRightLevel);
		pRetBuffer = (IMsgBuffer*)(&m_arBigBuffer[m_iBigBufferTag]);
		RollTagForward(m_iBigBufferTag, BIG_BUFFER_COUNT);
		return pRetBuffer;
	}

	ASSERT(FALSE);
	return NULL;
}
void CQQTUdpBufferMgr::Initialize()
{
	m_iSmallBufferTag = 0;
	m_iBigBufferTag = 0;
	int i;
	for(i = 0; i < SMALL_BUFFER_COUNT; i ++)
	{
		m_arSmallBuffer[i].ClearRef();
	}
	for(i = 0; i < BIG_BUFFER_COUNT; i ++)
	{
		m_arBigBuffer[i].ClearRef();
	}
	//Add By Hyson, 2005.03.14 17:19
	//For 支持事件包重发
	for(i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		m_arLastBuffer[i].ClearRef();
	}
	//End Add	
}
void CQQTUdpBufferMgr::SetLastBuffer(int iClientTag, IMsgBuffer *pMsgBuffer)
{
	if(iClientTag >= 0 && iClientTag < MAX_P2P_DEST_PLAYER)
	{
		m_arLastBuffer[iClientTag].ClearRef();

		if(pMsgBuffer)
		{
			byte *pBuffer = NULL;
			int iBufferLen = 0;
			if(pMsgBuffer->GetBuffer(pBuffer, iBufferLen))
			{
				m_arLastBuffer[iClientTag].SetBuffer(pBuffer, iBufferLen, pMsgBuffer->GetRightLevel());
			}
			else
			{
				ASSERT(FALSE);
				return;
			}
			
			m_arLastBuffer[iClientTag].SetBufferSeq(iClientTag, pMsgBuffer->GetSequence(iClientTag));
			m_arLastBuffer[iClientTag].SetBufferState(iClientTag, pMsgBuffer->GetBufferState(iClientTag));
			m_arLastBuffer[iClientTag].SetBufferTimeStamp(iClientTag, pMsgBuffer->GetBufferTimeStamp(iClientTag));
		}		
	}
	else
	{
		ASSERT(FALSE);
	}
}
CQQTUdpBufferMgr::IMsgBuffer* CQQTUdpBufferMgr::GetLastBuffer(int iClientTag)
{
	if(iClientTag >= 0 && iClientTag < MAX_P2P_DEST_PLAYER)
	{
		return &m_arLastBuffer[iClientTag];
	}
	ASSERT(FALSE);
	return NULL;
}