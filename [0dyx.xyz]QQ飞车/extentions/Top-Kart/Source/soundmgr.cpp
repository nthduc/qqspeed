////////////////////////////////////////////////
//     FileName:    soundmgr.cpp
//     Author:      Fish
//     Date:        2007-2-2 9:38:03
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "soundmgr.h"
#include "advtime.h"
//#include <NiAudio.h>
#include "common.h"
#include "gpscript.h"
#include "./fmod/FMODSystem.h"

CSoundMgr::CSoundMgr():
m_pFMODSystem(NULL)
{
	Initialize();
}

CSoundMgr::~CSoundMgr()
{
	UnInitialize();
}

int CSoundMgr::Initialize()
{
	//m_pAudioSystem = NiAudioSystem::GetAudioSystem();
	m_pFMODSystem = CFMODSystem::Get();

	CScriptState ls; 
	if (0 == ls.DoFile("config/soundid.lua") )
	{
		m_listConfig.clear();
		XTrace("加载了声音脚本配置文件，开始读取配置...");
		CVarArray varRoot("sounds");
		varRoot.GetFromScript(&ls);
		for ( int i = 0; i < varRoot.GetNum(); ++i )
		{
			SOUND_CONFIG kConfig;
			CVarTable* pVar = (CVarTable*)varRoot.Element(i);
			if ( pVar )
			{
				_ReadNumFromVar(&kConfig.dwSoundID, pVar->Element("id"));
				_ReadStringFromVar(kConfig.strFileName, pVar->Element("filename"));
				_ReadNumFromVar(&kConfig.fGain, pVar->Element("gain"));
				_ReadNumFromVar(&kConfig.nLoop, pVar->Element("loop"));
			}
			m_listConfig.push_back(kConfig);
		}
	}
	return m_listConfig.size();
}

void CSoundMgr::UnInitialize()
{
	m_listConfig.clear();
	//SOUND_DATA_LIST_Iterator it = m_listSound.begin();
	//for ( ; it != m_listSound.end(); ++it )
	//{
	//	LPSOUND_DATA pData = &*it;
	//	if ( pData && pData->pSound )
	//	{
	//		pData->pSound->Stop();
	//		pData->pSound = NULL;
	//	}
	//}
	//m_listSound.clear();
	m_kIDGen.Reset();
}

DWORD CSoundMgr::PlaySound(DWORD dwSoundID, BOOL bNewInstance)
{
	if ( !m_pFMODSystem )
	{
		return 0;
	}
	DWORD dwSoundIndex = 0;
	LPSOUND_CONFIG pConfig = FindConfig(dwSoundID);
	if ( !pConfig )
	{
		XWaring("FindConfig(soundid=%d) failed", dwSoundID);
		dwSoundIndex = 0;
		//goto KK_EXIT;
	}
	else
	{
		m_pFMODSystem->Play2D(NiApplication::ConvertMediaFilename(pConfig->strFileName.c_str()),
			dwSoundID, pConfig->fGain, pConfig->nLoop, true);
	}
	
	return dwSoundIndex;

//	GarbageCollect();
//	BOOL bClone = FALSE;
//	DWORD dwSoundIndex = 0;
//	NiAudioSourcePtr spSound = NULL;
//	LPSOUND_DATA pData = FindSound(dwSoundID);
//	if ( !pData )
//	{
//		bNewInstance = TRUE;
//	}
//	else
//	{
//		dwSoundIndex = pData->dwSoundIndex;
//		spSound = pData->pSound;
//		if ( bNewInstance )
//		{
//			bClone = TRUE;
//		}
//	}
//	
//	if ( bNewInstance )
//	{
//		LPSOUND_CONFIG pConfig = FindConfig(dwSoundID);
//		if ( !pConfig )
//		{
//			XWaring("FindConfig(soundid=%d) failed", dwSoundID);
//			dwSoundIndex = 0;
//			goto KK_EXIT;
//		}
//		spSound = m_pAudioSystem->CreateSource(NiAudioSource::TYPE_AMBIENT);
//		spSound->SetFilename(NiApplication::ConvertMediaFilename(pConfig->strFileName.c_str()));
//		spSound->SetStreamed(true);
//		if ( !spSound->Load() )
//		{
//			XWaring("Load Sound (%s) failed", pConfig->strFileName.c_str());
//			goto KK_EXIT;
//		}
//		spSound->SetGain(pConfig->fGain);
//		spSound->SetLoopCount(pConfig->nLoop);
//		spSound->Update(0);
//		
//		dwSoundIndex = m_kIDGen.Alloc();
//		dwSoundIndex = LoadSound(NiApplication::ConvertMediaFilename(pConfig->strFileName.c_str()));
//
//		SOUND_DATA kData;
//		kData.dwSoundID = dwSoundID;
//		kData.dwSoundIndex = dwSoundIndex;
//		kData.dwStartTime = GetHighResolutionTime();
//		kData.pSound = spSound;
//		kData.bClone = bClone;
//		m_listSound.push_back(kData);
//	}
//	KK_CHECK_POINTER(spSound);
//	if ( spSound->GetStatus() == NiAudioSource::PLAYING )
//	{
//		spSound->Stop();
//	}
//	//spSound->Play();
//	XTrace("PlaySound(%d)", dwSoundID);
//KK_EXIT:
	//return dwSoundIndex;
}
void CSoundMgr::StopAllSound()
{
	m_pFMODSystem->StopAllSound();
}
DWORD CSoundMgr::StopSound(DWORD dwSoundIndex)
{

	if ( !m_pFMODSystem )
	{
		return 0;
	}
	
	m_pFMODSystem->StopSound(dwSoundIndex);

	return dwSoundIndex;

	//if ( !m_pAudioSystem )
	//{
	//	return 0;
	//}
	//LPSOUND_DATA pData = FindSoundByIndex(dwSoundIndex);
	//if ( pData )
	//{
	//	if ( pData->pSound )
	//	{
	//		pData->pSound->Stop();
	//		XTrace("StopSound(%d)", pData->dwSoundID);
	//	}
	//	if ( pData->bClone )
	//	{
	//		RemoveSound(dwSoundIndex);
	//	}
	//	return dwSoundIndex;
	//}
	//else
	//{
	//	XWaring("StopSound(): bad args");
	//	return 0;
	//}
}

