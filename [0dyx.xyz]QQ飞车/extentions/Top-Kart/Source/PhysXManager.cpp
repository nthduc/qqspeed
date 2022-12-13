#include "Top-KartPCH.h"
#include ".\physxmanager.h"
#include "TerrainManager.h"
#include "Utility.h"
#include "./PhysKart/NxAllVehicles.h"
//#include "FenceManager.h"
#include "RMOperator.h"
#include "GPPoint.h"

TriggerCallback g_myTriggerCallback;

class MyContactReport : public NxUserContactReport
{
public:
	virtual void  onContactNotify(NxContactPair& pair, NxU32 events)
	{
		NxI32 carIndex = -1;
		NxPlayMgr::OnContactNotify(pair, events);
	}
} g_carContactReportObj;

ControllerManager PhysXManager::m_NxControllerMgr;

PhysXManager::PhysXManager(const char* filename)
{

	m_pPhysicsSDK = 0;
	m_pScene = 0;
	//	增加人:	wincoqin
	//  时间:	2007/02/01
	//  目的:	加个初始化	
	m_pGamePropReport = CGPPointMgr::GetReportTrigger();
	// 结束
#if defined (_PHYSX_2_7_0)
	m_kDefaultGravity = NxVec3(0.0f, 0.f, -4.8);
#else
	m_kDefaultGravity = NxVec3(0.0f, 0.f, GRAVITYACCEL);
#endif
	Initialize();

	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(filename))) 
	{
		assert(0);
	}

	NiNode * pkNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
	assert(pkNode);
	pkNode->Update(0.0f);
    
	NiAVObject * pkGroundNode = ((NiNode*)pkNode->GetObjectByName("ground"))->GetAt(0);//GetObjectByName("Editable Poly");

	m_pTerrainMgr = NiNew LookupTerrainManager(pkGroundNode, NULL, m_pScene);
	if (!m_pTerrainMgr)
	{
		NiOutputDebugString("LevelManager::LoadLevel -- "
			"TerrainManager Creation Failed\n");
		return;
	}

	NiAVObject * pkFenceNode = ((NiNode*)pkNode->GetObjectByName("hulan"))->GetAt(0);//GetObjectByName("Editable Poly");

	m_pFenceMgr = new CFenceManager(pkFenceNode, m_pScene, m_spMasterScene);

	if (!m_pFenceMgr)
	{
		NiOutputDebugString("FenceManager Creation Failed\n");
		return;
	}


//	NxPlayMgr::GetHostPlay()->SetKart(m_pKart);

	//m_pKart = NxPlayMgr::GetHostPlay()->GetKart();

	m_pScene->setActorGroupPairFlags(PhysXManager::CG_VEHICLE, PhysXManager::CG_ENVIRONMENT,
		NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH |NX_NOTIFY_ON_END_TOUCH);

	m_spMasterScene->SetFetchTime(0.0f);
	m_spMasterScene->SetDoFixedStep(true);
	m_spMasterScene->SetTimestep( 1.0f / 60.0f);

}

PhysXManager::~PhysXManager()
{
	if (Scheduler::Get())
		Fetch(Scheduler::Get()->GetClock().GetTime());
	CPhysXTask::Get()->Process();
	CPhysXTask::Destroy();

    SetGamePropCollisionReport(NULL);
	m_NxControllerMgr.purgeControllers();
	for (unsigned int i = 0; i < m_vSlaves.size(); ++i)
	{
		m_vSlaves[i] = NULL;
	}
	m_vSlaves.clear();
	SAFE_NIDELETE(m_pTerrainMgr);
	SAFE_NIDELETE(m_pFenceMgr);
	//SAFE_DELETE(m_pKart);
	m_spMasterScene->ReleaseScene();
	m_spMasterScene = 0;	
}


bool PhysXManager::Fetch(float time)
{

	bool bSimEnd = m_spMasterScene->FetchResults(time,false);

	return bSimEnd;

}
void PhysXManager::ProcessInput()
{
	//if (NxPlayMgr::GetHostPlay())
	//	NxPlayMgr::GetHostPlay()->GetKart()->Control();
}

void PhysXManager::Sim(float time)
{
	m_NxControllerMgr.updateControllers();
	m_spMasterScene->Simulate(time);
}

bool PhysXManager::Initialize()
{
	// Initialize PhysicsSDK
	//m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, 0, &m_kErrorStream);

	NiPhysXManager* pkPhysXManager = NiPhysXManager::GetPhysXManager();
	m_pPhysicsSDK = pkPhysXManager->m_pkPhysXSDK;


	if(!m_pPhysicsSDK)	return false;

#if defined(_PHYSX_2_7_0)
	//m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect ("primowang-pc3", 5425);
#endif

 

	NxInitCooking(NULL, &m_kErrorStream);

	//gPhysicsSDK->setParameter(NX_MIN_SEPARATION_FOR_PENALTY, -0.05f);
	//enable visualisation
	m_pPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 2);
	m_pPhysicsSDK->setParameter(NX_CONTINUOUS_CD, 1);

	m_pPhysicsSDK->setParameter(NX_VISUALIZE_BODY_AXES, 1.0f);
	m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.000001f);


	// Set the debug visualization parameters
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);


	m_pPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_AXES, 1.0f);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1.0f);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1.0f);

	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LOCAL_AXES, 1.0f);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_WORLD_AXES, 1.0f);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, 1.0f);

