#include "Top-KartPCH.h"

#include ".\fmodsystem.h"
#include <assert.h>
//CRITICAL_SECTION g_criticalSection;
#pragma comment(lib, "fmodex_vc")

CFMODSystem::CFMODSystem()
{
	m_pSystem = NULL;

	for (int i = 0; i < MAX_SOUND_NUM; i++)
		m_arSoundIds[i] = false;

	for (int j = 0; j < MAX_SOUND_CHANNELS; j++)
	{
		m_apChannels[j] = NULL;
	}

	Init();

//	InitializeCriticalSection(&g_criticalSection);

} 

CFMODSystem::~CFMODSystem()
{
	ReleaseAll();
	m_pSystem->release();
//	DeleteCriticalSection(&g_criticalSection);
}

void CFMODSystem::Init(void)
{
	FMOD_RESULT result;
	unsigned int     version;
	FMOD::Channel   *channel1 = 0, *channel2 = 0, *channel3 = 0;

	result = FMOD::System_Create(&m_pSystem);
	result = m_pSystem->getVersion(&version);
	result = m_pSystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);	

	m_pSystem->set3DSettings(1.0f, 1.0f, 1.0f);

	//m_pSystem->createChannelGroup("root", &m_pRootGroup);


}
void CFMODSystem::Close()
{
	FMOD_RESULT result = m_pSystem->release();
	//	m_pSystem->close();
}
void CFMODSystem::UpdatePlayingState()
{
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		CSound* pSound = m_apChannels[i];
		if (pSound)
		{
			bool isPlaying = false;

			pSound->fmodChannel->isPlaying(&isPlaying);
			if (!isPlaying)
			{
				pSound->isPlaying = false;
				m_apChannels[i] = NULL;
				//LogText("\n 释放 channel : %d\n", i);

			}
		}
	}
}

void CFMODSystem::Play2D(const char* szFilename, DWORD dwSoundId, float fVolume, int nLoopCount, bool bStream)
{
//	EnterCriticalSection(&g_criticalSection);
	////LogText("\n欲播放 %d \n", dwSoundId);
	int nSoundResource = HasSoundLoaded(dwSoundId);
	if(nSoundResource == -1)//若没载入，则载入。
	{
		//LogText("\n载入 %d \n",dwSoundId);
		int nType =  FMOD_2D|FMOD_LOOP_NORMAL|FMOD_SOFTWARE;
		if (bStream)
			nType |= FMOD_CREATESTREAM;

		CSound* pSound = NULL;
		nSoundResource = CreateSound(szFilename, nType,pSound);
		if (pSound)
		{
			pSound->nLoopCount = nLoopCount;
			pSound->fVolume = fVolume;
		}
	}

	if (nSoundResource == -1)//经过上面的步骤之后，仍然无效，说明载入过程出错
		return;

	if (IsSoundPlaying(dwSoundId))//正在播放，就退出了。
		return;

	//播放
	CSound *pSound = GetSound(nSoundResource);

	if (!pSound)
		return;

	int nFreeChannel = GetFreeChannel();


	if (nFreeChannel == -1)
		return;

	FMOD::Sound* sound = pSound->fmodSound;
	FMOD::Channel* channel;
	m_pSystem->playSound((FMOD_CHANNELINDEX)nFreeChannel,sound, false,&channel);
	
	//int nLC = (pSound->nLoopCount == -1)? -1: pSound->nLoopCount-1;
	int nLC = pSound->nLoopCount;
	channel->setLoopCount(nLC);
	channel->setVolume(pSound->fVolume);
	pSound->isPlaying = true;
	pSound->fmodChannel = channel;
	pSound->nChannelId = nFreeChannel;
	pSound->nSoundIndex = dwSoundId;

	m_apChannels[nFreeChannel] = pSound;

	//LogText("\n 播放于 channel : %d\n", nFreeChannel);
	//LeaveCriticalSection(&g_criticalSection);
}

int CFMODSystem::HasSoundLoaded(DWORD dwSoundNumericalId)
{
	Sound_iterator itr;
	for (itr = m_mpSounds.begin(); itr != m_mpSounds.end(); ++itr)
	{
		if (itr->second->nSoundIndex == dwSoundNumericalId)
			return (itr->first);//载入了，就返回资源索引
	}

	return -1;
}
void CFMODSystem::StopSound(DWORD nSoundId)
{
	if (IsSoundPlaying(nSoundId))
	{
		int nSoundResource = HasSoundLoaded(nSoundId);
		CSound* pSound = GetSound(nSoundResource);
		pSound->fmodChannel->stop();
		pSound->isPlaying = false;
		m_apChannels[pSound->nChannelId] = NULL;
	}
}
void CFMODSystem::StopAllSound()
{
	for (int i = 0; i < MAX_SOUND_CHANNELS; i++)
	{
		if (m_apChannels[i])
		{
			StopSound(m_apChannels[i]->nSoundIndex);
		}
	}

	ReleaseAll();
}

bool CFMODSystem::IsSoundPlaying(DWORD nSoundId)
{
	int nSoundResource = (int)HasSoundLoaded(nSoundId);
	if (nSoundResource == -1)
		return false;//没有载入，当然不会正在播放。
	else
	{
		CSound* pSound = GetSound(nSoundResource);
		return pSound->isPlaying;
	}
}
int CFMODSystem::GetFreeChannel()
{ 
	for (int i = 0; i < 50; i++)
	{
		if (!m_apChannels[i])
		{
			//LogText("\n 获得空闲channel %d \n", i);
			return i;
		}
	}
	return -1;
}

