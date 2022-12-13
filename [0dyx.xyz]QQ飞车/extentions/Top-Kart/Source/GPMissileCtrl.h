////////////////////////////////////////////////
//FileName:    GPMissileCtrl.h
//Author:      PigHuang
//Date:        2007-01-29
//Description: 导弹道具动作控制类
////////////////////////////////////////////////

#pragma once
#include "gpdefine.h"
///导弹模型nif的原始指向，用于旋转(测试用)
const	NiPoint3	conMissileNifDirection=NiPoint3(1.0f,0.0f,0.0f);
///导弹模型在Effect里的Id，目前是直接设定的id值
const UINT conMissileModleEffectId=14;
///导弹尾烟模型在Effect里的Id，目前是直接设定的id值
const UINT conMissileSpraySmokeEffectId=30;

///导弹道具动作控制类
class CGPMissileCtrl	:	public IGPMovementControl
{
private:
	///导弹螺旋飞行参数
	typedef	struct tagHelixParam
	{
		float	m_fHelixRadius;		///导弹螺旋飞行运动半径
		float	m_fCircleCount;		///总的飞行圈数
		tagHelixParam::tagHelixParam();
	}HelixParam;
public:
	CGPMissileCtrl(void);
	virtual ~CGPMissileCtrl(void);
	//实现在接口的定义函数
	virtual void SetTarget(IGPTarget* pSrc, IGPTarget* pDest);
	virtual void SetPos(NiPoint3& kSrcPos, NiPoint3& kDestPos);
	virtual void SetParam(NiNodePtr pkMissile,DWORD dwLife);
	virtual void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
	virtual BOOL IsPlaying();

protected:
	NiPoint3	GetSrcTargetPos();
	NiPoint3	GetDestTargetPos();

protected:
	NiPoint3	m_kSrcPos;
	NiPoint3	m_kDestPos;

	bool	m_bActive;		///当前导弹显示并update中标志
	NiPoint3	m_kStartVect;	///导弹触发的坐标
	NiPoint3	m_kStartDir;	///导弹触发时的飞行指向
	NiPoint3	m_kLastVec;	///上一次update时导弹的坐标
	NiPoint3	m_kLastDir;	///上一次update时导弹的飞行指向
	IGPTarget*	m_pSrcTarget;	///发射导弹的对象
	IGPTarget*	m_pDestTarget;	///被导弹锁定的对象
	NiNodePtr	m_pkMissile;	///导弹模型
	HelixParam	m_stHelixParam;	///螺旋飞行参数
	DWORD		m_dwWholeLife;		///道具动作过程生命期(ms)
	DWORD		m_dwLastUpdateTime;	///最后一次update时间(指累计时间,<=m_dwWholeLife)
	NiNodePtr	m_pkSmoke;	///导弹拖尾烟雾粒子模型
	NiAmbientLightPtr m_spAmbientLight;
	NiPSysBoxEmitterPtr	m_spSmokeEmitter;
	NiTimeControllerPtr	m_spSmokeEmitCtr;
	float m_fInitEmitSpeed;
	float m_fCurEmitSpeed;
	float m_fInitEmitFrequence;
};
