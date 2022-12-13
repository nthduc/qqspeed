#ifndef __TSCROLLVIEWCORE_I_H__
#define __TSCROLLVIEWCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/TScrollView_i.h"

namespace Tenio
{
	class ITScrollViewCore;

	TENIO_MAKE_TYPEID_MAP(ITScrollViewCore, COMPONENTID_TSCROLLVIEWCORE);

	class ITScrollViewCore : public IControlCore, public ITScrollView
	{
	public:
	};
};

#endif

// InterfaceName:TScrollViewCore
// SubFolderName:uicore
