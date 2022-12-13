//////////////////////////////////////////////////////////////////////////
// 多个定时器的辅助类 ，使用方法类似 window 的定时器方法
// MultiTimerHelper.h
//
// 使用举例：
//	1、继承 TMultiTimerHelper 模板
//
//	2、用 TenioSetTimer 设置定时器，
//		包含定时器nIDEvent， 定时间隔nEsapse， 搭载参数 lpParam
//
//	3、在子类中实现 OnMultiTimer 定时器处理函数
//		virtual void OnMultiTimer(UINT nIDEvent, LPVOID lpParam)
//		{
//			if (nIDEvent == 2) 
//			{
//				……
//			}
//		}	
//
//	4、用TenioClearTimer（ID）清除定时器。
//
//		
//			－－coolhomli
//////////////////////////////////////////////////////////////////////////



#pragma once

#include <map>
using namespace std;
#include "../include/Timer/timer_i.h"
#include "../Template/eventimpl_t.h"

namespace Tenio
{

#define	TIMER_ESAPSE					20

	typedef struct tagTimerItem
	{
		UINT nEsapse;
		UINT nCurrent;
		LPVOID lpParam;
	}
	TIMERITEM, *LPTIMERITEM;


	template<typename TRealType>
	class TMultiTimerHelper
		: public IEventImpl<IIntervalEvent, 1>
	{
		typedef map<UINT, TIMERITEM>   IDEVENT2ITEMMAP;

	public:
		TMultiTimerHelper() : m_pInterval(NULL)
		{
			
		}
		virtual  ~TMultiTimerHelper()
		{
			m_mapIDEvent2Item.clear();

			if (m_pInterval != NULL) 
			{
				m_pInterval->ClearInterval();

				Me()->ReleaseTenioComponent(&m_pInterval);
				m_pInterval = NULL;
			}
		}

	protected:
		void TenioSetTimer(UINT nIDEvent, UINT nElapse, LPVOID lpParam)
		{
			if (m_pInterval == NULL) 
			{
				Me()->CreateTenioComponent(&m_pInterval);
			}

			if (m_pInterval != NULL) 
			{
				if (m_mapIDEvent2Item.empty()) 
				{
					m_pInterval->SetInterval(
						TIMER_ESAPSE, 
						static_cast<IIntervalEvent*>(this),
						NULL,
						TRUE,
						NULL
						);
				}
				// Add New Item
				TIMERITEM stTimerItem = {0};
				stTimerItem.nEsapse = nElapse;
				stTimerItem.nCurrent = nElapse;
				stTimerItem.lpParam = lpParam;
				m_mapIDEvent2Item[nIDEvent] = stTimerItem;
			}
		}

		void TenioClearTimer(UINT nIDEvent)
		{
			IDEVENT2ITEMMAP::iterator it = m_mapIDEvent2Item.find(nIDEvent);

			if (it != m_mapIDEvent2Item.end()) 
			{
				m_mapIDEvent2Item.erase(it);
			}

			if (m_mapIDEvent2Item.empty()) 
			{
				m_pInterval->ClearInterval();
			}
		}

		// Functions
	protected:
		// IIntervalEvent
		virtual void OnInterval(DWORD dwParam)
		{
			IDEVENT2ITEMMAP mapTemp = m_mapIDEvent2Item;
			IDEVENT2ITEMMAP::iterator it = mapTemp.begin();
			
			for(; it != mapTemp.end(); ++it)
			{
				// Judge timer is already exist
				IDEVENT2ITEMMAP::iterator itTest = m_mapIDEvent2Item.find(it->first) ;

				if (itTest != m_mapIDEvent2Item.end()) 
				{
					TIMERITEM& stTimerItem = it->second;
					if (stTimerItem.nCurrent >= TIMER_ESAPSE)
					{
						stTimerItem.nCurrent -= TIMER_ESAPSE;
					}
					
					if (stTimerItem.nCurrent < TIMER_ESAPSE) 
					{
						OnMultiTimer(it->first, stTimerItem.lpParam);
						stTimerItem.nCurrent = stTimerItem.nEsapse;
					}
				}
			}

			it = mapTemp.begin();
			for(; it != mapTemp.end(); ++it)
			{
				IDEVENT2ITEMMAP::iterator it2 = m_mapIDEvent2Item.find(it->first);
				
				if (it2 != m_mapIDEvent2Item.end()) 
				{
					it2->second = it->second;
				}
			}

		}

		void ClearAllTimer()
		{
			m_mapIDEvent2Item.clear();

			if (m_pInterval != NULL) 
			{
				m_pInterval->ClearInterval();
			}	
		}

		virtual void OnMultiTimer(UINT nIDEvent, LPVOID lpParam)
		{

		}

		TRealType* Me()
		{
			return static_cast<TRealType*>(this);
		}

		// Attribute
	protected:
		IInterval* m_pInterval;
		IDEVENT2ITEMMAP m_mapIDEvent2Item;
	};

};