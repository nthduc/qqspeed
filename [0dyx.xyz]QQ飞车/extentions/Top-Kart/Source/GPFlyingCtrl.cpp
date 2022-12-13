#include "Top-KartPCH.h"
#include "common.h"
#include "gpflyingctrl.h"
#include "gpeffect.h"
#include "NiPSysEmitterCtlr.h"
#include "gpsettings.h"
#include "Swoosh.h"

CGPFlyingCtrl::tagHelixParam::tagHelixParam()
{
	m_fHelixMaxRadius=0.0f;
	m_fHelixMinRadius=0.0f;
	m_fHelixSideMaxAngle=0.0f;
	m_fHelixSideMinAngle=0.0f;
	m_fTurnMaxCount=0.0f;
	m_fTurnMinCount=0.0f;
	m_fTurnCount=0.0f;
	m_fNowTurnCount=0.0f;
	m_fLastTurnCount=0.0f;
	m_fLastTurnAngle=0.0f;
	m_kNowDestVec=NiPoint3::ZERO;
	m_kLastTurnVec=NiPoint3::ZERO;
}

CGPFlyingCtrl::CGPFlyingCtrl(void):
m_pkFlying(0),
m_pkSwoosh(0),
m_bActive(false)
{
}

CGPFlyingCtrl::~CGPFlyingCtrl(void)
{
	m_pkFlying=0;
	m_pkSmoke=0;
	if(m_pkSwoosh)
	{
		SAFE_NIDELETE(m_pkSwoosh);
	}
}

void CGPFlyingCtrl::SetTarget(IGPTarget* pSrc, IGPTarget* pDest)
{
	_ASSERTE(pSrc && pDest);
	m_pSrcTarget=pSrc;
	m_pDestTarget=pDest;
}

void CGPFlyingCtrl::SetPos(NiPoint3& kSrcPos, NiPoint3& kDestPos)
{

}

void	CGPFlyingCtrl::SetParam(NiNodePtr pkFlying,DWORD dwLife)
{
	m_pkFlying=pkFlying;
	m_dwWholeLife=dwLife;
}

