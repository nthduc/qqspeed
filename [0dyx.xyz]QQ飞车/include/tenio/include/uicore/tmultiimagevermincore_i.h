#ifndef __TMULTIIMAGEVERMINCORE_I_H__
#define __TMULTIIMAGEVERMINCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tmultiimagevermin_i.h"

namespace Tenio
{
	class ITMultiImageVerminCore;

	TENIO_MAKE_TYPEID_MAP(ITMultiImageVerminCore, COMPONENTID_TMULTIIMAGEVERMINCORE);

	class ITMultiImageVerminCore : public IControlCore, public ITMultiImageVermin
	{
	public:
	};
};

#endif

// InterfaceName:TMultiImageVerminCore
// SubFolderName:uicore
