#include "Top-KartPCH.h"
#include "NiToNxTools.h"
#include "Stream.h"
#include "../FenceManager.h"
#include "../PhysXManager.h"

NiToNxTools::NiToNxTools(){}
NiToNxTools::~NiToNxTools(){}

void NiToNxTools::LoadStaticColliders(const char* szFilename, NiPhysXScene* pkPhysxScene, 
									   NiNode* pkStaticParent, PhysXManager* pManager, 
									   PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData)
{
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(szFilename))) 
	{
		return;
	}
	NiNode * pkNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	assert(pkNode);

	LoadStaticObjects(pkPhysxScene->GetPhysXScene(), pkNode, pkStaticParent, eGroup, userData);
}

void NiToNxTools::LoadKinematicColliders(const char* szFilename, NiPhysXScene* pkPhysxScene, 
									  NiNode* pkDynamicParent, PhysXManager* pManager
									  , PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData)
{
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(szFilename))) 
	{
		return;
	}
	NiNode * pkNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	assert(pkNode);

	LoadKinematicObjects(pkPhysxScene, pkNode, pkDynamicParent, pManager,eGroup, userData);
}

void NiToNxTools::LoadDynamicColliders(const char* szFilename, NiPhysXScene* pkPhysxScene, 
											  NiNode* pkDynamicParent, PhysXManager* pManager)
{
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(szFilename))) 
	{
		return;
	}
	NiNode * pkNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	assert(pkNode);

	LoadDynamicObjects(pkPhysxScene, pkNode, pkDynamicParent, pManager);
}


void NiToNxTools::LoadStaticObjects(NxScene* pScene, NiNode* pkNode, NiNode* pkParent
									, PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData)
{

	for (int i = 0; i < pkNode->GetChildCount(); i++)
	{
		//NiNode* pkNd = NiDynamicCast(NiNode, pkNode->GetAt(i));
		//CFenceManager::TransformModelToWorldInPlace(pkNd, false);
		//pkNd->Update(0.0f);
		NiTriShape* pTri = NiDynamicCast(NiTriShape, pkNode->GetAt(i));
		assert(pTri);

		NxActor * pActor = CreateConvexShapeFromTriShape(pScene, *pTri, 0);

		NiPoint3 niPos = pTri->GetTranslate();
		NxVec3 nxPos;
		NiPhysXTypes::NiPoint3ToNxVec3(niPos, nxPos);

		pActor->setGroup(eGroup);

		XUserData* pUd = new XUserData();
		*pUd = userData;
		pActor->userData = pUd;

		pActor->setGlobalPosition(nxPos);
	}
}

void NiToNxTools::LoadDynamicObjects(NiPhysXScene* pScene, NiNode* pkNode, NiNode* pkParent, PhysXManager* pManager)
{
	pkParent->AttachChild(pkNode);

	for (int i = 0; i < pkNode->GetChildCount(); i++)
	{
		NiNode* pkNd = NiDynamicCast(NiNode, pkNode->GetAt(i));
		NiTriShape* pTri = NiDynamicCast(NiTriShape, pkNd->GetAt(0));
		assert(pTri);

		NxActor * pActor = CreateConvexShapeFromTriShape(pScene->GetPhysXScene(), *pTri, 10);

		//pManager->AddDynamicPair(pkNd, pActor);

		//NiPhysXTransformDestPtr pkDest = NiNew NiPhysXTransformDest(pkNd, pActor, NULL);
		//pkDest->SetTarget(pkNd);
		//pkDest->SetActor(pActor);
		//pkDest->SetActive(true);

		//pScene->AddDestination(pkDest);
	}
}

void NiToNxTools::LoadKinematicObjects(NiPhysXScene* pScene, 
									   NiNode* pkNode, NiNode* pkParent, PhysXManager* pManager
									   , PhysXManager::COLLISIONGROUP eGroup, const XUserData& userData)
{
	//CFenceManager::TransformModelToWorldInPlace(pkNode, false);
	//pkNode->Update(0.0f);
	
	pkParent->AttachChild(pkNode);

	for (int i = 0; i < pkNode->GetChildCount(); i++)
	{
		NiNode* pkNd = NiDynamicCast(NiNode, pkNode->GetAt(i));
		NiTriShape* pTri = NiDynamicCast(NiTriShape, pkNd->GetAt(0));
		assert(pTri);

		NxActor * pActor = CreateConvexShapeFromTriShape(pScene->GetPhysXScene(), *pTri, 10);
		pActor->raiseBodyFlag(NX_BF_KINEMATIC);

		NiPhysXKinematicSrcPtr pkSrc = NiNew NiPhysXKinematicSrc(pkNd, pActor);

		pkSrc->SetActive(true);
		pkSrc->SetInterpolate(true);

		pScene->AddSource(pkSrc);

		pActor->setGroup(eGroup);
		XUserData* pUd = new XUserData();
		*pUd = userData;
		pActor->userData = pUd;
		//pManager->AddKinematicPair(pkNd, pActor);

		


		//pActor->setGlobalPosition(nxPos);

	}
}

NxActor* NiToNxTools::CreateConvexShapeFromTriShape(NxScene* pScene, 
											   const NiTriShape& triShape, float density)
{
	NxActor* actor = NULL;

	unsigned int uiPhysXNumVerts = triShape.GetVertexCount();
	NiPoint3* pkVerts = triShape.GetVertices();
	NxVec3* pkPhysXMeshVerts = new NxVec3[uiPhysXNumVerts];
	for (unsigned int usIndex = 0; usIndex < uiPhysXNumVerts; usIndex++)
	{
		NiPhysXTypes::NiPoint3ToNxVec3(*pkVerts, pkPhysXMeshVerts[usIndex]);
		++pkVerts;
	}

	// Create descriptor for convex mesh
	NxConvexMeshDesc convexDesc;
	convexDesc.numVertices			= uiPhysXNumVerts;
	convexDesc.pointStrideBytes		= sizeof(NxVec3);
	convexDesc.points				= pkPhysXMeshVerts;
	convexDesc.flags				= NX_CF_COMPUTE_CONVEX;

	NxConvexShapeDesc convexShapeDesc;
	convexShapeDesc.localPose.t		= NxVec3(0,0,0);

	NxInitCooking();

	// Cooking from memory
	MemoryWriteBuffer buf;

	bool status = NxCookConvexMesh(convexDesc, buf);
	NiPhysXManager* pkPhysXManager = NiPhysXManager::GetPhysXManager();
	pkPhysXManager->WaitSDKLock();
	convexShapeDesc.meshData = pkPhysXManager->m_pkPhysXSDK->createConvexMesh(MemoryReadBuffer(buf.data));
	pkPhysXManager->ReleaseSDKLock();

	NxBodyDesc bodyDesc;

	if (convexShapeDesc.meshData)
	{
		NxActorDesc actorDesc;
		actorDesc.shapes.pushBack(&convexShapeDesc);
		if (density)
		{
			actorDesc.body = &bodyDesc;
			actorDesc.density = density;
		}
		else
		{
			actorDesc.body = NULL;
		}
		//actorDesc.globalPose.t  = pos;
		actor = pScene->createActor(actorDesc);

	}
	delete[] pkPhysXMeshVerts;

	return actor;
}