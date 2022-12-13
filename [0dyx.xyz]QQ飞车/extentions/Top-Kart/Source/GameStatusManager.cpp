#include "Top-KartPCH.h"

//#include "LevelManager.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "GameStatusManager.h"
#include "Play.h"
#include "Track\KartStatusManager.h"
const float GameStatusManager::ms_fMaxBulletTimeAmount = 10.0f;

//---------------------------------------------------------------------------
GameStatusManager::GameStatusManager() : m_eGameState(GameStatusManager::STATE_NONE),
    m_bWaitingForUpdate(false), m_bBulletTime(false), m_fBulletTimeAmount(10)
{
}
//---------------------------------------------------------------------------
bool GameStatusManager::Initialize()
{
    return true;
}
//---------------------------------------------------------------------------
GameStatusManager::~GameStatusManager()
{
}
//---------------------------------------------------------------------------
void GameStatusManager::RestartLevel()
{
	/*
	CSceneLevel * pkLevel = NxSceneMgr::Get()->GetActiveSceneLevel();


    
    m_bWaitingForUpdate = true;
    SetBulletTime(false,1.0f);
    SetBulletTimeAmount(ms_fMaxBulletTimeAmount);
    m_bFadeOutBulletTime = false;
	NxPlayMgr::GetHostPlay()->Restart();

	//if( NxGamePkgMgr::Get())
	//	NxGamePkgMgr::Get()->Restart();


    pkLevel->Restart();
    const float fRadius = 150.0f;

    */
}
//---------------------------------------------------------------------------
