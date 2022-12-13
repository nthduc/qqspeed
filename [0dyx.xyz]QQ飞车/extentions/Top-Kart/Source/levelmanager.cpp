////////////////////////////////////////////////
//     FileName:    LevelManager.cpp
//     Author:      Fish
//     Date:        2007-2-28 14:51:05
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "LevelManager.h"
#include "common.h"
#include "math.h"
#include "Utility.h"
#include <NiAnimation.h>
#include <NiApplication.h>
#include <NiMain.h>
#include <NiParticle.h>
#include <NiPhysX.h>
#include <NiMetricsLayer.h>
#include <NiD3DShaderFactory.h>
#include "gpcontrol.h"
#include "CameraController.h"
#include "EffectAccumulator.h"
#include "GameStatusManager.h"
#include "InputManager.h"
#include "Play.h"
#include "EffectManager.h"
#include "PowerUpManager.h"
#include "Scheduler.h"
#include "SoundManager.h"
#include "SoundTasks.h"
#include "HUDTasks.h"
#include "Footprints.h"
#include "GameAward.h"
#include "TerrainManager.h"
#include "FenceManager.h"
#include "PhysXManager.h"
#include "NxPlayMgr.h" 
#include "PhysXUserData.h"
#include "ProilingController.h"
#include "ScriptVM.h"
#include "ConsServer.h"
#include "Track/CheckEditor.h"
#include "Track/EagleEye.h"
#include "Track/KartPlayback.h"
#include "Track/KartStatusManager.h"
#include "Track/CountDownTask.h"
#include "Track/GlobalMap.h"
#include "./PhysKart/NxAllVehicles.h"
#include "framework/GameApp_i.h"
#include "framework/gui_i.h"
#include "Platform/GlobalInterface.h"
#include "MovePkgMgr.h"
#include "EventViewer.h"
#include "GameOverCDTast.h"
#include "PowerController.h"
#include "BaseItem.h"
#include "stringex.h"
#include "SceneHelper.h"
#include "GPNetInterCommn.h"
#include "RenderBase.h"
#include "NO2Controller.h"
#include "Win32SoundManager.h"
#include "./Platform/GlobalInterface.h"
#include "./Platform/MsgDataCache.h"
#include "./Avatar/AvatarMgr.h"
#include "./Avatar/ShopMgr.h"
#include "playerrank.h"
#include "DataCollecter.h"
#include "AcceleratePanel.h"
#include "./Track/CountDownTask.h"

#include "CameraBase.h"
#include "NxSceneMgr.h"
#include "Monster.h"

CLevelManager::CLevelManager():
m_fPrevFrameTime(0),
m_fAppStartTime(0),
m_fElapseTime(0),
m_pkPlayBack(NULL),
m_nCountDownTime(0)
{
}

CLevelManager::~CLevelManager()
{
	Leave();
}

