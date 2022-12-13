#ifndef __TSCROLLBARCORE_I_H__
#define __TSCROLLBARCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tscrollbar_i.h"

namespace Tenio
{
	class ITScrollBarCore;

	TENIO_MAKE_TYPEID_MAP(ITScrollBarCore, COMPONENTID_TSCROLLBARCORE);

	class ITScrollBarCore : public IControlCore, public ITScrollBar
	{
	public:
	};
};

#endif

// InterfaceName:TScrollBarCore
// SubFolderName:UICore
