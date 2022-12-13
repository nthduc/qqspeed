/********************************************************************
	作者:	　wincoqin
	时间:	  2007/04/18
    文件名:   JointCloth.cpp
	说明：     []
*********************************************************************/
// precompile header
#include "Top-KartPCH.h"
// libfile
#include "NiAVObject.h"

// userfile
#include "JointCloth.h"
static NxMat33 g_kRoateZ90;

CJointCloth * CJointCloth::CreateJointCloth()
{
	CJointCloth * pJointCloth = NULL;

	pJointCloth = new CJointCloth;
	
	pJointCloth->CreateStick();

	return pJointCloth;
}

CJointCloth::CJointCloth()
{
	m_unWidth	= 16;
	m_unHeight	= 1;
	m_fRadius	= 0.2f;

	m_pAttachedNode				= NULL;
	m_kAttachedNodePose.id();
	m_kAttachedNodeInvesePose.id();

	m_ppClothPatches   = NULL;

	m_spAttachedObject = NULL;
	m_spBindingObject  = NULL;
	m_spGeometry	   = NULL;
	
	g_kRoateZ90.id();
	g_kRoateZ90.rotZ(NxPi/2.f);
}

CJointCloth::~CJointCloth()
{
	NxScene * pScene = CLevelManager::Get()->GetPhysXManager()->GetPhyScene();

	if (!pScene)
		return;

	for (int i = 0; i < m_unWidth * m_unHeight; ++i)
	{
		pScene->releaseActor(*m_ppClothPatches[i]);
	}
}


NxActor* CJointCloth::CreatePatch(const NxVec3& pos, const NxVec3& patchDim, const NxReal density, NxScene * pScene)
{
	if (!pScene)
		return NULL;

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box 
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(patchDim.x,patchDim.y,patchDim.z);
	actorDesc.shapes.pushBack(&boxDesc);

	if (density)
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = density;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.globalPose.t = pos;
	return pScene->createActor(actorDesc);	
}



void CJointCloth::CreateMattress(const NxVec3& pos, const int width, const int height, const NxVec3& patchDim, NxScene * pScene)
{
	if (!pScene)
		return ;

	int i, j;

	m_ppClothPatches = new NxActor*[width*height];
	NxActor** patchPtr = m_ppClothPatches; 

	NxVec3 mattressOffset = pos + NxVec3(-width * patchDim.x, patchDim.y, -height * patchDim.z);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			NxVec3 patchPos;
			patchPos.x = i * 2 * patchDim.x;
			patchPos.y = patchDim.y;
			patchPos.z = j * 2 * patchDim.z;
			patchPos += mattressOffset;
			*patchPtr = CreatePatch(patchPos, patchDim, 0.1f, pScene);
			if (i == 0)
			{
				m_pAttachedNode = *patchPtr;
				m_pAttachedNode->raiseBodyFlag(NX_BF_KINEMATIC);
				m_pAttachedNode->raiseActorFlag(NX_AF_DISABLE_COLLISION );
			}
			if (i == width - 1)
			{
				m_pLastActor = *patchPtr;
				m_pLastActor->raiseActorFlag(NX_AF_DISABLE_COLLISION );
			}

			//			NxShape **patchShapes = (*patchPtr)->getShapes();
			//			patchShapes[0]->setGroup(1);
			//			gPhysicsSDK->setGroupCollisionFlag(1,1,false);
			patchPtr++;
		}
	}

	PatchLink** patchLinks = NULL;
	// Create Patch Links
	patchLinks = new PatchLink*[width*height];
	PatchLink** patchLinkPtr = patchLinks; 

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			NxVec3 patchPos;
			patchPos.x = i * 2 * patchDim.x;
			patchPos.y = patchDim.y;
			patchPos.z = j * 2 * patchDim.z;
			patchPos += mattressOffset;
			*patchLinkPtr = new PatchLink;
			(*patchLinkPtr)->xJoint = NULL;
			(*patchLinkPtr)->zJoint = NULL;
			// Hoizontal Spring
			if (i < width - 1)
			{
				NxActor* patch1 = m_ppClothPatches[i + j*width];
				NxActor* patch2 = m_ppClothPatches[i + j*width + 1];
				NxVec3 xJointPos = patchPos + NxVec3(patchDim.x,0,0); 
				(*patchLinkPtr)->xJoint = CreateClothSphericalJoint(patch1, patch2, xJointPos, NxVec3(0,1,0) , pScene);

				patch1->wakeUp(1e10);
				patch2->wakeUp(1e10);
			}
			// Vertical Spring
			if (j < height - 1)
			{
				NxActor* patch1 = m_ppClothPatches[i + j*width];
				NxActor* patch2 = m_ppClothPatches[i + (j+1)*width];
				NxVec3 zJointPos = patchPos + NxVec3(0,0,patchDim.z); 
				(*patchLinkPtr)->zJoint = CreateClothSphericalJoint(patch1, patch2, zJointPos, NxVec3(0,1,0) , pScene);

				patch1->wakeUp(1e10);
				patch2->wakeUp(1e10);
			}
			patchLinkPtr++;
		}
	}

	//	patches[width*(height-1)]->raiseBodyFlag(NX_BF_KINEMATIC);
	//	patches[width*height - 1]->raiseBodyFlag(NX_BF_KINEMATIC);
}

