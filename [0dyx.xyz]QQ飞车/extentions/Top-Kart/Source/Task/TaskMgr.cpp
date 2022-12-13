#include "Top-KartPCH.h"
#include "TaskMgr.h"
#include "TaskScript.h"
#include "../levelmanager.h"
#include "AtomProducer.h"
#include "DetectorProducer.h"
#include "TaskNet.h"


CTaskMgr::CTaskMgr():
m_pCurTask(0),
m_pCurTPackCfg(0),
m_pLuaState(0)
{
	CAtomProducer::Create();
	CDetectorProducer::Create();
	CDetectorProducer::Get()->Init();
}

CTaskMgr::~CTaskMgr()
{
	CAtomProducer::Destroy();
	CDetectorProducer::Destroy();
	m_pCurTPackCfg = NULL;
	SAFE_DELETE(m_pCurTask);
	SAFE_DELETE(m_pLuaState);
	m_listTaskCfg.clear();
	m_listTaskInfo.clear();
	m_listTPackCfg.clear();
}

void CTaskMgr::Init()
{
	_LoadConfig();
}

bool CTaskMgr::EnterTask(CNotifySingleGameBeginPara* pPara)
{
	if(_GetTaskAccessState(pPara->m_iTaskID) == ETAS_Available)
	{
		//查找任务配置文件配置
		PTASKCONFIG pTaskCfg = FindTaskCfg(pPara->m_iTaskID);
		m_pCurTask = new CTask;
		if(m_pCurTask)
		{
			CLevelManager::Create();
			CALL(CKartScene::Get(), Unload());
			CNotifyGameBeginPara* pGameBData = new CNotifyGameBeginPara;
			pGameBData->m_nMapID = pPara->m_iMapID;
			pGameBData->m_nLap = pPara->m_shTotalMapRound;
			pGameBData->m_unGameID = pPara->m_uiGameID;
			CLevelManager::Get()->Enter(pGameBData);
			if(pTaskCfg && m_pCurTask->Init(pTaskCfg))
			{
				//m_pCurTask->RegisterObserver(this);
				/*CTaskNet::RequestPrepareReady(NxPlayMgr::GetHostPlay()->GetPlayUin());*/
				//m_pCurTask->Start();
				return true;	
			}			
			SAFE_DELETE(m_pCurTask);
		}
	}
	return false;
}

void CTaskMgr::Update(float fTime)
{
	if(m_pCurTask)
	{
		m_pCurTask->Update(fTime);
		//if(m_pCurTask->GetState() == EDS_Success)
		//{
		//	m_pCurTask->Finish();
		//	//LeaveTask();
		//}
		//else if(m_pCurTask->GetState() == EDS_Fail)
		//{
		//	m_pCurTask->Fail();
		//	//LeaveTask();
		//}
	}
	
}

CTask* CTaskMgr::GetCurTask()
{
	return m_pCurTask;
}

void CTaskMgr::LeaveTask()
{
//	m_pCurTask->UnRegObserver(this);
	_UpdateTaskInfo();
	CALL(CKartScene::Get(), Reload());
	CLevelManager::Destroy();
	SAFE_DELETE(m_pCurTask);
}

void CTaskMgr::AddTaskInfo(TASKINFO tagTask)
{
	TASKINFO_LIST_IT it = m_listTaskInfo.begin();
	for(; it != m_listTaskInfo.end(); ++it)
	{
		if(it->dwID == tagTask.dwID)
		{
			return;
		}
	}
	m_listTaskInfo.push_back(tagTask);
}

PTASKINFO CTaskMgr::GetTaskInfo(DWORD dwID)
{
	TASKINFO_LIST_IT it = m_listTaskInfo.begin();
	for(; it != m_listTaskInfo.end(); ++it)
	{
		if(it->dwID == dwID)
		{
			return &(*it);
		}
	}
	return NULL;
}

