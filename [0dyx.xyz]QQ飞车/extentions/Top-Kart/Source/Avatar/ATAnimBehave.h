////////////////////////////////////////////////
//     FileName:    ATAnimBehave.h
//     Author:      Rayhuang
//     Date:        2007-4-25
//     Description: avatar特有动作表现
////////////////////////////////////////////////

#pragma once 
#include "ATBehaveBase.h"

class CATAnimBehave: public CATBehaveBase
{
public:
	CATAnimBehave(BEHAVEDATA tagBD);
	virtual ~CATAnimBehave();
	virtual void Start();
	virtual void Stop();
	virtual void Update(float fTime);
};