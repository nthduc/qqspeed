/********************************************************************
	����:	��wincoqin
	ʱ��:	  2007/01/27
    �ļ���:   CollisionBase.h
	Ŀ��:   	
*********************************************************************/
#pragma once

// libfile

// userfile
#include "NiPhysXManager.h"
// predefine
class CCollisionCenter;

/*
typedef enum enmCollisonObjectType
{
	ECOT_CAR_GROUND = 1 << 0,// ����
	ECOT_CAR_FENCE  = 1 << 1, // ��� 
	ECOT_CAR_DOOR	= 1 << 2,  // ���
	ECOT_CAR_GP01	= 1 << 3,  // ����
	ECOT_CAR_BOX	= 1 << 4,	// ����
	ECOT_CAR_CAR	= 1 << 5,   // ��
	ECOT_MAX

}ENMCOT;
*/

//  ���ã�  �ж�ĳ��ײ�����Ƿ���ָ������ײ����
//  ����˵����enmCollisionTypeΪ�ƶ�����ײ���ͣ� dwCollisionΪ���жϵ�����
//  ����ֵ��true��ʾ����ָ�����ͣ����򲻺�
inline bool IsKindOfCollisionType(XUserData::ACTORTYPE enmCollisionType, DWORD dwCollision)
{
	return ((dwCollision & ((DWORD)(enmCollisionType))) != 0);
}

typedef struct tagCollisionInfo
{
	NxShape * 	collisionShape;		// ����ײ����
	NxExtendedVec3	worldPos;		// ��ײ���λ�ã��������꣩
	NxVec3			worldNormal;	// ��ײ��ķ��ߣ��������꣩
	unsigned int	unId;			// Reserved
	NxVec3			dir;			// ��ײ����
	float			fLength;		// ��ײ����
}CollisionInfo;

class ICollisionCallback
{
public:
	virtual void OnTrigger(void * pSelf, void * pOtherInfo, const DWORD dwCollisionType) = 0;
	virtual DWORD GetCallbackType() = 0;
};

class CCollisionBase
	: public ICollisionCallback
{
	typedef CCollisionCenter Father;
public:
	CCollisionBase(CCollisionCenter * pCenter, const DWORD dwCollisionType);	
	virtual ~CCollisionBase();

	virtual void OnTrigger(void * pSelf, void * pOtherInfo, const DWORD dwCollisionType) = 0;
	inline virtual DWORD GetCallbackType()
	{
		return m_dwCollisionType;
	}

protected:
	Father * m_pFather;
	const DWORD m_dwCollisionType;
};
