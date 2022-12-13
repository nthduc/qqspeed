#include "Top-KartPCH.h"
#include "CDTime_Det.h"
#include "TaskScript.h"

CCDTime_Det::CCDTime_Det():
m_dwStartTime(0),
m_dwLife(0)
{

}

CCDTime_Det::~CCDTime_Det()
{

}

void CCDTime_Det::Start()
{
	CDetectorBase::Start();
	m_dwStartTime = GetTickCount();
}


void CCDTime_Det::Update(float fTime)
{
	CDetectorBase::Update(fTime);
	if(GetTickCount() - m_dwStartTime >= m_dwLife)
	{
		if(CLevelManager::Get())
		{
			CLevelManager::Get()->SetCountDownTime(0);
		}
		_SetState(EDS_Fail);
		return;
	}
	else 
	{
		if(CLevelManager::Get()->GetLinks()->IsFinishRace())
		{
			_SetState(EDS_Success);
			return;
		}	
		if(CLevelManager::Get())
		{
			CLevelManager::Get()->SetCountDownTime( (m_dwLife + m_dwStartTime) - GetTickCount());
		}
	}	
}

bool CCDTime_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		_TReadIntFromScript(&m_dwLife, pVarParam->Element("life"));
		if(CLevelManager::Get())
		{
			CLevelManager::Get()->SetCountDownTime(m_dwLife);
		}
		return true;
	}
	return false;
}