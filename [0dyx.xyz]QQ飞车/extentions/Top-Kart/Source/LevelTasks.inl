

//#include "LevelManager.h"
#include "Scheduler.h"
#include "SoundManager.h"
#include "Play.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"

//---------------------------------------------------------------------------
inline RenderTask::RenderTask()
{
    SetType(Task::TYPE_RENDER);
    SetTimeParams(0.0f, 0.0f, 0.0f);
}
//---------------------------------------------------------------------------
inline void RenderTask::Execute(unsigned int, float fTime)
{
    CSceneLevel* pkLevel = NxSceneMgr::GetActiveSceneLevel();

  //  assert(pkLevel);

	GameStatusManager* pkSM = NxSceneMgr::Get()->GetGameStatusManager();
    if ( 
		(pkSM->GetGameState() & GameStatusManager::GST_RACING)
         && !pkSM->GetWaitingForUpdate()
	   )
    {
		if( pkLevel ) 
		{
			pkLevel->Draw();
		}
    }
    else
    {
        ScreenOverlay::Get()->Draw();
    }
    
}
//---------------------------------------------------------------------------
inline RenderStartTask::RenderStartTask(float fLoadScreenDuration)
{
    SetType(Task::TYPE_TIME);
    SetTimeParams(fLoadScreenDuration, 0.0f, -1.0f);

    // Create the render task -- it will be "owned" by the scheduler
    // and destroyed (as necessary) by the scheduler, so we can do it
    // in local scope.  The rendering task is run automatically every
    // frame by the scheduler and requires no unique time parameters
    m_pkRenderTask = NiNew RenderTask;
    assert(m_pkRenderTask);
}
//---------------------------------------------------------------------------
inline void RenderStartTask::Execute(unsigned int, float fTime)
{
    // Start rendering
    Scheduler::Get()->Schedule(m_pkRenderTask);

}
//---------------------------------------------------------------------------
inline LevelUpdateTask::LevelUpdateTask()
{
    SetType(Task::TYPE_FRAME);
    SetTimeParams(0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
inline void LevelUpdateTask::Execute(unsigned int, float fTime)
{

	CSceneLevel * pkLevel = NxSceneMgr::GetActiveSceneLevel();


	
	GameStatusManager* pkStatusManger = NxSceneMgr::Get()->GetGameStatusManager();
 //   assert(pkLevel);

	if ( 
		(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() & GameStatusManager::GST_OUT_GAME)
	   )
	{
		return;
	}

	InputManager* pkInput = InputManager::Get();
    assert(pkInput);

   CCameraController* pkCameraController = 
            NxSceneMgr::Get()->GetCameraController();
        
   assert(pkCameraController);

   if( (NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_RACING
	   ||NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_STAR_RACE)
	   && pkCameraController->GetForeCam()->GetCamera())
   {
	   pkInput->GameUpdate(pkCameraController->GetForeCam()->GetCamera()->GetWorldRotate()); //??????????????????????????
   }


   if (pkStatusManger->GetBulletTime())
   {
	   float fElapsedTime = Scheduler::Get()->GetClock().GetFrameTime();

	   float fAmount = pkStatusManger->GetBulletTimeAmount() - 
		   fElapsedTime;
	   pkStatusManger->SetBulletTimeAmount(fAmount);

	   if (pkStatusManger->GetBulletTimeAmount() <= 0)
	   {
		   pkStatusManger->SetBulletTime(false, 1.0f);
	   }

   }
   else
   {
	   float fElapsedTime = Scheduler::Get()->GetClock().GetFrameTime();

	   // Slowly recharge the Momma's bullet time amount. Bullet time 
	   // amount will fully recharge itself in 20 seconds
	   float fRegenAmount = 20.0f / 
		   GameStatusManager::ms_fMaxBulletTimeAmount;

	   fRegenAmount = fElapsedTime / fRegenAmount;
	   float fAmount = pkStatusManger->GetBulletTimeAmount() + 
		   fRegenAmount;
	   pkStatusManger->SetBulletTimeAmount(fAmount);
   }
   if ( pkLevel )
   {
	    pkLevel->Update(fTime);
   }

	//if (NxSceneMgr::Get()->GetSoundManager())
	//{
	//	NxSceneMgr::Get()->GetSoundManager()->Update(fTime);
	//}
   if (CSoundMgr::Get())//primo 07.4.27
   {
	   CSoundMgr::Get()->Update(fTime);
   }
}
//---------------------------------------------------------------------------
