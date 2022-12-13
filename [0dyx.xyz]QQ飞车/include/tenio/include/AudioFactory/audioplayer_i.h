#ifndef __AUDIOPLAYER_I_H__
#define __AUDIOPLAYER_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IAudioPlayer;

	TENIO_MAKE_TYPEID_MAP(IAudioPlayer, COMPONENTID_AUDIOPLAYER);

	class IAudioPlayer : public IComponent
	{
	public:
		virtual int PlaySound(LPCTSTR lpszFileName) = 0;
		virtual void StopSound(int) = 0;
	};
};

#endif

// InterfaceName:AudioPlayer
// SubFolderName:AudioFactory
