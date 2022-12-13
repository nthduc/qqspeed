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
		CAtomBase* pAtom = CAtomProducer::Get()->CreateAtom(&(*atom_it));	//���������������ڶ����ڵ�ԭ��
		if(!pAtom)
		{
			return false;
		}
		m_listSubTaskPeriodAtom.push_back(pAtom);
	}
	ATOMLEVELCFG_LIST_IT it = m_pSubTaskCfg->listAtomLevel.begin();
	for (; it != m_pSubTaskCfg->listAtomLevel.end(); ++it)	//����level����
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
	m_dwState = dwState;	//����״̬
	NotifyObserver();		//֪ͨ�ϲ�۲���
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

//������Ϣ
void CSubTask::UpdateMessage()
{
	switch(m_pCurAtomLevel->GetState())
	{
	case EDS_Success:
		{
			if(m_qAtomLevel.empty())	//�����������
			{
				if(m_pSubTaskCfg->dwValidation)
				{
					DWORD dwUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
					CTaskNet::RequestReportCurrentOpt(dwUin, m_pSubTaskCfg->dwValidation);
				}
				m_dwState = EDS_Success;
				NotifyObserver();	//֪ͨ��һ��Ĺ۲���
			}
			else
			{
				if(m_pSubTaskCfg->dwProcessType == EPT_NeverBack)	//������������Ϊ���ɴ�ͷ�����������ǰ��level�����
				{
					m_levelGarbagePool.ClearAllGarbage();
				}
				m_pCurAtomLevel->_Finish();
				m_levelGarbagePool.AddGarbage(m_pCurAtomLevel);		//�ѵ�ǰ��level�ӵ���������
				m_pCurAtomLevel = _PopAtomLevel();		//�õ��µ�level		
				m_pCurAtomLevel->RegisterObserver(this);//����������۲��µ�level
				m_pCurAtomLevel->Start();
			}		
		}
		break;
	case EDS_Fail:
		{
			if(m_pSubTaskCfg->dwProcessType == EPT_Circle)	//��ǰ��������Ϊ��ѭ��ִ�У����ͷ��ʼ
			{
				m_pCurAtomLevel->_Fail();
				m_levelGarbagePool.AddGarbage(m_pCurAtomLevel);		//�ѵ�ǰ��level�ӵ���������
				list<CAtomLevel*> levelList;
				m_levelGarbagePool.ReuseAllGarbage(levelList);	//���������˼����ǰ��level������
				while(!m_qAtomLevel.empty())
				{
					levelList.push_back(m_qAtomLevel.front());	//�������ú����е�level
					m_qAtomLevel.pop();
				}
				list<CAtomLevel*>::iterator it = levelList.begin();
				while(it != levelList.end())	//��������level����
				{
					m_qAtomLevel.push(*it);
					++it;
				}
				levelList.clear();
				m_pCurAtomLevel = _PopAtomLevel();	//���¿�ʼ
				m_pCurAtomLevel->RegisterObserver(this);//����������۲��µ�level
				m_pCurAtomLevel->Start();
			}
			else		//����ѭ��ִ�е�������
			{
				m_dwState = EDS_Fail;
				NotifyObserver();		//�ϱ��ϲ�ʧ����
			}			
		}
		break;
	}
}