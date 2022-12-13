#include "Top-KartPCH.h"
#include ".\nxscenemgr.h"

#include "Utility.h"
#include <NiAnimation.h>
#include <NiApplication.h>
#include <NiMain.h>
#include <NiParticle.h>
#include <NiPhysX.h>
#include <NiMetricsLayer.h>

#if defined(_XENON)
#include <NiXenonRenderer.h>
#include <NiXenonRenderTargetGroupData.h>
#include <NiResolvableDepthStencilBuffer.h>
#elif defined(WIN32)
#include <NiDirectXVersion.h>
#include <NiDX9Renderer.h>
#endif

#include "Clock.h"
#include "CollisionManager.h"
#include "EffectAccumulator.h"
#include "GameStatusManager.h"
#include "InputManager.h"

#include "LevelTasks.h"
#include "Play.h"
#include "EffectManager.h"
#include "PowerUpManager.h"
#include "Scheduler.h"
#include "SoundTasks.h"
#include "HUDTasks.h"
#include "Footprints.h"
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
#include "PowerController.h"
//#include "BaseItem.h"
#include "NO2Controller.h"
#include "CameraController.h"
#include "CameraBase.h"

#include "TerrainManager.h"
#include "FenceManager.h"
#include "./PhysKart/NxAllVehicles.h"
#include "math.h"
#include "PhysXManager.h"


#include "NxPlayMgr.h" 
#include "singlet.h"
#include "PlayerRank.h"


#include "framework/gui_i.h"
#include "Platform/GlobalInterface.h"
#include "./Platform/GlobalInterface.h"
#include "./Platform/MsgDataCache.h"

#include <NiD3DShaderFactory.h>

#ifdef NI_USE_MEMORY_MANAGEMENT
#include <NiMemManager.h>
#endif

#include "SceneLevel.h"


#ifdef TP_USE_CONSLE
//------------------------------------------------------------------------------
//Console

//------------------------------------------------------------------------------
void NxSceneMgr::OnRecvConsleCommand(char* buffer,int len)
{
	//buffer是否要处理一下？
	if(strcmp( buffer, "ScriptFuction") == 0)
	{
		NxSceneMgr::Get()->GetScriptVM()->ExecuteScriptFile("consolefuction.lua");
	}
	else
	{
		NxSceneMgr::Get()->GetScriptVM()->ExecuteScript(buffer);
	}
}
//------------------------------------------------------------------------------
static void StartUpServer()
{
	if(NxSceneMgr::Get()->m_spServer == NULL)
	{
		CConsSocket::StartUp();
		NxSceneMgr::Get()->m_spServer = new CConsServer(NxSceneMgr::OnRecvConsleCommand);
		NxSceneMgr::Get()->m_spServer->Init();
	}
}
//------------------------------------------------------------------------------
static void ShutDownServer()
{
	if(NxSceneMgr::Get()->m_spServer)
	{
		NxSceneMgr::Get()->m_spServer->close();
		delete NxSceneMgr::Get()->m_spServer;
		NxSceneMgr::Get()->m_spServer = NULL;
		CConsSocket::ShutDown();
	}
}

#endif

//-----------------------------------------------------------------------

std::set< CSceneLevel * >			NxSceneMgr::m_AllScene;
NxSceneMgr					*		NxSceneMgr::ms_pkSceneMgr = NULL;
UINT								NxSceneMgr::ms_nActiveSceneID  = SCENELEVEL_NONE;



//-----------------------------------------------------------------------
NxSceneMgr::NxSceneMgr():
m_pkCameraController(0),
//m_pkCollisionManager(0),
m_pkGameStatusManager(0),
m_pkPowerUpManager(0),
m_pkProfilingController(0),
m_pkEffectManager(0)
//m_pkPowerController(0),
//m_pkNO2Controller(0)
//m_sGameModeBase(0),
//m_sGameModeSub(0)
{

}
NxSceneMgr::~NxSceneMgr()
{
	//Destory();
}

