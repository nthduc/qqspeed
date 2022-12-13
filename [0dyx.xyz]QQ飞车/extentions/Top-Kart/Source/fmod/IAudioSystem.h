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
*	һ������������,��װ�����������ļ�,����,�����������Եȵײ㹦��.
*	���Բ���2D��3D��Ƶ,����ͨ���ı���Դ�����ڵ�����,�����3D����Ч��.
*/
class IAudioSystem
{
public:
	/**
	*���캯��
	*/
	IAudioSystem(void);
	/**
	*��������
	*/
	virtual ~IAudioSystem(void);

	/**
	*��������.
	*���������ļ�����Ҫָ���ļ���������ķ�ʽ��
	*@param strFilename Ҫ������ļ���
	*@param nSoundType  ����ķ�ʽ����������������ֵ����ϣ�<P>
	*TSOUND_DEFAULT �൱��TSOUND_LOOP_OFF|TSOUND_2D|TSOUND_HARDWARE<P>
	*TSOUND_LOOP_OFF ��ѭ������<P>
	*TSOUND_LOOP_NORMAL ѭ������<P>
	*TSOUND_2D 2D����<P>
	*TSOUND_3D 3D����<P>
	*TSOUND_HARDWARE Ӳ������<P>
	*TSOUND_SOFTWARE �������<P>
	*TSOUND_CREATESTREAM ����������ʽ<p>
	*@return ��ʶһ��������Դ���������������ڴ���Դ�ĸ��ֲ���,��ͨ��������������С�
	*/
	virtual int CreateSound(const char* strFilename , int nSoundType) = 0;

	/**
	* ��������
	*@param nSound   ��Ҫ���ŵ�������Դ����
	*@param nChannel ����ʱҪռ�õ�ͨ����Ĭ��ֵΪCHANNEL_FREE����Ϊʹ�ÿ��е�ͨ�����Ŵ�����
	,���ָ����ͨ������ָ��ͨ�����ڲ��ţ���ֹͣ��ͨ������������Ƶ��
	*��ʹ��Play������ͨ��Ĭ��Ϊpaused��ʹ��SetChannelPaused()�������벥��״̬��
	*@see CreateSound 
	*@see SetChannelPaused
	*@return ���ز���ʱ��ʹ�õ�ͨ����
	*/
	virtual int Play(int nSound, int nChannel=CHANNEL_FREE)=0;

	/**
	*����ͨ������ͣ״̬
	*@param nChannel ͨ����
	*@param bPaused  �Ƿ���ͣ
	*/
	virtual void SetChannelPaused(int nChannel, bool bPaused) = 0;	

	/**
	*��ȡͨ������ͣ״̬
	*@param nChannel ͨ����
	*@param bPaused  �Ƿ���ͣ
	*/
	virtual bool GetChannelPaused(int nChannel) = 0;

	/**
	*ֹͣͨ���в��ŵ�����
	*@param nChannel ͨ����
	*/
	virtual void StopChannel(int nChannel) = 0;

	/*
	*����ͨ������״̬
	*@param nChannel ͨ����
	*@return ���nChannelͨ�������ڲ���״̬������true�����򷵻�false��
	*/
	virtual bool IsPlaying(int nChannel) = 0;

	/**
	*���ڴ����ͷ�������Դ
	*@param nSound ������Դ��������CreateSound()���� 
	*@see CreateSound
	*/
	virtual void ReleaseSound(int nSound) = 0;

	/**
	*����һ��ͨ����λ�ú��ٶ�
	*@param nChannel ͨ����
	*@param pos ͨ��λ��
	*@param vel ͨ���ٶ�
	*/
	virtual void SetChannel3DAttributes(int nChannel,const SOUND_VECTOR* pos, const SOUND_VECTOR* vel) = 0;

	/**
	*����һ��ͨ����λ�ú��ٶ�
	*@param nChannel ͨ����
	*@param posX,posY,posZ ͨ��λ��
	*@param velX,velY,velZ ͨ���ٶ�
	*@see SetChannel3DAttributes
	*/
	virtual void SetChannel3DAttributes(int nChannel,float posX, float posY, float posZ,
		float velX, float velY, float velZ) = 0;

