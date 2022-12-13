

//---------------------------------------------------------------------------
inline NiAVObject* TerrainManager::GetGroundSceneGraph() const
{
    return m_spTerrainGraph;
}
//---------------------------------------------------------------------------
inline unsigned int LookupTerrainManager::GetNumGeysers() const
{
    return m_uiNumGeysers;
}
//---------------------------------------------------------------------------
inline NiPoint3& LookupTerrainManager::GetBlockReleasePt(
    const unsigned int uiN)
{
    assert(uiN < m_uiNumGeysers);
    return m_pkBlockReleasePts[uiN];
}
//---------------------------------------------------------------------------
