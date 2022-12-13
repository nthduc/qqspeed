////////////////////////////////////////////////
//     FileName:    SubTask.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: 子任务
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "AtomLevel.h"
#include "Observer.h"
#include "AtomBase.h"
#include "GarbagePool.h"
#include <queue>
using namespace std;

class CSubTask : public CObserver, public CSource
{
	friend class CTask;
public:
	CSubTask();
	~CSubTask();
	bool Init(const PSUBTASKCFG& pSubTCfg);	//初始化子任务
	void Start();				//开始执行子任务
	void Update(float fTime);	//更新
	DWORD GetState();			//返回子任务的状态（包括准备期，执行期，完成，失败）
	void SetState(DWORD dwState);	//设置状态	
	void UpdateMessage();		//更新上下层收发的信息（上下层通信）
private:
	CAtomLevel* _PopAtomLevel();//弹出一个level 
	void _Finish();				//完成拉
	void _Fail();				//失败了
	
private:
	queue<CAtomLevel*> m_qAtomLevel;	//level队列
	CAtomLevel* m_pCurAtomLevel;		//当前活跃的level
	list<CAtomBase*> m_listSubTaskPeriodAtom;	//子任务周期内存在的原子链表
	PSUBTASKCFG m_pSubTaskCfg;			//子任务的配置
	CGarbagePool<CAtomLevel> m_levelGarbagePool;	//level垃圾回收池
	DWORD m_dwState;		//子任务的当前状态
};
