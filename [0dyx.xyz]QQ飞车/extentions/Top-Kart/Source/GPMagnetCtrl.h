////////////////////////////////////////////////
//FileName:    GPMagnetCtrl.h
//Author:      PigHuang
//Date:        2007-01-30
//Description: �������߶���������
////////////////////////////////////////////////

#pragma once
#include "gpaffectbase.h"


///�������߶���������
class CGPMagnetCtrl :
	public	CGPAffectBase
{
public:
	CGPMagnetCtrl(void);
	virtual ~CGPMagnetCtrl(void);
	//ʵ���ڽӿڵĶ��庯��
	virtual	void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
private:
	NxVec3	m_kStartVect;	///��������ʱ������ҳ��ӵ�����
	NxVec3	m_kLastVec;		///��һ��updateʱ���ӵ�����
	DWORD	m_dwWholeLife;		///���߶�������������(ms)
	DWORD	m_dwLastUpdateTime;	///���һ��updateʱ��(ָ�ۼ�ʱ��,<=m_dwWholeLife)
	float	m_fSaveSlipFriction;	///������Чǰ�Ĳ���Ħ����
};
