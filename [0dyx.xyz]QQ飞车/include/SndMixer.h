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
	//�������ͷ������ӿ�
	EXPORT_DLL static ISndMixer*	Create();
	EXPORT_DLL static void			Destroy();

//���֣�music������
	//���������ļ����������־��
	virtual HMUSIC LoadMusic( const char* fileName)=0;

	//�ͷ����־��
	virtual void FreeMusic( HMUSIC hMusic)=0;

	//��������. Loops: ��ʾ���Ŵ����� -1��ʾ����ѭ�����š�0��ʾ����1��
	virtual int PlayMusic( HMUSIC hMusic, int loops)=0; 

	//ֹͣ��������
	virtual int HaltMusic()=0;

	//�������ֵ�����������ԭ��������
	virtual int VolumeMusic( int volume)=0;//				{ return Mix_VolumeMusic( volume);}

	//�������ֲ�����ϵĻص�����
	virtual void HookMusicFinished( void (*music_finished)(void))=0;

//��Ч����
	//������Ч�ļ���������Ч���
	virtual HWAV LoadWAV( const char* fileName)=0;

	//�ͷ���Ч���
	virtual void FreeWAV( HWAV hWav)=0;

	//������Ч
	//channel: ָ��������Ч��ͨ��. -1��ʾʹ������һ������ͨ��
	//loop:    ѭ�����ŵĴ����� -1��ʾ����ѭ����	
	//���أ�   ������Ч��ͨ����
	virtual int PlayChannel( int channel, HWAV hWav, int loops)=0;

	//ֹͣ����ĳͨ��
	virtual int HaltChannel( int channel)=0;

	//��������
	//channel: ͨ���š� -1��ʾ��������ͨ��������
	//volume:  0-128. -1��ʾֻ����ĳͨ�������������ı�����������
	//���أ�   ԭ��������
	virtual int Volume( int channel, int volume)=0;

	//��������λ�á� Angle: 0-360, distance: 0-255
	virtual int SetPosition( int channel, short angle, BYTE distance)=0;

	virtual bool isPlaying( int channel)=0;//				{ return Mix_Playing(channel)!=0; }
};

#endif //_SND_MIXER_H_

