////////////////////////////////////////////////
//     FileName:    roadmap.cpp
//     Author:      Fish
//     Date:        2007-2-28 11:18:25
//     Description: 赛道地图的封装类
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "Utility.h"
#include <NiAnimation.h>
#include <NiApplication.h>
#include <NiMain.h>
#include <NiParticle.h>
#include <NiPhysX.h>
#include <NiMetricsLayer.h>
#include "GameMain.h"
#include "physxtask.h"
#include "roadmap.h"
#include "script.h"
#include "gpscript.h"
#include "Profiling.h"
#include "RMOperator.h"
#include "string"
#include "./PhysKart/NitoNxTools.h"
#include "logtime.h"
#include "TerrainEffectMgr.h"

using namespace std;

#define max(a,b)    (((a) > (b)) ? (a) : (b))
//---------------------------------------------------------------------------
static void DisableZBuffer(NiAVObject* pkObject)
{
	NiProperty* pkProp = pkObject->GetProperty(NiZBufferProperty::GetType());
	if (pkProp)
	{
		((NiZBufferProperty*)pkProp)->SetZBufferTest(false);
		((NiZBufferProperty*)pkProp)->SetZBufferWrite(false);
	}

	if (NiIsKindOf(NiNode, pkObject))
	{
		NiNode* pkNode = (NiNode*)pkObject;
		for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
		{
			NiAVObject* pkChild = pkNode->GetAt(i);
			if (pkChild)
				DisableZBuffer(pkChild);
		}
	}
}
HRESULT	CRoadMap::LoadMap(DWORD MapID, DWORD nLaps)
{
	LOG_TIME("loadmap");
	if ( S_OK != LoadMapConfig(MapID) )
	{
		return ERROR_BAD_ARGUMENTS;
	}

	//Load the main scene file
	NiTObjectArray<NiObjectPtr> kExtraObjects;
	m_spLevelRoot = RMOperator::OpenNifAndPrepack(m_kMapDesc.kSceneFileName.c_str(), &kExtraObjects);
	assert(m_spLevelRoot);
	m_spScene = NiNew NiNode;
	m_spScene->AttachChild(m_spLevelRoot);

	// Prepare scene for UpdateSelected
	bool bUpdate;
	bool bRigid;
	m_spScene->SetSelectiveUpdateFlags(bUpdate, true, bRigid);

	m_spMoveNode = (NiNode*)m_spLevelRoot->GetObjectByName("sc_move");			
	assert(m_spMoveNode);
	m_spAttachNode = m_spMoveNode;
	m_spStaticRoot = NiDynamicCast(NiNode, m_spLevelRoot->GetObjectByName("sc_freeze"));
	assert(m_spStaticRoot);
	TerrainEffectMgr::Create();
	
	NiAVObject* pSky = m_spLevelRoot->GetObjectByName("sky");
	if (pSky)
	{
		m_spSky = NiDynamicCast(NiNode, pSky);
		m_spSky->DetachParent();
		if (m_kMapDesc.nUsingSkyAlphaSorter == 0)
			DisableZBuffer(m_spSky);
		m_spSky->Update(0.0f);
		m_spSky->UpdateProperties();
		m_spSky->UpdateEffects();
		m_spSky->UpdateNodeBound();
	}
	else
	{
		m_spSky = 0;
	}


	//if (!TerrainEffectMgr::Get() );
	//	return S_FALSE;                   

	m_pPhysXManager = NiNew PhysXManager(m_kMapDesc.kPickMapFileName.c_str());
#if defined(_PHYSX_2_7_0)
	LoadStaticColliders(m_kMapDesc.kArrStaticColliders, m_pPhysXManager, m_spStaticRoot);
	LoadKinematicColliders(m_kMapDesc.kArrAnimColliders, m_pPhysXManager, m_spMoveNode);
#else	
	LoadCollidersInfo(m_kMapDesc.kArrStaticColliders, m_pPhysXManager);
	LoadCollidersInfo(m_kMapDesc.kArrAnimColliders, m_pPhysXManager, m_spMoveNode);
	LoadCollidersInfo(m_kMapDesc.kArrActiveColliders, m_pPhysXManager, m_spMoveNode);
#endif

	//载入地图startpoint,checkpoint,道具点数据
	m_kLinks.LoadLinks(m_kMapDesc.kCheckPointFileName.c_str());
	m_kLinks.SetLaps(max(nLaps,1));
	m_kLinks.SetCorrectRunDir(dirClockWise);
	NiTimeController::StartAnimations(m_spScene, GetMainApp()->GetAccumTime());
	

	//CreateFog(1.0f,1.0f,1.0f,0.5f,10.0f);

	m_spScene->UpdateProperties();
	m_spScene->UpdateEffects();
	m_spScene->UpdateNodeBound();
	m_spScene->Update(0.0f);

	m_pPhysXManager->m_spMasterScene->Simulate(0.01f);
	return S_OK;
}

