
#pragma once

#pragma warning(disable:4786)

#include <map>
using namespace std;
#include "event_i.h"
#include "portable.h"

namespace Tenio
{
	#define TENIO_MAKE_EVENTHANDLER_MAP(event)												\
	template<>																				\
	class EventHandler<event>																\
	{																						\
	public:																					\
		typedef std::map<event*, VOID*>				       EventHandler2ParamMap;			\
		typedef std::map<long, EventHandler2ParamMap>		EventHandler2ParamMapMap;		\
		typedef  EventHandler2ParamMap::iterator   EventHandler2ParamMapIterator;			\
		typedef  EventHandler2ParamMap::value_type EventHandler2ParamMapValue;				\
																							\
		static EventHandler2ParamMapMap& GetMapMap()										\
		{																					\
			static EventHandler2ParamMapMap m_EventHandler2ParamMapMap;						\
			return m_EventHandler2ParamMapMap;												\
		}																					\
																							\
		static EventHandler2ParamMap& GetMap(long thisAddress)								\
		{																					\
			EventHandler2ParamMapMap &m_EventHandler2ParamMapMap = GetMapMap();				\
			return m_EventHandler2ParamMapMap[thisAddress];									\
		}																					\
	};																						\
	
	

	template<class event>
		class EventHandler;
};