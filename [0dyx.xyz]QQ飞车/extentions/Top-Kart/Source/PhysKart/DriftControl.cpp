#include "../Top-KartPCH.h"
#include ".\driftcontrol.h"
#include <NiDI8InputSystem.h>
#include "../NxPlayMgr.h"
#include "../play.h"
#include "../KartCrack.h"

CDriftControl::CDriftControl(WheellessCar * pWheelCar):
m_pWheelCar(NULL)
{
	m_nDriftStatus = DRIFT_NONE;
	m_nMinDriftingTriggerVel = 18.0f / 5.0f;
	m_fDriftingTotalTime = 3.5f;
	
	m_vDriftGlobalVel = NxVec3(0,0,0);	
	m_nDriftDir = 0; 
	
	m_fDriftingTime = 0.0f;

	m_fDriftingAngle = 0.0f;
	m_fDriftingAngleVel = 0.0f;
	m_fDriftingAngleVelMax = NxPi/180.0f * 90.0f;

	m_fDriftingSideAngleCur = 0.0f;
	m_fDriftingSideAngleMax = 0.0f;

	_bCreateNewTrack =  true;
	m_pWheelCar = pWheelCar;
	m_fMaxFricAccel = 25.0f;
	m_fVecPower_AngelDrifting = 0.0f;

	m_pInputSystem = InputManager::Get()->GetInputSystem();
	m_nIsAngelDrifting = DescConstant::KarBaseParam[DescConstant::KarType].bDreamDrifting;
}

CDriftControl::~CDriftControl()
{
}

static float ClockAngleDeg = 120;
static float   nLastdir = 0.0f;

//-----------------------------------------------------------------------

void CDriftControl::StartDrift(bool bkeyLeft, bool bKeyRight)
{
	// 暂时规定： 漂移状态中时不运行开始新的漂移
	if ( DRIFT_NONE != GetDriftStatus())
	{
		return;
	}
	ClockAngleDeg = 120;
	nLastdir = 0.0f;

	m_vDriftGlobalVel  = m_pWheelCar->getGlobalOrientation() * 
		m_pWheelCar->GetVelocityLocal();

	m_nDriftDir = bKeyRight? DRIFT_RIGHT: DRIFT_LEFT;
	
	m_fDriftingTime = 0.0f;
	m_fDriftingAngle = 0.0f;
	m_fDriftingSideAngleCur = 0.0f;

	m_fDriftingAngleVel = m_fDriftingAngleVelMax;

	NxReal fVel = m_pWheelCar->GetVelocityLocal().magnitude();
	fVel *= fVel;
	NxReal coeff = fVel/ (1.0f + fVel);
	m_fDriftingSideAngleMax = NxPi/180.0f * DRIFT_SIDE_ANGLE * coeff;

	SetDriftStatus( DRIFT_START );
	ResetAngelDrifting();
	ClearVecPower();
	m_fShiftStartTime = GetTickCount();
	m_fShiftLstTime = 0.0f;
	m_fShiftRlsTime		= 0.0f;
	m_fShiftMaxLstTime = 0.0f;
}