HRESULT CLevelManager::Enter(CNotifyGameBeginPara* pPara)
{
	HRESULT hr = E_FAIL;
	{
		m_fAppStartTime = NiGetCurrentTimeInSec();

		KK_CHECK_POINTER(pPara);
		m_kGameBeginPara = *pPara;
		m_kGameBeginPara.m_nMapID -= 100;

		CGameEffectMgr::Create();
		CGPControl::Create();
		CKartStatusManager::Create();
		CEagleEyeMap::Create();
		NxGamePkgMgr::Create();
		CPlayerRankMgr::Create();
		CPowerController::Create();
		CNO2Controller::Create();
		CAcceleratePanel::Create();
		
		//log数据收集（策划要求，发布时可去掉）
		CDataCollecter::Create();
		CDataCollecter::Get()->SetTarget(NxPlayMgr::GetHostPlay());
		CDataCollecter::Get()->SetKeyboard(InputManager::Get()->GetInputSystem()->GetKeyboard());

		//加载地图
		hr = LoadMap(m_kGameBeginPara.m_nMapID, m_kGameBeginPara.m_nLap);
		KK_CHECK(hr);

		//小地图
		bool bRet = false;
		CALLEX(CEagleEyeMap::Get(), Init(m_kMapDesc.kEagleMapFileName.c_str()), bRet); 
		assert(bRet == true);

		KartInfo kartInfo;
		m_pkPlayBack = NiNew CKartPlayBack(kartInfo);

		///赋值残影特效需要跟踪的物体对象
		m_kKartCrack.Init(
			NiDynamicCast(NiNode,NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_1")));

		if(m_kMapDesc.kAmbientSoundName.size() > 0)
		{
			//NxSceneMgr::Get()->GetSoundManager()->SetSource(1, m_kMapDesc.kAmbientSoundName.c_str());
			CSoundMgr::Get()->SetSource(CSoundMgr::ST_AMBIENT, m_kMapDesc.kAmbientSoundName.c_str(),0.4, -1, true);
		}
		

		//NxPlayMgr::PhysicAllPlay(); 

		NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_STAR_RACE);

		NxPlayMgr::SetAllPlayAnim("Idle");

		CALL(CAcceleratePanel::Get(), SetBaseTexture("AcceleratePanel/mph.dds"));
		CALL(CAcceleratePanel::Get(), SetDynamicTexture("AcceleratePanel/mph_color.dds"));
		CALL(CAcceleratePanel::Get(), SetPointerTexture("AcceleratePanel/mph_pointer.dds"));
		CALL(CAcceleratePanel::Get(), SetTotalAngle(180));
		CALL(CAcceleratePanel::Get(), SetMaxSpeed(380));
		CALL(CAcceleratePanel::Get(), SetRadius(105));
		CALL(CAcceleratePanel::Get(), MoveTo(0, 0));

		m_spGameCamera = NiNew NiCamera;
		NiFrustum kFrustum;
		kFrustum.m_fTop = 0.3107f;
		kFrustum.m_fBottom = -0.3107f;
		kFrustum.m_fLeft = -0.4142f;
		kFrustum.m_fRight = 0.4142f;
		kFrustum.m_fNear = 1.0f;
		kFrustum.m_fFar = 5000.f;
		m_spGameCamera->SetViewFrustum(kFrustum);

		NxPlayMgr::ShowAllPlayName(TRUE);
		NxPlayMgr::GetHostPlay()->UseStartCamera();
		NxSceneMgr::Get()->GetCameraController()->GetForeCam()->PlayAnim();
		//CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);

		NxPlayMgr::GetHostPlay()->UseStartCamera();
		NxSceneMgr::Get()->GetCameraController()->GetForeCam()->PlayAnim();
		CALL(CGameEffectMgr::Get(), ResetEffect());
		CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SNOW,NULL));
		CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SPEED,NULL));

		CCustomCullingProcess& kCuller = GetMainApp()->GetCuller();
		bRet = kCuller.InitParam(m_kMapDesc.kAppCullStrategy.fAllVisibleDistance,
			m_kMapDesc.kAppCullStrategy.fMinAngle, 
			m_kMapDesc.kAppCullStrategy.fMaxProcessBoundRadius);
		KK_CHECK_BOOL(bRet);

		SetShadow();

		bRet = EventViewerMgr::CreateEventViewerMgr(m_spLevelRoot);
		KK_CHECK_BOOL(bRet);
 
		GlobalInterface::m_pApp->EnableIME(FALSE);

		CMonsterMgr::Create();

		Reset();

		GetMainApp()->HidePointer();
		hr = S_OK;
	}
KK_EXIT:
	return hr;
}

