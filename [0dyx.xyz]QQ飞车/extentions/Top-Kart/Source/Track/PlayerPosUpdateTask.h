#pragma once
#include "../Task.h"

class CPlayerPosUpdateTask
	: public Task
{
public:
	CPlayerPosUpdateTask(void);
	// �޸��ˣ� wincoqin
	// ʱ�䣺 2007/1/21
	// ��ʽ: ���
	virtual ~CPlayerPosUpdateTask();
	// ��ӽ���
	virtual void Execute(unsigned int uiFrameCount, float fTime);

public:
	// �޸��ˣ� wincoqin
	// ʱ�䣺 2007/1/21
	// ��ʽ: ���
	// Ŀ�ģ� ��taskΪ��Ϸ����������ֻ��Ҫ����һ��
	static bool ms_bCreated;
	// ��ӽ���
protected:
	DWORD m_dwWrongwayShowTime; // ��¼����wrongway�Ĵ���
};

#include "PlayerPosUpdateTask.inl"