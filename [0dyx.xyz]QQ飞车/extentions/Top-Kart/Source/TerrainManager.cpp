

#include "Top-KartPCH.h"

#include <NiApplication.h>
#include "TerrainManager.h"
//#include "LevelManager.h"

#include <NiPhysX.h>
#include <NxPhysics.h>
#include <NxCooking.h>
#include "./PhysKart/Stream.h"
#include "Utility.h"

#include "PhysXManager.h"
#include "TerrainEffectMgr.h"

//---------------------------------------------------------------------------
TerrainManager::~TerrainManager()
{
	/* */
	m_spTerrainGraph = 0;
}
//---------------------------------------------------------------------------
LookupTerrainManager::LookupTerrainManager(NiAVObject * pkNodeTerrain,
										   const char* pcShadowFilename,NxScene* pScene)
{
	assert(pkNodeTerrain);
	//    assert(pcShadowFilename);

	m_pkLookupTable = 0;
	m_pkBlockReleasePts = 0;	

	TransformModelToWorldInPlace(pkNodeTerrain, false);

	m_spTerrainGraph = pkNodeTerrain;
	NxScene* pkNxScene = pScene;// LevelManager::Get()->GetPhysXScene()->GetPhysXScene();

	NxActorDesc kActorDesc;
	kActorDesc.name = "Terrain";


	//pkNodeTerrain = pkNodeTerrain->GetAt(0);
	if (pkNodeTerrain)
	{
		//for (unsigned int i = 0; i < pkNodeTerrain->GetChildCount(); i++ )

		{
			if (NiIsKindOf(NiTriBasedGeom, pkNodeTerrain))
			{
				NiTriBasedGeom* pkTriBaseGeom = (NiTriBasedGeom*)pkNodeTerrain;


				//AppendMesh(&kActorDesc, pkTriBaseGeom);

				if (NiIsKindOf(NiTriShape, pkTriBaseGeom))
				{
					NiTriShape* pkTriSp = (NiTriShape*)pkTriBaseGeom;
					unsigned short* triangles = pkTriSp->GetTriList();
					NiPoint3* pkVerts = pkTriSp->GetVertices();

					unsigned int uiPhysXNumVerts = pkTriSp->GetVertexCount();
					unsigned int uiPhysXNumFaces = pkTriSp->GetTriangleCount();

					NxVec3* pkPhysXMeshVerts = new NxVec3[uiPhysXNumVerts];
					NxU32* pkPhysXMeshFaces = new NxU32[uiPhysXNumFaces * 3];

					//NxMat34 mat;
					//NxMat33 orient;
					//orient.setColumn(0, NxVec3(-1,0,0));
					//orient.setColumn(1, NxVec3(0,0,1));
					//orient.setColumn(2, NxVec3(0,1,0));
					//mat.M = orient;

					for (unsigned int usIndex = 0; usIndex < uiPhysXNumVerts; usIndex++)
					{
						NiPhysXTypes::NiPoint3ToNxVec3(*pkVerts, pkPhysXMeshVerts[usIndex]);

						//从max坐标系-> physx坐标系.....
						//pkPhysXMeshVerts[usIndex] = mat* pkPhysXMeshVerts[usIndex];
						++pkVerts;
					}

					for (unsigned int uiFaces = 0; uiFaces < uiPhysXNumFaces*3; uiFaces++)
					{
						pkPhysXMeshFaces[uiFaces] = (NxU32)(triangles[uiFaces]);
					}

					
					
					

					//defines the material of the terrain
					NxMaterialIndex* mtrIndices = new NxMaterialIndex[uiPhysXNumFaces];


					
					//terrain materials:
					/*
					NxMaterialDesc	m;
					m.restitution		= 0.3f;
					m.staticFriction	= 0.9f;
					m.dynamicFriction	= 0.9f;
					NxMaterialIndex someMert = pkNxScene->createMaterial(m)->getMaterialIndex();
					*/
					for (unsigned int i = 0; i < uiPhysXNumFaces; i++)
					{						
						mtrIndices[i] = TerrainEffectMgr::GetTriangleMaterial(i, pkTriSp);//someMert;
					}


					////debug
					//		for (int i = 0; i < uiPhysXNumFaces; i++)
					//		{
					//			char buf[64];
					//			sprintf(buf, "triangle: %d (%d, %d, %d) \n", i, pkPhysXMeshFaces[i*3], pkPhysXMeshFaces[i*3+1], pkPhysXMeshFaces[i*3+2]);
					//			NiOutputDebugString(buf);
					//		
					//		}
					////-debug

					NxTriangleMeshDesc kMeshDesc;
					kMeshDesc.numVertices =uiPhysXNumVerts;
					kMeshDesc.pointStrideBytes = sizeof(NxVec3);
					kMeshDesc.points = pkPhysXMeshVerts;
					kMeshDesc.numTriangles = uiPhysXNumFaces;
					kMeshDesc.triangles = pkPhysXMeshFaces;				
					kMeshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
					kMeshDesc.flags = 0;

					//kMeshDesc.heightFieldVerticalAxis = NX_Z;
					//kMeshDesc.heightFieldVerticalExtent	= -10000.0f;

					kMeshDesc.materialIndexStride			= sizeof(NxMaterialIndex);
					kMeshDesc.materialIndices				= mtrIndices;

					kMeshDesc.heightFieldVerticalAxis		= NX_Z;
					kMeshDesc.heightFieldVerticalExtent	= -1000.0f;

					NxInitCooking();
					NiPhysXMemStream kMemStream;
					bool bStatus = NxCookTriangleMesh(kMeshDesc, kMemStream);
					//bool bStatus = NxCookTriangleMesh(kMeshDesc, UserStream("c:\\tmp.bin", false));
					assert(bStatus && "TerrainManager couldn't cook mesh");
					NxCloseCooking();

					// Seek the buffer back to the beginning for reading
					kMemStream.Reset();

					NiPhysXManager* pkPhysXManager = NiPhysXManager::GetPhysXManager();

					pkPhysXManager->WaitSDKLock();
					NxTriangleMesh* pkPhysXMesh = pkPhysXManager->m_pkPhysXSDK->createTriangleMesh(kMemStream);
					pkPhysXManager->ReleaseSDKLock();

					//NiPhysXTriangleMesh* pkMesh = NiNew NiPhysXTriangleMesh;
					//NiFixedString kMeshName("Terrain");
					//pkMesh->SetName(kMeshName);
					//pkMesh->SetMesh(pkPhysXMesh);
					//pkPhysXManager->SetTriangleMesh(pkMesh);


					delete[] pkPhysXMeshVerts;
					delete[] pkPhysXMeshFaces;
					delete[] mtrIndices;

					NxTriangleMeshShapeDesc kTriMeshDesc;
					kTriMeshDesc.meshData = pkPhysXMesh;


					kActorDesc.shapes.pushBack(&kTriMeshDesc);


					//					return true;

				}
			}
		}
	}

	//    kActorDesc.group = LevelManager::GetTerrainGroup();
	XUserData* pUd = new XUserData;
	pUd->_actorType = XUserData::AT_TERRAIN;
	kActorDesc.userData = pUd;
	
	//static object
	kActorDesc.body = NULL;


	m_pkTerrainActor = pkNxScene->createActor(kActorDesc);

	m_pkTerrainActor->setGroup(PhysXManager::CG_ENVIRONMENT);

}
//---------------------------------------------------------------------------
LookupTerrainManager::~LookupTerrainManager()
{
	/*if(m_pkTerrainActor->userData)
		NiDelete (PhysXUserData*)m_pkTerrainActor->userData;*/
	//NiDelete[] m_pkLookupTable;
	SAFE_NIDELETE(m_pkLookupTable);
	//NiDelete[] m_pkBlockReleasePts;
	SAFE_NIDELETE(m_pkBlockReleasePts);

	if (m_pkTerrainActor)
	{
		if (m_pkTerrainActor->userData)
		{
			XUserData* pUd = (XUserData*)m_pkTerrainActor->userData;
			delete pUd;
			pUd = NULL;
		}
	}
}