NxSphericalJoint* CJointCloth::CreateClothSphericalJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis, NxScene * pScene)
{
	if (!pScene)
		return NULL;
	NxSphericalJointDesc sphericalDesc;
	sphericalDesc.actor[0] = a0;
	sphericalDesc.actor[1] = a1;
	sphericalDesc.setGlobalAnchor(globalAnchor);
	sphericalDesc.setGlobalAxis(globalAxis);

	sphericalDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
	sphericalDesc.twistLimit.low.value = -0.0001f;//-(NxReal)0.025*NxPi;
	sphericalDesc.twistLimit.low.hardness = 0.5;
	sphericalDesc.twistLimit.low.restitution = 0.01f;//0.5;
	sphericalDesc.twistLimit.high.value = 0.0001f;//(NxReal)0.025*NxPi;
	sphericalDesc.twistLimit.high.hardness = 0.5;
	sphericalDesc.twistLimit.high.restitution = 0.01f;//0.5;

	sphericalDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
	sphericalDesc.swingLimit.value = 0.05f;//(NxReal)0.25*NxPi;
	sphericalDesc.swingLimit.hardness = 0.5;
	sphericalDesc.swingLimit.restitution = 0.01f;//0.5;

	sphericalDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
	sphericalDesc.twistSpring.spring = 0.5 * 100;
	sphericalDesc.twistSpring.damper = 1 * 100;

	sphericalDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
	sphericalDesc.swingSpring.spring = 0.5 * 100;
	sphericalDesc.swingSpring.damper = 1 * 100;

	//	sphericalDesc.flags |= NX_SJF_JOINT_SPRING_ENABLED;
	//	sphericalDesc.jointSpring.spring = 0.5;
	//	sphericalDesc.jointSpring.damper = 1;

	sphericalDesc.projectionDistance = (NxReal)0.015;
	sphericalDesc.projectionMode = NX_JPM_POINT_MINDIST;

	return (NxSphericalJoint*)pScene->createJoint(sphericalDesc);
}

