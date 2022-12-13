#ifndef __TRICHEDITCORE_I_H__
#define __TRICHEDITCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/trichedit_i.h"

namespace Tenio
{
	class ITRichEditCore;

	TENIO_MAKE_TYPEID_MAP(ITRichEditCore, COMPONENTID_TRICHEDITCORE);

	class ITRichEditCore : public IControlCore, public ITRichEdit
	{
	public:
	};
};

#endif
