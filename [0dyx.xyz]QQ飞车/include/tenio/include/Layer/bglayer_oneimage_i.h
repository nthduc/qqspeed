#ifndef __BGLAYER_ONEIMAGE_I_H__
#define __BGLAYER_ONEIMAGE_I_H__

#include "../teniointerface.h"
#include "bglayer_i.h"

namespace Tenio
{
	typedef enum _tagONEIMAGEBGLAYERPAINTMODE
	{
		OBP_TILE = 1,
		OBP_STRETCH,
		OBP_CENTER
	}ONEIMAGEBGLAYERPAINTMODE;
	
	class IBGLayer_oneImage;

	TENIO_MAKE_TYPEID_MAP(IBGLayer_oneImage, COMPONENTID_BGLAYER_ONEIMAGE);

	class IBGLayer_oneImage : 
		public IBGLayer,
		public IComponent
	{
	public:
	};
};

#endif

// InterfaceName:BGLayer_oneImage
// SubFolderName:layer
