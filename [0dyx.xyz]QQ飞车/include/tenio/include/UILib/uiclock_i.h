#ifndef __UICLOCK_I_H__
#define __UICLOCK_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IUIClock;

	TENIO_MAKE_TYPEID_MAP(IUIClock, COMPONENTID_UICLOCK);

	class IUIClock : 
		public IComponent,
		public IEventProducer
	{
	public:
		virtual void SetUIClockInterval(DWORD dwMilliSecond) = 0;
		virtual DWORD GetUIClockInterval() = 0;

		virtual BOOL Start() = 0;
		virtual void Stop() = 0;

		virtual DWORD SetInterval(DWORD dwMilliSeconds, DWORD dwParam) = 0;
		virtual void ClearInterval(DWORD dwTimerID) = 0;
	};

	class IUIClockEvent : public IEvent
	{
	public:
		virtual void OnUIClock() = 0;
		virtual void OnInterval(DWORD dwTimerID, DWORD dwParam) = 0;
	};
};

#endif

// InterfaceName:UIClock
// SubFolderName:uilib
