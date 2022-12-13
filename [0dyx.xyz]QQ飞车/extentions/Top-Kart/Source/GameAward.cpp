#include "Top-KartPCH.h"
#include "GameAward.h"
#include "./Track/KartStatusManager.h"
#include "common.h"
#include "utility.h"
#include "SoundManager.h"
//#include "BaseItem.h"
#include "PowerController.h"
#include "EventViewer.h"
#include "./Track/EagleEye.h"
#include "./Platform/MsgDataCache.h"
#include "./Platform/GlobalInterface.h"
#include "CameraBase.h"

CGameAward::~CGameAward()
{
	int nPlayNum = min__( 3, m_kRaceOverData.m_bCurrentPlayerNum);

	for(int i = 0; i < nPlayNum; i++){
		Play * pWinner = NxPlayMgr::GetPlay
			(m_kRaceOverData.m_astRaceScore[i].m_unUin);

		if(!pWinner) 
			continue;

		pWinner->StartPhy();
	}
	//CALL(NxSceneMgr::Get()->GetSoundManager(), StopSound(SoundManager::ST_AMBIENT));
	CALL(CSoundMgr::Get(), StopSound(CSoundMgr::ST_AMBIENT));
}

bool CGameAward::Init(CNotifyRaceOverPara* pData)
{
	m_kRaceOverData = *pData;

	assert(GlobalInterface::m_pDataCache);
	SHORT nMode = GlobalInterface::m_pDataCache->m_RoomDesc.GameModeSub;
	if ( nMode == EGSI_SUBMODE_TEAM )
	{
		m_bShowTeam = true;
	}

	CRoadMap::Map_Desc* pMapDesc = CLevelManager::Get()->GetMapDesc();
	LPSTR lpszRes = "Map/jiangtai.nif";
	if ( pMapDesc->kAwardName.size() > 0 )
	{
		lpszRes = (LPSTR)pMapDesc->kAwardName.c_str();
	}
	if ( m_bShowTeam )
	{
		lpszRes = "Map/jiangtai_team.nif";
		if ( pMapDesc->kTeamAwardName.size() > 0 )
		{
			lpszRes = (LPSTR)pMapDesc->kTeamAwardName.c_str();
		}
	}

	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(lpszRes))) 
	{
		assert(0);
		return false;
	}

	m_spPlatform = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
	assert(m_spPlatform);

	m_fStartTime = NiGetCurrentTimeInSec();
	m_bLoaded = false;
	return true; 
}

void CGameAward::ShowArena()
{
	if ( m_bLoaded )
	{
		return;
	}
	ScreenOverlay::SetVisible(false);
	NiNodePtr pCamNode = NiDynamicCast(NiNode, m_spPlatform->GetObjectByName("Camera01"));
	//pCamNode->GetParent()->DetachChild(pCamNode);
//	NxSceneMgr::Get()->GetCameraController()->GetForeCam()->SetCamera(pCamNode);
	NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_MODEL)->SetCamera(pCamNode);
	NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_MODEL);

	NiPoint3 kPlatPos = CLevelManager::Get()->GetMapDesc()->kAwardTransform.kTrans;
	NiPoint3 kPlatAngle = CLevelManager::Get()->GetMapDesc()->kAwardTransform.kRot;

	NiMatrix3 kRotateX, kRotateY, kRotateZ;
	kRotateX.MakeXRotation(NxMath::degToRad(kPlatAngle.x));
	kRotateY.MakeYRotation(NxMath::degToRad(kPlatAngle.y));
	kRotateZ.MakeZRotation(NxMath::degToRad(kPlatAngle.z));


	NiMatrix3 kPlatRotate = kRotateX * kRotateY * kRotateZ;
	m_spPlatform->SetTranslate(kPlatPos);
	m_spPlatform->SetRotate(kPlatRotate);
	CLevelManager::Get()->AttachObject(m_spPlatform);

	if ( !m_bShowTeam )
	{
		int nPlayNum = min__( 3, m_kRaceOverData.m_bCurrentPlayerNum);

		char  posName[20] = "";
		for(int i = 0; i < nPlayNum; i++)
		{
			_snprintf(posName, 8, "number%d", i+1);
			NiNodePtr spStandPoint = NiDynamicCast(NiNode,m_spPlatform->GetObjectByName(posName));
			if(!spStandPoint)
			{
				_snprintf(posName, 8, "number0%d", i+1);
				spStandPoint = NiDynamicCast(NiNode,m_spPlatform->GetObjectByName(posName));
			}
			Play * pWinner = NxPlayMgr::GetPlay(m_kRaceOverData.m_astRaceScore[i].m_unUin);
		
			if(!pWinner) 
				continue;

			//判断前三名是否完比赛
			if(m_kRaceOverData.m_astRaceScore[i].m_unFinTime == (unsigned int)(-1) )
				continue;
		
			if ( !spStandPoint )
			{
				XError("领奖台资源有误!");
			}
			pWinner->StopPhy();
			pWinner->GetKartNIFRoot()->SetTranslate(spStandPoint->GetWorldTranslate() - NiPoint3(0, 0, 0.16f));
			pWinner->GetKartNIFRoot()->SetRotate(spStandPoint->GetWorldRotate());
			LogText("winner id: %d", m_kRaceOverData.m_astRaceScore[i].m_unUin);
		}
	}
	else
	{
		char  posName[20] = "";
		int index = 0;
		for(int i = 0; i < MAXSEATNUMINROOM; i++)
		{
			_snprintf(posName, 8, "number%d", index + 1);
			NiNodePtr spStandPoint = NiDynamicCast(NiNode,m_spPlatform->GetObjectByName(posName));
			if(!spStandPoint)
			{
				_snprintf(posName, 8, "number0%d", index+1);
				spStandPoint = NiDynamicCast(NiNode,m_spPlatform->GetObjectByName(posName));
			}
			Play * pWinner = NxPlayMgr::GetPlay(m_kRaceOverData.m_astRaceScore[i].m_unUin);

			if ( !pWinner ||
				( pWinner->GetTeam() != m_kRaceOverData.m_bWinTeamID ) )
			{
				continue;
			}

			if ( !spStandPoint )
			{
				XError("领奖台资源有误!");
			}
			pWinner->StopPhy();
			pWinner->GetKartNIFRoot()->SetTranslate(spStandPoint->GetWorldTranslate() - NiPoint3(0, 0, 0.16f));
			pWinner->GetKartNIFRoot()->SetRotate(spStandPoint->GetWorldRotate());
			LogText("winner id: %d", m_kRaceOverData.m_astRaceScore[i].m_unUin);
			index++;
		}
	}

	//关闭各界面元素，并重置
	{	
		ScreenOverlay::SetVisible(false);

		EventViewerMgr::TurnOffViewer();

		CEagleEyeMap::Get()->Show(false);

		CALL(CPowerController::Get(), TerminateSgPower());
		CALL(CPowerController::Get(), TerminateTeamPower());

//		if(CItemMgr::Get())
//			CItemMgr::Get()->TurnOffColumnShow();
	}

	NxPlayMgr::SetAllPlayAnim("Still");

	CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_CAIDAI, 0));
	if ( m_kRaceOverData.m_iWinType == EWT_PERFECT )
	{
		CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_PERFECT, 0));
	}

	m_bLoaded = true;
}

void CGameAward::Draw()
{
	float fCurTime = NiGetCurrentTimeInSec();
	// 展示领奖台，放 玩家 ，彩带 
	if ( 1000 * (fCurTime - m_fStartTime) > 3000 /*m_kRaceOverData.m_unReceiveHonorTime*/ )
	{
		ShowArena();
	}
}