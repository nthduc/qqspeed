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

#ifndef NIMENUITEMINTDATA_H
#define NIMENUITEMINTDATA_H

#include "NiMenuItem.h"
#include <NiInputGamePad.h>
#include <NiInputKeyboard.h>

class NiMenuItemIntData : public NiMenuItem
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
        int m_iIntData;
        int m_iMultiplier;
        int m_iMinValue;
        int m_iMaxValue;
        int m_iIncrementValue;
        // Keyboard
        NiInputKeyboard::KeyCode m_eKeyboardIncrement;
        NiInputKeyboard::KeyCode m_eKeyboardDecrement;
        NiInputKeyboard::KeyCode m_eKeyboardMultiplier;
        // GamePad
        NiInputGamePad::Button m_eGamePadIncrement;
        NiInputGamePad::Button m_eGamePadDecrement;
        NiInputGamePad::Button m_eGamePadMultiplier;
    };

    NiMenuItemIntData(InitStruct kInit);

    virtual ~NiMenuItemIntData();

    virtual const char* GetText();
    virtual void SetText(const char* pcText);

    virtual int GetIntData();
    virtual void SetIntData(int iNewData);

    virtual int Update(float fAccumTime);

protected:
    friend class NiMenu;

    char* m_pcText;
    int m_iIntData;
    int m_iMultiplier;
    int m_iMinValue;
    int m_iMaxValue;
    int m_iIncrementValue;

    // Keyboard IO values
    NiInputKeyboard::KeyCode m_eKeyboardIncrement;
    NiInputKeyboard::KeyCode m_eKeyboardDecrement;
    NiInputKeyboard::KeyCode m_eKeyboardMultiplier;

    // GamePad IO values
    NiInputGamePad::Button m_eGamePadIncrement;
    NiInputGamePad::Button m_eGamePadDecrement;
    NiInputGamePad::Button m_eGamePadMultiplier;
};

#endif  //NIMENUITEMINTDATA_H
