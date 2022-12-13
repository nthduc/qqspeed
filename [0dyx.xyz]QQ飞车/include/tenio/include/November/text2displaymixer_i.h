#ifndef __TEXT2DISPLAYMIXER_I_H__
#define __TEXT2DISPLAYMIXER_I_H__

#include "../teniointerface.h"
#include "../june/mixer_i.h"

namespace Tenio
{
	class IText2DisplayMixer;

	TENIO_MAKE_TYPEID_MAP(IText2DisplayMixer, COMPONENTID_TEXT2DISPLAYMIXER);

	class IText2DisplayMixer : public IComponent, public IJuneMixer
	{
	public:
	};
};

#endif

// InterfaceName:Text2DisplayMixer
// SubFolderName:November
