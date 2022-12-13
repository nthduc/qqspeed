
#include "InputManager.h"
//#include "LevelManager.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "Play.h"
#include "Scheduler.h"
#include "NxPlayMgr.h"

//---------------------------------------------------------------------------
inline MommaUpdateAnimationsTask::MommaUpdateAnimationsTask()
{
	ms_bCreated = true;
    SetType(Task::TYPE_FRAME);
    SetTimeParams(0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
inline void MommaUpdateAnimationsTask::Execute(unsigned int uiFrameCount, 
    float fTime)
{
   // Play* pkMomma = LevelManager::Get()->GetPlay();
    float fFrameTime = Scheduler::Get()->GetFrameLength();

    // Override the passed in time, and use the non-scaled version of time.
    // This is done so the momma will be animated in real time, regardless of
    // bullet time being active or not.
    fTime = Scheduler::Get()->GetClock().GetUnScaledTime();


    if (NxSceneMgr::Get()->GetGameStatusManager()->GetBulletTime())
    {
        // Invert the scale applied to the frame time so the momma will move
        // in real time instead of bullet time.
        float fScale = Scheduler::Get()->GetClock().GetScale();
        fFrameTime /= fScale;
    }
	NxPlayMgr::UpdateAllPlay(fTime, fFrameTime, true);

/*
    if (LevelManager::GetGameStatusManager()->GetGameState() == 
        GameStatusManager::PLAYING_GAME)
    {
	
        pkMomma->
            Update(fTime, fFrameTime,
            InputManager::Get()->GetRelativeDir(), 
            InputManager::Get()->GetStrafeDir(), true);
    }
    else
    {
	
        pkMomma->
            Update(fTime, fFrameTime, NiPoint2::ZERO, 0, true);
		
    }
	*/
}
//---------------------------------------------------------------------------
inline MommaJumpTask::MommaJumpTask(unsigned int uiWhich)
{
    SetType(Task::TYPE_FRAME);
    // start now, execute only once, for this frame only
    SetTimeParams((float)Scheduler::Get()->GetClock().GetFrame(), 
        1.0f, 1.0f); 

    m_uiPadID = uiWhich;
}
//---------------------------------------------------------------------------
inline void MommaJumpTask::Execute(unsigned int uiFrameCount, 
    float fTime)
{

}
//---------------------------------------------------------------------------