//-----------------------------------------------------------------------
void CDriftControl::EndDrift()
{
	if ( GetDriftStatus() != DRIFT_START )
	{
		return;
	}
	NxMat33 matInverse = m_pWheelCar->getGlobalOrientation();
	if (!matInverse.getInverse(matInverse))
		return;
	
	NxVec3 AdjustvelLocal = matInverse * m_vDriftGlobalVel;


	// 有问题的代码
	m_pWheelCar->AddTurnAngle(m_fDriftingAngle);
	//m_pWheelCar->setVelocityLocal( NxVec3(m_vDriftGlobalVel.magnitude() * 0.5f, 0, 0) );  
	//m_pWheelCar->setVelocityLocal( NxVec3(m_vDriftGlobalVel.x * cos(m_fDriftingAngle),
	//	m_vDriftGlobalVel.y * cos(m_fDriftingAngle),-0.3f ));  

	if ( CanAngleDrifting() )
	{
		// 玄漂之 凌波微步
		if ( GetVecPower() > 0.0f)
		{
			NxReal VecAngelDrifting = m_pWheelCar->GetCarAccelControl()->GetMaxVel() * GetVecPower() * 0.7 ;
			m_pWheelCar->setVelocityLocal( NxVec3 ( VecAngelDrifting,0,-0.3));
		}
		else
		{
			m_pWheelCar->setVelocityLocal(NxVec3( AdjustvelLocal.x,0,-0.3));
		}
	}
	else
	{
		m_pWheelCar->setVelocityLocal(NxVec3( AdjustvelLocal.x,0,-0.3));
	}

	m_fDriftingAngle = 0.0f;
	m_vDriftGlobalVel = NxVec3(0,0,0);
	_driftingType = 0;
	_bCreateNewTrack = true;

	SetDriftStatus( DRIFT_NONE );
	ResetAngelDrifting();
	ClearVecPower();

	m_fShiftLstTime		= 0.0f;
	m_fShiftRlsTime		= 0.0f;
	m_fShiftStartTime	= GetTickCount();
	m_fShiftMaxLstTime = 0.0f;
	//StopMagical();
}

//-----------------------------------------------------------------------
void CDriftControl::ShiftProcess(bool bkeyShift,float dt)
{
	if ( bkeyShift ) 
	{
		m_fShiftLstTime += dt*1000;
		m_fShiftMaxLstTime = m_fShiftLstTime;
		m_fShiftRlsTime = 0.0f;
	}
	else
	{
		m_fShiftRlsTime += dt*1000;
	}
	return;
}

bool CDriftControl::GetShiftDecide()
{
	if ( m_fShiftRlsTime > 2 * m_fShiftMaxLstTime + 100   )
	{
		m_fShiftLstTime = 0.0f;
		m_fShiftRlsTime = 0.0f;
		return true;
	}
	return false;
}					

//-----------------------------------------------------------------------
void CDriftControl::DriftControlUpdate(bool bkeyShift, bool bkeyUp, bool bkeyLeft, bool bkeyRight, float dt)
{

	NiInputKeyboard* pkKeyboard = m_pInputSystem->GetKeyboard();

	pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT) ;

		// 这里车身角度大于90度的时候， 会触发新的漂移 , 很有技巧的，可能是浑然天成的附带品


	if ( DRIFT_NONE == GetDriftStatus() )
	{
		if ( m_pWheelCar->GetVelocityLocal().magnitude() > m_nMinDriftingTriggerVel &&
			bkeyShift && (bkeyLeft || bkeyRight))
		{
			return StartDrift( bkeyLeft, bkeyRight );
		}
	}

	if ( DRIFT_START == GetDriftStatus() )
	{
		if ( m_vDriftGlobalVel.magnitude() <= m_nMinDriftingTriggerVel  ||
			(m_fDriftingTime > m_fDriftingTotalTime  ) || GetShiftDecide())
		{
			return EndDrift();
		}

		/*
		else if ( m_pWheelCar->GetVelocityLocal().magnitude() > m_nMinDriftingTriggerVel * 2 &&
			( pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT) && m_nDriftDir == DRIFT_RIGHT ) ||
			 ( pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT) && m_nDriftDir == DRIFT_RIGHT ) )
		{
			return EndDrift();
			;	//return StartDrift( bkeyLeft,bkeyRight );
		}
		*/
	}
	if ( bkeyUp &&  ((bkeyLeft)||(bkeyRight)) ) 
	{
		UpdateDriftKeyUp(bkeyShift, bkeyUp, bkeyLeft, bkeyRight, dt);
	}
	else
	{
		UpdateDrift(bkeyShift, bkeyUp, bkeyLeft, bkeyRight, dt);
	}

	ShiftProcess(bkeyShift, dt);
}

//-----------------------------------------------------------------------
NxReal CDriftControl::GetDriftingAngleAccel()
{
	NxReal  fAccel			=	m_pWheelCar->GetForwardAccel();  //考虑重力等情况
	NxVec3	VelLocal		=   m_pWheelCar->GetVelocityLocal();
	
	
	//摩擦公式
	if ( abs(m_fMaxFricAccel - fAccel) < 0.01 ) 
	{
		return 0.0f;
	}
	else 
	{
		return abs(m_fMaxFricAccel - fAccel) * abs(cos( GetDriftingAngle())) + 5.0;
	}
}

