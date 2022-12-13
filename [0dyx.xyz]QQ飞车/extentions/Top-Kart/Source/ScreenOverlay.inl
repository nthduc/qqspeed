

//---------------------------------------------------------------------------
inline ScreenOverlay* ScreenOverlay::Get()
{
    return ms_pkTheScreenOverlay;
}
//---------------------------------------------------------------------------
inline void ScreenOverlay::SetVisible(bool bShow)
{
	if (ms_pkTheScreenOverlay)
	{
		ms_pkTheScreenOverlay->m_bShow = bShow;
	}
}