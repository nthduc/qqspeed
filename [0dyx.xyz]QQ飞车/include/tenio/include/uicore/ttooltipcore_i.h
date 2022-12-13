#ifndef __TTOOLTIPCORE_I_H__
#define __TTOOLTIPCORE_I_H__

#include "../teniointerface.h"
#include "../uilib/ttooltip_i.h"
#include "controlcore_i.h"

namespace Tenio
{
	class ITToolTipCore;

	TENIO_MAKE_TYPEID_MAP(ITToolTipCore, COMPONENTID_TTOOLTIPCORE);

	class ITToolTipCore : public ITToolTip, public IControlCore
	{
	public:
	};
};

#endif

// InterfaceName:TToolTipCore
// SubFolderName:uicore
