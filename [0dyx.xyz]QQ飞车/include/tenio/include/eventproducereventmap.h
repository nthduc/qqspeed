#ifndef __TENIO_EVENTPRODUCER_EVENT_MAP_H__
#define __TENIO_EVENTPRODUCER_EVENT_MAP_H__

#include "event_i.h"

namespace Tenio
{
	template<class ProducerType>
	class Producer2Event;
};

#define TENIO_MAKE_EVENT_MAP(producertype, eventtype) \
template<>                                      \
	class Producer2Event<producertype>          \
	{                                           \
	public:                                     \
		typedef eventtype EventType;            \
	};                                          \

#endif