NxReal CDriftControl::GetAngleBetweenVelAndDrift()
{
	NxVec3	CarGlobalVel		=   m_vDriftGlobalVel;
	NxVec3  CarGlobalForward    = m_pWheelCar->getGlobalForward();

	NxMat33 matInverse = m_pWheelCar->getGlobalOrientation();
	if (!matInverse.getInverse(matInverse))
		return 0.0f;

	NxVec3 CarLocalForward = matInverse * CarGlobalForward;
	NxVec3 CarLocallVel    = matInverse * CarGlobalVel;

	CarLocalForward.normalize();
	CarLocallVel.normalize();

	NxReal alpha = NxMath::acos(CarLocalForward.dot(CarLocallVel)); 

	return (alpha);
}

//-----------------------------------------------------------------------
static float testMaxAngleVel = 0.0f;
void CDriftControl::UpdateDriftKeyUp(bool keyShift, bool keyUp, bool keyLeft, bool keyRight, float dt)
{
	if ( DRIFT_START != GetDriftStatus() )
		return;

	m_fDriftingTime += dt;

	m_vDriftGlobalVel *= 0.98;

	NxMat33 matInverse = m_pWheelCar->getGlobalOrientation();
	if (!matInverse.getInverse(matInverse))
		return;

	NxVec3 AdjustvelLocal = matInverse * m_vDriftGlobalVel;
	//m_pWheelCar->setVelocityLocal(NxVec3( AdjustvelLocal.x,0,-0.3));

	m_pWheelCar->setVelocityLocal(AdjustvelLocal);

	if ( m_nDriftDir == DRIFT_LEFT && keyRight)
		m_nDriftDir = DRIFT_RIGHT;

	if ( m_nDriftDir == DRIFT_RIGHT && keyLeft)
		m_nDriftDir = DRIFT_LEFT;

	if ((  m_nDriftDir == DRIFT_LEFT && keyRight ) ||
		( m_nDriftDir == DRIFT_RIGHT && keyLeft))
	{
		m_fDriftingAngleVel = 0.0f;
	}

	if (GetAngleBetweenVelAndDrift() < NxPi /2 )
	{
		float DeltaAngle = 0.0f;
		if ( (m_nDriftDir == DRIFT_LEFT))
		{
			m_fDriftingAngleVel = __min(m_fDriftingAngleVelMax, m_fDriftingAngleVel + m_fDriftingAngleVel * dt);
			DeltaAngle +=(m_fDriftingAngleVel * dt +0.5 * GetDriftingAngleAccel() * dt * dt);
		}
		else  if ( m_nDriftDir == DRIFT_RIGHT)
		{
			m_fDriftingAngleVel = __min(m_fDriftingAngleVelMax, m_fDriftingAngleVel + m_fDriftingAngleVel * dt);
			DeltaAngle -= (m_fDriftingAngleVel * dt + 0.5 * GetDriftingAngleAccel() * dt * dt);
		}
		m_fDriftingAngle += DeltaAngle;
		ResetAngelDrifting();
	}
	else if ( GetAngleBetweenVelAndDrift() > NxPi/2 ) 
	{
		NxMat33 matInverse = m_pWheelCar->getGlobalOrientation();
		if (!matInverse.getInverse(matInverse))
			return;
		NxMat33 rotZ;
		NxVec3 AdjustvelLocal = matInverse * m_vDriftGlobalVel;

		NxReal NormalAngleVel = m_pWheelCar->GetGriftingAngleVel() * 5.0f;
		
		// 玄漂判定代码
		if ( !CanAngleDrifting() )
		{
			if ( abs( NormalAngleVel) > 1.0)
			{
				NormalAngleVel = 1.0 * NxMath::sign(NormalAngleVel);
			}
		}
		else
		{
			if ( abs( NormalAngleVel) > 5.0)
			{
				NormalAngleVel = 5.0 * NxMath::sign(NormalAngleVel);
			}
		}

		rotZ.rotZ(- NormalAngleVel  * dt);
		AdjustvelLocal = rotZ * AdjustvelLocal ;

		m_pWheelCar->setVelocityLocal(AdjustvelLocal);
		m_vDriftGlobalVel =m_pWheelCar->getGlobalOrientation() * AdjustvelLocal;

		m_fDriftingAngle += - NormalAngleVel  * dt;
		
		// 玄漂开始
		if ( CanAngleDrifting() )
		{
			UpdateAngelDrifting ( dt,NormalAngleVel );
		}
		else
		{
			if ( testMaxAngleVel < abs (NormalAngleVel))
			{
				testMaxAngleVel = NormalAngleVel;
			}
			m_vDriftGlobalVel *= 0.98;
		}

	}


	// 这里应该是一条曲线

	if ( m_pWheelCar->HasContactGrd() == CONTACTGRD )
	{

		////增加车身倾斜角度
		//if (_driftingSideAngleCur < _driftingSideAngleMax)
		//	_driftingSideAngleCur += (NxPi/180.0f * DRIFT_SIDE_ANGLE * dt);
		//else
		//	_driftingSideAngleCur = _driftingSideAngleMax;
	}
	else
	{
		m_vDriftGlobalVel.z =-0.3f;;
	}

	NxVec3 displacement = m_vDriftGlobalVel * dt;
	NxF32 sharpness = 1.0f;

	if( _collisionFlags & NXCC_COLLISION_DOWN)
		m_vDriftGlobalVel.z = -3.0f;

	m_pWheelCar->getBoxController()->move(displacement, PhysXManager::CG_ENVIRONMENT, 0.000001f, _collisionFlags, sharpness);
	UpdateDriftTrack();
}

