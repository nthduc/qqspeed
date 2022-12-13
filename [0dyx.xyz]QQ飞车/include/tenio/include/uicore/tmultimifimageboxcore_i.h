#ifndef __TMULTIMIFIMAGEBOXCORE_I_H__
#define __TMULTIMIFIMAGEBOXCORE_I_H__

#include "uicore/controlcore_i.h"
#include "tmultimifimagebox_i.h"

namespace Tenio
{
	class ITMultiMifImageBoxCore;

	TENIO_MAKE_TYPEID_MAP(ITMultiMifImageBoxCore, COMPONENTID_TMULTIMIFIMAGEBOXCORE);

	class ITMultiMifImageBoxCore : public IControlCore, public ITMultiMifImageBox
	{
	public:
	};
};

#endif

// InterfaceName:TMultiMifImageBoxCore
// SubFolderName:uilib
