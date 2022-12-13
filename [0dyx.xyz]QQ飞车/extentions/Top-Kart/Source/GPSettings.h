////////////////////////////////////////////////
//     FileName:    GPSettings.h
//     Author:      Fish
//     Date:        2007-1-22 18:05:58
//     Description: []
////////////////////////////////////////////////

#ifndef _GPSETTINGS_H_
#define _GPSETTINGS_H_

//tolua_begin
namespace GPSettings
{
	extern float nearDistance;
	extern float nearAngel;
	extern float farDistance;
	extern float farAngel;
	extern float exchangetime;
	extern float aimernode;
	extern float aimednode;
	extern float aimboardx;
	extern float aimedboardx;
	extern float aimoffsetx;
	extern float aimoffsety;
	extern float aimoffsetz;
	extern float aimedmex;
	extern float aimedmey;
	extern float aimedmez;
	extern float aimedoffsetx;
	extern float aimedoffsety;
	extern float aimedoffsetz;
	extern float aimeddistance;
	extern float aimrotspeed;
	extern float aimscalespeed;
	extern int	 gottenshownode;
	extern int	 throwanimation;
	extern float watchcameraspeed;
	extern float eatsound;
	extern int	 eatensound;
	extern float gottendelay;

	//��·�Ϸ��õľ�̬���ߺв���
	extern float	gpPropPointRadius;			//���ߵ�Ĭ�ϵ�������ײ����뾶
	extern float	gpStaticBoxFloatToGroud;	//��̬���ߺ���ʾλ������ڵ��渡��ĸ߶�
	//�������߲���
	extern float	gpMagnetAcceForce;			//�����̶���������ֱ��������ʹ�ô��������
	extern float	gpMagnetTargetModifyParam;	//��ʩ�ô�������������ĵ���ϵ��������ʹ�ô����������=gpMagnetAcceForce*gpMagnetTargetModifyParam
	extern float	gpMagnetSlackSlipFriction;	//���������ڼ�Բ���Ħ�����ļ���ϵ��
	//�������߲���
	extern float	gpMissileFloatToGround;		//�������г�ʼ״̬ʱ����ڵ��渡��ĸ߶�
	extern float	gpMissileHelixRadius;		//�������������˶�ʱ�ĳ�ʼ�뾶
	extern float	gpMissileHelixCircleCount;	//������������Ȧ��
	//���ը������
	extern float	gpFlyingFloatToGround;		///���ը�����г�ʼ״̬ʱ����ڵ��渡��ĸ߶�
	extern float	gpFlyingHelixMaxRadius;		///����ƽ���˶�������ư뾶
	extern float	gpFlyingHelixMinRadius;		///����ƽ���˶���С���ư뾶
	extern float	gpFlyingHelixSideMaxAngle;	///����ƽ�浥����ת��󻡶�
	extern float	gpFlyingHelixSideMinAngle;	///����ƽ�浥����ת��С����
	extern float	gpFlyingTurnMaxCount;		///�ܵ�ת�۷��е�������ƴ���
	extern float	gpFlyingTurnMinCount;		///�ܵ�ת�۷��е���С���ƴ���
};
//tolua_end

#endif