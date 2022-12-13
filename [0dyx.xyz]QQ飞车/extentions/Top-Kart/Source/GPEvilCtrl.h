////////////////////////////////////////////////
//FileName:    GPEvilCtrl.h
//Author:      PigHuang
//Date:        2007-02-27
//Description: 魔王道具效果控制类
////////////////////////////////////////////////

#pragma once
#include "gpaffectbase.h"


///魔王道具效果控制类
class CGPEvilCtrl :
	public	CGPAffectBase
{
public:
	CGPEvilCtrl(void);
	virtual ~CGPEvilCtrl(void);
	//实现在接口的定义函数
	virtual	void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
private:
	NiShaderPtr m_spGlow;	///魔王特效shader
	NiShaderPtr m_spOldShader;	///保存用shader
};
