#include "Top-KartPCH.h"
#include ".\csound.h"

CSound::CSound(void)
{
}

CSound::~CSound(void)
{
}

CChannel::CChannel()
{
	m_pChannel	= NULL;
	m_pRolloff	= NULL;
}

CChannel::CChannel(FMOD::Channel* pChannel)
{
	m_pChannel	= pChannel;
	m_pRolloff	= NULL;
}

CChannel::~CChannel()
{
	if(m_pRolloff)
		delete[] m_pRolloff;
}

void	CChannel::SetRolloff(const FMOD_VECTOR* prolloff, int ikeynum, float fratio)
{
	m_pRolloff = new FMOD_VECTOR[ikeynum];
	for(int i = 0; i < ikeynum; i ++)
	{
		m_pRolloff[i].x = prolloff[i].x * fratio;
		m_pRolloff[i].y = prolloff[i].y;
		m_pRolloff[i].z = prolloff[i].z;
	}
	m_pChannel->set3DCustomRolloff(m_pRolloff, ikeynum);
}
