/********************************************************************
	����:	��wincoqin
	ʱ��:	  2007/01/30
    �ļ���:   PhysUtility.cpp
	Ŀ��:   	  �����
*********************************************************************/
#include "Top-KartPCH.h"
// libfile
#include ".\physutility.h"

// userfile
#include "NxIntersectionBoxBox.h"


float PhysUtility::GetDistanceBetweenLine(const NxVec3 & l1, const NxVec3 & pos1, 
										  const NxVec3 & l2, const NxVec3 & pos2)
{
	// �������������������ķ��߷���
	NxVec3 norm = l1.cross(l2); 
	// �����������ϵ�һ��������ͶӰ����������õ�����ֱ�ߵľ���
	float fDistance = pos1.dot(norm) - pos2.dot(norm);

	return fDistance;
}
NxVec3 PhysUtility::CaculateVelocity(const NxVec3 & vel, const NxVec3 & accel, const float fDeltaTime)
{
	NxVec3 velRet;
	velRet.x = (vel.x+ 0.5f * accel.x * fDeltaTime) * fDeltaTime;
	velRet.y = (vel.y+ 0.5f * accel.y * fDeltaTime) * fDeltaTime;
	velRet.z = (vel.z+ 0.5f * accel.z * fDeltaTime) * fDeltaTime;

	return velRet;
}

void PhysUtility::SolveLinearGlobalCollision(const float fSpeed1, const float fMass1, float & fSpeedRet1, 
											 const float fSpeed2, const float fMass2, float & fSpeedRet2)
{
	float fInverseTotalMass = 1.f / (fMass1 + fMass2);
	float fDeltaMass = fMass1 - fMass2;
	
	fSpeedRet1 = (2.f * fMass2 * fSpeed2 + fDeltaMass * fSpeed1) * fInverseTotalMass;
	fSpeedRet2 = (2.f * fMass1 * fSpeed1 - fDeltaMass * fSpeed2) * fInverseTotalMass;		    
}

// Zֵû�з�����ײ��ֻ��ƽ���
bool PhysUtility::RectCollision(const NxVec3& extents0, const NxVec3& center0, const NxMat33& rotation0,
								const NxVec3& extents1, const NxVec3& center1, const NxMat33& rotation1)
{

	return NxBoxBoxIntersect(extents0, center0, rotation0, extents1, center1, rotation1, true);
}