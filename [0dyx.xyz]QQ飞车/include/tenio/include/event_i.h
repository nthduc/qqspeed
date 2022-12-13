#ifndef __EVENT_I_H__
#define __EVENT_I_H__

namespace Tenio
{
	class IEvent;
	class IEventProducer;

	class IEvent
	{
	public:
		virtual int GetEventVersion() = 0;
	};

	class IEventProducer
	{
	public:
		virtual void AddEventHandler(IEvent* pEvent, LPVOID lpParam) = 0;
		virtual void RemoveEventHandler(IEvent* pEvent) = 0;
	};
};

#endif
