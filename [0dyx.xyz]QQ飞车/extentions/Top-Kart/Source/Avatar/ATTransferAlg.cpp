#include "Top-KartPCH.h"
#include "ATTransferAlg.h"
#include <ctime>
#include <cstdlib>


CATTransferAlg::CATTransferAlg(ALGDATA tagALGD):
CATAlgorithmBase(tagALGD)
{

}

CATTransferAlg::~CATTransferAlg()
{

}

DWORD CATTransferAlg::Calculate(DWORD dwParam)
{
	if(dwParam == m_dwCondition)
	{
		srand((unsigned)time(NULL));
		if((rand()%100) < m_dwRate)
		{
			return m_dwExpectation;
		}
	}
	return dwParam;
}

//-------------------------------------------------------------------------
CATTimeTranAlg::CATTimeTranAlg(ALGDATA tagAlGD):
CATAlgorithmBase(tagAlGD)
{

}

CATTimeTranAlg::~CATTimeTranAlg()
{

}

DWORD CATTimeTranAlg::Calculate(DWORD dwParam)
{
	if(dwParam == m_dwCondition)
	{
		if((rand()%100) < m_dwRate)
		{
			return m_dwExpectation;
		}
	}
	return DWORD(-1);
}