void CLevelManager::Reset()
{
	m_fElapseTime = 0.0f;        

	assert(CKartStatusManager::Get());
	GetLinks()->ClearRecord();
	CALL(NxPlayMgr::GetHostPlay(), SetFinish(false));
	CALL(NxGamePkgMgr::Get(), Restart());

	{

		CGPControl::Get()->Start();
		CPowerController * pPowerCtrler = CPowerController::Get();

		if(GlobalInterface::m_pDataCache && pPowerCtrler)
		{
			if(GlobalInterface::m_pDataCache->m_RoomDesc.GameModeBase == EGBI_BASEMODE_RACE)
			{
				if(GlobalInterface::m_pDataCache->m_RoomDesc.GameModeSub == EGSI_SUBMODE_SINGLE)
				{
					pPowerCtrler->StartSgPower();
					pPowerCtrler->TerminateTeamPower();					
					
				}
				else if(GlobalInterface::m_pDataCache->m_RoomDesc.GameModeSub == EGSI_SUBMODE_TEAM)
				{
					pPowerCtrler->StartSgPower();
					pPowerCtrler->StartTeamPower();						
				}
			}
			else
			{
				pPowerCtrler->TerminateSgPower();
				pPowerCtrler->TerminateTeamPower();				
			}

			if(GlobalInterface::m_pDataCache->m_RoomDesc.GameModeSub == EGSI_SUBMODE_SINGLE)
			{
				CPlayerRankMgr::Get()->SetRaceType(false);
			}
			else if(GlobalInterface::m_pDataCache->m_RoomDesc.GameModeSub == EGSI_SUBMODE_TEAM)
			{
				CPlayerRankMgr::Get()->SetRaceType(true);
			}
		}

		EventViewerMgr::TurnOnViewer();

		//ScreenOverlay::SetVisible(true);
	}

}

void CLevelManager::Leave()
{
	//CALL(NxSceneMgr::Get(), GetSoundManager()->StopAllSound());
	CALL(CSoundMgr::Get(), StopAllSound());
	CALL(NxPlayMgr::GetHostPlay(), TerminateShadow());
	NxPlayMgr::ShowAllPlayName(FALSE);
	CPlayerRankMgr::Destroy();
	NxGamePkgMgr::Destroy();
	CGPControl::Destroy();
	EventViewerMgr::Destroy();
	CGameAward::Destroy();
	NxPlayMgr::UnPhysicAllPlay();
	CDataCollecter::Destroy();
	CMonsterMgr::Destory();
	CGameEffectMgr::Destroy();
	CPowerController::Destroy();
	CNO2Controller::Destroy();
	CAcceleratePanel::Destroy();

	CALL(m_pkPlayBack, Save("Play.txt"));
	SAFE_NIDELETE(m_pkPlayBack);

	m_kKartCrack.Destroy();
	m_spGameCamera = NULL;

	CGlobalMap::Destroy();
	CEagleEyeMap::Destroy();
	CKartStatusManager::Destroy();

	NxSceneMgr::Get()->GetCameraController()->DeactivateAllCam();
	NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_ROOM);

	NxPlayMgr::SetAllPlayAnim("Idle");

	UnLoadMap();

	GlobalInterface::m_pApp->EnableIME(TRUE);
	GetMainApp()->ShowPointer();

	float fTotalTimeInMins = NiGetCurrentTimeInSec() - m_fAppStartTime;
	fTotalTimeInMins /= 60.0f;

	NIMETRICS_ADDVALUE("Time in Demo (mins)", fTotalTimeInMins);

}

