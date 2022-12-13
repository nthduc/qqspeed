#pragma once

#ifndef __CFMODSYSTEM_H__
#define __CFMODSYSTEM_H__
//#include "iaudiosystem.h"

#include <fmod.hpp>
#include <map>
#include <vector>
#include "csound.h"
#include "singlet.h"


#define MAX_SOUND_CHANNELS       50
#define MAX_SOUND_NUM            1024

#define INVALID_SOUND_INDEX      0
#define INVALID_SOUND_CHANNEL    -1
#define CHANNEL_FREE          -1

#ifndef SOUNDVECTOR_

typedef struct Q3
{
	float x, y, z;
}SOUND_VECTOR;

#define SOUNDVECTOR_
#endif 
using namespace std;


typedef pair <int, CSound*> Sound_Pair;
typedef map <int, CSound*>::const_iterator Sound_iterator;

class CFMODSystem;
class CSound;

class CFMODSystem  : public CSingleT<CFMODSystem>
{

public:
	//static CFMODSystem* GetFMODSystem();

	//static CFMODSystem* m_pstFMODSystem;
	CFMODSystem(void);
	~CFMODSystem(void);

	void Update();

	void Play2D(const char* szFilename, DWORD dwSoundId, float fVolume, int nLoopCount, bool bStream);
	int HasSoundLoaded(DWORD dwSoundNumericalId);

	//基本函数
	 int CreateSound(const char* strFilename , int nSoundType, CSound*& pSound);
	 void UpdatePlayingState();
	 int GetFreeChannel();
	 void StopSound(DWORD nSoundId);
	 void StopAllSound();
	 bool IsSoundPlaying(DWORD nSoundId);
	 CSound* GetSound(int nSoundResourceId);
	 void SetSource(DWORD unType, const char* strName, float fVolume, int nLoopCount, bool bStream);
	 void PlayMotorSound(float curSpeed, const char* szFilename, DWORD dwSoundId, float fVolume, int nLoopCount, bool bStream);

	 void SetVolume(DWORD dwSoundId, float fVolume);

	 void SetPlaybackRate(DWORD dwSoundId, float rate);

	 void ReleaseSound(DWORD dwSoundId);
	 void ReleaseAll();

	// int CreateSound(const char* strFilename, const char* pfilebuf, int filelen, int nSoundType, unsigned int& length);
	// int Play(int nSound, int nChannel=CHANNEL_FREE);

	////音乐播放
	// void SetChannelPaused(int nChannel, bool bPaused);

	// bool IsPlaying(int nChannel);
	// bool IsPaused(int nChannel);

	// void StopChannel(int nChannel);

	////通道属性
	// bool GetChannelPaused(int nChannel);
	// void SetChannel3DAttributes(int nChannel, const SOUND_VECTOR* pos, const SOUND_VECTOR* vel);
	// void SetChannel3DAttributes(int nChannel,float posX, float posY, float posZ,
	//	float velX, float velY, float velZ);

	// void Set3DMinMaxDistance(int nChannel,float  mindistance, float  maxdistance);

	////音频管理
	// void ReleaseSound(int nSound);

	// void Set3DListenerAttributes(
	//	const SOUND_VECTOR *  pos,
	//	const SOUND_VECTOR *  vel, 
	//	const SOUND_VECTOR *  forward, 
	//	const SOUND_VECTOR *  up);

	// void Get3DListenerAttributes(
	//	SOUND_VECTOR *  pos,
	//	SOUND_VECTOR *  vel, 
	//	SOUND_VECTOR *  forward, 
	//	SOUND_VECTOR *  up);

	// unsigned int GetSoundLength(int nSound);

	// void SetVolume(int nChannel, float fVol);
	// float GetVolume(int nChannel) ;
	// char* GetFilename(int nSound) ;
	// int GetPlayingSoundIndex(int nChannel) ;


	// void ReaseAllSources();
	// void ReleaseGroup();
	// void ReleaseSound();


	// void SetChannelLoopCount(int nChannel, int nCount);

	 void Close();

	 //void SetCallBack(IChannelCallBack* callback);
	// void SetChannelCallBack(int nChannel, STRCTCALLBACK* pSC);


	// int CreateChannelGroup();
	// void AddToChannelGroup(int nChannel, int nChannelGroup);
	// void SetGroupVolume(int nGroup, float fvol);
	// float GetGroupVolume(int nGroup);
	// void ReleaseGroup(int nGroup);
	// void SetGroupMute(int nGroup, bool bMute);
	// bool GetGroupMute(int nGroup);
	// void SetGroupPaused(int nGroup, bool bPaused);

	// void SetReverbType(int nRev);

	//void SetGlobalGroupPaused(bool bPaused);
	//bool GetGlobalGroupPaused();

	//void SetGlobalGroupVolume(float volume);
	//float GetGlobalGroupVolume();

	//void SetSoundEffectReverb(int nChannelGroup, float romesize, float damp, float wetmix,
	//	float drymix, float width, float mode);
	//void SetReverbActive(bool bActive);
	//void SetDspTypeActive(FMOD_DSP_TYPE type, bool bActive);
	//FMOD::DSP* FindDspType(FMOD_DSP_TYPE type);
	//void SetDefaultDspProperties();

	//int	 InsertSubSound(int* index, unsigned int subsoundnum);
	//bool AddSubSound(int iParentid, int iChildid, int index);
	//int	 CreateSubSound();
	//void PlaySubSound(const char* filename1, const char* filename2);
	//void PlaySubSound(const char** filename, unsigned int subsoundnum);
	//void SetSequence(int Soundindex, int* index, int num);
	//void SetRolloff(int ichannelindex);
	//void PlayCustomRollof(const char* filename);

	// void Play2DSound(const char* filename, int nLoopCount);
	FMOD::System* m_pSystem;
private:

	FMOD::Sound*  m_pBakSound;	
	
	int m_nMaxSoundIndex;
	std::map<int ,CSound*> m_mpSounds;
	//std::map<int, CChannel*> m_mpChannels;
	bool m_arSoundIds[MAX_SOUND_NUM];

	void Init(void);
	//FMOD::Sound* FindSound(int nSound);
	//CSound* FindSoundObj(int nSound);
	//FMOD::Channel* GetChannel(int nChannel);
	//CChannel* GetCChannel(int nChannel);
	DWORD GetValidSoundIndex(void);

	//static IChannelCallBack* m_pCallback;
	//	static InnerCallBack* m_pInnerCallback;
	int m_nGroupIndex;
//	std::map<int, GROUPINFO*> m_mpGroups;
//	GROUPINFO* GetChannelGroup(int nGroup);

//	FMOD::ChannelGroup * m_pRootGroup;
//	bool m_bGlobalGroupPaused;

//	std::map<FMOD_DSP_TYPE, FMOD::DSP*>		m_ReverbDsp;
	//FMOD::DSP*	m_pReverbDsp;			//test novelwu 07/03/21
	//FMOD::DSP*	m_pHighPassDsp;			//test novelwu 07/03/21
	//FMOD::DSP*	m_pEchoDsp;				//test novelwu 07/03/21


//	int	 m_iSubSoundlist[5];			//声音子集
	//static FMOD_RESULT F_CALLBACK PlayCallBack(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, int command, unsigned int commanddata1, unsigned int commanddata2);


	CSound* m_apChannels[MAX_SOUND_CHANNELS];//确定每个channel与sound的关系


public:

};
#endif 
