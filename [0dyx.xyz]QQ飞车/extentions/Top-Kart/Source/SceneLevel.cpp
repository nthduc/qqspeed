#include "Top-KartPCH.h"
#include ".\scenelevel.h"
#include "Utility.h"
#include <NiAnimation.h>
#include <NiApplication.h>
#include <NiMain.h>
#include <NiParticle.h>
#include <NiPhysX.h>
#include <NiMetricsLayer.h>
#include "gpcontrol.h"
#include "physxtask.h"

#if defined(_XENON)
#include <NiXenonRenderer.h>
#include <NiXenonRenderTargetGroupData.h>
#include <NiResolvableDepthStencilBuffer.h>
#elif defined(WIN32)
#include <NiDirectXVersion.h>
#include <NiDX9Renderer.h>
#endif

#include "Clock.h"
#include "CameraController.h"
#include "CollisionManager.h"
#include "EffectAccumulator.h"
#include "GameStatusManager.h"
#include "InputManager.h"
#include "LevelTasks.h"
#include "Play.h"
#include "EffectManager.h"
#include "PowerUpManager.h"
#include "Scheduler.h"
#include "SoundManager.h"
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
#include "GameAward.h"

#include "TerrainManager.h"
#include "FenceManager.h"
#include "./PhysKart/NxAllVehicles.h"
#include "math.h"
#include "PhysXManager.h"


#include "NxPlayMgr.h" 

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
#include "Track/PlayerPosUpdateTask.h"
#include "GPNetInterCommn.h"
#include "common.h"
#include "RenderBase.h"
#include "NO2Controller.h"
#include "./Platform/GlobalInterface.h"
#include "./Platform/MsgDataCache.h"

#include "Win32SoundManager.h"
#include <NiD3DShaderFactory.h>

#ifdef NI_USE_MEMORY_MANAGEMENT
#include <NiMemManager.h>
#endif


#if defined(WIN32)
#if defined(_DX8)
#include <NiDX8Renderer.h>
#elif defined (_DX9)
#include <NiDX9Renderer.h>
#elif defined(_XBOX)
#include <NiXBoxRenderer.h>
#endif
#endif // #if defined(WIN32)
#define max(a,b)    (((a) > (b)) ? (a) : (b))
const  float BF_CR = 0.6f;
const  float BF_CG = 0.6f;
const  float BF_CB = 0.75f;
//-----------------------------------------------------------------------
struct Map_Desc
	: public NiMemObject
{
	struct Collider
		: public NiMemObject
	{
		Collider(int nType, int nGroup, const NiFixedString& kName)
			: nColliderType(nType), nGroupType(nGroup), kFileName(kName){}
			int				nColliderType;
			int				nGroupType;
			NiFixedString	kFileName;		
	};
	int			  nHardness;
	NiFixedString kMapName;	
	NiFixedString kCheckPointFileName;
	NiFixedString kPlaybackCameraFileName;
	NiFixedString kEagleMapFileName;
	NiFixedString kPickMapFileName;
	NiFixedString kSceneFileName;
	NiFixedString kSmallIconFileName;
	NiFixedString kBigIconFileName;
	NiFixedString kIntroCameraName;
	NiTPrimitiveArray<Collider*> kArrStaticColliders;
	NiTPrimitiveArray<Collider*> kArrAnimColliders;
	NiTPrimitiveArray<Collider*> kArrActiveColliders;

	~Map_Desc()
	{
		Destroy(kArrStaticColliders);
		Destroy(kArrAnimColliders);
		Destroy(kArrActiveColliders);
	}
protected:
	void Destroy(NiTPrimitiveArray<Collider*> & kArr)
	{
		unsigned int uiNum = kArr.GetSize();
		for (unsigned int i = 0; i < uiNum; ++i)
		{
			Collider * pkCollider = kArr.GetAt(i);
			NiDelete pkCollider;
		}
		kArr.RemoveAll();
	}
};
//-----------------------------------------------------------------------

CSceneLevel::CSceneLevel(SHORT nLap):
m_fElapseTime(0), 
m_fPrevFrameTime(0),
m_fLevelStartTime(0),
//m_eLevelLoaded(LEVEL_NONE),
m_kVisible(1024, 1024), // Maybe shouldn't be here
m_kCuller(&m_kVisible),
m_pkPlayBack(NULL),
m_spMoveNode(NULL),// Maybe shouldn't be here
m_pPhysXManager(0),
m_nSceneID(SCENELEVEL_NONE),
m_pkLinks(NULL),
m_spScene(0),
m_spStaticRoot(0),
m_spLevelRoot(0),
m_fAppStartTime(0),
m_spGameCamera(0),
m_spEffectSorter(0),
m_tpRenderer(NULL),
m_tpCamera(NULL),
m_pkDXRenderer(0),
m_pkDevice(0),
m_nLap(nLap),
m_pSceneHelper(NULL),
m_bRaceOver(FALSE)
{
	m_pkPlay = 0;
}

CSceneLevel::~CSceneLevel()
{
	UnloadLevel();
}
//-----------------------------------------------------------------------

bool CSceneLevel::LoadLevel(UINT SceneID )
{
	m_nSceneID = SceneID;

	if (!MommaUpdateAnimationsTask::ms_bCreated)
	{
		MommaUpdateAnimationsTask* pkUpdateAnimsTask =  NiNew MommaUpdateAnimationsTask();

		if (!pkUpdateAnimsTask)
			return false;

		Scheduler::Get()->Schedule(pkUpdateAnimsTask);
	}

	return true;
}

void CSceneLevel::UnloadLevel()
{
	m_spScene = 0;
	NxSceneMgr::Get()->GetSoundManager()->StopAllSound();
}

