#include "Top-KartPCH.h"
#include "AtomLevel.h"
#include "DetectorProducer.h"
#include "AtomProducer.h"

CAtomLevel::CAtomLevel():
m_pAtomLevelCfg(0),
m_pDetector(0),
m_dwState(EDS_Prepair)
{

}

CAtomLevel::~CAtomLevel()
{
//	m_pDetector->UnRegObserver(this);
	SAFE_DELETE(m_pDetector);
	list<CAtomBase*>::iterator it = m_listAtom.begin();
	for (; it != m_listAtom.end(); ++it)
	{
		SAFE_DELETE((*it));
	}
	m_listAtom.clear();
}

bool CAtomLevel::Init(const PATOMLEVELCFG& pAtomLCfg)
{
	m_pAtomLevelCfg = pAtomLCfg;
	m_pDetector = CDetectorProducer::Get()->CreateDetector(m_pAtomLevelCfg->dwDetectorID);
	if(!m_pDetector)
	{
		return false;
	}
//	m_pDetector->RegisterObserver(this);
	ATOMCFG_LIST_IT it = m_pAtomLevelCfg->listAtom.begin();
	for(; it != m_pAtomLevelCfg->listAtom.end(); ++it)
	{
		CAtomBase* pAtom = CAtomProducer::Get()->CreateAtom(&(*it));
		if(!pAtom)
		{
			return false;
		}
		m_listAtom.push_back(pAtom);
	}
	return true;
}

void CAtomLevel::Start()
{
	if(m_pDetector)
	{
		m_dwState = EDS_Executing;
		m_pDetector->RegisterObserver(this);
		m_pDetector->Start();
		list<CAtomBase*>::iterator it = m_listAtom.begin();
		for(; it != m_listAtom.end(); ++it)
		{
			(*it)->Start();
		}
	}
}

void CAtomLevel::_Finish()
{
	if(m_pDetector)
	{
		m_pDetector->Succeed();
		_AllAtomFinish();
	}
}

void CAtomLevel::_Fail()
{
	if(m_pDetector)
	{
		m_pDetector->Fail();
		_AllAtomFail();
	}
}

void CAtomLevel::_AllAtomFinish()
{
	list<CAtomBase*>::iterator it = m_listAtom.begin();
	for(; it != m_listAtom.end(); ++it)
	{
		(*it)->Finish();
	}
}

void CAtomLevel::_AllAtomFail()
{
	list<CAtomBase*>::iterator it = m_listAtom.begin();
	for(; it != m_listAtom.end(); ++it)
	{
		(*it)->Fail();
	}
}

void CAtomLevel::Update(float fTime)
{
	if(m_pDetector && m_dwState == EDS_Executing)
	{
		m_pDetector->Update(fTime);
		if(m_dwState == EDS_Executing)			//detetor可能已经检测到不再执行了，原子们也不执行
		{
			list<CAtomBase*>::iterator it = m_listAtom.begin();
			for(; it != m_listAtom.end(); ++it)
			{
				(*it)->Update(fTime);
			}
		}
		
	}
}

void CAtomLevel::SetState(DWORD dwState)
{
	m_dwState = dwState;
}

DWORD CAtomLevel::GetState()
{
	return m_dwState;
}

void CAtomLevel::UpdateMessage()
{
	switch(m_pDetector->GetState())
	{
	case EDS_Success:
		{
			m_dwState = EDS_Success;
		}
		break;
	case EDS_Fail:
		{
			m_dwState = EDS_Fail;
		}
		break;
	default:
		break;
	}
	NotifyObserver();
}