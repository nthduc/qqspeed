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
	{//���󲻴���
		return;
	}
	//todo:
	const float conLocalTimeUnit=0.25f;  //1/5
	//ԭ��:
	//ǰ1/5ʱ�䴦�����𶯻�
	//�ƶ�����=��һ��update����(ָ��)Ŀ�공���꣬ʵʱ����
	//�ƶ�����=��һ��updateʱ�˶��ٶ�(ģֵ) * (��һ��update������update������ʱ��)
	//��ȡ��������ָ��
	//��1/5ʱ�䴦���½��������
	NiPoint3 kDestVec;
	NiPoint3 kTmpVec;
	NiPoint3 kDirVec;
	float fTmp,fAngle,fTimeScale,fAngTmp;
	NiMatrix3	kMat3;
	fTimeScale=((float)dwLifeAcc)/((float)m_dwWholeLife);
	if (fTimeScale<=conLocalTimeUnit)
	{//����״̬
		//���˶���ʼָ���ϼ�һ�������߶���ͬ��λ��
		kTmpVec=GetDestTargetPos()-GetSrcTargetPos();
		kDestVec=GetSrcTargetPos()+m_kStartDir*conLocalTimeUnit*kTmpVec.Length();//GPSettings::gpMissileFloatToGround;
		kDestVec.z += GPSettings::gpMissileFloatToGround;//���һ������߶ȣ�ȷ�������յ�
		kTmpVec = kDestVec-m_kLastVec;
		//���㱾���ƶ�����
		fTmp=((float)dwLifeAcc)/(conLocalTimeUnit*((float)m_dwWholeLife));
		kTmpVec=kTmpVec*fTmp;
		kDestVec = m_kLastVec+kTmpVec;
		//���������ƶ�����
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
	{//�½�״̬
		kDestVec=GetDestTargetPos();
		kDestVec.z += 0.3f;
		kTmpVec = kDestVec-m_kLastVec;
		//���㱾���ƶ�����
		fTmp=(((float)dwLifeAcc)-(1.0f-conLocalTimeUnit)*((float)m_dwWholeLife))/
			(conLocalTimeUnit*((float)m_dwWholeLife));
		kTmpVec=kTmpVec*fTmp;
		kDestVec = m_kLastVec+kTmpVec;
		//���������ƶ�����
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
	{//���й���
		kTmpVec=GetDestTargetPos()-GetSrcTargetPos();
		fTmp=kTmpVec.Length();
		NiPoint3::UnitizeVector(kTmpVec);
		//kDestVec = GetDestTargetPos();
		kDestVec=GetSrcTargetPos()+kTmpVec*(1.0f-2.0f*conLocalTimeUnit)*fTmp;
		//kDestVec.z += 0.3f;
		kDestVec.z += GPSettings::gpMissileFloatToGround;
		kTmpVec = kDestVec-m_kLastVec;
		//���㱾���ƶ�����
		fTmp=((float)(dwLifeAcc-m_dwLastUpdateTime))/
			((1-2*conLocalTimeUnit)*((float)m_dwWholeLife));
		kTmpVec=kTmpVec*fTmp;
		//��������ƶ������������
		m_kLastVec = m_kLastVec+kTmpVec;
		//fTimeScale=((float)dwLifeAcc)/((float)m_dwWholeLife);
		//��������߻���
		fAngle=fTimeScale*m_stHelixParam.m_fCircleCount*2*NxPi;
		kTmpVec = kDestVec-m_kLastVec;
		NiPoint3::UnitizeVector(kTmpVec);
		kDirVec=kTmpVec;
		m_kLastDir=kDirVec;
		//��ʼ���Ʒ��������������˶�������λ��
		fTmp=fabsf(kDirVec.z);
		if (fTmp>0.0f && fTmp<1.0f)
		{
			kTmpVec=kTmpVec.Cross(NiPoint3::UNIT_Z);
			NiPoint3::UnitizeVector(kTmpVec);
			kTmpVec=kTmpVec.Cross(kDirVec);
			NiPoint3::UnitizeVector(kTmpVec);
		}
		
		//kTmpVec *= (1.0-fTimeScale)*m_stHelixParam.m_fHelixRadius; //�����߰뾶����С
		kTmpVec *= m_stHelixParam.m_fHelixRadius;
		//�������ƶ�����������
		kMat3=NiMatrix3::IDENTITY;
		kMat3.MakeRotation(fAngle,kDirVec);
		kTmpVec=kMat3*kTmpVec;
		kTmpVec += m_kLastVec;
		kDestVec=kTmpVec;
		//��ʼ������ģ����ת��ʹģ����ָ��Ŀ�공��
		kTmpVec=conMissileNifDirection;
		fAngle=-acos(kTmpVec.Dot(kDirVec));
		kTmpVec=kTmpVec.Cross(kDirVec);
		NiPoint3::UnitizeVector(kTmpVec);
		kMat3.MakeRotation(fAngle,kTmpVec);
	}
	m_pkMissile->SetRotate(kMat3);
	m_pkMissile->SetTranslate(kDestVec.x,kDestVec.y,kDestVec.z);
	//β��
	m_pkSmoke->SetTranslate(kDestVec.x,kDestVec.y,kDestVec.z);
	//���²���
	m_dwLastUpdateTime=dwLifeAcc;
}	

void CGPMissileCtrl::Start()
{
	//�趨��ʼֵ
	m_kStartVect = GetSrcTargetPos();
	NiPoint3 kDirVec=GetDestTargetPos();
	m_kStartDir = kDirVec-m_kStartVect;
	NiPoint3::UnitizeVector(m_kStartDir);
	//NiPoint3::UnitizeVector(kDirVec);
	kDirVec=NiPoint3(0.0f,0.0f,1.0f);
	m_kLastVec=m_kStartVect;
	//ֱ����z���ϼ�һ������߶���Ϊ������ʼ����
	//m_kLastVec.z += GPSettings::gpMissileFloatToGround;
	m_dwLastUpdateTime=0;
	m_stHelixParam.m_fHelixRadius=GPSettings::gpMissileHelixRadius;
	m_stHelixParam.m_fCircleCount=GPSettings::gpMissileHelixCircleCount;
	//�ѵ���ģ����ת�����з���ԭ��update����˵��
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
	//β��
	m_pkSmoke=CGPEffectMgr::Get()->NewEffect(conMissileSpraySmokeEffectId,TRUE);
	CLevelManager::Get()->AttachObject(m_pkSmoke);
}

void CGPMissileCtrl::Stop()
{
	//ȡ����ʾ��
	CLevelManager::Get()->DetachObject(m_pkMissile);
	CLevelManager::Get()->DetachObject(m_pkSmoke);
	m_bActive=false;
}

BOOL CGPMissileCtrl::IsPlaying()
{
	return (BOOL)m_bActive;
}