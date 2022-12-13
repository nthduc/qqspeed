#include "Top-KartPCH.h"
#include "../LevelManager.h"
#include "../SceneLevel.h"
#include "../NxSceneMgr.h"
#include "../InputManager.h"
#include "../PhysKart/NxAllVehicles.h"
#include "../GameMain.h"
#include "../InputManager.h"
#include "KartStatusManager.h"
#include "CheckEditor.h"
#include "TriggerTask.h"
#include "GlobalMap.h"
#include "KartPlayBack.h"
#include "../nxPlayMgr.h"
#include "../MovePkgMgr.h"
#include "../GameOverCDTast.h"
#include "../EventContainer.h"
#include "../Platform/GlobalInterface.h"
#include "../MessageHouse.h"
#include "../EventViewer.h"
#include "../PowerController.h"
//#include "../BaseItem.h"
#include "../ScreenOverlay.h"
#include "EagleEye.h"
#include "../Platform/GlobalInterface.h"
#include "../gpcontrol.h"

//todo:测试checkpoint reset临时加入
#ifndef LOG_CHECKPT
#define LOG_CHECKPT
#endif

#ifdef LOG_CHECKPT
extern FILE* g_pFileCheckPointLog;
#endif

extern void FindAndAdd (NiObjectNET * pkRoot);

CKartStatusManager * CKartStatusManager::ms_KartStatusMgr = NULL;

void CKartStatusManager::EnableReset(bool bEnable)
{
	if (ms_KartStatusMgr)
		ms_KartStatusMgr->m_bEnable = bEnable;
}

bool CKartStatusManager::GetResetState()
{
	if (ms_KartStatusMgr) 
	{
		return ms_KartStatusMgr->m_bEnable;
	}

	return true;
}

CKartStatusManager::CKartStatusManager()
: m_bHasShowWrongHint(false)
, m_dwCurLap(1)
, m_bHasFinishRace(false)
, m_bEnable(true)
, m_dwWarningTime(0)
{
	for (int i = 0; i < enMarkNum; ++i)
	{
		m_aspMarks[i] = 0;
	}
}

CKartStatusManager::~CKartStatusManager()
{
	for (int i = 0; i < enMarkNum; ++i)
	{
		ShowHint((enMark)i, false);
		m_aspMarks[i] = 0;
	}
}

bool CKartStatusManager::Create()
{
	if (!ms_KartStatusMgr) 
	{
		ms_KartStatusMgr = NiNew CKartStatusManager();

		static const char * strFileName[enMarkNum] = 
		{
				"Map\\lap2.nif",
				"Map\\lap3.nif",
				"Map\\lastlap.nif",
				"Map\\win.nif",
				"Map\\lose.nif",
				"Map\\caidai.nif",
		};

		NiStream kStream;
		for (int i = 0; i < enMarkNum; ++i)
		{
			if (!kStream.Load(NiApplication::ConvertMediaFilename(strFileName[i]))) 
			{
				NiDelete ms_KartStatusMgr;
				ms_KartStatusMgr = 0;
				return false;
			}

			ms_KartStatusMgr->m_aspMarks[i] = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

			if (!ms_KartStatusMgr->m_aspMarks[i]) 
			{
				NiDelete ms_KartStatusMgr;
				ms_KartStatusMgr = 0;
				return false;
			}
			FindAndAdd(ms_KartStatusMgr->m_aspMarks[i]);
			ms_KartStatusMgr->m_aspMarks[i]->Update(0.0f);
		}
	
	}

	return true;
}

void CKartStatusManager::Destroy()
{
	if (ms_KartStatusMgr) 
	{
		NiDelete ms_KartStatusMgr;
		ms_KartStatusMgr = NULL;
	}
}

CKartStatusManager * CKartStatusManager::Get()
{
	return ms_KartStatusMgr;
}

void CKartStatusManager::ClearStatus()
{
	m_dwCurLap = 1;
}

NiPoint3 CKartStatusManager::GetRightDir(CCheckPoint * pChkPt, RunDirection dir)
{
	NiPoint3 kDir(0,0,0);
	NiPoint3 kTemp;
	unsigned int uiSize = 0;

	if (dir == dirClockWise) {
		uiSize = pChkPt->arrLinkTo.GetEffectiveSize();
		for (unsigned int i = 0; i < uiSize; ++i)
		{
			kTemp = pChkPt->arrLinkTo.GetAt(i)->pkChkPt->kPoint - pChkPt->kPoint;
			kTemp.Unitize();
			kDir += kTemp;
		}

		kDir /= float(uiSize);
	}

	else if (dir == dirAntiClockWise)
	{
		uiSize = pChkPt->arrLinkFrom.GetEffectiveSize();
		for (unsigned int i = 0; i < uiSize; ++i)
		{
			kTemp = pChkPt->arrLinkFrom.GetAt(i)->pkChkPt->kPoint - pChkPt->kPoint;
			kTemp.Unitize();
			kDir += kTemp;
		}

		kDir /= float(uiSize);
	}
	else
	{
		assert(0);
	}

	return kDir;
}