void CSceneLevel::PreDraw()
{
	m_tpRenderer = NiRenderer::GetRenderer();
	m_tpCamera = GetCamera();

	assert(m_tpRenderer); //&& m_tpCamera

	m_tpRenderer->BeginFrame();

#if defined(WIN32)

#if defined (_DX8)
	m_pkDXRenderer = (NiDX8Renderer*)m_tpRenderer;
	m_pkDevice = m_pkDXRenderer->GetD3DDevice();
#elif defined (_DX9)
	m_pkDXRenderer = (NiDX9Renderer*)m_tpRenderer;
	m_pkDevice = m_pkDXRenderer->GetD3DDevice();
#endif

	HRESULT eRes = m_pkDevice->TestCooperativeLevel();
	if (eRes == D3DERR_DEVICENOTRESET)
	{
		NiOutputDebugString("Resetting shadows...\n");
	}
#endif //#if defined(WIN32) 
}

void CSceneLevel::EndDraw()
{
	NiRenderer* pkRenderer = m_tpRenderer;

	if(pkRenderer)
	{
		pkRenderer->EndFrame();
		pkRenderer->DisplayFrame();
	}
}

void CSceneLevel::PartialRender(NiCamera* pkCamera, NiAVObject* pkObject)
{
	// Cull first.
	float fBeginTime = NiGetCurrentTimeInSec();

	m_kVisible.RemoveAll();
	m_kCuller.Process(pkCamera, pkObject, 0);

	float fEndTime = NiGetCurrentTimeInSec();
	((GameMain*)NiApplication::ms_pkApplication)->
		AddCullTime(fEndTime - fBeginTime);

	((GameMain*)NiApplication::ms_pkApplication)->
		AddVisObjects((float)m_kVisible.GetCount());

	// Draw
	NiDrawVisibleArrayAppend(m_kVisible);
}
//#define _FINAL


//-----------------------------------------------------------------------
void CSceneLevel::Update(float fTime)
{
	NIMETRICS_EVAL(NiMetricsScopePCTimer kTimer("SceneLevel.Update"));

	fTime = GetMainApp()->GetAccumTime();
	if (m_pkPlayBack && m_pkPlayBack->IsPlayBack()) 
	{
		m_pkPlayBack->Update(NiApplication::ms_pkApplication->GetCurrentTime());
	}

	//游戏中场景，时间，power等更新
	if (NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == 
		GameStatusManager::PLAYING_GAME)
	{
		//过去时间
		m_fElapseTime += fTime - m_fPrevFrameTime;
/*
		if(GlobalInterface::m_pDataCache)
		{
			if(GlobalInterface::m_pDataCache->m_RoomDesc.GameModeBase == EGBI_BASEMODE_RACE)
			{
				if(NxPlayMgr::GetHostPlay()->GetKart()->IsDrifting())
				{

					NxSceneMgr::Get()->GetPowerController()->AccumulatePower(
						unsigned int( NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * 0.25)
						);
				}
			
			}
		}
*/	}

	if(NxSceneMgr::Get())
	{
		NxSceneMgr::Get()->GetNO2Controller()->Update();
	}

	//修改人： ray     日期：1/2/2007
	//更新对象队列
	if(CRenderMgr::Get())
	{
		CRenderMgr::Get()->Update(fTime);
	}

	EventViewerMgr::Update(fTime - m_fPrevFrameTime);

	//update PhysX
	if ( !m_bRaceOver && m_pPhysXManager)
	{
		// 修改人:　wincoqin
		// 时间： 2007/1/18
		// 修改目的: 让物理更合理运转
		/*
		m_pPhysXManager->UpdateDestinations(fTime);
		PROFILE("PHYSX_PROCESS");

		if (m_pPhysXManager->Fetch(m_fPrevFrameTime))
		{
		NxPlayMgr::UpdateAllPhysicPlay(fTime - m_fPrevFrameTime);
		m_pPhysXManager->Sim(fTime);

		}

		m_pPhysXManager->UpdateSources(fTime);
		*/

		PROFILE("Phys");
		
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
			m_pPhysXManager->UpdateSources(fTime + fDt);
			m_pPhysXManager->Sim(fTime + fDt);
		}
		// 修改结束
		
	}
	

	m_fPrevFrameTime = fTime;

	NiCamera* pkCamera = NxSceneMgr::Get()->GetCameraController()->GetCamera();

	if (m_pSceneHelper) 
	{
		m_pSceneHelper->CulledScene(pkCamera);
	}

	// 修改人:　wincoqin
	// 时间：2007/01/18
	// 修改目的：　提高更新效率

	if(m_spMoveNode)
	{
		PROFILE("scene update");
		m_spMoveNode->Update(fTime);
		//m_spStaticRoot->UpdateSelected(fTime);
	}
	// 修改结束

	NiPoint3 pFace;
	if( NxPlayMgr::GetHostPlay())
	{
		PROFILE("updatecamera");
		NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate().GetCol(1, pFace);
		//m_pkPlay->GetNIFRoot()->GetRotate().GetCol(1,pFace);
		pFace = -pFace;
		NiPoint3 pUp;


		NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate().GetCol(1, pUp);
		//m_pkPlay->GetNIFRoot()->GetRotate().GetCol(2,pUp);

		NiPoint3 kWheelCenter(0, 0,0);
		NiAVObject *pModel = NxPlayMgr::GetHostPlay()->GetKartNIFRoot();
		kWheelCenter = kWheelCenter + pModel->GetObjectByName("RF_W")->GetWorldTranslate();
		kWheelCenter = kWheelCenter + pModel->GetObjectByName("LF_W")->GetWorldTranslate();
		kWheelCenter = kWheelCenter + pModel->GetObjectByName("LB_W")->GetWorldTranslate();
		kWheelCenter = kWheelCenter + pModel->GetObjectByName("RB_W")->GetWorldTranslate();
		kWheelCenter = kWheelCenter /4.0f;

		NxSceneMgr::Get()->GetCameraController()->UpdateCamera(
			kWheelCenter,
			//NxPlayMgr::GetHostPlay()->GetPos(), 
			pFace,
			pUp,
			InputManager::Get()->GetZoomFactor(),
			true);
	}
	
	// 修改人: wincoqin
	// 修改原因: 场景和人物目前都没有mophy,所以屏蔽下面代码
