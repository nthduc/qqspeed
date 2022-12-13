#ifndef __TCHECKBOXCORE_I_H__
#define __TCHECKBOXCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tcheckbox_i.h"

namespace Tenio
{
	class ITCheckBoxCore;

	TENIO_MAKE_TYPEID_MAP(ITCheckBoxCore, COMPONENTID_TCHECKBOXCORE);

	/**
	 * @ingroup tenuilib
	 * @brief uilib checkbox¿Ø¼þºËÐÄ
	 * @sa IControlCore , ITCheckBox
	 */

	class ITCheckBoxCore : public IControlCore, public ITCheckBox
	{
	public:
	};
};

#endif

// InterfaceName:TCheckBoxCore
// SubFolderName:Include\uicore
