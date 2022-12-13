#include "Top-KartPCH.h"

#include ".\fencemanager.h"
#include "NxCooking.h"
#include "TerrainManager.h"
#include "PhysXManager.h"
#include "./PhysKart/Stream.h"


CFenceManager::CFenceManager(NiAVObject * pkFenceNode, NxScene* pScene,NiPhysXScene* pNiScene)
{
	assert(pkFenceNode);

	TransformModelToWorldInPlace(pkFenceNode, false);

	NxScene* pkNxScene = pScene;// LevelManager::Get()->GetPhysXScene()->GetPhysXScene();

	NxActorDesc kActorDesc;
	kActorDesc.name = "Fence";



	if (pkFenceNode)
	{

		if (NiIsKindOf(NiTriBasedGeom, pkFenceNode))
		{
			NiTriBasedGeom* pkTriBaseGeom = (NiTriBasedGeom*)pkFenceNode;	

			if (NiIsKindOf(NiTriShape, pkTriBaseGeom))
			{
				NiTriShape* pkTriSp = (NiTriShape*)pkTriBaseGeom;
				unsigned short* triangles = pkTriSp->GetTriList();
				NiPoint3* pkVerts = pkTriSp->GetVertices();

				unsigned int uiPhysXNumVerts = pkTriSp->GetVertexCount();
				unsigned int uiPhysXNumFaces = pkTriSp->GetTriangleCount();

				NxVec3* pkPhysXMeshVerts = new NxVec3[uiPhysXNumVerts];
				NxU32* pkPhysXMeshFaces = new NxU32[uiPhysXNumFaces * 3];


				for (unsigned int usIndex = 0; usIndex < uiPhysXNumVerts; usIndex++)
				{
					NiPhysXTypes::NiPoint3ToNxVec3(*pkVerts, pkPhysXMeshVerts[usIndex]);

					++pkVerts;
				}

				for (unsigned int uiFaces = 0; uiFaces < uiPhysXNumFaces*3; uiFaces++)
				{
					pkPhysXMeshFaces[uiFaces] = (NxU32)(triangles[uiFaces]);
				}

				//defines the material of the terrain
				NxMaterialIndex* mtrIndices = new NxMaterialIndex[uiPhysXNumFaces];


				//terrain materials:
				NxMaterialDesc	m;
				m.restitution		= 0.0f;
				m.staticFriction	= 0.0f;
				m.dynamicFriction	= 0.0f;
				NxMaterialIndex someMert = pkNxScene->createMaterial(m)->getMaterialIndex();

				for (unsigned int i = 0; i < uiPhysXNumFaces; i++)
					mtrIndices[i] = someMert;


				NxTriangleMeshDesc kMeshDesc;
				kMeshDesc.numVertices =uiPhysXNumVerts;
				kMeshDesc.pointStrideBytes = sizeof(NxVec3);
				kMeshDesc.points = pkPhysXMeshVerts;
				kMeshDesc.numTriangles = uiPhysXNumFaces;
				kMeshDesc.triangles = pkPhysXMeshFaces;				
				kMeshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
				kMeshDesc.flags = 0;


				kMeshDesc.materialIndexStride			= sizeof(NxMaterialIndex);
				kMeshDesc.materialIndices				= mtrIndices;

				//kMeshDesc.heightFieldVerticalAxis		= NX_Z;
				//kMeshDesc.heightFieldVerticalExtent	= -1000.0f;

				NxInitCooking();
				NiPhysXMemStream kMemStream;
				bool bStatus = NxCookTriangleMesh(kMeshDesc, kMemStream);
				//bool bStatus = NxCookTriangleMesh(kMeshDesc, UserStream("c:\\tmp.bin", false));
				assert(bStatus && "FenceManager couldn't cook mesh");
				NxCloseCooking();

				// Seek the buffer back to the beginning for reading
				kMemStream.Reset();

				NiPhysXManager* pkPhysXManager = NiPhysXManager::GetPhysXManager();

				pkPhysXManager->WaitSDKLock();
				NxTriangleMesh* pkPhysXMesh = pkPhysXManager->m_pkPhysXSDK->createTriangleMesh(kMemStream);
				pkPhysXManager->ReleaseSDKLock();

				delete[] pkPhysXMeshVerts;
				delete[] pkPhysXMeshFaces;
				delete[] mtrIndices;

				NxTriangleMeshShapeDesc kTriMeshDesc;
				kTriMeshDesc.meshData = pkPhysXMesh;

				kActorDesc.shapes.pushBack(&kTriMeshDesc);

			}
		}
	}

	//    kActorDesc.group = LevelManager::GetTerrainGroup();
	//kActorDesc.userData = NiNew PhysXUserData(
	//	PhysXUserData::OBJECTTYPE_TERRAIN, 0);
	XUserData* pUd = new XUserData;
	pUd->_actorType = XUserData::AT_FENCE_MANUAL;
	kActorDesc.userData = pUd;
	
	//static object
	kActorDesc.body = NULL;
    
	m_pkFenceActor = pkNxScene->createActor(kActorDesc);

	m_pkFenceActor->setGroup(PhysXManager::CG_ENVIRONMENT);


	//NiNode* pkSomeBlocks = (NiNode*)pkNode->GetObjectByName("Protractor01");
	//if (pkSomeBlocks)
	//	CreateBlocks(pkSomeBlocks, pScene);


/***********************************/
	//NiTObjectArray<NiObjectPtr> kExtraObjects;
	//NiNodePtr spStaticRoot = LevelManager::OpenNifAndPrepack(filename, &kExtraObjects);


	//NiPhysXScenePtr spScene = 0;
	//NiPhysXScene* pkGlobalScene = pNiScene;

	//for (unsigned int ui = 0; ui < kExtraObjects.GetSize(); ui++)
	//{
	//	NiObject* pkObj = kExtraObjects.GetAt(ui);
	//	if (NiIsKindOf(NiPhysXScene, pkObj))
	//	{
	//		spScene = (NiPhysXScene*)pkObj;

	//		// Create the scene
	//		spScene->SetSlaved(pkGlobalScene, NiPhysXTypes::NXMAT34_ID);
	//		spScene->CreateSceneFromSnapshot(0);

	//		break;
	//	}
	//}

}