//-----------------------------------------------------------------------

void CDriftControl::CaculateMagicalCenterPos( NxReal AngleVel)
{
	//m_vDriftGlobalVel.magnitude()
	if ( abs( AngleVel - 0.0f ) > 0.001)
	{
		NxVec3 forward = m_pWheelCar->getGlobalForward();
		forward.normalize();
		NxVec3 Radio =m_vDriftGlobalVel.magnitude() / abs(AngleVel) * forward ;
		m_fCenter_AngelDrifting.x = m_pWheelCar->GetPos().x + Radio.x;
		m_fCenter_AngelDrifting.y = m_pWheelCar->GetPos().y + Radio.y;
		m_fCenter_AngelDrifting.z = m_pWheelCar->GetPos().z + Radio.z;
	}

}

//-----------------------------------------------------------------------
void CDriftControl::StopMagical()
{
	if ( m_vecEffect.empty())
	{
		return;
	}

	for ( int i = 0 ; i < m_vecEffect.size(); i++)
	{
		IGameEffect::EffectType keke =(IGameEffect::EffectType) m_vecEffect[i];
		CGameEffectMgr::DeActiveEffect(keke);
	}

}

void CDriftControl::IncVecPower( NxReal AngelVel ,float dt)
{
	m_fAccmulatePower += abs(AngelVel) * dt;
}

float CDriftControl::GetVecPower()
{
	if ( m_fAccmulatePower < NxPi ) 
		return 0.0f;
	if ( m_fAccmulatePower < NxPi * 210/ 180)
		return 0.3f;
	if ( m_fAccmulatePower < NxPi * 240/ 180)
		return 0.5f;
	if ( m_fAccmulatePower < NxPi * 300/ 180)
		return 0.75f;
	if ( m_fAccmulatePower > NxPi * 360/ 180)
		return 1.0f;
	return 0.0f;
}

//-----------------------------------------------------------------------
void CDriftControl::UpdateAngelDrifting(float dt, NxReal AngleVel)
{
	IncVecPower(AngleVel, dt);

	m_fAngle_AngelDrifting += AngleVel * dt ;
	if ( abs(m_fAngle_AngelDrifting)  > NxPi   )
	{
		int nMagical = GenerateMagical();
		TriggerMagical(nMagical);
		ResetAngelDrifting();
	}
	CaculateMagicalCenterPos(AngleVel);
}

