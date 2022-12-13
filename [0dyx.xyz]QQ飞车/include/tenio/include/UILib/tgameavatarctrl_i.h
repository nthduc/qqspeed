#ifndef __GAMEAVATARCTRL_I_H__
#define __GAMEAVATARCTRL_I_H__

#include "control_i.h"
#include "../Layer/mifimagelayer_i.h"

namespace Tenio
{
	class ITGameAvatarCtrl;
	class IGameAvatarCtrlEvent;

	TENIO_MAKE_TYPEID_MAP(ITGameAvatarCtrl, COMPONENTID_TGAMEAVATARCTRL);

	class ITGameAvatarCtrl : 
						public IControl,
						public IEventProducer
	{
	public:
		virtual void StartPlay(BOOL bLoop = FALSE) = 0;
		virtual void StopPlay() = 0;

		virtual void AddImageLayer(DWORD dwRefLayer, LPCTSTR lpszFileName) = 0;
		virtual void RemoveImageLayer(DWORD dwRefLayer) = 0;
		virtual void RemoveAllLayer() = 0;
		virtual void PlayItem(LPCTSTR lpszItem) = 0;

		virtual void ShowAvatarLayer(BOOL bShow) = 0;
		virtual void ShowItemLayer(BOOL bShow) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// 选择目标的事件
	//	1 mouse moving, game trigger OnMouseMove event
	//	2 select an object , game trigger OnSelectTarget event
	class IGameAvatarCtrlEvent : public IEvent
	{
	public:
		virtual void OnMouseMove(POINT pt){};
		virtual void OnMouseLeave(POINT pt){};
		virtual void OnSelectTarget(POINT pt){};
		virtual void OnCancelSelect(){};

		virtual void OnFrameChange(LONG nFrame){};
		virtual void OnFrameTrigger(LONG nFrame){};
		virtual void OnAnimateStart(){};
		virtual void OnAnimateEnd(){};
	};
};

#endif

// InterfaceName:GameAvatarCtrl
// SubFolderName:UILib
