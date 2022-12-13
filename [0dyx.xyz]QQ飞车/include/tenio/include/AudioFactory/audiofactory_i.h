#ifndef __AUDIOFACTORY_I_H__
#define __AUDIOFACTORY_I_H__

#include "../teniointerface.h"
#include "audioplayer_i.h"

namespace Tenio
{
	class IAudioFactory;

	TENIO_MAKE_TYPEID_MAP(IAudioFactory, COMPONENTID_AUDIOFACTORY);

	class IAudioFactory : public IComponent
	{
	public:
		virtual IAudioPlayer* GetAudioPlayer() = 0;
		virtual void SetAudioPlayer(IAudioPlayer* pAudioPlayer) = 0;
		virtual BOOL PlaySound(LPCTSTR lpszFileName) = 0;		
	};
};

#endif

// InterfaceName:AudioFactory
// SubFolderName:AudioFactory
