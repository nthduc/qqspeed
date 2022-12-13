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

#ifndef NIMENUITEMFLOATDATA_H
#define NIMENUITEMFLOATDATA_H

#include "NiMenuItem.h"
#include <NiInputGamePad.h>
#include <NiInputKeyboard.h>

class NiMenuItemFloatData : public NiMenuItem
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
        float m_fFloatData;
        float m_fMultiplier;
        float m_fMinValue;
        float m_fMaxValue;
        float m_fIncrementValue;
        unsigned int m_uiPrecision;
        // Keyboard
        NiInputKeyboard::KeyCode m_eKeyboardIncrement;
        NiInputKeyboard::KeyCode m_eKeyboardDecrement;
        NiInputKeyboard::KeyCode m_eKeyboardMultiplier;
        // GamePad
        NiInputGamePad::Button m_eGamePadIncrement;
        NiInputGamePad::Button m_eGamePadDecrement;
        NiInputGamePad::Button m_eGamePadMultiplier;
    };

    NiMenuItemFloatData(InitStruct kInit);

    virtual ~NiMenuItemFloatData();

    virtual const char* GetText();
    virtual void SetText(const char* pcText);

    virtual float GetFloatData();
    virtual void SetFloatData(float fNewData);

    virtual int Update(float fAccumTime);


protected:
    friend class NiMenu;

    char* m_pcText;
    float m_fFloatData;
    float m_fMultiplier;
    float m_fMinValue;
    float m_fMaxValue;
    float m_fIncrementValue;
    unsigned int m_uiPrecision;

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
