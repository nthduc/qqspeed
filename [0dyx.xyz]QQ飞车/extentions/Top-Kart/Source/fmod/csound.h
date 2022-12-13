#pragma once
#include "fmod.hpp"
#include "syscommon.h"

class CSound
{
public:
	CSound();
	~CSound();
	
	char strFilename[256];
	DWORD	nSoundIndex;//外部提供的一个数字标识

	FMOD::Sound* fmodSound;
	FMOD::Channel* fmodChannel;

	bool isPlaying;
	int nLoopCount;
	float fVolume;
	int nChannelId;

	int nType;
	//SOUND_TYPE m_nSoundType;
};

class CChannel
{
public:
	CChannel();
	CChannel(FMOD::Channel*);
	~CChannel();

	FMOD::Channel*	GetChannel()	{ return m_pChannel;}
	//自定义衰减曲线，关键点个数，相对比率
	void	SetChannel(FMOD::Channel* pChannel) { m_pChannel = pChannel;}
	void	SetRolloff(const FMOD_VECTOR* prolloff, int ikeynum, float fratio = 1.0f);
private:
	FMOD_VECTOR*	m_pRolloff;

	FMOD::Channel*	m_pChannel;
};