#ifndef _PHYSX_2_7_0
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_ERROR, 1.0f);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_FORCE, 1.0f);
	m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_REDUCED, 1.0f);
	m_pPhysicsSDK->setParameter(NX_MIN_SEPARATION_FOR_PENALTY, -0.01);
#endif

	// Don't slow down jointed objects
	//m_pPhysicsSDK->setParameter(NX_ADAPTIVE_FORCE, 0.0f);

	//create some materials -- note that we reuse the same NxMaterial every time,
	//as it gets copied to the SDK with every setMaterial() call, not just referenced.


	m_spMasterScene = NiNew NiPhysXScene();


	// Create a scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity				= m_kDefaultGravity;

	sceneDesc.userContactReport = &g_carContactReportObj;

	//	sceneDesc.userContactReport		= carContactReport;
	m_pScene = m_pPhysicsSDK->createScene(sceneDesc);

	assert(m_pScene);


	m_spMasterScene->SetPhysXScene(m_pScene);

	m_spMasterScene->SetUpdateSrc(true);
	m_spMasterScene->SetUpdateDest(true);
	

	//设置道具的碰撞回调
	m_pScene->setUserTriggerReport(&g_myTriggerCallback);

	//default material
	NxMaterialIndex	defaultMaterialIndex = 0;
	NxMaterial * defaultMaterial = m_pScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.1f);
	defaultMaterial->setStaticFriction(0.8f);
	defaultMaterial->setDynamicFriction(0.8f);

	CPhysXTask::Create();
	return true;
}

void PhysXManager::AddToSlaveList(NiPhysXScene* pScene)
{
	m_vSlaves.push_back(pScene);
}
void PhysXManager::UpdateDestinations(float fTime)
{
	std::vector<NiPhysXScene*>::iterator itr;
	for (itr = m_vSlaves.begin(); itr != m_vSlaves.end(); itr++)
	{
		(*itr)->UpdateDestinations(fTime);
	}
}

void PhysXManager::UpdateSources(float fTime)
{
	std::vector<NiPhysXScene*>::iterator itr;
	for (itr = m_vSlaves.begin(); itr != m_vSlaves.end(); itr++)
	{
		(*itr)->UpdateSources(fTime);
	}
	m_spMasterScene->UpdateSources(fTime, true);
}


void PhysXManager::SetPhysXDebugRender(const bool bRender, NiNode* pSceneRoot)
{
	NiPhysXManager* pkPhysXManager = NiPhysXManager::GetPhysXManager();
	pkPhysXManager->WaitSDKLock(); // Not strictly necessary - no other thread

	if (bRender)
	{     
		pkPhysXManager->m_pkPhysXSDK->setParameter(
			NX_VISUALIZATION_SCALE, 1.0f);
		pkPhysXManager->m_pkPhysXSDK->setParameter(
			NX_VISUALIZE_COLLISION_SHAPES, 1.0f); 
		pkPhysXManager->m_pkPhysXSDK->setParameter(
		    NX_VISUALIZE_BODY_MASS_AXES, 1.0f);
		m_spMasterScene->SetDebugRender(true, pSceneRoot);
	}
	else
	{
		pkPhysXManager->m_pkPhysXSDK->setParameter(
			NX_VISUALIZATION_SCALE, 0.0f);
		m_spMasterScene->SetDebugRender(false);
	}

	pkPhysXManager->ReleaseSDKLock();
}


NiPoint3 PhysXManager::GetTerrainNormal()
{
	return m_kNormalCarHitTerrain;
}
bool PhysXManager::GetPhysXDebugRender()
{
	return m_spMasterScene->GetDebugRender();
}
void PhysXManager::LoadPhysObject(const char* filename, NiNode* pParent, COLLISIONGROUP eGroup, const XUserData& userData)
{
	NiTObjectArray<NiObjectPtr> kExtraObjects;
	NiNodePtr spStaticRoot = RMOperator::OpenNifAndPrepack(filename, &kExtraObjects);
	spStaticRoot->Update(0.0f);

	if (pParent)
		pParent->AttachChild(spStaticRoot);//加入场景中
	
    
	NiPhysXScenePtr spScene = 0;
	NiPhysXScene* pkGlobalScene = m_spMasterScene;

	for (unsigned int ui = 0; ui < kExtraObjects.GetSize(); ui++)
	{
		NiObject* pkObj = kExtraObjects.GetAt(ui);
		if (NiIsKindOf(NiPhysXScene, pkObj))
		{
			spScene = (NiPhysXScene*)pkObj;

			// Create the scene
			//NiFixedString str = spScene->GetName();

			spScene->SetSlaved(pkGlobalScene, NiPhysXTypes::NXMAT34_ID);
			spScene->CreateSceneFromSnapshot(0);

			spScene->SetUpdateDest(true);
			spScene->SetUpdateSrc(true);

			//set group
			NxActor** ppActors = spScene->GetPhysXScene()->getActors();
			for (unsigned int i = 0; i < spScene->GetPhysXScene()->getNbActors(); i++)
			{
				if (ppActors[i]->userData != NULL)
					continue;
				XUserData* pUserData = new XUserData;
				*pUserData = userData;
				ppActors[i]->setGroup(eGroup);
				ppActors[i]->userData = pUserData;
			}
			//~set group

			AddToSlaveList(spScene);

			break;
		}
	}
	
}

