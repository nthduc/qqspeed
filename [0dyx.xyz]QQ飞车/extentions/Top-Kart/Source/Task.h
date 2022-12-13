//
// Based directly on the "Scheduling Game Events" Gem from 
// Game Programming Gems 3, which requires:
//
// Intel Open Source License for Game Scheduler Gem sample code
// Copyright (c) 2002, Intel Corporation.
// All rights reserved. 
//

#ifndef TASK_H
#define TASK_H

#include <NiMemObject.h>

class Task : public NiMemObject
{
public:
    Task();
    virtual ~Task();

	virtual void Execute(unsigned int uiFrameCount, float fTime) = 0;

    // TaskType is only used by Scheduler::Schedule to 
    // determine what list to put the Tasks on
    enum TaskType 
    { 
        TYPE_TIME, 
        TYPE_FRAME, 
        TYPE_RENDER 
    };
    void SetType(TaskType eType);
    TaskType GetType() const;

    // Set and Get the time based parameters for the task.  
    // For frame events the values should be in units of frames.
    // For time events the values should be in seconds.
    // Start is a absolute frame or time value.
    // Period represents the recurrence of the task, in frames
    // or seconds.  A value of 3 would execute every 3rd frame/3rd second.
    // Duration is a relative value such that a task's End time is computed
    // as (start + duration)
    // In the Intel code the End time was set to be (more correctly) 
    // (start + duration - 1) -- but that would only work with frame Tasks
    // given the use of floating point Clock like we have here.
    void SetTimeParams(float fStart, float fPeriod, float fDuration);
    void GetTimeParams(float& fStart, float& fPeriod, float& fDuration) const;

    // The ID is set by the scheduler, each ID is unique
    unsigned int GetID() const;

protected:

    enum TaskState 
    { 
        STATE_ACTIVE, 
        STATE_PAUSE, 
        STATE_DELETE 
    };

	Task* m_pkNext;
	TaskState m_eStatus; 
	TaskType m_eType; 

	unsigned int m_uiID;

    float m_fStart;
    float m_fEnd;
    float m_fPeriod;
    float m_fDuration;
    float m_fLast;
    float m_fNext;

    friend class Scheduler;
};

#include "Task.inl"

#endif
