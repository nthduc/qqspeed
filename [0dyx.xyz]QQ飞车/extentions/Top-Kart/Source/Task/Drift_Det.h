////////////////////////////////////////////////
//     FileName:    Drift_Det.h
//     Author:      rayhuang
//     Date:        2007-6-13
//     Description: 新手教学中速度，漂移，小喷等检测器
////////////////////////////////////////////////


#pragma once 
#include "DetectorBase.h"
#include "NiInputKeyboard.h"

//速度检测
class CSpeed_Det : public CDetectorBase
{
public:
	CSpeed_Det();
	~CSpeed_Det();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
private:
	float m_fTargetSpeed;		//检测的目标值
};


//检测是否漂移并且速度在某一值之上
class CDrift_Speed_Det : public CDetectorBase
{
public:
	CDrift_Speed_Det();
	~CDrift_Speed_Det();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
private:
	float m_fTargetSpeed;
	NiInputKeyboard::KeyCode m_arrKey[2];
};


//检测是否回搬车头并且当时处于漂移状态
class CTurnCar_Drift_Det : public CDetectorBase
{
public:
	CTurnCar_Drift_Det();
	~CTurnCar_Drift_Det();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
private:
	NiInputKeyboard::KeyCode m_arrKey[2];
};


//检测小喷是否处于激发状态
class CSmallNo2_Det : public CDetectorBase
{
public:
	CSmallNo2_Det();
	~CSmallNo2_Det();
	void Update(float fTime);
private:
	bool _IsStartNo2();
};
