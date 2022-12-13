#ifndef __BGLAYER_COLOR_I_H__
#define __BGLAYER_COLOR_I_H__

#include "teniointerface.h"
#include "bglayer_i.h"

namespace Tenio
{
	class IBGLayer_color;
	TENIO_MAKE_TYPEID_MAP(IBGLayer_color, COMPONENTID_BGCOLOR);
	//////////////////////////////////////////////////////////////////////////
	class IBGLayer_color : 
		public IBGLayer,
		public IComponent
	{
	};
}

#endif