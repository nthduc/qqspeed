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
#include "Top-KartPCH.h"
#include "CallbackStream.h"
#include "NifLoader.h"
#include <NiMain.h>
#include <NiAnimation.h>

void RecursiveSetAnimType(NiObjectNET * pkRoot, NiTimeController::AnimType Type = NiTimeController::APP_INIT)
{
	if(pkRoot == NULL)
		return;

	NiTimeController* pkController = pkRoot->GetControllers();
	while(pkController != NULL)
	{
		pkController->SetAnimType(Type);
		pkController = pkController->GetNext();
	}

	if(NiIsKindOf(NiAVObject, pkRoot))
	{
		NiAVObject* pkObj = (NiAVObject*) pkRoot;
		NiPropertyList* pkPropList = &(pkObj->GetPropertyList());
		NiTListIterator kIter = pkPropList->GetHeadPos();
		while(pkPropList != NULL && !pkPropList->IsEmpty()&& kIter)
		{
			NiProperty* pkProperty = pkPropList->GetNext(kIter);
			if(pkProperty)
			{
				RecursiveSetAnimType(pkProperty, Type);
			}
		}
	}

	if(NiIsKindOf(NiNode, pkRoot))
	{
		NiNode* pkNode = (NiNode*) pkRoot;
		for(unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
		{
			NiAVObject* pkObj = pkNode->GetAt(ui);
			RecursiveSetAnimType(pkObj, Type);
		}
		
		const NiDynamicEffectList* pkEffectList= &(pkNode->GetEffectList());
		NiTListIterator kIter = pkEffectList->GetHeadPos();
		while(pkEffectList != NULL && !pkEffectList->IsEmpty() && kIter)
		{
			NiDynamicEffect* pkEffect = pkEffectList->GetNext(kIter);
			if(pkEffect)
			{
				RecursiveSetAnimType(pkEffect, Type);
			}
		}
	}
}

CallbackStream::CallbackStream(INifLoaderEvent* pEvent, DWORD dwCookie):
m_pEvent(pEvent),
m_dwCookie(dwCookie)
{
	this->SetPriority(NiThread::ABOVE_NORMAL);
}

void CallbackStream::BackgroundLoadOnExit()
{
    assert(BackgroundLoadGetExitStatus());

	NiStream::LoadState kLoadState;
	NiStream::ThreadStatus eStatus = BackgroundLoadPoll(&kLoadState);

    unsigned int uiObjects = GetObjectCount();

    for (unsigned int i = 0; i < uiObjects; i++)
    {    
        NiObject* pkObject = GetObjectAt(i);

        if (NiIsKindOf(NiNode, pkObject))
        {
            NiNode* pkNode = (NiNode*) pkObject;
            pkNode->UpdateProperties();
            pkNode->UpdateEffects();
            Precache(pkNode);
			RecursiveSetAnimType(pkNode);
        }
    }
	if ( m_pEvent && m_pEvent->OnLoadExit(m_dwCookie, eStatus, (DWORD)this) )
	{
		return;
	}
}

//---------------------------------------------------------------------------
void CallbackStream::Precache(NiAVObject* pkObject)
{
	
    NiTimeController* pkCon;
    /*
    for (pkCon = pkObject->GetControllers(); pkCon != NULL;
    pkCon = pkCon->GetNext())
    {
        // This should be done in the art assets instead of in code.
        pkCon->SetCycleType(NiTimeController::LOOP);
    }
	*/

    if (NiIsKindOf(NiNode, pkObject))
    {
        NiNode* pkNode = (NiNode*) pkObject;

        for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
        {
            NiAVObject* pkChild = pkNode->GetAt(i);
            if (pkChild != NULL)
            {
                Precache(pkChild);
            }
        }
    }
    else if (NiIsKindOf(NiTriBasedGeom, pkObject))
    {
        // Precache static geometry data and free unneeded data.

        NiTriBasedGeom* pkGeom = (NiTriBasedGeom*) pkObject;

        // Don't precache software skinned object:

        NiSkinInstance* pkSkinInstance = pkGeom->GetSkinInstance();
        if (pkSkinInstance == NULL || 
            pkSkinInstance->GetSkinPartition() != NULL)
        {
            for (pkCon = pkGeom->GetControllers(); pkCon != NULL;
            pkCon = pkCon->GetNext())
            {
                // Dont't precache object with a controller that changes
                // vertex data:
                
                if (NiIsKindOf(NiUVController, pkCon) ||
                    NiIsKindOf(NiGeomMorpherController, pkCon))
                {
                    break;
                }
            }

            if (pkCon == NULL)
            {
                NiRenderer* pkRenderer = NiRenderer::GetRenderer();
                assert(pkRenderer != NULL);

                pkGeom->GetModelData()->SetConsistency(NiGeometryData::STATIC);

                pkRenderer->PrecacheGeometry(pkGeom, 0, 0);
            }
        }
    }
}
