

//---------------------------------------------------------------------------
inline void GameMain::UpdateFrameRate()
{
    m_pkFrameRate->TakeSample();
    m_pkFrameRate->Update();
}
//---------------------------------------------------------------------------
inline unsigned int GameMain::GetScreenWidth()
{
    return ms_uiScreenWidth;
}
//---------------------------------------------------------------------------
inline unsigned int GameMain::GetScreenHeight()
{
    return ms_uiScreenHeight;
}
//---------------------------------------------------------------------------
inline void GameMain::ToggleShowAllTrackers()
{
    SetShowAllTrackers(!m_bShowAllTrackers);
}
//---------------------------------------------------------------------------
inline void GameMain::AddUpdateTime(float fTime)
{
    m_fUpdateTime += fTime;
}
//---------------------------------------------------------------------------
inline void GameMain::AddCullTime(float fTime)
{
    m_fCullTime += fTime;
}
//---------------------------------------------------------------------------
inline void GameMain::AddRenderTime(float fTime)
{
    m_fRenderTime += fTime;
}
//---------------------------------------------------------------------------
inline void GameMain::AddAITime(float fTime)
{
    m_fAITime += fTime;
}
//---------------------------------------------------------------------------
inline void GameMain::AddVisObjects(float fCount)
{
    m_fVisableObjects += fCount;
}
//---------------------------------------------------------------------------
inline void GameMain::AddAnimTime(float fTime)
{
    m_fAnimTime += fTime;
}
//---------------------------------------------------------------------------
inline CCustomCullingProcess & GameMain::GetCuller()
{
	return m_kCustomCuller;
}
inline NiCullingProcess & GameMain::GetDefaultCuller()
{
	return m_kCuller;
}
inline GameMain * GetMainApp()
{
	return (GameMain*)(NiApplication::ms_pkApplication);
}
inline bool GameMain::IsDeviceLost()
{
	return m_bOnlostDevice;
}
