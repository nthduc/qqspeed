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
	//�趨��ʼֵ
	NiPoint3 kTmpPt3=m_pSrcTarget->GetPos();
	m_kStartVect=NxVec3(kTmpPt3.x,kTmpPt3.y,kTmpPt3.z);
	m_kLastVec=m_kStartVect;
	m_dwLastUpdateTime=0;
	if (NxPlayMgr::GetHostPlay()->GetKart())
	{//��������Ħ����
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
	{//�ָ�ԭ�в���Ħ����
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
	{//���󲻴���
		return;
	}

	IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
	IGPTarget* pHost = pIter ? pIter->GetHost() : NULL;
	if ( pHost != m_pDestTarget && pHost != m_pSrcTarget )
	{
		//���Լ�û��ϵ������
		return;
	}
	if ( m_pSrcTarget == m_pDestTarget )
	{
		//�Լ����ø��Լ���Ҳ����
		return;
	}

	float fModify = GPSettings::gpMagnetAcceForce;
	IGPTarget* pAffectTarget = NULL;
	if ( m_pSrcTarget == pHost )
	{//������Լ�ʹ�õĴ�������ô��һ���ϸߵļ���
		pAffectTarget = m_pSrcTarget;
	}
	else if ( m_pDestTarget == pHost )
	{//����Ǳ��˶����ô�������ô��һ���ϵ͵ļ���
		pAffectTarget = m_pDestTarget;
		fModify *= ((-1)*GPSettings::gpMagnetTargetModifyParam);
	}

	//ʵ��ԭ��:
	//����������=����updateʱ�������������ߣ�(����������)������ҳ�ָ��Է���
	//��������С=���������滯ֵ * ����ϵ��
	NxVec3	kDestVec;
	NxVec3	kSrcVec;
	//��ȡָ��
	NiPoint3 kTmpPt3=m_pSrcTarget->GetPos();
	kSrcVec=NxVec3(kTmpPt3.x,kTmpPt3.y,kTmpPt3.z);
	m_kLastVec = kSrcVec;
	kTmpPt3=m_pDestTarget->GetPos();
	kDestVec=NxVec3(kTmpPt3.x,kTmpPt3.y,kTmpPt3.z);
	kSrcVec=kDestVec-kSrcVec;
	kSrcVec.normalize();
	//����ϵ��������
	kSrcVec *= fModify;
	float faParam[3];
	faParam[0]=kSrcVec.x;
	faParam[1]=kSrcVec.y;
	faParam[2]=0; //kSrcVec.z;
	pAffectTarget->AddForce(faParam);
	//���²���
	m_dwLastUpdateTime=dwLifeAcc;

	CGPAffectBase::Update(dwLifeAcc);
}