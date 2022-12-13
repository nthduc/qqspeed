

//---------------------------------------------------------------------------
inline PhysXUserData::PhysXUserData(PhysXUserData::ObjectType eType, 
    void* pData)
{
    m_eObjectType = eType;
    m_pUserData = pData;
}
//---------------------------------------------------------------------------
inline PhysXUserData::ObjectType PhysXUserData::GetObjectType()
{
    return m_eObjectType;
}
//---------------------------------------------------------------------------
inline void* PhysXUserData::GetUserData()
{
    return m_pUserData;
}
//---------------------------------------------------------------------------