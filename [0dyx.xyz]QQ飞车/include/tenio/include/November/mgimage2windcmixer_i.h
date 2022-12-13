#ifndef __MGIMAGE2WINDCMIXER_I_H__
#define __MGIMAGE2WINDCMIXER_I_H__

#include "../teniointerface.h"
#include "../june/mixer_i.h"

namespace Tenio
{
	class IMGImage2WinDCMixer;

	TENIO_MAKE_TYPEID_MAP(IMGImage2WinDCMixer, COMPONENTID_MGIMAGE2WINDCMIXER);

	class IMGImage2WinDCMixer : 
		public IJuneMixer,
		public IComponent	
	{
	public:
	};
};

#endif

// InterfaceName:MGImage2WinDCMixer
// SubFolderName:November
