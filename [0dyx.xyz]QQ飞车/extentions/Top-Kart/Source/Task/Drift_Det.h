////////////////////////////////////////////////
//     FileName:    Drift_Det.h
//     Author:      rayhuang
//     Date:        2007-6-13
//     Description: ���ֽ�ѧ���ٶȣ�Ư�ƣ�С��ȼ����
////////////////////////////////////////////////


#pragma once 
#include "DetectorBase.h"
#include "NiInputKeyboard.h"

//�ٶȼ��
class CSpeed_Det : public CDetectorBase
{
public:
	CSpeed_Det();
	~CSpeed_Det();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
private:
	float m_fTargetSpeed;		//����Ŀ��ֵ
};


//����Ƿ�Ư�Ʋ����ٶ���ĳһֵ֮��
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


//����Ƿ�ذᳵͷ���ҵ�ʱ����Ư��״̬
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


//���С���Ƿ��ڼ���״̬
class CSmallNo2_Det : public CDetectorBase
{
public:
	CSmallNo2_Det();
	~CSmallNo2_Det();
	void Update(float fTime);
private:
	bool _IsStartNo2();
};