int CFMODSystem::CreateSound(const char* strFilename , int nSoundType, CSound*& pSound)
{
	if (strFilename == NULL)
		return -1;

	FMOD_RESULT    result;
	FMOD::Sound *  sound;

	CSound *pNewSound;

	//检查文件是否存在
	FILE* pf = fopen(strFilename, "r");
	if (!pf)
		return -1;
	fclose(pf);


	pNewSound = new CSound();
	strcpy(pNewSound->strFilename, strFilename);

	//nSoundType = FMOD_3D|FMOD_CREATESTREAM|FMOD_LOOP_NORMAL|FMOD_SOFTWARE|FMOD_3D_CUSTOMROLLOFF;
	result = m_pSystem->createSound(strFilename, nSoundType, 0, &sound);

	if (result != FMOD_OK)
	{
		return -1;
	}

	pNewSound->nChannelId = -1;
	pNewSound->fmodSound = sound;
	pNewSound->nType = nSoundType;

	int nSoundIndex = GetValidSoundIndex();
	m_mpSounds.insert(make_pair(nSoundIndex, pNewSound));

	pSound = pNewSound;

	return nSoundIndex;
}
CSound* CFMODSystem::GetSound(int nSoundResourceId)
{
	Sound_iterator itr;
	itr = m_mpSounds.find(nSoundResourceId);
	if (itr != m_mpSounds.end())
	{
		return itr->second;
	}
	else
		return NULL;
}

void CFMODSystem::Update()
{
	m_pSystem->update();
	UpdatePlayingState();
}


DWORD CFMODSystem::GetValidSoundIndex(void)
{
	for (DWORD i = 0; i < MAX_SOUND_NUM; i ++)
		if (m_arSoundIds[i] == false)
		{
			m_arSoundIds[i] = true;

			return i;
		}

		assert(0);
		return 0;

}

void CFMODSystem::SetSource(DWORD unType, const char* strName, float fVolume, int nLoopCount, bool bStream)
{
	int nSoundResource = HasSoundLoaded(unType);
	int nType =  FMOD_2D|FMOD_LOOP_NORMAL|FMOD_SOFTWARE;
	if (bStream)
		nType |= FMOD_CREATESTREAM;

	if (nSoundResource != -1)//已经载入
	{
		CSound* pSound = GetSound(nSoundResource);
		pSound->fmodSound->release();
		FMOD::Sound* sound;
		m_pSystem->createSound(strName, nType, 0, &sound);
		pSound->fmodSound = sound;
		pSound->nSoundIndex = unType;
		pSound->isPlaying = false;
		pSound->fVolume = fVolume;
		pSound->nLoopCount = nLoopCount;
		pSound->nSoundIndex = unType;
	}
	else
	{
		CSound* pSound = NULL;
		CreateSound(strName, nType,pSound);
		if (pSound)
		{
			pSound->nLoopCount = nLoopCount;
			pSound->fVolume = fVolume;
			pSound->isPlaying = false;
			pSound->nSoundIndex = unType;
		}

	}

}

void CFMODSystem::PlayMotorSound(float curSpeed, const char* szFilename, DWORD dwSoundId, float fVolume, int nLoopCount, bool bStream)
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
	Play2D(szFilename, dwSoundId, fVolume, nLoopCount, bStream);
	SetVolume(dwSoundId, fVolume);
	SetPlaybackRate(dwSoundId, rate);
}
void CFMODSystem::SetVolume(DWORD dwSoundId, float fVolume)
{
	int nSoundResource = HasSoundLoaded(dwSoundId);
	if (nSoundResource != -1)
	{
		CSound* pSound = GetSound(nSoundResource);
		if (pSound->isPlaying)
		{
			pSound->fmodChannel->setVolume(fVolume);
		}

		pSound->fVolume = fVolume;
	}
	//else 没有播放的音乐，设置音量无效
}

void CFMODSystem::SetPlaybackRate(DWORD dwSoundId, float rate)
{
	int nSoundResource = HasSoundLoaded(dwSoundId);
	if (nSoundResource != -1)
	{
		CSound* pSound = GetSound(nSoundResource);
		if (pSound->isPlaying)
		{
			pSound->fmodChannel->setFrequency(rate);
		}
	}
}

void ReleaseSound(DWORD dwSoundId)
{
	//int nSoundResource = HasSoundLoaded(dwSoundId);
	//if (nSoundResource != -1)
	//{
	//	CSound* pSound = GetSound(nSoundResource);
	//	if (pSound)
	//	{
	//		pSound->fmodSound->release();

	//	}
	//}
	
}

