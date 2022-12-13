// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef NO_SOUND

#ifndef WIN32SOUNDMANAGER_H
#define WIN32SOUNDMANAGER_H

#include "SoundManager.h"
#include "NiAudioSource.h"

NiSmartPointer(NiAudioSource);


class Win32SoundManager : public SoundManager
{
public:
    Win32SoundManager();
	virtual  ~Win32SoundManager();

    virtual bool Initialize();
    virtual void ToggleSounds();

    virtual NiAVObject* GetListener();

    virtual void Update(float fTime);

	virtual void PlaySound(SoundType type);
	virtual void StopSound(SoundType type);
	virtual void PlayMotorSound(float curSpeed);
	virtual void SetPlaybackRate(SoundType type, long rate);
	virtual void StopAllSound();
	virtual void SetSource(unsigned int unType, NiFixedString strName);

protected:
	NiAudioSourcePtr m_aspSound[MAX_SOUND_STATES];
	bool m_bIsSound;

};

#endif // WIN32SOUNDMANAGER_H

#endif // NO_SOUND