CFenceManager::~CFenceManager(void)
{
	if (m_pkFenceActor)
	{
		if (m_pkFenceActor->userData)
		{
			XUserData* pUd = (XUserData*)m_pkFenceActor->userData;
			delete pUd;
			pUd = NULL;
		}
	}
}

void CFenceManager::CreateBlocks(const NiAVObject* pNode, NxScene* pScene)
{
	if (NiIsKindOf(NiNode, pNode))
	{
		NiNode* pParent = (NiNode*)pNode;
		NxActorDesc ActorDesc;

		for (unsigned int i = 0; i < pParent->GetChildCount(); i++)
		{
			if (NiIsKindOf(NiTriShape, pParent->GetAt(i)))
			{

				NiTriShape* pkTris = (NiTriShape*)pParent->GetAt(i);

				//NiTransform trans = pkTris->GetWorldTransform();


				NiFixedString strName = pkTris->GetRTTI()->GetName();

				//create a hull
				unsigned int nbVerts = pkTris->GetVertexCount();
				NiPoint3* pkNiVerts = pkTris->GetVertices();

				NxVec3* nxVerts = new NxVec3[nbVerts];
				for (unsigned int usIndex = 0; usIndex < nbVerts; usIndex++)
				{
					//NiPoint3 ptTemp = trans* (*pkNiVerts);
					NiPhysXTypes::NiPoint3ToNxVec3(*pkNiVerts, nxVerts[usIndex]);
					++pkNiVerts;
				}

				NxConvexMeshDesc convexDesc;
				convexDesc.numVertices			= nbVerts;
				convexDesc.pointStrideBytes		= sizeof(NxVec3);
				convexDesc.points				= nxVerts;
				convexDesc.flags				= NX_CF_COMPUTE_CONVEX|NX_CF_USE_LEGACY_COOKER;

				NxInitCooking();

				// Cooking from memory
				MemoryWriteBuffer buf;
				if(NxCookConvexMesh(convexDesc, buf))
				{

					NxConvexShapeDesc convexShapeDesc;
					convexShapeDesc.meshData = pScene->getPhysicsSDK().createConvexMesh(MemoryReadBuffer(buf.data));

					if(convexShapeDesc.meshData)
					{
						ActorDesc.shapes.pushBack(&convexShapeDesc);
					}
				}

				delete[] nxVerts;

				//~create a hull

			}

		}

		ActorDesc.body			= NULL;//&BodyDesc;
		ActorDesc.density		= 10.0f;
		ActorDesc.globalPose.t  = NxVec3(0,0,0);//pos;
		NxActor* sadness = pScene->createActor(ActorDesc);
		sadness->userData = (void*)0;
		//gPhysicsSDK->releaseTriangleMesh(*convexShapeDesc.meshData);

	}
}
void CFenceManager::TransformModelToWorldInPlace(NiAVObject* pkObj, 
														bool bNormals)
{
	if (NiIsKindOf(NiGeometry, pkObj))
	{
		NiGeometry* pkGeom = (NiGeometry*)pkObj;

		unsigned int uiVerts = pkGeom->GetVertexCount();
		NiPoint3* pkVerts = pkGeom->GetVertices();
		TransformPointsBasicInPlace(uiVerts, (float*)pkVerts, 
			&(pkGeom->GetWorldTransform()));

		if (bNormals)
		{
			NiPoint3* pkNormals = pkGeom->GetNormals();
			TransformVectorsBasicInPlace(uiVerts, (float*)pkNormals, 
				&(pkGeom->GetWorldTransform()));
		}
	}
	else if (NiIsKindOf(NiNode, pkObj))
	{
		NiNode* pkNode = (NiNode*)pkObj;

		for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
		{
			NiAVObject* pkChild = pkNode->GetAt(i);

			if (pkChild)
				TransformModelToWorldInPlace(pkChild, bNormals);
		}
	}
}
//---------------------------------------------------------------------------
void CFenceManager::TransformPointsBasicInPlace(unsigned short uiVerts,
													   float* pModel, const NiTransform* pXform)
{

	for (unsigned int i = 0; i < uiVerts; i++, pModel += 3)
	{
		NiPoint3 kPoint;
		kPoint.x = *pModel;
		kPoint.y = *(pModel + 1);
		kPoint.z = *(pModel + 2);

		kPoint = *pXform * kPoint;

		*pModel = kPoint.x;
		*(pModel + 1) = kPoint.y;
		*(pModel + 2) = kPoint.z;
	}

}
//---------------------------------------------------------------------------
void CFenceManager::TransformVectorsBasicInPlace(unsigned short uiVerts,
														float* pModel, const NiTransform* pXform)
{

	// Only use rotation transformation information as we do not need to 
	// scale or translate normals when transforming them.
	NiTransform kXFrom;
	kXFrom.m_fScale = 1.0f;
	kXFrom.m_Translate = NiPoint3::ZERO;
	kXFrom.m_Rotate = pXform->m_Rotate;

	for (unsigned int i = 0; i < uiVerts; i++, pModel += 3)
	{
		NiPoint3 kPoint;
		kPoint.x = *pModel;
		kPoint.y = *(pModel + 1);
		kPoint.z = *(pModel + 2);

		kPoint = kXFrom * kPoint;

		*pModel = kPoint.x;
		*(pModel + 1) = kPoint.y;
		*(pModel + 2) = kPoint.z;
	}

}
