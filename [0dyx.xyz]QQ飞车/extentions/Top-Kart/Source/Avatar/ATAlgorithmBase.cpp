#include "Top-KartPCH.h"
#include "ATAlgorithmBase.h"

CATAlgorithmBase::CATAlgorithmBase(ALGDATA tagALGD):
m_dwType(tagALGD.dwType),
m_dwOccasion(tagALGD.dwOccasion),
m_shOpertor(tagALGD.shOpertor),
m_dwCondition(tagALGD.dwCondition),
m_dwExpectation(tagALGD.dwExpectation),
m_dwRate(tagALGD.dwRate)
{

}

CATAlgorithmBase::~CATAlgorithmBase()
{

}

void CATAlgorithmBase::SetType(DWORD dwType)
{
	m_dwType = dwType;
}

void CATAlgorithmBase::SetOccasion(DWORD dwOccasion)
{
	m_dwOccasion = dwOccasion;
}

void CATAlgorithmBase::SetOperator(SHORT shOp)
{
	m_shOpertor = shOp;
}

void CATAlgorithmBase::SetCondition(DWORD dwCondition)
{
	m_dwCondition = dwCondition;
}

void CATAlgorithmBase::SetExpectation(DWORD dwExp)
{
	m_dwExpectation = dwExp;
}

void CATAlgorithmBase::SetRate(DWORD dwRate)
{
	m_dwRate = dwRate;
}

DWORD CATAlgorithmBase::GetType()
{
	return m_dwType;
}

DWORD CATAlgorithmBase::GetOccasion()
{
	return m_dwOccasion;
}

DWORD CATAlgorithmBase::Calculate(DWORD dwParam)
{
	return dwParam;
}
