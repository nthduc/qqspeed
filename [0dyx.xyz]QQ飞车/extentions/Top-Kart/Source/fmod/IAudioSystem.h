#pragma once

#define TSOUND_DEFAULT  0x00000000
#define TSOUND_LOOP_OFF  0x00000001
#define TSOUND_LOOP_NORMAL  0x00000002
#define TSOUND_2D  0x00000008
#define TSOUND_3D  0x00000010
#define TSOUND_HARDWARE  0x00000020
#define TSOUND_SOFTWARE  0x00000040
#define TSOUND_CREATESTREAM  0x00000080



#define DOPPLER_SCALE         1.0
#define DISTANCE_FACTOR       1.0
#define ROLLOFF_SCALE         0.5


#define CHANNEL_FREE          -1

#ifndef SOUNDVECTOR_
typedef struct Q3
{
	float x, y, z;
}SOUND_VECTOR;
#define SOUNDVECTOR_
#endif 

typedef struct {
	long param1;
	long param2;
	long param3;
}STRCTCALLBACK;



class IAudioSystem;

class IChannelCallBack{
public:
	virtual void callback(int nChannel,int command)=0;
};

/**
*	一个声音抽象类,封装了载入声音文件,播放,控制声音属性等底层功能.
*	可以播放2D和3D音频,可以通过改变声源和听众的属性,来获得3D声场效果.
*/
class IAudioSystem
{
public:
	/**
	*构造函数
	*/
	IAudioSystem(void);
	/**
	*析构函数
	*/
	virtual ~IAudioSystem(void);

	/**
	*创建声音.
	*载入声音文件，需要指定文件名和载入的方式。
	*@param strFilename 要载入的文件名
	*@param nSoundType  载入的方式，可以是下面若干值的组合：<P>
	*TSOUND_DEFAULT 相当于TSOUND_LOOP_OFF|TSOUND_2D|TSOUND_HARDWARE<P>
	*TSOUND_LOOP_OFF 不循环播放<P>
	*TSOUND_LOOP_NORMAL 循环播放<P>
	*TSOUND_2D 2D声音<P>
	*TSOUND_3D 3D声音<P>
	*TSOUND_HARDWARE 硬件加速<P>
	*TSOUND_SOFTWARE 软件播放<P>
	*TSOUND_CREATESTREAM 采用流的形式<p>
	*@return 标识一个声音资源的索引。后续对于此资源的各种操作,都通过这个索引来进行。
	*/
	virtual int CreateSound(const char* strFilename , int nSoundType) = 0;

	/**
	* 播放声音
	*@param nSound   需要播放的声音资源索引
	*@param nChannel 播放时要占用的通道。默认值为CHANNEL_FREE，意为使用空闲的通道播放此声音
	,如果指定了通道号且指定通道正在播放，则停止该通道，播放新音频。
	*在使用Play函数后，通道默认为paused，使用SetChannelPaused()函数进入播放状态。
	*@see CreateSound 
	*@see SetChannelPaused
	*@return 返回播放时将使用的通道号
	*/
	virtual int Play(int nSound, int nChannel=CHANNEL_FREE)=0;

	/**
	*设置通道的暂停状态
	*@param nChannel 通道号
	*@param bPaused  是否暂停
	*/
	virtual void SetChannelPaused(int nChannel, bool bPaused) = 0;	

	/**
	*获取通道的暂停状态
	*@param nChannel 通道号
	*@param bPaused  是否暂停
	*/
	virtual bool GetChannelPaused(int nChannel) = 0;

	/**
	*停止通道中播放的声音
	*@param nChannel 通道号
	*/
	virtual void StopChannel(int nChannel) = 0;

	/*
	*返回通道播放状态
	*@param nChannel 通道号
	*@return 如果nChannel通道正处于播放状态，返回true，否则返回false。
	*/
	virtual bool IsPlaying(int nChannel) = 0;

	/**
	*从内存中释放声音资源
	*@param nSound 声音资源索引，由CreateSound()创建 
	*@see CreateSound
	*/
	virtual void ReleaseSound(int nSound) = 0;

	/**
	*设置一个通道的位置和速度
	*@param nChannel 通道号
	*@param pos 通道位置
	*@param vel 通道速度
	*/
	virtual void SetChannel3DAttributes(int nChannel,const SOUND_VECTOR* pos, const SOUND_VECTOR* vel) = 0;

