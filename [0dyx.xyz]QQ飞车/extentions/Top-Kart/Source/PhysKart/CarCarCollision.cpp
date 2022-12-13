/********************************************************************
	����:	��wincoqin
	ʱ��:	  2007/01/27
    �ļ���:   CarCarCollision.cpp
	Ŀ��:     Implementation	
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
	LogText("�������淢���˳�����ײ\n");

	CTencentCar * pSelfCar = pSelf->GetKart()->GetVehicle();
	CTencentCar * pOtherCar = pOther->GetKart()->GetVehicle();

	//TODO: ���ӵĵ�self��other car�ķ���
	NxVec3 kSpeedSelf  = pSelfCar->GetVecGlobal();
	NxVec3 kSpeedOther = pOtherCar->GetVecGlobal();

	NxVec3 kRestSpeedSelf = kSpeedSelf;
	NxVec3 kRestSpeedOther = kSpeedOther;
	// �Ȱ���ײ��������ײ������
	/*
	NxBox * pSelfBox  = pSelfCar->GetCollisionBox();
	NxBox * pOtherBox = pOtherCar->GetCollisionBox();

	NxUtilLib::NxBoxBoxIntersect()
	*/

	NxVec3 kPosSelf = pSelfCar->GetPosGlobal();
	NxVec3 kPosOther = pOtherCar->GetPosGlobal();

	// ȷ����ײ�ķ��߷���
	NxVec3 kNorm = kPosOther - kPosSelf;
	kNorm.normalize();

	// ���ٶ��ڷ��߷�����зֽ�
	float fSpeedNormSelf = kSpeedSelf.dot(kNorm);
	float fSpeedNormOther= kSpeedOther.dot(kNorm);

	// �����������Ħ��Ϊ�����򱣳ֲ���
	// ���������и�˥��
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

	// ���ݶ����غ㶨��������غ㶨�������ײ����ٶ�
	PhysUtility::SolveLinearGlobalCollision(fSpeedNormSelf,  fMassSelf,  fSpeedSelfRet,
											fSpeedNormOther, fMassOther, fSpeedOtherRet);


	// �õ���ײ����ٶ�
	 kRestSpeedSelf += fSpeedSelfRet * kNorm;
	 kRestSpeedOther+= fSpeedOtherRet* kNorm;

	 // ����
	 pSelfCar->SetVecGlobal(kRestSpeedSelf);
	 pOtherCar->SetVecGlobal(kRestSpeedOther);


	

	 return;
	 // ������㳵����ת
	 // ����Ӵ����λ��
	 NxBox * pSelfBox = NULL;//pSelfCar->GetCollisionBox();
	 NxBox * pOtherBox= NULL;//pOtherCar->GetCollisionBox();

	 // ����ϵ��
	 float fElasticityCoef = 1.0f;
	 // ת������
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
		 // �Ǿ�ȷ������ײ
		 NxPlane kPlane;
		 kPlane.normal = kNorm.cross(NxVec3(0, 0, 1));
		 kPlane.normal.normalize();
		 kPlane.d = kPosSelf.dot(kPlane.normal);

		 float fDistance = kPlane.distance(kCollisionPoint);

		 // ����ת��
		 float fAngularVelo1 = fDistance * fMassOther * (fSpeedOtherRet - fSpeedNormOther) / kInertiaTensor1.z * 0.01f;
		 float fAngularVelo2 = fAngularVelo1 * kInertiaTensor1.z / kInertiaTensor2.z;

		 pSelfCar->SetWecZGlobal(fAngularVelo1);
		 pOtherCar->SetWecZGlobal(fAngularVelo2);
	 }
}