

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#if defined(PS2) 
#define NO_SOUND
#endif

#include <NiSmartPointer.h>

class NiAVObject;
class NiNode;

class SoundManager : public NiMemObject
{
public:
	enum SoundType
	{

		ST_NONE = 0,
		ST_AMBIENT,
		ST_ROOM,
		ST_MOTOR_LOW,
		ST_MOTOR_MID,
		ST_MOTOR_HIGH,
		ST_MOTOR_STOP,
		ST_BRAKE,
		ST_SHIFT,
		ST_HIT,	
		ST_SPEEDUP,
		ST_CARBARN,
		ST_WAIT_START,
		ST_GO,
		MAX_SOUND_STATES
	};

    SoundManager();
    virtual ~SoundManager();

    virtual bool Initialize();
    virtual void ToggleSounds();

    virtual NiAVObject* GetListener();

    virtual void Update(float fTime);

	virtual void PlaySound(SoundType type);
	virtual void StopSound(SoundType type);
	virtual void SetPlaybackRate(SoundType type, long rate);
	virtual void PlayMotorSound(float curSpeed);
	virtual void StopAllSound();
	virtual void SetSource(unsigned int unType, NiFixedString strName);

};

#endif // SOUNDMANAGER_H