	/**
	*�������ڵ�λ�á��ٶȺͳ���
	*@param pos ����λ��
	*@param vel �����ٶ�
	*@param forward ������Եķ���
	*@param up ����ͷ������
	*@see SetChannel3DAttributes
	*/
	virtual void Set3DListenerAttributes(
		const SOUND_VECTOR *  pos,
		const SOUND_VECTOR *  vel, 
		const SOUND_VECTOR *  forward, 
		const SOUND_VECTOR *  up) = 0;

	/**
	*��ȡ���ڵ�λ�á��ٶȺͳ���
	*@param pos ����λ��
	*@param vel �����ٶ�
	*@param forward ������Եķ���
	*@param up ����ͷ������
	*@see SetChannel3DAttributes
	*/
	virtual void Get3DListenerAttributes(
		SOUND_VECTOR *  pos,
		SOUND_VECTOR *  vel, 
		SOUND_VECTOR *  forward, 
		SOUND_VECTOR *  up) = 0;

	/**
	*����������˥������
	*@param nChannel ������
	*@param mindistance ˥������С����
	*@param maxdistance ˥����������
	*@see SetChannel3DAttributes
	*/
	virtual void Set3DMinMaxDistance(int nChannel,float  mindistance,float  maxdistance)=0;

	/**
	*��ȡ��Ƶ�ĳ���
	*@param nSound ������Դ����
	*@see CreateSound
	*@return ��Ƶ�ĳ��ȣ���λms
	*/
	virtual unsigned int GetSoundLength(int nSound) =0;

	/**
	*����ͨ����ǰ���ŵ���Ƶ��λ��
	*@param nChannel ͨ����
	*@param nPos ����λ�ã���λms
	*/
	virtual void SetChannelPosition(int nChannel, unsigned int nPos) = 0;

	/**
	*��ȡͨ����ǰ���ŵ�ӦƸ��λ��
	*@param nChannel ͨ����
	*@see setChannelPosition
	*@return ����λ�ã���λms
	*/
	virtual unsigned int GetChannelPosition(int nChannel) = 0;

	/**
	*����ͨ��������
	*@param nChannel ͨ����
	*@param fVol ���� ����Χ0.0f - 1.0f��
	*/
	virtual void SetVolume(int nChannel, float fVol) = 0;

	/**
	*��ȡͨ��������
	*@param nChannel ͨ����
	*@return ���� ����Χ0.0f - 1.0f��
	*/
	virtual float GetVolume(int nChannel) = 0;

	/**
	*��ȡ��Ƶ��Դ������Ӧ���ļ���
	*@param nSound ��Ƶ��Դ����
	*@return �ļ���
	*/
	virtual char* GetFilename(int nSound) = 0;

	/**
	*��ȡͨ�������ڲ��ŵ���Ƶ��Դ���
	*@nChannel ͨ����
	*@return ��Ƶ��Դ���
	*/
	virtual int GetPlayingSoundIndex(int nChannel) = 0;

	/**
	*�ͷ�������Դ.
	*��Ҫ���ͷ���Ƶ��Դ��ռ�õ��ڴ棬�ڵ��ô˺���֮����ǰ����Ƶ��Դ���ȫ��ʧЧ��
	*/
	virtual void ReaseAllSources() = 0;

	/**
	*����ͨ��ѭ���Ĵ���
	*@param nChannel ͨ����
	*@param nCount ѭ������ nCount = 1����ѭ������һ�Σ����ղ������Ρ�
	*/
	virtual void SetChannelLoopCount(int nChannel, int nCount) = 0;

	/**
	*������Ƶϵͳ.�������Ӧ������Ϸ��ÿ�θ���ʱ����
	*/
	virtual void Update() = 0;
	
	//virtual bool SetChannelPlayEndCallback(int nChannel) = 0;


	/**
	*�ر�����ϵͳ
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