////////////////////////////////////////////////
//FileName:    GPReverseKeyAffect.h
//Author:      RayHuang
//Date:        2007-01-31
//Description: 反向键盘效果
////////////////////////////////////////////////

#pragma once 

#include "gpaffectbase.h"

//反向键盘效果

class CGPReverseKeyAffect: public CGPAffectBase
{
public:
	CGPReverseKeyAffect();
	~CGPReverseKeyAffect();

	//开始反向键盘
	virtual void Start();

	//停止反向键盘
	virtual void Stop();
private:
	static unsigned int m_unRefCout;
};