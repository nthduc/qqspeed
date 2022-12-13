#ifndef HUDTASKS_H
#define HUDTASKS_H

#include "Task.h"
/*
//----------------------------------------------------------------------------
class MenuTask: public Task
{
public:
    MenuTask();
	virtual void Execute(unsigned int uiFrameCount, float fTime);
protected:

};
*/
//----------------------------------------------------------------------------
class FrameRateTask: public Task
{
public:
    FrameRateTask();
	virtual void Execute(unsigned int uiFrameCount, float fTime);
protected:

};
//----------------------------------------------------------------------------

#include "HUDTasks.inl"

#endif // HUDTASKS_H
