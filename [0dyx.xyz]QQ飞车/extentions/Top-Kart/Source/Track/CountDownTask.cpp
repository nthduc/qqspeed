#include "Top-KartPCH.h"
#include "NiTPointerList.h"
#include "CountDownTask.h"
#include "../Utility.h"
#include "../Scheduler.h"
#include "../NxSceneMgr.h"
#include "KartPlayback.h"
#include "EagleEye.h"
#include "../MovePkgMgr.h"
#include "../NO2Controller.h"
#include "../gpcontrol.h"
#include "common.h"
#include "../CameraController.h"
#include "../CameraBase.h"


inline void FindAndAdd(NiObjectNET * pkRoot)
{
	if(pkRoot == NULL)
		return;


	NiTimeController* pkController = pkRoot->GetControllers();

	// Gather all time controllers from this object
	while(pkController != NULL)
	{
		pkController->SetAnimType(NiTimeController::APP_INIT);
		pkController = pkController->GetNext();
	}

	if(NiIsKindOf(NiAVObject, pkRoot))
	{
		// NiProperties can have time controllers, so search them too
		NiAVObject* pkObj = (NiAVObject*) pkRoot;
		NiPropertyList* pkPropList = &(pkObj->GetPropertyList());
		NiTListIterator kIter = pkPropList->GetHeadPos();
		while(pkPropList != NULL && !pkPropList->IsEmpty()&& kIter)
		{
			NiProperty* pkProperty = pkPropList->GetNext(kIter);
			if(pkProperty)
				FindAndAdd(pkProperty);
		}
	}
	if(NiIsKindOf(NiNode, pkRoot))
	{
		NiNode* pkNode = (NiNode*) pkRoot;
		// Search all of the children
		for(unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
		{
			NiAVObject* pkObj = pkNode->GetAt(ui);
			FindAndAdd(pkObj);
		}
		// NiDynamicEffects can have time controllers, so search them too
		const NiDynamicEffectList* pkEffectList= &(pkNode->GetEffectList());

		NiTListIterator kIter = pkEffectList->GetHeadPos();
		while(pkEffectList != NULL && !pkEffectList->IsEmpty() && kIter)
		{
			NiDynamicEffect* pkEffect = pkEffectList->GetNext(kIter);
			if(pkEffect)
				FindAndAdd(pkEffect);
		}
	}
	
}
//
//inline CountDownTask::CountDownTask(unsigned int uiLife)
//{
//	m_fStartTime = NiGetCurrentTimeInSec();
//
//	float fTime = Scheduler::Get()->GetClock().GetTime();
//	float scale = Scheduler::Get()->GetClock().GetScale();
//    	
//	SetType(Task::TYPE_TIME);
//	SetTimeParams(fTime, 0.1f, float(uiLife + 1));
//
//	XTrace("------------------CountDownTask-------------------------");
//	XTrace("new tast: %d", GetTickCount());
//	XTrace("Scheduler time: %f", Scheduler::Get()->GetClock().GetTime());
//	XTrace("scale: %f", scale);
//
//	CALL(NxPlayMgr::GetHostPlay()->GetKart(), SetMovable(false));
//
//	m_nExcutedTimes = 0;
//	m_nStartTime = 10;
//	m_nLife = uiLife * 10;
//
//
//	NiStream kStream;
//	if (!kStream.Load(NiApplication::ConvertMediaFilename("Map/countdown.nif"))) 
//	{
//		assert(0.0f);
//	}
//
//	m_spCountDown = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
//
//	FindAndAdd(m_spCountDown);
//
//	m_spCountDown->Update(0.0f);	
//}
//
//
//
//inline CountDownTask::~CountDownTask()
//{	
//	//	NxPlayMgr::GetHostPlay()->DetachHint(m_spCountDown);
//	if (!CLevelManager::Get()->GetPlayBack()->IsPlayBack()) 
//	{
//		InputManager::Get()->Enable(true);
//	}
//
//	m_spCountDown = 0;
//}
//
//inline void CountDownTask::Execute(unsigned int uiFrameCount, float fTime)
//{
//	if(m_nExcutedTimes == 0)
//	{
//		NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_EAGLEMAP)->SetTarget(NxPlayMgr::GetHostPlay());
//		NxSceneMgr::Get()->GetCameraController()->ActivateLogicCam(CCameraBase::ECT_EAGLEMAP);
//
//		NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_RACE)->SetTarget(NxPlayMgr::GetHostPlay());
//		NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_RACE);
//		NxSceneMgr::Get()->GetCameraController()->GetForeCam()->SetRotate(NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate());
//	
////		CLevelManager::Get()->UseCamera(CLevelManager::GameCamera);
//		CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SMOKE, 0));
//	}
//
//	if (m_nExcutedTimes == m_nStartTime )
//	{	
//		CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_STARTCOUNTDOWN,0));
//	}
//	if(m_nExcutedTimes == m_nStartTime + 3)
//	{
//		XTrace("step1: %d", GetTickCount());
//		XTrace("excutetime: %d  ---- starttime: %d\n", m_nExcutedTimes, m_nStartTime);
//		float scale = Scheduler::Get()->GetClock().GetScale();
//		XTrace("scale: %f", scale);
//		//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//		//NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 22200);
//		//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);		
//		
//		CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
//		CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 22200);
//		CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);		
//
//	}
//	if(m_nExcutedTimes == m_nStartTime + 13)
//	{
//		XTrace("step2: %d\n", GetTickCount());
//		XTrace("excutetime: %d  ---- starttime: %d\n", m_nExcutedTimes, m_nStartTime);
//		float scale = Scheduler::Get()->GetClock().GetScale();
//		XTrace("scale: %f", scale);
//		//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//		//NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 22200);
//		//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
//		CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
//		CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 22200);
//		CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);		
//	}
//	if(m_nExcutedTimes == m_nStartTime +23)
//	{
//		XTrace("step3: %d", GetTickCount());
//		XTrace("excutetime: %d  ---- starttime: %d\n", m_nExcutedTimes, m_nStartTime);
//		float scale = Scheduler::Get()->GetClock().GetScale();
//		XTrace("scale: %f", scale);
//		//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//		//NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 22200);
//		//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
//		CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
//		CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 22200);
//		CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);		
//	}
//	if(m_nExcutedTimes == m_nStartTime + 33)
//	{
//		XTrace("step4: %d", GetTickCount());
//		XTrace("excutetime: %d  ---- starttime: %d\n", m_nExcutedTimes, m_nStartTime);
//		float scale = Scheduler::Get()->GetClock().GetScale();
//		XTrace("scale: %f", scale);
//		//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//		//NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 25500);
//		//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
//		CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
//		CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 25500);
//		CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);		
//	}
//	if (m_nExcutedTimes == m_nLife)  
//	{
//	
//
//		XTrace("------------------CountDownTask-------------------------");
//		m_fEndTime = NiGetCurrentTimeInSec();
//		XTrace("CountDownTask Elapse Time=%f", m_fEndTime - m_fStartTime);
//		XTrace("end countdown: %d", GetTickCount());
//		XTrace("m_nExcutedTimes = %d", m_nExcutedTimes);
//		XTrace("Scheduler time: %f", Scheduler::Get()->GetClock().GetTime());
//
//		NxPlayMgr::GetHostPlay()->DetachHint(m_spCountDown);
//		if (!CLevelManager::Get()->GetPlayBack()->IsPlayBack()) 
//		{
//			InputManager::Get()->Enable(true);
//			std::set<Play*> &PlayersSet = NxPlayMgr::GetAllPlay();
//			std::set<Play*>::iterator it = PlayersSet.begin();
//			for(; it != PlayersSet.end(); ++it)
//			{
//				if((*it)->GetKart() && (*it)->GetKart()->GetVehicle())
//				{
//					(*it)->GetKart()->SetMovable(true);
//				}
//			}
//		}
//		if ( CEagleEyeMap::Get())
//			CEagleEyeMap::Get()->Show(true);
//
//		//CALL(NxGamePkgMgr::Get(), Start());
//
//		NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_RACING);
//	
//		CALL(CNO2Controller::Get(), ActivateNO2());
//		
//		//修改人：ray    描叙：开始时激活道具使用
//
//		if(CGPControl::Get())
//		{
//			CGPControl::Get()->ToggleOp(TRUE);
//			int uNumOfTarget = CGPControl::Get()->GetTargetIterator()->GetTargetCount();
//			for(int i = 0; i < uNumOfTarget; ++i)
//			{
//				CGPControl::Get()->GetTargetIterator()->GetTarget(i)->SetInvalidTarget(FALSE);
//			}
//		}
//	
//		CLevelManager::Get()->GetLinks()->StartRecord();
//	}
//
//	++m_nExcutedTimes;
//}


//inline CountDownTask::CountDownTask()
//{
//	float fTime = Scheduler::Get()->GetClock().GetTime();
//
//	SetType(Task::TYPE_TIME);
//
////	SetTimeParams(fTime, 0.1f, 20.f); 
//
//	InputManager::Get()->Enable(false);
//	m_nExcutedTimes = 0;
//	m_nExcutedFrame = 9999999;
//
//	NiStream kStream;
//	if (!kStream.Load(NiApplication::ConvertMediaFilename("Map/countdown.nif"))) 
//	{
//		assert(0.0f);
//	}
//
//	m_spCountDown = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
//	
//	FindAndAdd(m_spCountDown);
//
//	//m_spCountDown->SetScale(0.7f);
//	m_spCountDown->Update(0.0f);
//
//	m_fTime = 0.0f;
//	m_nMode = CDM_BEGIN;
//	m_nMode = 0;	
//}
//
//
//inline void CountDownTask::SetParam(int nMode,float fTime)
//{
//	float fTime1 = Scheduler::Get()->GetClock().GetTime();
//	SetTimeParams(fTime1, 0.1f, fTime); 
//	m_nMode = nMode;
//	m_fTime = fTime;
//	m_nTime = 10;
//}
//
//inline CountDownTask::~CountDownTask()
//{	
////	NxPlayMgr::GetHostPlay()->DetachHint(m_spCountDown);
//	if (!NxSceneMgr::Get()->GetActiveSceneLevel()->GetPlayBack()->IsPlayBack()) 
//	{
//		InputManager::Get()->Enable(true);
//	}
//
//	m_spCountDown = 0;
//}
//
//inline void CountDownTask::Execute(unsigned int uiFrameCount, float fTime)
//{
//	if(m_nMode == CDM_BEGIN)
//	{
//		if(m_nExcutedTimes == 0)
//		{
//			//NxPlayMgr::GetHostPlay()->UseStartCamera();
//			//NxSceneMgr::Get()->GetActiveSceneLevel()->UseCamera(CSceneLevel::IntrCamera);
//			//NxSceneMgr::Get()->GetCameraController()->PlayAnim();
//			NxSceneMgr::Get()->GetActiveSceneLevel()->UseCamera(CSceneLevel::GameCamera);
//			
//		}
//		
//		if (m_nExcutedTimes == m_nTime )
//		{			
//			CGameEffectMgr::ActiveEffect(IGameEffect::ET_SMOKE, 0);
//			//NxPlayMgr::GetHostPlay()->AttachHint(m_spCountDown);
//			CGameEffectMgr::ActiveEffect(IGameEffect::ET_STARTCOUNTDOWN,0);
//			m_nExcutedFrame = uiFrameCount;
//			::OutputDebugString(MakeString("CountDown Start Time = %f\n", fTime));
//			
//		}
//
//		if(m_nExcutedTimes == m_nTime + 3)
//		{
//			NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//			NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 22200);
//			NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);		
//		}
//		if(m_nExcutedTimes == m_nTime + 13)
//		{
//			NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//			NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 22200);
//			NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
//		}
//		if(m_nExcutedTimes == m_nTime +23)
//		{
//			NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//			NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 22200);
//			NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
//		}
//		if(m_nExcutedTimes == m_nTime + 33)
//		{
//			NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
//			NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 25500);
//			NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
//
//		}
//		if (m_nExcutedTimes == m_nTime + 39)  
//		{
//			NxPlayMgr::GetHostPlay()->DetachHint(m_spCountDown);
//			if (!NxSceneMgr::Get()->GetActiveSceneLevel()->GetPlayBack()->IsPlayBack()) 
//			{
//				InputManager::Get()->Enable(true);
//				std::set<Play*> &PlayersSet = NxPlayMgr::GetAllPlay();
//				std::set<Play*>::iterator it = PlayersSet.begin();
//				for(; it != PlayersSet.end(); ++it)
//				{
//					if((*it)->GetKart() && (*it)->GetKart()->GetVehicle())
//					{
//						(*it)->GetKart()->GetVehicle()->SetMovable(true);
//					}
//				}
//			}
//		
//			OutputDebugString(MakeString("CountDown Terminate = %f\n", fTime));
//
//			if ( CEagleEyeMap::Get())
//				CEagleEyeMap::Get()->Show(true);			
//			
//			NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::PLAYING_GAME);
//
//		//	NxGamePkgMgr::Create();  // 倒数计时结束 开始收发包 写在这里不好 hover
//
//
///*			PlayAmbientSoundTask* pkSoundTask = NiNew PlayAmbientSoundTask;
//			if (pkSoundTask)
//			{
//				Scheduler::Get()->Schedule(pkSoundTask);
//			}
//*/			
//		}
//	}
//	
//	++m_nExcutedTimes;
//}

CCountDownTask::CCountDownTask()
{
	m_dwStartTime = GetTickCount();
	m_bIsActive = true;
	for(int i = 0; i < 5; ++i)
	{
		m_bArrStep[i] = true;
	}
	
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename("Map/countdown.nif"))) 
	{
		assert(0.0f);
	}
	m_spCountDown = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
	FindAndAdd(m_spCountDown);
	m_spCountDown->Update(0.0f);	
	CALL(NxPlayMgr::GetHostPlay()->GetKart(), SetMovable(false));	
}

