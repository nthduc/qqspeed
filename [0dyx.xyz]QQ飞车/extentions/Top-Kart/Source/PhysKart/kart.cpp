#include "Top-KartPCH.h"
#include "kart.h"
#include "NxAllVehicles.h"
#include "../InputManager.h"
#include "./Stream.h"
#include "NxCooking.h"
#include "../Utility.h"
//extern bool InputManager::keys[256];
#include "../DescConstant.h"
#include "../play.h"
#include "TencentCar.h"
#define LOG_SPEED
#ifdef LOG_SPEED
FILE * g_pSpeedFile = NULL;
BOOL   g_bRecordSpeed = FALSE;
#endif

CKart::CKart()
: m_pPhsVehicle(NULL)	
, m_bMovable(false)
, m_spCarRoot(NULL)
, m_fAngleAccum(0.0f) 
, m_bGameOver(false)
{
}

CKart::~CKart()
{
	if(m_pPhsVehicle)
	{

		delete m_pPhsVehicle;
		m_pPhsVehicle = NULL;
	}
#ifdef LOG_SPEED
	if (g_pSpeedFile) 
	{
		fclose(g_pSpeedFile);
		g_pSpeedFile = NULL;
		g_bRecordSpeed = FALSE;
	}
#endif
}



void CKart::Control(float dt)
{
	if(!m_pPhsVehicle)
	{ 
		assert(0);
		return;
	}

	bool left		= (InputManager::keys[NiInputKeyboard::KEY_A] == true);
	bool right		= (InputManager::keys[NiInputKeyboard::KEY_D] == true);
	bool forward	= (InputManager::keys[NiInputKeyboard::KEY_W] == true);
	bool backward	= (InputManager::keys[NiInputKeyboard::KEY_S] == true);
	bool drift		= (InputManager::keys[NiInputKeyboard::KEY_LSHIFT]		== true);
	bool bcontrol	= (InputManager::keys[NiInputKeyboard::KEY_LCONTROL]	== true);


	if (!m_bMovable) 
	{		
		forward		= false;
		backward	= false;
		drift		= false;
		bcontrol	= false;
	}

	CTencentCar::KEYINFO stKeyInfo;
	stKeyInfo.bLeft	= left;
	stKeyInfo.bRight	= right;
	stKeyInfo.bUp		= forward;
	stKeyInfo.bDown	= backward;
	stKeyInfo.bShift	= drift;
	stKeyInfo.bCtrl	= bcontrol;
	DWORD dwDt = dt * 1000 ;
	m_pPhsVehicle->UpdateTencentCar(dwDt,stKeyInfo);

#ifdef LOG_SPEED
	if (stKeyInfo.bCtrl && stKeyInfo.bDown) 
	{
		if (!g_pSpeedFile) 
		{
			g_pSpeedFile = fopen(NiApplication::ConvertMediaFilename("Speed.txt"), "w+");
		}
		g_bRecordSpeed = !g_bRecordSpeed;	
	}

	if (g_bRecordSpeed) 
	{
		fprintf(g_pSpeedFile, "%f\t%f\n", GetMainApp()->GetCurrentTime(), GetVelocity());
		fflush(g_pSpeedFile);
	}
#endif

	//NxReal steering = 0;
	//if (left && !right) 
	//{
	//	steering = -1;

	//}
	//else if	(right && !left) 
	//	steering = 1;
	//NxReal acceleration = 0;
	//if (forward && !backward) 
	//	acceleration = 1;
	//else if (backward && !forward) 
	//	acceleration = -1;
	////bool handbrake = InputManager::keys[NiInputKeyboard::KEY_SPACE];
	//bool handbrake = false;


	//m_pPhsVehicle->control(acceleration, -steering, dt, drift, forward,left, right);


}

void CKart::Update(float fDeltaTime)
{
;
}

void CKart::SetKartRotate(const NiMatrix3& rotate)
{
	float fXAngle = 0.0f, fYAngle = 0.0f, fZAngle = 0.0f;
	rotate.ToEulerAnglesXYZ(fXAngle, fYAngle, fZAngle);
	if (m_pPhsVehicle) 
	{
		NxMat33 rot;
		rot.rotZ(fZAngle);
		//NiPhysXTypes::NiMatrix3ToNxMat33(rotate, rot);
		m_pPhsVehicle->SetOriGlobal(rot);
		m_pPhsVehicle->SetMatGround(rot);
		m_pPhsVehicle->SetTurnAngle(fZAngle);
	}
}

void CKart::SetKartPose(const NxMat34& kPose)
{
	m_kKartPose = kPose;

	if(!m_pPhsVehicle)
	{ 
		assert(0);
		return;
	}

	// 为兼容 所添加临时代码
	m_pPhsVehicle->SetPosGlobal(NxVec3(kPose.t.x, kPose.t.y, kPose.t.z));



}

void CKart::SynchronizeModel(NiAVObject* pKartModel)
{
	return;
}	

void CKart::SetKartPos(const NiPoint3& pos)
{
	if(!m_pPhsVehicle) 
	{
		assert(0);
		return ;
	}
	m_kTranslate = pos;
	NxVec3 vPos(pos.x, pos.y, pos.z);


	m_pPhsVehicle->SetPosGlobal(vPos);


}

// by winco, 
const NiPoint3 CKart::GetKartTranslate()
{
	//return  m_kTranslate;
	if(!m_pPhsVehicle) 
	{
		assert(0);
	}


	NxVec3 v3 = m_pPhsVehicle->GetPosGlobal();

	NiPoint3 pt(v3.x, v3.y, v3.z);
	return pt;
}

const NiMatrix3 CKart::GetKartRotate()
{
	if(!m_pPhsVehicle) 
	{
		assert(0);
	}
	NiMatrix3 niMt3;

	NxMat33 nxMat = m_pPhsVehicle->GetOriGlobal();


	NiPhysXTypes::NxMat33ToNiMatrix3(nxMat, niMt3);

	return niMt3;
}

const NxMat34 & CKart::GetKartPose()
{
	return m_kKartPose;
}



bool CKart::InitKartParam(const NxVec3& pos,bool frontWheelDrive, bool backWheelDrive, 
						  bool corvette, bool bMonsterTrunk, bool bOldStyle, NxScene* gScene, Play * pPlay)
{
	NxVec3 posTemp(pos.x, pos.y, pos.z+1.5f);


	m_pPhsVehicle = new CTencentCar(gScene, m_spCarRoot , pPlay);
	if ( !m_pPhsVehicle->InitTecentCar( posTemp) )
	{
		delete m_pPhsVehicle ;
		m_pPhsVehicle = NULL;
	}


	if( m_pPhsVehicle )
		return true;

	return false;

}
void CKart::createCarWithDesc2(const NiAVObject* pNif,const NxVec3& pos, bool frontWheelDrive, 
							   bool backWheelDrive, bool corvette, bool monsterTruck, 
							   bool oldStyle, NxScene* gScene,Play * pPlay, int KarType)
{
	m_spCarRoot = (NiAVObject*)pNif;
	DescConstant::KarType = KarType;
	m_bGameOver = false;
	if(!InitKartParam( pos,frontWheelDrive,backWheelDrive, corvette, monsterTruck, oldStyle, gScene, pPlay))
	{
		assert(0);
	}


}
