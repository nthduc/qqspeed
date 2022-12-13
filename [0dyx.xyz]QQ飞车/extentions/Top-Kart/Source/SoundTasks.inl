

//#include "LevelManager.h"
#include "Scheduler.h"
//#include "SoundManager.h"
#include "soundmgr.h"
#include "InputManager.h"
#include "GameStatusManager.h"
#include "./PhysKart/NxAllVehicles.h"
#include "NxPlayMgr.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "DescConstant.h"


//---------------------------------------------------------------------------
inline PlayAmbientSoundTask::PlayAmbientSoundTask() 
{
    SetType(Task::TYPE_FRAME);

    // Execute every frame so the ambient sound can be turned on / off
    // based on the current game state
    SetTimeParams(0, 1.0f, 0); 

//	LevelManager::Get()->GetSoundManager()->StopSound(SoundManager::ST_AMBIENT);

}
//---------------------------------------------------------------------------
inline void PlayAmbientSoundTask::Execute(unsigned int uiFrameCount, 
    float fTime)
{
    
	if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState()&GameStatusManager::GST_IN_GAME)
	{
		//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_AMBIENT);
		if (CSoundMgr::Get())
		{

			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_AMBIENT);

			if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_RACING)
			{
				float speed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * GameSetting::SpeedDispCoef;
				CSoundMgr::Get()->PlayMotorSound(speed);

			}
			//NxWheel2* pxW0 = (NxWheel2*)NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getWheel(0);
			//if (pxW0)
			//{
			////		NxReal fSteerAngle = pxW0->wheelShape->getSteerAngle();

			//	NxReal fRoundAngle = pxW0->wheelShape->getAxleSpeed();
			//	NxReal fRadius = pxW0->wheelShape->getRadius();
			//	speed = fRadius* fRoundAngle*5;
			//}
			
		}


	}

	if ( NxPlayMgr::GetHostPlay() 
		&& NxPlayMgr::GetHostPlay()->GetKart() )
	{
		if ( NxPlayMgr::GetHostPlay()->GetKart()->IsDrifting())
		{
			//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_SHIFT);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_SHIFT);
		}
		else
		{
			//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_SHIFT);
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_SHIFT);
		}
	}


	//if(InputManager::keys[NiInputKeyboard::KEY_LSHIFT] == true)
	//{
	//	NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_SHIFT);
	//}
	//else
	//{
	//	NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_SHIFT);
	//}
/*
	if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::SELECT_CAR)
	{
		NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_CARBARN);
	}
	else
		NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_CARBARN);
*/
	//if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_STAR_RACE)
	//{
	//	//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_WAIT_START);
	//	CSoundMgr::Get()->PlaySound(CSoundMgr::ST_WAIT_START);
	//}
	//else
	//	//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_WAIT_START);
	//	CSoundMgr::Get()->StopSound(CSoundMgr::ST_WAIT_START);


}
//---------------------------------------------------------------------------
