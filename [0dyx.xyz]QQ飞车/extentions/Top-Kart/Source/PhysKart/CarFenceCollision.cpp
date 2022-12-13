/********************************************************************
作者:	　wincoqin
时间:	  2007/01/30
文件名:   CarFenceCollision.h
目的:     
*********************************************************************/
// precompile header
#include "Top-KartPCH.h"
// libfile
#include <NxMat33.h>

// userfile
#include "../play.h"
#include "CarFenceCollision.h"
#include "TencentCarCollision.h"
#include "TencentCar.h"
#include "common.h"
#include "DriftCenter.h"

CCarFenceCollision::CCarFenceCollision(CCollisionCenter * pCenter)
: CCollisionBase(pCenter, XUserData::AT_FENCE_GAMEBRYO | XUserData::AT_FENCE_MANUAL | XUserData::AT_PUSHABLE)
{
	m_dwLstTime = 0;
}

CCarFenceCollision::~CCarFenceCollision()
{

}

void CCarFenceCollision::OnTrigger(void * pSelf, void * pOtherInfo, const DWORD dwCollisionType)
{	

	if ( GetTickCount() <= m_dwLstTime + 500)
	{
		return;
	}

	m_dwLstTime = GetTickCount();
	
	assert(m_dwCollisionType & dwCollisionType);
	CDriftCenter * pDriftCenter = ((Play*)pSelf)->GetKart()->GetVehicle()->GetDriftCenter();
	if ( pDriftCenter && pDriftCenter->IsDrifting())
	{
		//pDriftCenter->DriftEnd(false);
		pDriftCenter->SetCollision(true);
	}
	ProcessCarFenceCollision((Play *) pSelf, (CollisionInfo *) pOtherInfo, dwCollisionType);
}

void CCarFenceCollision::ProcessCarFenceCollision(Play * pPlay, CollisionInfo * pCollisionInfo, 
												  const DWORD dwCollisionType)
{
	assert(pPlay && pCollisionInfo);

	NxShape * pkHitShape = pCollisionInfo->collisionShape;

	if (!pPlay || !pCollisionInfo || !pkHitShape) 
		return ;

	CTencentCar * pCar = pPlay->GetKart()->GetVehicle();

	if (!pCar)
		return ;

	if (pPlay->GetKart() && pPlay->GetKart()->GetVehicle())
	{
		pPlay->GetKart()->GetVehicle()->StopDrift();
	}

			
	if (dwCollisionType & XUserData::AT_PUSHABLE)
	{
		NxActor& actor = pkHitShape->getActor();
		if(actor.isDynamic())
		{
			// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
			// useless stress on the solver. It would be possible to enable/disable vertical pushes on
			// particular objects, if the gameplay requires it.
			//if(hit.dir.y==0.0f)
			{
				NxF32 coeff = actor.getMass() * pCollisionInfo->fLength * 30.0f;
				actor.addForceAtLocalPos(pCollisionInfo->dir*coeff, NxVec3(0,0,0), NX_IMPULSE);

				pCar->SetVecGlobal(-0.8f * pCar->GetVecGlobal());
			}
		}
	}
	else
	{
		if (dwCollisionType & XUserData::AT_FENCE_GAMEBRYO)
		{
			pCollisionInfo->worldNormal *= -1.0f;
			applyCounterforce(pCar, pCollisionInfo);
		}
		else
		{
			applyCounterforce(pCar, pCollisionInfo);
		}		

	}
		
}

