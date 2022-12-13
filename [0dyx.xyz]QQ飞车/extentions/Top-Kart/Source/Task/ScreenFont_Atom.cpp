#include "Top-KartPCH.h"
#include "ScreenFont_Atom.h"

CScreenFont_Atom::CScreenFont_Atom():
m_pFont(0)
{

}

CScreenFont_Atom::~CScreenFont_Atom()
{
	SAFE_DELETE(m_pFont);
}

void CScreenFont_Atom::Start()
{
	m_pFont = CScreenFontMgr::Get()->CreateScreenFont(m_dwParam);
	if(m_pFont)
	{
		m_pFont->SetActivity(true);
	}
}

void CScreenFont_Atom::_Stop()
{
	if(m_pFont)
	{
		m_pFont->SetActivity(false);
	}
}

