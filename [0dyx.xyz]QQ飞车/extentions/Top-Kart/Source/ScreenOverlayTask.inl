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

//---------------------------------------------------------------------------
#include "NxSceneMgr.h"
#include "SceneLevel.h"
inline ScreenOverlayTask::ScreenOverlayTask()
{
    SetType(Task::TYPE_FRAME);
    SetTimeParams(0.0f, 1.0f, 0.0f);
    
}
//---------------------------------------------------------------------------
inline void ScreenOverlayTask::Execute(unsigned int, float fTime)
{
    if (NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() &
		GameStatusManager::GST_IN_GAME)
    {
        return;
    }

    InputManager* pkInput = InputManager::Get();
    assert(pkInput);
    pkInput->MenuUpdate();
}
//---------------------------------------------------------------------------
