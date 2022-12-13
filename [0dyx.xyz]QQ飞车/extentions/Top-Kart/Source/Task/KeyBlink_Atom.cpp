#include "Top-KartPCH.h"
#include "KeyBlink_Atom.h"

CKeyBlink_Atom::CKeyBlink_Atom():
m_pScreenTexUp(0),
m_pScreenTexDown(0),
m_dwBlinkTime(0),
m_bIsShowUP(true)
{

}


CKeyBlink_Atom::~CKeyBlink_Atom()
{
	SAFE_DELETE(m_pScreenTexUp);
	SAFE_DELETE(m_pScreenTexDown);
}

void CKeyBlink_Atom::Start()
{
	CAtomBase::Start();
	if(!m_pScreenTexUp)		//重用时不用再生成
	{
		m_pScreenTexUp = CTScreenTexMgr::Get()->CreateScreenTex(m_dwParam);
	}
	if(!m_pScreenTexDown)
	{
		m_pScreenTexDown = CTScreenTexMgr::Get()->CreateScreenTex(m_dwOption);
	}
	
	if(m_pScreenTexUp && m_pScreenTexDown)
	{
		m_pScreenTexUp->SetActivity(false);
		m_pScreenTexDown->SetActivity(false);
	}	
	m_dwBlinkTime = GetTickCount();

}

void CKeyBlink_Atom::_Stop()
{
	if(m_pScreenTexUp && m_pScreenTexDown)
	{
		m_pScreenTexUp->SetActivity(false);
		m_pScreenTexDown->SetActivity(false);
	}	
}

void CKeyBlink_Atom::Update(float fTime)
{
	CAtomBase::Update(fTime);
	if(GetTickCount() - m_dwBlinkTime > 300)
	{
		if(m_bIsShowUP)
		{
			if(m_pScreenTexUp && m_pScreenTexDown)
			{
				m_pScreenTexUp->SetActivity(true);
				m_pScreenTexDown->SetActivity(false);
			}	
			m_bIsShowUP = false;
		}
		else
		{
			if(m_pScreenTexUp && m_pScreenTexDown)
			{
				m_pScreenTexUp->SetActivity(false);
				m_pScreenTexDown->SetActivity(true);
			}	
			m_bIsShowUP = true;	
		}
		m_dwBlinkTime = GetTickCount();
	}
	
}