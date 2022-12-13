#include "Top-KartPCH.h"
#include "AvatarBase.h"
#include "ATBehaveBase.h"
#include "ATAlgorithmBase.h"
#include "ATTransferAlg.h"
#include "ATAnimBehave.h"
#include "ATEffectBehave.h"
#include "ATSoundBehave.h"

CAvatarBase::CAvatarBase():
m_bCanSwitch(0),
m_bNoFog(0)
{

}

CAvatarBase::~CAvatarBase()
{
	list<CATBehaveBase*>::iterator behave_it = m_listActiveBehave.begin();
	for(; behave_it != m_listActiveBehave.end(); ++behave_it)
	{
		SAFE_DELETE(*behave_it);
	}
	m_listActiveBehave.clear();

	for(behave_it = m_listInactiveBehave.begin(); behave_it != m_listInactiveBehave.end(); ++behave_it)
	{
		SAFE_DELETE(*behave_it);
	}
	m_listInactiveBehave.clear();

	list<CATAlgorithmBase*>::iterator alg_it = m_listGPTranAlgthm.begin();
	for(; alg_it != m_listGPTranAlgthm.end(); ++alg_it)
	{
		SAFE_DELETE(*alg_it);
	}
	m_listGPTranAlgthm.clear();

	alg_it = m_listGPLifeAlgthm.begin();
	for(; alg_it != m_listGPLifeAlgthm.end(); ++alg_it)
	{
		SAFE_DELETE(*alg_it);
	}
	m_listGPLifeAlgthm.clear();

	m_listAnim.clear();
}

VOID CAvatarBase::Create(DWORD ID, AVATARPROPERTY tagPropt)
{
	m_dwID = ID;
	m_bCanSwitch = tagPropt.shSwitch;
	m_bNoFog = tagPropt.shNoFog;
	m_tagPhyPropt = tagPropt.tagPhyPrp;
	m_listAnim = tagPropt.listAnimData;

    BehaveDataList_It bd_it = tagPropt.listBehaveData.begin();
	for(; bd_it != tagPropt.listBehaveData.end(); ++bd_it)
	{
		CATBehaveBase* pBehave; 
		switch((*bd_it).dwType)
		{
		case EBI_Animation:
			{
				pBehave = new CATAnimBehave(*bd_it);
			}
			break;
		case EBI_Effect:
			{
				pBehave = new CATEffectBehave(*bd_it);
			}
			break;
		case EBI_Sound:
			{
				pBehave = new CATSoundBehave(*bd_it);
			}
			break;
		default:
			break;
		}
		m_listInactiveBehave.push_back(pBehave);
	}

	AlgDataList_It ad_it = tagPropt.listAlgData.begin();
	for(; ad_it != tagPropt.listAlgData.end(); ++ad_it)
	{
		CATAlgorithmBase* pAlg;
		switch((*ad_it).dwType)
		{
		case EAI_GPTansfer:
			{
				pAlg = new CATTransferAlg(*ad_it);
				m_listGPTranAlgthm.push_back(pAlg);
			}
			break;
		case EAI_GPReduceTime:
			{
				pAlg = new CATTimeTranAlg(*ad_it);
				m_listGPLifeAlgthm.push_back(pAlg);
			}
			break;
		default:
			break;
		}		
	}
}

DWORD CAvatarBase::GetID()
{
	return m_dwID;
}

BOOL CAvatarBase::CanSwitch()
{
	return m_bCanSwitch;
}

BOOL CAvatarBase::CanThroughFog()
{
	return m_bNoFog;
}

PHYSICPROPERTY CAvatarBase::GetPhysicPropt()
{
	return m_tagPhyPropt;
}

DWORD CAvatarBase::AnimTransform(DWORD dwSource)
{
	AnimDataList_It anim_it = m_listAnim.begin();
	for(; anim_it != m_listAnim.end(); ++anim_it)
	{
		if(dwSource == (*anim_it).dwCondition)
		{
			return (*anim_it).dwExpectation;
		}
	}
	return dwSource;
}

DWORD CAvatarBase::GPTransform(DWORD dwSource)
{
	list<CATAlgorithmBase*>::const_iterator alg_it = m_listGPTranAlgthm.begin();
	DWORD dwResult;
	for(; alg_it != m_listGPTranAlgthm.end(); ++alg_it)
	{
		dwResult = (*alg_it)->Calculate(dwSource);
		if(dwResult != dwSource)
		{
			return dwResult;
		}
	}
	return dwSource;
}

DWORD CAvatarBase::GPLifeTransfrom(DWORD dwCondition, DWORD dwLife)
{
	list<CATAlgorithmBase*>::const_iterator alg_it = m_listGPLifeAlgthm.begin();
	DWORD dwResult;
	for(; alg_it != m_listGPLifeAlgthm.end(); ++alg_it)
	{
		dwResult = (*alg_it)->Calculate(dwCondition);
		if(dwResult != DWORD(-1))
		{
			return dwResult;
		}
	}
	return dwLife;
}


VOID CAvatarBase::TriggerBehave(DWORD dwType, DWORD dwOccasion)
{
	list<CATBehaveBase*>::iterator bh_it = m_listInactiveBehave.begin();
	for(; bh_it != m_listInactiveBehave.end(); ++bh_it)
	{
		if((*bh_it)->GetType() == dwType && (*bh_it)->GetOccasion() == dwOccasion)
		{
			(*bh_it)->Start();
			m_listActiveBehave.push_back(*bh_it);
			m_listInactiveBehave.erase(bh_it);			
		}
	}
}

VOID CAvatarBase::DeactivateBehave(CATBehaveBase* pBehave)
{
	m_listInactiveBehave.push_back(pBehave);
	m_listActiveBehave.remove(pBehave);
}

VOID CAvatarBase::Update(float fTime)
{
	list<CATBehaveBase*>::const_iterator bh_it = m_listActiveBehave.begin();
	for(; bh_it != m_listActiveBehave.end(); ++bh_it)
	{
		if((*bh_it)->IsActive())
		{
			(*bh_it)->Update(fTime);
		}
		else
		{
			DeactivateBehave(*bh_it);
		}	
	}
}

VOID CAvatarBase::SetWearer(CATWearer* pWearer)
{
	list<CATBehaveBase*>::const_iterator bh_it = m_listInactiveBehave.begin();
	for(; bh_it != m_listInactiveBehave.end(); ++bh_it)
	{
		(*bh_it)->SetWearer(pWearer);
	}
	for(bh_it = m_listActiveBehave.begin(); bh_it != m_listActiveBehave.end(); ++bh_it)
	{
		(*bh_it)->SetWearer(pWearer);
	}
}