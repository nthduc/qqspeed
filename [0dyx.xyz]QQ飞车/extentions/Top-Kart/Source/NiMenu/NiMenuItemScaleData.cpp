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
#include "NiMenuItemScaleData.h"
#include "NiScreenText.h"
#include "../InputManager.h"

//---------------------------------------------------------------------------
NiMenuItemScaleData::NiMenuItemScaleData(
    NiMenuItemScaleData::InitStruct kInit)
{
    m_pcText = NULL;

    m_fScaleData = kInit.m_fScaleData;
    m_fMultiplier = kInit.m_fMultiplier;
    m_fMinValue = kInit.m_fMinValue;
    m_fMaxValue = kInit.m_fMaxValue;
    m_fIncrementValue = kInit.m_fIncrementValue;

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
NiMenuItemScaleData::~NiMenuItemScaleData()
{
    NiFree(m_pcText);
}
//---------------------------------------------------------------------------
const char* NiMenuItemScaleData::GetText()
{
    return m_pcText;
}
//---------------------------------------------------------------------------
void NiMenuItemScaleData::SetText(const char* pcText)
{
    // Save the Text Internally
    NiFree(m_pcText);

    m_pcText = (char*)NiMalloc(strlen(pcText) + 1);

    NiStrcpy(m_pcText, strlen(pcText) + 1, pcText);

    // Rebuild the Screen Text
    char* pcTemp = (char*)NiMalloc(strlen(m_pcText) + 64);
    NiSprintf(pcTemp, strlen(m_pcText) + 64, "%s : %f", 
        m_pcText, m_fScaleData);

    m_pkScreenText->SetString(pcTemp);

    NiFree(pcTemp);
}
//---------------------------------------------------------------------------
float NiMenuItemScaleData::GetScaleData()
{
    return m_fScaleData;
}
//---------------------------------------------------------------------------
void NiMenuItemScaleData::SetScaleData(float fNewData)
{
    // Check for no changes
    if (fNewData == m_fScaleData)
        return;

    m_fScaleData = fNewData;

    // Rebuild the Screen Text
    char* pcTemp = (char*)NiMalloc(strlen(m_pcText) + 64);
    NiSprintf(pcTemp, strlen(m_pcText) + 64, "%s : %.2f",
        m_pcText, m_fScaleData);

    m_pkScreenText->SetString(pcTemp);

    NiFree(pcTemp);
}
//---------------------------------------------------------------------------
int NiMenuItemScaleData::Update(float fAccumTime)
{
    float fMultiplier = 1.0f;

    NiInputKeyboard* pkKeyboard = InputManager::Get()->
        GetInputSystem()->GetKeyboard();

    // Check for the keyboard exists
    if (pkKeyboard)
    {
        // Set the Multiplier if the Multiplier button is down
        if (pkKeyboard->KeyIsDown(m_eKeyboardMultiplier))
        {
            fMultiplier = m_fMultiplier;
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkKeyboard->KeyIsDown(m_eKeyboardIncrement))
        {
            if ((m_fScaleData + (m_fIncrementValue * fMultiplier)) <= 
                m_fMaxValue)
            {
                SetScaleData(m_fScaleData + (m_fIncrementValue * fMultiplier));
                return NiMenu::RETURN_ITEM_SELECTED;
            }
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkKeyboard->KeyIsDown(m_eKeyboardDecrement))
        {
            if ((m_fScaleData - (m_fIncrementValue * fMultiplier)) >= 
                m_fMinValue)
            {
                SetScaleData(m_fScaleData - (m_fIncrementValue * fMultiplier));
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
            fMultiplier = m_fMultiplier;
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkGamePad->ButtonWasPressed(m_eGamePadIncrement))
        {
            if ((m_fScaleData + (m_fIncrementValue * fMultiplier)) <= 
                m_fMaxValue)
            {
                SetScaleData(m_fScaleData + (m_fIncrementValue * fMultiplier));
                return NiMenu::RETURN_ITEM_SELECTED;
            }
        }
        
        // Check if the button is down and we won't exceed our bounds
        if (pkGamePad->ButtonWasPressed(m_eGamePadDecrement))
        {
            if ((m_fScaleData - (m_fIncrementValue * fMultiplier)) >= 
                m_fMinValue)
            {
                SetScaleData(m_fScaleData - (m_fIncrementValue * fMultiplier));
                return NiMenu::RETURN_ITEM_SELECTED;
            }
        }
    }

    return NiMenu::RETURN_NONE;
}
//---------------------------------------------------------------------------
