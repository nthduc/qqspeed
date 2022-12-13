#include "Top-KartPCH.h"
#include ".\rmoperator.h"


#include "Utility.h"
#include <NiAnimation.h>
#include <NiApplication.h>
#include <NiMain.h>
#include <NiParticle.h>
#include <NiPhysX.h>
#include <NiMetricsLayer.h>

#if defined(_XENON)
#include <NiXenonRenderer.h>
#include <NiXenonRenderTargetGroupData.h>
#include <NiResolvableDepthStencilBuffer.h>
#elif defined(WIN32)
#include <NiDirectXVersion.h>
#include <NiDX9Renderer.h>
#endif

#if defined(WIN32)
#if !defined(_XBOX)
#include "Win32SoundManager.h"
#endif  //#if !defined(_XBOX)
#endif  //#if defined(WIN32)

#include <NiD3DShaderFactory.h>

#ifdef NI_USE_MEMORY_MANAGEMENT
#include <NiMemManager.h>
#endif

#if defined(WIN32)
#if defined(_DX8)
#include <NiDX8Renderer.h>
#elif defined (_DX9)
#include <NiDX9Renderer.h>
#elif defined(_XBOX)
#include <NiXBoxRenderer.h>
#endif
#endif // #if defined(WIN32)

bool RMOperator::ms_bUseShaders = true;

