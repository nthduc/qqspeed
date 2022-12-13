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

#ifndef POWERUPMANAGER_H
#define POWERUPMANAGER_H

#include <NiTPointerList.h>

class PowerUp;

class PowerUpManager : public NiMemObject
{
public:
    // scheduler calls QueuePowerUp via a PowerUpTask
    void QueuePowerUp(PowerUp* pkPowerUp);    

    // IceBlockManager calls PopPowerUp when creating ice blocks
    PowerUp* PopPowerUp();

protected:

    NiTPointerList<PowerUp*> m_kPowerUpQueue;
    
};

#endif // #ifndef POWERUPMANAGER_H