void	CLevelManager::Update(float fTime)
{
	NIMETRICS_EVAL(NiMetricsScopePCTimer kTimer("SceneLevel.Update"));

	if ( m_pPhysXManager )
	{
		//PROFILE("Phys");
		// Get results from the simulation
		if (m_pPhysXManager->Fetch(fTime))
		{
			CPhysXTask::Get()->Process();
			// Push the simulation results out to the nodes
			m_pPhysXManager->UpdateDestinations(fTime);
			// update PhysX
			// Simulate to the next guessed frame time
			float fDt = Scheduler::Get()->GetClock().GetFrameEnd()
				- Scheduler::Get()->GetClock().GetFrameStart();

			NxPlayMgr::UpdateAllPhysicPlay(fTime - m_fPrevFrameTime);
			CMonsterMgr::Update(fTime - m_fPrevFrameTime);

			m_pPhysXManager->UpdateSources(fTime + fDt);
			m_pPhysXManager->Sim(fTime);// + fDt);
		}
	}
	//临时加，等有了timer去掉
	CALL(CCountDownTask::Get(), Update(0));

	float fUnScaledTime = Scheduler::Get()->GetClock().GetUnScaledTime();
	float fFrameTime = Scheduler::Get()->GetFrameLength();
	NxPlayMgr::UpdateAllPlay(fUnScaledTime, fFrameTime, true);
	

	m_PlayerPosUpdateTask.Execute(0, 0);
	InputManager* pkInput = InputManager::Get();
	assert(pkInput);

	CCameraController* pkCameraController = 
		NxSceneMgr::Get()->GetCameraController();

	assert(pkCameraController);

	GameStatusManager::GameState enGameState = NxSceneMgr::Get()->GetGameStatusManager()->GetGameState();
	NiCamera * pkCamera = pkCameraController->GetForeCam()->GetCamera();

	
	if( (enGameState == GameStatusManager::GST_RACING
		||enGameState == GameStatusManager::GST_STAR_RACE)
		&& pkCamera)
	{
		pkInput->GameUpdate(pkCamera->GetRotate()); //??????????????????????????
	}


	CRoadMap::Update(fTime);

	//fTime = GetMainApp()->GetAccumTime();
	if (m_pkPlayBack && m_pkPlayBack->IsPlayBack()) 
	{
		m_pkPlayBack->Update(NiApplication::ms_pkApplication->GetCurrentTime());
	}

	//游戏中场景，时间，power等更新
	if (enGameState == GameStatusManager::GST_RACING)
	{
		m_fElapseTime += fTime - m_fPrevFrameTime;
	}

	CALL(CNO2Controller::Get(), Update());
	CALL(CRenderMgr::Get(), Update(fTime));
	EventViewerMgr::Update(fTime - m_fPrevFrameTime);

	m_fPrevFrameTime = fTime;

	pkCameraController->Update(fTime);
	
	CALL(CGameEffectMgr::Get(), Update(fTime));

	m_kKartCrack.Update(fTime);

	CALL(CGPControl::Get(), Update(1.0));
	
	CALL(CAvatarMgr::Get(), Update(fTime));

	CALL(CShopMgr::Get(), Update(fTime));

	CALL(NxGamePkgMgr::Get(), Update(fTime));

	//log数据收集（策划要求，发布时可去掉）
	CALL(CDataCollecter::Get(), Update(fTime));

	NxSceneMgr::Get()->GetGameStatusManager()->SetWaitingForUpdate(false);
	m_fPrevFrameTime = fTime;
	
}
/*
void CLevelManager::UseCamera(eCameraType CameraType)
{
	switch( CameraType )
	{
	case GameCamera:
		{
			NxSceneMgr::Get()->GetCameraController()->SetCamera(m_spGameCamera, CameraController::CCM_FLLOW);
			if (NxPlayMgr::GetHostPlay())
			{
				NxSceneMgr::Get()->GetCameraController()->SetCameraParamType(CameraController::CPT_NORMAL);
				NxSceneMgr::Get()->GetCameraController()->Follow(NxPlayMgr::GetHostPlay()->GetKartNIFRoot());
			}
			break;
		}
	default:
		break;
	}
}
*/
void CLevelManager::Render()
{
	NiRenderer* pkRenderer = NiRenderer::GetRenderer();
	NiCamera* pkCamera = NxSceneMgr::Get()->GetCameraController()->GetForeCam()->GetCamera();
	assert(pkRenderer);
	assert(pkCamera);

	float fPreRenderTime = NiGetCurrentTimeInSec();
	GetMainApp()->AddAnimTime(NiGetCurrentTimeInSec() - fPreRenderTime);

	// renderer camera port settings
	pkRenderer->SetCameraData(pkCamera);


	//解决尾气无法显示问题，改变render顺序	pighuang 20070424
	//CALL(CGameEffectMgr::Get(), Render(pkCamera));

	CRoadMap::Render(pkCamera);	
	//加入Avatar眼镜后发现Alpha排序有问题，所以增加排序代码 pighuang 20070323
	//绘制玩家放到后面，防止被场景挡住
	// Render Players
	{
		PROFILE("DrawPlayer");
		NxPlayMgr::DrawAllPlay(pkCamera);
	}
	CMonsterMgr::Draw(pkCamera);

	CALL(CGameEffectMgr::Get(), Render(pkCamera));

	// Draw Crack
	m_kKartCrack.Draw(pkCamera);

	// Draw Award
	CALL(CGameAward::Get(), Draw());

	//渲染对象队列
	CALL(CRenderMgr::Get(), Render());
}


