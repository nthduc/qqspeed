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

#ifndef GAMESTATUSMANAGER_H
#define GAMESTATUSMANAGER_H

class GameStatusManager : public NiMemObject
{
public:

    enum GameState
    {
		STATE_NONE = 0,
        
		GST_IN_GAME = 1,
		GST_OUT_GAME = 2,

		GST_STAR_RACE	= (1 << 2) | GST_IN_GAME,
		GST_RACING		= (2 << 2) | GST_IN_GAME,
		GST_END_RACE	= (3 << 2) | GST_IN_GAME,

		GST_ROOM		= (1 << 2) | GST_OUT_GAME,
			
		MAX_GAME_STATES = 4,
    };

    GameStatusManager();
    ~GameStatusManager();

    bool Initialize();
    GameState GetGameState();
    void SetGameState(GameState eGameState);
    void RestartLevel();

    void SetWaitingForUpdate(bool bWait);
    bool GetWaitingForUpdate();

    bool GetFadeOutBulletTime();
    bool GetBulletTime();
    void SetBulletTime(bool bActive, float fScale);
    float GetBulletTimeEndStamp();
    float GetBulletTimeStartStamp();

    float GetBulletTimeAmount();
    void  SetBulletTimeAmount(float fBulletTimeAmount);

    const static float ms_fMaxBulletTimeAmount;

private:
    GameState m_eGameState;
    bool m_bWaitingForUpdate;
    bool m_bBulletTime;
    bool m_bFadeOutBulletTime;
    float m_fEndBulletTimeStamp;
    float m_fStartBulletTimeStamp;

    // The total amount of the the player is allowed to be active in
    // bullet time.
    float m_fBulletTimeAmount; 
};

#include "GameStatusManager.inl"

#endif // #ifndef GAMESTATUSMANAGER_H