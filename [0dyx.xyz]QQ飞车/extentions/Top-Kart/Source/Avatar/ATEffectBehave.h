////////////////////////////////////////////////
//     FileName:    ATEffectBehave.h
//     Author:      Rayhuang
//     Date:        2007-4-25
//     Description: avatar特有特效表现
////////////////////////////////////////////////

#pragma once 
#include "ATBehaveBase.h"

class CATEffectBehave : public CATBehaveBase 
{
public:
	CATEffectBehave(BEHAVEDATA tagBD);
	virtual ~ CATEffectBehave();
	virtual void Start();
	virtual void Stop();
	virtual void Update(float fTime);

private:
	NiNodePtr m_spEffect;
};
