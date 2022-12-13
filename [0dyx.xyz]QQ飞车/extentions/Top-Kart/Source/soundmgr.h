////////////////////////////////////////////////
//     FileName:    soundmgr.h
//     Author:      Fish
//     Date:        2007-2-2 9:38:05
//     Description: []
////////////////////////////////////////////////

#ifndef _SOUNDMGR_H_
#define _SOUNDMGR_H_

#include "string"
#include "list"
using namespace std;
#include "idgen.h"
#include "singlet.h"

//#include "AudioEditor_i.h"
//#include "teniopointer_t.h"
//using namespace Tenio;
class CFMODSystem;

//class NiAudioSystem;
typedef struct tagSOUND_CONFIG
{
	DWORD	dwSoundID;
	string	strFileName;
	float	fGain;
	int		nLoop;
}SOUND_CONFIG, *PSOUND_CONFIG, *LPSOUND_CONFIG;

typedef list<SOUND_CONFIG> SOUND_CONFIG_LIST;
typedef SOUND_CONFIG_LIST::iterator SOUND_CONFIG_LIST_Iterator;

typedef struct tagSOUND_DATA
{
	DWORD	dwSoundID;//声音文件标识符
	DWORD	dwSoundIndex;//声音在载入FMOD后返回的索引
	DWORD   dwChannelIndex;//声音播放后在FMOD中声道索引
//	NiAudioSourcePtr	pSound;
	DWORD	dwStartTime;
	BOOL	bClone;
}SOUND_DATA, *PSOUND_DATA, *LPSOUND_DATA;

typedef list<SOUND_DATA> SOUND_DATA_LIST;
typedef SOUND_DATA_LIST::iterator SOUND_DATA_LIST_Iterator;

class CSoundMgr : public CSingleT<CSoundMgr>
{
public:

	enum SoundType
	{
		ST_NONE = 899,
		ST_AMBIENT,
		ST_ROOM,
		ST_MOTOR_LOW,
		ST_MOTOR_MID,
		ST_MOTOR_HIGH,
		ST_MOTOR_STOP,
		ST_BRAKE,
		ST_SHIFT,
		ST_HIT,	
		ST_SPEEDUP,
		ST_CARBARN,
		ST_WAIT_START,
		ST_GO,
		MAX_SOUND_STATES
	};

	CSoundMgr();
	~CSoundMgr();

	int Initialize();
	void UnInitialize();

	void Update(DWORD dwTime);

	DWORD PlaySound(DWORD dwSoundID, BOOL bNewInstance = FALSE);
	DWORD StopSound(DWORD dwSoundIndex);
	void StopAllSound();
	void SetSource(DWORD unType, const char* strName, float fVolume, int nLoopCount, bool bStream);
	void SetPlaybackRate(DWORD dwSoundIndex, long rate);
	DWORD StopSoundByID(DWORD dwSoundID);
	DWORD RemoveSound(DWORD dwSoundIndex);
	void PlayMotorSound(float curSpeed);

	LPSOUND_CONFIG	FindConfig(DWORD dwSoundID);
	LPSOUND_DATA	FindSound(DWORD dwSoundID, DWORD dwSoundIndex = 0);
	LPSOUND_DATA	FindSoundByIndex(DWORD dwSoundIndex);

private:
	void	GarbageCollect();

private:
	//NiAudioSystem*		m_pAudioSystem;		
	SOUND_CONFIG_LIST	m_listConfig;
	SOUND_DATA_LIST		m_listSound;
	CIDGen				m_kIDGen;

	CFMODSystem* m_pFMODSystem;
	////TTenioPtr<IAudioEditor> m_pAudioEditor;
	////返回声音文件载入后在FMOD系统中的索引号
	//int LoadSound(const char* filename, bool bStream);
	////返回声音文件播放后在FMOD系统中占用的声道号
	//int PlaySound(const SOUND_DATA& data);
	////void SetVolume(SoundType eSoundType,float fVolume);
};

#endif