void	CRoadMap::UnLoadMap()
{
	TerrainEffectMgr::Destroy();
	m_kLinks.Clear();
	SAFE_NIDELETE(m_pPhysXManager);
	m_spAttachNode = NULL;
	m_spMoveNode = NULL;
	m_spStaticRoot = NULL;
	m_spLevelRoot = NULL;
	m_spScene = NULL;	
}

HRESULT CRoadMap::LoadMapConfig(DWORD MapID)
{
	char szDir[MAX_PATH] = "";
	_snprintf(szDir, MAX_PATH, "Map/Common Map/Map%0.2d/", MapID);

	char szMapDescFile[MAX_PATH] = "";
	_makepath(szMapDescFile, NULL, szDir, "map_desc.lua", NULL);

	CVarTable kVarTable("map_desc");
	if ( !CVar::LoadVarsFromFile(&kVarTable, NiApplication::ConvertMediaFilename(szMapDescFile)) )
	{
		NiMessageBox(szMapDescFile, "File Wrong or Not Exist!", NULL);
		return E_FAIL;
	}

	_ReadStringFromVar(m_kMapDesc.kMapName, kVarTable.Element("map_name"));
	m_kMapDesc.kMapName = string(szDir) + m_kMapDesc.kMapName;
	_ReadStringFromVar(m_kMapDesc.kCheckPointFileName, kVarTable.Element("map_checkpoint"));
	m_kMapDesc.kCheckPointFileName = string(szDir) + m_kMapDesc.kCheckPointFileName;
	_ReadStringFromVar(m_kMapDesc.kPlaybackCameraFileName, kVarTable.Element("map_camplayback"));
	m_kMapDesc.kPlaybackCameraFileName = string(szDir) + m_kMapDesc.kPlaybackCameraFileName;
	_ReadStringFromVar(m_kMapDesc.kEagleMapFileName, kVarTable.Element("map_eaglemap"));
	m_kMapDesc.kEagleMapFileName = string(szDir) + m_kMapDesc.kEagleMapFileName;
	_ReadStringFromVar(m_kMapDesc.kPickMapFileName, kVarTable.Element("map_pickmap"));
	m_kMapDesc.kPickMapFileName = string(szDir) + m_kMapDesc.kPickMapFileName;
	_ReadStringFromVar(m_kMapDesc.kSceneFileName, kVarTable.Element("map_scene"));
	m_kMapDesc.kSceneFileName = string(szDir) + m_kMapDesc.kSceneFileName;
	_ReadStringFromVar(m_kMapDesc.kIntroCameraName, kVarTable.Element("camera")->Element("intro_camera"));
	m_kMapDesc.kIntroCameraName = string(szDir) + m_kMapDesc.kIntroCameraName;
	_ReadNumFromVar(&m_kMapDesc.dwPropBox, kVarTable.Element("propbox"));
	_ReadStringFromVar(m_kMapDesc.kAwardName, kVarTable.Element("awardname"));	
	_ReadStringFromVar(m_kMapDesc.kTeamAwardName, kVarTable.Element("teamawardname"));
	_ReadStringFromVar(m_kMapDesc.kAmbientSoundName, kVarTable.Element("ambientsoundname"));
	_ReadTransformFromVar(m_kMapDesc.kAwardTransform, kVarTable.Element("awardtransform"));
	_ReadTransformFromVar(m_kMapDesc.kAwardCamTransform, kVarTable.Element("awardcamtran"));
	_ReadMirrorsFromVar(m_kMapDesc.kArrMirrors, kVarTable.Element("mirrors"));
	_ReadAppCullStrategy(m_kMapDesc.kAppCullStrategy, kVarTable.Element("disp_strategy"));
	_ReadCollidersFromVar(szDir, m_kMapDesc.kArrStaticColliders, kVarTable.Element("map_static_colliders"));
	_ReadCollidersFromVar(szDir, m_kMapDesc.kArrAnimColliders, kVarTable.Element("map_anim_colliders"));
	_ReadCollidersFromVar(szDir, m_kMapDesc.kArrActiveColliders, kVarTable.Element("map_active_colliders"));
	_ReadNumFromVar(&m_kMapDesc.nUsingSkyAlphaSorter, kVarTable.Element("usingAlphaSorterToSky"));
	int nEnableCheckPoint = 1;
	m_bEnableCheckPoint = true;
	if (_ReadNumFromVar(&nEnableCheckPoint, kVarTable.Element("enablecheckpoint")))
	{
		m_bEnableCheckPoint = (nEnableCheckPoint != 0);
	}
	_ReadSusPosFromVar(m_kMapDesc.kSusspensionPos, kVarTable.Element("physx_susPos"));
	
	return S_OK;
}

