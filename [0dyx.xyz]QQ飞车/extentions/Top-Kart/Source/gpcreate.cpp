////////////////////////////////////////////////
//     FileName:    gpcreate.cpp
//     Author:      Fish
//     Date:        2007-1-19 15:18:36
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "gpdefine.h"
#include "gpcreate.h"
#include "common.h"
#include "Play.h"
#include "NxPlayMgr.h"
#include "gpeffect.h"
#include "gpcontrol.h"
#include "GPMissileCtrl.h"
#include "gpflyingctrl.h"
#include "GPReverseKeyAffact.h"
#include "GPLockKeyAffact.h"
#include "GPMagnetCtrl.h"
#include "GPLiftCtrl.h"
#include "GPJetAffect.h"
#include "GPEvilCtrl.h"
#include "GPNo2Affect.h"
#include "Monster.h"

BOOL CreateGPTargetIterator(IGPTargetIterator** ppv)
{
	assert(ppv);
	CGPTargetIterator* pv = new CGPTargetIterator;
	assert(pv);
	*ppv = pv;
	return TRUE;
}

void DestroyGPTargetIterator(IGPTargetIterator* pv)
{
	assert(pv);
	CGPTargetIterator* p = (CGPTargetIterator*)pv;
	delete p;
}

BOOL CreateGPAffect(ENMAFFECTID id, IGPAffect** ppv)
{
	assert(ppv);
	DWORD dwID = (DWORD)id;
	if ( dwID < 0 || dwID > EAI_MAX )
		return FALSE;

	BOOL bRet = TRUE;
	switch(id)
	{
	case EAI_None:
		{
			CGPDummyAffect* pAffect = new CGPDummyAffect;
			*ppv = pAffect;
		}
		break;
	case EAI_FreezeGP:
		{
			CGPAffect_FreezeGP* pAffect = new CGPAffect_FreezeGP;
			*ppv = pAffect;
		}
		break;
	case EAI_Animation:
		{
			CGPAffect_Animation* pAffect = new CGPAffect_Animation;
			*ppv = pAffect;
		}
		break;
	case EAI_Effect:
		{
			CGPAffect_Effect* pAffect = new CGPAffect_Effect;
			*ppv = pAffect;
		}
		break;
	case EAI_Sound:
		{
			CGPAffect_Sound* pAffect = new CGPAffect_Sound;
			*ppv = pAffect;
		}
		break;
	case EAI_Speed:
		{
			CGPAffect_Speed* pAffect = new CGPAffect_Speed;
			*ppv = pAffect;
		}
		break;
	case EAI_InvertLR:
		{
			CGPReverseKeyAffect* pAffect = new CGPReverseKeyAffect;
			*ppv = pAffect;
		}
		break;
	case EAI_FreezeOp:
		{
			CGPLockKeyAffact* pAffect = new CGPLockKeyAffact;
			*ppv = pAffect;
		}
		break;
	case EAI_Lift:
		{
			CGPLiftCtrl* pAffect = new CGPLiftCtrl;
			*ppv = pAffect;
		}
		break;
	case EAI_Magnet:
		{
			CGPMagnetCtrl* pAffect = new CGPMagnetCtrl;
			*ppv = pAffect;
		}
		break;
	case EAI_Jet:
		{
			CGPJectAffect* pAffect = new CGPJectAffect;
			*ppv = pAffect;
		}
		break;
	case EAI_Evil:
		{
			CGPEvilCtrl* pAffect = new CGPEvilCtrl;
			*ppv = pAffect;
		}
		break;
	case EAI_Generator:
		{
			CGPAffect_Generator* pAffect = new CGPAffect_Generator;
			*ppv = pAffect;
		}
		break;
	case EAI_No2:
		{
			CGPNo2Affect* pAffect = new CGPNo2Affect;
			*ppv = pAffect;
		}
		break;
	case EAI_No2Emitter:
		{
			CGPAffect_No2Emitter* pAffect = new CGPAffect_No2Emitter;
			*ppv = pAffect;
		}
		break;
	default:
		{
			bRet = FALSE;
		}
	}
	return bRet;
}