/*
	// morph trees manually since they are clones
	NiTListIterator kPos = m_kTreeMorphList.GetHeadPos();
	while (kPos)
	{
		NiGeomMorpherController* pkMC = m_kTreeMorphList.GetNext(kPos);
		pkMC->OnPreDisplay();
	}
*/
	// 修改结束

#ifdef TP_USE_CONSLE
	NxSceneMgr::Get()->m_spServer->SendOutMsg();
	NxSceneMgr::Get()->m_spServer->SendErrMsg();
#endif

	CGameEffectMgr::Update(fTime);

	m_sKartCrack.Update(fTime);

	if ( CGPControl::Get() )
	{
		CGPControl::Get()->Update(1.0);
	}

	if( NxGamePkgMgr::Get())
		NxGamePkgMgr::Get()->Update(fTime);


	NxSceneMgr::Get()->GetGameStatusManager()->SetWaitingForUpdate(false);
	//CGameOverCountDnMgr::UpdateCountDownTast(fTime);
}

STDMETHODIMP CSceneLevel::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

//---------------------------------------------------------------------------
void CSceneLevel::AddCollider(NiAVObject* pkObject)
{
	CollisionManager::AddCollider(pkObject);
}

void CSceneLevel::RemoveAllColliders()
{
	CollisionManager::RemoveAllColliders();
}


const NiTPointerList<NiAVObject*> * CSceneLevel::GetColliders() 
{
	return CollisionManager::GetColliders();
	
}

//-----------------------------------------------------------------------
inline void GetCollidersInfo(NiTPrimitiveArray<Map_Desc::Collider*> & kArrColliders, CVarArray * pColliderArray, const char * pcFilePath)
{
	int nStaticCollidersNum = pColliderArray->GetNum();
	CStringEx strTmp;
	for (int i = 0; i < nStaticCollidersNum; ++i) 
	{
		CVarTable * pVarTable = (CVarTable*) (pColliderArray->Element(i));
		assert(pVarTable->GetType() == VAR_TABLE);

		int nColliderType = pVarTable->Element("type")->Integer();
		int nGroupType = pVarTable->Element("group")->Integer();
		/*
		const char * pFileName = MakeString("%s%s", pcFilePath, pVarTable->Element("filename")->String());
		assert(pFileName);
		if (pFileName) 
		{
			kArrColliders.Add(NiNew Map_Desc::Collider(nColliderType, nGroupType, pFileName));
		}
		*/
		strTmp.Format("%s%s", pcFilePath, pVarTable->Element("filename")->String());
		kArrColliders.Add(NiNew Map_Desc::Collider(nColliderType, nGroupType, strTmp.c_str()));
	}
}
//-----------------------------------------------------------------------
inline void LoadCollidersInfo(NiTPrimitiveArray<Map_Desc::Collider*> & kArrColliders, PhysXManager *& pPhysXMgr, NiNode * pkFatherNode = NULL)
{
	assert(pPhysXMgr);
	unsigned int uiColliderNum = kArrColliders.GetSize();

	for (unsigned int i = 0; i < uiColliderNum; ++i)
	{
		Map_Desc::Collider * pkCollider = kArrColliders.GetAt(i);
		XUserData udObj;
		udObj._actorType = (XUserData::ACTORTYPE)pkCollider->nColliderType;

		pPhysXMgr->LoadPhysObject(pkCollider->kFileName, pkFatherNode, (PhysXManager::COLLISIONGROUP)pkCollider->nGroupType, udObj);
	}
}

void CSceneLevel::ImmRender(NiCamera* pkCamera,NiAVObject* pkObject)
{
		NiDrawScene(pkCamera,pkObject,m_kCuller);
}

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

CRaceGameSceneLevel::CRaceGameSceneLevel(SHORT nLap):CSceneLevel(nLap)
{
	m_nLap = nLap;
	m_fAppStartTime = NiGetCurrentTimeInSec();
	m_nPlayerPosUpdateTaskID = -1; // 初始化为-1

}

CRaceGameSceneLevel::~CRaceGameSceneLevel()
{

}

void CSceneLevel::UseCamera(eCameraType CameraType)
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
//-----------------------------------------------------------------------