void CCarFenceCollision::applyCounterforce(CTencentCar * pCar, CollisionInfo * pCollisionInfo)
{


	NxVec3 vecZ(1, 0, 0.0f);
	NxVec3 carDir = pCar->GetForwardGlobal();
	carDir.normalize();
	carDir = -carDir;

	NxVec3 wall(pCollisionInfo->worldNormal);
	wall.normalize();

	NxReal delta = acos(carDir.dot(wall));

	NxVec3 kVelocityLocal = pCar->GetVecLocal();
	NxVec3 kVelocityDir = kVelocityLocal;
	NxVec3 dirU = wall.cross(carDir);

	NxVec3 kVelocityGlobal = pCar->GetVecGlobal();
	NxVec3 kVelocityGlobal_normal = kVelocityGlobal.dot(wall) * wall;
	NxVec3 kVelocityGlobal_tangent= kVelocityGlobal - kVelocityGlobal_normal;
	NxMat33 kRotate = pCar->GetOriGlobal();

	int nSteering = 0;
	if (InputManager::keys[NiInputKeyboard::KEY_A]) 
	{
		nSteering = 1;
	}
	else if (InputManager::keys[NiInputKeyboard::KEY_D])
	{
		nSteering = -1;
	}
	CDriftCenter * pDriftCenter = pCar->GetDriftCenter();
	if ( pDriftCenter && pDriftCenter->IsDrifting())  // && delta < NxMath::degToRad(ContactConfig::midangle)
	{
		//	TODO: 这样直接弄漂移有问题，后面再查	
		NxVec3 kDriftVecLocal  = pDriftCenter->GetLDV();
		NxVec3 kDriftVecGlobal = NxVec3( 0.0f, 0.0f , 0.0f );
		pCar->LocalToGlobal(kDriftVecLocal,kDriftVecGlobal);


		NxVec3 kDriftVecGlobal_normal = kDriftVecGlobal.dot(wall) * wall;
		NxVec3 kDriftVecGlobal_tangent= kDriftVecGlobal - kDriftVecGlobal_normal;
		kDriftVecGlobal = (kDriftVecGlobal_tangent - kDriftVecGlobal_normal);

		pCar->GlobalToLocal(kDriftVecGlobal,kDriftVecLocal);
		pDriftCenter->SetLDV(kDriftVecLocal * 0.35f);

		LogText("漂移反弹!");
		/*pDriftCenter->SetDriftVecGlobal(kVelocityGlobal);
		float fWecZ = NxMath::sign(dirU.z * kVelocityLocal.x)
			* delta;
		pCar->SetWecZLocal(fWecZ);
		delta = NxMath::sign(dirU.z * kVelocityLocal.x) * delta;
		if (delta > 0)
			delta = NxPi + delta;
		else
			delta = delta - NxPi;
		pCar->SetTurnAngle(pCar->GetTurnAngle() + delta );
		RotateMat33ZAxis( kRotate, delta);

		pCar->SetMatGround(kRotate);*/
		return;
	}

	float  fScaleVelocity = kVelocityLocal.magnitude();
	
	if ( delta < NxMath::degToRad(ContactConfig::smallangle) )
	{
		float fTurnAngle = 0.0f;
		fTurnAngle = -NxMath::sign(dirU.z * kVelocityLocal.x)
			* delta * 3.0f * fScaleVelocity / (0.5f + fScaleVelocity);
		if (!nSteering)
		{
			pCar->SetWecZLocal(fTurnAngle);		
		}

		else
		{
			fTurnAngle = fabsf(fTurnAngle) * nSteering * 0.5f;
			NxMat33 kRotate = pCar->GetOriGlobal();
			RotateMat33ZAxis( kRotate, fTurnAngle);
			pCar->SetMatGround(kRotate);
			pCar->SetTurnAngle(fTurnAngle + pCar->GetTurnAngle());
		}
	

		kVelocityLocal.x *= -ContactConfig::inversevelocityrate;		
		LogText("反弹头!");
		pCar->SetVecLocal(kVelocityLocal);
	}
	else if ( delta < NxMath::degToRad(ContactConfig::midangle) )
	{				
		float fCoefAngle = 0.0f;
		LogText("Delta = %f\n", delta);
		if (delta < NxPi / 2.f) 
		{
			NxReal coeff = fScaleVelocity / (0.5f + fScaleVelocity);
			float fTurnAngle = NxMath::degToRad(1.0f + 2.0f * coeff);
			fTurnAngle = NxMath::sign(dirU.z * kVelocityLocal.x)
				* delta * 2.f * fTurnAngle / (1.f + fTurnAngle);
			if (fabs(fTurnAngle) > NxPi / 4.f)
				fTurnAngle = NxMath::sign(fTurnAngle) * NxPi / 4.f;
			
			if (delta < NxMath::degToRad(80.0f))
			{
				LogText("正面一点 TurnAngle = %f\n", fTurnAngle);								

				RotateMat33ZAxis( kRotate, fTurnAngle);
				pCar->SetMatGround(kRotate);
				pCar->SetTurnAngle(fTurnAngle + pCar->GetTurnAngle());
				float VecLen = kVelocityGlobal.magnitude();
				NxVec3 k1, k2; 
				float len1 = kVelocityGlobal_normal.magnitude();
				float len2 = kVelocityGlobal_tangent.magnitude();

				NxVec3 vec = pCar->GetVecGlobal() * -1.0f;
				vec.normalize();
				float angle = acos(vec.dot(wall));

				if (len1 >= len2)
				{
					k1 = -0.5f * kVelocityGlobal_normal;
					k2 = 0.5f * kVelocityGlobal_tangent;
				}
				else 
				{
					if (fabsf(NxMath::radToDeg(delta) - NxMath::radToDeg(angle)) < 1.0f)
					{
						k1 = -1.1f * kVelocityGlobal_normal;
						k2 = 0.5f * kVelocityGlobal_tangent;
					}
					else
					{
						k1 = -0.5f * kVelocityGlobal_normal;
						k2 = 0.5f * kVelocityGlobal_tangent;
					}
				}

				kVelocityGlobal = k1 + k2;

//				LogText("%f %f===============================> %f %f\n", NxMath::radToDeg(delta), NxMath::radToDeg(angle), VecLen, kVelocityGlobal.magnitude());
//				LogText("%f %f<--> %f %f\n", kVelocityGlobal_normal.magnitude(), kVelocityGlobal_tangent.magnitude(), 
//					k1.magnitude(), k2.magnitude());
			}			
			else
			{
				fTurnAngle *= 0.1f;
				LogText("侧面一点 TurnAngle = %f\n", fTurnAngle);	
				RotateMat33ZAxis( kRotate, fTurnAngle);
				pCar->SetMatGround(kRotate);
				pCar->SetTurnAngle(fTurnAngle + pCar->GetTurnAngle());
				kVelocityGlobal = -0.5f * kVelocityGlobal_normal + 0.9f * kVelocityGlobal_tangent;				
			}

			pCar->SetVecGlobal(kVelocityGlobal);
		}		
		
	}
	else if ( delta > NxMath::degToRad(ContactConfig::bigangle) )
	{
		kVelocityLocal.x *= -ContactConfig::inversevelocityrate;
		pCar->SetVecLocal(kVelocityLocal);
		LogText("反弹尾!");
	}

	

}
