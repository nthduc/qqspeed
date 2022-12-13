#ifndef KART_H
#define KART_H

#include "Vehicle.h"
#include "NxVehicle.h"
#include "NiAVObject.h"
#include "../PhysXManager.h"

#include "TencentCar.h"

class Play;
class CKart //:public CVehicle
{
public:
	CKart();
	~CKart();

public:


	// 未使用接口函数
	/*
	//Inherited
	NiNodePtr GetNiRoot();

	bool BuildCarFromFile(const char* nifname);
	bool BuildCarWithDesc();
	//PhysX
	void SetPhsModel(NxVehicle* pNxCar);
	NxVehicle* GetPhsModel();

	void CreateSimpleCart(const NiAVObject* pNif, const NxVec3& pos, 
	bool frontWheelDrive, bool backWheelDrive, bool oldStyle, NxScene* gScene);

	void createCarWithDesc(const NiAVObject* pNif,const NxVec3& pos, bool frontWheelDrive, 
	bool backWheelDrive, bool corvette, bool monsterTruck, 
	bool oldStyle, NxScene* gScene);
	*/
	void SetKartRotate(const NiMatrix3& rotate);
	void SetKartPos(const NiPoint3& pos);
	const NiPoint3  GetKartTranslate();// add by winco, for get the pose of the kart 
	const NiMatrix3 GetKartRotate();// add by winco, for get the pose of the kart 
	const NxMat34  & GetKartPose();
	void SetKartPose(const NxMat34& kPose);
	void SetGameFinish()
	{
		if (m_pPhsVehicle) 
		{
			m_bMovable = false;
			m_bGameOver = true;
			m_pPhsVehicle->SetRollFrictionForce(2.0f);
		}
	}

	bool IsKartMoveable()
	{
		return m_bMovable;
	}

	bool IsGameFinish()
	{
		return m_bGameOver;
	}

	float GetVelocity() 
	{ 
		if (!m_pPhsVehicle)
			return 0;

//		if (CTencentCar::ECS_NONECONTACT != m_pPhsVehicle->GetContactStatus()) 
		if (m_pPhsVehicle->GetWheel(0)->GetContactStatus() == CCarWheel::ENS_NEAR ||
			m_pPhsVehicle->GetWheel(1)->GetContactStatus() == CCarWheel::ENS_NEAR ||
			m_pPhsVehicle->GetWheel(2)->GetContactStatus() == CCarWheel::ENS_NEAR ||
			m_pPhsVehicle->GetWheel(3)->GetContactStatus() == CCarWheel::ENS_NEAR ||
			m_pPhsVehicle->GetWheel(0)->GetContactStatus() == CCarWheel::ENS_FAR ||
			m_pPhsVehicle->GetWheel(1)->GetContactStatus() == CCarWheel::ENS_FAR ||
			m_pPhsVehicle->GetWheel(2)->GetContactStatus() == CCarWheel::ENS_FAR ||
			m_pPhsVehicle->GetWheel(3)->GetContactStatus() == CCarWheel::ENS_FAR)
		{
			NxVec3 kVecLocal = m_pPhsVehicle->GetVecLocal();
			kVecLocal.z = 0;

			NxVec3 kVecGlobal;
			m_pPhsVehicle->LocalToGlobal(kVecLocal, kVecGlobal);

			return kVecGlobal.magnitude();
		}
		else
			return m_pPhsVehicle->GetVecGlobal().magnitude();
	}

	void SetMovable(bool val) 
	{
		m_bMovable = val;
		if( !m_pPhsVehicle )
		{
			assert(0);
			return;
		}
		
		if ( !val)
		{
			m_pPhsVehicle->LockMotion( CTencentCar::ELM_MOVE_XAXIS | CTencentCar::ELM_MOVE_YAXIS);
		}
		else
		{
			m_pPhsVehicle->LockMotion( CTencentCar::ELM_LOCKNONE );
		}
	}

	bool IsAccumulate()
	{
		if ( m_pPhsVehicle )
		{
			return m_pPhsVehicle->IsAccumulate();
		}
		return false;
	}