bool LookupTerrainManager::AppendMesh(NxActorDesc* pNxActorDesc, const NiTriBasedGeom* pkTriBaseGeom)
{



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

		////debug
		//		for (int i = 0; i < uiPhysXNumFaces; i++)
		//		{
		//			char buf[64];
		//			sprintf(buf, "triangle: %d (%d, %d, %d) \n", i, pkPhysXMeshFaces[i*3], pkPhysXMeshFaces[i*3+1], pkPhysXMeshFaces[i*3+2]);
		//			NiOutputDebugString(buf);
		//		
		//		}
		////-debug

		NxTriangleMeshDesc kMeshDesc;
		kMeshDesc.numVertices =uiPhysXNumVerts;
		kMeshDesc.pointStrideBytes = sizeof(NxVec3);
		kMeshDesc.points = pkPhysXMeshVerts;
		kMeshDesc.numTriangles = uiPhysXNumFaces;
		kMeshDesc.triangles = pkPhysXMeshFaces;				
		kMeshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
		kMeshDesc.flags = 0;

		//kMeshDesc.heightFieldVerticalAxis = NX_Z;
		//kMeshDesc.heightFieldVerticalExtent	= -10000.0f;

		NxInitCooking();
		NiPhysXMemStream kMemStream;
		bool bStatus = NxCookTriangleMesh(kMeshDesc, kMemStream);
		//bool bStatus = NxCookTriangleMesh(kMeshDesc, UserStream("c:\\tmp.bin", false));
		assert(bStatus && "TerrainManager couldn't cook mesh");
		NxCloseCooking();

		// Seek the buffer back to the beginning for reading
		kMemStream.Reset();

		NiPhysXManager* pkPhysXManager = NiPhysXManager::GetPhysXManager();

		pkPhysXManager->WaitSDKLock();
		NxTriangleMesh* pkPhysXMesh = pkPhysXManager->m_pkPhysXSDK->createTriangleMesh(kMemStream);
		pkPhysXManager->ReleaseSDKLock();

		//NiPhysXTriangleMesh* pkMesh = NiNew NiPhysXTriangleMesh;
		//NiFixedString kMeshName("Terrain");
		//pkMesh->SetName(kMeshName);
		//pkMesh->SetMesh(pkPhysXMesh);
		//pkPhysXManager->SetTriangleMesh(pkMesh);


		delete[] pkPhysXMeshVerts;
		delete[] pkPhysXMeshFaces;

		NxTriangleMeshShapeDesc kTriMeshDesc;
		kTriMeshDesc.meshData = pkPhysXMesh;


		pNxActorDesc->shapes.pushBack(&kTriMeshDesc);

		return true;

	}
	return false;
}

