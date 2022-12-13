#pragma once 
#include "NxVec3.h"
#define MAX_KEY_NUM			108
#define UDP_MSG_PLAYMOVE   20000

struct STUdpPack
{
	UINT MsgID;
	void * pUdpData;
};

class CUdpPackagePlayMove
{
	UINT unUin;		//Դ���uin
	UCHAR bHasKeyChange;  //�а����仯û��
	UCHAR bArrKey[MAX_KEY_NUM];		//����״̬�����а����仯ʱ�������ã�
	NxVec3 m_stLinearVelocity[3];		//��������������ٶ�
	NxVec3 m_stAngularVelocity;		//��������������ٶ�
	NxVec3 m_stArrPose[4];			//�������������̬��������ת�����λ�ã�
	UINT unPackageIndex;	//�����
	UINT unTimeStamp;		//ʱ���
};