	bool IsDrifting()
	{ 
		if ( m_pPhsVehicle ) 
		{
			return m_pPhsVehicle->IsDrifting();
		}
		else
		{
			return false; 
		}
	}


	void setVelocityLocal(NxVec3& vel)
	{ 
		if ( !m_pPhsVehicle)
		{
			assert(0);
			return;
		}
		else
		{
			m_pPhsVehicle->SetVecLocal(vel);
		}
	}

	void SetTurnAngle(NxReal val) 
	{
		if ( !m_pPhsVehicle )
		{
			assert(0);
			return;
		}
		else
		{
			return m_pPhsVehicle->SetTurnAngle(val);}
		}


	NxVec3 GetTerrainNormal() 
	{ 
		if ( !m_pPhsVehicle )
		{
			assert(0);
			return NxVec3(0.0f,0.0f,1.0f);
		}
		else
		{
			return m_pPhsVehicle->GetTerrainNormal(); 
		}
	}

	//NxVec3 GetVelDrifting() { return m_pPhsVehicle->GetVelDrifting(); }



	NxVec3 GetPos() 
	{ 
		if ( m_pPhsVehicle )
		{
			return m_pPhsVehicle->GetPosGlobal();
		}
		else
		{
			assert(0);
			return NxVec3 ( 0.0f,0.0f,0.0f);
		}
	}

	// 修改人: wincoqin
	// 时间:   2007/2/5
	// 原因:   在RemoteCar中需要传入的参数是ms, 而且是整数，所以要乘以1000
	void RemoteCarUpdate(float time) 
	{
		if ( !m_pPhsVehicle )
		{
			assert(0);
			return;
		}
		return m_pPhsVehicle->RemoteCarUpdate(time * 1000); 
	}
	
	void SetDrifting(bool val)  { return;}
	
	
	void SetVelDrifting(NxVec3 vel) {  /*return m_pPhsVehicle->SetVelDrifting(vel); */ }

	void setAccelTime(NxReal life = 0, NxReal gear = 0) { /*  return m_pPhsVehicle->setAccelTime(life, gear); */ }

	NxVec3 GetVelocityLocal() 
	{ 
		if ( !m_pPhsVehicle )
		{
			assert(0);
			return NxVec3 ( 0.0f,0.0f,0.0f );
		}
		return m_pPhsVehicle->GetVecLocal();
	}

	float GetTurnAngle() 
	{
		if ( !m_pPhsVehicle )
		{
			assert (0);
			return 0.0f;
		}
		return m_pPhsVehicle->GetTurnAngle(); 
	}

	void SetPos(NxVec3 pos) 
	{ 
		if ( !m_pPhsVehicle )
		{
			return;
		}
		return m_pPhsVehicle->SetPosGlobal(pos);
	}

	
	void createCarWithDesc2(const NiAVObject* pNif,const NxVec3& pos, bool frontWheelDrive, 
		bool backWheelDrive, bool corvette, bool monsterTruck, 
		bool oldStyle, NxScene* gScene, Play * pPlay, int KarType = 0);

	void UnCreateCarWithDesc2(){
		if ( m_pPhsVehicle ){
			delete m_pPhsVehicle;
			m_pPhsVehicle = NULL;
		}
	}

	void Control(float dt);
	void SynchronizeModel(NiAVObject* pKartModel);
	void Update(float fDeltaTime);

public:


	CTencentCar  * GetVehicle(){ return m_pPhsVehicle;}

protected:
	bool InitKartParam(const NxVec3& pos,bool frontWheelDrive, bool backWheelDrive, 
		bool corvette, bool bMonsterTrunk, bool bOldStyle, NxScene* gScene, Play * pPlay);

protected:

	CTencentCar * m_pPhsVehicle;


	NiAVObject* m_spCarRoot;

	NiPoint3	m_kTranslate;
	NiMatrix3	m_kRotate;

	NxMat34     m_kKartPose;

	NxReal      m_fAngleAccum; 

	//the original position of the wheels.
	NiPoint3    m_posWheels[4];
	bool		m_bMovable;
	bool		m_bGameOver;
};

#endif