DWORD CSoundMgr::StopSoundByID(DWORD dwSoundID)
{
	if ( !m_pFMODSystem )
	{
		return 0;
	}

	m_pFMODSystem->StopSound(dwSoundID);

	return dwSoundID;
	//if ( !m_pAudioSystem )
	//{
	//	return 0;
	//}
	//LPSOUND_DATA pData = FindSound(dwSoundID);
	//if ( pData )
	//{
	//	if ( pData->pSound )
	//	{
	//		XTrace("StopSound(%d)", dwSoundID);
	//		pData->pSound->Stop();
	//	}
	//	if ( pData->bClone )
	//	{
	//		RemoveSound(pData->dwSoundIndex);
	//	}
	//	return dwSoundID;
	//}
	//else
	//{
	//	XWaring("StopSound(): bad args");
	//	return 0;
	//}
}

void CSoundMgr::Update(DWORD dwTime)
{
	//NiAudioSystem::GetAudioSystem()->Update(fTime, true);
	m_pFMODSystem->Update();
	
}

void	CSoundMgr::GarbageCollect()
{
	//SOUND_DATA_LIST_Iterator it = m_listSound.begin();
	//for ( ; it != m_listSound.end(); ++it )
	//{
	//	LPSOUND_DATA pData = &*it;
	//	if ( pData->pSound->GetStatus() != NiAudioSource::PLAYING)
	//	{
	//		RemoveSound(pData->dwSoundIndex);
	//		break;
	//	}
	//}

}

LPSOUND_CONFIG	CSoundMgr::FindConfig(DWORD dwSoundID)
{
	SOUND_CONFIG_LIST_Iterator it = m_listConfig.begin();
	for ( ; it != m_listConfig.end(); ++it )
	{
		LPSOUND_CONFIG pConfig = &*it;
		if ( pConfig->dwSoundID == dwSoundID )
		{
			return pConfig;
		}
	}
	return NULL;
}
LPSOUND_DATA	CSoundMgr::FindSound(DWORD dwSoundID, DWORD dwSoundIndex )
{
	//SOUND_DATA_LIST_Iterator it = m_listSound.begin();
	//for ( ; it != m_listSound.end(); ++it )
	//{
	//	LPSOUND_DATA pData = &*it;
	//	if ( pData->dwSoundID == dwSoundID )
	//	{
	//		if ( dwSoundIndex == 0 )
	//		{
	//			return pData;
	//		}
	//		else if ( dwSoundIndex == pData->dwSoundIndex )
	//		{
	//			return pData;
	//		}
	//	}
	//}
	return NULL;
}

LPSOUND_DATA	CSoundMgr::FindSoundByIndex(DWORD dwSoundIndex )
{
	//SOUND_DATA_LIST_Iterator it = m_listSound.begin();
	//for ( ; it != m_listSound.end(); ++it )
	//{
	//	LPSOUND_DATA pData = &*it;
	//	if ( dwSoundIndex == pData->dwSoundIndex )
	//	{
	//		return pData;
	//	}
	//}
	return NULL;
}

DWORD CSoundMgr::RemoveSound(DWORD dwSoundIndex)
{
	//SOUND_DATA_LIST_Iterator it = m_listSound.begin();
	//for ( ; it != m_listSound.end(); ++it )
	//{
	//	LPSOUND_DATA pData = &*it;
	//	if ( dwSoundIndex == pData->dwSoundIndex )
	//	{
	//		XTrace("RemoveSound(%d)", pData->dwSoundID);
	//		m_listSound.erase(it);
	//		return dwSoundIndex;
	//	}
	//}
	return 0;
}
void CSoundMgr::SetPlaybackRate(DWORD dwSoundIndex, long rate)
{
	m_pFMODSystem->SetPlaybackRate(dwSoundIndex, rate);
}



void CSoundMgr::SetSource(DWORD unType, const char* strName, float fVolume, int nLoopCount, bool bStream)
{
	m_pFMODSystem->SetSource(unType, strName, fVolume, nLoopCount, bStream);
}

void CSoundMgr::PlayMotorSound(float curSpeed)
{
	LPSOUND_CONFIG pConfig = FindConfig(ST_MOTOR_MID);

	m_pFMODSystem->PlayMotorSound(curSpeed, NiApplication::ConvertMediaFilename(pConfig->strFileName.c_str()),
		ST_MOTOR_MID,pConfig->fGain, pConfig->nLoop, true);
}
//int CSoundMgr::LoadSound(const char* filename, bool bStream)
//{
//	AUDIOSYSTEMTESTBOOL
//
//	m_apSound[eSoundType]->nSoundId = m_pAudioEditor->CreateSound(filename, bStream);
//
//	LogText("soundId %d, channelId %d\n", m_apSound[eSoundType]->nSoundId, m_apSound[eSoundType]->nChannelId);
//
//	return (m_apSound[eSoundType]->nSoundId > -1)? true: false;
//}
//int CSoundMgr::PlaySound(const SOUND_DATA& data)
//{
//	m_pAudioEditor->PlayAccordingToLoops(data.dwSoundIndex, )
//}