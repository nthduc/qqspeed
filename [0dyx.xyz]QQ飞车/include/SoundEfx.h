#pragma once
#include "windows.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma comment(lib,"fmodexp_vc.lib")

class CSoundEfx
{
public:

//�����ӿ�
static CSoundEfx* CreateSoundEfx(HWND hWnd);
static CSoundEfx* GetSoundEfx();

static void ShutDownSoundEfx();



//���Žӿ�
bool   Play2DSound(LPCSTR lpcSoundName,bool bStream = true);
bool   Stop2DSound(LPCSTR lpcSoundName);

void   Update(int time);


private :
	bool Init(HWND hWnd); 
	CSoundEfx(void);
	~CSoundEfx(void);
};
