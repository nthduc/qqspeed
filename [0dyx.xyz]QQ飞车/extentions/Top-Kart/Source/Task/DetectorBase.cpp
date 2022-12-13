#include "Top-KartPCH.h"
#include "DetectorBase.h"

CDetectorBase::CDetectorBase():
m_dwState(EDS_Prepair)
{

}

CDetectorBase::~CDetectorBase()
{

}

void CDetectorBase::_SetState(DWORD dwState)
{
	m_dwState = dwState;
}

void CDetectorBase::SetState(DWORD dwState)
{
	m_dwState = dwState;
}

DWORD CDetectorBase::GetState()
{	
	return m_dwState;
}

void CDetectorBase::Start()
{
	m_dwState = EDS_Executing;
}

void CDetectorBase::Succeed()
{
	m_dwState = EDS_Success;
}

void CDetectorBase::Fail()
{
	m_dwState = EDS_Fail;
}

void CDetectorBase::Update(float fTime)
{
	if(m_dwState != EDS_Executing)
	{
		NotifyObserver();
		return;
	}	
}

bool CDetectorBase::LoadConfig(CVarTable* pTable)
{
	return true;
}