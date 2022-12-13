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

#ifndef SCREENOVERLAY_H
#define SCREENOVERLAY_H

#include <NiSystem.h>
#include <NiFont.h>
#include <NiString2D.h>
#include "GameStatusManager.h"
#include "AcceleratePanel.h"

struct TextContainer
{
	TextContainer() : m_uiOriginX(0), m_uiOriginY(0), m_spString(NULL){}
	~TextContainer()
	{
		m_spString = NULL;
	}

	void AdjustPosition(float fXScale, float fYScale)
	{
		m_spString->SetPosition(fXScale * m_uiOriginX, fYScale * m_uiOriginY);
	}		

	unsigned int m_uiOriginX;
	unsigned int m_uiOriginY;
	NiString2DPtr m_spString;
};

class ScreenOverlay : public NiMemObject
{
public:

    ScreenOverlay();
    ~ScreenOverlay();

    static bool Create();
    static void Destroy();
	static void SetVisible(bool bShow);

	void Draw();
    static ScreenOverlay* Get();

private:
    void DrawHUD();		//绘制游戏中界面
	void DrawCarbarnMenu();	//绘制车库界面
    void DrawScore();	//绘制比赛结果分数等

	static ScreenOverlay* ms_pkTheScreenOverlay;

	GameStatusManager::GameState	m_ePrevState;
    unsigned int m_uiNumScreenItems;
    unsigned int m_uiSafeZoneWidth;
    unsigned int m_uiSafeZoneHeight;

	bool		 m_bShow;

    NiFontPtr m_spHUDFont;	

	TextContainer m_kTimeElapse1;
	TextContainer m_kTimeElapse2;
	TextContainer m_kTimeElapse3;
	TextContainer m_kNumString;
	TextContainer m_kTimeString;
	TextContainer m_kLapsString;
	TextContainer m_kLapCounts;

	TextContainer m_kUdpRcvStatic;
/*
    NiString2DPtr m_spTimeElapseString1;
    NiString2DPtr m_spTimeElapseString2;
	NiString2DPtr m_spTimeElapseString3;
    NiString2DPtr m_spNumString;
    NiString2DPtr m_spTimeString;
  	NiString2DPtr m_spLapString;
	NiString2DPtr m_spLapCount;

	NiString2DPtr m_spUdpRcvStatic;  
*/
//	CAcceleratePanel * m_pkAccePanel;
};

#include "ScreenOverlay.inl"

#endif