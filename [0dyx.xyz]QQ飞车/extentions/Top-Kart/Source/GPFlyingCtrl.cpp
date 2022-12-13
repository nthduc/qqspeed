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
	{//���󲻴���
		return;
	}
	//ԭ��:
	//�ƶ�����=��һ��update����(ָ��)Ŀ�공���꣬ʵʱ����
	//�ƶ�����=��һ��updateʱ�˶��ٶ�(ģֵ) * (��һ��update������update������ʱ��)
	//��ȡ���ը������ָ��
	NiPoint3 kDestVec = m_pDestTarget->GetPos();
	NiPoint3 kTmpVec;
	NiPoint3 kDirVec;
	NiMatrix3	kMat3;
	float fTmp,fTimeScale,fDistance;
	fTimeScale=((float)dwLifeAcc)/((float)m_dwWholeLife);
	//ȷ����ǰ���������ڵ�ת����
	m_stHelixParam.m_fNowTurnCount=::ceil(fTimeScale*m_stHelixParam.m_fTurnCount);
	if ((m_stHelixParam.m_fNowTurnCount-m_stHelixParam.m_fLastTurnCount)>0.1f ||
		m_stHelixParam.m_fLastTurnCount<0.001f)
	{//Ŀ�����Ҫ����
		//���Ŀ��㻡��
		UINT nRandTmp=rand();
		//fTmp=m_stHelixParam.m_fHelixSideMinAngle+
		//	(m_stHelixParam.m_fHelixSideMaxAngle-m_stHelixParam.m_fHelixSideMinAngle)*
		//	((float)nRandTmp/(float)RAND_MAX);
		fTmp=m_stHelixParam.m_fHelixSideMinAngle+
			(m_stHelixParam.m_fHelixSideMaxAngle-m_stHelixParam.m_fHelixSideMinAngle)*
			((float)nRandTmp/(float)RAND_MAX);
		if (m_stHelixParam.m_fLastTurnCount<0.001f)
		{//��һ��
			if(0==(nRandTmp%2))
			{//�������
				m_stHelixParam.m_fLastTurnAngle = (-1)*fTmp;
			}
			else
			{
				m_stHelixParam.m_fLastTurnAngle	= fTmp;
			}
		}
		else
		{//ȡ��
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
		{//���һ��ת��,ֱ�ӷ���Ŀ�공

			m_stHelixParam.m_kNowDestVec = kDestVec;
			//����������������
			m_stHelixParam.m_kLastTurnVec=m_stHelixParam.m_kNowDestVec;
		}
		else
		{//��������Ԥ��Ŀ���
			//�����ǰÿһת�۷����ó̵�ʱ��
			fTmp=((float)(m_stHelixParam.m_fNowTurnCount-m_stHelixParam.m_fLastTurnCount))/
				((float)(m_stHelixParam.m_fTurnCount-m_stHelixParam.m_fLastTurnCount));
			kTmpVec=kDestVec-m_stHelixParam.m_kLastTurnVec;
			fDistance=kTmpVec.Length()*fTmp;
			NiPoint3::UnitizeVector(kTmpVec);
			kDirVec=kTmpVec;
			m_stHelixParam.m_kNowDestVec=kTmpVec*fDistance;
			//����������������
			m_stHelixParam.m_kLastTurnVec += m_stHelixParam.m_kNowDestVec;
			//��ʼ���Ʒ��������������˶�������λ��
			fTmp=fabsf(kDirVec.z);
			if (fTmp>0.0f && fTmp<1.0f)
			{
				kTmpVec=kTmpVec.Cross(NiPoint3::UNIT_Z);
				NiPoint3::UnitizeVector(kTmpVec);
				kTmpVec=kTmpVec.Cross(kDirVec);
				NiPoint3::UnitizeVector(kTmpVec);
			}
			//�뾶������ű���
			fTmp=m_stHelixParam.m_fHelixMinRadius+
				(m_stHelixParam.m_fHelixMaxRadius-m_stHelixParam.m_fHelixMinRadius)*
				((float)nRandTmp/(float)RAND_MAX);
			kTmpVec *= fTmp; //�����߰뾶
			//�������ƶ���Ŀ�����������
			kMat3=NiMatrix3::IDENTITY;
			kMat3.MakeRotation(m_stHelixParam.m_fLastTurnAngle,kDirVec);
			kTmpVec=kMat3*kTmpVec;
			m_stHelixParam.m_kNowDestVec=kTmpVec + m_stHelixParam.m_kLastTurnVec;
		}
		m_stHelixParam.m_fLastTurnCount=m_stHelixParam.m_fNowTurnCount;
	}

	//��ʼ��ʵ�ʷ����˶�������λ��
	kTmpVec = m_stHelixParam.m_kNowDestVec - m_kLastVec;
	NiPoint3::UnitizeVector(kTmpVec);
	kDirVec=kTmpVec;
	fTmp=m_stHelixParam.m_fNowTurnCount-(fTimeScale*m_stHelixParam.m_fTurnCount);
	_ASSERTE(fTmp>=0.0f);
	kTmpVec *= fTmp;
	//�����������տ��ը����������
	m_kLastVec += kTmpVec;
	//��ʼ������ը��ģ����ת��ʹģ����ָ����й켣
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
	//��Ӱ
	if (m_pkSwoosh)
	{
		m_pkSwoosh->TakeSample(dwLifeAcc);
	}
	//β��
	m_pkSmoke->SetTranslate(m_kLastVec.x,m_kLastVec.y,m_kLastVec.z);
	//���²���
	m_dwLastUpdateTime=dwLifeAcc;
}	

void CGPFlyingCtrl::Start()
{
	//�趨��ʼֵ
	float fTmp=0.0f;
	m_kStartVect = m_pSrcTarget->GetPos();
	NiPoint3 kDirVec=m_pDestTarget->GetPos();
	kDirVec = kDirVec-m_kStartVect;
	NiPoint3::UnitizeVector(kDirVec);
	m_kLastVec=m_kStartVect;
	//ֱ����z���ϼ�һ������߶���Ϊ���ը����ʼ����
	m_kLastVec.z += GPSettings::gpFlyingFloatToGround;
	//���÷���ԭʼֵ
	m_stHelixParam.m_kLastTurnVec=m_kLastVec;
	m_dwLastUpdateTime=0;
	//װ�ط��й켣��ʼֵ
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
	//���ɱ��η��е���ת����
	m_stHelixParam.m_fTurnCount=::ceil(m_stHelixParam.m_fTurnMinCount+
		(m_stHelixParam.m_fTurnMaxCount-m_stHelixParam.m_fTurnMinCount)*
		((float)rand()/(float)RAND_MAX));
	//�ѿ��ը��ģ����ת�����з���ԭ��update����˵��
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
	//��swoosh
	//todo:������swoosh
	/*m_pkSwoosh = NiNew Swoosh(m_pkFlying,0.0f);
	m_pkSwoosh->SetActivity(true);*/

	//β��
	m_pkSmoke=CGPEffectMgr::Get()->NewEffect(conFlyingSpraySmokeEffectId,TRUE);
	CLevelManager::Get()->AttachObject(m_pkSmoke);
}

void CGPFlyingCtrl::Stop()
{
	//ȡ����ʾ��
	CLevelManager::Get()->DetachObject(m_pkFlying);
	CLevelManager::Get()->DetachObject(m_pkSmoke);
	m_bActive=false;
}

BOOL CGPFlyingCtrl::IsPlaying()
{
	return (BOOL)m_bActive;
}