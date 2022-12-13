#include "StdAfx.h"
#include "..\..\..\include\soundefx.h"
#include <map>
#include <string>

namespace SoundEfx_Kernel
{
	static FMOD::System* pSys=NULL;
    static CSoundEfx* pSoundEfx=NULL;
	static std::map<std::string,FMOD::Sound*> pSoundSource;
	typedef std::map<std::string,FMOD::Sound*>::iterator iSoundSoure;
	typedef std::map<std::string,FMOD::Sound*>::const_iterator ciSoundSoure;
};

using namespace SoundEfx_Kernel;

CSoundEfx::CSoundEfx(void)
{
	SoundEfx_Kernel::pSoundEfx = NULL;
	SoundEfx_Kernel::pSys = NULL;

	SoundEfx_Kernel::pSoundSource.clear();

};

bool CSoundEfx::Init(HWND hWnd)
{
	FMOD_RESULT fr = FMOD::System_Create(&SoundEfx_Kernel::pSys);

	if(fr != FMOD_OK)
	{
		SoundEfx_Kernel::pSys = NULL;
		return false;
	}

	if(SoundEfx_Kernel::pSys)
	{
		SoundEfx_Kernel::pSys->setDriver(-1);
		SoundEfx_Kernel::pSys->setOutput(FMOD_OUTPUTTYPE_DSOUND);
		SoundEfx_Kernel::pSys->init(100,FMOD_INIT_NORMAL, (void*)hWnd);
		
	}

	return true;
}

CSoundEfx::~CSoundEfx(void)
{
	if(SoundEfx_Kernel::pSys)
	{
		FMOD::ChannelGroup* cg;
		cg = NULL;
		SoundEfx_Kernel::pSys->getMasterChannelGroup(&cg);
		cg->stop();
		cg->release();
		cg = NULL;

		iSoundSoure it;

		for(it = SoundEfx_Kernel::pSoundSource.begin();
			it != SoundEfx_Kernel::pSoundSource.end();
			it++)
		{
			it->second->release();
		}

		SoundEfx_Kernel::pSys->release();
		SoundEfx_Kernel::pSys = NULL;
	}

};

//创建接口
CSoundEfx* CSoundEfx::CreateSoundEfx(HWND hWnd)
{
	if(SoundEfx_Kernel::pSoundEfx)
	{
		return SoundEfx_Kernel::pSoundEfx;
	}
	 
	SoundEfx_Kernel::pSoundEfx = new CSoundEfx;
	
	if(SoundEfx_Kernel::pSoundEfx->Init(hWnd))
	{
		return SoundEfx_Kernel::pSoundEfx;
	}

	ShutDownSoundEfx();

	return NULL;
};

CSoundEfx* CSoundEfx::GetSoundEfx()
{
	return SoundEfx_Kernel::pSoundEfx;
};


//简单的
bool CSoundEfx::Play2DSound(LPCSTR lpcSoundName,bool bStream)
{
	 if(!pSys)
	 {
		 return false;
	 }

	 FMOD_RESULT fr;
	 ciSoundSoure it = pSoundSource.find(lpcSoundName);
 
	 if(it == pSoundSource.end())
	 {
		FMOD::Sound* pS;

		if(bStream)
		{
			fr = pSys->createStream(lpcSoundName,FMOD_DEFAULT,NULL,&pS);
		}
		else
		{
			fr = pSys->createSound(lpcSoundName,FMOD_DEFAULT,NULL,&pS);
		}

		if(fr != FMOD_OK)
		{
			return false;
		}

		pSoundSource[lpcSoundName] = pS;
		pSys->playSound(FMOD_CHANNEL_FREE,pS, false,NULL);
		return true;
	 }

	 pSys->playSound(FMOD_CHANNEL_FREE,(FMOD::Sound*)it->second, false,NULL);

	return true;
}

void CSoundEfx::Update(int time)
{
	if(pSys)
	{
		pSys->update();
	}
}

bool CSoundEfx::Stop2DSound(LPCSTR lpcSoundName)
{
	return false;
}


void CSoundEfx::ShutDownSoundEfx()
{
	if(SoundEfx_Kernel::pSoundEfx)
	{
		delete SoundEfx_Kernel::pSoundEfx;
		SoundEfx_Kernel::pSoundEfx = NULL;
	}
}

