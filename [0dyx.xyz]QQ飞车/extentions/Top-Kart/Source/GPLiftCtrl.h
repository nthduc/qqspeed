////////////////////////////////////////////////
//FileName:    GPLiftCtrl.h
//Author:      PigHuang
//Date:        2007-01-31
//Description: 车子抬起动作控制类
////////////////////////////////////////////////

#pragma once
#include "gpaffectbase.h"

///车子抬起动作控制类
class CGPLiftCtrl :
	public	CGPAffectBase
{
public:
	CGPLiftCtrl(void);
	virtual ~CGPLiftCtrl(void);
	//实现在接口的定义函数
	virtual	void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
private:
	DWORD		m_dwLiftTime;
	NiPoint3	m_kOriginPos;
};
