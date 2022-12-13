

#ifndef MOMMATASKS_H
#define MOMMATASKS_H

#include "Task.h"

//----------------------------------------------------------------------------
class MommaUpdateAnimationsTask : public Task
{
public:
    MommaUpdateAnimationsTask();
	virtual ~MommaUpdateAnimationsTask(); // Add by wincoqin
    virtual void Execute(unsigned int uiFrameCount, float fTime);

public:
	// 修改人： wincoqin
	// 时间： 2007/1/21
	// 方式: 添加
	// 目的： 此task为游戏创建，并且只需要创建一次
	static bool ms_bCreated;
	// 添加结束
};
//----------------------------------------------------------------------------
class MommaJumpTask : public Task
{
public:
    MommaJumpTask(unsigned int uiWhich);
    virtual void Execute(unsigned int uiFrameCount, float fTime);
protected:
    unsigned int m_uiPadID;
};
//----------------------------------------------------------------------------

#include "MommaTasks.inl"

#endif // MOMMATASKS_H