void CFMODSystem::ReleaseAll()
{
	Sound_iterator itr;
	for (itr = m_mpSounds.begin(); itr != m_mpSounds.end(); ++itr)
	{
		CSound* pSound = itr->second;
		pSound->fmodChannel->stop();
		pSound->fmodSound->release();
		if ( pSound->nChannelId != -1 )
		{
			m_apChannels[pSound->nChannelId] = NULL;
		}
		delete pSound;
	}
	m_mpSounds.clear();
}
//变化：
/*    

*/
//int CFMODSystem::CreateSound(const char* strFilename, const char* pfilebuf, int filelen, int nSoundType, unsigned int& length)
//{
//	if (strFilename == NULL)
//		return -1;
//
//	FMOD_RESULT    result;
//	FMOD::Sound *  sound;
//
//	CSound *pNewSound;
//
//	pNewSound = new CSound();
//
//	strcpy(pNewSound->strFilename, strFilename);
//	nSoundType |= FMOD_OPENMEMORY;
//	FMOD_CREATESOUNDEXINFO info;
//	memset(&info, 0, sizeof(info));
//	info.cbsize = sizeof(info);
//	info.length = filelen;
//	//result = m_pSystem->createSound(strFilename, nSoundType, 0, &sound);
//	result = m_pSystem->createSound(pfilebuf, nSoundType, &info, &sound);
//
//	if (result != FMOD_OK)
//	{
//		return -1;
//	}
//
//	pNewSound->fmodSound = sound;
//	sound->getLength(&length, FMOD_TIMEUNIT_MS);//获取声音的长度
//	int nSoundIndex = GetValidSoundIndex();
//
//	m_mpSounds.insert(make_pair(nSoundIndex, pNewSound));
//
//	return nSoundIndex;
//}
//
//void CFMODSystem::SetSequence(int Soundindex, int* index, int num)
//{
////	m_pBakSound->setSubSoundSentence(index, num);
//	FMOD::Sound* pSound;
//	pSound = FindSound(Soundindex);
//	if(pSound)
//	{
//		pSound->setSubSoundSentence(index, num);
//	}
//}
//
//int CFMODSystem::InsertSubSound(int* index, unsigned int subsoundnum)
//{
//	FMOD_RESULT result;
//	FMOD_SOUND_TYPE		soundtype;
//	FMOD_SOUND_FORMAT	format;
//	int					channels;
//	int					bits;
//	float				frequency;
//	float				fvolume;
//	float				fpan;
//	int					priority;
//
//	FMOD::Sound *subsound = FindSound(index[0]);
//	subsound->getFormat(&soundtype, &format, &channels, &bits);
//	subsound->getDefaults(&frequency, &fvolume, &fpan, &priority);
//
//	FMOD_CREATESOUNDEXINFO exinfo;
//	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
//
//	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
//	exinfo.defaultfrequency = frequency;
//	exinfo.numsubsounds = subsoundnum;
//	exinfo.numchannels = channels;
//	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
//
//	FMOD::Sound* pBackSound;
//	result = m_pSystem->createSound(0, FMOD_CREATESTREAM|FMOD_OPENUSER|FMOD_ACCURATETIME, &exinfo, &pBackSound);
//
//	for(unsigned int i = 0; i < subsoundnum; i ++)
//	{
//		FMOD::Sound *subsound;
//		subsound = FindSound(index[i]);
//		//result = subsound->setLoopPoints(0, FMOD_TIMEUNIT_MS, 2000, FMOD_TIMEUNIT_MS);
//		//result = subsound->setLoopCount(2);
//		result = pBackSound->setSubSound(i, subsound);
//	}
//
//	int subsoundlist[5] = {0, 1, 2, 3, 4};
//	result = pBackSound->setSubSoundSentence(subsoundlist, subsoundnum);
//
//	CSound *pNewSound;
//	pNewSound = new CSound();
//	int nSoundIndex = GetValidSoundIndex();
//
//	pNewSound->fmodSound = pBackSound;
//	m_mpSounds.insert(make_pair(nSoundIndex, pNewSound));
//
//	unsigned int length;
//	pBackSound->getLength(&length, FMOD_TIMEUNIT_MS);//获取声音的长度
//
//	//result = pBackSound->setLoopPoints(0, FMOD_TIMEUNIT_MS, 2000, FMOD_TIMEUNIT_MS);
//	//result = pBackSound->setLoopCount(10);
//
//	return nSoundIndex;
//}
//
//void CFMODSystem::PlaySubSound(const char** filename, unsigned int subsoundnum)
//{
//	if(subsoundnum < 1)
//	{
//		return;
//	}
//	else if(subsoundnum > MAXSUBSOUNDNUM)
//	{
//		subsoundnum = MAXSUBSOUNDNUM;
//	}
//
//	FMOD_RESULT result;
//	FMOD_SOUND_TYPE		soundtype;
//	FMOD_SOUND_FORMAT	format;
//	int					channels;
//	int					bits;
//	float				frequency;
//	float				fvolume;
//	float				fpan;
//	int					priority;
//
//	FMOD::Sound	*subsound[5];
//	for(unsigned int i = 0; i < subsoundnum; i ++)
//	{
//		result = m_pSystem->createSound(filename[i], FMOD_CREATESTREAM|FMOD_LOOP_OFF|FMOD_2D|FMOD_SOFTWARE, 0, &subsound[i]);
//		subsound[i]->getFormat(&soundtype, &format, &channels, &bits);
//		subsound[i]->getDefaults(&frequency, &fvolume, &fpan, &priority);
//	}
//
//	FMOD_CREATESOUNDEXINFO exinfo;
//	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
//
//	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
//	exinfo.defaultfrequency = frequency;
//	exinfo.numsubsounds = subsoundnum;
//	exinfo.numchannels = channels;
//	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
//
//	result = m_pSystem->createSound(0, FMOD_CREATESTREAM|FMOD_LOOP_NORMAL|FMOD_OPENUSER|FMOD_ACCURATETIME, &exinfo, &m_pBakSound);
//
//
//	for(i = 0; i < subsoundnum; i ++)
//	{
//		result = m_pBakSound->setSubSound(i, subsound[i]);
//	}
//	//result = m_pSystem->createStream(filename1, FMOD_DEFAULT, 0, &subsound[0]);
//	//result = m_pSystem->createStream(filename2, FMOD_DEFAULT, 0, &subsound[1]);
//
//	//result = sound->setSubSound(0, subsound[0]);
//	//result = sound->setSubSound(1, subsound[1]);
//
//	int subsoundlist[5] = {0, 1, 2, 3, 4};
//	result = m_pBakSound->setSubSoundSentence(subsoundlist, subsoundnum);
//
//	FMOD::Channel *channel;
//	m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pBakSound, false, &channel);
//}
//
//void CFMODSystem::PlaySubSound(const char* filename1, const char* filename2)
//{
//	//Create two sounds, each with their own loop points 
//    FMOD_SYSTEM		*system;
//	FMOD_RESULT		result;
//	unsigned int	version;
//
//	FMOD_CREATESOUNDEXINFO exinfo;
//	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
//
//	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
//	exinfo.defaultfrequency = 22050;
//	exinfo.numsubsounds = 2;
//	exinfo.numchannels = 1;
////	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
//	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
//
//	result = FMOD_System_Create(&system);
//
//	result = FMOD_System_GetVersion(system, &version);
//
//	if (version < FMOD_VERSION)
//	{
//		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
//		return;
//	}
//
//	result = FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
//	/*
//	Create the 'parent' stream that contains the substreams.  Set it to loop so that it loops between subsound 0 and 1.
//	*/
//	FMOD_SOUND		*sound;
//	result = FMOD_System_CreateStream(system, 0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &sound);
//
//	/*
//	Add 2 of our streams as children of the parent.  They should be the same format (ie mono/stereo and bitdepth) as the parent sound.
//	When subsound 0 has finished and it is playing subsound 1, we will swap subsound 0 with a new sound, and the same for when subsound 1 has finished,
//	causing a continual double buffered flip, which means continuous sound.
//	*/
//	FMOD_SOUND       *subsound[2];
//
//	result = FMOD_System_CreateStream(system, filename1, FMOD_DEFAULT, 0, &subsound[0]);
//
//	result = FMOD_System_CreateStream(system, filename2, FMOD_DEFAULT, 0, &subsound[1]);
//
//	result = FMOD_Sound_SetSubSound(sound, 0, subsound[0]);
//
//	result = FMOD_Sound_SetSubSound(sound, 1, subsound[1]);
//
//	/*
//	Set up the gapless sentence to contain these first 2 streams.
//	*/
//	{
//		int soundlist[2] = { 0, 1 };
//
//		result = FMOD_Sound_SetSubSoundSentence(sound, soundlist, 2);
//	}
//	FMOD_CHANNEL     *channel = 0;
//	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound, FALSE, &channel);
//}
//
//bool CFMODSystem::AddSubSound(int iParentid, int iChildid, int index)
//{
//	FMOD::Sound* pParent	= FindSound(iParentid);
//	FMOD::Sound* pChild		= FindSound(iChildid);
//	FMOD_RESULT result;
//	result = pChild->setLoopPoints(30000, FMOD_TIMEUNIT_MS, 35000,FMOD_TIMEUNIT_MS); 
//	result = pChild->setLoopCount(10);
//	result = pParent->setSubSound(index, pChild);
//	if(result == FMOD_OK)
//	{
//		int test = 0;
//	}
//	FMOD_OPENSTATE	openstate;
//	unsigned int	percent;
//	bool			starving;
//	result = pParent->getOpenState(&openstate, &percent, &starving);
//	if(result == FMOD_OK)
//	{
//		int test = 0;
//	}
//	m_iSubSoundlist[index] = index;
//	result = pParent->setSubSoundSentence(m_iSubSoundlist, index+1);
//	if(result == FMOD_OK)
//	{
//		int test = 0;
//	}
////	pParent->setSubSoundSentence(m_iSubSoundlist, pParent->getNumSubSounds())
//	return true;
//}
//
//int CFMODSystem::CreateSubSound()
//{
//	FMOD_RESULT    result;
//	FMOD::Sound *  sound;
//
//	CSound *pNewSound;
//
//	pNewSound = new CSound();
//
//	FMOD_CREATESOUNDEXINFO exinfo;
//	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO)); 
//	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO); 
//	exinfo.defaultfrequency = 44100; 
//	exinfo.numsubsounds = 2; 
//	exinfo.numchannels = 2; 
//	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
//
//	result = m_pSystem->createSound(0, FMOD_SOFTWARE|FMOD_OPENUSER|FMOD_2D|FMOD_CREATESTREAM, &exinfo, &sound);
//
//	if (result != FMOD_OK)
//	{
//		return -1;
//	}
//
//	unsigned int length;
//	pNewSound->fmodSound = sound;
//	sound->getLength(&length, FMOD_TIMEUNIT_MS);//获取声音的长度
//	int nSoundIndex = GetValidSoundIndex();
//
//	m_mpSounds.insert(make_pair(nSoundIndex, pNewSound));
//
//	return nSoundIndex;
//}
//
//
//
//unsigned int CFMODSystem::GetSoundLength(int nSound)
//{
//	unsigned int nLen = 0;
//	if (FMOD::Sound* pSound = FindSound(nSound))
//	{
//		if (pSound->getLength(&nLen, FMOD_TIMEUNIT_MS) == FMOD_OK)
//			return nLen;
//		else 
//			return 0;
//	}
//	else
//		return 0;
//}
//
//void CFMODSystem::SetVolume(int nChannel, float fVol) 
//{
//	if (FMOD::Channel* pChannel = GetChannel(nChannel))
//	{
//		pChannel->setVolume(fVol);
//	}
//}
//
//float CFMODSystem::GetVolume(int nChannel) 
//{
//	float fVol = 0.0f;
//	if (FMOD::Channel* pChannel = GetChannel(nChannel))
//		if (pChannel->getVolume(&fVol) == FMOD_OK)
//			return fVol;
//		else return 0.0f;
//	else return 0.0f;
//}
//char* CFMODSystem::GetFilename(int nSound) 
//{
//	if (CSound* pSound = FindSoundObj(nSound))
//		return pSound->strFilename;
//	else
//		return NULL;
//}
//
//int CFMODSystem::GetPlayingSoundIndex(int nChannel) 
//{
//	FMOD::Sound* sound;
//	FMOD::Channel* channel;
//	if (channel = GetChannel(nChannel))
//	{
//		if (channel->getCurrentSound(&sound) == FMOD_OK)
//		{
//			Sound_iterator itr;
//			for (itr = m_mpSounds.begin(); itr != m_mpSounds.end(); itr++)
//			{
//				if (itr->second->fmodSound == sound)
//					return itr->first;
//			}
//			return -1;
//
//		}
//		else
//			return -1;
//
//	}
//	else
//		return -1;
//
//}
//
//void  CFMODSystem::SetChannelLoopCount(int nChannel, int nCount)
//{
//	FMOD::Channel* channel;
//	if (channel = GetChannel(nChannel))
//	{
//		channel->setMode(FMOD_LOOP_NORMAL);
//		channel->setLoopCount(nCount);
//	}
//
//}
//

