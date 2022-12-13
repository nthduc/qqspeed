#ifndef __TABLECORE_I_H__
#define __TABLECORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/Table_i.h"

namespace Tenio
{
	class ITableCore;

	TENIO_MAKE_TYPEID_MAP(ITableCore, COMPONENTID_TABLECORE);

	class ITableCore : public IControlCore, public ITable
	{
	public:
	};
};

#endif

// InterfaceName:TableCore
// SubFolderName:uicore
