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

#ifndef NIMENUITEM_H
#define NIMENUITEM_H

#include <NiScreenElements.h>
#include <NiScreenTexture.h>
#include <NiCamera.h>

class NiScreenText;

class NiMenuItem : public NiMemObject
{
public:
    NiMenuItem(int iIdentifier, bool bSelectable, int iCharacterIndent,
        void* pvData, const char* pcText, NiColorA& kColor, 
        NiScreenElementsArray* pkScreenElements,
        NiScreenTextureArray* pkScreenTextures);

    NiMenuItem();   // Default Constructor Do not use

    virtual ~NiMenuItem();

    virtual void SetVisible(bool bVisible);
    virtual void SetColor(NiColorA& kNewColor);

    virtual int GetIdentifier();

    virtual const char* GetText();
    virtual void SetText(const char* pcText);

    virtual short GetHeight();
    virtual short GetWidth();

    virtual void AdjustScreenTextureLocation(short sX, short sY);

    virtual int Update(float fAccumTime);

protected:
    void Initialize(int iIdentifier, bool bSelectable, 
        int iCharacterIndent, void* pvData, const char* pcText, 
        NiColorA& kColor, NiScreenElementsArray* pkScreenElements,
        NiScreenTextureArray* pkScreenTextures);

    short GetIndentOffset();

    int m_iIdentifier;
    bool m_bSelectable;
    int m_iCharacterIndent;
    void* m_pvData;
    NiScreenText* m_pkScreenText;
    NiScreenElementsArray* m_pkScreenElements;
    NiScreenTextureArray* m_pkScreenTextures;

    friend class NiMenu;
};

#endif