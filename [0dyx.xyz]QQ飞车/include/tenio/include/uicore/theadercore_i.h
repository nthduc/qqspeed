#ifndef __THEADERCORE_I_H__
#define __THEADERCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tHeader_i.h"

namespace Tenio
{
	class ITHeaderCore;

	TENIO_MAKE_TYPEID_MAP(ITHeaderCore, COMPONENTID_THEADERCORE);

	class ITHeaderCore : public IControlCore, public ITHeader
	{
	public:
	};
};

#endif

// InterfaceName:THeaderCore
// SubFolderName:uicore
