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

#include "NiMenu.h"
//---------------------------------------------------------------------------
inline MenuManager* MenuManager::Get()
{
    return ms_pkTheMenuManager;
}
//---------------------------------------------------------------------------
inline bool MenuManager::GetVisible() const
{
    return m_pkMenu->GetVisible();
}
//---------------------------------------------------------------------------
inline void MenuManager::SetMinUpdateDelta(float fDelta)
{
    m_fMinUpdateDelta = fDelta;
}
//---------------------------------------------------------------------------
inline float MenuManager::GetMinUpdateDelta() const
{
    return m_fMinUpdateDelta;
}
//---------------------------------------------------------------------------
