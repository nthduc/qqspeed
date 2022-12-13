#include "Top-KartPCH.h"
#include "GPNo2Affect.h"
#include "Play.h"
#include "NxPlayMgr.h"
#include "gpcontrol.h"
#include "gpeffect.h"
#include "PhysKart/TencentCar.h"
#include "gpdefine.h"
#include "soundmgr.h"


typedef IntToStringMulMap::const_iterator IntToStrIter;
typedef pair<IntToStrIter, IntToStrIter> Range;


CGPNo2Affect::CGPNo2Affect():
//m_dwSound(0),
m_fForceX(0),
m_unStartTime(0),
m_spAcceNode(0)
{
	for(unsigned int i = 0; i < MAX_PIPES; ++i)
	{
		m_spEffectNode[i] = 0;
	}
	m_unNumOfEffects = 0;
}


CGPNo2Affect::~CGPNo2Affect()
{
	for(int i = 0; i < m_unNumOfEffects; ++i)
	{
		m_spEffectNode[i] = 0;
	}
	m_spAcceNode = 0;
}

void CGPNo2Affect::Start()
{
	m_unStartTime = GetTickCount();
	m_unNumOfEffects = 0;
	if ( m_pDestTarget )
	{
		m_pDestTarget->PlayAnimation(1306, true);
		/*if(m_pDestTarget == CGPControl::Get()->GetTargetIterator()->GetHost())
		{
			m_dwSound = CSoundMgr::Get()->PlaySound(106);
		}*/

		m_unTargerID = m_pDestTarget->GetID();	

		if ( !NxPlayMgr::GetPlay(m_unTargerID))   // comment Npc bug test  hoverzhao
			return;

		std::list<CTencentCar::PHYSINFO> phyList = NxPlayMgr::GetPlay(m_unTargerID)->GetKart()->GetVehicle()->GetCarPhysParam();
		std::list<CTencentCar::PHYSINFO>::const_iterator phyList_it = phyList.begin();
		for(; phyList_it != phyList.end(); ++phyList_it)
		{
			if(m_fParam == phyList_it->dwPID)
			{
				SetLifeTime(phyList_it->fLifeTime);
				m_fForceX = phyList_it->fParam;
			}
		}
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
		m_spAcceNode = CGPEffectMgr::Get()->NewEffect(7, TRUE);
		m_pDestTarget->Attach(m_spAcceNode);
		CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_SMOKE));		
	}
	CGPAffectBase::Start();
}


void CGPNo2Affect::Stop()
{	
	m_pDestTarget->StopAnimation(1306, TRUE);
	m_pDestTarget->Detach(m_spAcceNode);
	GP_DATA_LIST& gpList = m_pDestTarget->GetAffectGPs();
	GP_DATA_LIST_Iterator gpList_it = gpList.begin();
	for(; gpList_it != gpList.end(); ++gpList_it)
	{
		if(gpList_it->dwID == m_fParam)
		{
			gpList_it->dwUseTime = 0;
			break;
		}
	}
	for(unsigned int i = 0; i < m_unNumOfEffects; ++i)
	{
		if(NxPlayMgr::GetPlay(m_unTargerID))
		{
			NxPlayMgr::GetPlay(m_unTargerID)->DetachDummy(m_strNodeName[i], m_spEffectNode[i]);
		}
	}

	/*if ( m_dwSound )
	{
		CSoundMgr::Get()->StopSound(m_dwSound);
	}*/
	CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SMOKE, 0));
	CGPAffectBase::Stop();
}

void CGPNo2Affect::Update(DWORD)
{

//	m_pDestTarget->PlayAnimation(1306, TRUE);
	IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
	//如果是加速效果，用加力来实现
	if ( pIter && m_pDestTarget == pIter->GetHost())
	{
		NiPoint3 kVel = m_pDestTarget->GetVelocity(TRUE);
		//如果它没有速度就不要再折磨它了
		float fForce[3] = { m_fForceX, 0, 0 };
		m_pDestTarget->AddForce(fForce, TRUE);
	}
	if(GetTickCount() - m_unStartTime > m_dwLifeTime)
	{
		Stop();
	}
}