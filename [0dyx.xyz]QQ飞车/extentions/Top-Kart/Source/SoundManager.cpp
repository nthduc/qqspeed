

#include "Top-KartPCH.h"

#include "SoundManager.h"

//---------------------------------------------------------------------------
SoundManager::SoundManager()
{
    /* */
}
//---------------------------------------------------------------------------
SoundManager::~SoundManager() 
{
    /* */
}
//---------------------------------------------------------------------------
NiAVObject* SoundManager::GetListener()
{
    return 0;
}
//---------------------------------------------------------------------------
void SoundManager::Update(float fTime)
{
    /* */
}
//---------------------------------------------------------------------------
bool SoundManager::Initialize()
{
    return true;
}
//---------------------------------------------------------------------------
void SoundManager::ToggleSounds()
{
    /* */
}
//---------------------------------------------------------------------------
void SoundManager::PlaySound(SoundType type)
{
	/* */
}
//---------------------------------------------------------------------------
void SoundManager::StopSound(SoundType type)
{
	/* */
}
//---------------------------------------------------------------------------
void  SoundManager::PlayMotorSound(float curSpeed)
{
	/* */
}

//----------------------------------------------------------------------------
void SoundManager::SetPlaybackRate(SoundType type, long rate)
{
	/* */
}
//----------------------------------------------------------------------------
void SoundManager::StopAllSound()
{
	/* */
}
void SoundManager::SetSource(unsigned int unType, NiFixedString strName)
{
	/* */
}