void CKartStatusManager::Reset(bool bAuto)
{
	if (!m_bEnable) 
	{
		return ;
	}

#ifdef LOG_CHECKPT
	if (g_pFileCheckPointLog)
	{
		fprintf(g_pFileCheckPointLog, "CKartStatusManager Reset() Time=%u\n",::GetTickCount());	
		fflush(g_pFileCheckPointLog);
	}
	else
	{
		_ASSERTE(0);
	}

#endif

	Play * pkPlay = NxPlayMgr::GetHostPlay();
	CLinks * pLinks = CLevelManager::Get()->GetLinks();
	//CCheckPoint * pChkPt = pLinks->GetLastRightCheckPoint(false);
	
	if (!pLinks->m_kLastPair.pPtFrom)
		return;

	pLinks->ResetWarning();

	NiPoint3 kPosition = pLinks->m_kLastPair.pPtFrom->kPoint; //pChkPt->kPoint;
	kPosition.z += 0.5f;

	NiPoint3 kRightDir = pLinks->m_kLastPair.pPtTo->kPoint - pLinks->m_kLastPair.pPtFrom->kPoint; //GetRightDir(pChkPt, pLinks->GetCorrectRunDir());
	NiPoint3::UnitizeVector(kRightDir);
	CKart * pkKart = pkPlay->GetKart();
	if (!pkKart) 
	{
		assert(0);
		return;
	}
	NiPoint3 kPos = pkKart->GetKartTranslate();

	NiPoint3 kCurDir = kPos - pLinks->m_kLastPair.pPtFrom->kPoint;//pChkPt->kPoint;
	kCurDir.Unitize();

	if (kCurDir.Dot(kRightDir) < -0.001f) 
	{
		pLinks->AddResetPoint();
	}

	kRightDir.z = 0;

	kRightDir.Unitize();

	float fRad = 0.0f;
	if (fabs(kRightDir.x) >= 0.00001f)
	{
		fRad = NxMath::atan(kRightDir.y/kRightDir.x);
		if (fRad > 0 && kRightDir.x < 0)
		{
			fRad += NI_PI;			
		}
		if (fRad < 0 && kRightDir.x < 0)
		{
			fRad -= NI_PI;
		}

	}
	else
	{
		if (kRightDir.x > 0)
			fRad = 0.0f;
		else
			fRad = NI_PI;
	}

	NiMatrix3 kRotate;
	kRotate.MakeZRotation(fRad);
	

	NxPlayMgr::GetHostPlay()->GetKart()->setVelocityLocal(NxVec3(0, 0, 0));
	NxPlayMgr::GetHostPlay()->GetKart()->SetPos(NxVec3(kPosition.x, kPosition.y, kPosition.z));
	NxPlayMgr::GetHostPlay()->GetKart()->SetKartRotate(kRotate);
	//NxPlayMgr::GetHostPlay()->GetKart()->SetPose(kRot
}

void CKartStatusManager::OnTriggerRaceWait(float fWaitTime)
{
	ClearStatus();
	
}

void CKartStatusManager::OnTriggerRaceCountDown()
{

}

void CKartStatusManager::OnTriggerRaceStart()
{

}

void CKartStatusManager::OnTriggerRevertRunDir()
{

}
void CKartStatusManager::OnTriggerLapsUp()
{
	++m_dwCurLap;
	TriggerHintTask * pTask = NULL;
	if (CGlobalMap::Get())
	{
		CGlobalMap::Get()->HiLightAll(false);
	}
	float fCurTime = GetMainApp()->GetAccumTime();
	if (m_dwCurLap == 2) 
	{
		pTask = NiNew TriggerHintTask(enLap2, fCurTime, fCurTime + 5.7f);
		Scheduler::Get()->Schedule(pTask);
	}
	else if (m_dwCurLap == 3) 
	{
		pTask = NiNew TriggerHintTask(enLap3, fCurTime, fCurTime + 5.7f);
		Scheduler::Get()->Schedule(pTask);
	}
}
void CKartStatusManager::OnTriggerKnockItemEvent(DWORD dwItemID, void * ItemData)
{
	
}
void CKartStatusManager::OnTriggerBeUsedItemEvent(DWORD dwItemID, void * ItemData)
{

}
void CKartStatusManager::OnTriggerExceptionEvent(DWORD dwExceptionCode, void * ExceptionData)
{
	CLinks * pLinks = CLevelManager::Get()->GetLinks();

	if (KART_EXCEPTSTATE_WRONGWAY == dwExceptionCode) 
	{		
		pLinks->AddWarningTime();
		if (pLinks->ShouldShowWarning()) 
		{
			OnTriggerShowWrongWay(true);			
		}
		
		if (CLevelManager::Get()->GetLinks()->ShouldReset()) 
		{
			Reset(false);
		}
	}
	else if (KART_EXCEPTSTATE_NONE == dwExceptionCode) 
	{
		//NxPlayMgr::GetHostPlay()->DetachHint(m_aspMarks[enWrongWay]);
		OnTriggerShowWrongWay(false);
		pLinks->ResetWarning();
	}
	else
	{
		Reset(false);
	}
}

