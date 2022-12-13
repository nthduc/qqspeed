#include "Top-KartPCH.h"
#include "BNPress_Det.h"
#include "TaskScript.h"
#include "../InputManager.h"

CBNPress_Det::CBNPress_Det():
m_dwOp(0),
m_spKeyboard(0)
{
	m_listKey.clear();
}

CBNPress_Det::~CBNPress_Det()
{
	m_listKey.clear();
}

void CBNPress_Det::Start()
{
	CDetectorBase::Start();
	m_spKeyboard = InputManager::Get()->GetInputSystem()->GetKeyboard();
}

void CBNPress_Det::Succeed()
{
	CDetectorBase::Succeed();
}

void CBNPress_Det::Fail()
{
	CDetectorBase::Fail();
}

void CBNPress_Det::Update(float fTime)
{
	CDetectorBase::Update(fTime);
	if(m_dwOp)
	{
		list<NiInputKeyboard::KeyCode>::iterator it = m_listKey.begin();
		for(; it != m_listKey.end(); ++it)
		{
			if(!m_spKeyboard->KeyIsDown(*it))
			{
				return;
			}
		}
		_SetState(EDS_Success);
	}
	else
	{
		list<NiInputKeyboard::KeyCode>::iterator it = m_listKey.begin();
		for(; it != m_listKey.end(); ++it)
		{
			if(m_spKeyboard->KeyIsDown(*it))
			{
				_SetState(EDS_Success);
				return;
			}
		}
	}
}

bool CBNPress_Det::LoadConfig(CVarTable* pTable)
{
	CVarTable* pVarParam = (CVarTable*)pTable->Element("params");
	if(pVarParam)
	{
		_TReadIntFromScript(&m_dwOp, pVarParam->Element("op"));
		CVarArray* pSubVar = (CVarArray*)pVarParam->Element("keys");
		for(int i = 0; i < pSubVar->GetNum(); ++i)
		{
			DWORD dwKey;
			_TReadIntFromScript(&dwKey, pSubVar->Element(i));
			m_listKey.push_back((NiInputKeyboard::KeyCode)dwKey);
		}
	}	
	return true;
}