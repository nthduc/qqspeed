#include "Top-KartPCH.h"
#include <NiApplication.h>
#include <NiFont.h>
#include <NiString2D.h>
#include "GameMain.h"
#include "Scheduler.h"
#include "CameraController.h"
#include "GameStatusManager.h"
#include "track/CheckEditor.h"
#include "ScreenOverlay.h"
#include "ScreenOverlayTask.h"
#include "ProilingController.h"
#include "Track/KartPlayback.h"
#include "PhysKart/NxAllVehicles.h"
#include "PlayerRank.h"
#include "MovePkgMgr.h"
#include "PowerController.h"
#include "BaseItem.h"


extern float g_fXUIScale;
extern float g_fYUIScale;
#define INTRO_CAMERA_ACTIVE

ScreenOverlay* ScreenOverlay::ms_pkTheScreenOverlay = 0;

//---------------------------------------------------------------------------
ScreenOverlay::ScreenOverlay() : m_uiNumScreenItems(2), m_bShow(true)
{
}

bool ScreenOverlay::Create()
{
    NiRenderer* pkRenderer = NiRenderer::GetRenderer();

    ms_pkTheScreenOverlay = NiNew ScreenOverlay();

    ms_pkTheScreenOverlay->m_uiSafeZoneWidth = 0;
    ms_pkTheScreenOverlay->m_uiSafeZoneHeight = 0;

    // Load HUD Font
    ms_pkTheScreenOverlay->m_spHUDFont = NiFont::Create(pkRenderer,
        NiApplication::ConvertMediaFilename("ScreenOverLay/HUD.nff"));
    if (!ms_pkTheScreenOverlay->m_spHUDFont)
    {
        NiOutputDebugString("Error: ScreenOverlay::Create() Failed\n");
        return false;
    }

    unsigned int uiScreenWidth = GameMain::GetScreenWidth();
    unsigned int uiScreenHeight = GameMain::GetScreenHeight();

    // Initialize 2D Strings 
	NiColorA kColor(1.0f, 1.0f, 1.0f, 1.0f);
	

    ms_pkTheScreenOverlay->m_kTimeString.m_spString/*m_spTimeString */= NiNew NiString2D(
        ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32, 
        "Time:", kColor, uiScreenWidth-370 - 
        ms_pkTheScreenOverlay->m_uiSafeZoneWidth, 
        5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight);
	ms_pkTheScreenOverlay->m_kTimeString.m_uiOriginX =  uiScreenWidth-370 - ms_pkTheScreenOverlay->m_uiSafeZoneWidth;
	ms_pkTheScreenOverlay->m_kTimeString.m_uiOriginY =  5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight;

    ms_pkTheScreenOverlay->m_kNumString.m_spString/*m_spNumString */= NiNew NiString2D(
        ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32, 
        "Time:", kColor, uiScreenWidth-320, 
        5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight);
	ms_pkTheScreenOverlay->m_kNumString.m_uiOriginX =  uiScreenWidth-320;
	ms_pkTheScreenOverlay->m_kNumString.m_uiOriginY =  5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight;

	ms_pkTheScreenOverlay->m_kTimeElapse1.m_spString/*m_spTimeElapseString1*/ = NiNew NiString2D(
        ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32, 
        "Time:", kColor, uiScreenWidth-250, 
        5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight);
	ms_pkTheScreenOverlay->m_kTimeElapse1.m_uiOriginX = uiScreenWidth-250;
	ms_pkTheScreenOverlay->m_kTimeElapse1.m_uiOriginY = 5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight;

    ms_pkTheScreenOverlay->m_kTimeElapse2.m_spString/*m_spTimeElapseString2*/ = NiNew NiString2D(
        ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32, 
        "Time:", kColor, uiScreenWidth-190, 
        5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight);
	ms_pkTheScreenOverlay->m_kTimeElapse2.m_uiOriginX = uiScreenWidth-190;
	ms_pkTheScreenOverlay->m_kTimeElapse2.m_uiOriginY = 5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight;

	ms_pkTheScreenOverlay->m_kTimeElapse3.m_spString/*m_spTimeElapseString3 */= NiNew NiString2D(
		ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32, 
		"Time:", kColor, uiScreenWidth-130, 
		5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight); 
	ms_pkTheScreenOverlay->m_kTimeElapse3.m_uiOriginX = uiScreenWidth-130;
	ms_pkTheScreenOverlay->m_kTimeElapse3.m_uiOriginY = 5 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight;
	

	ms_pkTheScreenOverlay->m_kLapsString.m_spString/*m_spLapString */= NiNew NiString2D(
		ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32,
		"LAPS: ", kColor,uiScreenWidth-360 - 
		ms_pkTheScreenOverlay->m_uiSafeZoneWidth, 
		45 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight);
	ms_pkTheScreenOverlay->m_kLapsString.m_uiOriginX = uiScreenWidth-360 - ms_pkTheScreenOverlay->m_uiSafeZoneWidth;
	ms_pkTheScreenOverlay->m_kLapsString.m_uiOriginY = 45 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight;

	//ms_pkTheScreenOverlay->m_kUdpRcvStatic.m_spString/*m_spUdpRcvStatic */= NiNew NiString2D(
	//	ms_pkTheScreenOverlay->m_spHUDFont, NiFontString::COLORED, 32,
	//	"UDP Recive :",kColor,uiScreenWidth-370 - 
	//	ms_pkTheScreenOverlay->m_uiSafeZoneWidth, 
	//	100 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight);
	//ms_pkTheScreenOverlay->m_kUdpRcvStatic.m_uiOriginX = uiScreenWidth-370 - ms_pkTheScreenOverlay->m_uiSafeZoneWidth;
	//ms_pkTheScreenOverlay->m_kUdpRcvStatic.m_uiOriginY = 100 + ms_pkTheScreenOverlay->m_uiSafeZoneHeight;


    // Create a FRAME_TASK to for the screen overlay so it can update the 
    // input from the player 
    ScreenOverlayTask* pkScreenOverlayTask = NiNew ScreenOverlayTask;
    if (!pkScreenOverlayTask)
        return false;
    
    Scheduler::Get()->Schedule(pkScreenOverlayTask);

    return true;
}
//---------------------------------------------------------------------------
void ScreenOverlay::Destroy()
{
    NiDelete ms_pkTheScreenOverlay;
}
//---------------------------------------------------------------------------
ScreenOverlay::~ScreenOverlay()
{ /*
    m_spNumString = 0;
    m_spTimeString = 0;
 
    m_spTimeElapseString1 = 0;
    m_spTimeElapseString2 = 0;
	m_spTimeElapseString3 = 0;
	m_spLapString = 0;*/
    m_spHUDFont = 0;
/*	m_spLapCount = 0;
	m_spUdpRcvStatic =0;*/
	m_ePrevState = GameStatusManager::STATE_NONE;
/*
	if(m_pkAccePanel)
		NiDelete m_pkAccePanel;
*/
//	CPlayerRankMgr::Destroy();
}
//---------------------------------------------------------------------------
void ScreenOverlay::Draw()
{
	if (!m_bShow)
		return;

    GameStatusManager* pkSM = NxSceneMgr::Get()->GetGameStatusManager();
    GameStatusManager::GameState eState = pkSM->GetGameState();

    // If the menu is waiting for the game to update the level, then display
    // the menu screen instead of a none updated game level
    if (pkSM->GetWaitingForUpdate())
        eState = m_ePrevState;

    switch (eState)
    {
		case GameStatusManager::GST_STAR_RACE:
			DrawHUD();
		break;

		case GameStatusManager::GST_RACING:
             DrawHUD();
        break;

    };

    m_ePrevState = pkSM->GetGameState();
}
//---------------------------------------------------------------------------
void ScreenOverlay::DrawHUD()
{
    NiRenderer* pkRenderer = NiRenderer::GetRenderer();
    unsigned int uiScreenWidth = GameMain::GetScreenWidth();
    unsigned int uiScreenHeight = GameMain::GetScreenHeight();
    pkRenderer->SetScreenSpaceCameraData();

    // Draw Time
    float fTimeLeft = CLevelManager::Get()->GetLevelTime();

    int iMin = (int)(fTimeLeft / 60.0f);
	m_kTimeElapse1.AdjustPosition(g_fXUIScale, g_fYUIScale);
    if (iMin < 10)
	{
		if(iMin < 0)
		{
			m_kTimeElapse1.m_spString/*m_spTimeElapseString1*/->sprintf("00:");
		}
		else
		{
			m_kTimeElapse1.m_spString/*m_spTimeElapseString1*/->sprintf("0%d:", iMin);
		}
	}
	else
	{
		m_kTimeElapse1.m_spString/*m_spTimeElapseString1*/->sprintf("%d", iMin);
	}

//    m_spTimeElapseString1->sprintf("%d:",iMin);
//    m_spTimeElapseString1->SetPosition(uiScreenWidth - 125 - m_uiSafeZoneWidth,
//        5 + m_uiSafeZoneHeight );
    m_kTimeElapse1.m_spString/*m_spTimeElapseString1*/->Draw(pkRenderer);

	m_kTimeElapse2.AdjustPosition(g_fXUIScale, g_fYUIScale);
    int iSecs = (int)fTimeLeft - (iMin * 60);
    if (iSecs < 10)
    {
        if (iSecs < 0)
			m_kTimeElapse2.m_spString/* m_spTimeElapseString2*/->sprintf("00:");
        else
			m_kTimeElapse2.m_spString/* m_spTimeElapseString2*/->sprintf("0%d:",iSecs);
    }
    else
    {
		m_kTimeElapse2.m_spString/* m_spTimeElapseString2*/->sprintf("%d:",iSecs);
    }
//    m_spTimeElapseString2->SetPosition(uiScreenWidth-125 - m_uiSafeZoneWidth, 
//       5 + m_uiSafeZoneHeight);

	int iMSec = int((fTimeLeft - (iMin * 60) - iSecs) * 100.0);
	m_kTimeElapse3.AdjustPosition(g_fXUIScale, g_fYUIScale);
	if(iMSec < 10)
	{
		if(iMSec < 0)
		{
			m_kTimeElapse3.m_spString/* m_spTimeElapseString3*/->sprintf("00");
		}
		else
		{
			m_kTimeElapse3.m_spString/* m_spTimeElapseString3*/->sprintf("0%d", iMSec);
		}
	}
	else
	{
		m_kTimeElapse3.m_spString/* m_spTimeElapseString3*/->sprintf("%d", iMSec);
	}
//	m_spTimeElapseString3->SetPosition(uiScreenWidth-125 - m_uiSafeZoneWidth, 
//		5 + m_uiSafeZoneHeight);

    m_kTimeElapse2.m_spString/* m_spTimeElapseString2*/->Draw(pkRenderer);
	m_kTimeElapse3.m_spString/* m_spTimeElapseString3*/->Draw(pkRenderer);
	m_kTimeString.AdjustPosition(g_fXUIScale, g_fYUIScale);
    m_kTimeString.m_spString/* m_spTimeString*/->Draw(pkRenderer);

	m_kLapsString.AdjustPosition(g_fXUIScale, g_fYUIScale);
	m_kLapsString.m_spString/*.m_spLapString*/->sprintf("LAPS : %d / %d",CLevelManager::Get()->GetLinks()->GetCurLaps(),
		CLevelManager::Get()->GetLinks()->GetTotalLaps());
	m_kLapsString.m_spString/*.m_spLapString*/->Draw(pkRenderer);

	NxSceneMgr::Get()->GetProfilingController()->ProfileDraw();
}

//---------------------------------------------------------------------------
void ScreenOverlay::DrawCarbarnMenu()
{
	
}

//---------------------------------------------------------------------------
void ScreenOverlay::DrawScore()
{
	
}
//---------------------------------------------------------------------------
