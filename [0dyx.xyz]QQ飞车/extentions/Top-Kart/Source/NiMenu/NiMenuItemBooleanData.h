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

#ifndef NIMENUITEMBOOLEANDATA_H
#define NIMENUITEMBOOLEANDATA_H

#include "NiMenuItem.h"
#include "NiInputKeyboard.h"
#include "NiInputGamePad.h"

class NiMenuItemBooleanData : public NiMenuItem
{
public:

    class InitStruct
    {
    public:
        NiMenuControlMode m_eControlMethod;
        int m_iIdentifier;
        bool m_bSelectable;
        int m_iCharacterIndent;
        void* m_pvData;
        const char* m_pcText;
        const char* m_pcTrueText;
        const char* m_pcFalseText;
        NiColorA m_kColor; 
        NiScreenElementsArray* m_pkScreenElements;
        NiScreenTextureArray* m_pkScreenTextures;
        bool m_bBooleanData;
        // Keyboard
        NiInputKeyboard::KeyCode m_eKeyboardToggle;
        // Gamepad
        NiInputGamePad::Button m_eGamePadToggle;
    };

    NiMenuItemBooleanData(InitStruct kInit);

    virtual ~NiMenuItemBooleanData();

    virtual const char* GetText();
    virtual void SetText(const char* pcText);

    virtual bool GetBooleanData();
    virtual void SetBooleanData(bool bNewData);

    virtual int Update(float fAccumTime);

protected:
    friend class NiMenu;

    NiMenuControlMode m_eControlMethod;
    char* m_pcText;
    char* m_pcTrueText;
    char* m_pcFalseText;
    bool m_bBooleanData;

    // Keyboard IO values
    NiInputKeyboard::KeyCode m_eKeyboardToggle;
    // GamePad IO values
    NiInputGamePad::Button m_eGamePadToggle;
};


#endif
