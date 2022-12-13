#ifndef __TIMERHELPER_T_H__
#define __TIMERHELPER_T_H__

#include "../include/Timer/timer_i.h"
#include "eventimpl_t.h"
#include <list>

namespace Tenio
{
	
	typedef std::list<IComponent*> TIMEROBJECTLIST;
	
	template<class TSubclass, BOOL bReleaseTimerObjectInCallbackMode = FALSE>
		class TTimerHelper : 
	public IEventImpl<ITimeoutEvent, 1>,
		public IEventImpl<IIntervalEvent, 1>
	{
protected:
	
	typedef void (TSubclass::*PTIMEOUTFUNCTION)(DWORD dwParam);
	typedef void (TSubclass::*PINTERVALFUNCTION)(DWORD dwParam);
	
public:
	//-----------------------------------------------------------------------------	
	//【构造与析构函数】
	//-----------------------------------------------------------------------------	
	
	TTimerHelper()
	{
		m_pTimeoutFunction   = NULL;
		m_pIntervalFunction  = NULL;
		m_pIntervalGenerator = NULL;
		m_pTimeroutGenerator = NULL;
	}
	
	virtual ~TTimerHelper()
	{
		ReleaseComponents();
	}
	
public:
	
	//-----------------------------------------------------------------------------
	//【对外接口】设置 Timeout, pTimeoutFunction 为回调函数名
	//-----------------------------------------------------------------------------
	
	BOOL SetTimeout(DWORD dwMilliSeconds, DWORD dwParam, 
		PTIMEOUTFUNCTION pTimeoutFunction)
	{
		BOOL bSuccess = FALSE;
		
		if(m_pTimeroutGenerator != NULL)
		{
			m_pTimeroutGenerator->ClearTimeout();
			
			m_listWaitingToDeleteTimeObjects.push_back(m_pTimeroutGenerator);
			
			m_pTimeroutGenerator = NULL;
		}
		
		m_pTimeoutFunction = pTimeoutFunction;
		
		ITimeout* pTimeout = GetTimeoutGenerator();
		
		m_pTimeroutGenerator = pTimeout;
		
		if(pTimeout != NULL)
		{
			pTimeout->SetTimeout(dwMilliSeconds, this, dwParam, NULL);
			
			bSuccess = TRUE;
		}
		
		return bSuccess;
	}
	
	//-----------------------------------------------------------------------------
	//【对外接口】清除 Timeout
	//-----------------------------------------------------------------------------
	
	void ClearTimeout()
	{
		if(m_pTimeroutGenerator != NULL)
		{
			m_pTimeroutGenerator->ClearTimeout();
		}
	}
	
	//-----------------------------------------------------------------------------
	//【对外接口】发生 Timeout 事件
	//-----------------------------------------------------------------------------
	
	void OnTimeout(DWORD dwParam)
	{
		if(m_pTimeoutFunction != NULL)
		{
			(Me()->*m_pTimeoutFunction)(dwParam);
		}
	}
	
	//-----------------------------------------------------------------------------
	//【对外接口】设置 Interval, pIntervalEvent 为回调函数名
	//-----------------------------------------------------------------------------
	
	BOOL SetInterval(DWORD dwMilliSeconds, DWORD dwParam,
		PINTERVALFUNCTION pIntervalFunction)
	{
		BOOL bSuccess = FALSE;
		
		if(m_pIntervalGenerator != NULL)
		{
			m_pIntervalGenerator->ClearInterval();
			
			m_listWaitingToDeleteTimeObjects.push_back(m_pIntervalGenerator);
			
			m_pIntervalGenerator = NULL;
		}
		
		m_pIntervalFunction = pIntervalFunction;
		
		IInterval* pInterval = GetIntervalGenerator();
		
		m_pIntervalGenerator = pInterval;
		
		if(pInterval != NULL)
		{
			pInterval->SetInterval(dwMilliSeconds, 
				static_cast<IIntervalEvent*>(this), dwParam, FALSE, NULL);
			
//			Me()->OutputTraceInfo(TEXT("SetInterval %d"), dwMilliSeconds);
			bSuccess = TRUE;
		}
		
		return bSuccess;
	}
	
	//-----------------------------------------------------------------------------
	//【对外接口】清除 Interval
	//-----------------------------------------------------------------------------
	
	void ClearInterval()
	{
		if(m_pIntervalGenerator != NULL)
		{
			m_pIntervalGenerator->ClearInterval();
		}
	}
	
	//-----------------------------------------------------------------------------
	//【对外接口】Interval 事件
	//-----------------------------------------------------------------------------
	
	void OnInterval(DWORD dwParam)
	{
		if(m_pIntervalFunction != NULL)
		{
			(Me()->*m_pIntervalFunction)(dwParam);
		}
	}
	
protected:
	
	//-----------------------------------------------------------------------------
	//【内部接口】获取组件, 在第一次调用时创建组件, 析构时删除
	//-----------------------------------------------------------------------------
	
	ITimeout* GetTimeoutGenerator()
	{
		if(m_pTimeroutGenerator == NULL)
		{
			Me()->CreateTenioComponent(&m_pTimeroutGenerator);
			
//			Me()->OutputTraceInfo(TEXT("Create new Timer=%p"), m_pTimeroutGenerator);
		}
		
		return m_pTimeroutGenerator;
	}
	
	IInterval* GetIntervalGenerator()
	{
		if(m_pIntervalGenerator == NULL)
		{
			Me()->CreateTenioComponent(&m_pIntervalGenerator);
			
//			Me()->OutputTraceInfo(TEXT("Create new Interval=%p"), m_pIntervalGenerator);
			
		}
		
		return m_pIntervalGenerator;
	}
	
	//-----------------------------------------------------------------------------
	//【内部接口】释放组件
	//-----------------------------------------------------------------------------
	
	void ReleaseComponents()
	{
		if(m_pTimeroutGenerator != NULL)
		{
//			Me()->OutputTraceInfo(TEXT("Delete Timer:%p"), m_pTimeroutGenerator);
			
			Me()->ReleaseTenioComponent(&m_pTimeroutGenerator);
			
			m_pTimeroutGenerator = NULL;
		}
		
		if(m_pIntervalGenerator != NULL)
		{
//			Me()->OutputTraceInfo(TEXT("Delete Interval:%p"), m_pIntervalGenerator);
			
			Me()->ReleaseTenioComponent(&m_pIntervalGenerator);
			
			m_pIntervalGenerator = NULL;
		}
		
		ReleaseStockTimeObjects();
	}
	
	void ReleaseStockTimeObjects()
	{
		TIMEROBJECTLIST::iterator it;
		
		for(it  = m_listWaitingToDeleteTimeObjects.begin();
		it != m_listWaitingToDeleteTimeObjects.end(); ++it)
		{
			IComponent* pCommonObject = *it;
			
			Me()->ReleaseTenioComponent(&pCommonObject);
			
//			Me()->OutputTraceInfo(TEXT("Delete Interval/Timer:%p"), 
//				pCommonObject);
		}
		
		m_listWaitingToDeleteTimeObjects.clear();
	}
	
private:
	ITimeout* m_pTimeroutGenerator;
	IInterval* m_pIntervalGenerator;
	
	PTIMEOUTFUNCTION  m_pTimeoutFunction;
	PINTERVALFUNCTION m_pIntervalFunction;
	
	TIMEROBJECTLIST m_listWaitingToDeleteTimeObjects;
	
private:
	TSubclass* Me()
	{
		return static_cast<TSubclass*> (this);
	}
};

};
#endif