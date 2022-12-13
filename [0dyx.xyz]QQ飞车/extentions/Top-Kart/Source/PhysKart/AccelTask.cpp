#include "../Top-KartPCH.h"
#include ".\acceltask.h"
#include "../Scheduler.h"
#include "NxVehicle.h"
#include "NxAllVehicles.h"
#include "../GameEffectMgr.h"
#include "../NxPlayMgr.h"

AccelTask::AccelTask(float fLife, float fGear)
{
	float fTime = Scheduler::Get()->GetClock().GetTime();

	SetType(Task::TYPE_TIME);

	SetTimeParams(fTime, fLife, fLife); 

	if( NxPlayMgr::GetHostPlay()->GetKart())
		if (NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle())
		{
			NxPlayMgr::GetHostPlay()->GetKart()->setAccelTime(fLife, fGear);

			CGameEffectMgr::ActiveEffect(IGameEffect::ET_BACKFIRE, NULL);

			CGameEffectMgr::ActiveEffect(IGameEffect::ET_ACCLINE,NULL);

			NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_SPEEDUP);
			CGameEffectMgr::DeActiveEffect(IGameEffect::ET_SMOKE);
		}
}

AccelTask::~AccelTask(void)
{
	if( NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle())
	{
		NxPlayMgr::GetHostPlay()->GetKart()->setAccelTime(0);
	}

	NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_SPEEDUP);

	CGameEffectMgr::DeActiveEffect(IGameEffect::ET_ACCLINE);
	CGameEffectMgr::DeActiveEffect(IGameEffect::ET_BACKFIRE);
	CGameEffectMgr::ActiveEffect(IGameEffect::ET_SMOKE, 0);

	NxSceneMgr::Get()->GetCameraController()->SetCameraParamType(CameraController::CPT_NORMAL);

	if (NxPlayMgr::GetHostPlay()->GetKart() && NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle())

	CGameEffectMgr::DeActiveEffect(IGameEffect::ET_ACCLINE);
}