NiTriShape * CJointCloth::CreateStickGeometry(const NxVec3 & kPos, unsigned int unKnotNum, float fStickLenth, float fHalfWidth, const char * pPicturePath)
{
	/*NiTriShapeData(unsigned short usVertices, NiPoint3* pkVertex,
		NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture,
		unsigned short usNumTextureSets, NiGeometryData::DataFlags eNBTMethod,
		unsigned short usTriangles, unsigned short* pusTriList);*/	

	unsigned short usVertices = (unKnotNum + 1) * 4;
	unsigned short usNumTextureSets = unKnotNum * 4;
	unsigned short usTriangles = unKnotNum * 8;

	NiPoint3 * pkVertex = NiNew NiPoint3[usVertices];
	NiPoint3 * pkNormal = NiNew NiPoint3[usVertices];
	NiPoint2 * pkTexture= NiNew NiPoint2[usVertices];
	float fStep = fStickLenth / unKnotNum;
	// 计算顶点, 法线和纹理坐标
	for (int i = 0, j = 0; i < unKnotNum + 1; ++i, j += 4)
	{
		// 一圈一圈去连这个咚咚
		// 0
		// 顶点
		pkVertex[j]   = NiPoint3(-fHalfWidth, -fHalfWidth, i * fStep);
		// 法线
		pkNormal[j]   = pkVertex[j];
		pkNormal[j].z = 0;
		pkNormal[j].Unitize();
		// 纹理坐标
		pkTexture[j]  = NiPoint2(i%2, 0);

		// 1
		// 顶点
		pkVertex[j+1] = NiPoint3(-fHalfWidth,  fHalfWidth, i * fStep);
		// 法线
		pkNormal[j+1]   = pkVertex[j+1];
		pkNormal[j+1].z = 0;
		pkNormal[j+1].Unitize();
		// 纹理坐标
		pkTexture[j+1]  = NiPoint2(i%2, 1);

		// 2
		// 顶点
		pkVertex[j+2] = NiPoint3( fHalfWidth,  fHalfWidth, i * fStep);
		// 法线
		pkNormal[j+2]   = pkVertex[j+2];
		pkNormal[j+2].z = 0;
		pkNormal[j+2].Unitize();
		// 纹理坐标
		pkTexture[j+2]  = NiPoint2(i%2, 0);

		// 3
		// 顶点
		pkVertex[j+3] = NiPoint3( fHalfWidth, -fHalfWidth, i * fStep);
		// 法线
		pkNormal[j+3]   = pkVertex[j+3];
		pkNormal[j+3].z = 0;
		pkNormal[j+3].Unitize();
		// 纹理坐标
		pkTexture[j+3]  = NiPoint2(i%2, 1);
	}
	
	// 计算三角形
	unsigned short * pkTriList = NiAlloc(unsigned short, usTriangles * 3);
	for (int i = 0, j = 0; i < unKnotNum; ++i)
	{
		// 第i层的三角形

		// -Y方向三角形, 
		// 第1个三角形
		pkTriList[j]     = i * 4;
		pkTriList[j + 1] = i * 4 + 3;
		pkTriList[j + 2] = i * 4 + 7;
		j += 3;
		// 第2个三角形
		pkTriList[j]     = i * 4;
		pkTriList[j + 1] = i * 4 + 7;
		pkTriList[j + 2] = i * 4 + 4;
		j += 3;

		// X方向三角形
		// 第1个三角形
		pkTriList[j]     = i * 4 + 3;
		pkTriList[j + 1] = i * 4 + 2;
		pkTriList[j + 2] = i * 4 + 6;
		j += 3;
		// 第2个三角形
		pkTriList[j]     = i * 4 + 3;
		pkTriList[j + 1] = i * 4 + 6;
		pkTriList[j + 2] = i * 4 + 7;
		j += 3;

		// Y方向三角形
		// 第1个三角形
		pkTriList[j]     = i * 4 + 2;
		pkTriList[j + 1] = i * 4 + 1;
		pkTriList[j + 2] = i * 4 + 5;
		j += 3;
		// 第2个三角形
		pkTriList[j]     = i * 4 + 2;
		pkTriList[j + 1] = i * 4 + 5;
		pkTriList[j + 2] = i * 4 + 6;
		j += 3;

		// +X方向三角形
		// 第1个三角形
		pkTriList[j]     = i * 4 + 1;
		pkTriList[j + 1] = i * 4 ;
		pkTriList[j + 2] = i * 4 + 4;
		j += 3;
		// 第2个三角形
		pkTriList[j]     = i * 4 + 1;
		pkTriList[j + 1] = i * 4 + 4;
		pkTriList[j + 2] = i * 4 + 5;
		j += 3;
	}


	NiTriShapeData * pkTriShapeData = NiNew NiTriShapeData(usVertices, pkVertex,
		pkNormal, NULL, pkTexture, 1, NiGeometryData::NBT_METHOD_NONE, usTriangles, pkTriList);

	NiTriShape * pkTriShape = NiNew NiTriShape(pkTriShapeData);

	NiSourceTexture * pkSourceTexture = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(pPicturePath));

	NiMaterialProperty* pkMaterialProp = NiNew NiMaterialProperty;
	pkMaterialProp->SetEmittance(NiColor(1.0f, 1.0f, 1.0f));
	pkMaterialProp->SetAlpha(0.8f);

	NiTexturingProperty::Map* pkMap = NiNew NiTexturingProperty::Map(
		pkSourceTexture, 0, NiTexturingProperty::WRAP_S_WRAP_T,
		NiTexturingProperty::FILTER_BILERP);

	NiTexturingProperty* pkTexProp = NiNew NiTexturingProperty;

	pkTexProp->SetBaseMap(pkMap);
	pkTexProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);

	pkTriShape->AttachProperty(pkMaterialProp);
	pkTriShape->AttachProperty(pkTexProp);

	pkTriShape->UpdateProperties();
	pkTriShape->UpdateNodeBound();
	pkTriShape->Update(0.0f);

	return pkTriShape;
}