//-----------------------------------------------------------------------
int CDriftControl::GenerateMagical()
{
	return CDriftControl::EAM_EFFECT;
}

void CDriftControl::TriggerMagical(int eMagical)
{
	NxVec3 Pos =  m_pWheelCar->GetPos();
	NxVec3 CenterPos = GetMagicalCenterPos();
	NiPoint3 CarPoint = NiPoint3( Pos.x, Pos.y, Pos.z);
	NiPoint3 CarCenterPoint = NiPoint3( CenterPos.x, CenterPos.y, CenterPos.z );

	switch (eMagical)
	{
	case CDriftControl::EAM_EFFECT:
		{
			CGameEffectMgr::ActiveEffect(IGameEffect::ET_ANGLEDRIFT,&CarPoint);
			CGameEffectMgr::ActiveEffect(IGameEffect::ET_ANGLEDRIFTCENTER, &CarCenterPoint);
			m_vecEffect.push_back(IGameEffect::ET_ANGLEDRIFT);
			break;
		}
	case CDriftControl::EAM_FOG:
		break;
	case CDriftControl::EAM_BANA:
		break;
	case CDriftControl::EAM_MISSILE:
		break;
	default:
		break;
	}
}

//-----------------------------------------------------------------------
// 星宿老仙风格的代码

void CDriftControl::ModifyClockAngleFactor()
{
	float DeltaA =ClockAngleDeg - 90;
	ClockAngleDeg = 90 - DeltaA * 0.5;
	if ( abs( ClockAngleDeg - 90) < 2.5 )
	{
		ClockAngleDeg = 90;
	}
}

float CDriftControl::GetClockAngle()
{

	if ( nLastdir * NxMath::sign(NxPi/180 * ClockAngleDeg - abs(m_fDriftingAngle)) < 0)
	{
		EndDrift();
		return 0.0;
		//ModifyClockAngleFactor();
	}
	
	float nfactor = abs( ClockAngleDeg - abs( m_fDriftingAngle));

	nLastdir =  NxMath::sign(NxPi/180 * ClockAngleDeg - abs(m_fDriftingAngle));

	if ( nfactor < 5.0f)
		return nLastdir / 100.0f;
	else
		return nLastdir;

}

