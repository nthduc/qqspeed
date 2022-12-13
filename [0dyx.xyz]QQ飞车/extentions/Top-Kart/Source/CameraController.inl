

//---------------------------------------------------------------------------
inline NiCamera* CameraController::GetCamera() const
{
    return m_spCamera;
}
//---------------------------------------------------------------------------
inline NiNode* CameraController::GetTranslationNode()
{
    return m_spCameraTranslate;
}
//---------------------------------------------------------------------------
inline void CameraController::SetTargetDist(float fDist)
{
    m_fZoom = fDist;
}
//---------------------------------------------------------------------------
inline void CameraController::ToggleFramerate()
{
    m_bFrameRateEnabled = !m_bFrameRateEnabled;
}
//---------------------------------------------------------------------------
