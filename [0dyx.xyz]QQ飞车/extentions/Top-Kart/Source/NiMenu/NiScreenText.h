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

#ifndef NISCREENTEXT_H
#define NISCREENTEXT_H

#include <NiScreenTexture.h>
#include <NiColor.h>
#include <NiPoint2.h>
#include <NiPoint3.h>
#include <NiSmartPointer.h>

NiSmartPointer(NiScreenTexture);
NiSmartPointer(NiTexture);
NiSmartPointer(NiCamera);

class NiScreenText : public NiMemObject
{
public:
    NiScreenText(unsigned int uiMaxChars,
        NiScreenTextureArray* pkScreenTextures, const NiColorA& kColor);
    virtual ~NiScreenText();

    // Will truncate the string to the pre-set max number of characters
    void SetString(const char* pcString);
    const char* GetString();

    // Works in screen space pixel coords: <0 - (m-1), 0 - (n-1)>, where the
    // screen size is (m,n), to set the position of the upper-left edge of 
    // the text
    void SetTextOrigin(unsigned int uiX, unsigned int uiY);
    void GetTextOrigin(unsigned int& uiX, unsigned int& uiY) const;

	// Add by wincoqin for caculate the pos of the text
	unsigned int GetCurRows();
	unsigned int GetCurCols();
    // Add end

    void SetScrollDown(bool bDown);
    bool GetScrollDown() const;

    void SetColor(NiColorA& kNewColor);
    NiColorA& GetColor();

    void SetVisible(bool bVisible);

    inline short GetHeight(){ return (short)ms_uiCharSpacingY; }
    inline short GetWidth(){ return (short)(ms_uiCharSpacingX * 
        m_uiNumChars); }
    inline short GetCharacterSpacing(){ return(short)ms_uiCharSpacingX; }

    // Sets the line length for word wrapping  This should always be less 
    // than the text size, or else truncation will occur.
    void SetMaxLineLength(unsigned int uiColumns);
    unsigned int GetMaxLineLength();

    // Will not append beyond the pre-set max number of characters
    void AppendCharacter(char cChar);
    void DeleteLastCharacter();

    void RecreateText();

protected:
    NiScreenTexturePtr m_spScreenTexture;
    NiScreenTextureArray* m_pkScreenTextures;

    NiColorA m_kColor;
    bool m_bIsVisible;

    unsigned int m_uiTextOriginX;
    unsigned int m_uiTextOriginY;
    
    char* m_pcString;
    unsigned int m_uiMaxChars;
    unsigned int m_uiNumChars;
    unsigned int m_uiMaxCols;

    unsigned int m_uiNumRects;
    unsigned int m_uiNumCurrentRows;
    unsigned int m_uiCurrentColumn;

    bool m_bScrollDown;

    static const unsigned int ms_uiCharWidth;
    static const unsigned int ms_uiCharHeight;
    static const unsigned int ms_uiCharSpacingX;
    static const unsigned int ms_uiCharSpacingY;
    static const unsigned int ms_uiCharBaseU;
    static const unsigned int ms_uiCharBaseV;
    static NiTexturePtr ms_spTextTexture;
    static const char* ms_pcTextImage;
    static const unsigned int ms_uiASCIIMin;
    static const unsigned int ms_uiASCIIMax;
    static const unsigned int ms_uiASCIICols;
    static int ms_iCount;
};
//---------------------------------------------------------------------------
inline void NiScreenText::SetString(const char* pcString)
{
    unsigned int uiNewLength = strlen(pcString);

	// Add by wincoqin
	m_uiNumChars = uiNewLength > m_uiMaxChars - 1 ? m_uiMaxChars : uiNewLength;

	NiStrncpy(m_pcString, m_uiMaxChars, pcString, m_uiNumChars - 1);	
	// Add end
	
	// Comment by wincoqin
	/*
    if (uiNewLength > m_uiMaxChars)
        uiNewLength = m_uiMaxChars;

    m_uiNumChars = uiNewLength + 1;

	NiStrcpy(m_pcString, strlen(pcString) + 1, pcString);
	*/
    RecreateText();
}
//---------------------------------------------------------------------------
inline const char* NiScreenText::GetString()
{
    return m_pcString;
}
//---------------------------------------------------------------------------
inline void NiScreenText::SetTextOrigin(unsigned int uiX, unsigned int uiY)
{
    if (m_uiTextOriginX != uiX || m_uiTextOriginY != uiY)
    {
        m_uiTextOriginX = uiX;
        m_uiTextOriginY = uiY;
        RecreateText();
    }
}
//---------------------------------------------------------------------------
inline void NiScreenText::GetTextOrigin(unsigned int& uiX, unsigned int& uiY)
    const
{
    uiX = m_uiTextOriginX;
    uiY = m_uiTextOriginY;
}
//---------------------------------------------------------------------------
inline unsigned int NiScreenText::GetCurRows()
{
	return m_uiNumCurrentRows;
}

//---------------------------------------------------------------------------
inline unsigned int NiScreenText::GetCurCols()
{
	return m_uiCurrentColumn;
}

//---------------------------------------------------------------------------
inline void NiScreenText::SetScrollDown(bool bDown)
{
    if (m_bScrollDown != bDown)
    {
        m_bScrollDown = bDown;
        RecreateText();
    }
}
//---------------------------------------------------------------------------
inline bool NiScreenText::GetScrollDown() const
{
    return m_bScrollDown;
}
//---------------------------------------------------------------------------
inline void NiScreenText::SetMaxLineLength(unsigned int uiColumns)
{
    if (m_uiMaxCols != uiColumns)
    {
        m_uiMaxCols = uiColumns;
        RecreateText();
    }
}
//---------------------------------------------------------------------------
inline unsigned int NiScreenText::GetMaxLineLength()
{
    return m_uiMaxCols;
}
//---------------------------------------------------------------------------
#endif
