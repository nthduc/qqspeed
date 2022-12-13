#ifndef __TLABELCORE_I_H__
#define __TLABELCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tlabel_i.h"

namespace Tenio
{
	class ITLabelCore;

	TENIO_MAKE_TYPEID_MAP(ITLabelCore, COMPONENTID_TLABELCORE);

	class ITLabelCore :public IControlCore, public ITLabel
	{
	public:
	};
};

#endif

// InterfaceName:TLabelCore
// SubFolderName:uicore