//-----------------------------------------------------------------------
void CDriftControl::UpdateDrift(bool keyShift, bool keyUp, bool keyLeft, bool keyRight, float dt)
{
	if ( DRIFT_START != GetDriftStatus() )
		return;

		m_fDriftingTime += dt;


	//	m_vDriftGlobalVel.x -=NxMath::sign(m_vDriftGlobalVel.x) * (20.0f * ( 1 - abs( cos( m_fDriftingAngle ))) * dt + 20.0f * dt);

	m_vDriftGlobalVel *= 0.98;

	if (((m_nDriftDir == DRIFT_LEFT) && (!keyLeft)) ||
		((m_nDriftDir == DRIFT_RIGHT) && (!keyRight)))	
	{
	//	if (abs(m_fDriftingAngle) < (NxPi/180.f * 5.f))
			//_driftingType = SIMPLE_DRIFT;
	}


	NxMat33 matInverse = m_pWheelCar->getGlobalOrientation();
	if (!matInverse.getInverse(matInverse))
		return;

	NxVec3 AdjustvelLocal = matInverse * m_vDriftGlobalVel;
	m_pWheelCar->setVelocityLocal(NxVec3( AdjustvelLocal.x,0,-0.3));

	if ( m_nDriftDir == DRIFT_LEFT && keyRight)
	{
		m_nDriftDir = DRIFT_RIGHT;
	}

	if ( m_nDriftDir == DRIFT_RIGHT && keyLeft)
	{
		m_nDriftDir = DRIFT_LEFT;
	}

//	float UseAngleVel = m_fDriftingAngleVel;
	if ((  m_nDriftDir == DRIFT_LEFT && keyRight ) ||
		( m_nDriftDir == DRIFT_RIGHT && keyLeft))
	{
		m_fDriftingAngleVel = 0.0f;
	}

	if (_driftingType != SIMPLE_DRIFT)
	{
		if ( (m_nDriftDir == DRIFT_LEFT))
		{
			if ( abs(m_fDriftingAngle) < NxPi * 180 / ClockAngleDeg)
			{
				m_fDriftingAngleVel = __min(m_fDriftingAngleVelMax, m_fDriftingAngleVel + m_fDriftingAngleVel * dt);
				m_fDriftingAngle += GetClockAngle() * (m_fDriftingAngleVel * dt +0.5 * GetDriftingAngleAccel() * dt * dt);

				/*
				m_fDriftingAngle += GetClockAngle() * 
				(m_fDriftingAngleVel*dt + 0.5 * GetDriftingAngleAccel() * dt * dt);
				*/
			}
		}
		else if (m_nDriftDir == DRIFT_RIGHT)
		{
			if ( abs(m_fDriftingAngle) < NxPi * 180 / ClockAngleDeg)
			{
				m_fDriftingAngleVel = __min(m_fDriftingAngleVelMax, m_fDriftingAngleVel + m_fDriftingAngleVel * dt);
				m_fDriftingAngle -= GetClockAngle() * (m_fDriftingAngleVel * dt + 0.5 * GetDriftingAngleAccel() * dt * dt);
				/*
				m_fDriftingAngle -= GetClockAngle() *
				(m_fDriftingAngleVel*dt + 0.5 * GetDriftingAngleAccel() * dt * dt);
				*/
			}
		}
	}
	else
	{
		m_vDriftGlobalVel *= 0.96;
	}

	// 这里应该是一条曲线


	if ( m_pWheelCar->HasContactGrd() == CONTACTGRD )
	{

		////增加车身倾斜角度
		//if (_driftingSideAngleCur < _driftingSideAngleMax)
		//	_driftingSideAngleCur += (NxPi/180.0f * DRIFT_SIDE_ANGLE * dt);
		//else
		//	_driftingSideAngleCur = _driftingSideAngleMax;
	}
	else
	{
		m_vDriftGlobalVel.z =-0.3f;;
	}

	NxVec3 displacement = m_vDriftGlobalVel * dt;
	NxF32 sharpness = 1.0f;

	if( _collisionFlags & NXCC_COLLISION_DOWN)
		m_vDriftGlobalVel.z = -3.0f;

	m_pWheelCar->getBoxController()->move(displacement, PhysXManager::CG_ENVIRONMENT, 0.000001f, _collisionFlags, sharpness);
	UpdateDriftTrack();
	
}

//-----------------------------------------------------------------------
void CDriftControl::UpdateDriftTrack()
{
	if(NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle() != m_pWheelCar)
	{
		return;
	}

	if(_bCreateNewTrack)
	{
		NxSceneMgr::Get()->GetActiveSceneLevel()->m_sKartCrack.AddNewTrack();
		_bCreateNewTrack = false;
	}

	for(int i = 2; i < 4; i++)
	{
		if(m_pWheelCar->GetWheel(i)->_contactType == CWheel::CT_NEAR)
		{
			NiPoint3 pUp,pDir,pPos;

			NxVec3 nv3 = m_pWheelCar->GetWheel(i)->_contactPos;
			pPos.x = nv3.x;
			pPos.y = nv3.y;
			pPos.z = nv3.z;


			NiMatrix3 pR;

			pR = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate();
	
			pR.GetCol(1,pDir);
			pR.GetCol(2,pUp);
			pDir = -pDir;
			pDir.Unitize();

			NiPoint3 p;
			NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate().GetCol(0,p);

			p.Unitize();

			//	修改人:	wincoqin
			//  时间:	2007/02/01
			//  目的:   调整车痕离车的距离			
			pPos += pDir * 0.08f;
			float fDel  = 0.07f;
			// 结束

			if(i == 2)
			{
				pPos += p * fDel;
			}
			else
			{
				pPos -= p * fDel;
			}
			NxSceneMgr::Get()->GetActiveSceneLevel()->m_sKartCrack.AddPoints(i - 2,pPos,pUp,pDir, 0.0f);
		}
	}
}

