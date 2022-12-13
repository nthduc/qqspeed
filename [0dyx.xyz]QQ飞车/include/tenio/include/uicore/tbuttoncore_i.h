#ifndef __TBUTTONCORE_I_H__
#define __TBUTTONCORE_I_H__

#include "controlcore_i.h"
#include "../uilib/tbutton_i.h"

namespace Tenio
{
	class ITButtonCore;
	TENIO_MAKE_TYPEID_MAP(ITButtonCore, COMPONENTID_TBUTTONCORE);
	class ITButtonCore : 
		public IControlCore,
		public ITButton
	{
	public:
	};
}
#endif