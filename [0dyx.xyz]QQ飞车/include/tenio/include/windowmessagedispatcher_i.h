#ifndef __WINDOWMESSAGEDISPATCHER_I_H__
#define __WINDOWMESSAGEDISPATCHER_I_H__

#include "teniointerface.h"
//////////////////////////////////////////////////////////////////////////
namespace Tenio
{
	class IWindowMessageDispatcher;
	class IWindowMessageDispatcherEvent;
	TENIO_MAKE_TYPEID_MAP(IWindowMessageDispatcher, COMPONENTID_WINDOWMESSAGEDISPATCHER)
	TENIO_MAKE_EVENT_MAP(IWindowMessageDispatcher, IWindowMessageDispatcherEvent)

	class IWindowMessageDispatcher : 
		public IComponent,
		public IEventProducer
	{
	public:

	//-----------------------------------------------------------------------------
	//  创建一个窗口, 该窗口的所有消息都发送到 pEventHandler
	//-----------------------------------------------------------------------------

		virtual BOOL CreateDispatchWindow(HWND* phWin,
			IWindowMessageDispatcherEvent* pEventHandler) = 0;

	//-----------------------------------------------------------------------------
	//  创建一个窗口, 该窗口的所有消息都发送到 pEventHandler
	//-----------------------------------------------------------------------------

		virtual BOOL CreateStyleWindow(DWORD dwStyle, DWORD dwStyleEx,
			HWND hParent, RECT* prcWindow, HWND* phWin, 
			IWindowMessageDispatcherEvent* pEventHandler) = 0;
	};

	class IWindowMessageDispatcherEvent : public IEvent
	{
	public:

	//-----------------------------------------------------------------------------
	//  收到 Window 窗口消息, 返回 TRUE 表示不需要调用默认的处理函数. pnResult 中
	//  设置返回值
	//-----------------------------------------------------------------------------

		virtual BOOL OnWindowMessage(HWND hWin, UINT uMessageID, WPARAM wParam, 
			LPARAM lParam, LRESULT* pnResult) = 0;
	};
}
#endif