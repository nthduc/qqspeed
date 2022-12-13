#include "Top-KartPCH.h"
#include "ATEffectBehave.h"
#include "../gpeffect.h"


CATEffectBehave::CATEffectBehave(BEHAVEDATA tagBD):
CATBehaveBase(tagBD)
{

}

CATEffectBehave::~CATEffectBehave()
{
	m_spEffect = 0;
}

void CATEffectBehave::Start()
{
	CATBehaveBase::Start();
	m_spEffect = CGPEffectMgr::Get()->NewEffect(m_dwParam);
	if(m_dwOption & EATT_World)
	{
		
	}
	else 
	{

	}
}

void CATEffectBehave::Stop()
{
	CATBehaveBase::Stop();
	if(m_dwOption & EATT_World)
	{

	}
	else 
	{

	}
}

void CATEffectBehave::Update(float fTime)
{
	CATBehaveBase::Update(fTime);
}