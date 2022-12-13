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

#include "NiMenu.h"
#include "NiMenuItemIntData.h"
#include "../InputManager.h"
#include "NiScreenText.h"

//---------------------------------------------------------------------------
NiMenuItemIntData::NiMenuItemIntData(
    NiMenuItemIntData::InitStruct kInit)
{
    m_pcText = NULL;

    m_iIntData = kInit.m_iIntData;
    m_iMultiplier = kInit.m_iMultiplier;
    m_iMinValue = kInit.m_iMinValue;
    m_iMaxValue = kInit.m_iMaxValue;
    m_iIncrementValue = kInit.m_iIncrementValue;

    m_eKeyboardIncrement = kInit.m_eKeyboardIncrement;
    m_eKeyboardDecrement = kInit.m_eKeyboardDecrement;
    m_eKeyboardMultiplier = kInit.m_eKeyboardMultiplier;

    m_eGamePadIncrement = kInit.m_eGamePadIncrement;
    m_eGamePadDecrement = kInit.m_eGamePadDecrement;
    m_eGamePadMultiplier = kInit.m_eGamePadMultiplier;

    Initialize(kInit.m_iIdentifier, kInit.m_bSelectable, 
        kInit.m_iCharacterIndent, kInit.m_pvData, kInit.m_pcText, 
        kInit.m_kColor, kInit.m_pkScreenElements, kInit.m_pkScreenTextures);
}
//---------------------------------------------------------------------------
NiMenuItemIntData::~NiMenuItemIntData()
{
    NiFree(m_pcText);
}
//---------------------------------------------------------------------------
const char* NiMenuItemIntData::GetText()
{
    return m_pcText;
}
//---------------------------------------------------------------------------
void NiMenuItemIntData::SetText(const char* pcText)
{
    // Save the Text Internally 
    NiFree(m_pcText);

    m_pcText = (char*)NiMalloc(strlen(pcText) + 1);

    NiStrcpy(m_pcText, strlen(pcText) + 1, pcText);

    // Rebuild the Screen Text
    char* pcTemp = (char*)NiMalloc(strlen(m_pcText) + 64);
    NiSprintf(pcTemp, strlen(m_pcText) + 64, "%s : %d", m_pcText, m_iIntData);

    m_pkScreenText->SetString(pcTemp);

    NiFree(pcTemp);
}
//---------------------------------------------------------------------------
int NiMenuItemIntData::GetIntData()
{
    return m_iIntData;
}
//---------------------------------------------------------------------------
void NiMenuItemIntData::SetIntData(int iNewData)
{
    // Check for no changes
    if (iNewData == m_iIntData)
        return;

    m_iIntData = iNewData;

    // Rebuild the Screen Text
    char* pcTemp = (char*)NiMalloc(strlen(m_pcText) + 64);
    NiSprintf(pcTemp, strlen(m_pcText) + 64, "%s : %d", m_pcText, m_iIntData);

    m_pkScreenText->SetString(pcTemp);

    NiFree(pcTemp);
}
//---------------------------------------------------------------------------
int NiMenuItemIntData::Update(float fAccumTime)
{
    int iMultiplier = 1;

    
    NiInputKeyboard* pkKeyboard = InputManager::Get()->
        GetInputSystem()->GetKeyboard();

    // Check for the keyboard exists
    if (pkKeyboard)
    {
        // Set the Multiplier if the Multiplier button is down
        if (pkKeyboard->KeyIsDown(m_eKeyboardMultiplier))
        {
            iMultiplier = m_iMultiplier;
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkKeyboard->KeyIsDown(m_eKeyboardIncrement))
        {
            if ((m_iIntData + (m_iIncrementValue * iMultiplier)) <= 
                m_iMaxValue)
            {
                SetIntData(m_iIntData + (m_iIncrementValue * iMultiplier));
                return NiMenu::RETURN_ITEM_SELECTED;
            }
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkKeyboard->KeyIsDown(m_eKeyboardDecrement))
        {
            if ((m_iIntData - (m_iIncrementValue * iMultiplier)) >= 
                m_iMinValue)
            {
                SetIntData(m_iIntData - (m_iIncrementValue * iMultiplier));
                return NiMenu::RETURN_ITEM_SELECTED;
            }
        }
    }
    
    
    
    NiInputGamePad* pkGamePad = InputManager::Get()->
        GetInputSystem()->GetGamePad(0,0);

    // Check the gamepad exists
    if (pkGamePad)
    {
        // Set the Multiplier if the Multiplier button is down
        if (pkGamePad->ButtonIsDown(m_eGamePadMultiplier))
        {
            iMultiplier = m_iMultiplier;
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkGamePad->ButtonWasPressed(m_eGamePadIncrement))
        {
            if ((m_iIntData + (m_iIncrementValue * iMultiplier)) <= 
                m_iMaxValue)
            {
                SetIntData(m_iIntData + (m_iIncrementValue * iMultiplier));
                return NiMenu::RETURN_ITEM_SELECTED;
            }
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkGamePad->ButtonWasPressed(m_eGamePadDecrement))
        {
            if ((m_iIntData - (m_iIncrementValue * iMultiplier)) >= 
                m_iMinValue)
            {
                SetIntData(m_iIntData - (m_iIncrementValue * iMultiplier));
                return NiMenu::RETURN_ITEM_SELECTED;
            }
        }
    }
    return NiMenu::RETURN_NONE;
}
//---------------------------------------------------------------------------
