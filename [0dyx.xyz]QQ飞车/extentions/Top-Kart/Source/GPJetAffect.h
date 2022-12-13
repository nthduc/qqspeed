////////////////////////////////////////////////
//FileName:    GPJectAffect.h
//Author:      RayHuang
//Date:        2007-02-07
//Description: Åç»ðÐ§¹û
////////////////////////////////////////////////

#pragma once 

#include "gpaffectbase.h"

const unsigned int MAX_PIPES = 6;

class CGPJectAffect: public CGPAffectBase
{
public:
	CGPJectAffect();
	~CGPJectAffect();

	virtual void Start();
	virtual void Stop();

private:
	
	unsigned int	m_unNumOfEffects;
	unsigned int	m_unTargerID;
	NiNodePtr		m_spEffectNode[MAX_PIPES];
	NiFixedString	m_strNodeName[MAX_PIPES];

};