#include "..\Top-KartPCH.h"
#include "XCart.h"
#include "NxAllVehicles.h"
#include "../InputManager.h"
#include "./Stream.h"
#include "NxCooking.h"
#include "../Utility.h"
#include "../DescConstant.h"
#include "../PhysXManager.h"
#include "../LevelManager.h"

void createCart(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, bool oldStyle);
void createCarWithDesc(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, bool corvette, bool monsterTruck, bool oldStyle, NxPhysicsSDK* physicsSDK);

extern NxScene* gScene;

XCart::XCart():
m_pPhsVehicle(NULL)	
{
	m_spCarRoot = 0;
	m_fAngleAccum = 0.0f;

}

XCart::~XCart()
{
	if(m_pPhsVehicle)
	{
		delete m_pPhsVehicle;
	}
}

void XCart::Control(float dt)
{
	bool left = (InputManager::keys[NiInputKeyboard::KEY_A] == true);
	bool right = (InputManager::keys[NiInputKeyboard::KEY_D] == true);
	bool forward = (InputManager::keys[NiInputKeyboard::KEY_W] == true);
	bool backward = (InputManager::keys[NiInputKeyboard::KEY_S] == true);

	bool drift = (InputManager::keys[NiInputKeyboard::KEY_LSHIFT] == true);

	NxReal steering = 0;
	if (left && !right) 
	{
		steering = -1;

	}
	else if	(right && !left) 
		steering = 1;
	NxReal acceleration = 0;
	if (forward && !backward) 
		acceleration = 1;
	else if (backward && !forward) 
		acceleration = -1;
	bool handbrake = false;

	if(!m_pPhsVehicle)
	{ 
		assert(0);
		return;
	}
	m_pPhsVehicle->control(steering, false, acceleration, false, handbrake);
}

void XCart::Update(float fDeltaTime)
{
}

void XCart::SetKartRotate(const NiMatrix3& rotate)
{
}

void XCart::SetKartPose(const NxMat34& kPose)
{
	NxVec3 pos(kPose.t.x, kPose.t.y, kPose.t.z);
	m_pPhsVehicle->getActor()->setGlobalPosition(pos);
}

void XCart::SynchronizeModel(NiAVObject* pKartModel)
{
	NxVec3 pos = m_pPhsVehicle->getActor()->getGlobalPosition();
	NxMat33 ori = m_pPhsVehicle->getActor()->getGlobalOrientation();
	pKartModel->SetTranslate(pos.x, pos.y, pos.z);
	pKartModel->SetRotate(0, 1, 0, 0);
}	

void XCart::SetKartPos(const NiPoint3& pos)
{
	NxVec3 vecPos(pos.x, pos.y, pos.z);
	m_pPhsVehicle->getActor()->setGlobalPosition(vecPos);
}

NxVec3 XCart::GetPos() 
{ 
	return m_pPhsVehicle->getActor()->getGlobalPosition(); 
}

void XCart::SetPos(NxVec3 pos) 
{ 
	return m_pPhsVehicle->getActor()->setGlobalPosition(pos); 
}

const NiPoint3 XCart::GetKartTranslate()
{
	NxVec3 vecPos = m_pPhsVehicle->getActor()->getGlobalPosition();
	NiPoint3 ptPos(vecPos.y, vecPos.y, vecPos.z);
	return ptPos;
}

const NiMatrix3 XCart::GetKartRotate()
{
	NiMatrix3 mat;
	return mat;
}

const NxMat34 & XCart::GetKartPose()
{
	NxMat34 mat;
	return mat;
}



bool XCart::InitKartParam(const NxVec3& pos,bool frontWheelDrive, bool backWheelDrive, 
						  bool corvette, bool bMonsterTrunk, bool bOldStyle, NxScene* gScene)
{
	return true;
}

void XCart::createCarWithDesc2(const NiAVObject* pNif,const NxVec3& pos, bool frontWheelDrive, 
							   bool backWheelDrive, bool corvette, bool monsterTruck, 
							   bool oldStyle, NxScene* Scene,int KarType)
{
	if ( gScene == NULL )
	{
		gScene = Scene; //NxSceneMgr::GetActiveSceneLevel()->GetPhysXManager()->GetPhyScene();
	}
	createCart(pos, false, true, false);
	NxAllVehicles::selectNext();
	NxAllVehicles::selectNext();
	m_pPhsVehicle = NxAllVehicles::getActiveVehicle();
}
