#ifndef __UICONTAINERCORE_I_H__
#define __UICONTAINERCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/control_i.h"

namespace Tenio
{
	class IUIContainerCore;

	TENIO_MAKE_TYPEID_MAP(IUIContainerCore, COMPONENTID_UICONTAINERCORE);

	class IUIContainerCore : 
		public IControlCore, 
		public IControl
	{
	public:
	};
};

#endif

// InterfaceName:UIContainerCore
// SubFolderName:UILib
