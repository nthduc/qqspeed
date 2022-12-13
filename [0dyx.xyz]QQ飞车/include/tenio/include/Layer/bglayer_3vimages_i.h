#ifndef __BGLAYER_3VIMAGES_I_H__
#define __BGLAYER_3VIMAGES_I_H__

#include "teniointerface.h"
#include "bglayer_i.h"

namespace Tenio
{
	class IBGLayer_3vImages;
	TENIO_MAKE_TYPEID_MAP(IBGLayer_3vImages, COMPONENTID_BG3VERTIMAGES);
	//////////////////////////////////////////////////////////////////////////
	class IBGLayer_3vImages : 
		public IBGLayer,
		public IComponent
	{
	};
}

#endif