//---------------------------------------------------------------------------
NiNodePtr RMOperator::OpenNif(const char* pcNifName,
								NiTObjectArray<NiObjectPtr>* kExtraObjects)
{
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(pcNifName)))
		return NULL;

	// assumes specific NIF layout -- be careful
	assert(NiIsKindOf(NiNode, kStream.GetObjectAt(0)));
	NiNodePtr spNode = (NiNode*)kStream.GetObjectAt(0);

	// Add any extras
	if (kExtraObjects)
	{
		for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
		{
			kExtraObjects->AddFirstEmpty(kStream.GetObjectAt(ui));
		}
	}
	return spNode;
}
//---------------------------------------------------------------------------
NiNodePtr RMOperator::OpenNifAndPrepack(const char* pcNifName,
										  NiTObjectArray<NiObjectPtr>* kExtraObjects)
{
	NiNodePtr spNode = OpenNif(pcNifName, kExtraObjects);

	//	assert(spNode && "Could not load level NIF file\n");
	if (!spNode) {
		MessageBox(NULL, pcNifName, "ERROR, File Can't be find!", MB_OK);
		return NULL;
	}

	// Needed so that the StandardMaterial shader finds the properties.
	spNode->UpdateProperties();
	spNode->UpdateEffects();

	RecursivePrepack(spNode, true);
	RecursivePrepack(spNode, false);

	return spNode;
}
//---------------------------------------------------------------------------
void RMOperator::RecursiveRemoveMorph(NiAVObject* pkObject)
{
	NiGeomMorpherControllerPtr spMorph =
		NiGetController(NiGeomMorpherController, pkObject);
	if (spMorph)
		spMorph->SetTarget(0);

	if (NiIsKindOf(NiNode, pkObject))
	{
		NiNode* pkNode = (NiNode*)pkObject;
		for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
		{
			NiAVObject* pkChild = pkNode->GetAt(i);
			if (pkChild)
				RecursiveRemoveMorph(pkChild);
		}
	}
}
//---------------------------------------------------------------------------
void RMOperator::RecursiveAddToMorphList(NiAVObject* pkObject,
										   float fPhase)
{
	NiTimeController* pkController = pkObject->GetControllers();
	while (pkController)
	{
		float fSpan = pkController->GetEndKeyTime() - 
			pkController->GetBeginKeyTime();
		pkController->SetPhase(fSpan * fPhase);

		NiGeomMorpherController* pkMorph = 
			NiDynamicCast(NiGeomMorpherController, pkController);
		if (pkMorph)
		{
			m_kTreeMorphList.AddHead(pkMorph);
		}
		pkController = pkController->GetNext();
	}

	if (NiIsKindOf(NiNode, pkObject))
	{
		NiNode* pkNode = (NiNode*)pkObject;
		for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
		{
			NiAVObject* pkChild = pkNode->GetAt(i);
			if (pkChild)
				RecursiveAddToMorphList(pkChild, fPhase);
		}
	}
}
//---------------------------------------------------------------------------
void RMOperator::RecursivePrepack(NiAVObject* pkObject, bool bBLODOnly)
{
	if (pkObject->GetAppCulled())
		return;

	NiRenderer* pkRenderer = NiRenderer::GetRenderer();
	assert(pkRenderer);

	bool bDynamic = false;
#if defined(WIN32) && !defined(_XBOX)
	if (!(pkRenderer->GetFlags() & NiRenderer::CAPS_HARDWARESKINNING))
	{
		bDynamic = true;
	}
#endif  //#if defined(WIN32) && !defined(_XBOX)


	if (!bDynamic && bBLODOnly)
	{
		// handle extra skin instances held by bone lod controllers
		NiBoneLODController* pkBLOD = NiGetController(NiBoneLODController, 
			pkObject);
		if (pkBLOD)
		{
			NiTPrimitiveSet<NiTriBasedGeom*> kGeoms;
			NiTPrimitiveSet<NiSkinInstance*> kInstances;

			pkBLOD->GetSkinData(kGeoms, kInstances);

			assert (kGeoms.GetSize() == kInstances.GetSize());

			for (unsigned int ui = 0; ui < kGeoms.GetSize(); ui++)
			{
				// do not mark these skin instances as static, the geometry
				// data will be thrown away later
				NiSkinInstancePtr pkOriginalSkin = kGeoms.GetAt(ui)->
					GetSkinInstance();

				kGeoms.GetAt(ui)->SetSkinInstance(kInstances.GetAt(ui));
				pkRenderer->PrecacheGeometry(kGeoms.GetAt(ui), 0, 0);
				kGeoms.GetAt(ui)->SetSkinInstance(pkOriginalSkin);
			}
		}
	}
	else if (NiIsKindOf(NiGeometry, pkObject))
	{
#ifdef REMOVEPARTICLES
		if (NiIsKindOf(NiParticles, pkObject))
		{
			pkObject->GetParent()->DetachChild(pkObject);
			return;
		}
#endif
		// Search for morpher controllers
		NiTimeController* pkController;

		for (pkController = pkObject->GetControllers(); 
			pkController != NULL; pkController = pkController->GetNext())
		{
			if (NiIsKindOf(NiGeomMorpherController, pkController))
				bDynamic = true;
		}

		if (!NiIsKindOf(NiParticles, pkObject) && !bDynamic)
		{
			pkRenderer->PrecacheGeometry((NiGeometry*)pkObject, 0, NULL);
		}
	}

	if (NiIsKindOf(NiNode, pkObject))
	{
		NiNode* pkNode = (NiNode*)pkObject;
		for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
		{
			NiAVObject* pkChild = pkNode->GetAt(i);
			if (pkChild)
				RecursivePrepack(pkChild, bBLODOnly);
		}
	}
}
//---------------------------------------------------------------------------
#ifdef WIN32
void RMOperator::RecursiveAddToStaticGeometryGroup(
	NiAVObject* pkObject, NiGeometryGroup* pkGroup)   
{
	if (!(NiRenderer::GetRenderer()->GetFlags() & 
		NiRenderer::CAPS_HARDWARESKINNING))
	{
		return;
	}

	// handle extra skin instances held by bone lod controllers
	NiBoneLODController* pkBLOD = NiGetController(NiBoneLODController, 
		pkObject);
	if (pkBLOD)
	{
		NiTPrimitiveSet<NiTriBasedGeom*> kGeoms;
		NiTPrimitiveSet<NiSkinInstance*> kInstances;

		pkBLOD->GetSkinData(kGeoms, kInstances);

		assert (kGeoms.GetSize() == kInstances.GetSize());

		for (unsigned int ui = 0; ui < kGeoms.GetSize(); ui++)
		{
			NiGeometryGroupManager::GetManager()->AddObjectToGroup(
				pkGroup, kGeoms.GetAt(ui)->GetModelData(), 
				kInstances.GetAt(ui));
		}
	}

	NiGeometry* pkGeom = NiDynamicCast(NiGeometry, pkObject);
	if (pkGeom)
	{
		NiGeometryGroupManager::GetManager()->AddObjectToGroup(
			pkGroup, pkGeom->GetModelData(), pkGeom->GetSkinInstance());
	}


	// recurse
	NiNode* pkNode = NiDynamicCast(NiNode, pkObject);
	if (pkNode)
	{
		for (unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
			{
				RecursiveAddToStaticGeometryGroup(pkChild, pkGroup);
			}
		}
	}
}
#endif  //#if defined(WIN32)
//---------------------------------------------------------------------------
void RMOperator::LightObject(NiNode* pkNode)
{
}
//---------------------------------------------------------------------------
void RMOperator::RecursiveEnableParticles(NiAVObject* pkObject, 
											bool bEnable)
{
	NiParticleSystem* pkPSys = NiDynamicCast(NiParticleSystem, pkObject);
	if (pkPSys)
	{       
		if (bEnable)
		{
			NiTimeController::StartAnimations(pkObject,
				Scheduler::Get()->GetClock().GetTime());
		}
		else
		{
			pkPSys->ResetParticleSystem();
			pkPSys->Update(Scheduler::Get()->GetClock().GetTime());
			NiTimeController::StopAnimations(pkObject);
		}
	}

	// recurse
	NiNode* pkNode = NiDynamicCast(NiNode, pkObject);
	if (pkNode)
	{
		for (unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
			{
				RecursiveEnableParticles(pkChild, bEnable);
			}
		}
	}
}
//---------------------------------------------------------------------------
NiAVObject* RMOperator::GetObjectByType(NiAVObject* pkObject, 
										  const NiRTTI* pkRTTI)
{
	if (pkObject->IsKindOf(pkRTTI))
		return pkObject;

	else if (pkObject->IsKindOf(&NiNode::ms_RTTI))	
	{
		NiNode* pkNode = (NiNode*)pkObject;
		for (unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
			{
				NiAVObject* pkReturn = GetObjectByType(pkChild, pkRTTI);
				if (pkReturn)
					return pkReturn;
			}
		}
	}
	return 0;
}

void RMOperator::SetUseShaders(bool bUseShaders)
{
	RMOperator::ms_bUseShaders = bUseShaders;
}
//---------------------------------------------------------------------------
bool RMOperator::GetUseShaders()
{
	return RMOperator::ms_bUseShaders;
}