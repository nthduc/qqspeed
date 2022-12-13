#ifndef __REGISTEREVENT_T_H__
#define __REGISTEREVENT_T_H__

#include "globalobjectmanager_t.h"

namespace Tenio
{
template<class IEventProducerInterface>
class TRegisterEvent
{
public:
	TRegisterEvent(Producer2Event<IEventProducerInterface>::EventType* pEventHandler,
		LPVOID lpParam = 0)
	{
		TGlobalObject<IEventProducerInterface> spEventProducer;
		if(spEventProducer != NULL)
		{
			spEventProducer->AddEventHandler(pEventHandler, lpParam);
		}
	};
}; // TRegisterEvent

template<class IEventProducerInterface>
class TUnregisterEvent
{
public:
	TUnregisterEvent(Producer2Event<IEventProducerInterface>::EventType* pEventHandler)
	{
		TGlobalObject<IEventProducerInterface> spEventProducer;
		if(spEventProducer != NULL)
		{
			spEventProducer->RemoveEventHandler(pEventHandler);
		}
	}
}; // TUnregisterEvent

}; //namespace

#endif // __REGISTEREVENT_T_H__