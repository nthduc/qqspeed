#ifndef __TLISTCORE_I_H__
#define __TLISTCORE_I_H__

#include "../teniointerface.h"
#include "../uilib/tlist_i.h"
#include "../uicore/controlcore_i.h"

namespace Tenio
{
	class ITListCore;

	TENIO_MAKE_TYPEID_MAP(ITListCore, COMPONENTID_TLISTCORE);

	class ITListCore : public IControlCore, public ITList
	{
	public:

	};
};

#endif

// InterfaceName:TListCore
// SubFolderName:uilib
