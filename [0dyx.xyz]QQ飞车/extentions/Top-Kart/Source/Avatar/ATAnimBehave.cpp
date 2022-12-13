#include "Top-KartPCH.h"
#include "ATAnimBehave.h"


CATAnimBehave::CATAnimBehave(BEHAVEDATA tagBD):
CATBehaveBase(tagBD)
{

}

CATAnimBehave::~CATAnimBehave()
{

}

void CATAnimBehave::Start()
{
	CATBehaveBase::Start();
}

void CATAnimBehave::Stop()
{
	CATBehaveBase::Stop();
}

void CATAnimBehave::Update(float fTime)
{
	CATBehaveBase::Update(fTime);
}