//int CFMODSystem::Play(int nSound, int nChannel)
//{
//	FMOD::Sound* sound = FindSound(nSound);
//	FMOD::Channel* channel;
//	int newChannel = nChannel;
//
//	if (sound)
//	{
//		m_pSystem->playSound((FMOD_CHANNELINDEX)nChannel,sound, false,&channel);
//
//		if(nSound != CHANNEL_FREE)
//			channel->getIndex(&newChannel);
//		if(channel)
//		{
//			CChannel* pChannel = new CChannel(channel);
//			m_mpChannels.insert(make_pair(newChannel, pChannel));
//			return newChannel;
//		}
////		m_mpChannels.insert(make_pair(newChannel, channel));
//		
//	}
//	else
//		return -1;
//}
//
//void CFMODSystem::SetChannelPaused(int nChannel, bool bPaused)
//{
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//	if(pChannel)
//		pChannel->setPaused(bPaused);
//	//FMOD::Channel* pChannel;
//	//FMOD_RESULT result = m_pSystem->getChannel(nChannel, &pChannel);
//	//if (result == FMOD_OK)
//	//	pChannel->setPaused(bPaused);
//	
//}
//
//bool CFMODSystem::GetChannelPaused(int nChannel)
//{
//	bool bpaused = false;
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//	if(pChannel)
//		pChannel->getPaused(&bpaused);
//
//	return bpaused;
//	//bool bpaused = false;
//	//FMOD::Channel* pChannel;
//	//FMOD_RESULT result = m_pSystem->getChannel(nChannel, &pChannel);
//	//if (result == FMOD_OK)
//	//	pChannel->getPaused(&bpaused);
//	//return bpaused;
//
//}
//
//void CFMODSystem::StopChannel(int nChannel)
//{
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//	if (pChannel)
//		pChannel->stop();
//}
//
//void CFMODSystem::ReleaseSound(int nSound)
//{
//	map <int, CSound*>::iterator itr;
//
//	itr = m_mpSounds.find(nSound);
//
//
//	//char buf[256];
//	//sprintf(buf, "Release sound：callback: psc:0x%xh, p1:%d, p2:%d\n",itr->first,itr->first,itr->first);
//	//OutputDebugString(buf);
//
//	if (itr != m_mpSounds.end())
//	{
//		FMOD_RESULT result = itr->second->fmodSound->release();
//		assert(result == FMOD_OK);
//		delete (*itr).second;
//		m_mpSounds.erase(itr);
//
//		m_arSoundIds[nSound] = false;
//	}
//
//
//}
//void CFMODSystem::SetChannel3DAttributes(int nChannel,float posX, float posY, float posZ,
//										 float velX, float velY, float velZ)
//{
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//
//	FMOD_VECTOR fvpos = {posX, posY, posZ};
//	FMOD_VECTOR fvvel = {velX, velY, velZ};
//
//	if (pChannel)
//	{
//		pChannel->set3DAttributes(&fvpos, &fvvel);
//	}
//}
//
//void CFMODSystem::SetChannel3DAttributes(int nChannel,const SOUND_VECTOR* pos, const SOUND_VECTOR* vel)
//{
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//
//	FMOD_VECTOR fvpos = {pos->x, pos->y, pos->z};
//	FMOD_VECTOR fvvel = {vel->x, vel->y, vel->z};
//
//	if (pChannel)
//		pChannel->set3DAttributes(&fvpos, &fvvel);
//
//}
//void CFMODSystem::Set3DListenerAttributes(
//	const SOUND_VECTOR *  pos,
//	const SOUND_VECTOR *  vel, 
//	const SOUND_VECTOR *  forward, 
//	const SOUND_VECTOR *  up)
//{
//	FMOD_VECTOR fvpos = {pos->x, pos->y, pos->z};
//	FMOD_VECTOR fvvel = {vel->x, vel->y, vel->z};
//	FMOD_VECTOR fvforward = {forward->x, forward->y, forward->z};
//	FMOD_VECTOR fvup = {up->x, up->y, up->z};
//
//
//	m_pSystem->set3DListenerAttributes(0,&fvpos,&fvvel,&fvforward,&fvup);
//}
//
//void CFMODSystem::ReleaseGroup()
//{
//	map<int, GROUPINFO*>::iterator it;
//
//	for(it = m_mpGroups.begin(); it != m_mpGroups.end();)
//	{
//		GROUPINFO* pInfo = it->second;
//		if(pInfo)
//		{
//			pInfo->pDSP->release();
//			pInfo->pGroup->release();
//		}
//		delete pInfo;
//		it = m_mpGroups.erase(it);
//	}
//}
//
//void CFMODSystem::ReleaseSound()
//{
//	map <int, CSound*>::iterator itr;
//
//	for (itr = m_mpSounds.begin(); itr != m_mpSounds.end();)
//	{
//		//		int index = itr->first;
//		//		assert(itr->first > MAX_SOUND_NUM);
//		m_arSoundIds[itr->first] = false;
//
//		itr->second->fmodSound->release();
//		delete itr->second;
//		itr = m_mpSounds.erase(itr);
//	}
//}
//
//void CFMODSystem::ReaseAllSources()
//{
//	ReleaseSound();
//	ReleaseGroup();
//}
//
//void CFMODSystem::Get3DListenerAttributes(
//	SOUND_VECTOR *  pos,
//	SOUND_VECTOR *  vel, 
//	SOUND_VECTOR *  forward, 
//	SOUND_VECTOR *  up)
//{
//	FMOD_VECTOR fvpos, fvvel, fvforward, fvup;
//
//	if (m_pSystem->get3DListenerAttributes(0,&fvpos,&fvvel,&fvforward,&fvup) == FMOD_OK)
//	{
//
//		pos->x = fvpos.x; pos->y = fvpos.y; pos->z = fvpos.z;
//		vel->x = fvvel.x; vel->y = fvvel.y; vel->z = fvvel.z;
//		forward->x = fvforward.x; forward->y = fvforward.y; forward->z = fvforward.z; 
//		up->x = fvup.x; up->y = fvup.y; up->z = fvup.z; 
//	}
//}
//
//bool CFMODSystem::IsPlaying(int nChannel)
//{
//	bool isPlaying = false;
//
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//	if(pChannel && FMOD_OK == pChannel->isPlaying(&isPlaying))
//		return isPlaying;
//	else
//		return false;
//}
//bool CFMODSystem::IsPaused(int nChannel)
//{
//	bool isPaused = false;
//
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//	if(pChannel && FMOD_OK == pChannel->isPlaying(&isPaused))
//		return isPaused;
//	else
//		return false;
//
//}
//void CFMODSystem::Set3DMinMaxDistance(int nChannel, float  mindistance,  float  maxdistance)
//{
//	FMOD::Channel* pChannel = GetChannel(nChannel);
//	if (pChannel)
//	{
//		pChannel->set3DMinMaxDistance(mindistance, maxdistance);
//	}
//}
//
//FMOD::Sound* CFMODSystem::FindSound(int nSound)
//{
//	Sound_iterator itr;
//	itr = m_mpSounds.find(nSound);
//	if (itr != m_mpSounds.end())
//		return (FMOD::Sound*)itr->second->fmodSound;
//	else 
//		return NULL;
//
//}
//CSound* CFMODSystem::FindSoundObj(int nSound)
//{
//	Sound_iterator itr;
//
//	itr = m_mpSounds.find(nSound);
//	if (itr != m_mpSounds.end())
//		return (CSound*)itr->second;
//	else 
//		return NULL;
//}
//
//CChannel*	CFMODSystem::GetCChannel(int nChannel)
//{
//	if(m_mpChannels.find(nChannel) == m_mpChannels.end())
//		return NULL;
//	return m_mpChannels[nChannel];
//}
//
//FMOD::Channel* CFMODSystem::GetChannel(int nChannel)
//{
//	CChannel* pChannel = GetCChannel(nChannel);
//	if(pChannel)
//		return pChannel->GetChannel();
//	return NULL;
//	//FMOD::Channel* pChannel;
//	//FMOD_RESULT result = m_pSystem->getChannel(nChannel, &pChannel);
//	//if (result == FMOD_OK)
//	//	return pChannel;
//	//else 
//	//	return NULL;
//}
//