void CRaceGameSceneLevel::Restart()
{
	// bug   并未重置所有 使用的静态变量

	m_fElapseTime = 0.0f;        
	m_fLevelStartTime = Scheduler::Get()->GetClock().GetTime();
	m_bRaceOver = false;

	assert(CKartStatusManager::Get());
	m_pkLinks->ClearRecord();
	//CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);
	if( NxGamePkgMgr::Get())
		NxGamePkgMgr::Get()->Restart();

	//开始各界面元素
	{
	

		//if(CItemMgr::Get())
		//	CItemMgr::Get()->TurnOnColumnShow();

		CGPControl::Get()->Start();

		if(NxSceneMgr::Get())
		{
			if(GlobalInterface::m_pDataCache)
			{
				if(GlobalInterface::m_pDataCache->m_RoomDesc.GameModeBase == EGBI_BASEMODE_RACE)
				{				
					NxSceneMgr::Get()->GetPowerController()->SetActivity(true);
				}
				else
				{
					NxSceneMgr::Get()->GetPowerController()->SetActivity(false);
				}
			}

			NxSceneMgr::Get()->GetAccelPanel()->SetActivity(true);
			NxSceneMgr::Get()->GetPlayerRankMgr()->SetActivity(true);
		}	

		EventViewerMgr::TurnOnViewer();

		ScreenOverlay::SetVisible(true);
	}
	
/*
	CountDownTask * pkCountDownTask = NiNew CountDownTask;
	pkCountDownTask->SetParam(CountDownTask::CDM_BEGIN,5.0f);
	Scheduler::Get()->Schedule(pkCountDownTask);
	*/
}

//-----------------------------------------------------------------------
void CRaceGameSceneLevel::UnloadLevel()
{
	CGPControl::Destroy();
	SAFE_NIDELETE(m_pSceneHelper);
	if (Scheduler::Get() && -1 != m_nPlayerPosUpdateTaskID)
	{
		// 结束玩家位置更新task
		Scheduler::Get()->Terminate(m_nPlayerPosUpdateTaskID);
	}
	if (NxPlayMgr::GetHostPlay())
		NxPlayMgr::GetHostPlay()->UnLightObject(m_spScene);	
	EventViewerMgr::Destroy();

	RemoveAllColliders();

	if (NxPlayMgr::GetHostPlay()) 
	{
		NxPlayMgr::GetHostPlay()->TerminateShadow();
	}

	CGameAward::Destroy();

	NxPlayMgr::UnPhysicAllPlay();

	if (m_pkPlayBack) {
		m_pkPlayBack->Save("Play.txt");
		SAFE_NIDELETE(m_pkPlayBack);
	}
	m_spGameCamera = 0;
	
	m_sKartCrack.Destroy();
	m_spEffectSorter =NULL;

	CGlobalMap::Destroy();
	CEagleEyeMap::Destroy();

	if (m_pkLinks){
		SAFE_NIDELETE(m_pkLinks);		
	}

	CGameEffectMgr::DestroyEffect();
	CKartStatusManager::Destroy();

	float fTotalTimeInMins = NiGetCurrentTimeInSec() - m_fAppStartTime;
	fTotalTimeInMins /= 60.0f;

	NIMETRICS_ADDVALUE("Time in Demo (mins)", fTotalTimeInMins);
	SAFE_NIDELETE(m_pPhysXManager);
	//SAFE_NIDELETE(m_pkPlay);
	m_spMoveNode = 0;	

	CSceneLevel::UnloadLevel();


}