void	CRoadMap::Update(float fTime)
{
	if(m_spMoveNode)
	{
		PROFILE("scene update");		
		m_spMoveNode->Update(fTime);
		//m_spMoveNode->UpdateSelected(fTime);
	}
}

void	CRoadMap::Render(NiCamera* pkCamera)
{
	assert(pkCamera);

	CCustomCullingProcess & kCuller = GetMainApp()->GetCuller();	
	NiRenderer * pkRenderer = NiRenderer::GetRenderer();
	NiVisibleArray &kArray = GetMainApp()->GetVisibleArray();
	if (m_spSky)
	{
		NiAccumulatorPtr spSorter = pkRenderer->GetSorter();
		pkRenderer->SetSorter(NULL);
		NiFrustum frsOri = pkCamera->GetViewFrustum();
		NiFrustum frsSky = frsOri;
		frsSky.m_fFar = 2000.0f;
		pkCamera->SetViewFrustum(frsSky);
		NiCullScene(pkCamera, m_spSky, kCuller, kArray);
		NiDrawVisibleArray(pkCamera, kArray);				
		pkCamera->SetViewFrustum(frsOri);		
		pkRenderer->SetSorter(spSorter);
	}	
	
	NiCullScene(pkCamera, m_spScene, kCuller, kArray);	
	NiDrawVisibleArray(pkCamera, kArray);

	Statistics::AddVisibleArray(&kArray, Statistics::GT_SCENE);
}

void CRoadMap::AttachObject(NiNodePtr pNode)
{
	if ( !pNode )
	{
		assert(0);
		return;
	}
	if ( !m_spAttachNode )
	{
		assert(0);
		return;
	}

	float fTime = GetMainApp()->GetAccumTime();
	m_spAttachNode->AttachChild(pNode);
	pNode->UpdateNodeBound();
	NiTimeController::StartAnimations(pNode, fTime);
	pNode->UpdateEffects();
	pNode->UpdateProperties();
	pNode->Update(fTime);
}

void CRoadMap::DetachObject(NiNodePtr pNode)
{
	if ( !pNode )
	{
		assert(0);
		return;
	}
	if ( !m_spAttachNode )
	{
		assert(0);
		return;
	}
	m_spAttachNode->DetachChild(pNode);
}

