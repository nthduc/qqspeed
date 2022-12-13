
#include "Top-KartPCH.h"
#include "NiScreenText.h"

#include <NiCamera.h>
#include <NiTexturingProperty.h>
#include <NiApplication.h>

NiTexturePtr NiScreenText::ms_spTextTexture = NULL;

const char* NiScreenText::ms_pcTextImage = "ASCII_CON.TGA";
const unsigned int NiScreenText::ms_uiCharWidth = 8;
const unsigned int NiScreenText::ms_uiCharHeight = 12;
const unsigned int NiScreenText::ms_uiCharSpacingX = 9;
const unsigned int NiScreenText::ms_uiCharSpacingY = 13;
const unsigned int NiScreenText::ms_uiCharBaseU = 0;
const unsigned int NiScreenText::ms_uiCharBaseV = 0;

const unsigned int NiScreenText::ms_uiASCIIMin = 33;
const unsigned int NiScreenText::ms_uiASCIIMax = 122;
const unsigned int NiScreenText::ms_uiASCIICols = 12;
int NiScreenText::ms_iCount = 0;

//---------------------------------------------------------------------------
NiScreenText::NiScreenText(unsigned int uiMaxChars,
    NiScreenTextureArray* pkScreenTextures, const NiColorA& kColor)
{
    ms_iCount++;
    m_bIsVisible = false;

    if (!ms_spTextTexture)
    {
        ms_spTextTexture = NiSourceTexture::Create(
            NiApplication::ConvertMediaFilename(ms_pcTextImage));
    }

    m_uiMaxChars = uiMaxChars;
    m_kColor = kColor;
    m_spScreenTexture = NiNew NiScreenTexture(ms_spTextTexture);
    m_spScreenTexture->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);

    m_pkScreenTextures = pkScreenTextures;

    m_uiTextOriginX = 0;
    m_uiTextOriginY = 0;
    m_bScrollDown = false;

    // String is _not_ NULL terminated
    m_pcString = (char*)NiMalloc(m_uiMaxChars);
    m_uiNumChars = 0;

    m_uiMaxCols = 40;

    m_uiNumRects = 0;
    m_uiNumCurrentRows = 0;
    m_uiCurrentColumn = 0;

    AppendCharacter('T');
    AppendCharacter('E');
    AppendCharacter('S');
    AppendCharacter('T');
}
//---------------------------------------------------------------------------
NiScreenText::~NiScreenText()
{
    m_pkScreenTextures->Remove(m_spScreenTexture);

    ms_iCount--;

    if (ms_iCount < 1)
    {
        ms_spTextTexture = NULL;
    }

    NiFree(m_pcString);
}
//---------------------------------------------------------------------------
void NiScreenText::RecreateText()
{
    m_spScreenTexture->RemoveAllScreenRects();

    m_uiNumCurrentRows = 0;
    m_uiCurrentColumn = 0;

    for (unsigned int i = 0; i < m_uiNumChars; i++)
    {
        char cChar = m_pcString[i];

        // if we are at the end of a line or if the char is '\n' then move
        // to the start of the next line
        if ((m_uiCurrentColumn >= m_uiMaxCols) || (cChar == '\n'))
        {
            m_uiNumCurrentRows++;
            m_uiCurrentColumn = 0;

            if (!m_bScrollDown)
            {
                // Move all characters up one row
                unsigned int uiNumRects = 
                    m_spScreenTexture->GetNumScreenRects();
                for (unsigned int j = 0; j < uiNumRects; j++)
                {
                    NiScreenTexture::ScreenRect& kRect = 
                        m_spScreenTexture->GetScreenRect(j);
                    kRect.m_sPixTop -= ms_uiCharSpacingY;
                }
            }

            // if the extra char is a '\n', skip it
            if (cChar == '\n')
                continue;
        }

        unsigned int uiChar = (unsigned int)cChar;

        // skip whitespace or unprintable character
        if ((uiChar >= ms_uiASCIIMin) && (uiChar <= ms_uiASCIIMax))
        {
            uiChar -= ms_uiASCIIMin;
    
            unsigned short usPixTop = m_uiTextOriginY;
        if (m_bScrollDown)
            usPixTop += (m_uiNumCurrentRows + 1) * ms_uiCharSpacingY;
            unsigned short usPixLeft = m_uiTextOriginX + 
                m_uiCurrentColumn * ms_uiCharSpacingX;

            unsigned short usTexTop = ms_uiCharBaseV + 
                (uiChar / ms_uiASCIICols) * ms_uiCharSpacingY;
            unsigned short usTexLeft = ms_uiCharBaseU + 
                (uiChar % ms_uiASCIICols) * ms_uiCharSpacingX;

            m_spScreenTexture->AddNewScreenRect(usPixTop, usPixLeft, 
                ms_uiCharWidth, ms_uiCharHeight, usTexTop, usTexLeft, 
                m_kColor);
        }

        m_uiCurrentColumn++;
    }
    m_spScreenTexture->MarkAsChanged(NiScreenTexture::EVERYTHING_MASK);
}
//---------------------------------------------------------------------------
void NiScreenText::AppendCharacter(char cChar)
{
    if (m_uiNumChars >= (m_uiMaxChars - 1))
        return;
    
    m_pcString[m_uiNumChars] = cChar;

    m_uiNumChars++;
    m_pcString[m_uiNumChars] = '\0';

    m_spScreenTexture->MarkAsChanged(NiScreenTexture::EVERYTHING_MASK);

    // if we are at the end of a line or if the char is '\n' then move
    // to the start of the next line
    if ((m_uiCurrentColumn >= m_uiMaxCols) || (cChar == '\n'))
    {
        m_uiNumCurrentRows++;
        m_uiCurrentColumn = 0;

        if (!m_bScrollDown)
        {
            // Move all characters up one row
            unsigned int uiNumRects = m_spScreenTexture->GetNumScreenRects();
            for (unsigned int i = 0; i < uiNumRects; i++)
            {
                NiScreenTexture::ScreenRect& kRect = 
                    m_spScreenTexture->GetScreenRect(i);
                kRect.m_sPixTop -= ms_uiCharSpacingY;
            }
        }

        // if the extra char is a '\n', skip it
        if (cChar == '\n')
            return;
    }

    unsigned int uiChar = (unsigned int)cChar;

    // skip whitespace or unprintable character
    if ((uiChar >= ms_uiASCIIMin) && (uiChar <= ms_uiASCIIMax))
    {
        uiChar -= ms_uiASCIIMin;
    
        unsigned short usPixTop = m_uiTextOriginY;
        if (m_bScrollDown)
            usPixTop += (m_uiNumCurrentRows + 1) * ms_uiCharSpacingY;
        unsigned short usPixLeft = m_uiTextOriginX + 
            m_uiCurrentColumn * ms_uiCharSpacingX;

        unsigned short usTexTop = ms_uiCharBaseV + 
            (uiChar / ms_uiASCIICols) * ms_uiCharSpacingY;
        unsigned short usTexLeft = ms_uiCharBaseU + 
            (uiChar % ms_uiASCIICols) * ms_uiCharSpacingX;

        m_spScreenTexture->AddNewScreenRect(usPixTop, usPixLeft, 
            ms_uiCharWidth, ms_uiCharHeight, usTexTop, usTexLeft, m_kColor);
    }

    m_uiCurrentColumn++;
}
//---------------------------------------------------------------------------
void NiScreenText::DeleteLastCharacter()
{
    if (!m_uiNumChars)
        return;

    m_uiNumChars--;
    char cChar = m_pcString[m_uiNumChars];
    m_pcString[m_uiNumChars] = '\0';

    // if we are at the beginning of a line then return to end of the 
    // previous line - easiest way to do this is to recreate the text
    if (m_uiCurrentColumn == 0)
    {
        RecreateText();
    }
    else
    {
        m_spScreenTexture->MarkAsChanged(NiScreenTexture::EVERYTHING_MASK);

        unsigned int uiChar = (unsigned int)cChar;

        // skip whitespace or unprintable character
        if ((uiChar >= ms_uiASCIIMin) && (uiChar <= ms_uiASCIIMax))
        {
            // Remove character
            m_spScreenTexture->RemoveScreenRect(
                m_spScreenTexture->GetNumScreenRects() - 1);
        }
        m_uiCurrentColumn--;
    }
}
//---------------------------------------------------------------------------
void NiScreenText::SetColor(NiColorA& kNewColor)
{
    if(kNewColor == m_kColor)
        return;

    m_kColor = kNewColor;

    // Update all of the Screen Rectangles
    for(unsigned int uiLoop = 0; 
        uiLoop < m_spScreenTexture->GetNumScreenRects(); uiLoop++)
    {
        NiScreenTexture::ScreenRect& kTemp = 
            m_spScreenTexture->GetScreenRect(uiLoop);

        kTemp.m_kColor = m_kColor;
    }

    m_spScreenTexture->MarkAsChanged(NiScreenTexture::COLOR_MASK);
}
//---------------------------------------------------------------------------
NiColorA& NiScreenText::GetColor()
{
    return m_kColor;
}
//---------------------------------------------------------------------------
void NiScreenText::SetVisible(bool bVisible)
{
    m_bIsVisible = bVisible;
    if(bVisible)
        m_pkScreenTextures->AddFirstEmpty(m_spScreenTexture);
    else
        m_pkScreenTextures->Remove(m_spScreenTexture);
}
//---------------------------------------------------------------------------