	/**
	*设置一个通道的位置和速度
	*@param nChannel 通道号
	*@param posX,posY,posZ 通道位置
	*@param velX,velY,velZ 通道速度
	*@see SetChannel3DAttributes
	*/
	virtual void SetChannel3DAttributes(int nChannel,float posX, float posY, float posZ,
		float velX, float velY, float velZ) = 0;

	/**
	*设置听众的位置、速度和朝向
	*@param pos 听众位置
	*@param vel 听众速度
	*@param forward 听众面对的方向
	*@param up 听众头顶方向
	*@see SetChannel3DAttributes
	*/
	virtual void Set3DListenerAttributes(
		const SOUND_VECTOR *  pos,
		const SOUND_VECTOR *  vel, 
		const SOUND_VECTOR *  forward, 
		const SOUND_VECTOR *  up) = 0;

	/**
	*获取听众的位置、速度和朝向
	*@param pos 听众位置
	*@param vel 听众速度
	*@param forward 听众面对的方向
	*@param up 听众头顶方向
	*@see SetChannel3DAttributes
	*/
	virtual void Get3DListenerAttributes(
		SOUND_VECTOR *  pos,
		SOUND_VECTOR *  vel, 
		SOUND_VECTOR *  forward, 
		SOUND_VECTOR *  up) = 0;

	/**
	*设置声道的衰减距离
	*@param nChannel 声道号
	*@param mindistance 衰减的最小距离
	*@param maxdistance 衰减的最大距离
	*@see SetChannel3DAttributes
	*/
	virtual void Set3DMinMaxDistance(int nChannel,float  mindistance,float  maxdistance)=0;

	/**
	*获取音频的长度
	*@param nSound 声音资源索引
	*@see CreateSound
	*@return 音频的长度，单位ms
	*/
	virtual unsigned int GetSoundLength(int nSound) =0;

	/**
	*设置通道当前播放的音频的位置
	*@param nChannel 通道号
	*@param nPos 播放位置，单位ms
	*/
	virtual void SetChannelPosition(int nChannel, unsigned int nPos) = 0;

	/**
	*获取通道当前播放的应聘的位置
	*@param nChannel 通道号
	*@see setChannelPosition
	*@return 播放位置，单位ms
	*/
	virtual unsigned int GetChannelPosition(int nChannel) = 0;

	/**
	*设置通道的音量
	*@param nChannel 通道号
	*@param fVol 音量 （范围0.0f - 1.0f）
	*/
	virtual void SetVolume(int nChannel, float fVol) = 0;

	/**
	*获取通道的音量
	*@param nChannel 通道号
	*@return 音量 （范围0.0f - 1.0f）
	*/
	virtual float GetVolume(int nChannel) = 0;

	/**
	*获取音频资源索引对应的文件名
	*@param nSound 音频资源索引
	*@return 文件名
	*/
	virtual char* GetFilename(int nSound) = 0;

	/**
	*获取通道内正在播放的音频资源序号
	*@nChannel 通道号
	*@return 音频资源序号
	*/
	virtual int GetPlayingSoundIndex(int nChannel) = 0;

	/**
	*释放所有资源.
	*主要是释放音频资源所占用的内存，在调用此函数之后，先前的音频资源序号全部失效。
	*/
	virtual void ReaseAllSources() = 0;

	/**
	*设置通道循环的次数
	*@param nChannel 通道号
	*@param nCount 循环次数 nCount = 1，则循环播放一次，最终播放两次。
	*/
	virtual void SetChannelLoopCount(int nChannel, int nCount) = 0;

	/**
	*更新音频系统.这个函数应当在游戏的每次更新时调用
	*/
	virtual void Update() = 0;
	
	//virtual bool SetChannelPlayEndCallback(int nChannel) = 0;


	/**
	*关闭声音系统
	*/
	virtual void Close() = 0;
	

	virtual void SetCallBack(IChannelCallBack* callback) = 0;
	virtual void SetChannelCallBack(int nChannel, STRCTCALLBACK* pSC) = 0;
	

//	virtual void Play2DSound(const char* filename, int nLoopCount) = 0;

	virtual int CreateChannelGroup() = 0;
	virtual void AddToChannelGroup(int nChannel, int nChannelGroup) = 0;
	virtual void SetGroupVolume(int nGroup, float fvol) = 0;
	virtual float GetGroupVolume(int nGroup) = 0;
	virtual void ReleaseGroup(int nGroup) = 0;
	virtual void SetGroupMute(int nGroup, bool bMute) = 0;
	virtual bool GetGroupMute(int nGroup) = 0;
	static IAudioSystem* m_pAudioSystem;

};

extern "C" _declspec(dllexport) IAudioSystem* CreateAudioSystem(char* strSystem);