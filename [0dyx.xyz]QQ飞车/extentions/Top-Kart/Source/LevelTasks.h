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

#if defined(___xxx)

#ifndef LEVELTASKS_H
#define LEVELTASKS_H

#include "Task.h"

//----------------------------------------------------------------------------
class RenderTask : public Task
{
public:
    RenderTask();
	virtual void Execute(unsigned int uiFrameCount, float fTime);
};
//----------------------------------------------------------------------------
class RenderStartTask : public Task
{
public:
    RenderStartTask(float fLoadScreenDuration);
	virtual void Execute(unsigned int uiFrameCount, float fTime);

protected:
    RenderTask* m_pkRenderTask;
};
//----------------------------------------------------------------------------
class LevelUpdateTask : public Task
{
public:
    LevelUpdateTask();
    virtual void Execute(unsigned int uiFrameCount, float fTime);
};
//----------------------------------------------------------------------------

#include "LevelTasks.inl"

#endif // LEVELTASKS_H

#endif