//-----------------------------------------------------------------------
bool CRaceGameSceneLevel::LoadLevel(UINT SceneID )
{ 
	if( !CSceneLevel::LoadLevel(SceneID))
		return false;

	CGPControl::Create();
	CKartStatusManager::Create();
	CPlayerPosUpdateTask* pkPosUpdateTask = 
		NiNew CPlayerPosUpdateTask;

	if (!pkPosUpdateTask) 
	{
		return false;
	}

	Scheduler::Get()->Schedule(pkPosUpdateTask);

	m_nPlayerPosUpdateTaskID = pkPosUpdateTask->GetID();

	assert(m_nPlayerPosUpdateTaskID >= 0);
	
	
	
	m_spEffectSorter			= NiNew NiAlphaAccumulator;
	m_pkLinks					= NiNew CLinks;

	CStringEx strTmp,strPath;
	strPath.Format("Map/Common Map/Map0%d/",SceneID);
	/*移到初始化物理引擎后	pighuang 20070123
	CStringEx strTmp,strPath;
	strPath.Format("Map/Common Map/Map0%d/",SceneID);
	strTmp.Format("%scheckpoint.nif",strPath.c_str());
	m_pkLinks->LoadLinks(strTmp.c_str());
	m_pkLinks->SetLaps(max(m_nLap,1));
	m_pkLinks->SetCorrectRunDir(dirClockWise);
	*/

	KartInfo kartInfo;
	m_pkPlayBack = NiNew CKartPlayBack(kartInfo);
	/*因为要使用m_spMoveNode,时序问题需要移到后面 pighuang 20070226
	///赋值残影特效需要跟踪的物体对象
	m_sKartCrack.Init(
		NiDynamicCast(NiNode,NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_1")));
	*/
	//m_pkPlay = NxPlayMgr::GetHostPlay();

	if (!CGlobalMap::Create()) {
		NiOutputDebugString("LevelManager::CGlobalMap Create -- "
			"Manager Allocation Failed\n");
		return false;
	}
	if (!CEagleEyeMap::Create(NiRenderer::GetRenderer(),strPath.c_str())) 
	{
		NiOutputDebugString("LevelManager::Create CEagleEyeMap "
			"GameStatusManager Initialization Failed\n");
		return false;
	}

	CGameEffectMgr::InitEffect();

	m_spScene = NiNew NiNode;

	if(!m_spScene){ return false; }

	//m_eLevelLoaded = eWhichLevel;

	m_spScene->SetName("SceneRoot");

	Map_Desc * pMapDesc = NiNew Map_Desc;
	CVarTable map_desc("map_desc");
	strTmp.Format("%smap_desc.lua",strPath.c_str());
	CVar::LoadVarsFromFile(&map_desc, NiApplication::ConvertMediaFilename(strTmp.c_str()));
	pMapDesc->kMapName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_name")->String());
	pMapDesc->nHardness = map_desc.Element("map_hardness")->Integer();
	pMapDesc->kCheckPointFileName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_checkpoint")->String());
	pMapDesc->kPlaybackCameraFileName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_camplayback")->String());
	pMapDesc->kEagleMapFileName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_eaglemap")->String());
	pMapDesc->kPickMapFileName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_pickmap")->String());
	pMapDesc->kSceneFileName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_scene")->String());
	pMapDesc->kSmallIconFileName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_icon_small")->String());
	pMapDesc->kBigIconFileName = strTmp.FormatPt("%s%s", strPath.c_str(), map_desc.Element("map_icon_big")->String());

	CVar * pVarCamera = map_desc.Element("camera");
	if (pVarCamera)
	{
		pMapDesc->kIntroCameraName = strTmp.FormatPt("%s%s", strPath.c_str(), pVarCamera->Element("intro_camera")->String());
	}

	CVarArray * pStaticColliders = (CVarArray*)(map_desc.Element("map_static_colliders"));
	GetCollidersInfo(pMapDesc->kArrStaticColliders, pStaticColliders, strPath.c_str());

	CVarArray * pAnimColliders = (CVarArray*)(map_desc.Element("map_anim_colliders"));
	GetCollidersInfo(pMapDesc->kArrAnimColliders, pAnimColliders, strPath.c_str());

	CVarArray * pActiveColliders = (CVarArray*)(map_desc.Element("map_active_colliders"));
	GetCollidersInfo(pMapDesc->kArrActiveColliders, pActiveColliders, strPath.c_str());

	//Load the main scene file
	NiTObjectArray<NiObjectPtr> kExtraObjects;
	m_spLevelRoot = OpenNifAndPrepack(pMapDesc->kSceneFileName, &kExtraObjects);

	m_spMoveNode = (NiNode*) m_spLevelRoot->GetObjectByName("sc_move");			

	m_spStaticRoot = NiDynamicCast(NiNode, m_spLevelRoot->GetObjectByName("sc_freeze"));
	if (!m_spLevelRoot || !m_spStaticRoot || !m_spMoveNode)
	{
		NiOutputDebugString("LevelManager::LoadLevel -- "
			"赛道 Creation Failed\n");
		return false;
	}
	
	///赋值残影特效需要跟踪的物体对象
	m_sKartCrack.Init(
		NiDynamicCast(NiNode,NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_1")));

	SAFE_NIDELETE(m_pPhysXManager);
	m_pPhysXManager = NiNew PhysXManager(pMapDesc->kPickMapFileName);

	// 加载静态物体
	LoadCollidersInfo(pMapDesc->kArrStaticColliders, m_pPhysXManager);
	// 加载动画物体
	LoadCollidersInfo(pMapDesc->kArrAnimColliders, m_pPhysXManager, m_spMoveNode);
	// 加载活动物体
	LoadCollidersInfo(pMapDesc->kArrActiveColliders, m_pPhysXManager, m_spMoveNode);

	NxPlayMgr::PhysicAllPlay();

	NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GAME_START);

	m_spScene->AttachChild(m_spLevelRoot);

	
	m_spSkyNode = NiDynamicCast(NiNode, m_spStaticRoot->GetObjectByName("sky")); 

	if (m_spSkyNode)
	{
		m_spStaticRoot->DetachChild(m_spSkyNode);
		m_spStaticRoot->UpdateNodeBound();
		m_spLevelRoot->AttachChild(m_spSkyNode);
		m_spLevelRoot->UpdateNodeBound();
	}

	//todo:移到读checkpoint点后	pighuang 20070123
	//CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);
	//Get Flyover camera

	//Get Flyover camera//////////////////////////////////////////////////////////////////
	//Get Flyover camera
	/*
	NiNode* pkCamObj = 
	(NiNode*)m_spStaticRoot->GetObjectByName("cam");
	m_spGameCamera = NiDynamicCast(NiCamera, pkCamObj->GetObjectByName("startcam_front"));//GetAt(0));//->GetAt(0));
	assert(m_spGameCamera);*/
	m_spGameCamera = NiNew NiCamera;
	NiFrustum kFrustum;
	kFrustum.m_fTop = 0.3107f;
	kFrustum.m_fBottom = -0.3107f;
	kFrustum.m_fLeft = -0.4142f;
	kFrustum.m_fRight = 0.4142f;
	kFrustum.m_fNear = 1.0f;
	kFrustum.m_fFar = 5000.f;
	m_spGameCamera->SetViewFrustum(kFrustum);

	NxPlayMgr::GetHostPlay()->UseStartCamera();
	NxSceneMgr::Get()->GetCameraController()->PlayAnim();

	//载入地图startpoint,checkpoint,道具点数据
	strTmp.Format("%scheckpoint.nif",strPath.c_str());
	m_pkLinks->LoadLinks(strTmp.c_str());
	m_pkLinks->SetLaps(max(m_nLap,1));
	m_pkLinks->SetCorrectRunDir(dirClockWise);

	CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);

	NxPlayMgr::GetHostPlay()->UseStartCamera();
	NxSceneMgr::Get()->GetCameraController()->PlayAnim();

	m_spScene->Update(0.0f);

	CGameEffectMgr::ResetEffect();
	CGameEffectMgr::ActiveEffect(IGameEffect::ET_SNOW,NULL);
	CGameEffectMgr::ActiveEffect(IGameEffect::ET_SPEED,NULL);

	NiTimeController::StartAnimations(m_spScene, GetMainApp()->GetAccumTime());

	m_pSceneHelper = NiNew CSceneHelper(m_spLevelRoot);
	if (!m_pSceneHelper || !m_pSceneHelper->Init(&map_desc)) 
	{
		return false;
	}

	BOOL bShowShadow = TRUE;
	int  nShadowLevel = 6;
	if ( GlobalInterface::m_pSettings )
	{
		GlobalInterface::m_pSettings->GetIntValue("ShowShadow", &bShowShadow);
		GlobalInterface::m_pSettings->GetIntValue("ShadowLevel", &nShadowLevel);
	}
	if ( bShowShadow && NxPlayMgr::GetHostPlay() )
		NxPlayMgr::GetHostPlay()->StartShadow(nShadowLevel, 30);


	m_pPhysXManager->m_spMasterScene->Simulate(0.01f);

	SAFE_NIDELETE(pMapDesc);

	if (!EventViewerMgr::CreateEventViewerMgr(m_spLevelRoot))
	{
		return false;
	}
	//NxPlayMgr::GetHostPlay()->LightObject(m_spScene);	
//-----------------------------------------------------------------------

	return true;
}


