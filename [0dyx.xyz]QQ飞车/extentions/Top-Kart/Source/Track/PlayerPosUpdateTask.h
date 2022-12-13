#pragma once
#include "../Task.h"

class CPlayerPosUpdateTask
	: public Task
{
public:
	CPlayerPosUpdateTask(void);
	// 修改人： wincoqin
	// 时间： 2007/1/21
	// 方式: 添加
	virtual ~CPlayerPosUpdateTask();
	// 添加结束
	virtual void Execute(unsigned int uiFrameCount, float fTime);

public:
	// 修改人： wincoqin
	// 时间： 2007/1/21
	// 方式: 添加
	// 目的： 此task为游戏创建，并且只需要创建一次
	static bool ms_bCreated;
	// 添加结束
protected:
	DWORD m_dwWrongwayShowTime; // 记录出现wrongway的次数
};

#include "PlayerPosUpdateTask.inl"