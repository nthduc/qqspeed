/////////////////////////////
///道具点回调函数类
/////////////////////////////
#pragma once
class CGamePropCollisionReport
{
public:
	///lpGPData=道具点索引号,lpHitObjData=WheellessCar类指针
	virtual int OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData) = 0;
};
