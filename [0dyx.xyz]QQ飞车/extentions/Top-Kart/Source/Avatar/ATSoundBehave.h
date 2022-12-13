////////////////////////////////////////////////
//     FileName:    ATAnimBehave.h
//     Author:      Rayhuang
//     Date:        2007-4-25
//     Description: avatar������������
////////////////////////////////////////////////

#pragma once 
#include "ATBehaveBase.h"


class CATSoundBehave : public CATBehaveBase
{
public:
	CATSoundBehave(BEHAVEDATA tagBD);
	~CATSoundBehave();
	virtual void Start();
	virtual void Stop();
	virtual void Update(float fTime);
private:
	DWORD m_dwSound;
};