void CGPFlyingCtrl::Update(DWORD dwLifeAcc)
{
	_ASSERTE(dwLifeAcc>=m_dwLastUpdateTime);
	_ASSERTE(m_dwWholeLife>=dwLifeAcc);
	if (!m_bActive || dwLifeAcc==m_dwLastUpdateTime)
	{//对象不处理
		return;
	}
	//原理:
	//移动方向=上一次update坐标(指向)目标车坐标，实时坐标
	//移动距离=上一次update时运动速度(模值) * (上一次update到本次update经历的时间)
	//获取酷比炸弹飞行指向
	NiPoint3 kDestVec = m_pDestTarget->GetPos();
	NiPoint3 kTmpVec;
	NiPoint3 kDirVec;
	NiMatrix3	kMat3;
	float fTmp,fTimeScale,fDistance;
	fTimeScale=((float)dwLifeAcc)/((float)m_dwWholeLife);
	//确定当前飞行所处于的转折数
	m_stHelixParam.m_fNowTurnCount=::ceil(fTimeScale*m_stHelixParam.m_fTurnCount);
	if ((m_stHelixParam.m_fNowTurnCount-m_stHelixParam.m_fLastTurnCount)>0.1f ||
		m_stHelixParam.m_fLastTurnCount<0.001f)
	{//目标点需要更新
		//算出目标点弧度
		UINT nRandTmp=rand();
		//fTmp=m_stHelixParam.m_fHelixSideMinAngle+
		//	(m_stHelixParam.m_fHelixSideMaxAngle-m_stHelixParam.m_fHelixSideMinAngle)*
		//	((float)nRandTmp/(float)RAND_MAX);
		fTmp=m_stHelixParam.m_fHelixSideMinAngle+
			(m_stHelixParam.m_fHelixSideMaxAngle-m_stHelixParam.m_fHelixSideMinAngle)*
			((float)nRandTmp/(float)RAND_MAX);
		if (m_stHelixParam.m_fLastTurnCount<0.001f)
		{//第一次
			if(0==(nRandTmp%2))
			{//随机方向
				m_stHelixParam.m_fLastTurnAngle = (-1)*fTmp;
			}
			else
			{
				m_stHelixParam.m_fLastTurnAngle	= fTmp;
			}
		}
		else
		{//取反
			if(m_stHelixParam.m_fLastTurnAngle<0.0f)
			{
				m_stHelixParam.m_fLastTurnAngle= fTmp;
			}
			else
			{
				m_stHelixParam.m_fLastTurnAngle= (-1)*fTmp;
			}
		}
		fTmp=m_stHelixParam.m_fTurnCount-m_stHelixParam.m_fNowTurnCount;
		if (fTmp<0.99f)
		{//最后一次转折,直接飞向目标车

			m_stHelixParam.m_kNowDestVec = kDestVec;
			//更新主轴最新坐标
			m_stHelixParam.m_kLastTurnVec=m_stHelixParam.m_kNowDestVec;
		}
		else
		{//计算飞向的预定目标点
			//算出当前每一转折飞行旅程的时间
			fTmp=((float)(m_stHelixParam.m_fNowTurnCount-m_stHelixParam.m_fLastTurnCount))/
				((float)(m_stHelixParam.m_fTurnCount-m_stHelixParam.m_fLastTurnCount));
			kTmpVec=kDestVec-m_stHelixParam.m_kLastTurnVec;
			fDistance=kTmpVec.Length()*fTmp;
			NiPoint3::UnitizeVector(kTmpVec);
			kDirVec=kTmpVec;
			m_stHelixParam.m_kNowDestVec=kTmpVec*fDistance;
			//更新主轴最新坐标
			m_stHelixParam.m_kLastTurnVec += m_stHelixParam.m_kNowDestVec;
			//开始算绕飞行主轴螺旋线运动的坐标位移
			fTmp=fabsf(kDirVec.z);
			if (fTmp>0.0f && fTmp<1.0f)
			{
				kTmpVec=kTmpVec.Cross(NiPoint3::UNIT_Z);
				NiPoint3::UnitizeVector(kTmpVec);
				kTmpVec=kTmpVec.Cross(kDirVec);
				NiPoint3::UnitizeVector(kTmpVec);
			}
			//半径随机缩放比例
			fTmp=m_stHelixParam.m_fHelixMinRadius+
				(m_stHelixParam.m_fHelixMaxRadius-m_stHelixParam.m_fHelixMinRadius)*
				((float)nRandTmp/(float)RAND_MAX);
			kTmpVec *= fTmp; //螺旋线半径
			//最后算出移动的目标点世界坐标
			kMat3=NiMatrix3::IDENTITY;
			kMat3.MakeRotation(m_stHelixParam.m_fLastTurnAngle,kDirVec);
			kTmpVec=kMat3*kTmpVec;
			m_stHelixParam.m_kNowDestVec=kTmpVec + m_stHelixParam.m_kLastTurnVec;
		}
		m_stHelixParam.m_fLastTurnCount=m_stHelixParam.m_fNowTurnCount;
	}

	//开始算实际飞行运动的坐标位移
	kTmpVec = m_stHelixParam.m_kNowDestVec - m_kLastVec;
	NiPoint3::UnitizeVector(kTmpVec);
	kDirVec=kTmpVec;
	fTmp=m_stHelixParam.m_fNowTurnCount-(fTimeScale*m_stHelixParam.m_fTurnCount);
	_ASSERTE(fTmp>=0.0f);
	kTmpVec *= fTmp;
	//更新最新最终酷比炸弹世界坐标
	m_kLastVec += kTmpVec;
	//开始处理酷比炸弹模型旋转，使模型总指向飞行轨迹
	/*
	kTmpVec=conFlyingNifDirection;
	fTmp=-acos(kTmpVec.Dot(kDirVec));
	kTmpVec=kTmpVec.Cross(kDirVec);
	NiPoint3::UnitizeVector(kTmpVec);
	kMat3.MakeRotation(fTmp,kTmpVec);
	*/
	kMat3.MakeRotation(m_stHelixParam.m_fLastTurnAngle,kTmpVec);
	m_pkFlying->SetRotate(kMat3);
	m_pkFlying->SetTranslate(m_kLastVec.x,m_kLastVec.y,m_kLastVec.z);
	//残影
	if (m_pkSwoosh)
	{
		m_pkSwoosh->TakeSample(dwLifeAcc);
	}
	//尾烟
	m_pkSmoke->SetTranslate(m_kLastVec.x,m_kLastVec.y,m_kLastVec.z);
	//更新参数
	m_dwLastUpdateTime=dwLifeAcc;
}	