////void CFMODSystem::SetCallBack(IChannelCallBack* callback)
////{
////	m_pCallback = callback;
////}
//void CFMODSystem::SetChannelCallBack(int nChannel, STRCTCALLBACK* pSC)
//{
//	FMOD::Channel* pch = GetChannel(nChannel);
//	if (pch)
//	{
//		FMOD_RESULT result = pch->setCallback(FMOD_CHANNEL_CALLBACKTYPE_END,&PlayCallBack,(int)(pSC));
//		assert(result == FMOD_OK);
//
//	}
//}
////当一个音乐播放结束时，回调此函数
//FMOD_RESULT F_CALLBACK CFMODSystem::PlayCallBack(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, int command, unsigned int commanddata1, unsigned int commanddata2)
//{
//	FMOD::Channel* ch = (FMOD::Channel*) channel;
//	int index = 0;
//	if (ch)
//	{
//		bool isplaying;
//		ch->isPlaying(&isplaying);
//		ch->getIndex(&index);
//		//内部call back (用来自动释放资源）
//		//		if(m_pInnerCallback)
//		//			m_pInnerCallback->callback(index, command);
//		//外部（用户定义 call back)
//		if(m_pCallback)
//			m_pCallback->callback(index,command);
//	}
//	return FMOD_OK;
//}


