#ifndef __TLAYERSTATESWITCH_I_H__
#define __TLAYERSTATESWITCH_I_H__

#include "../teniointerface.h"
#include "layer/layer_i.h"
#include "Layer/multistate_i.h"

namespace Tenio
{
	#define INVALID_REGION   -1
	
	typedef struct _tagREGIONINFO
	{
		RECT rcRegion;
		IJuneLayer** arpLayer;
		IMultiState** arpMultiState;
		UINT uLayerCount;
	}REGIONINFO;

	class ITLayerStateSwitch;

	TENIO_MAKE_TYPEID_MAP(ITLayerStateSwitch, COMPONENTID_TLAYERSTATESWITCH);

	class ITLayerStateSwitch : public IComponent, public IEventProducer
	{
	public:
		virtual UINT AddRegion(REGIONINFO rcRgnInfo) = 0;
		virtual RECT ChangeRegion(UINT nRegionID, RECT rcNewRegion) = 0;
		virtual BOOL RemoveRegion(UINT nRegionID) = 0;
		virtual void OnMessage(UINT uMessage, WPARAM wParam, LPARAM lParam) = 0;
		virtual UINT GetRegionIDFromPoint(POINT pt) = 0;
		virtual RECT* GetRegionFromID(UINT uRegionID) = 0;
	};

	class ITLayerStateSwitchEvent : public IEvent
	{
	public:
		virtual void OnLButtonClick(UINT uRegionID) = 0;
	};

};

#endif

// InterfaceName:TLayerStateSwitch
// SubFolderName:uilib
