/********************************************************************
	作者:	　wincoqin
	时间:	  2007/01/27
    文件名:   CollisionBase.h
	目的:   	
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
	ECOT_CAR_GROUND = 1 << 0,// 地面
	ECOT_CAR_FENCE  = 1 << 1, // 篱笆 
	ECOT_CAR_DOOR	= 1 << 2,  // 活动门
	ECOT_CAR_GP01	= 1 << 3,  // 道具
	ECOT_CAR_BOX	= 1 << 4,	// 箱子
	ECOT_CAR_CAR	= 1 << 5,   // 车
	ECOT_MAX

}ENMCOT;
*/

//  作用：  判断某碰撞类型是否含有指定的碰撞类型
//  参数说明：enmCollisionType为制定的碰撞类型， dwCollision为待判断的类型
//  返回值：true表示含有指定类型，否则不含
inline bool IsKindOfCollisionType(XUserData::ACTORTYPE enmCollisionType, DWORD dwCollision)
{
	return ((dwCollision & ((DWORD)(enmCollisionType))) != 0);
}

typedef struct tagCollisionInfo
{
	NxShape * 	collisionShape;		// 被碰撞对象
	NxExtendedVec3	worldPos;		// 碰撞点的位置（世界坐标）
	NxVec3			worldNormal;	// 碰撞点的法线（世界坐标）
	unsigned int	unId;			// Reserved
	NxVec3			dir;			// 碰撞方向
	float			fLength;		// 碰撞距离
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
