
#include "Top-KartPCH.h"

#include "CollisionManager.h"
#include <NiAVObject.h>

//---------------------------------------------------------------------------
void CollisionManager::AddCollider(NiAVObject* pkObject)
{
    assert(pkObject->GetExtraData("r"));

    m_kColliders.AddTail(pkObject);
}
//---------------------------------------------------------------------------
void CollisionManager::RemoveAllColliders()
{
    m_kColliders.RemoveAll();
}
//---------------------------------------------------------------------------
