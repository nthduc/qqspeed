/********************************************************************
	����:	��wincoqin
	ʱ��:	 2007/01/25
	Ŀ��:	 �����
	˵��:    ����ر�ָ�������е�λ��Ϊ�����Ƶ�λ
*********************************************************************/


#pragma once

#include "NxMath.h"
#include "NxVec3.h"


class PhysUtility
{
public:
	// ���ã�������������ֱ�߼����
	// ���룺l1��ֱ�ߣ�����,pos1(ֱ�ߣ�������һ�㣩����l2��ֱ��2����,pos2(ֱ��2������һ��)
	// ���أ����أ���ֱ�߾���
	static float GetDistanceBetweenLine(const NxVec3 & l1, const NxVec3 & pos1, const NxVec3 & l2, const NxVec3 & pos2);
	// �����ٶ�
	// ���룺vel����ǰ�ٶȣ�����accel( ��ǰ���ٶȣ�����fDeltaTimeʱ����
	static NxVec3 CaculateVelocity(const NxVec3 & vel, const NxVec3 & accel, const float fDeltaTime);
	// ���ã����������ײ
	// ���룺��ײǰ�������ٶ�fSpeed1, fSpeed2������fMass1, fMass2, ������ײ���ٶ�fSpeedRet1��fSpeedRet2
	// ˵�������Ϊ��ȫ������ײ
	static void SolveLinearGlobalCollision(const float fSpeed1, const float fMass1, float & fSpeedRet1,
		const float fSpeed2, const float fMass2, float & fSpeedRet2);

	// �������������ײ���
	static bool RectCollision(const NxVec3& extents0, const NxVec3& center0, const NxMat33& rotation0,
		const NxVec3& extents1, const NxVec3& center1, const NxMat33& rotation1);

};