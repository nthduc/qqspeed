#ifndef __MGIMAGE2DISPLAYMIXER_I_H__
#define __MGIMAGE2DISPLAYMIXER_I_H__

#include "../teniointerface.h"
#include "../june/mixer_i.h"

namespace Tenio
{
	class IMGImage2DisplayMixer;

	TENIO_MAKE_TYPEID_MAP(IMGImage2DisplayMixer, COMPONENTID_MGIMAGE2DISPLAYMIXER);

	class IMGImage2DisplayMixer : 
		public IJuneMixer,
		public IComponent
	{
	public:

	};
};

#endif

// InterfaceName:MGImage2DisplayMixer
// SubFolderName:November
