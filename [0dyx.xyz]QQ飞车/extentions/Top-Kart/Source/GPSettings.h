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

	//道路上放置的静态道具盒参数
	extern float	gpPropPointRadius;			//道具点默认的物理碰撞球体半径
	extern float	gpStaticBoxFloatToGroud;	//静态道具盒显示位置相对于地面浮起的高度
	//磁铁道具参数
	extern float	gpMagnetAcceForce;			//磁铁固定加速力，直接作用于使用磁铁的玩家
	extern float	gpMagnetTargetModifyParam;	//被施用磁铁玩家所受力的调整系数，即被使用磁铁玩家受力=gpMagnetAcceForce*gpMagnetTargetModifyParam
	extern float	gpMagnetSlackSlipFriction;	//磁铁作用期间对侧向摩擦力的减弱系数
	//导弹道具参数
	extern float	gpMissileFloatToGround;		//导弹飞行初始状态时相对于地面浮起的高度
	extern float	gpMissileHelixRadius;		//导弹螺旋飞行运动时的初始半径
	extern float	gpMissileHelixCircleCount;	//弹螺旋飞行总圈数
	//酷比炸弹参数
	extern float	gpFlyingFloatToGround;		///酷比炸弹飞行初始状态时相对于地面浮起的高度
	extern float	gpFlyingHelixMaxRadius;		///螺旋平面运动最大限制半径
	extern float	gpFlyingHelixMinRadius;		///螺旋平面运动最小限制半径
	extern float	gpFlyingHelixSideMaxAngle;	///螺旋平面单侧旋转最大弧度
	extern float	gpFlyingHelixSideMinAngle;	///螺旋平面单侧旋转最小弧度
	extern float	gpFlyingTurnMaxCount;		///总的转折飞行的最大限制次数
	extern float	gpFlyingTurnMinCount;		///总的转折飞行的最小限制次数
};
//tolua_end

#endif