//-----------------------------------------------------------------------
/*
void CRaceGameSceneLevel::Draw()
{
	PreDraw();
	NiRenderer* pkRenderer = m_tpRenderer;
	NiCamera* pkCamera = m_tpCamera;

	assert(pkCamera);

	// 次函数被wincoqin大量修改,作出了调整,增加了性能检测代码
	// 并将天空和分开渲染,不进行alpha排序, 提高了渲染效率

	//float fPreRenderTime = NiGetCurrentTimeInSec();
	{
		//PROFILE("RenderMap");    
		if (CEagleEyeMap::Get() && NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() 
			== GameStatusManager::PLAYING_GAME)
		{
			CEagleEyeMap::Get()->Render(GetMainApp()->GetCuller());
		}
	}

	if (NxPlayMgr::GetHostPlay()) 
	{
		//PROFILE("RenderShadow");    
		NxPlayMgr::GetHostPlay()->RenderShadow(pkCamera);
	}

	{
		//PROFILE("RenderPortrait");
		NxPlayMgr::RenderAllPlayPortrait(GetMainApp()->GetCuller());
	}
	

	{
		//PROFILE("Render EventViewer");
		EventViewerMgr::Render(GetMainApp()->GetCuller());
	}
	//GetMainApp()->AddAnimTime(NiGetCurrentTimeInSec() - fPreRenderTime);

	pkRenderer->BeginUsingDefaultRenderTargetGroup(NiRenderer::CLEAR_ALL);
	pkRenderer->SetBackgroundColor(NiColorA(BF_CR, BF_CG, BF_CB,1.0f));

	// Override NiCamera::Click
	pkRenderer->ClearBuffer(&pkCamera->GetViewPort(),
		NiRenderer::CLEAR_BACKBUFFER | NiRenderer::CLEAR_ZBUFFER);

	// renderer camera port settings
	pkRenderer->SetCameraData(pkCamera->GetWorldTranslate(),
		pkCamera->GetWorldDirection(), pkCamera->GetWorldUpVector(),
		pkCamera->GetWorldRightVector(), pkCamera->GetViewFrustum(),
		pkCamera->GetViewPort());

	pkRenderer->SetSorter(m_spEffectSorter);
	// DrawSkyBox
	{
		//PROFILE("DrawSkyBox");
		PartialRender(pkCamera, m_spSkyNode);
	}
	m_spEffectSorter->StartAccumulating(pkCamera);
	{
		//PROFILE("DrawScene");
		PartialRender(pkCamera, m_spLevelRoot);
	}

	//play draw 

	{
		//PROFILE("DrawPlayers");
		NxPlayMgr::DrawAllPlay(pkCamera);
	}
	
	{
		//PROFILE("DrawEffectAndCountDown");
		CGameEffectMgr::Render(pkCamera);
		CGameOverCountDnMgr::RenderCountDownTast();
	}

	m_spEffectSorter->FinishAccumulating();
	pkRenderer->SetSorter(NULL);
	
	{
		//PROFILE("DrawCrack");
		m_sKartCrack.Draw(pkCamera);
	}

	{
		{
			//PROFILE("DrawScreen");
			ScreenOverlay::Get()->Draw();
		}

		{
			//PROFILE("DrawOther");
			CGameAward::Get()->Draw();
			NxSceneMgr::Get()->GetCameraController()->Draw2DElements();

			if (CEagleEyeMap::Get() &&	 NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() 
				== GameStatusManager::PLAYING_GAME)  
			{
				CEagleEyeMap::Get()->Draw();
			}

			if (CGlobalMap::Get() &&  NxSceneMgr::Get()->GetGameStatusManager()->GetGameState()
				== GameStatusManager::PLAYING_GAME) 
			{
				CGlobalMap::Get()->Draw(pkRenderer);
			}

			EventViewerMgr::Draw();

			CGameEffectMgr::ScreenRender(NULL);

			//gs_fDrawScreenTime += NiGetCurrentTimeInSec() - fScreenTime1;
			//float fUITime = NiGetCurrentTimeInSec();
			{
				//PROFILE("DrawUI");
				GlobalInterface::m_pGUI->Update(1000);
			}
		}
	}
	NiApplication::ms_pkApplication->RenderVisualTrackers();
	pkRenderer->EndUsingRenderTargetGroup();

	EndDraw();
}*/

