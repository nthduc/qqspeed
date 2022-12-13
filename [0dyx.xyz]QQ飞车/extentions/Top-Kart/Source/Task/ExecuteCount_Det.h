////////////////////////////////////////////////
//     FileName:    Timer_Det.h
//     Author:      rayhuang
//     Date:        2007-5-31
//     Description: ִ�д��������
////////////////////////////////////////////////

#pragma once 
#include "DetectorBase.h"

class CExecuteCout_Det : public CDetectorBase
{
public:
	CExecuteCout_Det();
	~CExecuteCout_Det();
	void Start();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
public:
	DWORD m_dwLife;
	DWORD m_dwCurCount;
};
