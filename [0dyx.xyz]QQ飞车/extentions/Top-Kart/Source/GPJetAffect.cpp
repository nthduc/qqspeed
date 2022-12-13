#include "Top-KartPCH.h"

#include "GPJetAffect.h"
#include "NxPlayMgr.h"
#include "Play.h"
#include "gpcontrol.h"
#include "gpeffect.h"

typedef IntToStringMulMap::const_iterator IntToStrIter;
typedef pair<IntToStrIter, IntToStrIter> Range;

CGPJectAffect::CGPJectAffect()
{

	for(unsigned int i = 0; i < MAX_PIPES; ++i)
	{
		m_spEffectNode[i] = 0;
	}
	m_unNumOfEffects = 0;
	
}

CGPJectAffect::~CGPJectAffect()
{
	for(int i = 0; i < m_unNumOfEffects; ++i)
	{
		m_spEffectNode[i] = 0;
	}
}


void CGPJectAffect::Start()
{

	m_unTargerID = m_pDestTarget->GetID();	

	Range effRange = NxPlayMgr::GetPlay(m_unTargerID)->m_stCurPlayerDesc.mapKartEffect.equal_range(32);
	for( IntToStrIter iter = effRange.first; iter != effRange.second; ++iter)
	{
		m_spEffectNode[m_unNumOfEffects] = CGPEffectMgr::Get()->NewEffect(32, TRUE);
		m_strNodeName[m_unNumOfEffects] = iter->second.c_str();
		NxPlayMgr::GetPlay(m_unTargerID)->AttachDummy(m_strNodeName[m_unNumOfEffects], m_spEffectNode[m_unNumOfEffects]);
		m_unNumOfEffects++;
	}

	effRange = NxPlayMgr::GetPlay(m_unTargerID)->m_stCurPlayerDesc.mapKartEffect.equal_range(33);

	for( IntToStrIter iter = effRange.first; iter != effRange.second; ++iter)
	{
		m_spEffectNode[m_unNumOfEffects] = CGPEffectMgr::Get()->NewEffect(33, TRUE);
		m_strNodeName[m_unNumOfEffects] = iter->second.c_str();
		NxPlayMgr::GetPlay(m_unTargerID)->AttachDummy(m_strNodeName[m_unNumOfEffects], m_spEffectNode[m_unNumOfEffects]);
		m_unNumOfEffects++;
	}


	CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_SMOKE));
	CGPAffectBase::Start();
}

void CGPJectAffect::Stop()
{
	for(unsigned int i = 0; i < m_unNumOfEffects; ++i)
	{
		if(NxPlayMgr::GetPlay(m_unTargerID))
		{
			NxPlayMgr::GetPlay(m_unTargerID)->DetachDummy(m_strNodeName[i], m_spEffectNode[i]);
		}
	}
	
	CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SMOKE, 0));
	CGPAffectBase::Stop();
}