// 修改者: wincoqin
// 目的：重新整理这个绘制函数
void CRaceGameSceneLevel::Draw()
{
	PreDraw();
	NiRenderer* pkRenderer = m_tpRenderer;
	NiCamera* pkCamera = m_tpCamera;

	assert(pkCamera);

	float fPreRenderTime = NiGetCurrentTimeInSec();

	NiCullingProcess & kCuller = GetMainApp()->GetCuller();

	if (CEagleEyeMap::Get())
	{
		CEagleEyeMap::Get()->Render(kCuller);
	}

	Play * pPlay = NxPlayMgr::GetHostPlay();
	if (pPlay) 
	{		
		pPlay->RenderShadow(pkCamera);
		pPlay->RenderPortrait(kCuller);
	}

	NxPlayMgr::RenderAllPlayName();
	//NxPlayMgr::RenderAllPlayPortrait(kCuller);

	EventViewerMgr::Render(kCuller);

	GetMainApp()->AddAnimTime(NiGetCurrentTimeInSec() - fPreRenderTime);

	pkRenderer->SetBackgroundColor(NiColorA(BF_CR, BF_CG, BF_CB,1.0f));
	pkRenderer->BeginUsingDefaultRenderTargetGroup(NiRenderer::CLEAR_ALL);

	// renderer camera port settings
	pkRenderer->SetCameraData(pkCamera);
	pkRenderer->SetSorter(m_spEffectSorter);
	// Render Sky
	if (m_spSkyNode)
		PartialRender(pkCamera, m_spSkyNode);
	m_spEffectSorter->StartAccumulating(pkCamera);
	// Render Scene
	PartialRender(pkCamera, m_spLevelRoot);
	// Render Effect
	CGameEffectMgr::Render(pkCamera);
	// Stop Accumulate
	m_spEffectSorter->FinishAccumulating();
	pkRenderer->SetSorter(NULL);

	//绘制玩家放到后面，防止被场景挡住
	// Render Players
	NxPlayMgr::DrawAllPlay(pkCamera);
	// Draw Crack
	m_sKartCrack.Draw(pkCamera);

	//修改人： ray     日期：1/2/2007
	//移动代码，避免render时使用了2d camera   pighuang	20070228
	//渲染对象队列
	if(CRenderMgr::Get())
	{
		CRenderMgr::Get()->Draw(pkRenderer);
	}

	// Draw Screen
	ScreenOverlay::Get()->Draw();
	
	// Draw Award
	CGameAward::Get()->Draw();

	// Draw EagleMap
	if (CEagleEyeMap::Get())  
	{
		CEagleEyeMap::Get()->Draw();
	}

	// Draw GlobalMap
	if (CGlobalMap::Get())				
	{
		CGlobalMap::Get()->Draw(pkRenderer);
	}

	/*
	//修改人： ray     日期：1/2/2007
	//渲染对象队列
	if(CRenderMgr::Get())
	{
		CRenderMgr::Get()->Draw(pkRenderer);
	}
	*/

	// Draw Event
	EventViewerMgr::Draw();
	
	// Draw Screen Effect
	CGameEffectMgr::ScreenRender(NULL);

	// Draw2D elements( include FrameRate, Menu etc.)
	NxSceneMgr::Get()->GetCameraController()->Draw2DElements();

	// Draw UI
	GlobalInterface::m_pGUI->Update(1000);

	NiApplication::ms_pkApplication->RenderVisualTrackers();
	pkRenderer->EndUsingRenderTargetGroup();

	EndDraw();
}


//-----------------------------------------------------------------------
static NiScreenTexture * g_spCarBarnBackdrop = NULL;
void CKartStorageSceneLevel::Draw()
{
	//if(m_eLevelLoaded == LEVEL_NONE){
	//	return;
	//}

	PreDraw();
	NiRenderer* pkRenderer = m_tpRenderer;
	NiCamera* pkCamera = m_tpCamera;

	NxPlayMgr::RenderAllPlayPortrait(GetMainApp()->GetCuller());

	pkRenderer->SetBackgroundColor(NiColorA(BF_CR, BF_CG, BF_CB,1.0f));
	pkRenderer->BeginUsingDefaultRenderTargetGroup(NiRenderer::CLEAR_ALL);
/*	
	pkRenderer->ClearBuffer(&pkCamera->GetViewPort(),
		NiRenderer::CLEAR_BACKBUFFER | NiRenderer::CLEAR_ZBUFFER);

	// renderer camera port settings
	pkRenderer->SetCameraData(pkCamera->GetWorldTranslate(),
		pkCamera->GetWorldDirection(), pkCamera->GetWorldUpVector(),
		pkCamera->GetWorldRightVector(), pkCamera->GetViewFrustum(),
		pkCamera->GetViewPort());
*/
//	pkRenderer->SetSorter(m_spEffectSorter);

//	m_spEffectSorter->StartAccumulating(pkCamera);

//	CGameEffectMgr::Render(pkCamera);
//	PartialRender(pkCamera, m_spLevelRoot);	

//	NxPlayMgr::DrawAllPlay(pkCamera);
	//	m_pkPlay->Draw(pkCamera);

//	m_spEffectSorter->FinishAccumulating();

	pkRenderer->SetSorter(NULL);

//	ScreenOverlay::Get()->Draw();

	g_spCarBarnBackdrop->Draw(pkRenderer);

/*	static ID3DXSprite * pSprite = NULL;
	if ( !pSprite )
	{
		NiDX9Renderer *pDX9Renderer = NULL;
		pDX9Renderer = NiDynamicCast(NiDX9Renderer, pkRenderer);
		IDirect3DDevice9 * pD3dDevice = pDX9Renderer->GetD3DDevice();
		D3DXCreateSprite( pD3dDevice, &pSprite );
	}
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
*/
	GlobalInterface::m_pGUI->Update(1000);
//	pSprite->End();

	//NiApplication::ms_pkApplication->RenderVisualTrackers();
	pkRenderer->EndUsingRenderTargetGroup();
	EndDraw();
}

