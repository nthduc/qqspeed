

#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "NiMenu.h"
#include "NiMenuItem.h"
#include "NiMenuItemBooleanData.h"
#include "NiMenuItemFloatData.h"
#include "NiMenuItemIntData.h"
#include "NiMenuItemScaleData.h"

#include <NiScreenElements.h>
#include <NiScreenTexture.h>
class NiCamera;
class NiMenu;
class MenuManager : public NiMemObject
{
public:

    enum MenuID {
        MENUID_QUIT,
        MENUID_PAUSE,
        MENUID_FRAMERATE,
        MENUID_SOUND,
		MENUID_PROFLIE,
        
    };
    static bool Create(NiScreenElementsArray* pkScreenElements,
        NiScreenTextureArray* pkScreenTextures);
    static void Destroy();

    static MenuManager* Get();

    void UpdateMenu(float fTime);

    bool GetVisible() const;
    
    void SetMinUpdateDelta(float fDelta);
    float GetMinUpdateDelta() const;

private:
    MenuManager();
    ~MenuManager();
    void UpdateChangedValues();

    NiMenu* m_pkMenu;
    float m_fLastUpdateTime;
    float m_fMinUpdateDelta;

    static MenuManager* ms_pkTheMenuManager;
};

#include "MenuManager.inl"

#endif // #ifndef MENUMANAGER_H
