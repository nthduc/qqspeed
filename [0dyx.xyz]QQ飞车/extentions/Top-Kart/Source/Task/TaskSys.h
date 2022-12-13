////////////////////////////////////////////////
//     FileName:    TaskSys.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: 任务
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "SubTask.h"
#include "AtomBase.h"
#include "Observer.h"
#include "GarbagePool.h"
#include <queue>
using namespace std;

class CTask : public CObserver
{
public:
	CTask();
	~CTask();
	bool Init(PTASKCONFIG pTaskCfg);	//初始化
	void Start();						//开始
	void Finish();						//完成
	void Fail();						//失败
	void Update(float fTime);			//更新
	PTASKCONFIG GetTaskCfg();			//返回任务的配置
	DWORD GetState();					//返回任务的当前状态
	void UpdateMessage();				//更新收发信息

private:
	CSubTask* _PopSubTask();			//弹出新的子任务
	void _InitSwitch();					//初始化开关表（小地图，氮气条等）

private:
	queue<CSubTask*>		m_qSubTask;				//子任务队列
	CSubTask*				m_pCurSubTask;			//当前子任务
	list<CAtomBase*>		m_listPeriodAtom;		//任务周期内的原子链表
	PTASKCONFIG				m_pTaskCfg;				//任务配置
	DWORD					m_dwState;				//任务的当前状态
	CGarbagePool<CSubTask>	m_subTaskGarbagePool;	//子任务垃圾回收池
};
