#include "Top-KartPCH.h"
#include "AtomBase.h"


CAtomBase::CAtomBase()
{

}

CAtomBase::~CAtomBase()
{

}

void CAtomBase::Start()
{
	return;
}

void CAtomBase::Finish()
{
	_Stop();
}

void CAtomBase::Fail()
{
	_Stop();
}

void CAtomBase::Update(float fTime)
{
	return;
}

void CAtomBase::SetParam(DWORD dwParam)
{
	m_dwParam = dwParam;
}

void CAtomBase::SetOption(DWORD dwOption)
{
	m_dwOption = dwOption;
}

void CAtomBase::_Stop()
{
	return;
}