void DestroyGPAffect(ENMAFFECTID id, IGPAffect* pv)
{
	assert(pv);
	DWORD dwID = (DWORD)id;
	if ( dwID < 0 || dwID > EAI_MAX )
		return;

	BOOL bRet = TRUE;
	switch(id)
	{
	case EAI_None:
		{
			CGPDummyAffect* pAffect = (CGPDummyAffect*)pv;
			delete pAffect;
		}
		break;
	case EAI_FreezeGP:
		{
			CGPAffect_FreezeGP* pAffect = (CGPAffect_FreezeGP*)pv;
			delete pAffect;
		}
		break;
	case EAI_Animation:
		{
			CGPAffect_Animation* pAffect = (CGPAffect_Animation*)pv;
			delete pAffect;
		}
		break;
	case EAI_Sound:
		{
			CGPAffect_Sound* pAffect = (CGPAffect_Sound*)pv;
			delete pAffect;
		}
		break;
	case EAI_Effect:
		{
			CGPAffect_Effect* pAffect = (CGPAffect_Effect*)pv;
			delete pAffect;
		}
		break;
	case EAI_Speed:
		{
			CGPAffect_Speed* pAffect = (CGPAffect_Speed*)pv;
			delete pAffect;
		}
		break;
	case EAI_InvertLR:
		{
			CGPReverseKeyAffect* pAffect = (CGPReverseKeyAffect*)pv;
			delete pAffect;
		}
		break;
	case EAI_FreezeOp:
		{
			CGPLockKeyAffact* pAffect = (CGPLockKeyAffact*)pv;
			delete pAffect;
		}
		break;
	case EAI_Lift:
		{
			CGPLiftCtrl* pAffect = (CGPLiftCtrl*)pv;
			delete pAffect;
		}
		break;
	case EAI_Magnet:
		{
			CGPMagnetCtrl* pAffect = (CGPMagnetCtrl*)pv;
			delete pAffect;
		}
		break;
	case EAI_Jet:
		{
			CGPJectAffect* pAffect = (CGPJectAffect*)pv;
			delete pAffect;
		}
		break;
	case EAI_Evil:
		{
			CGPEvilCtrl* pAffect =(CGPEvilCtrl*)pv;
			delete pAffect;
		}
		break;
	case EAI_Generator:
		{
			CGPAffect_Generator* pAffect = (CGPAffect_Generator*)pv;
			delete pAffect;
		}
		break;
	case EAI_No2:
		{
			CGPNo2Affect * pAffect = ( CGPNo2Affect * )pv;
			delete pAffect;
		}
		break;
	case EAI_No2Emitter:
		{
			CGPAffect_No2Emitter * pAffect = ( CGPAffect_No2Emitter * )pv;
			delete pAffect;
		}
		break;
	default:
		{
			bRet = FALSE;
		}
	}
	return;
}

BOOL CreateGPMovementControl(ENMMOVETYPE type, IGPMovementControl** ppv)
{
	BOOL bRet = TRUE;
	switch(type)
	{
	case EMT_Missile:
		{
			CGPMissileCtrl* pCtrl = new CGPMissileCtrl;
			*ppv = pCtrl;
		}
		break;
	case EMT_Flying:
		{
			CGPFlyingCtrl* pCtrl = new CGPFlyingCtrl;
			*ppv = pCtrl;
		}
		break;
	default:
		{
			XWaring("CreateGPMovementControl() invalid type : %d", type);
			bRet = FALSE;
		}
		break;
	}
	return bRet;
}

void DestroyGPMovementControl(ENMMOVETYPE type, IGPMovementControl* pv)
{
	BOOL bRet = TRUE;
	switch(type)
	{
	case EMT_Missile:
		{
			CGPMissileCtrl* pCtrl = (CGPMissileCtrl*)pv;
			delete pCtrl;
		}
		break;
	case EMT_Flying:
		{
			CGPFlyingCtrl* pCtrl = (CGPFlyingCtrl*)pv;
			delete pCtrl;
		}
		break;
	default:
		{
			XWaring("CreateGPMovementControl() invalid type : %d", type);
			bRet = FALSE;
		}
		break;
	}
	return;
}

BOOL CreateGPShelterIterator(IGPShelterIterator** ppv)
{
	XTrace("CreateGPShelterIterator暂时没实现");
	return FALSE;
}

void DestroyGPShelterIterator(IGPShelterIterator* pv)
{
	XTrace("CreateGPShelterIterator暂时没实现");
	return;
}

DWORD CGPTargetIterator::GetTargetCount()
{
	if ( CMonsterMgr::Get() )
		return NxPlayMgr::GetAllPlay().size() + CMonsterMgr::Get()->GetAllMonster().size();
	else
		return NxPlayMgr::GetAllPlay().size();
}

IGPTarget*	CGPTargetIterator::GetTarget(DWORD index)
{
	std::set< Play * >&		rPlays		= NxPlayMgr::GetAllPlay();
	std::set< Play * >		rActors	; rActors.empty();

	if ( CMonsterMgr::Get() )
		rActors = CMonsterMgr::Get()->GetAllMonster();

	std::set< Play * >::iterator it = rPlays.begin();
	for ( ; it != rPlays.end(); it++ )
	{
		rActors.insert(*it);
	}
	

	if ( index < 0 || index >= rActors.size() )
		return NULL;

	it = rActors.begin();
	for ( int i = 0; i < index; i++ )
	{
		it++;

	}
	return (IGPTarget*)*it;
}

IGPTarget*	CGPTargetIterator::GetHost()
{
	return (IGPTarget*)NxPlayMgr::GetHostPlay();
}

IGPTarget*	CGPTargetIterator::GetDummy()
{
	return &m_kDummyTarget;
}