void CJointCloth::CreateStick()
{
	unsigned int unKnotNum = 7;
	float		 fKnotLength = 0.02f;
	float		 fHalfWidth  = 0.005f;
	NxVec3		 kPos(0, 0, 0);
	NxVec3       kPathDim(fKnotLength, fHalfWidth, fHalfWidth);
	
	m_unWidth  = unKnotNum + 1;
	m_unHeight = 1;
	m_fRadius  = fHalfWidth; 

	m_spGeometry = CreateStickGeometry(kPos, unKnotNum, fKnotLength * unKnotNum, fHalfWidth, "stick.dds");

	NxScene * pScene = CLevelManager::Get()->GetPhysXManager()->GetPhyScene();

	CreateMattress(kPos, m_unWidth, m_unHeight, kPathDim, pScene);
}

void CJointCloth::AttachTo(NiAVObject * pObj)
{
	m_spAttachedObject = pObj;
}

void CJointCloth::BindObject(NiAVObject * pObj)
{
	m_spBindingObject = pObj;

	if (pObj && m_spGeometry) 
	{
		((NiNode*)((NiAVObject*)m_spBindingObject))->AttachChild(m_spGeometry);
	}
	else
	{
		assert(0);
	}
}

void CJointCloth::UnBind()
{
	if (m_spGeometry)
	{
		(NiDynamicCast(NiNode, m_spGeometry))->DetachParent();
	}
}

void CJointCloth::Update()
{
	NiNode * pkFather = m_spGeometry->GetParent();
	if (pkFather)
		UpdateClothPose(pkFather->GetWorldTransform());
	SynchronizeModel();
}

void CJointCloth::UpdateClothPose(const NiTransform& kTransform)
{
	NxMat34 kPose;
	NiPhysXTypes::NiTransformToNxMat34(kTransform.m_Rotate, kTransform.m_Translate, kPose);
	
	//kPose.t.z += 1;
	kPose.M *= g_kRoateZ90;
	m_pAttachedNode->moveGlobalPose(kPose);
	m_pLastActor->addForce(NxVec3(0, 0, 0.00002f));
}

void CJointCloth::UpdateAttachedNodeParam()
{
	m_kAttachedNodePose = m_pAttachedNode->getGlobalPose();

	if (!m_kAttachedNodePose.getInverse(m_kAttachedNodeInvesePose))
	{
		assert(0);
	}
}

void CJointCloth::UpdateKnotVertex(NxActor * pActor, NiPoint3 * pVertex)
{	
	NxMat34  kTargetPose= pActor->getGlobalPose();

	NxVec3 kLocalPosition = (m_kAttachedNodeInvesePose * kTargetPose.t);

	//NxMat33 kLocalToWorld = m_kAttachedNodePose.M * kTargetPose.M;
	// 计算相关的四个顶点的坐标
	NxVec3 kNxPointTemp(0, 0, 0);
	// 0
	kNxPointTemp = kLocalPosition + NxVec3(-m_fRadius, -m_fRadius, 0);
	kNxPointTemp = g_kRoateZ90 * kNxPointTemp;
	NiPhysXTypes::NxVec3ToNiPoint3(kNxPointTemp, pVertex[0]);

	// 1
	kNxPointTemp = kLocalPosition + NxVec3(-m_fRadius,  m_fRadius, 0);
	kNxPointTemp = g_kRoateZ90 * kNxPointTemp;
	NiPhysXTypes::NxVec3ToNiPoint3(kNxPointTemp, pVertex[1]);

	// 2
	kNxPointTemp = kLocalPosition + NxVec3( m_fRadius,  m_fRadius, 0);
	kNxPointTemp = g_kRoateZ90 * kNxPointTemp;
	NiPhysXTypes::NxVec3ToNiPoint3(kNxPointTemp, pVertex[2]);

	// 3
	kNxPointTemp = kLocalPosition + NxVec3( m_fRadius, -m_fRadius, 0);
	kNxPointTemp = g_kRoateZ90 * kNxPointTemp;
	NiPhysXTypes::NxVec3ToNiPoint3(kNxPointTemp, pVertex[3]);
}

void CJointCloth::SynchronizeModel()
{
	// 同步顶点
	NiPoint3 * pVertexes = m_spGeometry->GetVertices();
	unsigned int unVertexNum = m_spGeometry->GetVertexCount();
	assert(unVertexNum == (m_unWidth * 4));
	UpdateAttachedNodeParam();
	for (int i = 1; i < m_unWidth; ++i)
	{
		UpdateKnotVertex(m_ppClothPatches[i], pVertexes + i * 4);
	}

	m_spGeometry->GetModelData()->MarkAsChanged(NiGeometryData::VERTEX_MASK);
}