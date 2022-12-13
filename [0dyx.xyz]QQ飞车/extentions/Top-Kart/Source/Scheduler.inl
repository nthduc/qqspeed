//
// Based directly on the "Scheduling Game Events" Gem from 
// Game Programming Gems 3, which requires:
//
// Intel Open Source License for Game Scheduler Gem sample code
// Copyright (c) 2002, Intel Corporation.
// All rights reserved. 
//

//---------------------------------------------------------------------------
inline Scheduler* Scheduler::Get()
{
    return ms_pkTheScheduler;
}
//---------------------------------------------------------------------------
inline bool Scheduler::IsRunning() const
{ 
    return m_kClock.IsRunning(); 
}
//---------------------------------------------------------------------------
inline void Scheduler::Run() 
{ 
    m_kClock.Run(); 
}
//---------------------------------------------------------------------------
inline void Scheduler::Stop() 
{ 
    m_kClock.Stop(); 
}
//---------------------------------------------------------------------------
inline Clock& Scheduler::GetClock()
{
    return m_kClock;
}
//---------------------------------------------------------------------------
inline float Scheduler::GetFrameLength() const 
{
    return m_fFrameLength;
}
//---------------------------------------------------------------------------
