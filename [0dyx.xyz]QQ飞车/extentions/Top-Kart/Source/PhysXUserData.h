

#ifndef PHYSXUSERDATA_H
#define PHYSXUSERDATA_H

#include "NiMemObject.h"

class PhysXUserData : public NiMemObject
{
public:

    enum ObjectType
    {
     OBJECTTYPE_BABY,
     OBJECTTYPE_MOMMA,
     OBJECTTYPE_TERRAIN,
     OBJECTTYPE_ICEBLOCK,
     OBJECTTYPE_ICEPIECE,
     OBJECTTYPE_TREE,
     MAX_OBJECTYPES
    };

    ObjectType GetObjectType();
    void* GetUserData();
    PhysXUserData(ObjectType eType, void* pData);

protected:
    ObjectType m_eObjectType;
    void* m_pUserData;
};


#include "PhysXUserData.inl"

#endif // #ifndef PHYSXUSERDATA_H