//-----------------------------------------------------------------------
bool NxSceneMgr::Create()
{
	if( ms_pkSceneMgr )
		return true;

	NxSceneMgr::ms_pkSceneMgr = NiNew NxSceneMgr;
	if (!NxSceneMgr::ms_pkSceneMgr){
		NiOutputDebugString("NxSceneMgr::Create -- "
			"NxSceneMgr Creation Failed\n");
		return false;
	}

	NxSceneMgr::ms_pkSceneMgr->m_pScriptVM = new ScriptVM;
	if( !NxSceneMgr::ms_pkSceneMgr->m_pScriptVM->Init()){
		return false;
	}
	NxSceneMgr::ms_pkSceneMgr->m_pScriptVM->ExecuteScriptFile("Scripts/DescConstant.lua", true);
	NxSceneMgr::ms_pkSceneMgr->m_pScriptVM->ExecuteScriptFile("Scripts/GPSettings.lua", true);

	NxSceneMgr::ms_pkSceneMgr->m_pkCameraController		= NiNew CCameraController;
	//NxSceneMgr::ms_pkSceneMgr->m_pkCollisionManager		= NiNew CollisionManager;
	NxSceneMgr::ms_pkSceneMgr->m_pkGameStatusManager	= NiNew GameStatusManager;
	NxSceneMgr::ms_pkSceneMgr->m_pkPowerUpManager		= NiNew PowerUpManager;
	NxSceneMgr::ms_pkSceneMgr->m_pkProfilingController	= NiNew ProfilingController;
	//NxSceneMgr::ms_pkSceneMgr->m_pkPowerController		= NiNew CPowerController;
	//NxSceneMgr::ms_pkSceneMgr->m_pkNO2Controller		= NiNew CNO2Controller;
	//NxSceneMgr::ms_pkSceneMgr->m_pkAccePanel			= NiNew CAcceleratePanel(126, 280,180);
	//NxSceneMgr::ms_pkSceneMgr->m_pkPlayerRankMgr		= NiNew CPlayerRankMgr;
	/*
	if(ms_pkSceneMgr->m_pkPowerController)
		ms_pkSceneMgr->m_pkPowerController->SetPowerBarPos(GameMain::GetScreenWidth()/2 - 150, 
			GameMain::GetScreenHeight() - 85 );*/


	/*if(ms_pkSceneMgr->m_pkAccePanel)
	{
		ms_pkSceneMgr->m_pkAccePanel->SetBaseTexture("AcceleratePanel/mph.dds");
		ms_pkSceneMgr->m_pkAccePanel->SetDynamicTexture("AcceleratePanel/mph_color.dds");
		ms_pkSceneMgr->m_pkAccePanel->MoveTo(0.72f, 0.75f);

	}*/
	

/*
	if(!CItemMgr::Create())
	{
		return false;
	}
*/
	if (!NxSceneMgr::ms_pkSceneMgr->m_pkCameraController || 
		//!NxSceneMgr::ms_pkSceneMgr->m_pkCollisionManager || 
		!NxSceneMgr::ms_pkSceneMgr->m_pkGameStatusManager||         
		!NxSceneMgr::ms_pkSceneMgr->m_pkPowerUpManager	 ||
		!NxSceneMgr::ms_pkSceneMgr->m_pkProfilingController)
	{
		NiOutputDebugString("NxSceneMgr::Create -- "
			"Manager Allocation Failed\n");
		return false;
	}

	if (!NxSceneMgr::ms_pkSceneMgr->m_pkGameStatusManager->Initialize())
	{
		NiOutputDebugString("NxSceneMgr::Create -- "
			"GameStatusManager Initialization Failed\n");
		return false;
	}
	NxSceneMgr::ms_pkSceneMgr->m_pkCameraController->LoadConfig();



#ifndef NO_SOUND
	PlayAmbientSoundTask* pkSoundTask = NiNew PlayAmbientSoundTask;
	if (pkSoundTask)
	{  
		Scheduler::Get()->Schedule(pkSoundTask);
	}

#endif

	//Console
#ifdef TP_USE_CONSLE

	NxSceneMgr::ms_pkSceneMgr->m_spServer = NULL;
	StartUpServer();
#endif
	NxSceneMgr::ms_pkSceneMgr->m_unGameSvrID = 0;
	return true;    

}

void NxSceneMgr::Destory()
{		
	if(NxSceneMgr::Get()->m_pScriptVM)
	{
		NxSceneMgr::Get()->m_pScriptVM->Destroy();
		delete NxSceneMgr::Get()->m_pScriptVM;
		NxSceneMgr::Get()->m_pScriptVM = NULL;
	}

//	if(CItemMgr::Get())
//		CItemMgr::Destroy();

	SAFE_NIDELETE(NxSceneMgr::Get()->m_pkCameraController);
	//SAFE_NIDELETE(m_pkCollisionManager);
	SAFE_NIDELETE(NxSceneMgr::Get()->m_pkGameStatusManager);
	SAFE_NIDELETE(NxSceneMgr::Get()->m_pkPowerUpManager);
	SAFE_NIDELETE(NxSceneMgr::Get()->m_pkProfilingController);
	/*SAFE_NIDELETE(NxSceneMgr::Get()->m_pkPowerController);
	SAFE_NIDELETE(NxSceneMgr::Get()->m_pkNO2Controller);
	SAFE_NIDELETE(NxSceneMgr::Get()->m_pkAccePanel);*/
	//SAFE_NIDELETE(NxSceneMgr::Get()->m_pkPlayerRankMgr);

	
	

	//Console
#ifdef TP_USE_CONSLE
	ShutDownServer();
#endif
	SAFE_NIDELETE(ms_pkSceneMgr);
	ms_pkSceneMgr = NULL;
}
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
/*
	修改人：	ray
	日期：	1/30/2007
	描叙：	提供比赛模式接口
*/
short NxSceneMgr::GetCurGameModeBase()
{
	return GlobalInterface::m_pDataCache->m_RoomDesc.GameModeBase;
}

short NxSceneMgr::GetCurGameModeSub()
{
	return GlobalInterface::m_pDataCache->m_RoomDesc.GameModeSub;
}

//---------------------------------------------------------------------------
