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

#ifndef NIMENU_H
#define NIMENU_H

#include <NiScreenElements.h>
#include <NiScreenTexture.h>
#include <NiTPointerList.h>
#include <NiInputKeyboard.h>
#include <NiInputGamePad.h>

#include "NiMenuDefs.h"
#include "NiMenuItem.h"
class NiCamera;

class NiMenu : public NiMemObject
{
public:
    enum ReturnCode
    {
        RETURN_NONE = 0,
        RETURN_ITEM_SELECTED
    };

    class InitStruct
    {
    public:
        NiMenuControlMode m_eControlMethod;
        short m_sLocationX;
        short m_sLocationY;
        short m_sBorderWidth;
        short m_sBackBufferWidth;
        short m_sBackBufferHeight;
        NiScreenElementsArray* m_pkScreenElements; 
        NiScreenTextureArray* m_pkScreenTextures;
        int m_iMaxItemsShown; 
        NiColorA m_kTextColor; 
        NiColorA m_kSelectedColor;
        NiColorA m_kBackgroundColor;
        const char* m_pcMorePreviousIconFileName;
        const char* m_pcMoreNextIconFileName;
        const char* m_pcBackgroundFileName;
        // Key codes
        NiInputKeyboard::KeyCode m_ePreviousKey; 
        NiInputKeyboard::KeyCode m_eNextKey; 
        NiInputKeyboard::KeyCode m_eSelectKey; 
        NiInputKeyboard::KeyCode m_eOpenCloseKey;
        // GamePad buttons
        NiInputGamePad::Button m_ePreviousButton;
        NiInputGamePad::Button m_eNextButton;
        NiInputGamePad::Button m_eSelectButton; 
        NiInputGamePad::Button m_eOpenCloseButton;

        InitStruct();   
    };

protected:
    NiMenu();

    // Constructor
    NiMenu(InitStruct& kInit);

public:
    virtual ~NiMenu();

    void RecalcLayout();

    static NiMenu* Create(InitStruct& kInit);

    // Initializer
    bool Initialize(InitStruct& kInit);

    bool IsOpen();
    virtual void OpenMenu();
    virtual void CloseMenu();

    virtual void SetLocationX(short x) {m_sLocationX = x;};
    virtual void SetLocationY(short y) {m_sLocationY = y;};

    virtual short GetLocationX() {return m_sLocationX;};
    virtual short GetLocationY() {return m_sLocationX;};

    virtual int ProcessMenu(float fAccumTime){return RETURN_NONE;};
    virtual int Update(float fAccumTime, NiMenuItem*& pkItem);

    bool GetVisible();
    void SetVisible(bool bNew);

    // Create a basic text menu item
    void AddMenuItemAtEnd(int iIdentifier, bool bSelectable, 
        int iCharacterIndent, void* pvData, const char* pcText);
    void InsertMenuItemBefore(int iIdentifier, bool bSelectable, 
        int iCharacterIndent, void* pvData, const char* pcText,
        int iInsertionIdentifier);
    
    virtual void SelectNext();
    virtual void SelectPrevious();


    NiMenuItem* GetMenuItem(int iIdentifier);

    // Add a premade menu item
    void AddMenuItemAtEnd(NiMenuItem* pItem);
    bool AddMenuItemBefore(int iIdentifier, NiMenuItem* pItem);
    bool AddMenuItemAfter(int iIdentifier, NiMenuItem* pItem);

    void RemoveAll();
    bool RemoveItem(int iIdentifier);

    void SetText(int iIdentifier, const char* pcText);

    // Helper functions
    virtual bool InsertItem_Bool(int iIdentifier, int iIndent,
        void* pvData, const char* pszText, bool bInitialValue);
    virtual bool InsertItem_Float(int iIdentifier, int iIndent,
        void* pvData, const char* pszText, float fInitialValue, 
        float fMinValue = 0.0f, float fMaxValue = 4.0f, 
        float fIncrement = 0.25f, float fMultiplier = 1.0f,
        unsigned int uiPrecision = 2);
    virtual bool InsertItem_Int(int iIdentifier, int iIndent,
        void* pvData, const char* pszText, int iInitialValue, int iMinValue, 
        int iMaxValue, int iIncrement, int iMultiplier);

    // Override function to simplify menu creation
    virtual bool FillMenuItems();

    virtual void SetParentMenu(NiMenu* pkMenu);
protected:

    NiMenu* m_pkParentMenu;

    bool m_bOpen;

    NiMenuControlMode m_eControlMethod;
    bool m_bVisible;
    int m_iMaxItemsShown;
    short m_sLocationX;
    short m_sLocationY;
    short m_sBorderWidth;
    short m_sBackBufferWidth;
    short m_sBackBufferHeight;

    NiTPointerList <NiMenuItem*> m_kMenuItems;
    NiTListIterator m_kSelected;
    NiTListIterator m_kFirstItemShown;
    NiScreenElementsArray* m_pkScreenElements;
    NiScreenTextureArray* m_pkScreenTextures;
    NiScreenTexturePtr m_spMorePreviousIcon;
    NiScreenTexturePtr m_spMoreNextIcon;
    NiColorA m_kTextColor;
    NiColorA m_kSelectedColor;
    NiColorA m_kBackgroundColor;
    NiScreenElementsPtr m_spBackground;

    NiInputKeyboard::KeyCode m_ePreviousKey;
    NiInputKeyboard::KeyCode m_eNextKey;
    NiInputKeyboard::KeyCode m_eSelectKey;
    NiInputKeyboard::KeyCode m_eOpenCloseKey;

    NiInputGamePad::Button m_ePreviousButton;
    NiInputGamePad::Button m_eNextButton;
    NiInputGamePad::Button m_eSelectButton;
    NiInputGamePad::Button m_eOpenCloseButton;

    virtual void GetMenuItemStartingLocation(NiTListIterator kItem, short& sX, 
        short& sY);
    virtual void AdjustScreenTextureLocation(NiTListIterator kItem);

    bool IsMenuItemVisible(NiMenuItem* pkItem);
    bool IsMenuItemVisible(NiTListIterator kItem);

    bool IsMenuItemLastVisible(NiTListIterator kItem);

    void LoadMorePreviousIcon(const char* pcMorePreviousIconFileName);
    void LoadMoreNextIcon(const char* pcMoreNextIconFileName);
    void LoadBackground(const char* pcBackgroundFileName);

    void ResetMorePreviousIcon();
    void ResetMoreNextIcon();
    void ResetVisibleItems(NiTListIterator kNewFirstItemShown);
    virtual void ResetBackground();

    NiTListIterator GetIterator(NiMenuItem* pkItem);

    // Helper functions
    virtual NiMenuItem* CreateItem_Bool(int iIdentifier, int iIndent,
        void* pvData, const char* pszText, bool bInitialValue,
        NiColorA& kColor);
    virtual NiMenuItem* CreateItem_Float(int iIdentifier, int iIndent,
        void* pvData, const char* pszText, float fInitialValue,
        float fMinValue, float fMaxValue, float fIncrement, float fMultiplier, 
        unsigned int uiPrecision, NiColorA& kColor);
    virtual NiMenuItem* CreateItem_Int(int iIdentifier, int iIndent,
        void* pvData, const char* pszText, int iInitialValue, int iMinValue, 
        int iMaxValue, int iIncrement, int iMultiplier, NiColorA& kColor);
};

#endif