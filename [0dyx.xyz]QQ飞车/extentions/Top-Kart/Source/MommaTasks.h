

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
	// �޸��ˣ� wincoqin
	// ʱ�䣺 2007/1/21
	// ��ʽ: ���
	// Ŀ�ģ� ��taskΪ��Ϸ����������ֻ��Ҫ����һ��
	static bool ms_bCreated;
	// ��ӽ���
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
