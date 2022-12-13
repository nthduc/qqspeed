////////////////////////////////////////////////
//     FileName:    Executor.h
//     Author:      rayhuang
//     Date:        2007-5-21
//     Description: 任务执行者
////////////////////////////////////////////////

#pragma once 

class CExecutor
{
public:
	CExecutor();
	~CExecutor();
	void AttachEffect();
	void SetMovable(bool bState);
};