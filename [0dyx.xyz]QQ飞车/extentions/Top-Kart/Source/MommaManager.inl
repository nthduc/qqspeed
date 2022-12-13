
#include "NiCamera.h"
//---------------------------------------------------------------------------
inline void MommaManager::SetAutoModeTime(float fTime)
{
    m_fAutoModeTime = fTime;
}
//---------------------------------------------------------------------------
inline float MommaManager::GetAutoModeTime() const
{
    return m_fAutoModeTime;
}
//---------------------------------------------------------------------------
inline unsigned int MommaManager::GetMommaID() const
{
    return m_uiMommaID;
}
//---------------------------------------------------------------------------
inline unsigned int MommaManager::GetPadID() const
{
    return m_uiPadID;
}
//---------------------------------------------------------------------------
inline void MommaManager::DrawFootprints(NiCamera* pkCamera)
{
 //   m_pkDynaQuad->RenderImmediate(NiRenderer::GetRenderer());
}
//---------------------------------------------------------------------------
inline void MommaManager::Jump()
{
    if (m_bExcutingAction)
        return;
    m_bExcutingAction = true;
    m_bTriggerJump = true;
    m_ePostActionAnimation = m_spAM->GetTargetAnimation();
}
//---------------------------------------------------------------------------
inline void MommaManager::BreakIceAttack()
{
    if (m_bExcutingAction)
        return;
    m_bExcutingAction = true;
    m_bTriggerIceBreak = true;
    m_ePostActionAnimation = m_spAM->GetTargetAnimation();
}
//---------------------------------------------------------------------------
inline bool MommaManager::IsExcutingAction()
{
    return m_bExcutingAction;
}
//---------------------------------------------------------------------------

inline NiPoint3 MommaManager::GetSpawnPoint()
{
    return m_kSpawn;
}
//---------------------------------------------------------------------------