#include "Top-KartPCH.h"
#include "gpliftctrl.h"
#include "gpcontrol.h"

CGPLiftCtrl::CGPLiftCtrl(void):
m_dwLiftTime(0), m_kOriginPos(0, 0, 0)
{
}

CGPLiftCtrl::~CGPLiftCtrl(void)
{
}

void CGPLiftCtrl::Start()
{
	CGPAffectBase::Start();
	//设定初始值
	m_dwLiftTime = m_dwOption; //Option在这个作用中没有用到，且用作提升时间
	m_kOriginPos = m_pDestTarget->GetPhysXPos();
	if ( m_dwLiftTime == 0 )
	{
		NiPoint3 kNewPos = m_kOriginPos;
		kNewPos.z += m_fParam;
		m_pDestTarget->SetPhysXPos(kNewPos);
	}
	//停物理
	m_pDestTarget->ToggleSim(FALSE);
}

void CGPLiftCtrl::Stop()
{
	CGPAffectBase::Stop();
	//开物理
	m_pDestTarget->ToggleSim(TRUE);
}

void CGPLiftCtrl::Update(DWORD dwLifeAcc)
{
	CGPAffectBase::Update(dwLifeAcc);

	IGPTarget* pHostTarget = m_pControl->GetTargetIterator()->GetHost();
	if ( pHostTarget != m_pDestTarget )
	{	//如果不是本地玩家，就让网络同步去做吧
		return;
	}
	float fRatio = (dwLifeAcc / (m_dwLiftTime * 1.0));
	if ( fRatio > 1.0 )
	{
		fRatio = 1.0;
	}
	NiPoint3 kNewPos = m_kOriginPos;
	kNewPos.z += fRatio * m_fParam;
	m_pDestTarget->SetPhysXPos(kNewPos);
}