HACTOR PhysXManager::CreateGameProp(const NiPoint3& position, float fRadius, LPVOID lpUserData)
{
	NxActorDesc actorDesc;
	//这里new出来，CPhysXTask会处理释放的
	NxSphereShapeDesc *sphereDesc = new NxSphereShapeDesc;   
	sphereDesc->radius = fRadius;
	sphereDesc->shapeFlags |= NX_TRIGGER_ENABLE; //加入这个标志会导致进入独立设置的trigger
	actorDesc.name="gameproperty";
	actorDesc.shapes.pushBack(sphereDesc);
	NxVec3 pos(position.x, position.y, position.z);
	actorDesc.globalPose.t = pos;
	actorDesc.body = NULL;
	XUserData* pData = new XUserData;
	pData->_actorType = XUserData::AT_GAMEPROP;
	pData->userData = lpUserData;
	actorDesc.userData=pData;

	//by Fish
	actorDesc.group = PhysXManager::CG_ENVIRONMENT;
	return CPhysXTask::Get()->createActor(m_pScene, actorDesc);
	//by Fish End
	/*
	AUTOLOCK_PHYSXSDK();
	NxActor* pActor = m_pScene->createActor(actorDesc);
	if (pActor)
	{
		//XUserData* pData = new XUserData;
		//pData->_actorType = XUserData::AT_GAMEPROP;
		//pData->userData = lpUserData;
		//pActor->userData = pData;
		pActor->setGroup(PhysXManager::CG_ENVIRONMENT);
	}

	return pActor;
	*/
}

void TriggerCallback::onTrigger(NxShape& triggerShape, NxShape& otherShape,  NxTriggerFlag status)   
{       
	if(status & NX_TRIGGER_ON_ENTER
		|| status & NX_TRIGGER_ON_STAY )    
	{      
		// A body entered the trigger area for the first time
		XUserData* pUd = (XUserData*)triggerShape.getActor().userData;
		XUserData* pUdOther = (XUserData*)otherShape.getActor().userData;
		if (pUd)
		{
			//如果车子撞到了一个道具Trigger
			if (XUserData::AT_GAMEPROP==pUd->_actorType  &&
				XUserData::AT_WHEELLESSCAR==pUdOther->_actorType)
			{
				CGamePropCollisionReport* pReport = 
					CLevelManager::Get()->GetPhysXManager()->GetGamePropCollisionReport();
				if (pReport)
				{
					pReport->OnTrigger(pUd->userData,pUdOther->userData);
				}
			}
		}
	}

	if(status & NX_TRIGGER_ON_LEAVE)  
	{                
		// A body left the trigger area 
	}
}
void PhysXManager::AddKinematicPair(NiAVObject* pAV, NxActor* pActor)
{
	NiAndNxPair* pPair = new NiAndNxPair;
	pPair->pActor = pActor;
	pPair->pAV = pAV;

	m_vKinematics.push_back(pPair);
}

void PhysXManager::UpdateKinematics()
{
	std::vector<NiAndNxPair*>::iterator itr;

	for (itr = m_vKinematics.begin(); itr < m_vKinematics.end(); ++itr)
	{
		NiAndNxPair* pPair = (*itr);
	
		NiTransform niTrans = pPair->pAV->GetLocalTransform();
		NxMat34 nxTrans;
		NiPhysXTypes::NiTransformToNxMat34(niTrans.m_Rotate, niTrans.m_Translate, nxTrans);
		//pPair->pActor->moveGlobalPosition(nxTrans.t);
		pPair->pActor->moveGlobalPose(nxTrans);

	}
}
void PhysXManager::AddDynamicPair(NiAVObject* pAV, NxActor* pActor)
{
	NiAndNxPair* pPair = new NiAndNxPair;
	pPair->pActor = pActor;
	pPair->pAV = pAV;

	m_vDynamics.push_back(pPair);

}
void PhysXManager::UpdateDynamics()
{
	std::vector<NiAndNxPair*>::iterator itr;

	for (itr = m_vDynamics.begin(); itr < m_vDynamics.end(); ++itr)
	{
		NiAndNxPair* pPair = (*itr);

		NxMat34 nxTrans = pPair->pActor->getGlobalPose();
		NiTransform niTrans;
		NiPhysXTypes::NxMat34ToNiTransform(nxTrans, niTrans.m_Rotate, niTrans.m_Translate);
		//pPair->pActor->moveGlobalPosition(nxTrans.t);
		pPair->pAV->SetRotate(niTrans.m_Rotate);
		pPair->pAV->SetTranslate(niTrans.m_Translate);
		//pPair->pAV->Update(0);

	}
}

