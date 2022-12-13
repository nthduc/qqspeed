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

#include "../InputManager.h"
#include "NiMenu.h"
#include "NiMenuItemBooleanData.h"
#include "NiScreenText.h"

//---------------------------------------------------------------------------
NiMenuItemBooleanData::NiMenuItemBooleanData(
    NiMenuItemBooleanData::InitStruct kInit)
{
    m_pcText = NULL;

    m_eControlMethod = kInit.m_eControlMethod;

    m_bBooleanData = kInit.m_bBooleanData;
    m_eKeyboardToggle = kInit.m_eKeyboardToggle;
    m_eGamePadToggle = kInit.m_eGamePadToggle;

    // Save the True/False Test
    m_pcTrueText = (char*)NiMalloc(strlen(kInit.m_pcTrueText) + 1);
    NiStrcpy(m_pcTrueText, strlen(kInit.m_pcTrueText) + 1,
        kInit.m_pcTrueText);
        
    m_pcFalseText = (char*)NiMalloc(strlen(kInit.m_pcFalseText) + 1);
    NiStrcpy(m_pcFalseText, strlen(kInit.m_pcFalseText) + 1, 
        kInit.m_pcFalseText);

    Initialize(kInit.m_iIdentifier, kInit.m_bSelectable, 
        kInit.m_iCharacterIndent, kInit.m_pvData, kInit.m_pcText, 
        kInit.m_kColor, kInit.m_pkScreenElements, kInit.m_pkScreenTextures);
}
//---------------------------------------------------------------------------
NiMenuItemBooleanData::~NiMenuItemBooleanData()
{
    NiFree(m_pcText);
    NiFree(m_pcTrueText);
    NiFree(m_pcFalseText);
}
//---------------------------------------------------------------------------
const char* NiMenuItemBooleanData::GetText()
{
    return m_pcText;
}
//---------------------------------------------------------------------------
void NiMenuItemBooleanData::SetText(const char* pcText)
{
    // Save the Text Internally 
    NiFree(m_pcText);

    m_pcText = (char*)NiMalloc(strlen(pcText) + 1);

    NiStrcpy(m_pcText, strlen(pcText) + 1, pcText);

    // Rebuild the Screen Text
    char* pcTemp = NULL;
    char pcSeparator[10] = " : ";

    // Create the True / False String
    if (m_bBooleanData)
    {
        int iLength = strlen(m_pcText) + strlen(m_pcTrueText) + 
            strlen(pcSeparator) + 1;
        pcTemp = (char*)NiMalloc(iLength);
        NiSprintf(pcTemp, iLength, "%s%s%s",
            m_pcText, pcSeparator, m_pcTrueText);
    }
    else
    {
        int iLength = strlen(m_pcText) + strlen(m_pcFalseText) + 
            strlen(pcSeparator) + 1;
        pcTemp = (char*)NiMalloc(iLength);
        NiSprintf(pcTemp, iLength, "%s%s%s",
            m_pcText, pcSeparator, m_pcFalseText);
    }

    m_pkScreenText->SetString(pcTemp);

    NiFree(pcTemp);
}
//---------------------------------------------------------------------------
bool NiMenuItemBooleanData::GetBooleanData()
{
    return m_bBooleanData;
}
//---------------------------------------------------------------------------
void NiMenuItemBooleanData::SetBooleanData(bool bNewData)
{
    // Check for no changes
    if (bNewData == m_bBooleanData)
        return;

    m_bBooleanData = bNewData;

    // Rebuild the Screen Text
    char* pcTemp = NULL;
    char pcSeparator[10] = " : ";

    // Create the True / False String
    if (m_bBooleanData)
    {
        int iLength = strlen(m_pcText) + strlen(m_pcTrueText) + 
            strlen(pcSeparator) + 1;
        pcTemp = (char*)NiMalloc(iLength);
        NiSprintf(pcTemp, iLength, "%s%s%s",
            m_pcText, pcSeparator, m_pcTrueText);
    }
    else
    {
        int iLength = strlen(m_pcText) + strlen(m_pcFalseText) + 
            strlen(pcSeparator) + 1;
        pcTemp = (char*)NiMalloc(iLength);
        NiSprintf(pcTemp, iLength, "%s%s%s",
            m_pcText, pcSeparator, m_pcFalseText);
    }

    m_pkScreenText->SetString(pcTemp);

    NiFree(pcTemp);
}
//---------------------------------------------------------------------------
int NiMenuItemBooleanData::Update(float fAccumTime)
{

    NiInputKeyboard* pkKeyboard = InputManager::Get()->
        GetInputSystem()->GetKeyboard();

    // Check for the keyboard exists
    if (pkKeyboard)
    {
        // Check if the button is down
        if (pkKeyboard->KeyWasPressed(m_eKeyboardToggle))
        {
            SetBooleanData(!GetBooleanData());
            return NiMenu::RETURN_ITEM_SELECTED;
        }
    }

    NiInputGamePad* pkGamePad = InputManager::Get()->
        GetInputSystem()->GetGamePad(0,0);

    // Check the gamepad exists
    if (pkGamePad)
    {
        // Check if the button is down
        if (pkGamePad->ButtonWasPressed(m_eGamePadToggle))
        {
            SetBooleanData(!GetBooleanData());
            return NiMenu::RETURN_ITEM_SELECTED;
        }
    }

    return NiMenu::RETURN_NONE;
}
//---------------------------------------------------------------------------
