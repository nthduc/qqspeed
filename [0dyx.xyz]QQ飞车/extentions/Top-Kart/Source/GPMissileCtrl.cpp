#include "Top-KartPCH.h"
#include "common.h"
#include "gpmissilectrl.h"
#include "gpeffect.h"
#include "NiPSysEmitterCtlr.h"
#include "gpsettings.h"
#include <NiPSysSpawnModifier.h>
#include <NiPSysPositionModifier.h>


CGPMissileCtrl::tagHelixParam::tagHelixParam()
{
	m_fHelixRadius=0.0f;
	m_fCircleCount=0.0f;	
}

CGPMissileCtrl::CGPMissileCtrl(void):
m_pkMissile(0),
m_bActive(false)
{
}

CGPMissileCtrl::~CGPMissileCtrl(void)
{
	m_pkMissile=0;
	m_pkSmoke=0;
}

void CGPMissileCtrl::SetTarget(IGPTarget* pSrc, IGPTarget* pDest)
{
	_ASSERTE(pSrc && pDest);
	m_pSrcTarget=pSrc;
	m_pDestTarget=pDest;
}

void CGPMissileCtrl::SetPos(NiPoint3& kSrcPos, NiPoint3& kDestPos)
{
	m_kSrcPos = kSrcPos;
	m_kDestPos = kDestPos;
}

void	CGPMissileCtrl::SetParam(NiNodePtr pkMissile,DWORD dwLife)
{
	m_pkMissile=pkMissile;
	m_dwWholeLife=dwLife;
}

NiPoint3	CGPMissileCtrl::GetSrcTargetPos()
{
	if ( !m_pSrcTarget )
	{
		return m_kSrcPos;
	}
	return m_pSrcTarget->GetPos() + m_kSrcPos;
}

NiPoint3	CGPMissileCtrl::GetDestTargetPos()
{
	if ( !m_pDestTarget )
	{
		return m_kDestPos;
	}
	return m_pDestTarget->GetPos() + m_kDestPos;
}

