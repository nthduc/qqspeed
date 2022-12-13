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

#ifndef POWERUPTASKS_H
#define POWERUPTASKS_H

#include "Task.h"

class PowerUpManager;

//----------------------------------------------------------------------------
class PowerUpTask : public Task
{
public:
    PowerUpTask();

    virtual void Execute(unsigned int uiFrameCount, float fTime) = 0;

protected:
};
//----------------------------------------------------------------------------
class CreatePowerUpTask : public PowerUpTask
{
public:
    CreatePowerUpTask(PowerUp* pkPowerUp);

    virtual void Execute(unsigned int uiFrameCount, float fTime);

protected:
    PowerUp* m_pkPowerUp;
};
//----------------------------------------------------------------------------

#include "PowerUpTasks.inl"

#endif // POWERUPTASKS_H
