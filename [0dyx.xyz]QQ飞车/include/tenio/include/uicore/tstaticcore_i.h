#ifndef __TSTATICCORE_I_H__
#define __TSTATICCORE_I_H__

#include "controlcore_i.h"
#include "../uilib/tstatic_i.h"

namespace Tenio
{
	class ITStaticCore;

	TENIO_MAKE_TYPEID_MAP(ITStaticCore, COMPONENTID_TSTATICCORE);

	class ITStaticCore : public IControlCore, public ITStatic
	{
	public:
	};
};

#endif

// InterfaceName:TStaticCore
// SubFolderName:uicore
