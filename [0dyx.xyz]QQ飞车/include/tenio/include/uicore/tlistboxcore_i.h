#ifndef __TLISTBOXCORE_I_H__
#define __TLISTBOXCORE_I_H__

#include "../teniointerface.h"
#include "../uilib/tlistbox_i.h"
#include "controlcore_i.h"

namespace Tenio
{
	class ITListBoxCore;

	TENIO_MAKE_TYPEID_MAP(ITListBoxCore, COMPONENTID_TLISTBOXCORE);

	class ITListBoxCore : public IControlCore, public ITListBox
	{
	public:
	};
};

#endif

// InterfaceName:TListBoxCore
// SubFolderName:uicore
