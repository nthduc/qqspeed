////////////////////////////////////////////////
//     FileName:    customculler.cpp
//     Author:      Fish
//     Date:        2007-3-5 11:11:56
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "customculler.h"
#include "common.h"
#include <NiBound.h>
#include <NiNode.h>
#include <NiCamera.h>
#include <NiAVObject.h>
#include <limits.h>

#define UNDEFINED	99999999
#define ONE_DEG (NI_PI / 180.f)

CCustomCullingProcess::CCustomCullingProcess(NiVisibleArray* pkVisibleSet):
NiCullingProcess(pkVisibleSet, true),
m_fAllVisibleDistance(UNDEFINED),
m_fMinAngle(UNDEFINED),
m_fMaxProcessBoundRadius(UNDEFINED)
{
}

bool CCustomCullingProcess::InitParam(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius)
{
	m_fAllVisibleDistance = fAllVisibleDistance;
	m_fMinAngle = fMinAngle;
	m_fMaxProcessBoundRadius = fMaxProcessBoundRadius;
	return true;
}

float CCustomCullingProcess::ComputerDistance(NiGeometry * pkObj)
{
	assert(pkObj);
	float fDist = -1.0f;
	NiPoint3 kDist = pkObj->GetWorldBound().GetCenter() - m_pkCamera->GetWorldTranslate();
	//fDist = kDist.Length();
	//提高效率的近似计算
	fDist = abs(kDist.x) + abs(kDist.y) + abs(kDist.z);
	return fDist;
}

float CCustomCullingProcess::ComputerAngle(NiGeometry * pkObj, float fDist)
{
	assert(pkObj);
	float fBound = pkObj->GetWorldBound().GetRadius();
	return NxMath::atan(fBound / fDist);
}

void CCustomCullingProcess::AppendVirtual(NiGeometry& kVisible)
{
	float fRadius = kVisible.GetWorldBound().GetRadius();
	if ( fRadius < m_fMaxProcessBoundRadius )
	{
		float fDist = ComputerDistance(&kVisible);		
		if (fDist > m_fAllVisibleDistance ) 
		{
			float fAngle = ComputerAngle(&kVisible, fDist);
			if (fDist > fRadius && fAngle <= m_fMinAngle)
			{
				return;
			}					
		}
	}
	m_pkVisibleSet->Add(kVisible);
}