CCountDownTask::~CCountDownTask()
{
	m_spCountDown = 0;
}

void CCountDownTask::Update(float fTime)
{
	if(m_bIsActive)
	{
		if(m_bArrStep[0])
		{
			NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_EAGLEMAP)->SetTarget(NxPlayMgr::GetHostPlay());
			NxSceneMgr::Get()->GetCameraController()->ActivateLogicCam(CCameraBase::ECT_EAGLEMAP);

			NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_RACE)->SetTarget(NxPlayMgr::GetHostPlay());
			NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_RACE);
			NxSceneMgr::Get()->GetCameraController()->GetForeCam()->SetRotate(NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate());
			CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SMOKE, 0));
			m_bArrStep[0] = false;
		}
		else if( m_bArrStep[1] && GetTickCount() - m_dwStartTime > 1000)
		{
			CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_STARTCOUNTDOWN,0));
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
			CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 22200);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);	
			m_bArrStep[1] = false;
		}
		else if(m_bArrStep[2] && GetTickCount() - m_dwStartTime > 2000)
		{
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
			CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 22200);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);		
			m_bArrStep[2] = false;
		}
		else if(m_bArrStep[3] && GetTickCount() - m_dwStartTime > 3000)
		{
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
			CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 22200);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);		
			m_bArrStep[3] = false;
		}
		else if(m_bArrStep[4] && GetTickCount() - m_dwStartTime > 4000)
		{
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
			CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 25500);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);		

			NxPlayMgr::GetHostPlay()->DetachHint(m_spCountDown);
			if (!CLevelManager::Get()->GetPlayBack()->IsPlayBack()) 
			{
				InputManager::Get()->Enable(true);
				std::set<Play*> &PlayersSet = NxPlayMgr::GetAllPlay();
				std::set<Play*>::iterator it = PlayersSet.begin();
				for(; it != PlayersSet.end(); ++it)
				{
					if((*it)->GetKart() && (*it)->GetKart()->GetVehicle())
					{
						(*it)->GetKart()->SetMovable(true);
					}
				}
			}
			if ( CEagleEyeMap::Get())
				CEagleEyeMap::Get()->Show(true);

			//CALL(NxGamePkgMgr::Get(), Start());

			NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_RACING);

			CALL(CNO2Controller::Get(), ActivateNO2());

			//修改人：ray    描叙：开始时激活道具使用

			if(CGPControl::Get())
			{
				CGPControl::Get()->ToggleOp(TRUE);
				int uNumOfTarget = CGPControl::Get()->GetTargetIterator()->GetTargetCount();
				for(int i = 0; i < uNumOfTarget; ++i)
				{
					CGPControl::Get()->GetTargetIterator()->GetTarget(i)->SetInvalidTarget(FALSE);
				}
			}

			CLevelManager::Get()->GetLinks()->StartRecord();
			m_bArrStep[4] = false;
			m_bIsActive = false;
		}	
	}
}
