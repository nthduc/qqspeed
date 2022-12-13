

#include "Top-KartPCH.h"

#include "MenuManager.h"
#include "NiMenuItemFloatData.h"
#include "NiMenuItemIntData.h"
#include "NiMenuItemBooleanData.h"
#include "../InputManager.h"
//#include "../LevelManager.h"
#include "../Scheduler.h"
#include "../MommaManager.h"
#include "../ProilingController.h"
#include "../NxSceneMgr.h"
#include "../SceneLevel.h"
//---------------------------------------------------------------------------
MenuManager* MenuManager::ms_pkTheMenuManager = 0;
//---------------------------------------------------------------------------
bool MenuManager::Create(NiScreenElementsArray* pkScreenElements,
    NiScreenTextureArray* pkScreenTextures)
{
    if (!ms_pkTheMenuManager)
    {
        ms_pkTheMenuManager = NiNew MenuManager;
        NiMenu::InitStruct kMenuInit;    
        kMenuInit.m_pkScreenElements = pkScreenElements;
        kMenuInit.m_pkScreenTextures = pkScreenTextures;
        kMenuInit.m_ePreviousButton = NiInputGamePad::NIGP_LUP;
        kMenuInit.m_eNextButton = NiInputGamePad::NIGP_LDOWN;
        kMenuInit.m_eSelectButton = NiInputGamePad::NIGP_RDOWN;
        kMenuInit.m_eOpenCloseButton = NiInputGamePad::NIGP_NONE;
        kMenuInit.m_ePreviousKey = NiInputKeyboard::KEY_UP;
        kMenuInit.m_eNextKey = NiInputKeyboard::KEY_DOWN;
        kMenuInit.m_eSelectKey = NiInputKeyboard::KEY_RETURN;
        kMenuInit.m_eOpenCloseKey = NiInputKeyboard::KEY_NOKEY;
        kMenuInit.m_kTextColor = NiColorA(1.0f, 1.0f, 1.0f, 1.0f);
        kMenuInit.m_kBackgroundColor = NiColorA(0.0f, 0.0f, 0.0f, 0.7f);
        kMenuInit.m_iMaxItemsShown = 35;
        kMenuInit.m_sLocationX = 
            (short)((float)NiRenderer::GetRenderer()->
            GetDefaultRenderTargetGroup()->GetWidth(0) 
            * 0.60f);
#ifndef WIN32
        // If on a console, place menu inside the display safe-zone.
        kMenuInit.m_sLocationY = (short)((float)NiRenderer::GetRenderer()->
            GetDefaultRenderTargetGroup()->GetHeight(0) 
            * 0.05f);
#else
        kMenuInit.m_sLocationY = 0;
#endif

        kMenuInit.m_sBackBufferHeight = 
            NiRenderer::GetRenderer()->GetDefaultRenderTargetGroup()->
            GetHeight(0);
        kMenuInit.m_sBackBufferWidth = 
            NiRenderer::GetRenderer()->GetDefaultRenderTargetGroup()->
            GetWidth(0);

        ms_pkTheMenuManager->m_pkMenu = NiMenu::Create(kMenuInit);
        if (!ms_pkTheMenuManager->m_pkMenu)
        {
            NiOutputDebugString("MenuManager::Create -- "
                "Menu Initialization Failed\n");
            Destroy();
            return false;
        }

        ms_pkTheMenuManager->m_pkMenu->
            AddMenuItemAtEnd(MENUID_QUIT, true, 0, 0, "Quit");

        ms_pkTheMenuManager->m_pkMenu->InsertItem_Bool(MENUID_PAUSE, 0,
            0, "Pause", false);
      
        ms_pkTheMenuManager->m_pkMenu->InsertItem_Bool(
            MENUID_FRAMERATE, 0, 0, "Framerate", false);
   
        ms_pkTheMenuManager->m_pkMenu->InsertItem_Bool(
            MENUID_SOUND, 0, 0, "Sound", true);

		ms_pkTheMenuManager->m_pkMenu->InsertItem_Bool(
			MENUID_PROFLIE, 0, 0, "Profile", false);


        ms_pkTheMenuManager->m_pkMenu->SetVisible(false);

    }
    return true;
}
//---------------------------------------------------------------------------
void MenuManager::UpdateChangedValues()
{
//  ((NiMenuItemBooleanData*)m_pkMenu->GetMenuItem(MENUID_PAUSE))->
//      SetBooleanData(!Scheduler::Get()->IsRunning());

    //((NiMenuItemIntData*)m_pkMenu->GetMenuItem(MENUID_NUMBABIES))->
    //    SetIntData(LevelManager::GetFlockManager()->GetNumBabies());
}
//---------------------------------------------------------------------------
void MenuManager::Destroy()
{
    NiDelete ms_pkTheMenuManager;
    ms_pkTheMenuManager = 0;
}
//---------------------------------------------------------------------------
MenuManager::MenuManager() : m_pkMenu(0) 
{
}
//---------------------------------------------------------------------------
MenuManager::~MenuManager()
{
    NiDelete m_pkMenu;
}
//---------------------------------------------------------------------------
void MenuManager::UpdateMenu(float fTime)
{
	PROFILE("UpdateMenu");

    NiInputKeyboard* pkKeyboard = InputManager::Get()->GetInputSystem()->
        GetKeyboard();

    // Check for the keyboard exists
    if (pkKeyboard)
    {
        if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_ESCAPE) &&
            ( pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_LSHIFT) ||
            pkKeyboard->KeyIsDown(NiInputKeyboard::KEY_RSHIFT) ) )
        {
            m_pkMenu->SetVisible(!m_pkMenu->GetVisible());
        }
    }

    NiInputGamePad* pkGamePad = InputManager::Get()->GetInputSystem()->
        GetGamePad(0,0);

    // Check the gamepad exists
    if (pkGamePad)
    {
        if (pkGamePad->ButtonWasPressed(NiInputGamePad::NIGP_SELECT))
        {
            m_pkMenu->SetVisible(!m_pkMenu->GetVisible());
        }
    }

    // Don't do anything if not visible
    if (!m_pkMenu->GetVisible())
    {
        return;
    }

    NiMenuItem* pkItem;
    if (m_pkMenu->Update(fTime, pkItem) != NiMenu::RETURN_NONE)
    {
        switch(pkItem->GetIdentifier())
        {
		
        case MENUID_QUIT: 
            NiApplication::ms_pkApplication->QuitApplication();
            break;
        case MENUID_FRAMERATE:
         // NxSceneMgr::Get()->GetCameraController()->ToggleFramerate();
            break;
        case MENUID_PAUSE: 
            {
            if (Scheduler::Get()->IsRunning())
                Scheduler::Get()->Stop();
            else
                Scheduler::Get()->Run();
            break;
            }
	   case MENUID_PROFLIE:
		   if(((NiMenuItemBooleanData*)pkItem)->GetBooleanData() == true)
			   NxSceneMgr::Get()->GetProfilingController()->Profiling("show");
		   else
			   NxSceneMgr::Get()->GetProfilingController()->Profiling("quit");
		   break;
        }
    }

    UpdateChangedValues();
    m_fLastUpdateTime = fTime;
}
//---------------------------------------------------------------------------
