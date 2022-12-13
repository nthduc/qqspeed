#ifndef __EVENT_PRODUCER_IMPL_T_H__
#define __EVENT_PRODUCER_IMPL_T_H__

#include <map>

#include "../Include/event_i.h"
#include "../Include/portable.h"

namespace Tenio
{

template
<
	class IEventType = IEvent,
	class IActualEventProducerInterface = IEventProducer
>
class IEventProducerImpl : public IActualEventProducerInterface
{
public:
	IEventProducerImpl() {}
	virtual ~IEventProducerImpl() {}

	// Type definations
private:
	typedef std::map<IEventType*, VOID*>               EventHandler2ParamMap;
	typedef TYPENAME EventHandler2ParamMap::iterator   EventHandler2ParamMapIterator;
	typedef TYPENAME EventHandler2ParamMap::value_type EventHandler2ParamMapValue;

	// IEventProducer
public:
	virtual void AddEventHandler(IEvent* pEvent, LPVOID lpParam)
	{
		if(pEvent != NULL)
		{
			RemoveEventHandler(pEvent);
			m_mapEventHandler2Param.insert(EventHandler2ParamMapValue(
				static_cast<IEventType*>(pEvent), lpParam));
		}
	}

	virtual void RemoveEventHandler(IEvent* pEvent)
	{
		m_mapEventHandler2Param.erase(static_cast<IEventType*>(pEvent));
	}

	// Helper functions
protected:
	inline int GetEventHandlerCount()
	{
		return m_mapEventHandler2Param.size();
	}

	inline void ClearEventHandlers()
	{
		m_mapEventHandler2Param.clear();
	}


	//////////////////////////////////////////////////////////////////////////
	// Helper function

private:

	void CopyEventHandlersBeforeFiring(EventHandler2ParamMap& mapRecurring)
	{
		EventHandler2ParamMapIterator it = m_mapEventHandler2Param.begin();
		for(; it != m_mapEventHandler2Param.end(); ++it)
		{
			mapRecurring.insert(EventHandler2ParamMapValue(it->first, it->second));
		}
	}

	inline bool IsEventHandlerValid(IEventType* pEventHandler)
	{
		return m_mapEventHandler2Param.find(pEventHandler) != m_mapEventHandler2Param.end();
	}

	inline bool PopFromMap(EventHandler2ParamMap& theMap, IEventType*& pEventHandler, VOID*& pParam)
	{
		EventHandler2ParamMapIterator it = theMap.begin();
		pEventHandler = it->first;
		pParam        = it->second;
		theMap.erase(it);
		return IsEventHandlerValid(pEventHandler);
	}

	template<class Pred>
		inline bool PopFromMapCheckParam(EventHandler2ParamMap& theMap, IEventType*& pEventHandler, VOID*& pParam, Pred PredFunc)
	{
		return PopFromMap(theMap, pEventHandler, pParam) && PredFunc(pParam);
	}

	//////////////////////////////////////////////////////////////////////////
	// Event firer

protected:

	// 0 argument
	template<class Type> inline
		void FireEvent(void (Type::*EventName)())
	{
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
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
		EventHandler2ParamMap mapRecurringEventHandler2Param;
		CopyEventHandlersBeforeFiring(mapRecurringEventHandler2Param);

		IEventType* pEventHandler = NULL;
		VOID*       pParam        = NULL;

		while(!mapRecurringEventHandler2Param.empty())
		{
			if(PopFromMapCheckParam(mapRecurringEventHandler2Param, pEventHandler, pParam, PredFunc))
			{
				(pEventHandler->*EventName)(arg1, arg2, arg3, arg4, arg5, static_cast<ParamT>(pParam));
			}
		}
	}

	// Fields
protected:
	EventHandler2ParamMap m_mapEventHandler2Param;
};
};

#endif