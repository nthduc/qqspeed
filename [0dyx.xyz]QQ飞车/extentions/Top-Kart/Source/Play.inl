
#include "NiCamera.h"
//---------------------------------------------------------------------------
inline void Play::SetAutoModeTime(float fTime)
{
    m_fAutoModeTime = fTime;
}
//---------------------------------------------------------------------------
inline float Play::GetAutoModeTime() const
{
    return m_fAutoModeTime;
}
//---------------------------------------------------------------------------
inline UINT Play::GetCharacterAnim()
{
	return m_spCharacterAM->GetTargetAnimation();
}
//---------------------------------------------------------------------------
inline UINT Play::GetKartAnim()
{
	return m_spKartAM->GetTargetAnimation();
}

//---------------------------------------------------------------------------
//inline unsigned int Play::GetMommaID() const
//{
//    return m_uiMommaID;
//}
//---------------------------------------------------------------------------
//inline unsigned int Play::GetPadID() const
//{
//    return m_uiPadID;
//}
//---------------------------------------------------------------------------
inline bool Play::IsExcutingAction()
{
    return m_bExcutingAction;
}
//---------------------------------------------------------------------------

inline NiPoint3 Play::GetSpawnPoint()
{
    return m_kPos;
}
//---------------------------------------------------------------------------
#ifndef _XCART_
inline CKart* Play::GetKart()
#else
inline XCart* Play::GetKart()
#endif
{
	return m_pKart;
}