void CKartStatusManager::OnTriggerRaceFinish(DWORD dwFinishType, float fTime)
{
	// Disable Keyboard
	// Draw Effect
	// Draw Result
	if (!CLevelManager::Get()->IsEnableCheckPoint())
		return;

	switch(dwFinishType) {
	case RACE_FINISH_NORMAL:
		{
			CALL(NxPlayMgr::GetHostPlay()->GetKart(), SetGameFinish());

			CGameOverCountDnMgr::StopCountDownTast();

			//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_MOTOR_MID);
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_MOTOR_MID);

			//NxPlayMgr::GetHostPlay()->AttachHint(m_aspMarks[enMarkWin]);



			CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_OVERCOUNTDOWN));

			CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_FINISHTEXT,0));

			//			EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_WIN);
			if(NxPlayMgr::GetHostPlay())
				NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_FINISH);

			NxPlayMgr::GetHostPlay()->UseWinCamera();
			NxSceneMgr::Get()->GetCameraController()->GetForeCam()->PlayAnim();

			NxPlayMgr::GetHostPlay()->SetWin(true);
			NxPlayMgr::GetHostPlay()->SetFinish(true);

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			


			if ( NxGamePkgMgr::Get()) {
				NxGamePkgMgr::Get()->SendTcp(0);
			}
		}
		break;
	case RACE_FINISH_TIMEOUT:
		{
			CALL(NxPlayMgr::GetHostPlay()->GetKart(), SetGameFinish());

			NxPlayMgr::GetHostPlay()->SetFinish(true);

			//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_MOTOR_MID);
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_MOTOR_MID);

			//NxPlayMgr::GetHostPlay()->AttachHint(m_aspMarks[enMarkLose]);
			CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_UNFINISHTEXT,0));

			//			EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_FAIL);
			if(NxPlayMgr::GetHostPlay())
				NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_UNFINISH);
		}
		break;
	case RACE_FINISH_GM:
		break;
	default:
		assert(0);
		break;
	}

	m_bHasFinishRace = true;

	CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_SMOKE));

	//重置道具
	if(CGPControl::Get())
	{
		CGPControl::Get()->ToggleOp(FALSE);
		CGPControl::Get()->End();
	}

	//关闭各界面元素，并重置
	{	
		ScreenOverlay::SetVisible(false);

		CEagleEyeMap::Get()->Show(false);

		EventViewerMgr::TurnOffViewer();

		CALL(CPowerController::Get(), StopAllPowerCtrl());
		CALL(CAcceleratePanel::Get(), SetActivity(false));
		//NxSceneMgr::Get()->GetPlayerRankMgr()->SetActivity(false);
		CALL(CPlayerRankMgr::Get(), SetPlayerFinish(0));
		CALL(CPlayerRankMgr::Get(), Update(0));
		CALL(CPlayerRankMgr::Get(), SetActivity(false));
	}

	m_dwCurLap = 0;
	if(CLevelManager::Get()->GetPlayBack() && 
		CLevelManager::Get()->GetPlayBack()->IsPlayBack())
	{
		InputManager::Get()->Enable(true);
	}
	CLevelManager::Get()->GetLinks()->ClearRecord();
}

void CKartStatusManager::OnTriggerShowWrongWay(bool bOn /* = true */)
{
	if (m_bHasShowWrongHint)
	{
		CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_STOP));
		m_bHasShowWrongHint = false;
	}
	if (m_bHasFinishRace)
		return;

	CLinks * pkLink = CLevelManager::Get()->GetLinks();

	if ( NxPlayMgr::GetHostPlay() && NxPlayMgr::GetHostPlay()->IsFinish())
	{
		m_bEnable = false;
		m_bHasFinishRace = true;
	}

	if (!m_bEnable) 
	{		 
		if (m_bHasShowWrongHint)
		{
			CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_STOP));
			m_bHasShowWrongHint = false;
		}
		return;
	}

	if (bOn)
	{
		++m_dwWarningTime;
		if (!m_bHasShowWrongHint && m_bEnable)
		{
			CALL(pkLink, AddWarningTime());
			if (m_dwWarningTime > 50)
			{	
				CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_STOP,0));
				m_bHasShowWrongHint = true;
			}
		}
	}
	else
	{
		m_dwWarningTime = 0;
		if (m_bHasShowWrongHint)
		{
			CALL(pkLink, ResetWarning());
			CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_STOP));
			m_bHasShowWrongHint = false;
		}
	}
}

void CKartStatusManager::ShowHint(CKartStatusManager::enMark enHint, bool bShow)
{
	if (!NxPlayMgr::GetHostPlay()) 
	{
		return;
	}

	if( bShow )
	{
		switch (enHint)
		{
		case enLap2:
			CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SECONDLAP,0));
			break;
		case enLap3:
			CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_LASTLAP,0));
			break;
		default:
			break;
		}
	}
/*
	if (bShow) {
		NxPlayMgr::GetHostPlay()->AttachHint(m_aspMarks[enHint]);
	}
	else
	{
		NxPlayMgr::GetHostPlay()->DetachHint(m_aspMarks[enHint]);
	}
*/
}
