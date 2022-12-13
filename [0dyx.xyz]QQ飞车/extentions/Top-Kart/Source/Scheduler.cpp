

//
// Based directly on the "Scheduling Game Events" Gem from 
// Game Programming Gems 3, which requires:
//
// Intel Open Source License for Game Scheduler Gem sample code
// Copyright (c) 2002, Intel Corporation.
// All rights reserved. 
//

#include "Top-KartPCH.h"

#include <NiSystem.h>
#include "Scheduler.h"
#include "GameMain.h"


Scheduler* Scheduler::ms_pkTheScheduler = 0;

//---------------------------------------------------------------------------
bool Scheduler::Create()
{
    if (!ms_pkTheScheduler)
    {
        ms_pkTheScheduler = NiNew Scheduler;

        if (!ms_pkTheScheduler)
            return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void Scheduler::Destroy()
{
    NiDelete ms_pkTheScheduler;
    ms_pkTheScheduler = 0;
}
//---------------------------------------------------------------------------
Scheduler::Scheduler()
{
	m_pkTimeTaskList = 0;
	m_pkFrameTaskList = 0;
	m_pkRenderTask = 0;
    m_fFrameLength = 0.0f;

	m_uiNextID = RENDER_TASK_ID + 1;
}
//---------------------------------------------------------------------------
Scheduler::~Scheduler()
{
    // Clean up and residual Tasks -- the lists clean themselves up
	NiDelete m_pkTimeTaskList;
	NiDelete m_pkFrameTaskList;
    NiDelete m_pkRenderTask;
}
//---------------------------------------------------------------------------
void Scheduler::InsertTask(Task* pkTask)
{
	//
	// Tasks are inserted into list in time order
	//
    
    Task* pkList;
    if (pkTask->m_eType == Task::TYPE_TIME)
        pkList = m_pkTimeTaskList;
    else
        pkList = m_pkFrameTaskList;

	if (!pkList)
	{
        // No list
		pkList = pkTask;
        pkList->m_pkNext = 0;

        if (pkTask->m_eType == Task::TYPE_TIME)
        {
            m_pkTimeTaskList = pkList;
        }
        else
        {
            m_pkFrameTaskList = pkList;
        }
	}
	else if (pkList->m_fNext > pkTask->m_fNext)
	{
        // Insert at head
        if (pkTask->m_eType == Task::TYPE_TIME)
        {
		    pkTask->m_pkNext = m_pkTimeTaskList;
		    m_pkTimeTaskList = pkTask;
        }
        else
        {
		    pkTask->m_pkNext = m_pkFrameTaskList;
		    m_pkFrameTaskList = pkTask;
        }
	}
	else
	{
        // Insert elsewhere
		Task* pkTemp = pkList;

		while (pkTemp)
		{
			if (pkTemp->m_pkNext == 0)
			{
				pkTask->m_pkNext = 0;
				pkTemp->m_pkNext = pkTask;
				break;
			}
			else if (pkTemp->m_pkNext->m_fNext > pkTask->m_fNext)
			{
				pkTask->m_pkNext = pkTemp->m_pkNext;
				pkTemp->m_pkNext = pkTask;
				break;
			}

			pkTemp = pkTemp->m_pkNext;
		}
	}
}
//---------------------------------------------------------------------------
void Scheduler::GetNextFrameTask(Task*& pkTask)
{
	// return the next frame task. If no frame task is scheduled to run 
    // this frame, return null. note that this method REMOVES the task 
    // from the list, so the caller is responsible for either adding it 
    // back in (rescheduling) or deleting it.
    
	Task* pkNextTask = m_pkFrameTaskList;

	if (pkNextTask && pkNextTask->m_fNext <= m_kClock.GetFrame())
	{
		m_pkFrameTaskList = pkNextTask->m_pkNext;
		pkNextTask->m_pkNext = 0;
		pkTask = pkNextTask;

		return;
	}

	pkTask = 0;
}
//---------------------------------------------------------------------------
void Scheduler::GetNextTimeTask(Task*& pkTask)
{
	// return the next time task. If no time task is scheduled to run 
    // this frame, return null. note that this method REMOVES the task from 
    // the list, so the caller is responsible for either adding it back in 
    // (rescheduling) or deleting it.
    
	Task* pkNextTask = m_pkTimeTaskList;

	if (pkNextTask && pkNextTask->m_fNext <= m_kClock.GetFrameEnd())
	{
		m_pkTimeTaskList = pkNextTask->m_pkNext;
		pkNextTask->m_pkNext = 0;
		pkTask = pkNextTask;

		return;
	}

	pkTask = 0;
}
//---------------------------------------------------------------------------
void Scheduler::Schedule(Task* pkTask)
{
	//
	// Schedule a task of the appropriate type (RENDER, FRAME, or TIME).
    //
    // Time tasks and frame tasks are kept in separate lists for ease of 
    // handling. Render tasks are stored as a single special-case entry.
	//
	// time and frame tasks have a start time, a duration, and a period.
	// the duration is relative to the start time, except for duration 0 
    // which is a special case.
	// since the scheduler doesn't care about the duration itself, it 
    // converts it into an end time
	// and stores that instead. the render task does ignores 
    // start/duration/end.
	//
	// a unique task id is generated
	//

    assert(pkTask);
    
	if (pkTask->m_eType == Task::TYPE_RENDER)
	{
        NiDelete m_pkRenderTask;
		pkTask->m_uiID = RENDER_TASK_ID;
		m_pkRenderTask = pkTask;
	}
	else
	{
        pkTask->m_uiID = m_uiNextID++;
        pkTask->m_eStatus = Task::STATE_ACTIVE;

        // Task creator is responsible for setting Start, 
        // Duration, and Period, Task sets End, Scheduler sets Next

        pkTask->m_fNext = pkTask->m_fStart;

        /*
        printf("Scheduling %s task %u from %3u to %3u, every %2u %s\n",
            type==TASK_TIME?"time ":"frame",
            pTaskInfo->id,
            pTaskInfo->time.start,
            pTaskInfo->time.duration,
            pTaskInfo->time.period,
            type==TASK_TIME?"ms":"frames");
        */

        InsertTask(pkTask);
	}
}
//---------------------------------------------------------------------------
void Scheduler::Terminate(unsigned int uiID)
{
	//
	// Terminate a task. This is most useful with infinite-duration tasks, 
    // but it is also handy to delete finite tasks prematurely. 
    // It would be a simple matter to add a "suspend task" option which 
    // suspends a task instead of removing it.
	//
	// Terminate marks the task for deletion rather than just removing it,
	// since it may be called while ExecuteFrame is iterating through the 
    // list.  Marked tasks are swept up at the end of each frame.  Note
    // that the render task is the one exception to this rule as it is
    // deleted outright
	// 

	if (uiID == RENDER_TASK_ID)
	{
		NiDelete m_pkRenderTask;

		m_pkRenderTask = NULL;

        return;
	}

	Task* pkTask = m_pkTimeTaskList;

	while (pkTask)
	{
		if (pkTask->m_uiID == uiID)
		{
			pkTask->m_eStatus = Task::STATE_DELETE;

            return;
		}

		pkTask = pkTask->m_pkNext;
	}

	pkTask = m_pkFrameTaskList;

	while (pkTask)
	{
		if (pkTask->m_uiID == uiID)
		{
			pkTask->m_eStatus = Task::STATE_DELETE;

			return;
		}

		pkTask = pkTask->m_pkNext;
	}
}
//---------------------------------------------------------------------------
void Scheduler::SweepGarbage()
{
    // This function and the whole idea of it sucks -- 
    // when an item is marked to be deleted it should
    // simply be placed on a new list -- or in the case of block allocation
    // of tasks we can delete immediately since it will be effectively free
    
	//
	// Check both timelist and framelist for tasks that were marked
	// for deletion by the Terminate() method. This implementation is
	// pretty brute-force; garbage collection could be run as an
	// idle processing task.
	//
	Task* pkTask = m_pkTimeTaskList;
	Task* pkPrev = 0;

	while (pkTask)
	{
		if (pkTask->m_eStatus == Task::STATE_DELETE)
		{
			Task* pkTemp = pkTask;
			pkTask = pkTask->m_pkNext;

			if (pkTemp == m_pkTimeTaskList)
				m_pkTimeTaskList = pkTask;
			else
				pkPrev->m_pkNext = pkTask;

			pkTemp->m_pkNext = 0;
			NiDelete pkTemp;
		}
		else
		{
			pkPrev = pkTask;
			pkTask = pkTask->m_pkNext;
		}
	}

	pkTask = m_pkFrameTaskList;
	pkPrev = 0;

	while (pkTask)
	{
		if (pkTask->m_eStatus == Task::STATE_DELETE)
		{
			Task* pkTemp = pkTask;
			pkTask = pkTask->m_pkNext;

			if (pkTemp == m_pkFrameTaskList)
				m_pkFrameTaskList = pkTask;
			else
				pkPrev->m_pkNext = pkTask;

			pkTemp->m_pkNext = 0;
			NiDelete pkTemp;
		}
		else
		{
			pkPrev = pkTask;
			pkTask = pkTask->m_pkNext;
		}
	}
}
//---------------------------------------------------------------------------
void Scheduler::ExecuteFrame()
{
	//
	// Run one frame. This takes the time stamp marking the end of the frame
	// and then processes events for that frame retroactively. This method has
	// the advantage of flexibility, especially if the frame rate fluctuates.
	// However it is always a little behind, because it can't compute the
	// frame length until the end of the frame is reached. With a fixed known
	// frame rate you could optimize things a bit and make the start/end times
	// correspond exactly with real time.
	//
    float fBeginTime = NiGetCurrentTimeInSec();
	m_kClock.BeginFrame();
	float fStarted = m_kClock.GetSystem();

	//
	// Execute any time-based tasks
	//
	// (1) Pop the next task off the list. Since the list is always
	//     sorted, the first item in the list is always the next task.
	// (2) Execute it and update times
	// (3) If it's expired, delete it
	//     Otherwise, insert it into the list in its new position
	//
	Task* pkTask = 0;
	GetNextTimeTask(pkTask);

	while (pkTask)
	{
        // Advance clock to this task's time
		m_kClock.AdvanceTo(pkTask->m_fNext);

		pkTask->Execute(m_kClock.GetFrame(), m_kClock.GetTime());

		pkTask->m_fLast = pkTask->m_fNext;
		pkTask->m_fNext += pkTask->m_fPeriod;

		if (pkTask->m_fEnd == 0.0f ||
			pkTask->m_fEnd >= pkTask->m_fNext)
		{
			// re-insert into list with updated time
			InsertTask(pkTask);
		}
		else
		{
			// task is expired, delete it
			NiDelete pkTask;
		}

		GetNextTimeTask(pkTask);
	}

	//
	// Advance simulation clock to end of frame
	//
	m_kClock.AdvanceToEnd();

	//
	// Now execute all frame tasks in round-robin fashion.
	// Frame tasks always execute at the end of the frame just
	// before rendering. A priority scheme could be used to
	// control sequence. It would be more efficient to keep the
	// list sorted, the same as with time tasks 
	//
	pkTask = m_pkFrameTaskList;
	GetNextFrameTask(pkTask);

	while (pkTask)
	{
		pkTask->Execute(m_kClock.GetFrame(), m_kClock.GetTime());

		pkTask->m_fLast = pkTask->m_fNext;
		pkTask->m_fNext += pkTask->m_fPeriod;

		if (pkTask->m_fEnd == 0.0f ||
			pkTask->m_fEnd >= pkTask->m_fNext)
		{
			// re-insert into list with updated time
			InsertTask(pkTask);
		}
		else
		{
			// Task is expired, delete it
			pkTask->m_pkNext = 0;
			NiDelete pkTask;
		}

		GetNextFrameTask(pkTask);
	}

    float fEndTime = NiGetCurrentTimeInSec();
    ((GameMain*)NiApplication::ms_pkApplication)->
        AddUpdateTime(fEndTime - fBeginTime);

	//
	// Render
	//
    if (m_pkRenderTask)
    {
        float fBeginRenderTime = NiGetCurrentTimeInSec();

	    m_pkRenderTask->Execute(m_kClock.GetFrame(), m_kClock.GetTime());

        float fEndRenderTime = NiGetCurrentTimeInSec();
        ((GameMain*)NiApplication::ms_pkApplication)->
            AddRenderTime(fEndRenderTime - fBeginRenderTime);
    }

	//
	// Do idle processing or load balancing
	//
	float fElapsed = m_kClock.GetSystem() - fStarted;
    
    m_fFrameLength = IsRunning() ? 
        m_kClock.GetFrameEnd() - m_kClock.GetFrameStart() : 0.0f;
    /*
    printf("Busy %u ms, idle %u ms\n", elapsed, frameLength - elapsed);
	*/

	//
    // TODO: Get rid of this suckage
    //
	// If any tasks are terminated during execution, it is easier to leave
	// them in the list until we're finished iterating through it, then sweep
	// them out later.
	//
	SweepGarbage();
}
//---------------------------------------------------------------------------
