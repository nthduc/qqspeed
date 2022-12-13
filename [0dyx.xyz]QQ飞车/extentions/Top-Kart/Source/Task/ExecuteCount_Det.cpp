#include "Top-KartPCH.h"
#include "ExecuteCount_Det.h"


CExecuteCout_Det::CExecuteCout_Det():
m_dwCurCount(0),
m_dwLife(0)
{

}

CExecuteCout_Det::~CExecuteCout_Det()
{

}

void CExecuteCout_Det::Start()
{
	CDetectorBase::Start();
	m_dwCurCount = 0;
}

void CExecuteCout_Det::Update(float fTime)
{
	CDetectorBase::Update(fTime);
	++m_dwCurCount;
	if(m_dwCurCount > m_dwLife)
	{
		_SetState(EDS_Success);
	}
}

bool CExecuteCout_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		_TReadIntFromScript(&m_dwLife, pVarParam->Element("life"));
		return true;
	}
	return false;	
}
