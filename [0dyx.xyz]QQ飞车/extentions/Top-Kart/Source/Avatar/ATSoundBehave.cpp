#include "Top-KartPCH.h"
#include "ATSoundBehave.h"
#include "../soundmgr.h"


CATSoundBehave::CATSoundBehave(BEHAVEDATA tagBD):
CATBehaveBase(tagBD)
{

}

CATSoundBehave::~CATSoundBehave()
{

}

void CATSoundBehave::Start()
{
	CATBehaveBase::Start();
	m_dwSound = CSoundMgr::Get()->PlaySound(m_dwParam);
}

void CATSoundBehave::Stop()
{
	CATBehaveBase::Stop();
	CSoundMgr::Get()->StopSound(m_dwSound);
}

void CATSoundBehave::Update(float fTime)
{
	CATBehaveBase::Update(fTime);
}