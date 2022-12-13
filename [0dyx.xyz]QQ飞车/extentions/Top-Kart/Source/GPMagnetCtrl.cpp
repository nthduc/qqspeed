#include "Top-KartPCH.h"
#include "gpmagnetctrl.h"
#include "gpcontrol.h"
#include "gpsettings.h"

CGPMagnetCtrl::CGPMagnetCtrl(void)
{
	m_fSaveSlipFriction=0.0f;
}

CGPMagnetCtrl::~CGPMagnetCtrl(void)
{
}

void CGPMagnetCtrl::Start()
{
	//设定初始值
	NiPoint3 kTmpPt3=m_pSrcTarget->GetPos();
	m_kStartVect=NxVec3(kTmpPt3.x,kTmpPt3.y,kTmpPt3.z);
	m_kLastVec=m_kStartVect;
	m_dwLastUpdateTime=0;
	if (NxPlayMgr::GetHostPlay()->GetKart())
	{//减弱测向摩擦力
		CTencentCar * pTencentCar =	NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle();
		m_fSaveSlipFriction=pTencentCar->GetSlipFrictionForce();
		_ASSERTE(GPSettings::gpMagnetSlackSlipFriction>0.1f &&
			GPSettings::gpMagnetSlackSlipFriction<1.0f);
		pTencentCar->SetSlipFrictionForce(m_fSaveSlipFriction*GPSettings::gpMagnetSlackSlipFriction);
	}
	else
	{
		_ASSERTE(0);
	}
	CGPAffectBase::Start();
}

void CGPMagnetCtrl::Stop()
{
	if (NxPlayMgr::GetHostPlay()->GetKart())
	{//恢复原有测向摩擦力
		CTencentCar * pTencentCar =	NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle();
		pTencentCar->SetSlipFrictionForce(m_fSaveSlipFriction);
	}
	else
	{
		_ASSERTE(0);
	}
	CGPAffectBase::Stop();
}

void CGPMagnetCtrl::Update(DWORD dwLifeAcc)
{
	_ASSERTE(dwLifeAcc>=m_dwLastUpdateTime);
	_ASSERTE(m_dwWholeLife>=dwLifeAcc);
	if (!m_bPlaying || dwLifeAcc==m_dwLastUpdateTime)
	{//对象不处理
		return;
	}

	IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
	IGPTarget* pHost = pIter ? pIter->GetHost() : NULL;
	if ( pHost != m_pDestTarget && pHost != m_pSrcTarget )
	{
		//跟自己没关系，不管
		return;
	}
	if ( m_pSrcTarget == m_pDestTarget )
	{
		//自己作用给自己的也不管
		return;
	}

	float fModify = GPSettings::gpMagnetAcceForce;
	IGPTarget* pAffectTarget = NULL;
	if ( m_pSrcTarget == pHost )
	{//如果是自己使用的磁铁，那么给一个较高的加速
		pAffectTarget = m_pSrcTarget;
	}
	else if ( m_pDestTarget == pHost )
	{//如果是别人对我用磁铁，那么给一个较低的减速
		pAffectTarget = m_pDestTarget;
		fModify *= ((-1)*GPSettings::gpMagnetTargetModifyParam);
	}

	//实现原理:
	//作用力方向=本次update时，两车坐标连线，(作用力方向)本地玩家车指向对方车
	//作用力大小=作用力正规化值 * 调整系数
	NxVec3	kDestVec;
	NxVec3	kSrcVec;
	//获取指向
	NiPoint3 kTmpPt3=m_pSrcTarget->GetPos();
	kSrcVec=NxVec3(kTmpPt3.x,kTmpPt3.y,kTmpPt3.z);
	m_kLastVec = kSrcVec;
	kTmpPt3=m_pDestTarget->GetPos();
	kDestVec=NxVec3(kTmpPt3.x,kTmpPt3.y,kTmpPt3.z);
	kSrcVec=kDestVec-kSrcVec;
	kSrcVec.normalize();
	//根据系数调整力
	kSrcVec *= fModify;
	float faParam[3];
	faParam[0]=kSrcVec.x;
	faParam[1]=kSrcVec.y;
	faParam[2]=0; //kSrcVec.z;
	pAffectTarget->AddForce(faParam);
	//更新参数
	m_dwLastUpdateTime=dwLifeAcc;

	CGPAffectBase::Update(dwLifeAcc);
}