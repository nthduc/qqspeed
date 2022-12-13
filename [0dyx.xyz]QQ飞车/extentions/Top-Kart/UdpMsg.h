#pragma once 
#include "NxVec3.h"
#define MAX_KEY_NUM			108
#define UDP_MSG_PLAYMOVE   20000

struct STUdpPack
{
	UINT MsgID;
	void * pUdpData;
};

class CUdpPackagePlayMove
{
	UINT unUin;		//源玩家uin
	UCHAR bHasKeyChange;  //有按键变化没有
	UCHAR bArrKey[MAX_KEY_NUM];		//按键状态（在有按键变化时发挥作用）
	NxVec3 m_stLinearVelocity[3];		//本地玩家赛车线速度
	NxVec3 m_stAngularVelocity;		//本地玩家赛车角速度
	NxVec3 m_stArrPose[4];			//本地玩家赛车姿态（包括旋转矩阵和位置）
	UINT unPackageIndex;	//包序号
	UINT unTimeStamp;		//时间戳
};