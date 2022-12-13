#ifndef __TEDITCORE_I_H__
#define __TEDITCORE_I_H__

#include "controlcore_i.h"
#include "../uilib/tedit_i.h"

namespace Tenio
{
	class ITEditCore;

	TENIO_MAKE_TYPEID_MAP(ITEditCore, COMPONENTID_TEDITCORE);

	class ITEditCore : public IControlCore, public ITEdit
	{
	public:
	};
}; // Tenio

#endif // __TEDITCORE_I_H__

// InterfaceName:ITEditCore
// SubFolderName:uicore
