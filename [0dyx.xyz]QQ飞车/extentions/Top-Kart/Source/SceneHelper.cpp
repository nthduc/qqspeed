/********************************************************************
	作者:	　wincoqin
	时间:	2007/01/23
	目的:	
*********************************************************************/


#include "Top-KartPCH.h"
#include ".\scenehelper.h"
#include "common.h"

#include <NiBound.h>
#include <NiNode.h>
#include <NiCamera.h>
#include <NiAVObject.h>

#define ONE_DEG (NI_PI / 180.f)

CSceneHelper::CSceneHelper()
: m_fAllVisibleDistance(70.f)
, m_fMinAngle(ONE_DEG * 2.f)
, m_fMaxProcessBoundRadius(60.0f)
, m_bInitialized(false)
{

}

CSceneHelper::~CSceneHelper(void)
{
	m_arrVisibleScene.RemoveAll();
}

bool CSceneHelper::Init(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius)
{
	assert(m_spCurNode);
	if (!m_spCurNode) 
	{
		XError("节点不能为空");
		return false;
	}

	if (!InitParam(fAllVisibleDistance, fMinAngle, fMaxProcessBoundRadius)) 
	{
		XError("加载策略失败!");
		return false;
	}

	RecurviseInitScene(m_spCurNode);

	m_bInitialized = true;
	return true;
}

void CSceneHelper::SetNode(NiNode* pkNode)
{
	m_spCurNode = pkNode;
}

void CSceneHelper::RecurviseInitScene(NiNode * pkNode)
{
	unsigned int unCount = pkNode->GetChildCount();

	for (int i = 0; i < unCount; ++i)
	{
		NiAVObject * pkObj = pkNode->GetAt(i);

		if (NiIsKindOf(NiNode, pkObj)) 
		{
			RecurviseInitScene((NiNode*)pkObj);
		}
		else
		{
			m_arrVisibleScene.Add(pkObj);
		}
	}
}


bool CSceneHelper::InitParam(float fAllVisibleDistance, float fMinAngle, float fMaxProcessBoundRadius)
{
	/*
	if (!pVarTable)
		return false;

	CVarTable * pStrategy = (CVarTable*)(pVarTable->Element("disp_strategy"));

	m_fAllVisibleDistance = pStrategy->Element("allvisibledist")->Float();
	m_fMinAngle			  = NxMath::degToRad(pStrategy->Element("minvisibleangle")->Float());
	m_fMaxProcessBoundRadius = pStrategy->Element("maxprocessradius")->Float();
	*/

	m_fAllVisibleDistance = fAllVisibleDistance;
	m_fMinAngle = fMinAngle;
	m_fMaxProcessBoundRadius = fMaxProcessBoundRadius;
	return true;
}

float CSceneHelper::CaculateDistance(NiAVObject * pkObj)
{
	assert(m_pkCurCamera && pkObj);

	float fDist = -1.0f;

	if (m_pkCurCamera && pkObj) 
	{
		NiPoint3 kDist = pkObj->GetWorldBound().GetCenter() - m_pkCurCamera->GetWorldTranslate();
		fDist = kDist.Length();
	}

	return fDist;
}

float CSceneHelper::CaculateAngle(NiAVObject * pkObj, float fDist)
{
	if (!pkObj) 
	{
		return -1.f;
	}

	float fBound = pkObj->GetWorldBound().GetRadius();

	return NxMath::atan(fBound / fDist);
}

bool CSceneHelper::IsInCameraDir(NiAVObject * pkObj)
{
	if (!pkObj || !m_pkCurCamera) 
	{
		return false;
	}

	NiPoint3 kDist = pkObj->GetWorldBound().GetCenter() - m_pkCurCamera->GetWorldTranslate();
	float fProject = m_pkCurCamera->GetWorldDirection().Dot(kDist);

	if (fProject < 0.01f) 
	{
		return false;
	}

	return true;
}

void CSceneHelper::CulledScene(NiCamera * pkCamera)
{
	assert(m_bInitialized);

	m_pkCurCamera = pkCamera;
	
	unsigned int unSize = m_arrVisibleScene.GetEffectiveSize();

	for (unsigned int i = 0; i < unSize; ++i)
	{
		NiAVObject * pkObj = m_arrVisibleScene.GetAt(i);
		if (!pkObj)
			continue;
		pkObj->SetAppCulled(false);

		float fDist = CaculateDistance(pkObj);
		float fRadius = pkObj->GetWorldBound().GetRadius();
	/*	
		if (fRadius > m_fMaxProcessBoundRadius && !IsInCameraDir(pkObj)) 
		{
			pkObj->SetAppCulled(true);
			continue;
		}*/

		if (fDist > m_fAllVisibleDistance && fRadius < m_fMaxProcessBoundRadius) 
		{
			float fAngle = CaculateAngle(pkObj, fDist);
			if (fDist > fRadius && fAngle <= m_fMinAngle)
			{
				pkObj->SetAppCulled(true);
			}					
		}
	}

	m_pkCurCamera = NULL;
}