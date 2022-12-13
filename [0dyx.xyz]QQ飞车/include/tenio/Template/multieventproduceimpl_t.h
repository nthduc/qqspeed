//////////////////////////////////////////////////////////////////////////
//	多事件触发者模板
//
//	一、使用指南：
//
//	1、event 事件要成为多事件列表中的一个， 那么要定义 EventHandler，使用下面的宏
//		TENIO_MAKE_EVENTHANDLER_MAP(event)
//	
//	2、定义多事件列表，使用 MakeTypelist 模板
//	举例：
//		事件2 event1, 事件2 event2 …… 事件n eventn
//
//		typedef MakeTypelist<event1, event2, ... ... , eventn>::Result MyEventList;
//
//	3、多事件触发者，继承 IMultiEventProducerGenerator 模板类，
//		模板参数为 < 多事件列表，多事件触发者>
//	举例：
//		class CMultiEventTrigger 
//			: public IMultiEventProducerGenerator<MyEventList, CMultiEventTrigger>
//		{
//			... ...
//		};
//
//	[4]、如果多事件触发者， 需要触发 MyEventList 中的前面 m 个事件， 而不是全部的事件，
//		那么，可以使用另外一个模板 IMultiEventProducerImplGenerator 来实现。
//	举例：
//		class CMultiEventTrigger 
//			: public IMultiEventProducerImplGenerator<MyEventList, m, CMultiEventTrigger>
//		{
//			... ...
//		};	
//		这样多事件列表的定义可以重用。

//
//					---- By Coolhomli  [7/3/2006]
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

#include "../Include/event_i.h"
#include "../Include/portable.h"
#include "../Include/eventhandlermap.h"
#include "typelist.h"


namespace Tenio
{	
#pragma warning(push)
#pragma warning(disable:4311)
	
	template<class eventlist, class IRealImpl = IEventProducer>
		class IMultiEventProducerImplBase : public IRealImpl
	{
	public:
		IMultiEventProducerImplBase() {}
		virtual ~IMultiEventProducerImplBase() {}
		
		//////////////////////////////////////////////////////////////////////////
		
	protected:
		template<class EventType>
			void TryAddEventHandler(EventType* pEmpty, IEvent* pEvent, LPVOID lpParam)
		{
			EventType * pRealEvent 
				= dynamic_cast<EventType*>(pEvent);
			if (pRealEvent != NULL) 
			{					
				//RemoveEventHandler(pEvent);
				TryRemoveEventHandler(pRealEvent, pEvent);
				EventHandler<EventType>::GetMap((long)this).insert(
					EventHandler<EventType>::EventHandler2ParamMapValue(
					static_cast<EventType*>(pEvent), 
					lpParam
					)
					);
			}
		}
		
