/*
 *	SndMixer.h
 *  Sound Mixer functions header.
 *
 *  Mike, 2003.12.31.
 */
#ifndef _SND_MIXER_H_
#define _SND_MIXER_H_

#include <windows.h>

typedef unsigned long HWAV;
typedef unsigned long HMUSIC;

//add+lincoln
//[2004:11:25] [15:29]
#ifdef BUILD_DLL
#define  EXPORT_DLL __declspec(dllexport) 
#else 
#define  EXPORT_DLL 
#endif


class ISndMixer {
	static ISndMixer*	sndMixer_;
public:
	virtual ~ISndMixer(){};

	//modify+lincoln
	//[2004:11:25] [15:29]
	//创建和释放声音接口
	EXPORT_DLL static ISndMixer*	Create();
	EXPORT_DLL static void			Destroy();

//音乐（music）函数
	//载入音乐文件，返回音乐句柄
	virtual HMUSIC LoadMusic( const char* fileName)=0;

	//释放音乐句柄
	virtual void FreeMusic( HMUSIC hMusic)=0;

	//播放音乐. Loops: 表示播放次数。 -1表示无限循环播放。0表示播放1次
	virtual int PlayMusic( HMUSIC hMusic, int loops)=0; 

	//停止播放音乐
	virtual int HaltMusic()=0;

	//设置音乐的音量，返回原来的音量
	virtual int VolumeMusic( int volume)=0;//				{ return Mix_VolumeMusic( volume);}

	//设置音乐播放完毕的回调函数
	virtual void HookMusicFinished( void (*music_finished)(void))=0;

//音效函数
	//载入音效文件，返回音效句柄
	virtual HWAV LoadWAV( const char* fileName)=0;

	//释放音效句柄
	virtual void FreeWAV( HWAV hWav)=0;

	//播放音效
	//channel: 指定播放音效的通道. -1表示使用任意一个空闲通道
	//loop:    循环播放的次数。 -1表示无限循环，	
	//返回：   播放音效的通道号
	virtual int PlayChannel( int channel, HWAV hWav, int loops)=0;

	//停止播放某通道
	virtual int HaltChannel( int channel)=0;

	//设置音量
	//channel: 通道号。 -1表示设置所有通道的音量
	//volume:  0-128. -1表示只返回某通道的音量，不改变它的音量。
	//返回：   原来的音量
	virtual int Volume( int channel, int volume)=0;

	//设置声音位置。 Angle: 0-360, distance: 0-255
	virtual int SetPosition( int channel, short angle, BYTE distance)=0;

	virtual bool isPlaying( int channel)=0;//				{ return Mix_Playing(channel)!=0; }
};

#endif //_SND_MIXER_H_

