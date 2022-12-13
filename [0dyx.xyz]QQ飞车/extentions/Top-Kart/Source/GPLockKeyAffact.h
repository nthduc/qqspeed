////////////////////////////////////////////////
//FileName:    GPLockKeyAffact.h
//Author:      RayHuang
//Date:        2007-01-31
//Description: 锁键盘效果
////////////////////////////////////////////////

#pragma once 

#include "gpaffectbase.h"


//锁键盘效果
class CGPLockKeyAffact: public CGPAffectBase
{
public:
	CGPLockKeyAffact();
	~CGPLockKeyAffact();

	//开始锁键盘
	virtual void Start();

	//停止锁键盘
	virtual void Stop();

};
