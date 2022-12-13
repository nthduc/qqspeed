#include "Top-KartPCH.h"
#include "TaskSys.h"
#include "TaskNet.h"
#include "AtomProducer.h"
#include "../PowerController.h"
#include "../Platform/GlobalInterface.h"


CTask::CTask():
m_pCurSubTask(0),
m_pTaskCfg(0),
m_dwState(EDS_Prepair)
{
}

CTask::~CTask()
{
	m_pTaskCfg = NULL;
	SAFE_DELETE(m_pCurSubTask);
	PAtomBase_list_It atom_it = m_listPeriodAtom.begin();
	for(; atom_it != m_listPeriodAtom.end(); ++atom_it)
	{
		SAFE_DELETE((*atom_it));
		//m_listPeriodAtom.erase(atom_it);
	}
	m_listPeriodAtom.clear();
	while (!m_qSubTask.empty())
	{
		SAFE_DELETE(m_qSubTask.front());
		m_qSubTask.pop();
	}
}

bool CTask::Init(PTASKCONFIG pTaskCfg)
{
	m_pTaskCfg = pTaskCfg;
	ATOMCFG_LIST_IT atom_it = m_pTaskCfg->listTaskPeriodAtom.begin();
	for(; atom_it != m_pTaskCfg->listTaskPeriodAtom.end(); ++atom_it)
	{
		CAtomBase* pAtom = CAtomProducer::Get()->CreateAtom(&(*atom_it));
		m_listPeriodAtom.push_back(pAtom);
	}

	SUBTASKCFG_LIST_IT it = m_pTaskCfg->listSubTask.begin();
	for(; it != m_pTaskCfg->listSubTask.end(); ++it)
	{
		CSubTask* pSubTask = new CSubTask;
		if(pSubTask && pSubTask->Init(&(*it)))
		{			
			m_qSubTask.push(pSubTask);
		}
		else
		{
			SAFE_DELETE(pSubTask);
			return false;
		}
	}
	_InitSwitch();	
	return true;
}

void CTask::Start()
{
	PAtomBase_list_It atom_it = m_listPeriodAtom.begin();
	for(; atom_it != m_listPeriodAtom.end(); ++atom_it)
	{
		(*atom_it)->Start();
	}
	m_pCurSubTask = _PopSubTask();
	if(m_pCurSubTask)
	{
		m_pCurSubTask->RegisterObserver(this);
		m_pCurSubTask->Start();		
	}
	m_dwState = EDS_Executing;
}

void CTask::Update(float fTime)
{
	if(m_pCurSubTask && m_dwState == EDS_Executing)
	{
		PAtomBase_list_It atom_it = m_listPeriodAtom.begin();
		for(; atom_it != m_listPeriodAtom.end(); ++atom_it)
		{
			(*atom_it)->Update(fTime);
		}
		m_pCurSubTask->Update(fTime);
		//_DetectSubTask();
	}
}

//void CTask::_DetectSubTask()
//{
//	if(m_pCurSubTask->GetState() == EDS_Success)
//	{
//		if(m_qSubTask.empty())
//		{
//			m_dwState = EDS_Success;
//			Finish();
//		}
//		else
//		{
//			m_pCurSubTask->_Finish();
//			SAFE_DELETE(m_pCurSubTask);
//			m_pCurSubTask = _PopSubTask();
//			m_pCurSubTask->Start();
//		}
//	}
//	else if(m_pCurSubTask->GetState() == EDS_Fail)
//	{
//		m_dwState = EDS_Fail;
//		Fail();
//	}
//}


void CTask::Finish()
{
	if(m_dwState == EDS_Success)	//函数已经调过一次了
	{
		return;
	}
	m_dwState = EDS_Success;
	CTaskNet::RequestReportCheckPoint();
	//CKartStatusManager::Get()->OnTriggerRaceFinish(RACE_FINISH_NORMAL, 0);
	CALL(NxPlayMgr::GetHostPlay(), PushPlayerAnimEvent(CAnimEventMgr::EAET_FINISH));
	CALL(NxPlayMgr::GetHostPlay(), UseWinCamera());
	CALL(NxPlayMgr::GetHostPlay(), SetWin(true));
	CALL(NxPlayMgr::GetHostPlay(), SetFinish(true));
	NxSceneMgr::Get()->GetCameraController()->GetForeCam()->PlayAnim();
	NxPlayMgr::GetHostPlay()->GetKart()->SetMovable(false);
//	m_pCurSubTask->UnRegObserver(this);
	m_pCurSubTask->_Finish();
	PAtomBase_list_It atom_it = m_listPeriodAtom.begin();
	for(; atom_it != m_listPeriodAtom.end(); ++atom_it)
	{
		(*atom_it)->Finish();
	}
}

void CTask::Fail()
{
	if(m_dwState == EDS_Fail)	//函数已经调过一次了
	{
		return;
	}
	m_dwState = EDS_Fail;
	//CKartStatusManager::Get()->OnTriggerRaceFinish(RACE_FINISH_TIMEOUT, 0);
	NxPlayMgr::GetHostPlay()->GetKart()->SetMovable(false);
	CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_UNFINISHTEXT,0));
	NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_UNFINISH);
	m_pCurSubTask->_Fail();
	PAtomBase_list_It atom_it = m_listPeriodAtom.begin();
	for(; atom_it != m_listPeriodAtom.end(); ++atom_it)
	{
		(*atom_it)->Fail();
	}
}

DWORD CTask::GetState()
{
	return m_dwState;
}

PTASKCONFIG CTask::GetTaskCfg()
{
	return m_pTaskCfg;
}

void CTask::UpdateMessage()
{
	switch(m_pCurSubTask->GetState())
	{
	case EDS_Success:
		{
			if(m_qSubTask.empty())
			{
				Finish();
			}
			else
			{
				m_pCurSubTask->_Finish();
				m_subTaskGarbagePool.ClearAllGarbage();	//则把以前的subtask清除掉
				m_subTaskGarbagePool.AddGarbage(m_pCurSubTask);		//把当前的subtask扔到垃圾堆中
				m_pCurSubTask = _PopSubTask();	//得到新的subtask	
				m_pCurSubTask->RegisterObserver(this);//任务继续观察新的				
				m_pCurSubTask->Start();
			}
		}
		break;
	case EDS_Fail:
		{
			//m_dwState = EDS_Fail;
			Fail();
			//NotifyObserver();
		}
		break;
	default:
		break;
	}
}


CSubTask* CTask::_PopSubTask()
{
	if(m_qSubTask.empty())
	{
		return NULL;
	}
	CSubTask* pSubTask = m_qSubTask.front();
	m_qSubTask.pop();
	return pSubTask;
}

void CTask::_InitSwitch()
{
	SWITCHTABLE& kSwitchT = m_pTaskCfg->kSwitchTable; 
	
	CALL(CAcceleratePanel::Get(),					SetActivity(!kSwitchT.bAccePlaneOff));
	CALL(CPlayerRankMgr::Get(),						SetActivity(!kSwitchT.bPlayerRankOff));
	CALL(CPowerController::Get()->GetSinglePower(), SetActivity(!kSwitchT.bSinglePowerCtrlOff));
	CALL(CPowerController::Get()->GetTeamPower(),	SetActivity(!kSwitchT.bTeamPowerCtrlOff));
	CALL(CEagleEyeMap::Get(),						Show(!kSwitchT.bEagleMapOff));
	CKartStatusManager::EnableReset(!kSwitchT.bResetOff);	
}
