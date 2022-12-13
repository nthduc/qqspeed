#ifndef __TCOMBOBOXCORE_I_H__
#define __TCOMBOBOXCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "uilib/tcombobox_i.h"

namespace Tenio
{
	class ITComboBoxCore;

	TENIO_MAKE_TYPEID_MAP(ITComboBoxCore, COMPONENTID_TCOMBOBOXCORE);

	class ITComboBoxCore : public IControlCore, public ITComboBox
	{
	public:
	};
};

#endif

// InterfaceName:TComboBoxCore
// SubFolderName:uicore