//-----------------------------------------------------------------------
void CKartStorageSceneLevel::UnloadLevel()
{
	SAFE_NIDELETE(g_spCarBarnBackdrop);
	RemoveAllColliders();

	// wincoqin屏蔽了下面代码
	// 2007/1/17, 下面代码没有用
	/*
	if (m_pkPlayBack) {
		m_pkPlayBack->Save("Play.txt");
		SAFE_NIDELETE(m_pkPlayBack);
	}*/

	CGameEffectMgr::DestroyEffect();
	//m_sKartCrack.Destroy();
	m_spEffectSorter =NULL;

	// wincoqin屏蔽了下面代码
	// 2007/1/17, 下面代码没有用
	/*
	if (m_pkLinks){
		SAFE_NIDELETE(m_pkLinks);		
	}
	
	CKartStatusManager::Destroy();
	*/

	float fTotalTimeInMins = NiGetCurrentTimeInSec() - m_fAppStartTime;
	fTotalTimeInMins /= 60.0f;

	NIMETRICS_ADDVALUE("Time in Demo (mins)", fTotalTimeInMins);
	SAFE_NIDELETE(m_pPhysXManager);
	m_spStaticRoot = 0;
	m_spMoveNode   = 0;
	m_spSkyNode    = 0;
	m_spLevelRoot  = 0;
	CSceneLevel::UnloadLevel();

}
bool CKartStorageSceneLevel::LoadLevel(UINT SceneID )
{
	if( !CSceneLevel::LoadLevel(SceneID))
		return false;

	m_spEffectSorter			= NiNew NiAlphaAccumulator;


	CGameEffectMgr::InitEffect();


	//-----------------------------------------------------------------------
	// wincoqin 屏蔽了以下代码，因为下面代码没有用
	// 2007/1/17
/*	m_spScene = NiNew NiNode;

	if(!m_spScene)
	{ 
		return false; 
	}
	

	//m_eLevelLoaded = eWhichLevel;

	m_spScene->SetName("SceneRoot");

	char* pcLevelFilename = 0;
	char* pcPickMeshFilename = 0;
	char* pcSkyDomeFilename = 0;
	char* pcShadowMeshFilename = 0;


	pcLevelFilename = "CARSELECT/sc_cheku.nif";
	pcPickMeshFilename = "";
	pcSkyDomeFilename = "";
	pcShadowMeshFilename = "CARSELECT/ARENA-DICED.NIF";


	//Load the main scene file
	NiTObjectArray<NiObjectPtr> kExtraObjects;
	m_spStaticRoot = OpenNifAndPrepack(pcLevelFilename, &kExtraObjects);

	if (!m_spStaticRoot)
	{
		NiOutputDebugString("LevelManager::LoadLevel -- "
			"车库 Creation Failed\n");
		return false;
	}

	//Get Show camera
	NiNodePtr tpNode = OpenNifAndPrepack("CARSELECT/cam.nif",NULL);

	if(tpNode)
	{
		NiNode* pkCamObj = (NiNode*)tpNode->GetObjectByName("Camera01");

		NxSceneMgr::Get()->GetCameraController()->SetCamera((NiCamera*)pkCamObj);

		NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::SELECT_CAR);

		m_spScene->AttachChild(m_spStaticRoot);
		m_spLevelRoot = m_spStaticRoot;

		m_spScene->Update(0.0f);

	}

	//创建角色并且设置位置
	NiPoint3 Pos=m_spScene->GetObjectByName("chewei")->GetTranslate();


	KPlayInfo PlayInfo;
	PlayInfo.Clear();

*/
	GetMainApp()->ShowPointer();

	NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::SELECT_CAR);

	CGameEffectMgr::ResetEffect();

	NiSourceTexturePtr pkTexture = NiSourceTexture::Create(
		NiApplication::ConvertMediaFilename("ScreenOverLay/carbarnmenu.dds"));
	if(!pkTexture)
	{
		NiOutputDebugString("Error:ScreenOverlay::Create()Failed\n");
		return false;
	}
	g_spCarBarnBackdrop = NiNew NiScreenTexture(pkTexture);
	g_spCarBarnBackdrop->AddNewScreenRect(0, 0, 1, 1, 0, 0);

	
	return true;
}
void CSceneLevel::AttachObjectToScene(NiNodePtr pNode)
{
	assert(pNode);
	if ( !pNode )
	{
		return;
	}
	assert(m_spMoveNode);
	float fTime = GetMainApp()->GetAccumTime();
	m_spMoveNode->AttachChild(pNode);
	pNode->UpdateNodeBound();
	NiTimeController::StartAnimations(pNode, fTime);
	pNode->UpdateEffects();
	pNode->UpdateProperties();
	pNode->Update(fTime);
}

void CSceneLevel::DetachObjectToScene(NiNodePtr pNode)
{
	assert(pNode);
	if ( !pNode )
	{
		return;
	}
	assert(m_spMoveNode);
	m_spMoveNode->DetachChild(pNode);
}
void CSceneLevel::SetRaceOver()
{
	m_bRaceOver = TRUE;
}

//-----------------------------------------------------------------------
PhysXManager	*	CSceneLevel::GetPhysXManager(){
	return m_pPhysXManager;
}
bool				CSceneLevel::GetPhysXDebugRender()  {
	return m_pPhysXManager->GetPhysXDebugRender();
}
void				CSceneLevel::SetPhysXDebugRender(const bool bRender) {
	m_pPhysXManager->SetPhysXDebugRender(bRender, m_spLevelRoot);
}