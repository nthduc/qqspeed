////////////////////////////////////////////////
//     FileName:    CDTime_Det.h
//     Author:      rayhuang
//     Date:        2007-5-21
//     Description: µ¹¼ÆÊ±¼ì²âÆ÷
////////////////////////////////////////////////

#pragma once 
#include "DetectorBase.h"

class CCDTime_Det : public CDetectorBase
{
public:
	CCDTime_Det();
	~CCDTime_Det();
	void Start();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
	
private:
	DWORD m_dwStartTime;
	DWORD m_dwLife;
};