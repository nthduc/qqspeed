#include "Top-KartPCH.h"
#include "ATBehaveBase.h"


CATBehaveBase::CATBehaveBase(BEHAVEDATA tagBD):
m_dwType(tagBD.dwType),
m_dwOccasion(tagBD.dwOccasion),
m_dwCondition(tagBD.dwCondition),
m_dwLife(tagBD.dwLife),		
m_dwParam(tagBD.dwParam),
m_dwOption(tagBD.dwOption),
m_pWearer(0),	
m_bIsAcive(0),
m_dwStartTime(0)
{
};

CATBehaveBase::~CATBehaveBase()
{

}

void CATBehaveBase::SetWearer(CATWearer* pTarget)
{
	m_pWearer = pTarget;
}

bool CATBehaveBase::IsActive()
{
	return m_bIsAcive;
}

void CATBehaveBase::Start()
{
	m_dwStartTime = GetTickCount();
	m_bIsAcive = true;
}

void CATBehaveBase::Stop()
{
	m_bIsAcive = false;
}

void CATBehaveBase::SetLifeTime(DWORD dwLift)
{
	m_dwLife = dwLift;
}

void CATBehaveBase::SetOccasion(DWORD dwOccasion)
{
	m_dwOccasion = dwOccasion;
}

void CATBehaveBase::SetCondition(DWORD dwCondt)
{
	m_dwCondition = dwCondt;
}

void CATBehaveBase::SetParam(DWORD dwParam)
{
	m_dwParam = dwParam;
}

DWORD CATBehaveBase::GetType()
{
	return m_dwType;
}

DWORD CATBehaveBase::GetOccasion()
{
	return m_dwOccasion;
}

DWORD CATBehaveBase::GetConditioin()
{
	return m_dwCondition;
}

void CATBehaveBase::Update(float fTime)
{
	if((GetTickCount() - m_dwStartTime) > m_dwLife)
	{
		m_bIsAcive = false;
	}
}