void CGPFlyingCtrl::Start()
{
	//设定初始值
	float fTmp=0.0f;
	m_kStartVect = m_pSrcTarget->GetPos();
	NiPoint3 kDirVec=m_pDestTarget->GetPos();
	kDirVec = kDirVec-m_kStartVect;
	NiPoint3::UnitizeVector(kDirVec);
	m_kLastVec=m_kStartVect;
	//直接在z轴上加一个浮起高度作为酷比炸弹初始坐标
	m_kLastVec.z += GPSettings::gpFlyingFloatToGround;
	//设置飞行原始值
	m_stHelixParam.m_kLastTurnVec=m_kLastVec;
	m_dwLastUpdateTime=0;
	//装载飞行轨迹初始值
	m_stHelixParam.m_fHelixMaxRadius=GPSettings::gpFlyingHelixMaxRadius;
	m_stHelixParam.m_fHelixMinRadius=GPSettings::gpFlyingHelixMinRadius;
	_ASSERTE(m_stHelixParam.m_fHelixMinRadius>0.05f &&
		m_stHelixParam.m_fHelixMaxRadius>=m_stHelixParam.m_fHelixMinRadius);
	m_stHelixParam.m_fHelixSideMaxAngle=GPSettings::gpFlyingHelixSideMaxAngle;
	m_stHelixParam.m_fHelixSideMinAngle=GPSettings::gpFlyingHelixSideMinAngle;
	_ASSERTE(m_stHelixParam.m_fHelixSideMinAngle>0.05f &&
		m_stHelixParam.m_fHelixSideMaxAngle>=m_stHelixParam.m_fHelixSideMinAngle);
	m_stHelixParam.m_fTurnMaxCount=GPSettings::gpFlyingTurnMaxCount;
	m_stHelixParam.m_fTurnMinCount=GPSettings::gpFlyingTurnMinCount;
	_ASSERTE(m_stHelixParam.m_fTurnMinCount>=2.0f &&
		m_stHelixParam.m_fTurnMaxCount>=m_stHelixParam.m_fTurnMinCount);
	//生成本次飞行的总转折数
	m_stHelixParam.m_fTurnCount=::ceil(m_stHelixParam.m_fTurnMinCount+
		(m_stHelixParam.m_fTurnMaxCount-m_stHelixParam.m_fTurnMinCount)*
		((float)rand()/(float)RAND_MAX));
	//把酷比炸弹模型旋转到飞行方向，原理看update函数说明
	NiMatrix3	kMat3;
	NiPoint3 kTmpVec=conFlyingNifDirection;
	float fAngle=kTmpVec.Dot(kDirVec);
	fAngle= -acos(fAngle);
	kTmpVec=kTmpVec.Cross(kDirVec);
	NiPoint3::UnitizeVector(kTmpVec);
	kMat3.MakeRotation(fAngle,kTmpVec);
	CLevelManager::Get()->AttachObject(m_pkFlying);
	m_pkFlying->SetRotate(kMat3);
	m_pkFlying->SetTranslate(m_kLastVec.x,m_kLastVec.y,m_kLastVec.z);
	m_bActive=true;
	////////////////////
	//绑定swoosh
	//todo:先屏蔽swoosh
	/*m_pkSwoosh = NiNew Swoosh(m_pkFlying,0.0f);
	m_pkSwoosh->SetActivity(true);*/

	//尾烟
	m_pkSmoke=CGPEffectMgr::Get()->NewEffect(conFlyingSpraySmokeEffectId,TRUE);
	CLevelManager::Get()->AttachObject(m_pkSmoke);
}

void CGPFlyingCtrl::Stop()
{
	//取消显示绑定
	CLevelManager::Get()->DetachObject(m_pkFlying);
	CLevelManager::Get()->DetachObject(m_pkSmoke);
	m_bActive=false;
}

BOOL CGPFlyingCtrl::IsPlaying()
{
	return (BOOL)m_bActive;
}