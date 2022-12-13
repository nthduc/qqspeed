#include "../Top-KartPCH.h"

#include "Accelerator.h"
#include "NiMath.h"

const float CAccelerator::m_fTolerance = 0.5f;

CAccelerator::CAccelerator(float fDestSpeed, float fDurance)
: m_fDestSpeed(fDestSpeed)
, m_fDurance(fDurance)
{

}

CAccelerator::~CAccelerator()
{

}

float CAccelerator::GetDeltaForce(float fCurSpeed, float fCurTime)
{
	float fDeltaForce = 0.0f;

	float fDelta = fCurSpeed - m_fDestSpeed;

	float fSquareDelta = fDelta * fDelta;

	fDeltaForce = fDelta * fSquareDelta / (1.f + fSquareDelta);

	return fDeltaForce;
}

