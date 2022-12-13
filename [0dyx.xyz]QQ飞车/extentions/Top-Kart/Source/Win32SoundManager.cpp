

#include "Top-KartPCH.h"

#ifndef NO_SOUND

#include <NiApplication.h>
#include <NiAudio.h>

#include "SoundSystemSpecificInitialization.h"
#include "Win32SoundManager.h"
//#include "LevelManager.h"

//---------------------------------------------------------------------------
Win32SoundManager::Win32SoundManager()
{
	for (int i = 0; i < MAX_SOUND_STATES; ++i)
		m_aspSound[i] = 0;
}
//---------------------------------------------------------------------------
Win32SoundManager::~Win32SoundManager() 
{
    // Shutdown the Sound System
	for (int i = 0; i < MAX_SOUND_STATES; ++i)
		m_aspSound[i] = 0;
    NiAudioSystem::GetAudioSystem()->Shutdown();
}
//---------------------------------------------------------------------------
NiAVObject* Win32SoundManager::GetListener()
{
    return NiAudioSystem::GetAudioSystem()->GetListener();
}
//---------------------------------------------------------------------------
void Win32SoundManager::Update(float fTime)
{
    //
    // Click the sound system.  'True' parameter indicates
    // that all sources should be updated.
    //

    NiAudioSystem::GetAudioSystem()->Update(fTime, true);
}
//---------------------------------------------------------------------------
bool Win32SoundManager::Initialize()
{
    //
    // Start the sound system
    //
    if (!SoundSystemSpecificInitialization())
	{
		m_bIsSound = false;
		return false;
	}

    NiAudioSystem* pkSS = NiAudioSystem::GetAudioSystem();
    pkSS->SetSpeakerType(NiAudioSystem::TYPE_3D_2_SPEAKER);
    // inches
    pkSS->SetUnitsPerMeter(39.37f);
    
    // Create the background music
	m_aspSound[SoundType::ST_AMBIENT] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_AMBIENT]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/beijin.wav"));

	m_aspSound[SoundType::ST_AMBIENT]->SetStreamed(true);
	m_aspSound[SoundType::ST_AMBIENT]->Load();

	m_aspSound[SoundType::ST_AMBIENT]->SetGain(0.4f);
	m_aspSound[SoundType::ST_AMBIENT]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_AMBIENT]->Update(0);


	// Create the room background music
	m_aspSound[SoundType::ST_ROOM] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_ROOM]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/room.wav"));

	m_aspSound[SoundType::ST_ROOM]->SetStreamed(true);
	m_aspSound[SoundType::ST_ROOM]->Load();

	m_aspSound[SoundType::ST_ROOM]->SetGain(0.55f);
	m_aspSound[SoundType::ST_ROOM]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_ROOM]->Update(0);


	// create the motor sound
	m_aspSound[SoundType::ST_MOTOR_LOW] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_MOTOR_LOW]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/engine_low.wav"));

	m_aspSound[SoundType::ST_MOTOR_LOW]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_MOTOR_LOW]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_low.wav\n");
	}

	m_aspSound[SoundType::ST_MOTOR_LOW]->SetGain(0.2f);
	m_aspSound[SoundType::ST_MOTOR_LOW]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_MOTOR_LOW]->Update(0);


	//the motor sound of the mid speed
	m_aspSound[SoundType::ST_MOTOR_MID] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_MOTOR_MID]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/motor.wav"));

	m_aspSound[SoundType::ST_MOTOR_MID]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_MOTOR_MID]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_mid.wav\n");
	}

	m_aspSound[SoundType::ST_MOTOR_MID]->SetGain(0.40f);
	m_aspSound[SoundType::ST_MOTOR_MID]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_MOTOR_MID]->Update(0);

	//the motor sound of the high speed
	m_aspSound[SoundType::ST_MOTOR_HIGH] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_MOTOR_HIGH]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/engine-high.wav"));

	m_aspSound[SoundType::ST_MOTOR_HIGH]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_MOTOR_HIGH]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_high.wav\n");
	}

	m_aspSound[SoundType::ST_MOTOR_HIGH]->SetGain(0.25f);
	m_aspSound[SoundType::ST_MOTOR_HIGH]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_MOTOR_HIGH]->Update(0);

	//when the car is stop the motor's sound
	m_aspSound[SoundType::ST_MOTOR_STOP] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_MOTOR_STOP]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/stopmotor.wav"));

	m_aspSound[SoundType::ST_MOTOR_STOP]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_MOTOR_STOP]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_low.wav\n");
	}

	m_aspSound[SoundType::ST_MOTOR_STOP]->SetGain(0.1f);
	m_aspSound[SoundType::ST_MOTOR_STOP]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_MOTOR_STOP]->Update(0);

	//shift sound
	m_aspSound[SoundType::ST_SHIFT] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_SHIFT]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/drift.wav"));

	m_aspSound[SoundType::ST_SHIFT]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_SHIFT]->Load())
	{
		OutputDebugString("SoundManager -- failed to load shift.wav\n");
	}

	m_aspSound[SoundType::ST_SHIFT]->SetGain(0.9f);
	m_aspSound[SoundType::ST_SHIFT]->SetLoopCount(NiAudioSource::LOOP_SINGLE);
	m_aspSound[SoundType::ST_SHIFT]->Update(0);

	//hit sound
	m_aspSound[SoundType::ST_HIT] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_HIT]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/crash.wav"));

	m_aspSound[SoundType::ST_HIT]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_HIT]->Load())
	{
		OutputDebugString("SoundManager -- failed to load shift.wav\n");
	}

	m_aspSound[SoundType::ST_HIT]->SetGain(0.8f);
	m_aspSound[SoundType::ST_HIT]->SetLoopCount(NiAudioSource::LOOP_SINGLE);
	m_aspSound[SoundType::ST_HIT]->Update(0);


	//speedup sound
	m_aspSound[SoundType::ST_SPEEDUP] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_SPEEDUP]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/booster.wav"));

	m_aspSound[SoundType::ST_SPEEDUP]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_SPEEDUP]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_high.wav\n");
	}

	m_aspSound[SoundType::ST_SPEEDUP]->SetGain(0.8f);
	m_aspSound[SoundType::ST_SPEEDUP]->SetLoopCount(NiAudioSource::LOOP_SINGLE);
	m_aspSound[SoundType::ST_SPEEDUP]->Update(0);
