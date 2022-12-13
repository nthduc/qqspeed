#ifndef PlaySoundTask_H
#define PlaySoundTask_H

#include "Task.h"

class PlaySoundTask :public Task
{
public:
	PlaySoundTask();
	virtual void Execute(unsigned int uiFrameCount, float fTime);
};

#endif