		template<class EventType>
			void TryRemoveEventHandler(EventType* pEmpty, IEvent* pEvent)
		{
			EventType * pRealEvent 
				= dynamic_cast<EventType*>(pEvent);
			if (pRealEvent != NULL) 
			{					
				EventHandler<EventType>::GetMap((long)this).erase(
					static_cast<EventType*>(pEvent)
					);
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Helper function
	private:
		
		template<class EventHandler2ParamMap>
			void CopyEventHandlersBeforeFiring(EventHandler2ParamMap& mapRecurring, EventHandler2ParamMap& mapHode)
		{
			EventHandler2ParamMap::iterator it = mapHode.begin();
			for(; it != mapHode.end(); ++it)
			{
				mapRecurring.insert(EventHandler2ParamMap::value_type(it->first, it->second));
			}
		}
		
		template<class EventHandler2ParamMap, class Type>
			inline bool PopFromMap(EventHandler2ParamMap& theMap, Type*& pEventHandler, VOID*& pParam)
		{
			EventHandler2ParamMap::iterator it = theMap.begin();
			pEventHandler = it->first;
			pParam        = it->second;
			theMap.erase(it);
			return IsEventHandlerValid(pEventHandler);
		}
		
		template<class Type>
			inline bool IsEventHandlerValid(Type* pEventHandler)
		{
			return EventHandler<Type>::GetMap((long)this).find(pEventHandler) != EventHandler<Type>::GetMap((long)this).end();
		}
		
		template<class EventHandler2ParamMap,class Type, class Pred>
			inline bool PopFromMapCheckParam(EventHandler2ParamMap& theMap, Type*& pEventHandler, VOID*& pParam, Pred PredFunc)
		{
			return PopFromMap(theMap, pEventHandler, pParam) && PredFunc(pParam);
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Event firer
		
	protected:
		
		template<class Type> inline
			void FireEvent(void (Type::*EventName)())
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));			
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)();
				}
			}
		}
		
		template<class Type, class Pred> inline
			void FireEventIf(void (Type::*EventName)(), Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)();
				}
			}
		}
		
		// 1 argument
		template<class Type, class ArgT> inline
			void FireEvent(void (Type::*EventName)(ArgT), const ArgT& arg)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg);
				}
			}
		}
		
		template<class Type, class ArgT, class Pred> inline
			void FireEventIf(void (Type::*EventName)(ArgT), const ArgT& arg, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg);
				}
			}
		}
		
		// 2 arguments
		template<class Type, class Arg1T, class Arg2T> inline
			void FireEvent(void (Type::*EventName)(Arg1T, Arg2T),
			const Arg1T& arg1, const Arg2T& arg2)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2);
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Pred> inline
			void FireEventIf(void (Type::*EventName)(Arg1T, Arg2T),
			const Arg1T& arg1, const Arg2T& arg2, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2);
				}
			}
		}
		
		// 3 arguments
		template<class Type, class Arg1T, class Arg2T, class Arg3T> inline
			void FireEvent(void (Type::*EventName)(Arg1T, Arg2T, Arg3T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3);
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Pred> inline
			void FireEventIf(void (Type::*EventName)(Arg1T, Arg2T, Arg3T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3);
				}
			}
		}
		
		// 4 arguments
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T> inline
			void FireEvent(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4);
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Pred> inline
			void FireEventIf(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4);
				}
			}
		}
		
		// 5 arguments
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T> inline
			void FireEvent(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, Arg5T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, const Arg5T& arg5)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, arg5);
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T, class Pred> inline
			void FireEventIf(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, Arg5T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, const Arg5T& arg5, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, arg5);
				}
			}
		}
		
		// 6 arguments
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T, class Arg6T> inline
			void FireEvent(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, Arg5T, Arg6T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, const Arg5T& arg5, const Arg6T& arg6)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, arg5, arg6);
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T, class Arg6T, class Pred> inline
			void FireEventIf(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, Arg5T, Arg6T),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, const Arg5T& arg5, const Arg6T& arg6, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, arg5, arg6);
				}
			}
		}
		
		
		//////////////////////////////////////////////////////////////////////////
		// FireEvent with Param as the last argument
		// 1 argument
		template<class Type, class ParamT> inline
			void FireEventWithParam(void (Type::*EventName)(ParamT))
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(static_cast<ParamT>(pParam));
				}
			}
		}
		
		template<class Type, class ParamT, class Pred> inline
			void FireEventWithParamIf(void (Type::*EventName)(ParamT), Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(static_cast<ParamT>(pParam));
				}
			}
		}
		
		// 2 argument
		template<class Type, class ArgT, class ParamT> inline
			void FireEventWithParam(void (Type::*EventName)(ArgT, ParamT), const ArgT& arg)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg, static_cast<ParamT>(pParam));
				}
			}
		}
		
		template<class Type, class ArgT, class ParamT, class Pred> inline
			void FireEventWithParamIf(void (Type::*EventName)(ArgT, ParamT), const ArgT& arg, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg, static_cast<ParamT>(pParam));
				}
			}
		}
		
		// 3 arguments
		template<class Type, class Arg1T, class Arg2T, class ParamT> inline
			void FireEventWithParam(void (Type::*EventName)(Arg1T, Arg2T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, static_cast<ParamT>(pParam));
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class ParamT, class Pred> inline
			void FireEventWithParamIf(void (Type::*EventName)(Arg1T, Arg2T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, static_cast<ParamT>(pParam));
				}
			}
		}
		
		// 4 arguments
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class ParamT> inline
			void FireEventWithParam(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, static_cast<ParamT>(pParam));
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class ParamT, class Pred> inline
			void FireEventWithParamIf(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, static_cast<ParamT>(pParam));
				}
			}
		}
		
		// 5 arguments
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class ParamT> inline
			void FireEventWithParam(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3, const Arg4T& arg4)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, static_cast<ParamT>(pParam));
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class ParamT, class Pred> inline
			void FireEventWithParamIf(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, static_cast<ParamT>(pParam));
				}
			}
		}
		
		// 6 arguments
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T, class ParamT> inline
			void FireEventWithParam(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, Arg5T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, const Arg5T& arg5)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMap(mapRecurringEventHandler2Param, pEventHandler, pParam))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, arg5, static_cast<ParamT>(pParam));
				}
			}
		}
		
		template<class Type, class Arg1T, class Arg2T, class Arg3T, class Arg4T, class Arg5T, class ParamT, class Pred> inline
			void FireEventWithParamIf(void (Type::*EventName)(Arg1T, Arg2T, Arg3T, Arg4T, Arg5T, ParamT),
			const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3,
			const Arg4T& arg4, const Arg5T& arg5, Pred PredFunc)
		{
			EventHandler<Type>::EventHandler2ParamMap mapRecurringEventHandler2Param;
			CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param, EventHandler<Type>::GetMap((long)this));
			
			Type* pEventHandler = NULL;
			VOID*       pParam        = NULL;
			
			while(!mapRecurringEventHandler2Param.empty())
			{
				if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
				{
					(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, arg5, static_cast<ParamT>(pParam));
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		
	};
	
	//////////////////////////////////////////////////////////////////////////
	
#define MetaTryAddEventHandler(index)						\
	TypeAt<eventlist, index>::Result *pEmpty_##index = NULL;	\
	TryAddEventHandler(pEmpty_##index, pEvent, lpParam);		\
	
	
#define MetaTryRemoveEventHandler(index)					\
	TypeAt<eventlist, index>::Result *pEmpty_##index = NULL;	\
	TryRemoveEventHandler(pEmpty_##index, pEvent);			\
	
#define DEFINE_EVENTHANDLER(index)\
	typedef typename EventHandler<TypeAt<eventlist, index>::Result >  EventHandler_##index;\
	
	//////////////////////////////////////////////////////////////////////////
	// For 1 event producer
	
	template<class eventlist, class IRealImpl = IEventProducer>
		class IMultiEventProducerImpl_1 : public IMultiEventProducerImplBase<eventlist, IRealImpl>
	{
	public:
		IMultiEventProducerImpl_1() 
		{
		}
		virtual ~IMultiEventProducerImpl_1() {}
		
		// IEventProducer
	public:
		virtual void AddEventHandler(IEvent* pEvent, LPVOID lpParam)
		{
			if(pEvent != NULL)
			{
				MetaTryAddEventHandler(0);
			}
		}
		
		virtual void RemoveEventHandler(IEvent* pEvent)
		{
			if(pEvent != NULL)
			{
				MetaTryRemoveEventHandler(0);
			}
		}		
	};
	
	
	//////////////////////////////////////////////////////////////////////////
	// MultiEventProducerImpl Generator
	
	template<
		int nEventCount,
		class Base,
		class eventlist, 
		class IRealImpl = IEventProducer
		>
		class IMultiEventProducerImplGenerator__ : public Base
	{
		enum 
		{
			nLessEventCount = nEventCount - 1
		};
	public:
		IMultiEventProducerImplGenerator__() {}
		virtual ~IMultiEventProducerImplGenerator__() {}
		
		// IEventProducer
	public:
		virtual void AddEventHandler(IEvent* pEvent, LPVOID lpParam)
		{
			if(pEvent != NULL)
			{
				Base::AddEventHandler(pEvent, lpParam);
				MetaTryAddEventHandler(nLessEventCount);
			}
		}
		
		virtual void RemoveEventHandler(IEvent* pEvent)
		{
			if(pEvent != NULL)
			{
				Base::RemoveEventHandler(pEvent);
				MetaTryRemoveEventHandler(nLessEventCount);
			}
		}		
	};
	
	
	////////////////////////////////////////////////////////////
#if _MSC_VER  > 1200 // >= VC7.0
	
	template<class eventlist,int index,  class IRealImpl>
		class IMultiEventProducerImplGenerator;
	
	template<class eventlist, class IRealImpl>
		class IMultiEventProducerImplGenerator< eventlist,1,  IRealImpl> 
	{
	public:
		typedef typename IMultiEventProducerImpl_1<eventlist, IRealImpl> Result;
	};
	
	template<class eventlist, class IRealImpl, int index>
		class IMultiEventProducerImplGenerator<eventlist, index, IRealImpl>
	{
	public:
		typedef typename IMultiEventProducerImplGenerator<eventlist, index - 1, IRealImpl>::Result Base;
		typedef typename IMultiEventProducerImplGenerator__<index, Base, eventlist, IRealImpl>  Result;
	};
	
#else // = VC6.0
	
	template<class eventlist, int index, class IRealImpl>
		class IMultiEventProducerImplGenerator
	{
		
	private:
        template<int index>
			struct In
        {
			typedef typename IMultiEventProducerImplGenerator<eventlist, index - 1, IRealImpl>::Result Base;
			typedef typename IMultiEventProducerImplGenerator__<index, Base, eventlist, IRealImpl>  Result;
        };
		
        template<>
			struct In<1>
        {
            typedef typename IMultiEventProducerImpl_1<eventlist, IRealImpl> Result;
        };
		
	public:
        typedef typename In<index>::Result Result;
	};
	
	
#endif
	//////////////////////////////////////////////////////////////////////////
	
	template<class eventlist, class IRealImpl>
		class IMultiEventProducerGenerator 
		: public IMultiEventProducerImplGenerator<eventlist, Length<eventlist>::value, IRealImpl>
	{
		
	};
	
	//////////////////////////////////////////////////////////////////////////
	// For 2 event producer
	
	template<class eventlist, class IRealImpl = IEventProducer>
		class IMultiEventProducerImpl_2 : public IMultiEventProducerImpl_1<eventlist, IRealImpl>
	{
	public:
		IMultiEventProducerImpl_2() {}
		virtual ~IMultiEventProducerImpl_2() {}
		
		// IEventProducer
	public:
		virtual void AddEventHandler(IEvent* pEvent, LPVOID lpParam)
		{
			if(pEvent != NULL)
			{
				IMultiEventProducerImpl_1<eventlist, IRealImpl>::AddEventHandler(pEvent, lpParam);
				MetaTryAddEventHandler(1);
			}
		}
		
		virtual void RemoveEventHandler(IEvent* pEvent)
		{
			if(pEvent != NULL)
			{
				IMultiEventProducerImpl_1<eventlist, IRealImpl>::RemoveEventHandler(pEvent);
				MetaTryRemoveEventHandler(1);
			}
		}		
	};
	
#pragma warning(pop)
	
};
