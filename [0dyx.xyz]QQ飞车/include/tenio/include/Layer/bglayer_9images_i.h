#ifndef __BGLAYER_9IMAGES_I_H__
#define __BGLAYER_9IMAGES_I_H__

#include "teniointerface.h"
#include "bglayer_i.h"

namespace Tenio
{
	class IBGLayer_9Images;
	TENIO_MAKE_TYPEID_MAP(IBGLayer_9Images, COMPONENTID_BG9IMAGES);
	//////////////////////////////////////////////////////////////////////////
	class IBGLayer_9Images : 
		public IBGLayer,
		public IComponent		
	{
	};
}

#endif