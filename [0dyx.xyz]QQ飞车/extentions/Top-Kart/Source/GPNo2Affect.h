////////////////////////////////////////////////
//FileName:    GPNo2Affect.h
//Author:      RayHuang
//Date:        2007-04-12
//Description: µªÆøÐ§¹û
////////////////////////////////////////////////
#pragma once 
#include "gpaffectbase.h"
#include "GPJetAffect.h"

class CGPNo2Affect : public CGPAffectBase
{
public:
	CGPNo2Affect();
	~CGPNo2Affect();

	virtual void Start();
	
	virtual void Stop();
	
	virtual void Update(DWORD);
private:
	//DWORD m_dwSound;

	unsigned int	m_unNumOfEffects;
	unsigned int	m_unTargerID;
	NiNodePtr		m_spEffectNode[MAX_PIPES];
	NiNodePtr		m_spAcceNode;
	NiFixedString	m_strNodeName[MAX_PIPES];

	float m_fForceX;

	unsigned int m_unStartTime;
};