void CGPMissileCtrl::Update(DWORD dwLifeAcc)
{
	_ASSERTE(dwLifeAcc>=m_dwLastUpdateTime);
	_ASSERTE(m_dwWholeLife>=dwLifeAcc);
	if (!m_bActive || dwLifeAcc==m_dwLastUpdateTime)
	{//对象不处理
		return;
	}
	//todo:
	const float conLocalTimeUnit=0.25f;  //1/5
	//原理:
	//前1/5时间处理升起动画
	//移动方向=上一次update坐标(指向)目标车坐标，实时坐标
	//移动距离=上一次update时运动速度(模值) * (上一次update到本次update经历的时间)
	//获取导弹飞行指向
	//后1/5时间处理下降击中玩家
	NiPoint3 kDestVec;
	NiPoint3 kTmpVec;
	NiPoint3 kDirVec;
	float fTmp,fAngle,fTimeScale,fAngTmp;
	NiMatrix3	kMat3;
	fTimeScale=((float)dwLifeAcc)/((float)m_dwWholeLife);
	if (fTimeScale<=conLocalTimeUnit)
	{//飞起状态
		//在运动初始指向上加一个与地面高度相同的位移
		kTmpVec=GetDestTargetPos()-GetSrcTargetPos();
		kDestVec=GetSrcTargetPos()+m_kStartDir*conLocalTimeUnit*kTmpVec.Length();//GPSettings::gpMissileFloatToGround;
		kDestVec.z += GPSettings::gpMissileFloatToGround;//提高一个地面高度，确定飞行终点
		kTmpVec = kDestVec-m_kLastVec;
		//计算本次移动长度
		fTmp=((float)dwLifeAcc)/(conLocalTimeUnit*((float)m_dwWholeLife));
		kTmpVec=kTmpVec*fTmp;
		kDestVec = m_kLastVec+kTmpVec;
		//设置最后的移动坐标
		m_kLastVec=kDestVec;
		kTmpVec=NiPoint3(0.0f,0.0f,1.0f);
		fAngTmp=kTmpVec.Dot(m_kStartDir);
		fAngle= -1*acos(fAngTmp)*fTmp;
		kTmpVec=kTmpVec.Cross(m_kStartDir);
		NiPoint3::UnitizeVector(kTmpVec);
		kMat3.MakeRotation(fAngle,kTmpVec);
		kDirVec= kMat3*NiPoint3(0.0f,0.0f,1.0f);
		m_kLastDir=kDirVec;
		kTmpVec=conMissileNifDirection;
		fAngle=kTmpVec.Dot(kDirVec);
		fAngle= -1*acos(fAngle);
		kTmpVec=kTmpVec.Cross(kDirVec);
		NiPoint3::UnitizeVector(kTmpVec);
		kMat3.MakeRotation(fAngle,kTmpVec);
	}
	else if (fTimeScale>=(1.0f-conLocalTimeUnit))
	{//下降状态
		kDestVec=GetDestTargetPos();
		kDestVec.z += 0.3f;
		kTmpVec = kDestVec-m_kLastVec;
		//计算本次移动长度
		fTmp=(((float)dwLifeAcc)-(1.0f-conLocalTimeUnit)*((float)m_dwWholeLife))/
			(conLocalTimeUnit*((float)m_dwWholeLife));
		kTmpVec=kTmpVec*fTmp;
		kDestVec = m_kLastVec+kTmpVec;
		//设置最后的移动坐标
		m_kLastVec=kDestVec;
		kTmpVec=m_kLastDir;
		kDirVec=NiPoint3(0.0f,0.0f,-1.0f);
		fAngTmp=kTmpVec.Dot(kDirVec);
		fAngle= -1*acos(fAngTmp)*fTmp;
		kTmpVec=kTmpVec.Cross(kDirVec);
		NiPoint3::UnitizeVector(kTmpVec);
		kMat3.MakeRotation(fAngle,kTmpVec);
		kDirVec= kMat3*m_kLastDir;
		//m_kLastDir=kDirVec;
		kTmpVec=conMissileNifDirection;
		fAngle=kTmpVec.Dot(kDirVec);
		fAngle= -1*acos(fAngle);
		kTmpVec=kTmpVec.Cross(kDirVec);
		NiPoint3::UnitizeVector(kTmpVec);
		kMat3.MakeRotation(fAngle,kTmpVec);
	}
	else
	{//飞行过程
		kTmpVec=GetDestTargetPos()-GetSrcTargetPos();
		fTmp=kTmpVec.Length();
		NiPoint3::UnitizeVector(kTmpVec);
		//kDestVec = GetDestTargetPos();
		kDestVec=GetSrcTargetPos()+kTmpVec*(1.0f-2.0f*conLocalTimeUnit)*fTmp;
		//kDestVec.z += 0.3f;
		kDestVec.z += GPSettings::gpMissileFloatToGround;
		kTmpVec = kDestVec-m_kLastVec;
		//计算本次移动长度
		fTmp=((float)(dwLifeAcc-m_dwLastUpdateTime))/
			((1-2*conLocalTimeUnit)*((float)m_dwWholeLife));
		kTmpVec=kTmpVec*fTmp;
		//算出具体移动后的世界坐标
		m_kLastVec = m_kLastVec+kTmpVec;
		//fTimeScale=((float)dwLifeAcc)/((float)m_dwWholeLife);
		//算出螺旋线弧度
		fAngle=fTimeScale*m_stHelixParam.m_fCircleCount*2*NxPi;
		kTmpVec = kDestVec-m_kLastVec;
		NiPoint3::UnitizeVector(kTmpVec);
		kDirVec=kTmpVec;
		m_kLastDir=kDirVec;
		//开始算绕飞行主轴螺旋线运动的坐标位移
		fTmp=fabsf(kDirVec.z);
		if (fTmp>0.0f && fTmp<1.0f)
		{
			kTmpVec=kTmpVec.Cross(NiPoint3::UNIT_Z);
			NiPoint3::UnitizeVector(kTmpVec);
			kTmpVec=kTmpVec.Cross(kDirVec);
			NiPoint3::UnitizeVector(kTmpVec);
		}
		
		//kTmpVec *= (1.0-fTimeScale)*m_stHelixParam.m_fHelixRadius; //螺旋线半径逐步缩小
		kTmpVec *= m_stHelixParam.m_fHelixRadius;
		//最后算出移动的世界坐标
		kMat3=NiMatrix3::IDENTITY;
		kMat3.MakeRotation(fAngle,kDirVec);
		kTmpVec=kMat3*kTmpVec;
		kTmpVec += m_kLastVec;
		kDestVec=kTmpVec;
		//开始处理导弹模型旋转，使模型总指向目标车子
		kTmpVec=conMissileNifDirection;
		fAngle=-acos(kTmpVec.Dot(kDirVec));
		kTmpVec=kTmpVec.Cross(kDirVec);
		NiPoint3::UnitizeVector(kTmpVec);
		kMat3.MakeRotation(fAngle,kTmpVec);
	}
	m_pkMissile->SetRotate(kMat3);
	m_pkMissile->SetTranslate(kDestVec.x,kDestVec.y,kDestVec.z);
	//尾烟
	m_pkSmoke->SetTranslate(kDestVec.x,kDestVec.y,kDestVec.z);
	//更新参数
	m_dwLastUpdateTime=dwLifeAcc;
}	

