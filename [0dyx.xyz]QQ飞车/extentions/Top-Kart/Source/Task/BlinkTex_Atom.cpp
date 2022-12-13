#include "Top-KartPCH.h"
#include "BlinkTex_Atom.h"


CBlinkTex_Atom::CBlinkTex_Atom():
m_pScreenTex(0),
m_dwBlinkTime(0),
m_bIsShow(false)
{

}

CBlinkTex_Atom::~CBlinkTex_Atom()
{
	SAFE_DELETE(m_pScreenTex);
}

void CBlinkTex_Atom::Start()
{
	m_pScreenTex = CTScreenTexMgr::Get()->CreateScreenTex(m_dwParam);
	if(m_pScreenTex)
	{
		m_pScreenTex->SetActivity(true);
	}
	m_dwBlinkTime = GetTickCount();

}

void CBlinkTex_Atom::Finish()
{
	_Stop();
}

void CBlinkTex_Atom::Fail()
{
	_Stop();
}

void CBlinkTex_Atom::_Stop()
{
	if(m_pScreenTex)
	{
		m_pScreenTex->SetActivity(false);
	}

}

void CBlinkTex_Atom::Update(float fTime)
{
	if(GetTickCount() - m_dwBlinkTime > m_dwOption)
	{
		m_pScreenTex->SetActivity(m_bIsShow);
		m_dwBlinkTime = GetTickCount();
		m_bIsShow = m_bIsShow ? false : true;
	}
}