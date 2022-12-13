#include "Top-KartPCH.h"
#include "ScreenFontMgr.h"
#include "display.h"
#include "../Platform/GlobalInterface.h"
#include "TaskScript.h"


CScreenFont::CScreenFont():
m_fX(0),
m_fY(0),
m_fFontSize(16),
m_dwColor(0xffffffff)
{

}

CScreenFont::~CScreenFont()
{

}

void CScreenFont::Update(float fTime)
{

}

void CScreenFont::Draw(NiRendererPtr spRenderer)
{
	GlobalInterface::m_pDisplay->ClearScreen(0);
	GlobalInterface::m_pDisplay->DrawText(m_fX, m_fY, m_strDesc.c_str(), m_dwColor, m_fFontSize, 0);
	//GlobalInterface::m_pDisplay->DrawTextEx(m_fX, m_fY, 50, 50, m_strDesc.c_str(), m_fFontSize, 0, m_dwColor);
	GlobalInterface::m_pDisplay->UpdateScreen();
}

void CScreenFont::SetConfig(const SCREENFONTCFG& kFontCfg)
{
	m_fX = kFontCfg.fX;
	m_fY = kFontCfg.fY;
	if(kFontCfg.fFontSize)
	{
		m_fFontSize = kFontCfg.fFontSize;
	}	
	if(kFontCfg.dwColor)
	{
		m_dwColor = kFontCfg.dwColor;
	}
	m_strDesc = kFontCfg.strDesc;	
}


CScreenFontMgr::CScreenFontMgr()
{

}

CScreenFontMgr::~CScreenFontMgr()
{

}

bool CScreenFontMgr::Init()
{
	if(m_lsFont.DoFile("TaskConfig/screenfont.lua") == 0)
	{
		CVarArray vtRoot("screenfont");
		vtRoot.GetFromScript(&m_lsFont);
		for (int i = 0; i < vtRoot.GetNum(); ++i)
		{
			CVarTable* pSubVar = (CVarTable*)vtRoot.Element(i);
			SCREENFONTCFG kFontCfg;
			_TReadIntFromScript(&kFontCfg.dwID, pSubVar->Element("id"));
			_TReadFloatFromScript(&kFontCfg.fX, pSubVar->Element("x"));
			_TReadFloatFromScript(&kFontCfg.fY, pSubVar->Element("y"));
			_TReadFloatFromScript(&kFontCfg.fFontSize, pSubVar->Element("fontsize"));
			_TReadIntFromScript(&kFontCfg.dwColor, pSubVar->Element("color"));
			_TReadStringFromScript(kFontCfg.strDesc, pSubVar->Element("desc"));
			m_listFontCfg.push_back(kFontCfg);
		}
		return true;
	}
	return false;
}

CScreenFont* CScreenFontMgr::CreateScreenFont(DWORD dwID)
{
	SCRFONTCFG_LIST_IT it = m_listFontCfg.begin();
	while(it != m_listFontCfg.end())
	{
		if(it->dwID == dwID)
		{
			CScreenFont* pScreenFont = new CScreenFont;
			if(pScreenFont)
			{
				pScreenFont->SetConfig(*it);
			}
			return pScreenFont;
		}
		++it;
	}
	return NULL;
}