#ifndef __GAMEAVATARLAYER_I_H__
#define __GAMEAVATARLAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"

namespace Tenio
{
	class IGameAvatarLayer;
	class IGameAvatarAnimateEvent;

	TENIO_MAKE_TYPEID_MAP(IGameAvatarLayer, COMPONENTID_GAMEAVATARLAYER);
	TENIO_MAKE_EVENT_MAP(IGameAvatarLayer, IGameAvatarAnimateEvent);

	class IGameAvatarLayer : 
			public IJuneLayer,
			public IEventProducer,
			public IComponent
	{
	public:
		virtual void StartPlay(BOOL bLoop = FALSE) = 0;
		virtual void StopPlay() = 0;

		virtual void AddImageLayer(DWORD dwRefLayer, LPCTSTR lpszFileName) = 0;
		virtual void RemoveImageLayer(DWORD dwRefLayer) = 0;
		virtual void RemoveAllLayer() = 0;
	};

	class IGameAvatarAnimateEvent : public IEvent
	{
	public:
		/**
		 * @brief Ö¡ÊÂ¼þ
		 */
		virtual void UpdateAvatarLayer() = 0;
	};
};

#endif

// InterfaceName:GameAvatarLayer
// SubFolderName:Layer
