//
// Based directly on the "Scheduling Game Events" Gem from 
// Game Programming Gems 3, which requires:
//
// Intel Open Source License for Game Scheduler Gem sample code
// Copyright (c) 2002, Intel Corporation.
// All rights reserved. 
//

//----------------------------------------------------------------------------
inline void Task::SetType(Task::TaskType eType)
{
    m_eType = eType;
}
//----------------------------------------------------------------------------
inline Task::TaskType Task::GetType() const
{
    return m_eType;
}
//----------------------------------------------------------------------------
inline void Task::SetTimeParams(float fStart, float fPeriod, float fDuration)
{
    m_fStart = fStart;
    m_fPeriod = fPeriod;
    m_fDuration = fDuration;
    // Is this an infinite task?  0.0f duration is the flag to indicate that
    if (m_fDuration == 0.0f)
        m_fEnd = 0.0f;
    else
        m_fEnd = m_fStart + m_fDuration;
}
//----------------------------------------------------------------------------
inline void Task::GetTimeParams(float& fStart, float& fPeriod, 
    float& fDuration) const
{
    fStart = m_fStart;
    fPeriod = m_fPeriod;
    fDuration = m_fDuration;
}
//----------------------------------------------------------------------------
inline unsigned int Task::GetID() const
{
    return m_uiID;
}
//----------------------------------------------------------------------------
