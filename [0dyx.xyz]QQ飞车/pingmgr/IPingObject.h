#ifndef _IPINGOBJECT_H_
#define _IPINGOBJECT_H_

#define INVALID_PING_INDEX		(-1)
#define DEFAULT_PING_LIMIT_TIME 2500
#define DEFAULT_PING_COUNT		4

#include <mmsystem.h>

class IPingObject
{
protected:
	unsigned int m_ulSvrIP;
	unsigned short m_usSvrPort;
	int m_nStartTime;
	int m_nLimitTime;
	int m_nIndex;

	BOOL m_bIsCanceled;

public:

	IPingObject() 
	{
		m_bIsCanceled = FALSE;
	}
	
	virtual BOOL IsCanceled() 
	{
		return m_bIsCanceled;
	}

	virtual ~IPingObject()
	{
	}

	virtual BOOL IsSameDestServer(unsigned int ulSvrIP, unsigned short usSvrPort)
	{
		return (ulSvrIP == m_ulSvrIP && usSvrPort == m_usSvrPort);
	}

	virtual BOOL IsObject(int nIndex)
	{
		return (m_nIndex == nIndex);
	}

	virtual void SetIndex(int nIndex)
	{
		m_nIndex = nIndex;
	}

	virtual int GetIndex()
	{
		return m_nIndex;
	}

	virtual int GetStartTime()
	{
		return m_nStartTime;
	}

	virtual int GetLimitTime()
	{
		return m_nLimitTime;
	}

	virtual BOOL IsTimeOut()
	{
		return timeGetTime() - m_nStartTime >= m_nLimitTime;
	}
	

	virtual void CancelPing()
	{
		m_bIsCanceled = TRUE;
	}

	virtual void GetPingResult(int &nTTL, float &fLossRate, int &nAverageRTT) = 0;
	virtual void Ping(unsigned int ulSvrIP, unsigned short usSvrPort, int nPingCount, int nStartTime, int nLimitTime) = 0;

	virtual BOOL CanDelete() = 0;

	virtual void CheckCanDelete() = 0;
};

#endif