void CLevelManager::Draw()
{
	// Draw Screen
	//CALL(ScreenOverlay::Get(), Draw());

	// Draw EagleMap
	CALL(CEagleEyeMap::Get(), Draw());
	
	// Draw GlobalMap
	//CALL(CGlobalMap::Get(), Draw());
	
	// Draw Event
	EventViewerMgr::Draw();

	//渲染对象队列
	CALL(CRenderMgr::Get(), ScreenRender());
	
	// Draw Screen Effect
	CALL(CGameEffectMgr::Get(), ScreenRender(NULL));

	// Draw2D elements( include FrameRate, Menu etc.)
//	NxSceneMgr::Get()->GetCameraController()->Draw2DElements();
}

void CLevelManager::CacheRender()
{
	NiRenderer* pkRenderer = NiRenderer::GetRenderer();

	NiCamera* pkCamera = NxSceneMgr::Get()->GetCameraController()->GetForeCam()->GetCamera();
	assert(pkCamera);

	CALL(CEagleEyeMap::Get(), Render());
	
	RenderShadow(pkCamera);
	//CALL(NxPlayMgr::GetHostPlay(), RenderPortrait());
	
	//NxPlayMgr::RenderAllPlayName();
	//加入Avatar眼镜后发现Alpha排序有问题，所以增加排序代码 pighuang 20070323
	EventViewerMgr::Render();
}

NiCamera* CLevelManager::GetCamera()
{
	 return (NxSceneMgr::Get()->GetCameraController()->GetForeCam()->GetCamera());
}

void CLevelManager::SetCountDownTime(int nTime)
{
	m_nCountDownTime = nTime;
}

int CLevelManager::GetCountDownTime()
{
	return m_nCountDownTime;
}
void CLevelManager::SetShadow()
{
	int nShowShadow = 0;
	//int  nShadowLevel = 0;
	if ( GlobalInterface::m_pSettings )
	{
		GlobalInterface::m_pSettings->GetIntValue("ShowShadow", &nShowShadow);
		//GlobalInterface::m_pSettings->GetIntValue("ShadowLevel", &nShadowLevel);
	}
	
	if (nShowShadow == 1)
	{
		if (NxPlayMgr::GetHostPlay() )
			NxPlayMgr::GetHostPlay()->StartShadow(nShowShadow, 30);
	}
	else if (nShowShadow == 2)
	{
		std::set<Play*>& players =  NxPlayMgr::GetAllPlay();
		std::set<Play*>::iterator itr;
		for (itr = players.begin(); itr != players.end(); ++itr)
		{
			(*itr)->StartShadow(nShowShadow, 30);
		}
	}
}
void CLevelManager::RenderShadow(NiCamera* pkCamera)
{
	std::set<Play*>& players =  NxPlayMgr::GetAllPlay();
	std::set<Play*>::iterator itr;
	for (itr = players.begin(); itr != players.end(); ++itr)
	{
		CALL((*itr), RenderShadow(pkCamera));
	}
	
}