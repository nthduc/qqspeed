

//----------------------------------------------------------------------------
inline void ShadowGeometry::SetGroundSceneGraph(NiAVObject* pkGround)
{ 
    m_spGround = pkGround; 
}
//----------------------------------------------------------------------------
inline NiTriShape* ShadowGeometry::GetShadowGeometry() const
{ 
    return m_spShadowGeometry; 
}
//----------------------------------------------------------------------------
inline void ShadowGeometry::ForceRender()
{
    m_bForceRender = true;
}
//----------------------------------------------------------------------------