/////////////////carbarn sound
	m_aspSound[SoundType::ST_CARBARN] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_CARBARN]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/carbarn.wav"));

	m_aspSound[SoundType::ST_CARBARN]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_CARBARN]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_high.wav\n");
	}

	m_aspSound[SoundType::ST_CARBARN]->SetGain(0.4f);
	m_aspSound[SoundType::ST_CARBARN]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_CARBARN]->Update(0);

	
	//wait start sound
	m_aspSound[SoundType::ST_WAIT_START] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_WAIT_START]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/waitstart.wav"));

	m_aspSound[SoundType::ST_WAIT_START]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_WAIT_START]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_high.wav\n");
	}

	m_aspSound[SoundType::ST_WAIT_START]->SetGain(0.4f);
	m_aspSound[SoundType::ST_WAIT_START]->SetLoopCount(NiAudioSource::LOOP_INFINITE);
	m_aspSound[SoundType::ST_WAIT_START]->Update(0);

	//go sound
	m_aspSound[SoundType::ST_GO] = 
		pkSS->CreateSource(NiAudioSource::TYPE_AMBIENT);
	m_aspSound[SoundType::ST_GO]->SetFilename(
		NiApplication::ConvertMediaFilename("SOUNDS/count_go.wav"));

	m_aspSound[SoundType::ST_GO]->SetStreamed(true);
	if(!m_aspSound[SoundType::ST_GO]->Load())
	{
		OutputDebugString("SoundManager -- failed to load engine_high.wav\n");
	}

	m_aspSound[SoundType::ST_GO]->SetGain(0.4f);
	m_aspSound[SoundType::ST_GO]->SetLoopCount(NiAudioSource::LOOP_SINGLE);
	m_aspSound[SoundType::ST_GO]->Update(0);


    // Update the sound system for all of the sounds
    pkSS->Update(0.0f, true);

    return true;
}
//---------------------------------------------------------------------------
void Win32SoundManager::ToggleSounds()
{
	if(m_bIsSound)
	{
		assert(m_aspSound[SoundType::ST_AMBIENT]);

		NiAudioSource::Status eStatus = m_aspSound[SoundType::ST_AMBIENT]->GetStatus();
		if (eStatus == NiAudioSource::PLAYING)
		{
			m_aspSound[SoundType::ST_AMBIENT]->Stop();
		}
		else// if (eStatus == NiAudioSource::STOPPED)
		{
			// Miles does not return a valid stopped status so always play
			m_aspSound[SoundType::ST_AMBIENT]->Play();
		}

	}
	

}

//--------------------------------------------------------------------------

void Win32SoundManager::SetPlaybackRate(SoundType type, long rate)
{
	if(m_bIsSound)
	m_aspSound[type]->SetPlaybackRate(rate);
	
}
//---------------------------------------------------------------------------
void Win32SoundManager::PlaySound(SoundType type)
{
	if(m_bIsSound)
	{
		assert(m_aspSound[type]);
		if(m_aspSound[type]->GetStatus() != NiAudioSource::PLAYING)
			m_aspSound[type]->Play();
	}
}

//-----------------------------------------------------------------------------
void Win32SoundManager::StopSound(SoundType type)
{
	if(m_bIsSound)
	{
		assert(m_aspSound[type]);
		if(m_aspSound[type]->GetStatus() == NiAudioSource::PLAYING)
		{
			m_aspSound[type]->Stop();
		}
	}
	
}

void Win32SoundManager::PlayMotorSound(float curSpeed)
{
	if(m_bIsSound)
	{
		float gain = (0.2 + curSpeed*0.01);
		if(gain > 0.7)
		{
			gain = 0.7;
		}
		float rate = 14000+curSpeed*50.0f;
		if(rate > 23200)
		{
			rate = 23200;
		}
		m_aspSound[ST_MOTOR_MID]->SetGain(gain);
		m_aspSound[ST_MOTOR_MID]->SetPlaybackRate(rate);
		PlaySound(ST_MOTOR_MID);
	}
}

void Win32SoundManager::StopAllSound()
{
	for(int i = 0; i < MAX_SOUND_STATES; i++)
	{
		if(m_aspSound[i])
		{
			m_aspSound[i]->Stop();
		}
	}
}

void Win32SoundManager::SetSource(unsigned int unType, NiFixedString strName)
{
	m_aspSound[unType]->Unload();
	m_aspSound[unType]->SetFilename(strName);
	m_aspSound[unType]->SetStreamed(true);
	m_aspSound[unType]->Load();
}

#endif // NO_SOUND
