////////////////////////////////////////////////
//     FileName:    play_gptarget.cpp
//     Author:      Fish
//     Date:        2007-1-31 10:38:06
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "Play.h"
#include "platform/MsgDataCache.h"
#include "platform/GlobalInterface.h"
#include "common.h"
#include "PhysKart/TencentCar.h"
#include "singlet.h"
#include "NxSceneMgr.h"

DWORD Play::GetID()
{
	return m_stPlayInfo.m_unUin;
}

DWORD Play::GetIndex()
{
	if ( NxSceneMgr::Get() )
	{
		return CPlayerRankMgr::Get()->GetPlayerRank(m_stPlayInfo.m_unUin);
	}
	return 0;
}

NiPoint3 Play::GetForward()
{
	NiMatrix3 matRot = GetKart()->GetKartRotate();
	float x, y, z;
	matRot.GetCol(0, x, y, z);
	return NiPoint3(x, y, z);
}

DWORD Play::GetTeam()
{
	KK_CHECK_POINTER(GlobalInterface::m_pDataCache);
	DWORD dwTeamID = 0;
	{
		if ( IsHost() )
		{
			dwTeamID = GlobalInterface::m_pDataCache->m_RoomDesc.TeamID;
		}
		else
		{
			STPlayerRoomVisibleInfo* pi = GlobalInterface::m_pDataCache->FindPlayerInfo(m_stPlayInfo.m_unUin);
			if ( pi )
			{
				dwTeamID = pi->m_bTeamID;
			}
		}
		//如果是个人赛，给每个玩家分配一个不同的TeamID
		if ( EGSI_SUBMODE_SINGLE ==
			GlobalInterface::m_pDataCache->m_RoomDesc.GameModeSub )
		{
			dwTeamID = m_stPlayInfo.m_unUin;
		}
	}
KK_EXIT:
	return dwTeamID;
}

BOOL IsRunOffTime(NiObjectNET* pParent)
{
	if ( NiIsKindOf(NiNode, pParent) )
	{
		CNifAnimationInfo kAnimationInfo;
		kAnimationInfo.CollectData((NiNode*)pParent);
		if ( kAnimationInfo.GetNumberOfControllers() == 0 
			|| kAnimationInfo.GetLoop() )
		{	//没有动画
			return FALSE;
		}
		float fMinTime = kAnimationInfo.GetMinTime();
		float fMaxTime = kAnimationInfo.GetMaxTime();
		float fMinStartTime = kAnimationInfo.GetMinStartTime();
		float fMaxLastTime = kAnimationInfo.GetMaxLastTime();
		if ( fMaxLastTime - fMinStartTime >= fMaxTime - fMinTime )
		{
			return TRUE;
		}
	}
	return FALSE;
	/*
	if ( !pParent )
	{
		return TRUE;
	}

	unsigned int nTimeControllerCount = 0;
	NiTimeController* pTimeController = pParent->GetControllers();
	while ( pTimeController )
	{
		float fHiKeyTime = pTimeController->GetEndKeyTime();
		float fStartTime = pTimeController->GetStartTime();
		float fLastTime = pTimeController->GetLastTime();
		if ( fLastTime - fStartTime < fHiKeyTime )
		{
			return FALSE;
		}
		pTimeController = pTimeController->GetNext();
		++nTimeControllerCount;
	}
	if ( nTimeControllerCount > 0 )
	{
		return TRUE;
	}

	BOOL bIsExpected = FALSE;
	if ( NiIsKindOf(NiAVObject, pParent) )
	{
		bIsExpected = TRUE;
		NiAVObject* pObj = (NiAVObject*)pParent;
		NiPropertyList* pPropList = &pObj->GetPropertyList();
		NiTListIterator kIter = pPropList->GetHeadPos();
		while ( pPropList && !pPropList->IsEmpty() && kIter )
		{
			NiProperty* pProp = pPropList->GetNext(kIter);
			if ( pProp )
			{
				if ( !IsRunOffTime(pProp) )
				{
					return FALSE;
				}
			}
		}
	}
	
	if ( NiIsKindOf(NiNode, pParent) )
	{
		bIsExpected = TRUE;
		NiNode* pNode = (NiNode*)pParent;
		for ( unsigned int i = 0; i < pNode->GetArrayCount(); ++i )
		{
			NiAVObject* pObj = pNode->GetAt(i);
			if ( !IsRunOffTime(pObj) )
			{
				return FALSE;
			}
		}

		const NiDynamicEffectList* pEffectList = &pNode->GetEffectList();
		NiTListIterator kIter = pEffectList->GetHeadPos();
		while ( pEffectList != NULL && !pEffectList->IsEmpty() && kIter )
		{
			NiDynamicEffect* pEffect = pEffectList->GetNext(kIter);
			if ( !IsRunOffTime(pEffect) )
			{
				return FALSE;
			}
		}		
	}

	return TRUE;
	*/
}

