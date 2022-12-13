////////////////////////////////////////////////
//     FileName:    DetectorBase.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: 阶段任务完成情况检测器
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "TaskScript.h"
#include "Observer.h"

class CDetectorBase : public CSource 
{
public:
	CDetectorBase();
	virtual ~CDetectorBase();
	virtual void Start();
	virtual void Succeed();
	virtual void Fail();
	virtual void Update(float fTime);
	virtual void SetState(DWORD dwState);
	virtual DWORD GetState();
	virtual bool LoadConfig(CVarTable* pTable);
protected:
	void _SetState(DWORD dwState); 
protected:
	DWORD m_dwState;
};