void CGPMissileCtrl::Start()
{
	//设定初始值
	m_kStartVect = GetSrcTargetPos();
	NiPoint3 kDirVec=GetDestTargetPos();
	m_kStartDir = kDirVec-m_kStartVect;
	NiPoint3::UnitizeVector(m_kStartDir);
	//NiPoint3::UnitizeVector(kDirVec);
	kDirVec=NiPoint3(0.0f,0.0f,1.0f);
	m_kLastVec=m_kStartVect;
	//直接在z轴上加一个浮起高度作为导弹初始坐标
	//m_kLastVec.z += GPSettings::gpMissileFloatToGround;
	m_dwLastUpdateTime=0;
	m_stHelixParam.m_fHelixRadius=GPSettings::gpMissileHelixRadius;
	m_stHelixParam.m_fCircleCount=GPSettings::gpMissileHelixCircleCount;
	//把导弹模型旋转到飞行方向，原理看update函数说明
	NiMatrix3	kMat3;
	NiPoint3	kTmpVec=conMissileNifDirection;
	float fAngle=kTmpVec.Dot(kDirVec);
	fAngle= -acos(fAngle);
	kTmpVec=kTmpVec.Cross(kDirVec);
	NiPoint3::UnitizeVector(kTmpVec);
	kMat3.MakeRotation(fAngle,kTmpVec);
	CLevelManager::Get()->AttachObject(m_pkMissile);
	m_pkMissile->SetRotate(kMat3);
	m_pkMissile->SetTranslate(m_kLastVec.x,m_kLastVec.y,m_kLastVec.z);
	m_bActive=true;
	////////////////////
	//尾烟
	m_pkSmoke=CGPEffectMgr::Get()->NewEffect(conMissileSpraySmokeEffectId,TRUE);
	CLevelManager::Get()->AttachObject(m_pkSmoke);
}

void CGPMissileCtrl::Stop()
{
	//取消显示绑定
	CLevelManager::Get()->DetachObject(m_pkMissile);
	CLevelManager::Get()->DetachObject(m_pkSmoke);
	m_bActive=false;
}

BOOL CGPMissileCtrl::IsPlaying()
{
	return (BOOL)m_bActive;
}