void ClearUpObsoleteNodes(NiNode* pNode)
{
	if ( !pNode )
	{
		return;
	}
	for ( unsigned int i = 0; i < pNode->GetArrayCount(); ++i )
	{
		NiAVObject* pObj = pNode->GetAt(i);
		//删除过期的特效
		if ( pObj && IsRunOffTime(pObj) )
		{
			pNode->DetachChild(pObj);
			pNode->CompactChildArray();
		}
	}
}

BOOL Play::Attach(NiAVObject* pNode)
{
	return AttachHint(pNode);
}

BOOL Play::Detach(NiAVObject* pNode)
{
	return DetachHint(pNode);
}

BOOL Play::PlayAnimation(DWORD AnimID, BOOL bPlay)
{
	if ( bPlay )
	{
		SetCharaterAnimation(AnimID);
	}
	else
	{
		SetKartAnimation(AnimID);
	}
	return TRUE;
}

BOOL Play::StopAnimation(DWORD AnimID, BOOL bPlay)
{
	if ( bPlay )
	{
		SetCharaterAnimation(1101);
	}
	else
	{
		SetKartAnimation(2305);
	}
	return TRUE;
}

void Play::AddForce(float force[3], BOOL bLocal)
{
	CTencentCar* pCar = GetKart()->GetVehicle();
	if ( pCar )
	{
		if ( bLocal )
		{
			pCar->AddForceLocal(NxVec3(force[0], force[1], force[2]));
			if ( pCar->IsDrifting())
			{
				pCar->AddDriftForceLocal(NxVec3(force[0], force[1], force[2]));
			}
		}
		else
		{
			pCar->AddForceGlobal(NxVec3(force[0], force[1], force[2]));
			if ( pCar->IsDrifting())
			{
				pCar->AddDriftForceGlobal(NxVec3(force[0], force[1], force[2]));
			}
		}
	}
}

void Play::ControlSpeed(float fRatio)
{
	CTencentCar* pCar = GetKart()->GetVehicle();
	if ( pCar )
	{
		NxReal fFriction = pCar->GetRollFrictionForce();
		pCar->SetRollFrictionForce(fFriction / fRatio);
	}
}

BOOL Play::ToggleSim(BOOL bToggle)
{
	CTencentCar* pCar = GetKart()->GetVehicle();
	if ( pCar )
	{
		if ( bToggle )
		{
			pCar->LockMotion(CTencentCar::ELM_LOCKNONE);
		}
		else
		{
			pCar->LockMotion(CTencentCar::ELM_LOCKALL);
		}
	}
	return TRUE;
}

NiPoint3 Play::GetPhysXPos()
{
	NxVec3 kPos = GetKart()->GetPos();
	return NiPoint3(kPos.x, kPos.y, kPos.z);
}

void Play::SetPhysXPos(const NiPoint3& kPos)
{
	GetKart()->SetPos(NxVec3(kPos.x, kPos.y, kPos.z));
}

NiPoint3 Play::GetVelocity(BOOL bLocal)
{
	assert(bLocal == TRUE); //目前只支持Local的
	NxVec3 kVel = GetKart()->GetVelocityLocal();
	return NiPoint3(kVel.x, kVel.y, kVel.z);
}
void Play::ZeroVelocity()
{
	GetKart()->setVelocityLocal(NxVec3(0.0f, 0.0f, 0.0f));
}