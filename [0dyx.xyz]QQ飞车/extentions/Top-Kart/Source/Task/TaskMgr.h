////////////////////////////////////////////////
//     FileName:    TaskMgr.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: 任务控制类
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "TaskSys.h"
#include "singlet.h"

class CTaskMgr : public CSingleT<CTaskMgr>
{
public:
	CTaskMgr();
	~CTaskMgr();
	//初始化
	void Init();

	//进入任务dwID
	bool EnterTask(CNotifySingleGameBeginPara* pPara);

	//退出任务
	void LeaveTask();

	//更新
	void Update(float fTime);

	//查找某任务的配置
	PTASKCONFIG FindTaskCfg(DWORD dwID);

	//返回当前游戏
	CTask* GetCurTask(); 

	//增加任务信息到任务链表（包括可玩，不可玩，完成的）
	void AddTaskInfo(TASKINFO tagTask);

	//返回id为dwid的任务的信息
	PTASKINFO GetTaskInfo(DWORD dwID);	

	//增加任务包到包链表（可玩的）
	void AddTaskPack(DWORD dwPackID);

	//设置当前的任务包
	void SetCurTaskPack(DWORD dwPackID);

	//返回当前任务包的信息
	PTASKPACKCFG GetCurTaskPack(); 	

	//返回lua state
	CScriptState* GetLuaState();

	//判断是否有新手任务
	bool IsFinishNewHandTask(DWORD dwID);


private:
	//加载任务配置
	void _LoadConfig();

	//更新任务信息
	void _UpdateTaskInfo();

	//返回id为dwID的任务状态
	DWORD _GetTaskAccessState(DWORD dwID);

	//返回id为dwID的任务状态
	void  _SetTaskAccessState(DWORD dwID, DWORD dwState);
	

private:
	CScriptState*		m_pLuaState;
	TASKINFO_LIST		m_listTaskInfo;		//任务此时的状态信息
	PTASKPACKCFG		m_pCurTPackCfg;		//当前任务包的信息
	TASKPACKCFG_LIST	m_listTPackCfg;		//可玩任务包信息
	TASKCONFIG_LIST		m_listTaskCfg;		//所有任务的配置
	CTask*				m_pCurTask;			//当前任务
};