////////////////////////////////////////////////
//     FileName:    Timer_Det.h
//     Author:      rayhuang
//     Date:        2007-5-28
//     Description: ��ʱ�������
////////////////////////////////////////////////


#pragma once 
#include "DetectorBase.h"


class CTimer_Det : public CDetectorBase
{
public: 
	CTimer_Det();
	~CTimer_Det();
	void Start();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
private:
	DWORD m_dwStartTime;
	DWORD m_dwLife;
};