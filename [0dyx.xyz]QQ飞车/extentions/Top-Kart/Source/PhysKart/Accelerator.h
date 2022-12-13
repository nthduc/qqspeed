#pragma once
#include "NiMemObject.h"

class CAccelerator
	: public NiMemObject
{
public:
	CAccelerator(float fDestSpeed, float fDurance);
	virtual ~CAccelerator();

	float GetDeltaForce(float fCurSpeed, float fCurTime);

protected:
	static const float m_fTolerance;

	float m_fDestSpeed;
	float m_fDurance;
	float m_fDelta;
};