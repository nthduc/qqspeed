

//---------------------------------------------------------------------------
inline bool Clock::IsRunning() const
{ 
    return m_bRunning; 
}
//---------------------------------------------------------------------------
inline float Clock::GetSystem() 
{ 
    Update(); 

    return m_fSystemTime; 
}
//---------------------------------------------------------------------------
inline float Clock::GetTime() const
{ 
    return m_fSimTime; 
}
//---------------------------------------------------------------------------
inline float Clock::GetUnScaledTime() const
{ 
    return m_fUnScaledSimTime; 
}
//---------------------------------------------------------------------------
inline unsigned int Clock::GetFrame() const
{ 
    return m_uiFrameCount; 
}
//---------------------------------------------------------------------------
inline float Clock::GetFrameStart() const
{ 
    return m_fFrameStart; 
}
//---------------------------------------------------------------------------
inline float Clock::GetFrameEnd() const
{ 
    return m_fFrameEnd; 
}
//---------------------------------------------------------------------------
inline float Clock::GetScale()
{
    return m_fScale;
}
//---------------------------------------------------------------------------
inline void Clock::SetScale(float fScale)
{
    m_fScale = fScale;
}
//---------------------------------------------------------------------------
inline void Clock::GetMinFrameTime(bool& bOn, float& fMinFrameTime)
{
    bOn = m_bFrameTimeLocked;
    m_fMinFrameTime = fMinFrameTime;
}
//---------------------------------------------------------------------------
inline void Clock::SetMinFrameTime(bool bOn, float fMinFrameTime)
{
    m_fMinFrameTime = fMinFrameTime;
    m_bFrameTimeLocked = bOn;
}
//---------------------------------------------------------------------------
inline float Clock::GetFrameTime()
{
    return m_fFrameTime;
}
//---------------------------------------------------------------------------
inline float Clock::GetConstantFrameLength()
{
    return m_fConstantFrameLength;
}
//---------------------------------------------------------------------------
inline void Clock::SetConstantFrameLength(float fLength)
{
    m_fConstantFrameLength = fLength;
}
//---------------------------------------------------------------------------
inline bool Clock::IsConstantLengthEnabled()
{
    return m_bConstantFrameLengthEnabled;
}
//---------------------------------------------------------------------------
inline void Clock::EnableConstantLength(bool bEnable)
{
    m_bConstantFrameLengthEnabled = bEnable;
}
//---------------------------------------------------------------------------

