////////////////////////////////////////////////
//FileName:    GPLiftCtrl.h
//Author:      PigHuang
//Date:        2007-01-31
//Description: ����̧����������
////////////////////////////////////////////////

#pragma once
#include "gpaffectbase.h"

///����̧����������
class CGPLiftCtrl :
	public	CGPAffectBase
{
public:
	CGPLiftCtrl(void);
	virtual ~CGPLiftCtrl(void);
	//ʵ���ڽӿڵĶ��庯��
	virtual	void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
private:
	DWORD		m_dwLiftTime;
	NiPoint3	m_kOriginPos;
};
