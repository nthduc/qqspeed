/********************************************************************
	作者:	　wincoqin
	时间:	  2007/01/27
    文件名:   CarCarCollision.cpp
	目的:     Implementation	
*********************************************************************/
#include "Top-KartPCH.h"

// libfile


// userfile
#include "../Play.h"
#include "../PhysUtility.h"
#include "TencentCar.h"
#include "CarCarCollision.h"
#include "TencentCarCollision.h"
#include "DriftCenter.h"


CCarCarCollision::CCarCarCollision(CCollisionCenter * pCollisionCenter)
: CCollisionBase(pCollisionCenter, XUserData::AT_WHEELLESSCAR)
{

}

CCarCarCollision::~CCarCarCollision()
{

}

void CCarCarCollision::OnTrigger(void * pSelf, void * pOtherInfo, const DWORD dwCollisionType)
{
	return;
	assert(m_dwCollisionType == dwCollisionType);
    Play * pPlaySelf  = (Play *)pSelf;
	Play * pPlayOther = (Play *)pOtherInfo;

	CDriftCenter * pDriftCenter = ((Play*)pSelf)->GetKart()->GetVehicle()->GetDriftCenter();
	if ( pDriftCenter && pDriftCenter->IsDrifting())
	{
		pDriftCenter->DriftEnd(false);
	}
	
	if (pSelf && pOtherInfo) 
	{
		ProcessCarCarCollision(pPlaySelf, pPlayOther);
	}
}

void CCarCarCollision::ProcessCarCarCollision(Play * pSelf, Play * pOther)
{
	return;
	LogText("物理引擎发生了车车碰撞\n");

	CTencentCar * pSelfCar = pSelf->GetKart()->GetVehicle();
	CTencentCar * pOtherCar = pOther->GetKart()->GetVehicle();

	//TODO: 增加的到self和other car的方法
	NxVec3 kSpeedSelf  = pSelfCar->GetVecGlobal();
	NxVec3 kSpeedOther = pOtherCar->GetVecGlobal();

	NxVec3 kRestSpeedSelf = kSpeedSelf;
	NxVec3 kRestSpeedOther = kSpeedOther;
	// 先把碰撞当球形碰撞来处理
	/*
	NxBox * pSelfBox  = pSelfCar->GetCollisionBox();
	NxBox * pOtherBox = pOtherCar->GetCollisionBox();

	NxUtilLib::NxBoxBoxIntersect()
	*/

	NxVec3 kPosSelf = pSelfCar->GetPosGlobal();
	NxVec3 kPosOther = pOtherCar->GetPosGlobal();

	// 确定碰撞的法线方向
	NxVec3 kNorm = kPosOther - kPosSelf;
	kNorm.normalize();

	// 将速度在法线方向进行分解
	float fSpeedNormSelf = kSpeedSelf.dot(kNorm);
	float fSpeedNormOther= kSpeedOther.dot(kNorm);

	// 其他方向如果摩擦为０，则保持不变
	// 否则，且向有个衰减
	float fSlideAttenuation = 0.8f;
	kRestSpeedSelf -= fSpeedNormSelf * kNorm;
	kRestSpeedOther-= fSpeedNormOther * kNorm;

	float fTangentSelfVelo = kRestSpeedSelf.magnitude();
	float fTangentOtherVelo= kRestSpeedOther.magnitude();

    kRestSpeedSelf *= (1.0f - fSlideAttenuation * fTangentSelfVelo / (0.5f + fTangentSelfVelo));
	kRestSpeedOther*= (1.0f - fSlideAttenuation * fTangentOtherVelo/ (0.5f + fTangentOtherVelo));

	float fMassSelf  = pSelfCar->GetMass();
	float fMassOther = pOtherCar->GetMass();

	float fSpeedSelfRet	 = 0.0f;
	float fSpeedOtherRet = 0.0f;

	// 根据动量守恒定理和能量守恒定理计算碰撞后的速度
	PhysUtility::SolveLinearGlobalCollision(fSpeedNormSelf,  fMassSelf,  fSpeedSelfRet,
											fSpeedNormOther, fMassOther, fSpeedOtherRet);


	// 得到碰撞后的速度
	 kRestSpeedSelf += fSpeedSelfRet * kNorm;
	 kRestSpeedOther+= fSpeedOtherRet* kNorm;

	 // 回设
	 pSelfCar->SetVecGlobal(kRestSpeedSelf);
	 pOtherCar->SetVecGlobal(kRestSpeedOther);


	

	 return;
	 // 下面计算车辆旋转
	 // 计算接触点的位置
	 NxBox * pSelfBox = NULL;//pSelfCar->GetCollisionBox();
	 NxBox * pOtherBox= NULL;//pOtherCar->GetCollisionBox();

	 // 弹性系数
	 float fElasticityCoef = 1.0f;
	 // 转动惯量
	 NxVec3 kInertiaTensor1;
	 NxVec3 kInertiaTensor2;
	 NxComputeBoxInertiaTensor(kInertiaTensor1, fMassSelf, pSelfBox->extents.x, pSelfBox->extents.y, pSelfBox->extents.z);
	 NxComputeBoxInertiaTensor(kInertiaTensor2, fMassOther,pOtherBox->extents.x,pOtherBox->extents.y,pOtherBox->extents.z);

	 NxPoint * pOtherPoints = new NxPoint[8];
	 if (NxComputeBoxPoints(*pOtherBox, pOtherPoints))
	 {
		 SAFE_DELETE_ARRAY(pOtherPoints);
		 return;
	 }
	 NxPoint kCollisionPoint;
	 bool bFind = false;
	 for (int i = 0; i < 8; ++i)
	 {
		 if (NxBoxContainsPoint(*pSelfBox, pOtherPoints[i])) 
		 {
			 kCollisionPoint = pOtherPoints[i];
			 bFind = true;
			 break;
		 }
	 }

	 SAFE_DELETE_ARRAY(pOtherPoints);

	 if (bFind)
	 {
		 // 非精确计算碰撞
		 NxPlane kPlane;
		 kPlane.normal = kNorm.cross(NxVec3(0, 0, 1));
		 kPlane.normal.normalize();
		 kPlane.d = kPosSelf.dot(kPlane.normal);

		 float fDistance = kPlane.distance(kCollisionPoint);

		 // 计算转动
		 float fAngularVelo1 = fDistance * fMassOther * (fSpeedOtherRet - fSpeedNormOther) / kInertiaTensor1.z * 0.01f;
		 float fAngularVelo2 = fAngularVelo1 * kInertiaTensor1.z / kInertiaTensor2.z;

		 pSelfCar->SetWecZGlobal(fAngularVelo1);
		 pOtherCar->SetWecZGlobal(fAngularVelo2);
	 }
}