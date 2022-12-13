#include "Top-KartPCH.h"
#include "SubTask.h"
#include "TaskNet.h"
#include "AtomProducer.h"

CSubTask::CSubTask():m_pCurAtomLevel(0),
m_pSubTaskCfg(0),
m_dwState(EDS_Prepair)
{
}

CSubTask::~CSubTask()
{
	m_pSubTaskCfg = NULL;
	SAFE_DELETE(m_pCurAtomLevel);
	PAtomBase_list_It atom_it = m_listSubTaskPeriodAtom.begin();
	for(; atom_it != m_listSubTaskPeriodAtom.end(); ++atom_it)
	{
		SAFE_DELETE((*atom_it));
	}
	m_listSubTaskPeriodAtom.clear();
	while(!m_qAtomLevel.empty())
	{
		SAFE_DELETE(m_qAtomLevel.front());
		m_qAtomLevel.pop();
	}
}

bool CSubTask::Init(const PSUBTASKCFG& pSubTCfg)
{
	m_pSubTaskCfg = pSubTCfg;
	ATOMCFG_LIST_IT atom_it = m_pSubTaskCfg->listSubTaskPeriodAtom.begin();
	for(; atom_it != m_pSubTaskCfg->listSubTaskPeriodAtom.end(); ++atom_it)
	{
		CAtomBase* pAtom = CAtomProducer::Get()->CreateAtom(&(*atom_it));	//生成子任务周期内都存在的原子
		if(!pAtom)
		{
			return false;
		}
		m_listSubTaskPeriodAtom.push_back(pAtom);
	}
	ATOMLEVELCFG_LIST_IT it = m_pSubTaskCfg->listAtomLevel.begin();
	for (; it != m_pSubTaskCfg->listAtomLevel.end(); ++it)	//生成level链表
	{
		CAtomLevel* pAtomLevel = new CAtomLevel;
		if(pAtomLevel && pAtomLevel->Init(&(*it)))
		{
			m_qAtomLevel.push(pAtomLevel);		
		}
		else
		{
			SAFE_DELETE(pAtomLevel);
			return false;			
		}		
	}
	return true;
}

void CSubTask::Start()
{
	m_dwState = EDS_Executing;
	PAtomBase_list_It atom_it = m_listSubTaskPeriodAtom.begin();
	for(; atom_it != m_listSubTaskPeriodAtom.end(); ++atom_it)
	{
		(*atom_it)->Start();
	}
	m_pCurAtomLevel = _PopAtomLevel();
	m_pCurAtomLevel->RegisterObserver(this);
	m_pCurAtomLevel->Start();
	
}


void CSubTask::_Finish()
{
	m_pCurAtomLevel->_Finish();	
	PAtomBase_list_It atom_it = m_listSubTaskPeriodAtom.begin();
	for(; atom_it != m_listSubTaskPeriodAtom.end(); ++atom_it)
	{
		(*atom_it)->Finish();
	}
}

void CSubTask::_Fail()
{
//	m_pCurAtomLevel->UnRegObserver(this);
	m_pCurAtomLevel->_Fail();
	PAtomBase_list_It atom_it = m_listSubTaskPeriodAtom.begin();
	for(; atom_it != m_listSubTaskPeriodAtom.end(); ++atom_it)
	{
		(*atom_it)->Fail();
	}
}

void CSubTask::Update(float fTime)
{
	if(m_pCurAtomLevel && m_dwState == EDS_Executing)
	{
		PAtomBase_list_It atom_it = m_listSubTaskPeriodAtom.begin();
		for(; atom_it != m_listSubTaskPeriodAtom.end(); ++atom_it)
		{
			(*atom_it)->Update(fTime);
		}
		m_pCurAtomLevel->Update(fTime);
		//_DetectLevel();
	}
}

DWORD CSubTask::GetState()
{
	return m_dwState;
}

void CSubTask::SetState(DWORD dwState)
{
	m_dwState = dwState;	//设置状态
	NotifyObserver();		//通知上层观察者
}

//void CSubTask::_DetectLevel()
//{
//	if(m_pCurAtomLevel->GetState() == EDS_Success)
//	{
//		if(m_qAtomLevel.empty())
//		{
//			m_dwState = EDS_Success;
//			if(m_pSubTaskCfg->dwValidation)
//			{
//				DWORD dwUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
//				CTaskNet::RequestReportCurrentOpt(dwUin, m_pSubTaskCfg->dwValidation);
//			}
//		}
//		else
//		{
//			m_pCurAtomLevel->_Finish();
//			SAFE_DELETE(m_pCurAtomLevel);
//			m_pCurAtomLevel = _PopAtomLevel();
//			m_pCurAtomLevel->Start();
//		}		
//	}
//	else if(m_pCurAtomLevel->GetState() == EDS_Fail)
//	{	
//		m_pCurAtomLevel->_Fail();
//		m_dwState = EDS_Fail;
//	}
//}

CAtomLevel* CSubTask::_PopAtomLevel()
{
	if(m_qAtomLevel.empty())
	{
		return NULL;
	}
	CAtomLevel* pAtomLevel = m_qAtomLevel.front();
	m_qAtomLevel.pop();
	return pAtomLevel;
}

//更新信息
void CSubTask::UpdateMessage()
{
	switch(m_pCurAtomLevel->GetState())
	{
	case EDS_Success:
		{
			if(m_qAtomLevel.empty())	//子任务完成了
			{
				if(m_pSubTaskCfg->dwValidation)
				{
					DWORD dwUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
					CTaskNet::RequestReportCurrentOpt(dwUin, m_pSubTaskCfg->dwValidation);
				}
				m_dwState = EDS_Success;
				NotifyObserver();	//通知上一层的观察者
			}
			else
			{
				if(m_pSubTaskCfg->dwProcessType == EPT_NeverBack)	//若子任务类型为不可从头再来，则把以前的level清除掉
				{
					m_levelGarbagePool.ClearAllGarbage();
				}
				m_pCurAtomLevel->_Finish();
				m_levelGarbagePool.AddGarbage(m_pCurAtomLevel);		//把当前的level扔到垃圾堆中
				m_pCurAtomLevel = _PopAtomLevel();		//得到新的level		
				m_pCurAtomLevel->RegisterObserver(this);//子任务继续观察新的level
				m_pCurAtomLevel->Start();
			}		
		}
		break;
	case EDS_Fail:
		{
			if(m_pSubTaskCfg->dwProcessType == EPT_Circle)	//当前的子任务为可循环执行，则从头开始
			{
				m_pCurAtomLevel->_Fail();
				m_levelGarbagePool.AddGarbage(m_pCurAtomLevel);		//把当前的level扔到垃圾堆中
				list<CAtomLevel*> levelList;
				m_levelGarbagePool.ReuseAllGarbage(levelList);	//从垃圾堆了捡回以前的level，命大啊
				while(!m_qAtomLevel.empty())
				{
					levelList.push_back(m_qAtomLevel.front());	//连接重用和现有的level
					m_qAtomLevel.pop();
				}
				list<CAtomLevel*>::iterator it = levelList.begin();
				while(it != levelList.end())	//重新生成level队列
				{
					m_qAtomLevel.push(*it);
					++it;
				}
				levelList.clear();
				m_pCurAtomLevel = _PopAtomLevel();	//重新开始
				m_pCurAtomLevel->RegisterObserver(this);//子任务继续观察新的level
				m_pCurAtomLevel->Start();
			}
			else		//不是循环执行的子任务
			{
				m_dwState = EDS_Fail;
				NotifyObserver();		//上报上层失败了
			}			
		}
		break;
	}
}