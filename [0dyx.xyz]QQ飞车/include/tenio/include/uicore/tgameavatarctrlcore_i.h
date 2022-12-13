#ifndef __TGAMEAVATARCTRLCORE_I_H__
#define __TGAMEAVATARCTRLCORE_I_H__

#include "uicore/controlcore_i.h"
#include "tgameavatarctrl_i.h"

namespace Tenio
{
	class ITGameAvatarCtrlCore;

	TENIO_MAKE_TYPEID_MAP(ITGameAvatarCtrlCore, COMPONENTID_TGAMEAVATARCTRLCORE);
	TENIO_MAKE_EVENT_MAP(ITGameAvatarCtrlCore, IGameAvatarCtrlEvent);

	class ITGameAvatarCtrlCore : public IControlCore, public ITGameAvatarCtrl
	{
	public:
	};
};

#endif

// InterfaceName:tgameavatarctrlCore
// SubFolderName:uicore
