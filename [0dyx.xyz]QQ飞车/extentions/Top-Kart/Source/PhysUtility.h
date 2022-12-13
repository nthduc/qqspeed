/********************************************************************
	作者:	　wincoqin
	时间:	 2007/01/25
	目的:	 物理库
	说明:    如非特别指出，所有单位均为国际制单位
*********************************************************************/


#pragma once

#include "NxMath.h"
#include "NxVec3.h"


class PhysUtility
{
public:
	// 作用：计算任意两条直线间距离
	// 输入：l1（直线１方向）,pos1(直线１上任意一点），　l2（直线2方向）,pos2(直线2上任意一点)
	// 返回：返回２条直线距离
	static float GetDistanceBetweenLine(const NxVec3 & l1, const NxVec3 & pos1, const NxVec3 & l2, const NxVec3 & pos2);
	// 计算速度
	// 输入：vel（当前速度），　accel( 当前加速度），　fDeltaTime时间间隔
	static NxVec3 CaculateVelocity(const NxVec3 & vel, const NxVec3 & accel, const float fDeltaTime);
	// 作用：求解线形碰撞
	// 输入：碰撞前两物体速度fSpeed1, fSpeed2和质量fMass1, fMass2, 返回碰撞后速度fSpeedRet1和fSpeedRet2
	// 说明：求解为完全弹性碰撞
	static void SolveLinearGlobalCollision(const float fSpeed1, const float fMass1, float & fSpeedRet1,
		const float fSpeed2, const float fMass2, float & fSpeedRet2);

	// 解决二个矩形碰撞检测
	static bool RectCollision(const NxVec3& extents0, const NxVec3& center0, const NxMat33& rotation0,
		const NxVec3& extents1, const NxVec3& center1, const NxMat33& rotation1);

};