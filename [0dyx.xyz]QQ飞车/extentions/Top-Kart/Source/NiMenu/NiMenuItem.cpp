// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#include "Top-KartPCH.h"

#include "NiMenuItem.h"
#include "NiScreenText.h"

//---------------------------------------------------------------------------
NiMenuItem::NiMenuItem(int iIdentifier, bool bSelectable,
    int iCharacterIndent, void* pvData, const char* pcText, NiColorA& kColor,
    NiScreenElementsArray* pkScreenElements,
    NiScreenTextureArray* pkScreenTextures
    )
{
    m_pkScreenElements = NULL;
    m_pkScreenTextures = NULL;
    m_pkScreenText = NULL;

    Initialize(iIdentifier, bSelectable, iCharacterIndent, pvData, pcText, 
        kColor, pkScreenElements, pkScreenTextures);
}
//---------------------------------------------------------------------------
NiMenuItem::NiMenuItem()
{
    // Default Constructor Do not use
    m_pkScreenElements = NULL;
    m_pkScreenTextures = NULL;
    m_pkScreenText = NULL;
}
//---------------------------------------------------------------------------
NiMenuItem::~NiMenuItem()
{
    NiDelete m_pkScreenText;
}
//---------------------------------------------------------------------------
void NiMenuItem::Initialize(int iIdentifier, bool bSelectable, 
    int iCharacterIndent, void* pvData, const char* pcText, NiColorA& kColor, 
    NiScreenElementsArray* pkScreenElements,
    NiScreenTextureArray* pkScreenTextures)
{
    assert(pcText != NULL);

    m_iIdentifier = iIdentifier;
    m_bSelectable = bSelectable;
    m_iCharacterIndent = iCharacterIndent;
    m_pvData = pvData;

    if(m_pkScreenText == NULL)
    m_pkScreenText = NiNew NiScreenText(256, pkScreenTextures, kColor);
    SetText(pcText);
    
    m_pkScreenElements = pkScreenElements;
    m_pkScreenTextures = pkScreenTextures;
}
//---------------------------------------------------------------------------
short NiMenuItem::GetHeight()
{
    return m_pkScreenText->GetHeight();
}
//---------------------------------------------------------------------------
short NiMenuItem::GetWidth()
{
    return GetIndentOffset() + m_pkScreenText->GetWidth();
}
//---------------------------------------------------------------------------
void NiMenuItem::AdjustScreenTextureLocation(short sX, short sY)
{
    m_pkScreenText->SetTextOrigin(sX + GetIndentOffset(), sY);
}
//---------------------------------------------------------------------------
void NiMenuItem::SetVisible(bool bVisible)
{
    m_pkScreenText->SetVisible(bVisible);
}
//---------------------------------------------------------------------------
void NiMenuItem::SetColor(NiColorA& kNewColor)
{
    m_pkScreenText->SetColor(kNewColor);
}
//---------------------------------------------------------------------------
short NiMenuItem::GetIndentOffset()
{
    return m_iCharacterIndent * m_pkScreenText->GetCharacterSpacing();
}
//---------------------------------------------------------------------------
int NiMenuItem::Update(float fAccumTime)
{
    return 0;
}
//---------------------------------------------------------------------------
int NiMenuItem::GetIdentifier()
{
    return m_iIdentifier;
}
//---------------------------------------------------------------------------
const char* NiMenuItem::GetText()
{
    return m_pkScreenText->GetString();
}
//---------------------------------------------------------------------------
void NiMenuItem::SetText(const char* pcText)
{
    m_pkScreenText->SetString(pcText);
}
//---------------------------------------------------------------------------
