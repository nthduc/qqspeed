/////////////////////////////
///���ߵ�ص�������
/////////////////////////////
#pragma once
class CGamePropCollisionReport
{
public:
	///lpGPData=���ߵ�������,lpHitObjData=WheellessCar��ָ��
	virtual int OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData) = 0;
};
