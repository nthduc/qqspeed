#pragma once

#include "./PhysKart/ErrorStream.h"
#include "NxCooking.h"


//class FenceManager;
#include "FenceManager.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "NiPhysXScene.h"
#include "ControllerManager.h"
#include <NxUserContactReport.h>
#include "GamePropCollisionReport.h"
#include "physxtask.h"
#include "levelmanager.h"

class CPhysXSDKAutoLock
{
public:
	CPhysXSDKAutoLock()
	{
		m_pPhysXManager = NiPhysXManager::GetPhysXManager();
		assert(m_pPhysXManager);
		m_pPhysXManager->WaitSDKLock(); 
	}
	~CPhysXSDKAutoLock()
	{
		assert(m_pPhysXManager);
		m_pPhysXManager->ReleaseSDKLock();
	}
private:
	NiPhysXManager*	m_pPhysXManager;
};

#define AUTOLOCK_PHYSXSDK()	CPhysXSDKAutoLock kLock

class CKart;

class WheellessCar;

class XUserData
{
public:
	XUserData(){_actorType= AT_NODEFINE;};
	~XUserData(){};

	enum ACTORTYPE{
		AT_TERRAIN			= 1 << 0,			//地面
		AT_FENCE_MANUAL		= 1 << 1,		//护栏等静态的物体,代码生成的mesh
		AT_FENCE_GAMEBRYO	= 1 << 2,      //护栏等静态的物体，Gamebryo插件导出的
		AT_WHEELLESSCAR		= 1 << 3,		//赛车
		AT_PUSHABLE			= 1 << 4,			//可以推动的物体，盒子，桶...
		AT_GAMEPROP			= 1 << 5,			//可碰撞的道具
		AT_NONRESPONSE		= 1 << 6,
		AT_NODEFINE			= 0//
	};

	int _actorType;
	void * userData;			//用来保存用户自定义数据，如果是赛车，则保存controller指针
};

class PhysXManager
	: public NiMemObject
{
public:
	PhysXManager(const char* filename);
	~PhysXManager();

	enum COLLISIONGROUP{
		CG_VEHICLE,
		CG_ENVIRONMENT,

		CG_NOCOLLISIONREPORT
	};
	struct NiAndNxPair {
		NxActor* pActor;
		NiAVObject* pAV;
	};
	bool Initialize();

	NxScene * GetPhyScene()	 { return m_pScene; }

	NiPhysXScenePtr  GetMasterPhyScene() { return m_spMasterScene; }

	bool Fetch(float time);

	void ProcessInput();

	void Sim(float time);
	void SetPhysXDebugRender(const bool bRender, NiNode* pSceneRoot);

	void AddToSlaveList(NiPhysXScene* pScene);
	void UpdateDestinations(float fTime);
	void UpdateSources(float fTime);
	bool GetPhysXDebugRender();

	void LoadPhysObject(const char* filename, NiNode* pParent, COLLISIONGROUP eGroup, const XUserData& userData);

	//增加一个道具
	HACTOR CreateGameProp(const NiPoint3& position, float fRadius, LPVOID lpUserData);
	void SetGamePropCollisionReport(CGamePropCollisionReport* pReport){m_pGamePropReport = pReport;}
	CGamePropCollisionReport* GetGamePropCollisionReport(){return m_pGamePropReport;}


	std::vector<NiPhysXScene*> m_vSlaves;
	std::vector<NiAndNxPair*> m_vKinematics;
	void AddKinematicPair(NiAVObject* pAV, NxActor* pActor);
	void UpdateKinematics();

	std::vector<NiAndNxPair*> m_vDynamics;
	void AddDynamicPair(NiAVObject* pAV, NxActor* pActor);
	void UpdateDynamics();

	NxPhysicsSDK*		m_pPhysicsSDK;
	NxScene*			m_pScene ;
	NxVec3				m_kDefaultGravity;
	ErrorStream			m_kErrorStream;

	NiPhysXScenePtr m_spMasterScene;

	static ControllerManager m_NxControllerMgr;
	static ControllerManager& GetControllerMgr(){return m_NxControllerMgr;}



	NiPoint3 m_kNormalCarHitTerrain;
	NiPoint3 GetTerrainNormal();


	TerrainManager* m_pTerrainMgr;
	CFenceManager* m_pFenceMgr;
	CKart* m_pKart;
private:
	CGamePropCollisionReport* m_pGamePropReport;
};
/*
class myRaycastReport : public NxUserRaycastReport
{
	virtual bool onHit(const NxRaycastHit& hit)
	{
		if (hit.shape->getActor().userData)
		{	
			XUserData* pUd = (XUserData*)hit.shape->getActor().userData;
			if (pUd->_actorType == XUserData::AT_TERRAIN)
			{
				//NxSceneMgr::Get()->GetActiveSceneLevel()->GetPhysXManager()->m_kNormalCarHitTerrain.x = hit.worldNormal.x;
				//NxSceneMgr::Get()->GetActiveSceneLevel()->GetPhysXManager()->m_kNormalCarHitTerrain.y = hit.worldNormal.y;
				//NxSceneMgr::Get()->GetActiveSceneLevel()->GetPhysXManager()->m_kNormalCarHitTerrain.z = hit.worldNormal.z;
				
				CLevelManager::Get()->GetPhysXManager()->m_kNormalCarHitTerrain.x = hit.worldNormal.x;
				CLevelManager::Get()->GetPhysXManager()->m_kNormalCarHitTerrain.y = hit.worldNormal.y;
				CLevelManager::Get()->GetPhysXManager()->m_kNormalCarHitTerrain.z = hit.worldNormal.z;

				return false;
			}
			else
				return true;

		}
		return false;
	}
};
*/
class TriggerCallback : public NxUserTriggerReport    
{
public:
	void onTrigger(NxShape& triggerShape, NxShape& otherShape,  NxTriggerFlag status);
};