//int CFMODSystem::CreateChannelGroup()
//{
//	FMOD::ChannelGroup* group;
//	if (m_pSystem->createChannelGroup("goup",&group) == FMOD_OK)
//	{
//		m_pRootGroup->addGroup(group);
//
//		GROUPINFO* pg = new GROUPINFO;
//		pg->bMute = false;
//		pg->pDSP = NULL;
//		pg->pGroup = group;
//		m_mpGroups.insert(make_pair(m_nGroupIndex++,pg));
//	}
//	return m_nGroupIndex -1;
//	//m_nGroupIndex
//}
//GROUPINFO* CFMODSystem::GetChannelGroup(int nGroup)
//{
//	map<int, GROUPINFO*>::iterator itr;
//	itr = m_mpGroups.find(nGroup);
//	if (itr != m_mpGroups.end())
//		return itr->second;
//	else
//		return NULL;
//}
//void CFMODSystem::AddToChannelGroup(int nChannel, int nChannelGroup)
//{
//	FMOD::Channel* channel = GetChannel(nChannel);
//	GROUPINFO* groupinfo = GetChannelGroup(nChannelGroup);
//
//	if (!channel || !groupinfo)
//		return;
//
//	channel->setChannelGroup(groupinfo->pGroup);
//}
//void CFMODSystem::SetGroupVolume(int nGroup, float fvol)
//{
//	GROUPINFO* info = GetChannelGroup(nGroup);
//
//	if (info)
//	{
//		info->pGroup->setVolume(fvol);
//	}
//
//}
//float CFMODSystem::GetGroupVolume(int nGroup)
//{
//	float fvol = 0.0f;
//	GROUPINFO* info = GetChannelGroup(nGroup);
//
//	if (info)
//	{
//		info->pGroup->getVolume(&fvol);
//	}
//	return fvol;
//
//}
//void CFMODSystem::ReleaseGroup(int nGroup)
//{
//	GROUPINFO* info = GetChannelGroup(nGroup);
//	if (info)
//	{
//		info->pGroup->release();
//	}
//}
//void CFMODSystem::SetGroupMute(int nGroup, bool bMute)
//{
//	GROUPINFO* info = GetChannelGroup(nGroup);
//	if (info)
//	{
//		info->pGroup->overrideMute(bMute);
//		info->bMute = bMute;
//	}
//
//}
//bool CFMODSystem::GetGroupMute(int nGroup)
//{
//	GROUPINFO* info = GetChannelGroup(nGroup);
//	if (info)
//	{
//		return info->bMute;	
//	}
//}
//void CFMODSystem::SetGroupPaused(int nGroup, bool bPaused)
//{
//	GROUPINFO* info = GetChannelGroup(nGroup);
//	if (info)
//	{
//		info->pGroup->overridePaused(bPaused);
//	}
//}
//
//void CFMODSystem::SetReverbType(int nRev)
//{
//	if (nRev < 0 || nRev > REVERB_COUNT-1)
//		return;
//
//	FMOD_RESULT re = m_pSystem->setReverbProperties(&g_Reverbs[nRev]);
//
//	if (re != FMOD_OK)
//		return;
//}
//
//void CFMODSystem::SetGlobalGroupPaused(bool bPaused)
//{
//	if (m_pRootGroup)
//	{
//		m_pRootGroup->overridePaused(bPaused);
//		m_bGlobalGroupPaused = bPaused;
//	}
//}
//bool CFMODSystem::GetGlobalGroupPaused()
//{
//	if (m_pRootGroup)
//	{
//		return m_bGlobalGroupPaused;
//	}
//}
//void CFMODSystem::SetGlobalGroupVolume(float volume)
//{
//	if (m_pRootGroup)
//	{
//		m_pRootGroup->setVolume(volume);
//	}
//}
//float CFMODSystem::GetGlobalGroupVolume()
//{
//	if (m_pRootGroup)
//	{
//		float volume;
//		m_pRootGroup->getVolume(&volume);
//		return volume;
//	}
//	return 0.0f;
//}
//
////add reverb dsp for sound effects.
//void CFMODSystem::SetSoundEffectReverb(int nChannelGroup, float romesize, float damp, float wetmix,
//									   float drymix, float width, float mode)
//{
//
//	FMOD::DSP *dsp = FindDspType(FMOD_DSP_TYPE_REVERB);
//	if (dsp)
//	{
//		dsp->setParameter( FMOD_DSP_REVERB_ROOMSIZE, romesize);
//		dsp->setParameter( FMOD_DSP_REVERB_DAMP, damp);
//		dsp->setParameter( FMOD_DSP_REVERB_WETMIX, wetmix);
//		dsp->setParameter( FMOD_DSP_REVERB_DRYMIX, drymix);
//		dsp->setParameter( FMOD_DSP_REVERB_WIDTH, width);
//		dsp->setParameter( FMOD_DSP_REVERB_MODE, mode);
//	}
//
//	//if(!m_pHighPassDsp)
//	//{
//	//	m_pSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &m_pHighPassDsp);
//	//	m_pSystem->addDSP(m_pHighPassDsp);
//	//}
//
//	//GROUPINFO* pGI = GetChannelGroup(nChannelGroup);
//	//if (pGI)
//	//{
//	//	FMOD::DSP* pDSP;
//	//	if (pGI->pDSP == NULL)
//	//	{
//	//		m_pSystem->createDSPByType(  FMOD_DSP_TYPE_REVERB, & pDSP);
//	//		pGI->pDSP = pDSP;
//	//		pGI->pGroup->addDSP(pDSP);
//	//	}
//	//	else
//	//		pDSP = pGI->pDSP;
//
//	//	if (pDSP)
//	//	{
//	//		pDSP->setParameter( FMOD_DSP_REVERB_ROOMSIZE, romesize);
//	//		pDSP->setParameter( FMOD_DSP_REVERB_DAMP, damp);
//	//		pDSP->setParameter( FMOD_DSP_REVERB_WETMIX, wetmix);
//	//		pDSP->setParameter( FMOD_DSP_REVERB_DRYMIX, drymix);
//	//		pDSP->setParameter( FMOD_DSP_REVERB_WIDTH, width);
//	//		pDSP->setParameter( FMOD_DSP_REVERB_MODE, mode);
//
//	//	}
//	//}
//}
//
//void CFMODSystem::SetReverbActive(bool bActive)
//{
//	//if(m_pReverbDsp)
//	//{
////		m_pDsp->setActive(bActive);
//	//	m_pReverbDsp->setBypass(bActive);
//	//}
//
//	//if(m_pHighPassDsp)
//	//{
//	//	m_pHighPassDsp->setBypass(bActive);
//	//}
//}
//
//FMOD::DSP*	CFMODSystem::FindDspType(FMOD_DSP_TYPE type)
//{
//	FMOD::DSP *dsp = NULL;
//	std::map<FMOD_DSP_TYPE, FMOD::DSP*>::iterator it;
//	it = m_ReverbDsp.find(type);
//	if(it == m_ReverbDsp.end())
//	{
//		m_pSystem->createDSPByType(type, &dsp);
//		m_pSystem->addDSP(dsp);
//		m_ReverbDsp[type] = dsp;
//	}
//	else
//	{
//		dsp = it->second;
//	}
//	return dsp;
//}
//
//void CFMODSystem::SetDspTypeActive(FMOD_DSP_TYPE type, bool bActive)
//{
//	//FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_ROOM;
//	//m_pSystem->setReverbProperties(&prop);
////	if(bActive)
////	{
//////		FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_ROOM;
////		FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_CAVE;
////		m_pSystem->setReverbProperties(&prop);
////	}
////	else
////	{
////		FMOD_REVERB_PROPERTIES prop = FMOD_PRESET_OFF;
////		m_pSystem->setReverbProperties(&prop);
////	}
//	FMOD::DSP* dsp = FindDspType(type);
//	dsp->setBypass(!bActive);
//	//switch(type) {
//	//case SND_DSP_TYPE_HIGHPASS:
//	//case SND_DSP_TYPE_LOWPASS:
//	//case SND_DSP_TYPE_ECHO:
//	//	{
//	//		dsp->setBypass(bActive);
//	//	}
//	//	break;
//	//case SND_DSP_TYPE_REVERB:
//	//	{
//
//	//	}
//	//	break;
//	//default:
//	//	break;
//	//}
//}
//
//void CFMODSystem::SetRolloff(int ichannelindex)
//{
//	CChannel *pchannel = GetCChannel(ichannelindex);
//	FMOD_VECTOR rollof[4] =
//	{
//		{ 0.0f, 1.0f, 0.0f },
//		{ 200.0f, 1.0f, 0.0f },
//		{ 900.0f, 0.8f, 0.0f },
//		{ 1000.0f, 0.0f, 0.0f }
//	};
//	pchannel->SetRolloff(rollof, 4);
////	pchannel->set3DCustomRolloff(rollof, 4);
//}
//
//void CFMODSystem::PlayCustomRollof(const char* filename)
//{
//	FMOD_RESULT result;
//	FMOD::Sound		*sound;
//	FMOD::Channel	*channel;
//
//	result = m_pSystem->createSound(filename, FMOD_SOFTWARE|FMOD_3D|FMOD_LOOP_NORMAL/*|FMOD_3D_CUSTOMROLLOFF*/|FMOD_CREATESTREAM, 0, &sound);
//	result = sound->set3DMinMaxDistance(100.0f, 5000.0f);
// 
//	result = sound->setMode(FMOD_3D_CUSTOMROLLOFF);
//
//
//	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
//	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
//
//	int channelid = -1;
//	FMOD_VECTOR rollof[4] =
//	{
//		{ 0.0f, 1.0f, 0.0f },
//		{ 200.0f, 1.0f, 0.0f },
//		{ 900.0f, 0.8f, 0.0f },
//		{ 1000.0f, 0.0f, 0.0f }
//	};
//	result = sound->set3DCustomRolloff(rollof, 4);
//	result = m_pSystem->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
//	channel->getIndex(&channelid);
//	result = channel->set3DAttributes(&pos, &vel);
//	result = channel->setPaused(false);
//
//}
//
//
//
