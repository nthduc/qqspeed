#pragma once
#include "../task.h"

class AccelTask :
	public Task
{
public:
	AccelTask(float fLife, float fGear);
	virtual ~AccelTask(void);
	virtual void Execute(unsigned int uiFrameCount, float fTime){}
};
