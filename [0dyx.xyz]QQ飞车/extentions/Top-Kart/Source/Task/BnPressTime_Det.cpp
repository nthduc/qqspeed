#include "Top-KartPCH.h"
#include "BnPressTime_Det.h"
#include "TaskScript.h"
#include "../InputManager.h"

CBnPressTime_Det::CBnPressTime_Det():
m_dwStartTime(0),
m_dwLife(0),
m_bFirstPress(true)
{
	m_listKey.clear();
}

CBnPressTime_Det::~CBnPressTime_Det()
{
	m_listKey.clear();
}

void CBnPressTime_Det::Start()
{
	CDetectorBase::Start();
}

void CBnPressTime_Det::Succeed()
{
	CDetectorBase::Succeed();
}

void CBnPressTime_Det::Fail()
{
	CDetectorBase::Fail();
}

void CBnPressTime_Det::Update(float fTime)
{
	CDetectorBase::Update(fTime);
	list<KEYCODE>::iterator it = m_listKey.begin();
	for(; it != m_listKey.end(); ++it)
	{
		if(!InputManager::keys[*it])
		{
			m_bFirstPress = true;
			return;
		}
	}
	if(m_bFirstPress)
	{
		m_dwStartTime = GetTickCount();
		m_bFirstPress = false;
	}
	else if(GetTickCount() - m_dwStartTime > m_dwLife)
	{
		_SetState(EDS_Success);
		m_bFirstPress = true;
	}
}

bool CBnPressTime_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		_TReadIntFromScript(&m_dwLife, pVarParam->Element("life"));
		CVarArray* pSubVar = (CVarArray*)pVarParam->Element("keys");
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			DWORD dwKey;
			_TReadIntFromScript(&dwKey, pSubVar->Element(i));
			m_listKey.push_back((KEYCODE)dwKey);
		}
	}	
	return true;
}