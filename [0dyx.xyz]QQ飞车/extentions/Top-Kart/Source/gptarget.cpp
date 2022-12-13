////////////////////////////////////////////////
//     FileName:    gptarget.cpp
//     Author:      Fish
//     Date:        2007-1-19 15:29:17
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "gptarget.h"
#include "gpcontrol.h"
#include "common.h"

CGPTargetBase::CGPTargetBase():
m_bAimed(FALSE),
m_bInvalidTarget(FALSE)
{
}

CGPTargetBase::~CGPTargetBase()
{
}

DWORD	CGPTargetBase::GetType()
{
	return 0;
}

DWORD	CGPTargetBase::GetID()
{
	return 0;
}

DWORD	CGPTargetBase::GetIndex()
{
	return 0;
}

DWORD	CGPTargetBase::GetTeam()
{
	return 0;
}

NiPoint3 CGPTargetBase::GetPos()
{
	return NiPoint3(0, 0, 0);
}

void	CGPTargetBase::SetPos(const NiPoint3&)
{
}

NiPoint3 CGPTargetBase::GetPhysXPos()
{
	return NiPoint3(0, 0, 0);
}

void CGPTargetBase::SetPhysXPos(const NiPoint3&)
{
}

NiPoint3 CGPTargetBase::GetVelocity(BOOL bLocal)
{
	return NiPoint3(0, 0, 0);
}

void CGPTargetBase::ZeroVelocity()
{
}

NiPoint3 CGPTargetBase::GetForward()
{
	return NiPoint3(1, 0, 0);
}

NiTransform CGPTargetBase::GetTransform()
{
	NiTransform trans;
	trans.MakeIdentity();
	return trans;
}

NiNode* CGPTargetBase::GetNode()
{
	return NULL;
}

BOOL	CGPTargetBase::Attach(const char* effect, const char* node)
{
	return TRUE;
}

BOOL	CGPTargetBase::Detach(const char* node)
{
	return TRUE;
}


BOOL	CGPTargetBase::Attach(NiAVObject* pNode)
{
	return TRUE;
}

BOOL	CGPTargetBase::Detach(NiAVObject* pNode)
{
	return TRUE;
}

BOOL	CGPTargetBase::PlayAnimation(DWORD AnimID, BOOL bPlay)
{
	return TRUE;
}

BOOL	CGPTargetBase::StopAnimation(DWORD AnimID, BOOL bPlay)
{
	return TRUE;
}

void	CGPTargetBase::AddForce(float force[3], BOOL bLocal)
{
}

void CGPTargetBase::ControlSpeed(float fRatio)
{
}

BOOL	CGPTargetBase::ToggleSim(BOOL)
{
	return TRUE;
}

GP_ID_LIST&	CGPTargetBase::GetGPs()
{
	return m_listGPs;
}

void	CGPTargetBase::AddGP(DWORD id, DWORD index)
{
	GP_ID gpid;
	gpid.dwID = id;
	gpid.dwIndex = index;
	m_listGPs.push_back(gpid);
}

void	CGPTargetBase::RemoveGP(DWORD id)
{
	GP_ID_LIST_Iterator it = m_listGPs.begin();
	for ( ; it != m_listGPs.end(); ++it )
	{
		LPGP_ID pGPID = &*it;
		if ( pGPID->dwID == id )
		{
			m_listGPs.erase(it);
			break;
		}
	}
}

GP_DATA_LIST&	CGPTargetBase::GetAffectGPs()
{
	return m_listAffectGPs;
}

void	CGPTargetBase::AddAffectGP(LPGP_DATA pData)
{
	GP_DATA_LIST_Iterator it = m_listAffectGPs.begin();
	for ( ; it != m_listAffectGPs.end(); ++it )
	{
		LPGP_DATA pDataTmp = &*it;
		DWORD t1 = pDataTmp->dwUseTime + pDataTmp->pConfig->dwReadyTime + pDataTmp->pConfig->dwTriggerTime;
		DWORD t2 = pData->dwUseTime + pData->pConfig->dwReadyTime + pData->pConfig->dwTriggerTime;
		if ( t1 > t2 )
		{
			if ( it == m_listAffectGPs.begin() )
			{
				m_listAffectGPs.push_front(*pData);
			}
			else
			{
				--it;
				m_listAffectGPs.insert(it, *pData);
			}
			return;
		}
	}
	m_listAffectGPs.push_back(*pData);
}

void	CGPTargetBase::RemoveAffectGP(DWORD index)
{
	GP_DATA_LIST_Iterator it = m_listAffectGPs.begin();
	for ( ; it != m_listAffectGPs.end(); ++it )
	{
		LPGP_DATA pData = &*it;
		if ( pData->dwIndex == index )
		{
			_DestroyAffectGP(pData);
			m_listAffectGPs.erase(it);
			break;
		}
	}
}

void CGPTargetBase::ClearAffectGPs()
{
	GP_DATA_LIST_Iterator it = m_listAffectGPs.begin();
	for ( ; it != m_listAffectGPs.end(); ++it )
	{
		LPGP_DATA pData = &*it;
		_DestroyAffectGP(pData);
	}
	m_listAffectGPs.clear();
}

void CGPTargetBase::_DestroyAffectGP(LPGP_DATA pData)
{
	//É¾³ýpMoveCtrl
	if ( pData->pMoveCtrl )
	{
		if ( pData->pMoveCtrl->IsPlaying() )
		{
			pData->pMoveCtrl->Stop();
		}
		DestroyGPMovementControl((ENMMOVETYPE)pData->pConfig->stMoveCtrl.dwMoveType, pData->pMoveCtrl);
	}
	//É¾³ýpAffect
	if ( pData->listAffect.size() > 0 )
	{
		GP_AFFECTDATA_LIST_Iterator itAffect = pData->listAffect.begin();
		for ( ; itAffect != pData->listAffect.end(); ++itAffect )
		{
			LPGP_AFFECTDATA pAffectData = &*itAffect;
			if ( pAffectData )
			{
				if ( pAffectData->pAffect && pAffectData->pAffect->IsPlaying() )
				{
					pAffectData->pAffect->Stop();
				}
				DestroyGPAffect((ENMAFFECTID)pAffectData->pAffectConfig->dwID, pAffectData->pAffect);
			}
		}
	}
}

BOOL CGPTargetBase::SetAimed(BOOL bSet)
{
	BOOL bOld = m_bAimed;
	m_bAimed = bSet;
	return bOld;
}

BOOL CGPTargetBase::GetAimed()
{
	return m_bAimed;
}

BOOL CGPTargetBase::SetInvalidTarget(BOOL bSet)
{
	BOOL bOld = m_bInvalidTarget;
	m_bInvalidTarget = bSet;
	return bOld;
}

BOOL CGPTargetBase::GetInvalidTarget()
{
	return m_bInvalidTarget;
}
