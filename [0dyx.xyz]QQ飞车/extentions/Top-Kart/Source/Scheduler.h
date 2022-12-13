//
// Based directly on the "Scheduling Game Events" Gem from 
// Game Programming Gems 3, which requires:
//
// Intel Open Source License for Game Scheduler Gem sample code
// Copyright (c) 2002, Intel Corporation.
// All rights reserved. 
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"
#include "Clock.h"

class Scheduler : public NiMemObject
{
public:
    static bool Create();
    static void Destroy();

    static Scheduler* Get();

	bool IsRunning() const;
	void Run();
	void Stop();
	void ExecuteFrame();

	void InsertTask(Task* pkTask);
	void GetNextTimeTask(Task*& pkTask);
	void GetNextFrameTask(Task*& pkTask);

	void Schedule(Task* pkTask);
	void Terminate(unsigned int uiID);

	void SweepGarbage();

    float GetFrameLength() const;

	Clock& GetClock();

protected:
	Scheduler();
	~Scheduler();

    enum 
    { 
        RENDER_TASK_ID = 1 
    };

    Task* m_pkRenderTask;
    Task* m_pkTimeTaskList;
    Task* m_pkFrameTaskList;
    unsigned int m_uiNextID;

    float m_fFrameLength;

    Clock m_kClock;

    static Scheduler* ms_pkTheScheduler;
};

#include "Scheduler.inl"

#endif