//---------------------------------------------------------------------------
bool LookupTerrainManager::GetHeight(float fX, float fY, 
									 float &fHeight) const
{
	assert(m_pkLookupTable);

	bool bOnTerrain = true;

	if (fX < m_fMinX)
	{
		fX = m_fMinX;
		bOnTerrain = false;
	}
	else if (fX >= m_fMaxX)
	{
		fX = m_fMaxX - m_fOOBlockDimX;
		bOnTerrain = false;
	}
	if (fY < m_fMinY)
	{
		fY = m_fMinY;
		bOnTerrain = false;
	}
	else if (fY >= m_fMaxY)
	{
		fY = m_fMaxY - m_fOOBlockDimY;
		bOnTerrain = false;
	}


	float fXRel = (fX - m_fMinX) * m_fOOBlockDimX;
	float fYRel = (fY - m_fMinY) * m_fOOBlockDimY;

	unsigned short usXLow = (unsigned short)fXRel;
	unsigned short usYLow = (unsigned short)fYRel;
	unsigned short usXHi = usXLow + 1;
	unsigned short usYHi = usYLow + 1;

	float fXDecimal = fXRel - ((float)usXLow);
	float fYDecimal = fYRel - ((float)usYLow);
	float fOMXDecimal = 1.0f - fXDecimal;
	float fOMYDecimal = 1.0f - fYDecimal;

	assert(usYHi * m_usVertsX + usXHi < m_usVertsX * m_usVertsY);

	// bilerp
	fHeight = 
		fOMXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXLow].m_fZ +
		fOMXDecimal * fYDecimal * 
		m_pkLookupTable[usYHi * m_usVertsX + usXLow].m_fZ +
		fXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXHi].m_fZ +
		fXDecimal * fYDecimal * 
		m_pkLookupTable[usYHi * m_usVertsX + usXHi].m_fZ;

	return bOnTerrain;
}
//---------------------------------------------------------------------------
bool LookupTerrainManager::GetHeightAndNormal(float fX, float fY, 
											  float &fHeight, NiPoint3& kNormal) const
{
	assert(m_pkLookupTable);

	bool bOnTerrain = true;

	if (fX < m_fMinX)
	{
		fX = m_fMinX;
		bOnTerrain = false;
	}
	else if (fX >= m_fMaxX)
	{
		fX = m_fMaxX - 0.001f;
		bOnTerrain = false;
	}
	if (fY < m_fMinY)
	{
		fY = m_fMinY;
		bOnTerrain = false;
	}
	else if (fY >= m_fMaxY)
	{
		fY = m_fMaxY - 0.001f;
		bOnTerrain = false;
	}



	float fXRel = (fX - m_fMinX) * m_fOOBlockDimX;
	float fYRel = (fY - m_fMinY) * m_fOOBlockDimY;

	unsigned short usXLow = (unsigned short)fXRel;
	unsigned short usYLow = (unsigned short)fYRel;
	unsigned short usXHi = usXLow + 1;
	unsigned short usYHi = usYLow + 1;

	float fXDecimal = fXRel - ((float)usXLow);
	float fYDecimal = fYRel - ((float)usYLow);
	float fOMXDecimal = 1.0f - fXDecimal;
	float fOMYDecimal = 1.0f - fYDecimal;

	assert(usYHi * m_usVertsX + usXHi < m_usVertsX * m_usVertsY);

	// bilerp
	fHeight = 
		fOMXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXLow].m_fZ +
		fOMXDecimal * fYDecimal * 
		m_pkLookupTable[usYHi * m_usVertsX + usXLow].m_fZ +
		fXDecimal * fOMYDecimal *
		m_pkLookupTable[usYLow * m_usVertsX + usXHi].m_fZ +
		fXDecimal * fYDecimal *
		m_pkLookupTable[usYHi * m_usVertsX + usXHi].m_fZ;

	kNormal = 
		fOMXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXLow].m_kNormal +
		fOMXDecimal * fYDecimal *
		m_pkLookupTable[usYHi * m_usVertsX + usXLow].m_kNormal +
		fXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXHi].m_kNormal +
		fXDecimal * fYDecimal *
		m_pkLookupTable[usYHi * m_usVertsX + usXHi].m_kNormal;

	kNormal.Unitize();

	return bOnTerrain;
}
//---------------------------------------------------------------------------
bool LookupTerrainManager::GetHeightAndColor(float fX, float fY, 
											 float &fHeight, NiColorA& kColor) const
{
	assert(m_pkLookupTable);

	bool bOnTerrain = true;

	if (fX < m_fMinX)
	{
		fX = m_fMinX;
		bOnTerrain = false;
	}
	else if (fX >= m_fMaxX)
	{
		fX = m_fMaxX - m_fOOBlockDimX;
		bOnTerrain = false;
	}
	if (fY < m_fMinY)
	{
		fY = m_fMinY;
		bOnTerrain = false;
	}
	else if (fY >= m_fMaxY)
	{
		fY = m_fMaxY - m_fOOBlockDimY;
		bOnTerrain = false;
	}

	float fXRel = (fX - m_fMinX) * m_fOOBlockDimX;
	float fYRel = (fY - m_fMinY) * m_fOOBlockDimY;

	unsigned short usXLow = (unsigned short)fXRel;
	unsigned short usYLow = (unsigned short)fYRel;
	unsigned short usXHi = usXLow + 1;
	unsigned short usYHi = usYLow + 1;

	float fXDecimal = fXRel - ((float)usXLow);
	float fYDecimal = fYRel - ((float)usYLow);
	float fOMXDecimal = 1.0f - fXDecimal;
	float fOMYDecimal = 1.0f - fYDecimal;

	assert(usYHi * m_usVertsX + usXHi < m_usVertsX * m_usVertsY);

	// bilerp
	fHeight = 
		fOMXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXLow].m_fZ +
		fOMXDecimal * fYDecimal * 
		m_pkLookupTable[usYHi * m_usVertsX + usXLow].m_fZ +
		fXDecimal * fOMYDecimal *
		m_pkLookupTable[usYLow * m_usVertsX + usXHi].m_fZ +
		fXDecimal * fYDecimal *
		m_pkLookupTable[usYHi * m_usVertsX + usXHi].m_fZ;

	kColor = 
		fOMXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXLow].m_kColor +
		fOMXDecimal * fYDecimal *
		m_pkLookupTable[usYHi * m_usVertsX + usXLow].m_kColor +
		fXDecimal * fOMYDecimal * 
		m_pkLookupTable[usYLow * m_usVertsX + usXHi].m_kColor +
		fXDecimal * fYDecimal *
		m_pkLookupTable[usYHi * m_usVertsX + usXHi].m_kColor;

	return bOnTerrain;
}
//---------------------------------------------------------------------------
void LookupTerrainManager::TransformModelToWorldInPlace(NiAVObject* pkObj, 
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
void LookupTerrainManager::TransformPointsBasicInPlace(unsigned short uiVerts,
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
void LookupTerrainManager::TransformVectorsBasicInPlace(unsigned short uiVerts,
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
//---------------------------------------------------------------------------
float LookupTerrainManager::GetMaxX() const
{
	return m_fMaxX;
}
//---------------------------------------------------------------------------
float LookupTerrainManager::GetMaxY() const
{
	return m_fMaxY;
}
//---------------------------------------------------------------------------
float LookupTerrainManager::GetMinX() const
{
	return m_fMinX;
}
//---------------------------------------------------------------------------
float LookupTerrainManager::GetMinY() const
{
	return m_fMinY;
}
