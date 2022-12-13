

#include "Scheduler.h"

//---------------------------------------------------------------------------
inline GameStatusManager::GameState GameStatusManager::GetGameState()
{
    return m_eGameState;
}
//---------------------------------------------------------------------------
inline void GameStatusManager::SetGameState(GameState eGameState)
{
    m_eGameState = eGameState;
}
//---------------------------------------------------------------------------
inline void GameStatusManager::SetWaitingForUpdate(bool bWait)
{
    m_bWaitingForUpdate = bWait;
}
//---------------------------------------------------------------------------
inline bool GameStatusManager::GetWaitingForUpdate()
{
    return m_bWaitingForUpdate;
}
//---------------------------------------------------------------------------
inline bool GameStatusManager::GetBulletTime()
{
    return m_bBulletTime;
}
//---------------------------------------------------------------------------
inline float GameStatusManager::GetBulletTimeEndStamp()
{
    return m_fEndBulletTimeStamp;
}
//---------------------------------------------------------------------------
inline float GameStatusManager::GetBulletTimeStartStamp()
{
    return m_fStartBulletTimeStamp;
}
//---------------------------------------------------------------------------
inline float GameStatusManager::GetBulletTimeAmount()
{
    return m_fBulletTimeAmount;
}
//---------------------------------------------------------------------------
inline void GameStatusManager::SetBulletTimeAmount(float fBulletTimeAmount)
{
    m_fBulletTimeAmount = fBulletTimeAmount;

    if (m_fBulletTimeAmount < 0)
        m_fBulletTimeAmount = 0;

    if (m_fBulletTimeAmount >= ms_fMaxBulletTimeAmount)
        m_fBulletTimeAmount = ms_fMaxBulletTimeAmount;
}
//---------------------------------------------------------------------------
inline bool GameStatusManager::GetFadeOutBulletTime()
{
    if (NiGetCurrentTimeInSec() - m_fEndBulletTimeStamp > 1.5f)
    {
        m_bFadeOutBulletTime = false;
    }

    return m_bFadeOutBulletTime;
}
//---------------------------------------------------------------------------
inline void GameStatusManager::SetBulletTime(bool bActive, float fScale)
{
    m_bBulletTime = bActive;
    
    if (m_bBulletTime)
    {
        m_bFadeOutBulletTime = false;
        Scheduler::Get()->GetClock().SetScale(fScale);
        m_fStartBulletTimeStamp = Scheduler::Get()->
            GetClock().GetUnScaledTime();
    }
    else
    {
        Scheduler::Get()->GetClock().SetScale(1.0f);
        m_bFadeOutBulletTime = true;
        m_fEndBulletTimeStamp = Scheduler::Get()->GetClock().GetUnScaledTime();
    }

}
//---------------------------------------------------------------------------
