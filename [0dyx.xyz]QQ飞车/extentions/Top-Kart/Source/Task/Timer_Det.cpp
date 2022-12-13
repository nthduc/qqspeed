#include "Top-KartPCH.h"
#include "Timer_Det.h"
#include "TaskScript.h"

CTimer_Det::CTimer_Det():
m_dwStartTime(0),
m_dwLife(0)
{

}

CTimer_Det::~CTimer_Det()
{

}

void CTimer_Det::Start()
{
	CDetectorBase::Start();
	m_dwStartTime = GetTickCount();
}

void CTimer_Det::Update(float fTime)
{
	CDetectorBase::Update(fTime);
	if(GetTickCount() - m_dwStartTime > m_dwLife)
	{
		_SetState(EDS_Success);
	}
}

bool CTimer_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		_TReadIntFromScript(&m_dwLife, pVarParam->Element("life"));
		return true;
	}
	return false;	
}