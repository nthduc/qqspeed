////////////////////////////////////////////////
//FileName:    GPLockKeyAffact.h
//Author:      RayHuang
//Date:        2007-01-31
//Description: ������Ч��
////////////////////////////////////////////////

#pragma once 

#include "gpaffectbase.h"


//������Ч��
class CGPLockKeyAffact: public CGPAffectBase
{
public:
	CGPLockKeyAffact();
	~CGPLockKeyAffact();

	//��ʼ������
	virtual void Start();

	//ֹͣ������
	virtual void Stop();

};
