#ifndef __IDIBMIXER_I_H__
#define __IDIBMIXER_I_H__

#include "../teniointerface.h"
#include "../June/mixer_i.h"

namespace Tenio
{
	class IDIBMixer;

	TENIO_MAKE_TYPEID_MAP(IDIBMixer, COMPONENTID_IDIBMIXER);

	class IDIBMixer : public IJuneMixer, public IComponent
	{
	public:
	};
};

#endif

// InterfaceName:IDIBMixer
// SubFolderName:MGClock
