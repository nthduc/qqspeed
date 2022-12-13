////////////////////////////////////////////////
//FileName:    GPFlyingCtrl.h
//Author:      PigHuang
//Date:        2007-02-09
//Description: 酷比炸弹道具动作控制类
////////////////////////////////////////////////

#pragma once
#include "gpdefine.h"
///酷比炸弹模型nif的原始指向，用于旋转(测试用)
const	NiPoint3	conFlyingNifDirection=NiPoint3(1.0f,0.0f,0.0f);
///酷比炸弹模型在Effect里的Id，目前是直接设定的id值
const UINT conFlyingModleEffectId=12;
///todo:酷比炸弹尾烟模型在Effect里的Id，!!!!目前是直接使用导弹尾烟的id值
const UINT conFlyingSpraySmokeEffectId=30;

//残影特效
class Swoosh;

///酷比炸弹道具动作控制类
class CGPFlyingCtrl	:	public IGPMovementControl
{
private:
	///酷比炸弹转折飞行参数
	typedef	struct tagHelixParam
	{
		float	m_fHelixMaxRadius;		///酷比炸弹螺旋平面运动最大限制半径
		float	m_fHelixMinRadius;		///酷比炸弹螺旋平面运动最小限制半径
		float	m_fHelixSideMaxAngle;		///酷比炸弹螺旋平面单侧旋转最大弧度
		float	m_fHelixSideMinAngle;		///酷比炸弹螺旋平面单侧旋转最小弧度
		float	m_fTurnMaxCount;		///总的转折飞行的最大限制次数
		float	m_fTurnMinCount;		///总的转折飞行的最小限制次数
		float	m_fTurnCount;		///总的转折飞行的次数,Start时确定
		float	m_fNowTurnCount;	///当前所处的转折飞行的次数,初始飞行时是0.0f
		float	m_fLastTurnCount;	///当前所处的转折飞行的次数,初始飞行时是0.0f
		float	m_fLastTurnAngle;	///当前转折飞行目标点的主轴旋转弧度
		NiPoint3	m_kNowDestVec;	///当前酷比炸弹确定的本次转折飞行的最终坐标
		NiPoint3	m_kLastTurnVec;	///上一次update时酷比炸弹确定的本次转折飞行的主轴坐标
		tagHelixParam::tagHelixParam();
	}HelixParam;
public:
	CGPFlyingCtrl(void);
	virtual ~CGPFlyingCtrl(void);
	//实现在接口的定义函数
	virtual void SetTarget(IGPTarget* pSrc, IGPTarget* pDest);
	virtual void SetPos(NiPoint3& kSrcPos, NiPoint3& kDestPos);
	virtual void SetParam(NiNodePtr pkFlying,DWORD dwLife);
	virtual void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
	virtual BOOL IsPlaying();
protected:
	bool	m_bActive;		///当前酷比炸弹显示并update中标志
	NiPoint3	m_kStartVect;	///酷比炸弹触发的坐标
	NiPoint3	m_kLastVec;	///上一次update时酷比炸弹的坐标
	IGPTarget*	m_pSrcTarget;	///发射酷比炸弹的对象
	IGPTarget*	m_pDestTarget;	///被酷比炸弹锁定的对象
	NiNodePtr	m_pkFlying;	///酷比炸弹模型
	HelixParam	m_stHelixParam;	///螺旋飞行参数
	DWORD		m_dwWholeLife;		///道具动作过程生命期(ms)
	DWORD		m_dwLastUpdateTime;	///最后一次update时间(指累计时间,<=m_dwWholeLife)
	NiNodePtr	m_pkSmoke;	///酷比炸弹拖尾烟雾粒子模型
	//加入残影特效
	Swoosh* m_pkSwoosh;
};