void CRoadMap::_ReadCollidersFromVar(LPCSTR pszDir, vector<Map_Desc::Collider>& kArrColliders, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_ARRAY )
	{
		CVarArray* pArray = (CVarArray*)pVar;
		for ( int i = 0; i < pArray->GetNum(); i++ )
		{
			CVar* pVarItem = pArray->Element(i);
			if ( pVarItem && pVarItem->GetType() == VAR_TABLE )
			{
				Map_Desc::Collider kCollider;
				_ReadStringFromVar(kCollider.kFileName, pVarItem->Element("filename"));
				kCollider.kFileName = string(pszDir) + kCollider.kFileName;
				_ReadNumFromVar(&kCollider.nColliderType, pVarItem->Element("type"));
				_ReadNumFromVar(&kCollider.nGroupType, pVarItem->Element("group"));
				kArrColliders.push_back(kCollider);
			}
		}
	}
}

void CRoadMap::_ReadMirrorsFromVar(vector<CRoadMap::Map_Desc::Mirror>& kArrMirrors, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_ARRAY )
	{
		CVarArray* pArray = (CVarArray*)pVar;
		for ( int i = 0; i < pArray->GetNum(); i++ )
		{
			CVar* pVarItem = pArray->Element(i);
			if ( pVarItem && pVarItem->GetType() == VAR_TABLE )
			{
				Map_Desc::Mirror kMirror;
				_ReadStringFromVar(kMirror.kMirrorName, pVarItem->Element("mirror_name"));
				_ReadStringFromVar(kMirror.kNodeName, pVarItem->Element("node_name"));
				_ReadStringFromVar(kMirror.kTriShapeName, pVarItem->Element("trishape_name"));
				_ReadPointFromVar(kMirror.kCenterPos, pVarItem->Element("centpoint"));
				_ReadPointFromVar(kMirror.kRight, pVarItem->Element("right_dir"));
				_ReadPointFromVar(kMirror.kUp, pVarItem->Element("up_dir"));
			}
		}
	}
}

void CRoadMap::_ReadTransformFromVar(CRoadMap::Map_Desc::Transform& kTransform, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_TABLE )
	{
		_ReadPointFromVar(kTransform.kTrans, pVar->Element("pos"));
		_ReadPointFromVar(kTransform.kRot, pVar->Element("rotate"));
	}
}

void CRoadMap::_ReadPointFromVar(NiPoint3& kPoint, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_ARRAY )
	{
		CVarArray* pArray = (CVarArray*)pVar;
		if ( pArray->GetNum() >= 3 )
		{
			_ReadNumFromVar(&kPoint.x, pArray->Element(0));
			_ReadNumFromVar(&kPoint.y, pArray->Element(1));
			_ReadNumFromVar(&kPoint.z, pArray->Element(2));
		}
	}
}

void CRoadMap::_ReadAppCullStrategy(CRoadMap::Map_Desc::AppCullStrategy& kStrategy, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_TABLE )
	{
		_ReadNumFromVar(&kStrategy.fAllVisibleDistance, pVar->Element("allvisibledist"));
		_ReadNumFromVar(&kStrategy.fMinAngle, pVar->Element("minvisibleangle"));
		_ReadNumFromVar(&kStrategy.fMaxProcessBoundRadius, pVar->Element("maxprocessradius"));
	}
}

void CRoadMap::LoadCollidersInfo(vector<Map_Desc::Collider> & kArrColliders, PhysXManager *& pPhysXMgr, NiNode * pkFatherNode)
{
	assert(pPhysXMgr);
	unsigned int uiColliderNum = kArrColliders.size();

	for (unsigned int i = 0; i < uiColliderNum; ++i)
	{
		Map_Desc::Collider*  pkCollider = &kArrColliders[i];
		XUserData udObj;
		udObj._actorType = (XUserData::ACTORTYPE)pkCollider->nColliderType;

		pPhysXMgr->LoadPhysObject(pkCollider->kFileName.c_str(), pkFatherNode, (PhysXManager::COLLISIONGROUP)pkCollider->nGroupType, udObj);
	}
}

CLinks*	CRoadMap::GetLinks()
{
	return &m_kLinks;
}

