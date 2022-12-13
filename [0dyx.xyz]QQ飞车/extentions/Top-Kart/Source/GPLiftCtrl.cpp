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
	//�趨��ʼֵ
	m_dwLiftTime = m_dwOption; //Option�����������û���õ�������������ʱ��
	m_kOriginPos = m_pDestTarget->GetPhysXPos();
	if ( m_dwLiftTime == 0 )
	{
		NiPoint3 kNewPos = m_kOriginPos;
		kNewPos.z += m_fParam;
		m_pDestTarget->SetPhysXPos(kNewPos);
	}
	//ͣ����
	m_pDestTarget->ToggleSim(FALSE);
}

void CGPLiftCtrl::Stop()
{
	CGPAffectBase::Stop();
	//������
	m_pDestTarget->ToggleSim(TRUE);
}

void CGPLiftCtrl::Update(DWORD dwLifeAcc)
{
	CGPAffectBase::Update(dwLifeAcc);

	IGPTarget* pHostTarget = m_pControl->GetTargetIterator()->GetHost();
	if ( pHostTarget != m_pDestTarget )
	{	//������Ǳ�����ң���������ͬ��ȥ����
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