void CTaskMgr::AddTaskPack(DWORD dwPackID)
{
	CVarArray varrPack("TaskPack");
	varrPack.GetFromScript(m_pLuaState);
	for(int i = 0; i < varrPack.GetNum(); ++i)
	{
		CVarTable* pSubVar = (CVarTable*) varrPack.Element(i);
		if(pSubVar && pSubVar->Element("id")->Integer() == dwPackID)
		{
			TASKPACKCFG tagPack;
			tagPack.dwPackID = dwPackID;
			CVarArray* pVarTask = (CVarArray*)pSubVar->Element("TaskList");
			for(int k = 0; k < pVarTask->GetNum(); ++k)
			{
				tagPack.listTask.push_back(pVarTask->Element(k)->Integer());
			}
			m_listTPackCfg.push_back(tagPack);
		}
	}
}

PTASKPACKCFG CTaskMgr::GetCurTaskPack()
{
	return m_pCurTPackCfg;
}

//PTASKCONFIG CTaskMgr::GetTaskCfg(DWORD dwID)
//{
//	TASKCONFIG_LIST_IT it = m_listTaskCfg.begin();
//	for (; it != m_listTaskCfg.end(); ++it)
//	{
//		if(it->dwID == dwID)
//		{
//			return &(*it);
//		}
//	}
//	return NULL;
//}


CScriptState* CTaskMgr::GetLuaState()
{
	return m_pLuaState;
}

void CTaskMgr::_LoadConfig()
{
	m_pLuaState = new CScriptState;
	if(m_pLuaState->DoFile("taskconfig/task.lua") == 0)
	{
		CVarArray varrTask("Task");
		varrTask.GetFromScript(m_pLuaState);
		m_listTaskCfg.clear();
		for(int i = 0; i < varrTask.GetNum(); ++i)
		{
			CVarTable* pSubVar = (CVarTable*)varrTask.Element(i);
			if(pSubVar && pSubVar->GetType() == VAR_TABLE)
			{
				TASKCONFIG tagTask;
				_TReadTaskFromScript(pSubVar, tagTask);
				m_listTaskCfg.push_back(tagTask);
			}
		}
	}
}

PTASKCONFIG CTaskMgr::FindTaskCfg(DWORD dwID)
{
	TASKCONFIG_LIST_IT cfg_it = m_listTaskCfg.begin();
	for (; cfg_it != m_listTaskCfg.end(); ++cfg_it)
	{
		if(cfg_it->dwID == dwID)
		{
			return &(*cfg_it);
		}
	}
	return NULL;
}

void CTaskMgr::_UpdateTaskInfo()
{
	if(m_pCurTask->GetState() == EDS_Success)//还要更新其他相关任务的信息（可不可玩）
	{
		//_SetTaskState(m_pCurTask->GetTaskCfg()->dwID, ETST_Finish);
		PTASKINFO pTaskInfo = GetTaskInfo(m_pCurTask->GetTaskCfg()->dwID);
		pTaskInfo->dwAchieveState = ETST_Finish;
		//暂时措施，等调整任务包时去掉
		if(m_pCurTask->GetTaskCfg()->dwID == 1)
		{
			TASKINFO tagInfo;
			tagInfo.dwID = 2;
			tagInfo.dwAccessState = ETAS_Available;
			AddTaskInfo(tagInfo);
		}
	}
}


DWORD CTaskMgr::_GetTaskAccessState(DWORD dwID)
{
	TASKINFO_LIST_IT info_it = m_listTaskInfo.begin();
	for(; info_it != m_listTaskInfo.end(); ++info_it)
	{
		if(info_it->dwID == dwID)
		{
			return info_it->dwAccessState;
		}
	}
	return ETAS_Unavailable; 
}

void CTaskMgr::_SetTaskAccessState(DWORD dwID, DWORD dwState)
{
	TASKINFO_LIST_IT it = m_listTaskInfo.begin();
	for(; it != m_listTaskInfo.end(); ++it)
	{
		if(it->dwID == dwID)
		{
			it->dwAccessState = dwState;
		}
	}
}

bool CTaskMgr::IsFinishNewHandTask(DWORD dwID)
{
	TASKINFO_LIST_IT it = m_listTaskInfo.begin();
	for(; it != m_listTaskInfo.end(); ++it)
	{
		if(it->dwID == dwID)
		{
			if(it->dwAchieveState == ETST_Finish)
			{
				return true;
			}	
			return false;
		}			
	}
	return true;
}

