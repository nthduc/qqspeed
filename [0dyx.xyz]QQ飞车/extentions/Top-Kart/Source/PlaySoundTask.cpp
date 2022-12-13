#include "PlaySoundTask.h"

inline PlaySoundTask::PlaySoundTask()
{

	SetType(Task::TYPE_FRAME);
	// start now, execute only once, for this frame only
	SetTimeParams((float)Scheduler::Get()->GetClock().GetFrame(), 
		1.0f, 0.0f); 
}


inline void PlaySoundTask::Execute(unsigned int uiFrameCount, float fTime)
{
	EventContainer::SoundEvent soundevent;
	while(soundevent = EventContainer::Get()->PopSoundEvent())
	{
		LevelManager::Get()->GetSoundManager()->PlaySound(soundevent);
	}
}