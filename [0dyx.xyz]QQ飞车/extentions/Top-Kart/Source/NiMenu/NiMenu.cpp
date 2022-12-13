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

#include <NiColor.h>

#include "NiMenu.h"
#include "NiMenuItem.h"
#include "NiMenuItemBooleanData.h"
#include "NiMenuItemFloatData.h"
#include "NiMenuItemIntData.h"
#include "../InputManager.h"

#define NEAR_ZERO 0.00001f

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
NiMenu::InitStruct::InitStruct()
{
    m_sLocationX = 0;
    m_sLocationY = 0;
    m_sBorderWidth = 10;
    m_sBackBufferWidth = 640;
    m_sBackBufferHeight = 480;
    m_pkScreenElements = NULL; 
    m_pkScreenTextures = NULL; 
    m_iMaxItemsShown = 10; 
    m_kTextColor = NiColorA::WHITE; 
    m_kSelectedColor = NiColorA(1.0f, 0.0f, 0.0f, 1.0f);
    m_kBackgroundColor = NiColorA::WHITE;
    m_pcMorePreviousIconFileName = NULL;
    m_pcMoreNextIconFileName = NULL;
    m_pcBackgroundFileName = NULL;
    //
    m_ePreviousKey = NiInputKeyboard::KEY_NOKEY; 
    m_eNextKey = NiInputKeyboard::KEY_NOKEY; 
    m_eSelectKey = NiInputKeyboard::KEY_NOKEY; 
    m_eOpenCloseKey = NiInputKeyboard::KEY_NOKEY;
    //
    m_ePreviousButton = NiInputGamePad::NIGP_NONE; 
    m_eNextButton = NiInputGamePad::NIGP_NONE; 
    m_eSelectButton = NiInputGamePad::NIGP_NONE; 
    m_eOpenCloseButton = NiInputGamePad::NIGP_NONE;
}
//---------------------------------------------------------------------------
NiMenu::NiMenu()
{
    m_bOpen = true;

    m_kSelected = NULL;
    m_kFirstItemShown = NULL;

    m_sLocationX = 0;
    m_sLocationY = 0;
    m_sBorderWidth = 4;
    m_sBackBufferWidth = 640;
    m_sBackBufferHeight = 480;
    m_pkScreenElements = 0;
    m_pkScreenTextures = 0;
    m_iMaxItemsShown = 6;
    m_kTextColor = NiColorA::WHITE;
    m_kSelectedColor = NiColorA(1.0f, 0.0f, 0.0f, 1.0f);
    m_kBackgroundColor = NiColorA(1.0f, 1.0f, 1.0f, 0.5f);

    m_ePreviousKey = NiInputKeyboard::KEY_H; 
    m_eNextKey = NiInputKeyboard::KEY_N; 
    m_eSelectKey = NiInputKeyboard::KEY_SPACE; 
    m_eOpenCloseKey = NiInputKeyboard::KEY_Y;

    m_ePreviousButton = NiInputGamePad::NIGP_LUP; 
    m_eNextButton = NiInputGamePad::NIGP_LDOWN; 
    m_eSelectButton = NiInputGamePad::NIGP_LDOWN; 
    m_eOpenCloseButton = NiInputGamePad::NIGP_NONE;

    m_spMorePreviousIcon = 0;
    m_spMoreNextIcon = 0;
    m_spBackground = 0;

    m_bVisible = true;
}
//---------------------------------------------------------------------------
NiMenu::NiMenu(NiMenu::InitStruct& kInit)
{
    m_bOpen = true;
    m_pkParentMenu = NULL;
    m_bVisible = true;

    if (!Initialize(kInit))
    {
        assert(0);
    }
}
//---------------------------------------------------------------------------
NiMenu::~NiMenu()
{
    RemoveAll();

    // Remove Previous from the camera
    if (m_spMorePreviousIcon)
    {
        m_spMorePreviousIcon->RemoveAllScreenRects();
        m_pkScreenTextures->Remove(m_spMorePreviousIcon);
    }

    // Remove Next From the camera
    if (m_spMoreNextIcon)
    {
        m_spMoreNextIcon->RemoveAllScreenRects();
        m_pkScreenTextures->Remove(m_spMoreNextIcon);
    }

    // Remove the background from the camera
    if (m_spBackground)
        m_pkScreenElements->Remove(m_spBackground);

    m_spMorePreviousIcon = 0;
    m_spMoreNextIcon = 0;
    m_spBackground = 0;
}
//---------------------------------------------------------------------------
NiMenu* NiMenu::Create(InitStruct& kInit)
{
    NiMenu* pkMenu = NiNew NiMenu();
    if (pkMenu)
    {
        bool bResult = pkMenu->Initialize(kInit);

        if (bResult)
            bResult = pkMenu->FillMenuItems();

        if (!bResult)
        {
            NiDelete pkMenu;
            pkMenu = 0;
        }
    }

    return pkMenu;
}
//---------------------------------------------------------------------------
// Initializer
bool NiMenu::Initialize(InitStruct& kInit)
{
    m_eControlMethod = kInit.m_eControlMethod;

    m_kSelected = NULL;
    m_kFirstItemShown = NULL;

    m_sLocationX = kInit.m_sLocationX;
    m_sLocationY = kInit.m_sLocationY;
    m_sBorderWidth = kInit.m_sBorderWidth;
    m_sBackBufferWidth = kInit.m_sBackBufferWidth;
    m_sBackBufferHeight = kInit.m_sBackBufferHeight;
    m_pkScreenElements = kInit.m_pkScreenElements;
    m_pkScreenTextures = kInit.m_pkScreenTextures;
    m_iMaxItemsShown = kInit.m_iMaxItemsShown;
    m_kTextColor = kInit.m_kTextColor;
    m_kSelectedColor = kInit.m_kSelectedColor;
    m_kBackgroundColor = kInit.m_kBackgroundColor;
    //
    m_ePreviousKey = kInit.m_ePreviousKey;
    m_eNextKey = kInit.m_eNextKey;
    m_eSelectKey = kInit.m_eSelectKey;
    m_eOpenCloseKey = kInit.m_eOpenCloseKey;
    //
    m_ePreviousButton = kInit.m_ePreviousButton;
    m_eNextButton = kInit.m_eNextButton;
    m_eSelectButton = kInit.m_eSelectButton;
    m_eOpenCloseButton = kInit.m_eOpenCloseButton;

    LoadMorePreviousIcon(kInit.m_pcMorePreviousIconFileName);
    LoadMoreNextIcon(kInit.m_pcMoreNextIconFileName);
    LoadBackground(kInit.m_pcBackgroundFileName);

    return true;
}
//---------------------------------------------------------------------------
bool NiMenu::IsOpen()
{
    return m_bOpen;
}
//---------------------------------------------------------------------------
void NiMenu::OpenMenu()
{
    if (!m_bOpen)
    {
        m_bOpen = true;
        SetVisible(true);
    }
}
//---------------------------------------------------------------------------
void NiMenu::CloseMenu()
{
    if (m_bOpen)
    {
        m_bOpen = false;
        SetVisible(false);
    }
}
//---------------------------------------------------------------------------
int NiMenu::Update(float fAccumTime,  NiMenuItem*& pkItem)
{
    // Allow the Menu Item to Update
    if (m_kSelected != NULL)
    {
        int iReturn = m_kMenuItems.Get(m_kSelected)->Update(fAccumTime);
        if (iReturn != RETURN_NONE)
        {
            pkItem = m_kMenuItems.Get(m_kSelected);

            return iReturn;
        }
    }    

    NiInputKeyboard* pkKeyboard = InputManager::Get()->
        GetInputSystem()->GetKeyboard();

    // Check for the keyboard exists
    if (pkKeyboard)
    {
        if (pkKeyboard->KeyWasPressed(m_ePreviousKey))
        {
            SelectPrevious();
        }
        
        if (pkKeyboard->KeyWasPressed(m_eNextKey))
        {
            SelectNext();
        }
        
        if (pkKeyboard->KeyWasPressed(m_eSelectKey))
        {
            pkItem = m_kMenuItems.Get(m_kSelected);
            return NiMenu::RETURN_ITEM_SELECTED;
        }
        
        if (pkKeyboard->KeyWasPressed(m_eOpenCloseKey))
        {
            SetVisible(!GetVisible());
        }
    }

    NiInputGamePad* pkGamePad = InputManager::Get()->
        GetInputSystem()->GetGamePad(0,0);

    // Check the gamepad exists
    if (pkGamePad)
    {
        if (pkGamePad->ButtonWasPressed(m_ePreviousButton))
        {
            SelectPrevious();
        }
        
        if (pkGamePad->ButtonWasPressed(m_eNextButton))
        {
            SelectNext();
        }
        
        if (pkGamePad->ButtonWasPressed(m_eSelectButton))
        {
            pkItem = m_kMenuItems.Get(m_kSelected);
            return NiMenu::RETURN_ITEM_SELECTED;
        }
        
        if (pkGamePad->ButtonWasPressed(m_eOpenCloseButton))
        {
            SetVisible(!GetVisible());
        }
    }
    return NiMenu::RETURN_NONE;
}
//---------------------------------------------------------------------------
NiMenuItem* NiMenu::GetMenuItem(int iIdentifier)
{
    // Find the Entry
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();
    while((kIter != NULL) && 
        (m_kMenuItems.Get(kIter)->m_iIdentifier != iIdentifier))
    {
        kIter = m_kMenuItems.GetNextPos(kIter);
    }

    // Item Not Found
    if (kIter == NULL)
        return 0;

    return m_kMenuItems.Get(kIter);
}
//---------------------------------------------------------------------------
void NiMenu::AddMenuItemAtEnd(int iIdentifier, bool bSelectable, 
        int iCharacterIndent, void* pvData, const char* pcText)
{
    NiMenuItem* pkNewItem = NiNew NiMenuItem(iIdentifier, bSelectable,
        iCharacterIndent, pvData, pcText, m_kTextColor, m_pkScreenElements,
        m_pkScreenTextures);

    AddMenuItemAtEnd(pkNewItem);
}
//---------------------------------------------------------------------------
void NiMenu::InsertMenuItemBefore(int iIdentifier, bool bSelectable, 
        int iCharacterIndent, void* pvData, const char* pcText,
        int iInsertionIdentifier)
{
    NiMenuItem* pkNewItem = NiNew NiMenuItem(iIdentifier, bSelectable,
        iCharacterIndent, pvData, pcText, m_kTextColor, m_pkScreenElements,
        m_pkScreenTextures);

    AddMenuItemBefore(iInsertionIdentifier, pkNewItem);
}
//---------------------------------------------------------------------------
void NiMenu::AddMenuItemAtEnd(NiMenuItem* pkNewItem)
{
    bool bOldTailVisible = IsMenuItemVisible(m_kMenuItems.GetTailPos());

    m_kMenuItems.AddTail(pkNewItem);

    // Set the more next icon
    if (bOldTailVisible && !IsMenuItemVisible(m_kMenuItems.GetTailPos()))
    {
        ResetMoreNextIcon();
        m_pkScreenTextures->AddFirstEmpty(m_spMoreNextIcon);
    }


    // Initialy set the first item shown
    if (m_kFirstItemShown == NULL)
        m_kFirstItemShown = m_kMenuItems.GetHeadPos();

    
    // Adjust the screen texure by what is above it.
    AdjustScreenTextureLocation(m_kMenuItems.GetTailPos());

        // Assign Default Selection
    if ((m_kSelected == NULL) && pkNewItem->m_bSelectable)
    {
        m_kSelected = m_kMenuItems.GetTailPos();

        // Set the color of the selected Item
        m_kMenuItems.Get(m_kSelected)->SetColor(m_kSelectedColor);
    }

    // Set the Visibility
    if (IsOpen())
        pkNewItem->SetVisible(IsMenuItemVisible(pkNewItem));
    else
        pkNewItem->SetVisible(false);

    // Resize the background
    ResetBackground();
}
//---------------------------------------------------------------------------
bool NiMenu::AddMenuItemBefore(int iIdentifier, NiMenuItem* pItem)
{
    // Find the Entry
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();
    while((kIter != NULL) && 
        (m_kMenuItems.Get(kIter)->m_iIdentifier != iIdentifier))
        kIter = m_kMenuItems.GetNextPos(kIter);

    // Item Not Found
    if (kIter == NULL)
        return false;

    // Check to see if we are modifying what is not visible
    if ((kIter == m_kFirstItemShown) || !IsMenuItemVisible(kIter))
    {
        // Check for the beginning of the list and turning on 
        // the More Previous icon
        if (m_kMenuItems.GetHeadPos() == kIter)
            m_pkScreenTextures->AddFirstEmpty(m_spMorePreviousIcon);

        // Add To the List
        m_kMenuItems.InsertBefore(kIter, pItem);

    }
    else
    {
        // Turn off the menu to remove items from the camera
        SetVisible(false);
    
        // Insert the Menu Item
        m_kMenuItems.InsertBefore(kIter, pItem);    

        // Turn on the menu to add items back to the camera
        SetVisible(true);

        // Loop through the possibly visible menu items and set there Location
        NiTListIterator kIter = m_kFirstItemShown;

        for(int iLoop = 0; (iLoop < m_iMaxItemsShown) && (kIter != NULL);
            iLoop++, kIter = m_kMenuItems.GetNextPos(kIter))
        {
            AdjustScreenTextureLocation(kIter);
        }
    }

    ResetMorePreviousIcon();
    ResetMoreNextIcon();
    ResetBackground();
    return true;
}
//---------------------------------------------------------------------------
bool NiMenu::AddMenuItemAfter(int iIdentifier, NiMenuItem* pItem)
{
    // Find the Entry
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();
    while((kIter != NULL) && 
        (m_kMenuItems.Get(kIter)->m_iIdentifier != iIdentifier))
        kIter = m_kMenuItems.GetNextPos(kIter);

    // Item Not Found
    if (kIter == NULL)
        return false;

    // Check to see if we are modifying what is not visible
    if (!IsMenuItemVisible(kIter) || IsMenuItemLastVisible(kIter))
    {
        // Check for the end of the list and turning on 
        // the more next icon
        if (IsMenuItemLastVisible(kIter) &&
            (m_kMenuItems.GetTailPos() == kIter))
        {
            m_pkScreenTextures->AddFirstEmpty(m_spMoreNextIcon);
        }

        m_kMenuItems.InsertAfter(kIter, pItem);
    }
    else
    {
        // Turn off the menu to remove items from the camera
        SetVisible(false);
    
        // Insert the Menu Item
        m_kMenuItems.InsertAfter(kIter, pItem);    

        // Turn on the menu to add items back to the camera
        SetVisible(true);

        // Loop through the possibly visible menu items and set there Location
        NiTListIterator kIter = m_kFirstItemShown;

        for(int iLoop = 0; (iLoop < m_iMaxItemsShown) && (kIter != NULL);
            iLoop++, kIter = m_kMenuItems.GetNextPos(kIter))
        {
            AdjustScreenTextureLocation(kIter);
        }
    }

    ResetMorePreviousIcon();
    ResetMoreNextIcon();
    ResetBackground();
    return true;
}
//---------------------------------------------------------------------------
void NiMenu::RemoveAll()
{
    m_pkScreenTextures->Remove(m_spMorePreviousIcon);
    m_pkScreenTextures->Remove(m_spMoreNextIcon);

    m_kSelected = NULL;
    m_kFirstItemShown = NULL;

    // Remove all of the MenuItems
    while(!m_kMenuItems.IsEmpty())
    {
        NiDelete m_kMenuItems.RemoveHead();
    }
}
//---------------------------------------------------------------------------
bool NiMenu::RemoveItem(int iIdentifier)
{
    // Find the Entry
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();
    while((kIter != NULL) && 
        (m_kMenuItems.Get(kIter)->m_iIdentifier != iIdentifier))
        kIter = m_kMenuItems.GetNextPos(kIter);

    // Object not found
    if (kIter == NULL)
        return false;

    // Item is not visible so you can remove it
    if (!IsMenuItemVisible(kIter))
    {
        // Check for turning off the More Prev Icon
        if (kIter == m_kMenuItems.GetHeadPos() &&
            (kIter == m_kMenuItems.GetPrevPos(m_kFirstItemShown)))
        {
            m_pkScreenTextures->Remove(m_spMorePreviousIcon);
        }

        // Check for turnning off the More Next Icon
        if (kIter == m_kMenuItems.GetTailPos() &&
           IsMenuItemLastVisible(m_kMenuItems.GetPrevPos(kIter)))
        {
            m_pkScreenTextures->Remove(m_spMoreNextIcon);
        }

        // Delete the menu item
        NiDelete m_kMenuItems.RemovePos(kIter);
    }
    else
    {
        // Turn off the menu to remove items from the camera
        SetVisible(false);
    
        // Check for the first item shown
        if (kIter == m_kFirstItemShown)
        {
            // Incorment the first items shown
            m_kFirstItemShown = m_kMenuItems.GetNextPos(m_kFirstItemShown);
        }

        // Delete the menu item
        NiDelete m_kMenuItems.RemovePos(kIter);
    
        // Turn on the menu to add items back to the camera
        SetVisible(true);

        // Loop through the possibly visible menu items and set there Location
        NiTListIterator kIter = m_kFirstItemShown;

        for(int iLoop = 0; (iLoop < m_iMaxItemsShown) && (kIter != NULL);
            iLoop++, kIter = m_kMenuItems.GetNextPos(kIter))
        {
            AdjustScreenTextureLocation(kIter);
        }

        ResetMorePreviousIcon();
        ResetMoreNextIcon();
        ResetBackground();
    }

    return true;
}
//---------------------------------------------------------------------------
void NiMenu::SetText(int iIdentifier, const char* pcText)
{
    // Find the Entry
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();
    while((kIter != NULL) && 
        (m_kMenuItems.Get(kIter)->m_iIdentifier != iIdentifier))
    {
        kIter = m_kMenuItems.GetNextPos(kIter);
    }

    // Item Not Found
    if (kIter == NULL)
        return;

    NiMenuItem* pkItem = m_kMenuItems.Get(kIter);
    if (pkItem)
        pkItem->SetText(pcText);

    ResetMorePreviousIcon();
    ResetMoreNextIcon();
    ResetBackground();
}
//---------------------------------------------------------------------------
void NiMenu::GetMenuItemStartingLocation(NiTListIterator kItem, short& sX, 
    short& sY)
{
    // Start at the top left cornere of the menu
    sX = m_sLocationX + m_sBorderWidth;
    sY = m_sLocationY + m_sBorderWidth;
    
    // Offset by the More Previous Icon
    sY += m_spMorePreviousIcon ? 
        m_spMorePreviousIcon->GetScreenRect(0).m_usPixHeight : 0;


    // Add in the visible menu items
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();

    while((kIter != NULL) && (kIter != kItem))
    {
        if (IsMenuItemVisible(kIter))
            sY += m_kMenuItems.Get(kIter)->GetHeight();

        kIter = m_kMenuItems.GetNextPos(kIter);
    }
}
//---------------------------------------------------------------------------
void NiMenu::AdjustScreenTextureLocation(NiTListIterator kItem)
{
    short sX;
    short sY;

    GetMenuItemStartingLocation(kItem, sX, sY);

    m_kMenuItems.Get(kItem)->AdjustScreenTextureLocation(sX, sY);
}
//---------------------------------------------------------------------------
bool NiMenu::IsMenuItemVisible(NiMenuItem* pkItem)
{
    NiTListIterator kCurrent = m_kFirstItemShown;

    // Loop in the list looking for the end or 
    // the maximum number of visible items
    for(int iLoop = 0; (kCurrent != NULL) && iLoop < m_iMaxItemsShown;
        iLoop++, kCurrent = m_kMenuItems.GetNextPos(kCurrent))
    {
        if (m_kMenuItems.Get(kCurrent) == pkItem)
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool NiMenu::IsMenuItemVisible(NiTListIterator kItem)
{
    NiTListIterator kCurrent = m_kFirstItemShown;

    // Loop in the list looking for the end or 
    // the maximum number of visible items
    for(int iLoop = 0; (kCurrent != NULL) && iLoop < m_iMaxItemsShown;
        iLoop++, kCurrent = m_kMenuItems.GetNextPos(kCurrent))
    {
        if (kCurrent == kItem)
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool NiMenu::IsMenuItemLastVisible(NiTListIterator kItem)
{
    NiTListIterator kCurrent = m_kFirstItemShown;

    for(int iLoop = 0; (kCurrent != NULL) && iLoop < (m_iMaxItemsShown - 1);
        iLoop++, kCurrent = m_kMenuItems.GetNextPos(kCurrent))
    {
    }

    // Beyond the end of the list
    if (kCurrent == NULL)
        return false;

    return(kCurrent == kItem);
}
//---------------------------------------------------------------------------
void NiMenu::SelectNext()
{
    NiTListIterator kNewSelected = m_kSelected;
    NiTListIterator kNewFirstItemShown = m_kFirstItemShown;


    // Increment the first item shown if the selected is at the end of the list
    if (IsMenuItemLastVisible(m_kSelected))
        kNewFirstItemShown = m_kMenuItems.GetNextPos(m_kFirstItemShown);

    // Increment the selected 
    kNewSelected = m_kMenuItems.GetNextPos(m_kSelected);


    // Keep incrementing until the end of the list or something selectable
    while((kNewSelected != NULL) &&
          (m_kMenuItems.Get(kNewSelected)->m_bSelectable == false))
    {
        // Increment the endi of the list if the new selected is at 
        // the end of what is visible
        if (IsMenuItemLastVisible(kNewSelected))
            kNewFirstItemShown = m_kMenuItems.GetNextPos(kNewFirstItemShown);

        kNewSelected = m_kMenuItems.GetNextPos(kNewSelected);
    }

    if (kNewSelected == NULL)
        kNewSelected = m_kMenuItems.GetHeadPos();

    // Did the Selected item change?
    if (kNewSelected != NULL)
    {
        // Rebuild the list if the first Item shown has moved
        ResetVisibleItems(kNewFirstItemShown);

        // Clear the old selected color
        m_kMenuItems.Get(m_kSelected)->SetColor(m_kTextColor);

        m_kSelected = kNewSelected;

        // Set the selected text color
        m_kMenuItems.Get(m_kSelected)->SetColor(m_kSelectedColor);
    }
}
//---------------------------------------------------------------------------
void NiMenu::SelectPrevious()
{
    NiTListIterator kNewSelected = m_kSelected;
    NiTListIterator kNewFirstItemShown = m_kFirstItemShown;


    // Increment the first item shown if the selected is at the end of the list
    if (m_kSelected == m_kFirstItemShown && 
        m_kMenuItems.GetHeadPos() != m_kFirstItemShown)
    {
        kNewFirstItemShown = m_kMenuItems.GetPrevPos(m_kFirstItemShown);
    }

    // Increment the selected 
    kNewSelected = m_kMenuItems.GetPrevPos(m_kSelected);


    // Keep incrementing until the end of the list or something selectable
    while((kNewSelected != NULL) &&
          (m_kMenuItems.Get(kNewSelected)->m_bSelectable == false))
    {
        // Increment the endi of the list if the new selected is at 
        // the end of what is visible
        if (kNewSelected == kNewFirstItemShown)
            kNewFirstItemShown = m_kMenuItems.GetPrevPos(kNewFirstItemShown);

        kNewSelected = m_kMenuItems.GetPrevPos(kNewSelected);
    }

    if (kNewSelected == NULL)
        kNewSelected = m_kMenuItems.GetTailPos();

    // Did the Selected item change?
    if (kNewSelected != NULL)
    {

        // Rebuild the list if the first Item shown has moved
        ResetVisibleItems(kNewFirstItemShown);

        // Clear the old selected color
        m_kMenuItems.Get(m_kSelected)->SetColor(m_kTextColor);

        m_kSelected = kNewSelected;

        // Set the selected text color
        m_kMenuItems.Get(m_kSelected)->SetColor(m_kSelectedColor);
    }
}
//---------------------------------------------------------------------------
void NiMenu::LoadMorePreviousIcon(const char* pcMorePreviousIconFileName)
{
    if (!pcMorePreviousIconFileName)
        return;

    NiSourceTexture* pkTexture = NiSourceTexture::Create(
            NiApplication::ConvertMediaFilename(pcMorePreviousIconFileName));
    m_spMorePreviousIcon = NiNew NiScreenTexture(pkTexture);
    m_spMorePreviousIcon->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);

    // Create the ScreenRect
    m_spMorePreviousIcon->AddNewScreenRect( m_sLocationY + m_sBorderWidth, 
        m_sLocationX + m_sBorderWidth, pkTexture->GetWidth(),
        pkTexture->GetHeight(), 0, 0);
}
//---------------------------------------------------------------------------
void NiMenu::LoadMoreNextIcon(const char* pcMoreNextIconFileName)
{
    if (!pcMoreNextIconFileName)
        return;
        
    NiSourceTexture* pkTexture = NiSourceTexture::Create(
            NiApplication::ConvertMediaFilename(pcMoreNextIconFileName));
   
    m_spMoreNextIcon = NiNew NiScreenTexture(pkTexture);
    m_spMoreNextIcon->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);

    // Create the ScreenRect
    m_spMoreNextIcon->AddNewScreenRect( m_sLocationY + m_sBorderWidth, 
        m_sLocationX + m_sBorderWidth, pkTexture->GetWidth(),
        pkTexture->GetHeight(), 0, 0);
}
//---------------------------------------------------------------------------
void NiMenu::LoadBackground(const char* pcBackgroundFileName)
{
    m_spBackground = NiNew NiScreenElements(
        NiNew NiScreenElementsData(false, true, 1));

    // We know that the polygon handle is zero and will use it directly in
    // the vertex, texture coordinate, and color assignments.
    m_spBackground->Insert(4);

    // The default values are not relevant because they will be reassigned
    // when the background is reset.
    m_spBackground->SetRectangle(0, 0.35f, 0.35f, 0.30f, 0.30f);
    m_spBackground->UpdateBound();

    // Select texture coordinates for the full texture image.
    m_spBackground->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);

    // Use the background color for vertex colors.
    m_spBackground->SetColors(0, m_kBackgroundColor);

    if (pcBackgroundFileName && pcBackgroundFileName[0])
    {
        // texture for screen space polygon
        NiTexturingProperty* pkTexProp = NiNew NiTexturingProperty(
            NiApplication::ConvertMediaFilename(pcBackgroundFileName));

        pkTexProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
        pkTexProp->SetBaseFilterMode(NiTexturingProperty::FILTER_BILERP);
        m_spBackground->AttachProperty(pkTexProp);
    }

    // use vertex colors and alpha blending
    NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
    NiVertexColorProperty* pVertex = NiNew NiVertexColorProperty;
    pVertex->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
    pVertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
    m_spBackground->AttachProperty(pVertex);

    pAlpha->SetAlphaBlending(true);
    pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
    pAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
    m_spBackground->AttachProperty(pAlpha);

    m_spBackground->UpdateProperties();
    m_spBackground->Update(0.0f);

    // add the background
    m_pkScreenElements->AddFirstEmpty(m_spBackground);
}
//---------------------------------------------------------------------------
void NiMenu::ResetMorePreviousIcon()
{
    if (!m_spMorePreviousIcon)
        return;

    // Set the Rectangle properly
    unsigned short usWidth = m_spMoreNextIcon->GetScreenRect(0).m_usPixWidth;
    unsigned short usHeight = 
        m_spMoreNextIcon->GetScreenRect(0).m_usPixHeight;
        
    // Remove the Old Rectangle
    m_spMoreNextIcon->RemoveAllScreenRects();
    m_spMoreNextIcon->AddNewScreenRect( m_sLocationY + m_sBorderWidth, 
        m_sLocationX + m_sBorderWidth, usWidth, usHeight, 0, 0, 
        NiColorA::WHITE);
        
    m_spMoreNextIcon->MarkAsChanged(NiScreenTexture::RECTANGLE_COUNT_MASK);
}
//---------------------------------------------------------------------------
void NiMenu::ResetMoreNextIcon()
{
    if (!m_spMoreNextIcon)
        return;

    NiTListIterator kCurrent = m_kFirstItemShown;

    // Loop in the list looking for the end or 
    // the maximum number of visible items
    for(int iLoop = 0; (kCurrent != m_kMenuItems.GetTailPos()) && 
            (iLoop < m_iMaxItemsShown);
        iLoop++, kCurrent = m_kMenuItems.GetNextPos(kCurrent))
    {
    }

    // Set the Rectangle properly
    unsigned short usWidth = m_spMoreNextIcon->GetScreenRect(0).m_usPixWidth;
    unsigned short usHeight = 
        m_spMoreNextIcon->GetScreenRect(0).m_usPixHeight;
        
    short sTop;
    short sLeft;
    GetMenuItemStartingLocation(kCurrent, sLeft, sTop);
        
    // Remove the Old Rectangle
    m_spMoreNextIcon->RemoveAllScreenRects();
    m_spMoreNextIcon->AddNewScreenRect( sTop, sLeft, usWidth, usHeight, 0, 0,
        NiColorA::WHITE);
        
    m_spMoreNextIcon->MarkAsChanged(NiScreenTexture::RECTANGLE_COUNT_MASK);
        
}
//---------------------------------------------------------------------------
bool NiMenu::GetVisible()
{
    return m_bVisible;
}
//---------------------------------------------------------------------------
void NiMenu::SetVisible(bool bNew)
{
    if (bNew == m_bVisible)
        return;

    m_bVisible = bNew;

    if (m_bVisible)
    {
        // Attach in the more previous icon
        if (m_spMorePreviousIcon && 
            !IsMenuItemVisible(m_kMenuItems.GetHeadPos()))
        {
            m_pkScreenTextures->AddFirstEmpty(m_spMorePreviousIcon);
        }

        // Attach in the more next icon
        if (m_spMoreNextIcon && !IsMenuItemVisible(m_kMenuItems.GetTailPos()))
        {
            m_pkScreenTextures->AddFirstEmpty(m_spMoreNextIcon);
        }

        // Attach the Background
        if (m_spBackground)
            m_pkScreenElements->AddFirstEmpty(m_spBackground);
    }
    else
    {
        // Detach in the more previous icon
        if (m_spMorePreviousIcon && 
            !IsMenuItemVisible(m_kMenuItems.GetHeadPos()))
        {
            m_pkScreenTextures->Remove(m_spMorePreviousIcon);
        }

        // Detach in the more next icon
        if (m_spMoreNextIcon && !IsMenuItemVisible(m_kMenuItems.GetTailPos()))
            m_pkScreenTextures->Remove(m_spMoreNextIcon);

        // Dettach the Background
        if (m_spBackground)
            m_pkScreenElements->Remove(m_spBackground);
    }

    // Loop through the possibly visible menu items and set there visibility
    NiTListIterator kIter = m_kFirstItemShown;

    for(int iLoop = 0; (iLoop < m_iMaxItemsShown) && (kIter != NULL);
        iLoop++, kIter = m_kMenuItems.GetNextPos(kIter))
    {
        m_kMenuItems.Get(kIter)->SetVisible(m_bVisible);
    }
}
//---------------------------------------------------------------------------
void NiMenu::ResetVisibleItems(NiTListIterator kNewFirstItemShown)
{
    // Rebuild the list if the first Item shown has moved
    if (m_kFirstItemShown == kNewFirstItemShown)
        return;

    SetVisible(false);
    
    m_kFirstItemShown = kNewFirstItemShown;
    
    SetVisible(true);

    // Loop through the possibly visible menu items and set there visibility
    NiTListIterator kIter = m_kFirstItemShown;

    for(int iLoop = 0; (iLoop < m_iMaxItemsShown) && (kIter != NULL);
        iLoop++, kIter = m_kMenuItems.GetNextPos(kIter))
    {
        AdjustScreenTextureLocation(kIter);
    }

    ResetMorePreviousIcon();
    ResetMoreNextIcon();
    ResetBackground();
}
//---------------------------------------------------------------------------
void NiMenu::ResetBackground()
{
    // Sum the Heights and get the Maximum Width
    // Add in the Next Icon
    unsigned short usWidth = m_spMoreNextIcon ? 
        m_spMoreNextIcon->GetScreenRect(0).m_usPixWidth : 0;
    unsigned short usHeight = m_spMoreNextIcon ? 
        m_spMoreNextIcon->GetScreenRect(0).m_usPixHeight : 0;

    // Add in the Previous Icon
    usHeight += m_spMorePreviousIcon ? 
        m_spMorePreviousIcon->GetScreenRect(0).m_usPixHeight : 0;
    usWidth = NiMax(usWidth, m_spMorePreviousIcon ? 
        m_spMorePreviousIcon->GetScreenRect(0).m_usPixWidth : 0);


    // Loop through the possibly visible menu items and summ in there values
    NiTListIterator kIter = m_kFirstItemShown;

    for(int iLoop = 0; (iLoop < m_iMaxItemsShown) && (kIter != NULL);
        iLoop++, kIter = m_kMenuItems.GetNextPos(kIter))
    {
            usHeight += m_kMenuItems.Get(kIter)->GetHeight();
            usWidth = NiMax(usWidth, m_kMenuItems.Get(kIter)->GetWidth());
    }

    float fLeft = (float)(m_sLocationX) / m_sBackBufferWidth;
    float fTop = (float)(m_sLocationY) / m_sBackBufferHeight;
    float fRight = (float)(m_sLocationX + usWidth + m_sBorderWidth + 
        m_sBorderWidth) / m_sBackBufferWidth;
    float fBottom = (float)(m_sLocationY + usHeight + m_sBorderWidth + 
        m_sBorderWidth) / m_sBackBufferHeight;
    // reset the vertices for the background
    m_spBackground->SetRectangle(0, fLeft, fTop, fRight - fLeft,
        fBottom - fTop);
    m_spBackground->UpdateBound();
}
//---------------------------------------------------------------------------
// Helper functions
//---------------------------------------------------------------------------
NiMenuItem* NiMenu::CreateItem_Bool(int iIdentifier, int iIndent, 
    void* pvData, const char* pszText, bool bInitialValue, NiColorA& kColor)
{
    NiMenuItemBooleanData* pkItem_Bool = 0;

    // Create a Float Data Menu Item
    NiMenuItemBooleanData::InitStruct kInit;
    kInit.m_iIdentifier = iIdentifier;
    kInit.m_bSelectable = true;
    kInit.m_iCharacterIndent = iIndent;
    kInit.m_pvData = pvData;
    kInit.m_pcText = pszText;
    kInit.m_pcTrueText = "On";
    kInit.m_pcFalseText = "Off";
    kInit.m_kColor = kColor; 
    kInit.m_pkScreenElements = m_pkScreenElements;
    kInit.m_pkScreenTextures = m_pkScreenTextures;
    kInit.m_bBooleanData = bInitialValue;
    kInit.m_eKeyboardToggle = m_eSelectKey;
    kInit.m_eGamePadToggle = m_eSelectButton;

    pkItem_Bool = NiNew NiMenuItemBooleanData(kInit);

    return (NiMenuItem*)pkItem_Bool;
}
//---------------------------------------------------------------------------
bool NiMenu::InsertItem_Bool(int iIdentifier, int iIndent, void* pvData, 
    const char* pszText, bool bInitialValue)
{
    NiMenuItem* pkItem = CreateItem_Bool(iIdentifier, iIndent, pvData, 
        pszText, bInitialValue, m_kTextColor);
    if (!pkItem)
        return false;

    AddMenuItemAtEnd(pkItem);

    return true;
}
//---------------------------------------------------------------------------
NiMenuItem* NiMenu::CreateItem_Float(int iIdentifier, int iIndent, 
    void* pvData, const char* pszText, float fInitialValue, float fMinValue, 
    float fMaxValue, float fIncrement, float fMultiplier, 
    unsigned int uiPrecision, NiColorA& kColor)
{
    NiMenuItemFloatData* pkItem_Float = 0;

    // Create a Float Data Menu Item
    NiMenuItemFloatData::InitStruct kInit;
    kInit.m_iIdentifier = iIdentifier;
    kInit.m_bSelectable = true;
    kInit.m_iCharacterIndent = iIndent;
    kInit.m_pvData = pvData;
    kInit.m_pcText = pszText;
    kInit.m_kColor = kColor;
    kInit.m_pkScreenElements = m_pkScreenElements;
    kInit.m_pkScreenTextures = m_pkScreenTextures;
    kInit.m_fFloatData = fInitialValue;
    kInit.m_fMultiplier = fMultiplier;
    kInit.m_fMinValue = fMinValue;
    kInit.m_fMaxValue = fMaxValue;
    kInit.m_fIncrementValue = fIncrement;
    kInit.m_uiPrecision = uiPrecision;
    // Keyboard
    kInit.m_eKeyboardIncrement = NiInputKeyboard::KEY_RIGHT;
    kInit.m_eKeyboardDecrement = NiInputKeyboard::KEY_LEFT;
    kInit.m_eKeyboardMultiplier = NiInputKeyboard::KEY_RSHIFT;
    // GamePad
    kInit.m_eGamePadIncrement = NiInputGamePad::NIGP_LRIGHT;
    kInit.m_eGamePadDecrement = NiInputGamePad::NIGP_LLEFT;
    kInit.m_eGamePadMultiplier = NiInputGamePad::NIGP_NONE;

    pkItem_Float = NiNew NiMenuItemFloatData(kInit);

    return (NiMenuItem*)pkItem_Float;
}
//---------------------------------------------------------------------------
bool NiMenu::InsertItem_Float(int iIdentifier, int iIndent, void* pvData, 
    const char* pszText, float fInitialValue, float fMinValue,
    float fMaxValue, float fIncrement, float fMultiplier,
    unsigned int uiPrecision)
{
    NiMenuItem* pkItem = CreateItem_Float(iIdentifier, iIndent, pvData, 
        pszText, fInitialValue, fMinValue, fMaxValue, fIncrement, 
        fMultiplier, uiPrecision, m_kTextColor);
    if (!pkItem)
        return false;

    AddMenuItemAtEnd(pkItem);

    return true;
}
//---------------------------------------------------------------------------
NiMenuItem* NiMenu::CreateItem_Int(int iIdentifier, int iIndent, 
    void* pvData, const char* pszText, int iInitialValue, int iMinValue, 
    int iMaxValue, int iIncrement, int iMultiplier, NiColorA& kColor)
{
    NiMenuItemIntData* pkItem_Int = 0;

    // Create a Int Data Menu Item
    NiMenuItemIntData::InitStruct kInit;

    kInit.m_iIdentifier = iIdentifier;
    kInit.m_bSelectable = true;
    kInit.m_iCharacterIndent = iIndent;
    kInit.m_pvData = pvData;
    kInit.m_pcText = pszText;
    kInit.m_kColor = kColor;
    kInit.m_pkScreenElements = m_pkScreenElements;
    kInit.m_pkScreenTextures = m_pkScreenTextures;
    kInit.m_iIntData = iInitialValue;
    kInit.m_iMultiplier = iMultiplier;
    kInit.m_iMinValue = iMinValue;
    kInit.m_iMaxValue = iMaxValue;
    kInit.m_iIncrementValue = iIncrement;
    // Keyboard
    kInit.m_eKeyboardIncrement = NiInputKeyboard::KEY_RIGHT;
    kInit.m_eKeyboardDecrement = NiInputKeyboard::KEY_LEFT;
    kInit.m_eKeyboardMultiplier = NiInputKeyboard::KEY_RSHIFT;
    // GamePad
    kInit.m_eGamePadIncrement = NiInputGamePad::NIGP_LRIGHT;
    kInit.m_eGamePadDecrement = NiInputGamePad::NIGP_LLEFT;
    kInit.m_eGamePadMultiplier = NiInputGamePad::NIGP_NONE;

    pkItem_Int = NiNew NiMenuItemIntData(kInit);
    return (NiMenuItem*)pkItem_Int;
}
//---------------------------------------------------------------------------
bool NiMenu::InsertItem_Int(int iIdentifier, int iIndent, void* pvData, 
    const char* pszText, int iInitialValue, int iMinValue, int iMaxValue, 
    int iIncrement, int iMultiplier)
{
    NiMenuItem* pkItem = CreateItem_Int(iIdentifier, iIndent, pvData, 
        pszText, iInitialValue, iMinValue, iMaxValue, iIncrement, 
        iMultiplier, m_kTextColor);
    if (!pkItem)
        return false;

    AddMenuItemAtEnd(pkItem);

    return true;
}

//---------------------------------------------------------------------------
bool NiMenu::FillMenuItems()
{
    // Base implementation does nothing!
    // Derived classes should fill in their items!
    return true;
}
//---------------------------------------------------------------------------
void NiMenu::RecalcLayout()
{
    // Loop through the possibly visible menu items and set there Location
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();

    for(int iLoop = 0; (kIter != NULL);
        iLoop++, kIter = m_kMenuItems.GetNextPos(kIter))
    {
        AdjustScreenTextureLocation(kIter);
    }

    ResetMorePreviousIcon();
    ResetMoreNextIcon();
    ResetBackground();
}
//---------------------------------------------------------------------------
NiTListIterator NiMenu::GetIterator(NiMenuItem* pkItem)
{
    NiTListIterator kIter = m_kMenuItems.GetHeadPos();
    while(kIter != NULL && m_kMenuItems.Get(kIter) != pkItem)
    {
        kIter = m_kMenuItems.GetNextPos(kIter);
    }

    return kIter;
}
//---------------------------------------------------------------------------
void NiMenu::SetParentMenu(NiMenu* pkMenu)
{
    m_pkParentMenu = pkMenu;
}
