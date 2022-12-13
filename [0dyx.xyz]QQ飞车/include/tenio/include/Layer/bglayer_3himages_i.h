#ifndef __BGLAYER_3HIMAGES_I_H__
#define __BGLAYER_3HIMAGES_I_H__

#include "teniointerface.h"
#include "bglayer_i.h"

namespace Tenio
{
	class IBGLayer_3hImages;
	TENIO_MAKE_TYPEID_MAP(IBGLayer_3hImages, COMPONENTID_BG3HORZIMAGES);
	//////////////////////////////////////////////////////////////////////////
	class IBGLayer_3hImages : 
		public IBGLayer,
		public IComponent
	{
	};
}

#endif