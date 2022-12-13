////////////////////////////////////////////////
//FileName:    GPMagnetCtrl.h
//Author:      PigHuang
//Date:        2007-01-30
//Description: 磁铁道具动作控制类
////////////////////////////////////////////////

#pragma once
#include "gpaffectbase.h"


///磁铁道具动作控制类
class CGPMagnetCtrl :
	public	CGPAffectBase
{
public:
	CGPMagnetCtrl(void);
	virtual ~CGPMagnetCtrl(void);
	//实现在接口的定义函数
	virtual	void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
private:
	NxVec3	m_kStartVect;	///磁铁触发时本地玩家车子的坐标
	NxVec3	m_kLastVec;		///上一次update时车子的坐标
	DWORD	m_dwWholeLife;		///道具动作过程生命期(ms)
	DWORD	m_dwLastUpdateTime;	///最后一次update时间(指累计时间,<=m_dwWholeLife)
	float	m_fSaveSlipFriction;	///保存特效前的侧向摩擦力
};
