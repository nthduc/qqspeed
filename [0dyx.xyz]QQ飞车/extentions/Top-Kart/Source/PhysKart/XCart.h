#ifndef _XCART_H
#define _XCART_H

#include "Vehicle.h"
#include "NxVehicle.h"
#include "NiAVObject.h"
#include "../PhysXManager.h"

class XCart 
{
public:
	XCart();
	~XCart();

	void SetKartRotate(const NiMatrix3& rotate);
	void SetKartPos(const NiPoint3& pos);
	const NiPoint3  GetKartTranslate();
	const NiMatrix3 GetKartRotate();
	const NxMat34  & GetKartPose();
	void SetKartPose(const NxMat34& kPose);

	float GetVelocity() { return 0; }
	void SetMovable(bool val) {}
	bool IsDrifting() { return false; }
	void setVelocityLocal(NxVec3& vel) { }
	void SetTurnAngle(NxReal val) { }
	NxVec3 GetVelocityLocal() { return NxVec3(0, 0, 0); }
	NxVec3 GetTerrainNormal() { return NxVec3(0,1.0,0);}
	NxVec3 GetVelDrifting() { return NxVec3(0, 0, 0); }
	NxVec3 GetPos();
	void RemoteCarUpdate(float time) {}
	float GetTurnAngle() { return 0; }
	void SetDrifting(bool val) {}
	void SetVelDrifting(NxVec3 vel) {}
	void setAccelTime(NxReal life = 0, NxReal gear = 0) {}
	void SetPos(NxVec3 pos);

	void createCarWithDesc2(const NiAVObject* pNif,const NxVec3& pos, bool frontWheelDrive, 
		bool backWheelDrive, bool corvette, bool monsterTruck, 
		bool oldStyle, NxScene* gScene,int KarType = 0);

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
	NxVehicle * GetVehicle(){ return m_pPhsVehicle;}

protected:
	bool InitKartParam(const NxVec3& pos,bool frontWheelDrive, bool backWheelDrive, 
		bool corvette, bool bMonsterTrunk, bool bOldStyle, NxScene* gScene);

protected:
	//NxVehicle* m_pPhsVehicle;
	NxVehicle* m_pPhsVehicle;

	NiAVObject* m_spCarRoot;

	NiPoint3	m_kTranslate;
	NiMatrix3	m_kRotate;

	NxMat34     m_kKartPose;

	NxReal      m_fAngleAccum; 

	//the original position of the wheels.
	NiPoint3    m_posWheels[4];

};

#endif
