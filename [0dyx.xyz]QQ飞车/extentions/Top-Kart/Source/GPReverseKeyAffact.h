////////////////////////////////////////////////
//FileName:    GPReverseKeyAffect.h
//Author:      RayHuang
//Date:        2007-01-31
//Description: �������Ч��
////////////////////////////////////////////////

#pragma once 

#include "gpaffectbase.h"

//�������Ч��

class CGPReverseKeyAffect: public CGPAffectBase
{
public:
	CGPReverseKeyAffect();
	~CGPReverseKeyAffect();

	//��ʼ�������
	virtual void Start();

	//ֹͣ�������
	virtual void Stop();
private:
	static unsigned int m_unRefCout;
};