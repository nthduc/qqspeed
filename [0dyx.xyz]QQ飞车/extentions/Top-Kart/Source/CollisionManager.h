// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <NiTPointerList.h>

class NiAVObject;

class CollisionManager : public NiMemObject
{
public:
    void AddCollider(NiAVObject* pkObject); // expects pkObject to have 
                                            // NiFloatExtraData named "r" 
                                            // that represents the collider's
                                            // radius
    const NiTPointerList<NiAVObject*>* GetColliders() const;
    void RemoveAllColliders();
private:
    NiTPointerList<NiAVObject*> m_kColliders;
};

#include "CollisionManager.inl"

#endif // #ifndef COLLISIONMANAGER_H