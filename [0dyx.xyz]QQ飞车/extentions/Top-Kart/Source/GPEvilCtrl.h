////////////////////////////////////////////////
//FileName:    GPEvilCtrl.h
//Author:      PigHuang
//Date:        2007-02-27
//Description: ħ������Ч��������
////////////////////////////////////////////////

#pragma once
#include "gpaffectbase.h"


///ħ������Ч��������
class CGPEvilCtrl :
	public	CGPAffectBase
{
public:
	CGPEvilCtrl(void);
	virtual ~CGPEvilCtrl(void);
	//ʵ���ڽӿڵĶ��庯��
	virtual	void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
private:
	NiShaderPtr m_spGlow;	///ħ����Чshader
	NiShaderPtr m_spOldShader;	///������shader
};
