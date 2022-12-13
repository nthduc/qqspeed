

//---------------------------------------------------------------------------
inline InputManager* InputManager::Get()
{
    return ms_pkTheInputManager;
}
//---------------------------------------------------------------------------
inline const float& InputManager::GetStrafeDir() const
{
    return m_kInputData.m_fStrafeDir;
}
//---------------------------------------------------------------------------
inline const NiPoint2& InputManager::GetRelativeDir() const
{
    return m_kInputData.m_kRelativeDir;
}
//---------------------------------------------------------------------------
inline const NiPoint2& InputManager::GetCorrectedDir() const
{ 
    return m_kInputData.m_kA1Dir;
}
//---------------------------------------------------------------------------
inline const NiPoint2& InputManager::GetCameraDir() const
{  
    return m_kInputData.m_kA2Dir;
}
//---------------------------------------------------------------------------
inline float InputManager::GetZoomFactor() const
{
    return m_kInputData.m_fZoomVel;
}
//---------------------------------------------------------------------------
inline NiInputSystem* InputManager::GetInputSystem() const
{
    return m_spInputSystem;
}
//---------------------------------------------------------------------------
inline void InputManager::Enable(bool bEnable)
{
//	m_pEventMgr->SetEnable(bEnable);
}
