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

#ifndef NIMENUITEMSCALEDATA_H
#define NIMENUITEMSCALEDATA_H

#include "NiMenuItem.h"
#include <NiInputGamePad.h>
#include <NiInputKeyboard.h>

class NiMenuItemScaleData : public NiMenuItem
{
public:

    class InitStruct
    {
    public:
        int m_iIdentifier;
        bool m_bSelectable;
        int m_iCharacterIndent;
        void* m_pvData;
        const char* m_pcText;
        NiColorA m_kColor; 

        NiScreenElementsArray* m_pkScreenElements;
        NiScreenTextureArray* m_pkScreenTextures;

        float m_fScaleData;
        float m_fMinValue;
        float m_fMaxValue;
        float m_fIncrementValue;
        float m_fMultiplier;
        // Keyboard
        NiInputKeyboard::KeyCode m_eKeyboardIncrement;
        NiInputKeyboard::KeyCode m_eKeyboardDecrement;
        NiInputKeyboard::KeyCode m_eKeyboardMultiplier;
        // GamePad
        NiInputGamePad::Button m_eGamePadIncrement;
        NiInputGamePad::Button m_eGamePadDecrement;
        NiInputGamePad::Button m_eGamePadMultiplier;
    };

    NiMenuItemScaleData(InitStruct kInit);

    virtual ~NiMenuItemScaleData();

    virtual const char* GetText();
    virtual void SetText(const char* pcText);

    virtual float GetScaleData();
    virtual void SetScaleData(float fNewData);

    virtual int Update(float fAccumTime);

protected:
    friend class NiMenu;

    char* m_pcText;
    float m_fScaleData;
    float m_fMultiplier;
    float m_fMinValue;
    float m_fMaxValue;
    float m_fIncrementValue;

    // Keyboard IO values
    NiInputKeyboard::KeyCode m_eKeyboardIncrement;
    NiInputKeyboard::KeyCode m_eKeyboardDecrement;
    NiInputKeyboard::KeyCode m_eKeyboardMultiplier;

    // GamePad IO values
    NiInputGamePad::Button m_eGamePadIncrement;
    NiInputGamePad::Button m_eGamePadDecrement;
    NiInputGamePad::Button m_eGamePadMultiplier;
};

#endif
