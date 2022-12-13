#include "Top-KartPCH.h"
#include "ScreenTextManager.h"
#include "NiRenderer.h"


#define DEFAULT_MAX_CHARACTER 100

ScreenTextManager::ScreenTextManager()
: m_pkScreenTextures(NULL)
, m_unMaxCharacter(DEFAULT_MAX_CHARACTER)
, m_unLines(0)
, m_unWidth(0)
, m_nBasePosX(0)
, m_nBasePosY(0)
{
	m_kNiScreenTextPool.clear();
	m_pkScreenTextures = NiNew NiScreenTextureArray;
}

ScreenTextManager::~ScreenTextManager(void)
{
	Free();
}

void ScreenTextManager::SetMaxCharacter(unsigned int unMaxCharacter)
{
	m_unMaxCharacter = unMaxCharacter;
}

unsigned int ScreenTextManager::SetText(const char * lpText, unsigned int unTextPortID, NiColorA & textColor)
{
	if(unTextPortID < m_kNiScreenTextPool.size())
	{
		NiScreenText * kNiScreenTextPtr = m_kNiScreenTextPool[unTextPortID];
		kNiScreenTextPtr->SetString(lpText);
		kNiScreenTextPtr->SetColor(textColor);		
		kNiScreenTextPtr->SetVisible(true);
	}
	else
	{		
		unTextPortID = m_kNiScreenTextPool.size();
		NiScreenText * kNiScreenTextPtr = NiNew NiScreenText(m_unMaxCharacter, m_pkScreenTextures, textColor);
		kNiScreenTextPtr->SetScrollDown(true);
		kNiScreenTextPtr->SetTextOrigin(32 + m_nBasePosX, 16*m_unLines + m_nBasePosY);
		kNiScreenTextPtr->SetMaxLineLength(m_unMaxCharacter);
		kNiScreenTextPtr->SetVisible(true);
		kNiScreenTextPtr->SetString(lpText);
		m_kNiScreenTextPool.push_back(kNiScreenTextPtr);		
		m_unLines += kNiScreenTextPtr->GetCurRows() + 1;
		if (kNiScreenTextPtr->GetCurCols() > m_unWidth) 
		{
			m_unWidth = kNiScreenTextPtr->GetCurCols();
		}
	}

	return unTextPortID;
}

void ScreenTextManager::SetTextEx(const char * lpText, unsigned int & unTexPortID, NiColorA & textColor, int nPosX, int nPosY)
{
	SetText(lpText, unTexPortID, textColor);
	NiScreenText * kNiScreenTextPtr = m_kNiScreenTextPool[unTexPortID];
	kNiScreenTextPtr->SetTextOrigin(nPosX, nPosY);
}

void ScreenTextManager::Free()
{
	for (size_t i = 0; i < m_kNiScreenTextPool.size(); ++i)
	{
		NiDelete m_kNiScreenTextPool[i];
	}
	m_kNiScreenTextPool.clear();
	NiDelete m_pkScreenTextures;
}
/*
const NiScreenTextureArray * ScreenTextManager::GetNiScreenTexture() const
{
	return m_pkScreenTextures;
}*/

void ScreenTextManager::SetVisible(bool bVisible)
{
	for(size_t i=0; i < m_kNiScreenTextPool.size(); i++)
	{	
		if(m_kNiScreenTextPool[i] != NULL)
		{
			m_kNiScreenTextPool[i]->SetVisible(bVisible);
		}
	}
}

void ScreenTextManager::Draw()
{
	for (unsigned int i = 0; i < m_pkScreenTextures->GetSize(); ++i)
	{		
		m_pkScreenTextures->GetAt(i)->Draw(NiRenderer::GetRenderer());
	}
}

void ScreenTextManager::SetManagerBasePos(int x, int y)
{
	m_nBasePosX = x;
	m_nBasePosY = y;
}

UINT ScreenTextManager::GetManagerWidth()
{
	return m_unWidth * 9;
}

UINT ScreenTextManager::GetManagerHeight()
{
	return (m_unLines + 1) * 13;
}
