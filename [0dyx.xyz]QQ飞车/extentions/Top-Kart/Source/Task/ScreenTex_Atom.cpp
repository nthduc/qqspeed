#include "Top-KartPCH.h"
#include "ScreenTex_Atom.h"


CScreenTex_Atom::CScreenTex_Atom():
m_pScreenTex(0)
{
	
}

CScreenTex_Atom::~CScreenTex_Atom()
{
	SAFE_DELETE(m_pScreenTex);
}

void CScreenTex_Atom::Start()
{
	m_pScreenTex = CTScreenTexMgr::Get()->CreateScreenTex(m_dwParam);
	if(m_pScreenTex)
	{
		m_pScreenTex->SetActivity(true);
	}
	
}

void CScreenTex_Atom::Finish()
{
	_Stop();
}

void CScreenTex_Atom::Fail()
{
	_Stop();
}

void CScreenTex_Atom::_Stop()
{
	if(m_pScreenTex)
	{
		m_pScreenTex->SetActivity(false);
	}	
}

void CScreenTex_Atom::Update(float fTime)
{
}