PhysXManager*	CRoadMap::GetPhysXManager()
{
	return m_pPhysXManager;
}

bool	CRoadMap::GetPhysXDebugRender()  
{
	return m_pPhysXManager->GetPhysXDebugRender();
}

void	CRoadMap::SetPhysXDebugRender(const bool bRender) 
{
	m_pPhysXManager->SetPhysXDebugRender(bRender, m_spScene);
}

void CRoadMap::LoadStaticColliders(vector<Map_Desc::Collider> & kArrColliders, PhysXManager *& pPhysXMgr, NiNode * pkFatherNode)
{
	assert(pPhysXMgr);
	unsigned int uiColliderNum = kArrColliders.size();

	for (unsigned int i = 0; i < uiColliderNum; ++i)
	{
		Map_Desc::Collider*  pkCollider = &kArrColliders[i];
		XUserData udObj;
		udObj._actorType = (XUserData::ACTORTYPE)pkCollider->nColliderType;

		//pPhysXMgr->LoadPhysObject(pkCollider->kFileName.c_str(), pkFatherNode, (PhysXManager::COLLISIONGROUP)pkCollider->nGroupType, udObj);

		int pos = pkCollider->kFileName.rfind("/");
		string strTmp = pkCollider->kFileName.insert(pos+1, "phs27_");
		NiToNxTools::LoadStaticColliders(strTmp.c_str(), pPhysXMgr->GetMasterPhyScene(), 
			pkFatherNode,pPhysXMgr,(PhysXManager::COLLISIONGROUP)pkCollider->nGroupType, udObj);
	}
}

void CRoadMap::LoadKinematicColliders(vector<Map_Desc::Collider> & kArrColliders, PhysXManager *& pPhysXMgr, NiNode * pkFatherNode)
{
	assert(pPhysXMgr);
	unsigned int uiColliderNum = kArrColliders.size();

	for (unsigned int i = 0; i < uiColliderNum; ++i)
	{
		Map_Desc::Collider*  pkCollider = &kArrColliders[i];
		XUserData udObj;
		udObj._actorType = (XUserData::ACTORTYPE)pkCollider->nColliderType;

		char filename[256];
		strncpy(filename, "phs27_", 256);
		strncat(filename, pkCollider->kFileName.c_str(), 256 - strlen(pkCollider->kFileName.c_str()) - 1);

		NiToNxTools::LoadKinematicColliders(filename, pPhysXMgr->GetMasterPhyScene(), 
			m_spMoveNode,pPhysXMgr,(PhysXManager::COLLISIONGROUP)pkCollider->nGroupType, udObj);
	}
}

void  CRoadMap::CreateFog(const float r,const float g,const float b,const float depth, const float ffar)
{
	if (m_spScene)
	{	
		float kfar=ffar;
		float kdepth=depth;

		float nearPlane = 0.0f;
		float farPlane	= 130.f;

		kdepth = max( 0.1f, kdepth);
		kfar  = min( kfar, farPlane);
		kfar  = max( kfar, nearPlane);

		float len =	( kfar - nearPlane) * kdepth + (farPlane-kfar);
		float rdep = len / ( farPlane-nearPlane);
		float fogMax = len / (( kfar-nearPlane)*kdepth);


		NiFogProperty* pFog = NiNew NiFogProperty;

		NiColor fogColor(r,g,b);
		pFog->SetFogColor(fogColor);
		pFog->SetDepth(0.97f);
		pFog->SetFog(true);
		pFog->SetFogFunction( NiFogProperty::FOG_RANGE_SQ);//FOG_Z_LINEAR);
		NiRenderer::GetRenderer()->SetMaxFogValue(50.0f);

		m_spScene->AttachProperty(pFog);
		m_spScene->UpdateProperties();
	}
}

void CRoadMap::_ReadSusPosFromVar(NiPoint3& kSusPos, CVar* pVar)
{
	if ( pVar && pVar->GetType() == VAR_TABLE )
	{
		_ReadPointFromVar(kSusPos, pVar->Element("pos"));
	}
}