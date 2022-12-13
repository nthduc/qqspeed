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
	//  ����һ������, �ô��ڵ�������Ϣ�����͵� pEventHandler
	//-----------------------------------------------------------------------------

		virtual BOOL CreateDispatchWindow(HWND* phWin,
			IWindowMessageDispatcherEvent* pEventHandler) = 0;

	//-----------------------------------------------------------------------------
	//  ����һ������, �ô��ڵ�������Ϣ�����͵� pEventHandler
	//-----------------------------------------------------------------------------

		virtual BOOL CreateStyleWindow(DWORD dwStyle, DWORD dwStyleEx,
			HWND hParent, RECT* prcWindow, HWND* phWin, 
			IWindowMessageDispatcherEvent* pEventHandler) = 0;
	};

	class IWindowMessageDispatcherEvent : public IEvent
	{
	public:

	//-----------------------------------------------------------------------------
	//  �յ� Window ������Ϣ, ���� TRUE ��ʾ����Ҫ����Ĭ�ϵĴ�����. pnResult ��
	//  ���÷���ֵ
	//-----------------------------------------------------------------------------

		virtual BOOL OnWindowMessage(HWND hWin, UINT uMessageID, WPARAM wParam, 
			LPARAM lParam, LRESULT* pnResult) = 0;
	};
}
#endif