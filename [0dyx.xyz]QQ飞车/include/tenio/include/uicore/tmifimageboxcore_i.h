#ifndef __TMIFIMAGEBOXCORE_I_H__
#define __TMIFIMAGEBOXCORE_I_H__

#include "uicore/controlcore_i.h"
#include "tmifimagebox_i.h"

namespace Tenio
{
	class ITMifImageBoxCore;

	TENIO_MAKE_TYPEID_MAP(ITMifImageBoxCore, COMPONENTID_TMIFIMAGEBOXCORE);

	class ITMifImageBoxCore : public IControlCore, public ITMifImageBox
	{
	public:
	};
};

#endif

// InterfaceName:TMifImageBoxCore
// SubFolderName:UILibCore
