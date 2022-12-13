#include "Top-KartPCH.h"

#include ".\chassissimulator.h"

#define           GROUP_WHEELS              8
#define           GROUP_BASES               9

#define			  MIN_FLYHEIGHT				0.08

ChassisSimulator::ChassisSimulator(NxScene* pScene)//, NxVec3& pos)
{
	m_pNxScene = pScene;
	m_pActorChassis = NULL;
	m_pPillar = NULL;

	m_pMassTrans = NULL;

	m_bDriftPeriodFinished = true;

	CreateNoneBounceMaterial();

	InitNx();
}

ChassisSimulator::~ChassisSimulator(void)
{

}

void ChassisSimulator::InitNx()
{


	m_state = RS_INAIR;
	m_statePre = RS_INAIR;

	m_nDriftingState = NOT_DRIFTING;
	m_fDriftAngle = 0.0f;

	m_bIsFreezed = true;
	m_fOrientationEulerXPre = 0.0f;
	m_fOrientationEulerYPre = 0.0f;



	for(int i = 0; i < WHEELS; i++)
	{
		m_fHeightCur[i] = 0.0f;
		m_fHeightPre[i] = 0.0f;
		m_afZofWheels[i] = 0.0f;
		m_afDistanceToGround[i] = 0.0f;
	}
	m_fZofLowestWheel = 0.0f;
	m_nLowestWheel = 0;
	//m_vptSideTip[0] = (m_vBasePoints[1]+m_vBasePoints[2]) *0.5f;
	//m_vptSideTip[0].z = 0;

	//m_vptSideTip[1] = (m_vBasePoints[0]+m_vBasePoints[3]) *0.5f;
	//m_vptSideTip[1].z = 0;


	LoadSuspensionConfig();
	//InitSpecification();
	CreateBases();
	CreateChassis();
	CreateWheels();

	CreateSuspensions();
	CreatePillar();	

	for (int i = 0; i < WHEELS; ++i)
		ReconfigureSuspension(i);

	ReconfigurePillar(m_pPillar);

	SetClissionable();

	FreezeAllWheels();

}
void ChassisSimulator::InitSpecification()
{
	//m_vPosition.set(DescSuspension::suspension.sim_vPosition.x,
	//	DescSuspension::suspension.sim_vPosition.y,
	//	DescSuspension::suspension.sim_vPosition.z);

	NiPoint3 pt = CLevelManager::Get()->GetMapDesc()->kSusspensionPos;
	m_vPosition.set(pt.x, pt.y, pt.z);

	m_vSpecChassis.set(DescSuspension::suspension.sim_vSpecChassis.x,
		DescSuspension::suspension.sim_vSpecChassis.y, 
		DescSuspension::suspension.sim_vSpecChassis.z);//(0.15f, 0.15f, 0.01f);

	m_vSpecBase.set(DescSuspension::suspension.sim_vSpecBase.x,
		DescSuspension::suspension.sim_vSpecBase.y,
		DescSuspension::suspension.sim_vSpecBase.z);//(0.3f, 0.3f, 0.05f);

	m_fSpecWheelRadius = DescSuspension::suspension.sim_fSpecWheelRadius;//0.05f;

	m_fMassChassis = DescSuspension::suspension.sim_fMassChassis;//3.8f;
	m_fMassWheel = DescSuspension::suspension.sim_fMassWheel;//2.8005;


	 m_fZBases = DescSuspension::suspension. sim_fZBases ;
	 m_fZChassis = DescSuspension::suspension.sim_fZChassis ;
	 m_fZPillar = DescSuspension::suspension.sim_fZPillar ;
	 m_fZWheels = DescSuspension::suspension.sim_fZWheels ;

	 m_fAnchorZSuspensions = DescSuspension::suspension.sim_fAnchorZSuspensions;
	 m_fAnchorZPillar = DescSuspension::suspension.sim_fAnchorZPillar;
	 m_fAnchorChassis = DescSuspension::suspension.sim_fAnchorChassis;


	 m_fSusSpring = DescSuspension::suspension.sim_fSusSpring;
	 m_fSusDamping = DescSuspension::suspension.sim_fSusDamping;
	 m_fSusValue = DescSuspension::suspension.sim_fSusValue;
	 m_fSusRestitution = DescSuspension::suspension.sim_fSusRestitution;
	 m_fSusLimitPlane = DescSuspension::suspension.sim_fSusLimitPlane;

	 m_vBasePoints[1].set(m_vSpecChassis.x, m_vSpecChassis.y, m_fZBases);
	 m_vBasePoints[0].set(m_vSpecChassis.x, -m_vSpecChassis.y, m_fZBases);
	 m_vBasePoints[3].set(-m_vSpecChassis.x, -m_vSpecChassis.y, m_fZBases);
	 m_vBasePoints[2].set(-m_vSpecChassis.x, m_vSpecChassis.y, m_fZBases);




}
bool ChassisSimulator::CreateChassis()
{
	NxVec3 pos(0.0f, 0.0f, m_fZChassis);
	Offset(pos);
	NxVec3 offsetCenter(0.0f, 0.0f, 0.0f);

	float s = m_fChassisScale;
	NxVec3 vChassis;
	vChassis.x = s * m_vSpecChassis.x;
	vChassis.y = s * m_vSpecChassis.y;
	vChassis.z = m_vSpecChassis.z;


	m_pActorChassis = CreateBox(m_pNxScene, pos, vChassis, offsetCenter, m_fMassChassis);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	m_pActorChassis->setGroup(GROUP_WHEELS);
	//SetMaterial(m_pActorChassis, m_nNoneBounceMaterial);

	//m_pActorChassis->setCMassOffsetLocalPosition(NxVec3(0.0f, -vChassis.y*0.05f, 0.0f));


	bool ret = false;
	if (m_pActorChassis)
		ret = true;

	return ret;
}

bool ChassisSimulator::CreateBases()
{
	NxVec3 pos;
	NxVec3 offsetCenter(0.0f, 0.0f, -m_vSpecBase.z);

	for (int i = 0; i < WHEELS; ++i)
	{
		pos = m_vBasePoints[i];
		//pos.z = -0.20f;
		Offset(pos);
		
		m_pActorBases[i] = CreateBox(m_pNxScene, pos, m_vSpecBase, offsetCenter, 0);
		//m_pActorBases[i] = CreateCCDBox(m_pNxScene, pos, m_vSpecBase,10, offsetCenter, true);
		//m_pActorBases[i]->raiseBodyFlag(NX_BF_KINEMATIC);
		m_pActorBases[i]->setGroup(GROUP_BASES);
		SetMaterial(m_pActorBases[i], m_nNoneBounceMaterial);
	}

	//TODO:check result 
	return true;
}

//bool ChassisSimulator::CreateDriftingLifters()
//{
//	NxVec3 pos;
//	float fRadius = 0.1f;
//
//	NxVec3 centerOffset(0, 0, -fRadius);
//
//	pos = (m_vBasePoints[1]+m_vBasePoints[2]) * 0.5f;
//	//Offset(pos);
//	m_pActorLifters[0] = CreateSphere(m_pNxScene, pos, fRadius, 1, centerOffset);
//	m_pActorLifters[0]->raiseBodyFlag(NX_BF_KINEMATIC);
//	SetMaterial(m_pActorLifters[0], m_nNoneBounceMaterial);
//
//
//	pos = (m_vBasePoints[0]+m_vBasePoints[3]) * 0.5f;
//	//Offset(pos);
//	m_pActorLifters[1] = CreateSphere(m_pNxScene, pos, fRadius, 1, centerOffset);
//	m_pActorLifters[1]->raiseBodyFlag(NX_BF_KINEMATIC);
//	SetMaterial(m_pActorLifters[1], m_nNoneBounceMaterial);
//
//	return true;
//}

bool ChassisSimulator::CreateWheels()
{
	float radius = m_fSpecWheelRadius;
	float fMass = m_fMassWheel;

	for (int i = 0; i < WHEELS; ++i)
	{
		m_vWheelsPos[i] = m_vBasePoints[i];
		m_vWheelsPos[i].z = m_fZWheels;

		Offset(m_vWheelsPos[i]);

		NxVec3 extent(radius, radius, radius);
		//m_pActorWheels[i] = CreateCCDBox(m_pNxScene, m_vWheelsPos[i], extent, fMass, NxVec3(0, 0, 0), true);
		m_pActorWheels[i] = CreateSphere(m_pNxScene, m_vWheelsPos[i], radius, fMass);
		//m_pActorWheels[i] = CreateCCDSphere(m_pNxScene, m_vWheelsPos[i]	,radius, fMass, true);
		m_pActorWheels[i]->setGroup(GROUP_WHEELS);

		SetMaterial(m_pActorWheels[i], m_nNoneBounceMaterial);

		//m_pActorWheels[i]->raiseBodyFlag(NX_BF_KINEMATIC);
	}

	//m_pActorWheels[0]->raiseBodyFlag(NX_BF_KINEMATIC);
	//m_pActorWheels[1]->raiseBodyFlag(NX_BF_KINEMATIC);
	return true;
}
bool ChassisSimulator::CreateSuspensions()
{
	NxVec3 globalAxis(0.0f, 0.0f, -1.0f);

	for (int i = 0; i < WHEELS; ++i)
	{
		m_vGlobalAnchors[i] = m_vBasePoints[i];
		m_vGlobalAnchors[i].z = m_fAnchorZSuspensions;//0.9f;

		Offset(m_vGlobalAnchors[i]);

		m_pSuspensions[i] = CreateD6Joint(m_pNxScene, m_pActorChassis
			, m_pActorWheels[i], m_vGlobalAnchors[i], globalAxis);
		//m_pSuspensions[i] = CreateD6Joint(m_pNxScene, m_pActorWheels[i]
		//	, m_pActorChassis, m_vGlobalAnchors[i], globalAxis);
	}
	return true;
}
void ChassisSimulator::ReconfigureSuspension(int nJoint)
{
	NxActor* a0 = NULL;
	NxActor* a1 = NULL;
	m_pSuspensions[nJoint]->getActors(&a0, &a1);

	//NxActor* a0 = m_pActorChassis;
	//NxActor* a1 = m_pActorWheels[nJoint];

	NxD6JointDesc oldD6Desc, d6Desc;
	NxVec3 localAnchor[2], localAxis[2], localNormal[2], localBinormal[2];

	m_pSuspensions[nJoint]->saveToDesc(oldD6Desc);

	localAnchor[0] = oldD6Desc.localAnchor[0];
	localAnchor[1] = oldD6Desc.localAnchor[1];

	localAxis[0] = oldD6Desc.localAxis[0];
	localNormal[0] = oldD6Desc.localNormal[0];
	localBinormal[0] = localNormal[0].cross(localAxis[0]);

	localAxis[1] = oldD6Desc.localAxis[1];
	localNormal[1] = oldD6Desc.localNormal[1];
	localBinormal[1] = localNormal[1].cross(localAxis[1]);


	d6Desc.actor[0] = a0;
	d6Desc.actor[1] = a1;

	// Reset anchor and axis
	NxVec3 globalAnchor = m_vGlobalAnchors[nJoint];
	NxVec3 globalAxis = NxVec3(0,0, -1);

	d6Desc.setGlobalAnchor(globalAnchor);
	d6Desc.setGlobalAxis(globalAxis);

	//d6Desc.twistMotion = NX_D6JOINT_MOTION_LOCKED;
	//d6Desc.swing1Motion = NX_D6JOINT_MOTION_FREE;
	//d6Desc.swing2Motion = NX_D6JOINT_MOTION_FREE;

	//d6Desc.xMotion = NX_D6JOINT_MOTION_LIMITED;
	//d6Desc.yMotion = NX_D6JOINT_MOTION_LOCKED;
	//d6Desc.zMotion = NX_D6JOINT_MOTION_LOCKED;


	d6Desc.twistMotion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.swing1Motion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.swing2Motion = NX_D6JOINT_MOTION_LOCKED;

	d6Desc.xMotion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.yMotion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.zMotion = NX_D6JOINT_MOTION_LOCKED;


	d6Desc.linearLimit.spring = m_fSusSpring;//500.05;
	d6Desc.linearLimit.damping = m_fSusDamping;//0.1;
	d6Desc.linearLimit.value = m_fSusValue;//0.1;
	d6Desc.linearLimit.restitution = m_fSusRestitution;//0.008f;


	//d6Desc.projectionMode = NX_JPM_NONE;

	d6Desc.projectionMode = NX_JPM_POINT_MINDIST;
	d6Desc.projectionDistance = 0.01f;
	d6Desc.projectionAngle = NxMath::degToRad(3.0f);

	//d6Desc.jointFlags |= NX_JF_COLLISION_ENABLED;

	m_pSuspensions[nJoint]->loadFromDesc(d6Desc);

	m_pSuspensions[nJoint]->setLimitPoint(globalAnchor);
	m_pSuspensions[nJoint]->addLimitPlane(-globalAxis, globalAnchor + m_fSusLimitPlane * globalAxis);    
	m_pSuspensions[nJoint]->addLimitPlane(globalAxis, globalAnchor - m_fSusLimitPlane * globalAxis);
}
//bool ChassisSimulator::CreatePillarBase()	
//{
//	NxVec3 pos(0.0f, 0.0f, -20.0f);
//	NxVec3 vExtent(0.1f, 0.1f, 0.1f);
//
//	Offset(pos);
//
//	m_pActorPillarBase = CreateBox(m_pNxScene, pos, vExtent, NxVec3(0.0f, 0.0f, 0.0f),10);
//	m_pActorPillarBase->readBodyFlag(NX_BF_KINEMATIC);
//
//	return true;
//}

bool ChassisSimulator::CreatePillarActor()
{
	NxVec3 pos(0.0f, 0.0f, m_fZPillar);
	Offset(pos);
	NxVec3 offsetCenter(0.0f, 0.0f, 0.0f);
	NxVec3 boxDim(m_vSpecChassis);
	boxDim *= 0.5f;

	m_pActorPillar = CreateBox(m_pNxScene, pos, boxDim, offsetCenter, 
		m_fMassChassis* 0.1f);

	return true;
}

bool ChassisSimulator::CreatePillar()
{
	NxVec3 globalAxis(0.0f, 0.0f, -1.0f);
	
	NxVec3 pos(0.0f, 0.0f, m_fAnchorZPillar);
	Offset(pos);

	m_pPillar = CreateD6Joint(m_pNxScene, NULL, m_pActorChassis,
			 pos, globalAxis);

	return true;
}
void ChassisSimulator::ReconfigurePillar(NxD6Joint* pJoint)
{
	NxActor* a0 = NULL;
	NxActor* a1 = NULL;
	pJoint->getActors(&a0, &a1);

	NxD6JointDesc oldD6Desc, d6Desc;
	NxVec3 localAnchor[2], localAxis[2], localNormal[2], localBinormal[2];

	pJoint->saveToDesc(oldD6Desc);

	localAnchor[0] = oldD6Desc.localAnchor[0];
	localAnchor[1] = oldD6Desc.localAnchor[1];

	localAxis[0] = oldD6Desc.localAxis[0];
	localNormal[0] = oldD6Desc.localNormal[0];
	localBinormal[0] = localNormal[0].cross(localAxis[0]);

	localAxis[1] = oldD6Desc.localAxis[1];
	localNormal[1] = oldD6Desc.localNormal[1];
	localBinormal[1] = localNormal[1].cross(localAxis[1]);

	d6Desc.actor[0] = a0;
	d6Desc.actor[1] = a1;

	// Reset anchor and axis
	NxVec3 globalAnchor(0, 0, m_fAnchorZPillar);// = m_vGlobalAnchors[nJoint];
	Offset(globalAnchor);

	NxVec3 globalAxis = NxVec3(0,0, -1);

	d6Desc.setGlobalAnchor(globalAnchor);
	d6Desc.setGlobalAxis(globalAxis);

	d6Desc.twistMotion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.swing1Motion = NX_D6JOINT_MOTION_LIMITED;
	d6Desc.swing2Motion = NX_D6JOINT_MOTION_LIMITED;

	d6Desc.xMotion = NX_D6JOINT_MOTION_FREE;
	d6Desc.yMotion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.zMotion = NX_D6JOINT_MOTION_LOCKED;

	d6Desc.swing1Limit.value=NxMath::degToRad(60.0f);
	d6Desc.swing1Limit.damping=0.0f;
	d6Desc.swing1Limit.restitution=0.0f;

	d6Desc.swing2Limit.value=NxMath::degToRad(60.0f);
	d6Desc.swing2Limit.damping=0.0f;
	d6Desc.swing2Limit.restitution=0.0f;



	//d6Desc.projectionMode = NX_JPM_NONE;
	d6Desc.projectionMode = NX_JPM_POINT_MINDIST;
	d6Desc.projectionDistance = 0.001f;
	d6Desc.projectionAngle = NxMath::degToRad(1.0f);


	pJoint->loadFromDesc(d6Desc);

}
NxActor* ChassisSimulator::CreateSphere(NxScene* pScene, 
										const NxVec3& pos, 
										float fRadius, 
										const NxReal fMass,
										NxVec3 centerOffset)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	// The actor has one shape, a sphere
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = fRadius;
	sphereDesc.localPose.t = centerOffset;

	actorDesc.shapes.pushBack(&sphereDesc);

	if (fMass)
	{
		//bodyDesc.mass = fMass;
		actorDesc.body = &bodyDesc;
		actorDesc.density = fMass;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.globalPose.t = pos;
	NxActor* pActor = pScene->createActor(actorDesc);

	//if (pActor)
	//	pActor->setMass(fMass);

	return pActor;
}
NxCCDSkeleton* ChassisSimulator::CreateCCDSkeleton(NxScene* pScene, float size)
{
	NxU32 triangles[3 * 12] = { 
		0,1,3,
			0,3,2,
			3,7,6,
			3,6,2,
			1,5,7,
			1,7,3,
			4,6,7,
			4,7,5,
			1,0,4,
			5,1,4,
			4,0,2,
			4,2,6
	};

	NxVec3 points[8];

	// Static mesh
	points[0].set( size, -size, -size);
	points[1].set( size, -size,  size);
	points[2].set( size,  size, -size);
	points[3].set( size,  size,  size);

	points[4].set(-size, -size, -size);
	points[5].set(-size, -size,  size);
	points[6].set(-size,  size, -size);
	points[7].set(-size,  size,  size);

	NxSimpleTriangleMesh stm;
	stm.numVertices = 8;
	stm.numTriangles = 6*2;
	stm.pointStrideBytes = sizeof(NxVec3);
	stm.triangleStrideBytes = sizeof(NxU32)*3;

	stm.points = points;
	stm.triangles = triangles;
	stm.flags |= NX_MF_FLIPNORMALS;

	return pScene->getPhysicsSDK().createCCDSkeleton(stm);
}

NxActor* ChassisSimulator::CreateCCDBox(NxScene* pScene, const NxVec3& pos, 
										const NxVec3& boxDim, const NxReal density,
										const NxVec3& centerOffset, bool doDynamicCCD)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(boxDim.x,boxDim.y,boxDim.z);
	boxDesc.localPose.t = centerOffset;
	
	bodyDesc.CCDMotionThreshold = 1.0f;

	// Set box up with CCD skeleton
	boxDesc.ccdSkeleton = CreateCCDSkeleton(pScene, boxDim.x*0.8f);

	// Activate dynamic-dynamic CCD for this body   ---- v2.7.0
	//if (doDynamicCCD)  boxDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;  

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
NxActor* ChassisSimulator::CreateBox(NxScene* pScene, 
									 const NxVec3& pos, 
									 const NxVec3& boxDim,
									 const NxVec3& centerOffset,
									 const NxReal fMass)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions.set(boxDim.x,boxDim.y,boxDim.z);
	boxDesc.localPose.t = centerOffset;
	actorDesc.shapes.pushBack(&boxDesc);

	if (fMass)
	{
		//bodyDesc.mass = fMass;
		actorDesc.body = &bodyDesc;
		actorDesc.density = fMass;
		
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.globalPose.t = pos;

	actorDesc.isValid();

	NxActor* pActor = pScene->createActor(actorDesc);

	return pActor;
}

NxD6Joint* ChassisSimulator::CreateD6Joint(NxScene* pScene, NxActor* a0, 
									  NxActor* a1, const NxVec3& globalAnchor, 
									  const NxVec3& globalAxis)
{
	NxD6JointDesc d6Desc;
	d6Desc.actor[0] = a0;
	d6Desc.actor[1] = a1;
	d6Desc.setGlobalAxis(globalAxis);
	d6Desc.setGlobalAnchor(globalAnchor);

	d6Desc.twistMotion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.swing1Motion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.swing2Motion = NX_D6JOINT_MOTION_LOCKED;

	d6Desc.xMotion = NX_D6JOINT_MOTION_LOCKED ;
	d6Desc.yMotion = NX_D6JOINT_MOTION_LOCKED;
	d6Desc.zMotion = NX_D6JOINT_MOTION_LOCKED;

	d6Desc.projectionMode = NX_JPM_NONE;

	return (NxD6Joint*)pScene->createJoint(d6Desc);
}





//bool ChassisSimulator::CreateChassisJoint()
//{
//	NxVec3 globalAxis(0.0f, 0.0f, -1.0f);
//
//	NxVec3 pos(0.0f, 0.0f, m_fAnchorChassis);
//	Offset(pos);
//
//	m_pChassisJoint = CreateD6Joint(m_pNxScene,m_pActorPillar, m_pActorChassis,
//		pos, globalAxis);
//
//	return true;
//}



void ChassisSimulator::Offset(NxVec3& pos)
{
	pos += m_vPosition;
}

void ChassisSimulator::Update(float dt, NxVec3 vel, const NxVec3& velLocal, 
const NxVec3& vNormalGrd, int nDrift)
{

	InputProcess(dt);


	UpdateContactState();

	UpdateState(dt, vel, vNormalGrd);

	m_statePre = m_state;

//	UpdateDrift(dt);

	//UpdateMassTranslation(dt);

//	UpdateDriftingLifter(dt);
	//UpdateSideTip(dt, velLocal, nDrift);
	
}

//void ChassisSimulator::UpdateSideTip(const NxVec3& velLoal, int nDrift)
//{
//	//LogText("侧向速度 : %f \n", velLoal.y);
//
//	NxVec3 sideForce(0, 0, 0);
//	float coeff = NxMath::abs(velLoal.y)/15.0f; 
//
//	if (coeff < 0.3)
//		return;
//
//	if (!nDrift)
//		return;
//
//	int nSide = 0;
//
//	sideForce.z = 7.0f;
//
//	float revsCoeff = -0.5f;
//
//	float mainCoeff = 1.0f;
//
//	if (velLoal.y > 0.0f)
//	{
//		m_pActorChassis->addForceAtLocalPos(mainCoeff * sideForce, m_vBasePoints[0]);
//		m_pActorChassis->addForceAtLocalPos(sideForce, m_vBasePoints[3]);
//		m_pActorChassis->addForceAtLocalPos(revsCoeff * sideForce, m_vptSideTip[1]);
//		m_pActorChassis->addForceAtLocalPos(revsCoeff * sideForce, m_vptSideTip[2]);	
//	}
//	else
//	{
//		m_pActorChassis->addForceAtLocalPos(mainCoeff * sideForce, m_vBasePoints[1]);
//		m_pActorChassis->addForceAtLocalPos(sideForce, m_vBasePoints[2]);
//		m_pActorChassis->addForceAtLocalPos(revsCoeff * sideForce, m_vptSideTip[0]);
//		m_pActorChassis->addForceAtLocalPos(revsCoeff * sideForce, m_vptSideTip[3]);
//	}
//
//
//}

void ChassisSimulator::UpdateDriftingLifter(float dt)
{
	float sfDriftT = 0.80f;
	float sfOffsetMax = 0.07f;

	//if not drifting, move the lifters to a lower positions
	if (m_nDriftingState == NOT_DRIFTING)
	{
		//TODO:只在在漂移结束时更新一次
		for (int i = 0; i < WHEELS; ++i)
		{
			m_fDriftOffset[i] = 0.0f;
		}
		m_fDirftTime = 0.0f;

	}
	else
	{

		m_fDirftTime += dt;

		m_fDirftTime = NxMath::clamp(m_fDirftTime, sfDriftT, 0.0f);

		float fOffset = sfOffsetMax * NxMath::sin(m_fDirftTime/sfDriftT * NxPi * 0.5f);

		if (m_nDriftingState == LEFT_DRIFTING)
		{
			m_fDriftOffset[1] = fOffset;//sfOffsetMax * NxMath::sin(m_fDirftTime/sfDriftT * NxPi * 0.5f);
			m_fDriftOffset[2] = m_fDriftOffset[1];
		}
		else // right drifting
		{
			m_fDriftOffset[0] = fOffset;//sfOffsetMax * m_fDirftTime;
			m_fDriftOffset[3] = m_fDriftOffset[0];
		}
		
	}
	
}
void ChassisSimulator::SetLifterValue(int nDriftState, float fDriftAngle)
{

	if ((m_nDriftingState == NOT_DRIFTING) 	&& (m_nDriftingState != nDriftState))
		OnDriftStart(nDriftState);
	else if ((m_nDriftingState != NOT_DRIFTING) && (nDriftState == NOT_DRIFTING))
		OnDriftEnd(nDriftState);

	m_nDriftingState = nDriftState;
	m_fDriftAngle = fDriftAngle;

	if (nDriftState == NOT_DRIFTING)
		m_fDriftAngle = 0.0f;
}
//void ChassisSimulator::MoveLifter(int nId, float fZ)
//{
//	NxVec3 position = m_pActorLifters[nId]->getGlobalPosition();
//	position.z = fZ;
//	float fCurrentZ = m_pActorLifters[nId]->getGlobalPosition().z;
//
//	if (NxMath::abs(fCurrentZ - fZ) > 0.001f)
//		m_pActorLifters[nId]->moveGlobalPosition(position);
//}
void ChassisSimulator::SetClissionable()
{
	for (int i = 0; i < WHEELS; ++i)
	{
		NxActor* pWheel = m_pActorWheels[i];
		for (int j = 0; j <WHEELS; ++j)
		{
			NxActor* pBase = m_pActorBases[j];
			if (i == j)
				m_pNxScene->setActorPairFlags(*pWheel, *pBase,NX_NOTIFY_ALL );
			else
				m_pNxScene->setActorPairFlags(*pWheel, *pBase,NX_IGNORE_PAIR);
		}
	}

	for (int n = 0; n < WHEELS; ++n)
	{
		NxActor* pBase = m_pActorBases[n];
		m_pNxScene->setActorPairFlags(*pBase, *m_pActorChassis,NX_IGNORE_PAIR);
	}
}

void ChassisSimulator::SetBasePosition(float fB0, float fB1, float fB2, float fB3)
{
	m_fBasesPosition[0]	 = fB0;
	m_fBasesPosition[1]	 = fB1;
	m_fBasesPosition[2]	 = fB2;
	m_fBasesPosition[3]	 = fB3;
}

void ChassisSimulator::MoveBase(int nBaseId, float fZ)
{
	NxVec3 position = m_pActorBases[nBaseId]->getGlobalPosition();
	position.z = fZ;
	float fCurrentZ = m_pActorBases[nBaseId]->getGlobalPosition().z;

	if (NxMath::abs(fCurrentZ - fZ) > 0.005f)
	{
		//m_pActorBases[nBaseId]->moveGlobalPosition(position);
		m_pActorBases[nBaseId]->setGlobalPosition(position);

		if ((fCurrentZ - fZ) > 0.5f)
		{
			LogText("超高啦！！！！ %f\n", fCurrentZ-fZ);
		}
		//LogText("Base 移动：  %f  \n",fCurrentZ - fZ);
	}

	//TODD: 临时代码 测试

	//if (nBaseId==0)// || nBaseId==1)
	//{
	//	NxVec3 p = m_pActorWheels[nBaseId]->getGlobalPosition();
	//	p.z = fZ + m_fSpecWheelRadius;
	//	m_pActorWheels[nBaseId]->moveGlobalPosition(p);
	//}
}

void ChassisSimulator::MoveBaseWithDisp(int nBaseId, float fDisp)
{
	NxVec3 position = m_pActorBases[nBaseId]->getGlobalPosition();
	position.z += fDisp;
	//m_pActorBases[nBaseId]->moveGlobalPosition(position);
	m_pActorBases[nBaseId]->setGlobalPosition(position);

}

void ChassisSimulator::LiftBase(int nBaseId, float fZ)
{
	NxVec3 position = m_pActorBases[nBaseId]->getGlobalPosition();
	position.z += fZ;
	m_pActorBases[nBaseId]->moveGlobalPosition(position);
}
//#ifdef _DEBUG
void ChassisSimulator::InputProcess(float dt)
{
	static NxVec3 driftForce(0, 0, 0.1f);

	float fUpCoeff = (InputManager::keys[NiInputKeyboard::KEY_2])? -1.0f: 1.0f;
	float dispZ = 0.25f* dt * fUpCoeff;
	
	if (InputManager::keys[NiInputKeyboard::KEY_7])
	{
		LiftBase(0, dispZ);
	}
	if (InputManager::keys[NiInputKeyboard::KEY_9])
	{
		LiftBase(1, dispZ);
	}
	if (InputManager::keys[NiInputKeyboard::KEY_3])
	{
		LiftBase(2, dispZ);
	}
	if (InputManager::keys[NiInputKeyboard::KEY_1])
	{
		LiftBase(3, dispZ);
	}
	
	if (InputManager::keys[NiInputKeyboard::KEY_5])
	{
		driftForce.z = 10.0f;
		NxVec3 posF = (m_vBasePoints[0] + m_vBasePoints[3])* 0.5f;
		posF.z = 0.0f;

		m_pActorChassis->addForceAtLocalPos(driftForce, posF);
	}

	//RESET !!!!
	if (InputManager::keys[NiInputKeyboard::KEY_R])
	{
		//LevelManager::GetScriptVM()->ExecuteScriptFile("Scripts/DescConstant.lua", false);

		Reset();
	}

	if (InputManager::keys[NiInputKeyboard::KEY_4])
	{
		driftForce.z += 0.1f;
		NxVec3 posF = (m_vBasePoints[0] + m_vBasePoints[3])* 0.5f;
		posF.z = 0.0f;

		m_pActorChassis->addForceAtLocalPos(driftForce, posF);
	}
}
//#endif

void ChassisSimulator::SetChassisPosition(NxVec3 pos)
{
	//m_pActorPillar->readBodyFlag(NX_BF_KINEMATIC);
	//m_pActorChassis->readBodyFlag(NX_BF_KINEMATIC);
	//m_pActorPillar->moveGlobalPosition(pos);
	//m_pActorChassis->moveGlobalPosition(pos);
	//m_pActorPillar->clearBodyFlag(NX_BF_KINEMATIC);
	//m_pActorChassis->clearBodyFlag(NX_BF_KINEMATIC);
}

void ChassisSimulator::InitReset()
{
	//set all actors to their corresponding positions
	FreezeActor(m_pActorChassis);
	NxVec3 posChassis(0.0f, 0.0f, m_fZChassis);
	MoveTo(m_pActorChassis, posChassis);

	for(int i = 0; i < WHEELS; ++i)
	{
		//FreezeActor(m_pActorBases[i]);
		NxVec3 posBase = m_vBasePoints[i];
		MoveTo(m_pActorBases[i], posBase);
	}

	for (int j = 0; j < WHEELS; ++j)
	{
		FreezeActor(m_pActorWheels[j]);

		NxVec3 posWheel = m_vBasePoints[j];
		m_vWheelsPos[j].z = m_fZWheels;
		MoveTo(m_pActorWheels[j], posWheel);
	}

	//FreezeActor(m_pActorPillar);
	//NxVec3 posPillar(0.0f, 0.0f, m_fZPillar);
	//MoveTo(m_pActorPillar, posPillar);



	//refresh the position of the anchors
	for (int i = 0; i < WHEELS; ++i)
	{
		m_vGlobalAnchors[i] = m_vBasePoints[i];
		m_vGlobalAnchors[i].z = m_fAnchorZSuspensions;//0.9f;

		Offset(m_vGlobalAnchors[i]);
	}




}
void ChassisSimulator::EndReset()
{
	MeltActor(m_pActorChassis);


	for (int j = 0; j < WHEELS; ++j)
	{
		MeltActor(m_pActorWheels[j]);
	}

	//MeltActor(m_pActorPillar);
}

void ChassisSimulator::Reset()
{
	DestroyAll();
	InitNx();
}

void ChassisSimulator::FreezeActor(NxActor* pAcotr)
{
	pAcotr->raiseBodyFlag(NX_BF_KINEMATIC);
}
void ChassisSimulator::MeltActor(NxActor* pAcotr)
{
	pAcotr->clearBodyFlag(NX_BF_KINEMATIC);
	pAcotr->setMass(1.0f);
}
void ChassisSimulator::MoveTo(NxActor* pActor, NxVec3 pos)
{
	Offset(pos);
	NxMat33 ori;
	ori.id();
	pActor->moveGlobalOrientation(ori);
	pActor->moveGlobalPosition(pos);
}
void ChassisSimulator::FreezeAllActors()
{
	FreezeActor(m_pActorChassis);

	/*for(int i = 0; i < WHEELS; ++i)
		FreezeActor(m_pActorBases[i]);*/

	for (int j = 0; j < WHEELS; ++j)
		FreezeActor(m_pActorWheels[j]);

	//FreezeActor(m_pActorPillar);

}

void ChassisSimulator::DestroyAll()
{
	//release all joints
	//m_pNxScene->releaseJoint(*m_pChassisJoint);
	m_pNxScene->releaseJoint(*m_pPillar);
	for(int i = 0; i < WHEELS; ++i)
	{
		m_pNxScene->releaseJoint(*m_pSuspensions[i]);
		m_pSuspensions[i] = NULL;
	}

	//release all actors
	m_pNxScene->releaseActor(*m_pActorChassis);

	for(int i = 0; i < WHEELS; ++i)
		m_pNxScene->releaseActor(*m_pActorBases[i]);

	for (int j = 0; j < WHEELS; ++j)
		m_pNxScene->releaseActor(*m_pActorWheels[j]);

	m_pNxScene->releaseActor(*m_pActorChassis);
	//m_pNxScene->releaseActor(*m_pActorPillar);

}

void ChassisSimulator::CreateNoneBounceMaterial()
{
	NxMaterialDesc m;
	m.restitution = 0.0f;
	m.dynamicFriction = 0.5f;
	m.staticFriction = 0.5f;

	m_nNoneBounceMaterial = m_pNxScene->createMaterial(m)->getMaterialIndex();
}

void ChassisSimulator::SetMaterial(NxActor* pActor, NxMaterialIndex nMtr)
{
	NxShape** shapes = (NxShape**)pActor->getShapes();
	NxU32 nShapes = pActor->getNbShapes();
	while (nShapes--)
	{
		shapes[nShapes]->setMaterial(nMtr);
	}

}
//void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16)
//NxCCDSkeleton* CreateCCDSphereSkeleton(NxScene* pScene, const float r, 
//									   const int nRings, const int nSegments)
//{
//	//MeshPtr pSphere = MeshManager::getSingleton().createManual(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//	//SubMesh *pSphereVertex = pSphere->createSubMesh();
//
//	//pSphere->sharedVertexData = new VertexData();
//	//VertexData* vertexData = pSphere->sharedVertexData;
//
//	//// define the vertex format
//	//VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
//	//size_t currOffset = 0;
//	//// positions
//	//vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
//	//currOffset += VertexElement::getTypeSize(VET_FLOAT3);
//	//// normals
//	//vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
//	//currOffset += VertexElement::getTypeSize(VET_FLOAT3);
//	//// two dimensional texture coordinates
//	//vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
//	//currOffset += VertexElement::getTypeSize(VET_FLOAT2);
//
//	//// allocate the vertex buffer
//	//vertexData->vertexCount = (nRings + 1) * (nSegments+1);
//	//HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
//	//VertexBufferBinding* binding = vertexData->vertexBufferBinding;
//	//binding->setBinding(0, vBuf);
//	//float* pVertex = static_cast<float*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));
//
//	//// allocate index buffer
//	//pSphereVertex->indexData->indexCount = 6 * nRings * (nSegments + 1);
//	//pSphereVertex->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, pSphereVertex->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
//	//HardwareIndexBufferSharedPtr iBuf = pSphereVertex->indexData->indexBuffer;
//	//unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));
//	//NxU32 triangles[3 * nRings * (nSegments + 1)];
//
//	//NxVec3 points[(nRings + 1) * (nSegments+1)];
//
//	//// Static mesh
//	//points[0].set( size, -size, -size);
//	//points[1].set( size, -size,  size);
//	//points[2].set( size,  size, -size);
//	//points[3].set( size,  size,  size);
//
//	//points[4].set(-size, -size, -size);
//	//points[5].set(-size, -size,  size);
//	//points[6].set(-size,  size, -size);
//	//points[7].set(-size,  size,  size);
//
//	float fDeltaRingAngle = (Math::PI / nRings);
//	float fDeltaSegAngle = (2 * Math::PI / nSegments);
//	unsigned short wVerticeIndex = 0 ;
//	usigned int pointIndex = 0;
//
//	// Generate the group of rings for the sphere
//	for( int ring = 0; ring <= nRings; ring++ ) {
//		float r0 = r * sinf (ring * fDeltaRingAngle);
//		float y0 = r * cosf (ring * fDeltaRingAngle);
//
//		// Generate the group of segments for the current ring
//		for(int seg = 0; seg <= nSegments; seg++) {
//			float x0 = r0 * sinf(seg * fDeltaSegAngle);
//			float z0 = r0 * cosf(seg * fDeltaSegAngle);
//
//			// Add one vertex to the strip which makes up the sphere
//			//*pVertex++ = x0;
//			//*pVertex++ = y0;
//			//*pVertex++ = z0;
//			points[pointIndex++].set(x0, y0, z0);
//
//			//Vector3 vNormal = Vector3(x0, y0, z0).normalisedCopy();
//			//*pVertex++ = vNormal.x;
//			//*pVertex++ = vNormal.y;
//			//*pVertex++ = vNormal.z;
//
//			//*pVertex++ = (float) seg / (float) nSegments;
//			//*pVertex++ = (float) ring / (float) nRings;
//
//			if (ring != nRings) {
//				// each vertex (except the last) has six indices pointing to it
//				*triangles++ = wVerticeIndex + nSegments + 1;
//				*triangles++ = wVerticeIndex;               
//				*triangles++ = wVerticeIndex + nSegments;
//				*triangles++ = wVerticeIndex + nSegments + 1;
//				*triangles++ = wVerticeIndex + 1;
//				*triangles++ = wVerticeIndex;
//				wVerticeIndex ++;
//			}
//		}; // end for seg
//	} // end for ring
//
//}
NxActor* ChassisSimulator::CreateCCDSphere(NxScene* pScene, 
										   const NxVec3& pos,
										   float fRadius, 
										   const NxReal fMass,
										   bool doDynamicCCD)
{
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box
	NxSphereShapeDesc sphereDesc;
	sphereDesc.radius = fRadius;

	bodyDesc.CCDMotionThreshold = 1.0f;

	// Set box up with CCD skeleton
	sphereDesc.ccdSkeleton = GetCCDSphereSkeletonFromNif(fRadius);
	//if (doDynamicCCD)  sphereDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;  // Activate dynamic-dynamic CCD for this body

	actorDesc.shapes.pushBack(&sphereDesc);

	if (fMass)
	{
		actorDesc.body = &bodyDesc;
		actorDesc.density = fMass;
	}
	else
	{
		actorDesc.body = NULL;
	}
	actorDesc.globalPose.t = pos;
	return pScene->createActor(actorDesc);	
}

NxCCDSkeleton* ChassisSimulator::GetCCDSphereSkeletonFromNif(float fRadius)
{
	//fRadius 一个系数， 球的模型半径原本为1.0f

	NxCCDSkeleton* pSkeleton = NULL;

	char* filename = "Levels\\CCDwheel.nif";
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(filename)))
	{
		assert(0 && "file load failure -- move this to a factory");
	}

	// assumes specific NIF layout -- be careful
	NiNode* pkNode = (NiNode*)kStream.GetObjectAt(0);


	NxScene* pkNxScene = m_pNxScene;


	NiAVObject* pkNodeTerrain = ((NiNode*)pkNode->GetObjectByName("Sphere01"))->GetAt(0); 

	if (pkNodeTerrain)
	{
		if (NiIsKindOf(NiTriShape, pkNodeTerrain))
		{
			NiTriShape* pkTriSp = (NiTriShape*)pkNodeTerrain;
			unsigned short* triangles = pkTriSp->GetTriList();
			NiPoint3* pkVerts = pkTriSp->GetVertices();

			unsigned int uiPhysXNumVerts = pkTriSp->GetVertexCount();
			unsigned int uiPhysXNumFaces = pkTriSp->GetTriangleCount();

			NxVec3* pkPhysXMeshVerts = new NxVec3[uiPhysXNumVerts];
			NxU32* pkPhysXMeshFaces = new NxU32[uiPhysXNumFaces * 3];


			for (unsigned int usIndex = 0; usIndex < uiPhysXNumVerts; usIndex++)
			{
				//乘一下比例系数
				NiPhysXTypes::NiPoint3ToNxVec3(fRadius * (*pkVerts), pkPhysXMeshVerts[usIndex]);
				++pkVerts;
			}

			for (unsigned int uiFaces = 0; uiFaces < uiPhysXNumFaces*3; uiFaces++)
			{
				pkPhysXMeshFaces[uiFaces] = (NxU32)(triangles[uiFaces]);
			}

			
			NxSimpleTriangleMesh stm;
			stm.numVertices = uiPhysXNumVerts;
			stm.numTriangles =uiPhysXNumFaces;
			stm.pointStrideBytes = sizeof(NxVec3);
			stm.triangleStrideBytes = sizeof(NxU32)*3;

			stm.points = pkPhysXMeshVerts;
			stm.triangles = pkPhysXMeshFaces;
			stm.flags |= NX_MF_FLIPNORMALS;

			pSkeleton = m_pNxScene->getPhysicsSDK().createCCDSkeleton(stm);

			delete[] pkPhysXMeshVerts;
			delete[] pkPhysXMeshFaces;

			
		}
	}

	return pSkeleton;

}
NxVec3 ChassisSimulator::GetWheelPosition(int nWheelId)
{
	assert (nWheelId > -1 && nWheelId < WHEELS);

	NxVec3 pos = m_pActorWheels[nWheelId]->getGlobalPosition();
	return pos;
}

//有一个轮子落地，就认为车落地
int ChassisSimulator::GetContactState()
{
	//if (m_bIsFreezed)
	//	return 0;

	//for (int i = 0; i < WHEELS; ++i)
	//{
	//	NxVec3 posWheel = m_pActorWheels[i]->getGlobalPosition();
	//	NxVec3 posBase = m_pActorBases[i]->getGlobalPosition();
	//	
	//	if (NxMath::abs(posWheel.z - m_fSpecWheelRadius - posBase.z) < 0.03)
	//		return 1;

	//}

	//return 0;
	if (m_state == RS_ONEARTH)
		return 1;
	else
		return 0;
}
NxVec3 ChassisSimulator::GetBasePointGlobal(int id)
{
	NxVec3 posLocal = m_vBasePoints[id];
	posLocal.z = 0.0f;

	return m_pActorChassis->getGlobalPose()* posLocal;
}
int ChassisSimulator::WhoShouldBeKinematic(const NxVec3& vel)
{
	int ret = 0;
	//1) if all the wheels will descend, everyone is kinematic.
	//2) else, find the first wheel w1 in the velocity direction, if w1
	//   is going to ascend, it should be the very one, and if 
	//3) the w1 is going to descend, the wheel on the cross should be
	//   the kinematic one.

	if ((m_fBasesDisplacementZ[0] < 0.0f)&&
		(m_fBasesDisplacementZ[1] < 0.0f)&&
		(m_fBasesDisplacementZ[2] < 0.0f)&&
		(m_fBasesDisplacementZ[3] < 0.0f))
	{
		//MeltAllWheels();
	}
	else
	{
		int nHigher = FindMax(m_fBasesDisplacementZ, WHEELS);
		ret = nHigher;
	}
	return ret;
}

int ChassisSimulator::FindMax(float* pArray, int num)
{
	int nMaxIndex = 0;
	for (int i = 0; i < num; ++i)
	{
		if (pArray[i] > pArray[nMaxIndex] )
		{
			nMaxIndex = i;
		}
	}

	return nMaxIndex;
}
void ChassisSimulator::MeltAllWheels()
{
	for (int i = 0; i < WHEELS; ++i)
	{
		MeltActor(m_pActorWheels[i]);
		m_pActorWheels[i]->wakeUp();
	}
}

void ChassisSimulator::FreezeAllWheels()
{
	for (int i = 0; i < WHEELS; ++i)
	{
		FreezeActor(m_pActorWheels[i]);
	}
}


void ChassisSimulator::CaculateDisplacements()
{
	for (int i = 0; i < WHEELS; ++i)
	{
		NxVec3 position = m_pActorBases[i]->getGlobalPosition();
		m_fBasesDisplacementZ[i] = m_fBasesPosition[i] - position.z;

	}
}
void ChassisSimulator::MoveWheel(int nWId)
{
	NxVec3 p = m_pActorWheels[nWId]->getGlobalPosition();
	p.z = m_fBasesPosition[nWId] + m_fSpecWheelRadius+ 0.001f;
	m_pActorWheels[nWId]->moveGlobalPosition(p);

}
void ChassisSimulator::MoveWheels(float disp)
{
	for (int i = 0; i < WHEELS; ++i)
	{
		//must be kinematic
		assert(m_pActorWheels[i]->readBodyFlag(NX_BF_KINEMATIC));

		NxVec3 p = m_pActorWheels[i]->getGlobalPosition();
		p.z += disp;
		m_pActorWheels[i]->moveGlobalPosition(p);
	}
}
void ChassisSimulator::UpdateFreezState(float dt, const NxVec3& velGlobal, const NxVec3& vNormalGrd)
{
	static float fTinyDist = m_fSpecWheelRadius/2.0f;
	//如果在地上，就把所有的轮子固定在地上。。。3147
	if (m_state == RS_ONEARTH)
	{
		float alpha = 0.0f;
		int nTrend = GetTrend(vNormalGrd, velGlobal, alpha);

		if (nTrend < 0)//沿速度方向下降
		{
			//LogText("下降...\n");
			//LogText("m_fBasesDisplacementZ: ");
			//for (int n = 0; n < WHEELS; ++n)
			//{
			//	LogText("[%f]  ", m_fBasesDisplacementZ[n]);
			//}
			//LogText("\n");
			for (int i = 0; i < WHEELS; ++i)	
			{
				if (m_pActorWheels[i]->readBodyFlag(NX_BF_KINEMATIC))
				{
					//是否悬空了？悬空就自由下垂
					if ((m_fBasesDisplacementZ[i]< 0.0f) &&  (m_fBasesDisplacementZ[i] < -fTinyDist))
					{
						//LogText("我悬空了 被激活了 -- [%d]\n", i);
						NxVec3 posW = m_pActorWheels[i]->getGlobalPosition();
						NxVec3 posB = m_pActorBases[i]->getGlobalPosition();
						//m_pActorWheels[i]->clearBodyFlag(NX_BF_KINEMATIC);
					}
					else
					{
						LogText("我抓地呢！ -- [%d]\n", i);
					}

					//是不是全都自由了？全都自由，那就全都固定
					//并且转换到空中飞行的状态
					if (GetKinematicWheelNum() == 0)
					{
						LogText("我开始飞啦！！！\n");
						FreezeAllWheels();
						m_state = RS_INAIR;
					}
				}
			}

		}
		else if (nTrend > 0)//沿速度方向上升
		{
			//if (alpha > NxMath::degToRad(15.0f)) //跳起！！！！！！！
			LogText("上升。。。\n");
		}
		else
		{
			//LogText("平稳。。。\n");
		}

		for (int i = 0; i < WHEELS; ++i)	
		{
			if (!m_pActorWheels[i]->readBodyFlag(NX_BF_KINEMATIC))
			//活动的轮子是否落地了？落地就固定它
			{

				if (NxMath::abs(m_pActorWheels[i]->getGlobalPosition().z - 
					m_pActorBases[i]->getGlobalPosition().z) < (fTinyDist+ m_fSpecWheelRadius))
				{
					m_pActorWheels[i]->raiseBodyFlag(NX_BF_KINEMATIC);
					LogText("我落地了,固定我吧！！！[%d]\n", i);
				}
			}

		}

		//将固定的轮子移动到该去的位置
		MoveKinematicWheels();
		

		//for (int i = 0; i < WHEELS; ++i)	
		//{
		//	if (m_pActorWheels[i]->readBodyFlag(NX_BF_KINEMATIC))
		//	{
		//		//是否悬空了？悬空就自由下垂
		//		if (m_fBasesDisplacementZ[i] > fTinyDist)
		//		{
		//			m_pActorWheels[i]->clearBodyFlag(NX_BF_KINEMATIC);
		//		}

		//		//是不是全都自由了？全都自由，那就全都固定
		//		//并且转换到空中飞行的状态
		//		if (GetKinematicWheelNum() == 0)
		//		{
		//			FreezeAllWheels();
		//			m_state = RS_INAIR;
		//		}
		//	}
		//	else //活动的轮子是否落地了？落地就固定它
		//	{

		//		if (NxMath::abs(m_pActorWheels[i]->getGlobalPosition().z - 
		//			m_pActorBases[i]->getGlobalPosition().z) < fTinyDist)
		//		{
		//			m_pActorWheels[i]->raiseBodyFlag(NX_BF_KINEMATIC);
		//		}
		//	}

		//	//把落地的轮子移动到地面上
		//}

	}
	else if (m_state == RS_INAIR)
	{
		//check if any wheel is going to contact with the ground,
		float bWillContactGround = false;

		float disp = dt* velGlobal.z;

		if (disp < 0.0f)//下落
		{

			for (int j = 0; j < WHEELS; ++j)
			{
				//轮子距离地的距离
				float distGround = m_pActorWheels[j]->getGlobalPosition().z -
					m_fBasesPosition[j] - m_fSpecWheelRadius;

				//如果这一贞内车轮移动的距离会超出地面，那么激活所有wheel
				if (distGround < disp)
				{
					LogText("我落地了。。。\n");
					//判断是否需要在地面弹跳？

					//不需要弹跳
					{
						LogText("我落地了--所有轮子都活动了。。。\n");
						MeltAllWheels();
						bWillContactGround = true;
						m_state = RS_ONEARTH;
						break;
					}
				}
			}
		}
		
		//不会接触到地
		if (!bWillContactGround)
		{
			LogText("我在空中飞行！！！\n");
			//移动轮子
			MoveWheels(disp);
		}
	}
}
int ChassisSimulator::GetKinematicWheelNum()
{
	int num = 0;

	for(int i = 0; i < WHEELS; ++i)
	{
		if (m_pActorWheels[i]->readBodyFlag(NX_BF_KINEMATIC))
			num++;
	}
	return num;
}
int ChassisSimulator::GetTrend(const NxVec3& vNormalGrd, const NxVec3& velGlobal, float& alpha)
{
	int ret = 0;
	NxVec3 velG =velGlobal;
	NxVec3 vPreContactN = m_pActorChassis->getGlobalOrientation()* NxVec3(0, 0, 1);

	NxVec3 vCurContactN = vNormalGrd;

	NxReal angle = NxMath::acos(vPreContactN.dot(vCurContactN));
	alpha = angle;
	if (angle > NxMath::degToRad(3.0f))
	{
		//判断车在前进方向会向上、向下偏转
		//NxVec3 velG = _parent->_rotateTotal * _parent->_velocityLocal;

		NxVec3 vCross = vPreContactN.cross(vCurContactN);
		vCross.normalize();
		NxVec3 vCross2 = vPreContactN.cross(velG);
		vCross2.normalize();

		float fTheta = NxMath::acos(vCross.dot(vCross2));

		if (fTheta > NxMath::degToRad(90.0f))
		{
			ret = 1;//上坡
		}
		else
		{
			ret = -1;//下坡
		}
	}
	return ret;
}

void ChassisSimulator::MoveKinematicWheels()
{
	for (int i = 0; i < WHEELS; ++i)	
	{
		if (m_pActorWheels[i]->readBodyFlag(NX_BF_KINEMATIC))
		{
			if (m_fBasesDisplacementZ)
			MoveWheel(i);
		}
	}
}

//debug
//void ChassisSimulator::CreateWheelBalls()
//{
//	for (int i = 0; i < 4; ++i)
//	{
//		m_wheelBalls[i] = CreateSphere(m_pNxScene, NxVec3(0, 0, 0), 0.1, 10);
//		m_wheelBalls[i]->readBodyFlag(NX_BF_KINEMATIC);	
//	}
//}
//void ChassisSimulator::SetBallPosition(NxVec3 pos, int nIndex)
//{
//		m_wheelBalls[nIndex]->moveGlobalPosition(pos);
////		LogText("Wheel   %d   : (%f, %f, %f)\n", nIndex, pos.x, pos.y, pos.z);
//}

void ChassisSimulator::UpdateContactState()
{
	if (this->GetDistToGround() < (SKINWIDTH + 0.25))
		m_state = RS_ONEARTH;
	else
		m_state = RS_INAIR;

}

void ChassisSimulator::UpdateState(float dt, const NxVec3& velGlobal, const NxVec3& vNormalGrd)
{
	if (m_statePre == RS_ONEARTH)
	{
		//LogText("在地上\n");
		if (m_state == RS_INAIR)
		{
			LogText("-------------------腾空瞬间---------------------\n");
			// 给车一个调整姿态
			NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->SetCarFallPose();

			FreezeAllWheels();
		}
		else
		{
			MoveBasesWhenOnEarth();
		}

	}
	else if (m_statePre == RS_INAIR)
	{
//		LogText("-------------------在空中---------------------\n");
		if (m_state == RS_ONEARTH)//即将落地，把地面准备好
		{
			LogText("-------------------落地瞬间---------------------\n");
			MoveBaseWhenTouchGround();
			MeltAllWheels();
			LogText("落地速度 %f\n", velGlobal.z);
			NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->SetModalFall();
		}
	}
}



void ChassisSimulator::MoveBasesWhenOnEarth()
{
	int nStaticWheel = 0;
	
	//int np = GetPNInfo(m_fDistToMove, 4);
	//if (np == 1 || np == 0)//轮子全上升/有升有降
	//	nStaticWheel = GetMinPositiveElementIndex(m_fDistToMove, WHEELS);
	//else 
	//	nStaticWheel = GetMaxNagtiveElementIndex(m_fDistToMove, WHEELS);

	
	nStaticWheel = GetMaxElementIndex(m_fDistToMove, WHEELS);


	for (int i = 0; i < WHEELS; ++i)
	{
		if (i != nStaticWheel)
		{
			
			//float disp = m_fDistToMove[i]-m_fDistToMove[nStaticWheel];
			float disp = (m_fHeightCur[i] - m_fHeightCur[nStaticWheel])
				- (m_pActorBases[i]->getGlobalPosition().z - 
				m_pActorBases[nStaticWheel]->getGlobalPosition().z);

			//if (NxMath::abs(disp) > 0.05)
			{
//				LogText(" w%d : %f", i, disp);
				MoveBaseWithDisp(i, disp);
			}

		}
		else
		{
			//MeltAllWheels();
   //         FreezeActor(m_pActorWheels[i]);		
		}
	}
//	LogText("\n");
}

void ChassisSimulator::MoveBaseWhenTouchGround()
{
	//把base移动到Wheel下方
	//最高的地面以最低的轮子为参照
	int lowestWheel = GetLowestWheel();

	int hightestBase = GetHeightest();

	//最高的base，应该放在这里
	float fHeightestBase = m_pActorWheels[lowestWheel]->getGlobalPosition().z - 
		m_fSpecWheelRadius - 0.08f;

	//移动各个base
	for (int i = 0; i < WHEELS; ++i)
	{
		if (i != hightestBase)
		{
			MoveBase(i, fHeightestBase + m_fHeightCur[i]- m_fHeightCur[hightestBase]);
		}
		else
		{
			MoveBase(i, fHeightestBase);
		}
	}
}

// all positive? nagtive? or ..
int ChassisSimulator::GetPNInfo(float* pArray, int num)
{
	bool bHasPositive = false;
	bool bHasNagtive = false;
	for(int i = 0; i < num; ++i)
	{
		if (bHasNagtive&& bHasPositive)
			break;

		if (pArray[i] < 0.0f)
		{
			bHasNagtive = true;
		}
		else
		{
			bHasPositive = true;
		}
	}

	if (bHasPositive&&bHasNagtive)
		return 0;
	else if (bHasNagtive)
		return -1;
	else
		return 1;
}
int ChassisSimulator::GetMinPositiveElementIndex(float* pArray, int num)
{
	int index = -1;
	for (int i = 0; i < num; ++i)
	{
		if (pArray[i]>=0.0f)
		{
			if (index ==-1)
				index = i;
			else if ((pArray[i]  <= pArray[index]))
				index = i;
		}
	}
	return index;
}

int ChassisSimulator::GetMaxElementIndex(float* pArray, int num)
{
	int index = 0;
	for (int i = 0; i < num; ++i)
	{
		if (pArray[i]  > pArray[index])
			index = i;
	}
	return index;

}

int ChassisSimulator::GetMaxNagtiveElementIndex(float* pArray, int num)
{
	int index = -1;
	for (int i = 0; i < num; ++i)
	{
		if (pArray[i]<0.0f)
		{
			if (index ==-1)
				index = i;
			else if ((pArray[i]  > pArray[index]))
				index = i;
		}
	}
	return index;
}

bool ChassisSimulator::CanIFly(float fDispZThisFrame)
{
	bool bInAir = true;
	for (int i = 0; i < WHEELS; ++i)
	{
		if (m_fDistToMove[i] < 0.0f)
		{//轮子全部下降，且下降的[绝对值]要小于此帧重力方向的位移
			if (m_fDistToMove[i] < fDispZThisFrame)//both nagtive
			{
				bInAir = false;
				break;
			}
		}
		else
		{//还有需要上升的轮子，肯定不会在空中飞。
			bInAir = false;
			break;
		}
	}

	return bInAir;
}
void ChassisSimulator::SetDisplaceMent(float* pArray, int num)
{
	for (int i = 0; i < WHEELS; ++i)
	{
		m_fDistToMove[i] = *(pArray+i);
	}
}
void ChassisSimulator::SetHeight(float* pArray, int num)
{
	for (int i = 0; i < WHEELS; ++i)
	{
		m_fHeightPre[i] = m_fHeightCur[i];
		m_fHeightCur[i] = *(pArray+i);
		//m_fHeightCur[i] += m_fDriftOffset[i];//把漂移升高的高度加在这里

		m_fDistToMove[i] = m_fHeightCur[i] - m_fHeightPre[i];
	}
}

void ChassisSimulator::SetContactState(RuningState st)
{
	m_statePre = m_state;
	m_state = st;
}

int ChassisSimulator::GetLowestWheel()
{
	//LogText("GetLowestWheel(): ");
	int index = 0;
	for (int i = 0; i < WHEELS; ++i)
	{
	//	LogText("w%d.z: %f", i, m_pActorWheels[i]->getGlobalPosition().z);
		if (m_pActorWheels[i]->getGlobalPosition().z < 
			m_pActorWheels[index]->getGlobalPosition().z)
		{
			index = i;
		}
	}
	//LogText(" result: %d\n", index);
	return index;
}
int ChassisSimulator::GetHeightest()
{
	LogText("GetHeightest(): ");
	int index = 0; 
	for (int i = 0; i < WHEELS; ++i)
	{
		LogText("m_fHeightCur[%d]: %f", i,m_fHeightCur[i]);
		if (m_fHeightCur[i] > m_fHeightCur[index])
		{
			index = i;
		}
	}
	LogText("result : %d\n", index);
	return index;
}
float* ChassisSimulator::GetDriftOffsets()
{
	return m_fDriftOffset;
}


bool ChassisSimulator::LoadSuspensionConfig()
{
	CScriptState ls;
	if(ls.DoFile("config/SuspensionCfg.lua") != 0)
		return false;

	CVarTable vtSus("SuspensionParam");
	vtSus.GetFromScript(&ls);

	m_fChassisScale = vtSus.Element("sim_fChassisScale")->Float();

	//m_vPosition.set(vtSus.Element("sim_vPositionx")->Float(),
	//	vtSus.Element("sim_vPositiony")->Float(),
	//	vtSus.Element("sim_vPositionz")->Float());

	NiPoint3 pt = CLevelManager::Get()->GetMapDesc()->kSusspensionPos;
	m_vPosition.set(pt.x, pt.y, pt.z);

	m_vSpecChassis.set(vtSus.Element("sim_vSpecChassisx")->Float(),
		vtSus.Element("sim_vSpecChassisy")->Float(), 
		vtSus.Element("sim_vSpecChassisz")->Float());

	m_vSpecBase.set(vtSus.Element("sim_vSpecBasex")->Float(),
		vtSus.Element("sim_vSpecBasey")->Float(), 
		vtSus.Element("sim_vSpecBasez")->Float());

	m_fSpecWheelRadius = vtSus.Element("sim_fSpecWheelRadius")->Float();

	m_fMassChassis = vtSus.Element("sim_fMassChassis")->Float();
	m_fMassWheel = vtSus.Element("sim_fMassWheel")->Float();


	m_fZBases = vtSus.Element("sim_fZBases")->Float();
	m_fZChassis = vtSus.Element("sim_fZChassis")->Float();
	m_fZPillar = vtSus.Element("sim_fZPillar")->Float();
	m_fZWheels = vtSus.Element("sim_fZWheels")->Float();

	m_fAnchorZSuspensions = vtSus.Element("sim_fAnchorZSuspensions")->Float();
	m_fAnchorZPillar = vtSus.Element("sim_fAnchorZPillar")->Float();
	m_fAnchorChassis = vtSus.Element("sim_fAnchorChassis")->Float();


	m_fSusSpring = vtSus.Element("sim_fSusSpring")->Float();
	m_fSusDamping = vtSus.Element("sim_fSusDamping")->Float();
	m_fSusValue = vtSus.Element("sim_fSusValue")->Float();
	m_fSusRestitution = vtSus.Element("sim_fSusRestitution")->Float();
	m_fSusLimitPlane = vtSus.Element("sim_fSusLimitPlane")->Float();


	m_vBasePoints[1].set(m_vSpecChassis.x, m_vSpecChassis.y, m_fZBases);
	m_vBasePoints[0].set(m_vSpecChassis.x, -m_vSpecChassis.y, m_fZBases);
	m_vBasePoints[3].set(-m_vSpecChassis.x, -m_vSpecChassis.y, m_fZBases);
	m_vBasePoints[2].set(-m_vSpecChassis.x, m_vSpecChassis.y, m_fZBases);


	return true;
}

void ChassisSimulator::StartMassTranslationWhenDrifting(int nDriftState)
{
	if (m_pMassTrans)
		return;
	
	m_pMassTrans = new MASSTRANSINFO;

	if (nDriftState == LEFT_DRIFTING)
	{
		m_pMassTrans->fRadFrom = 0.0f;
		m_pMassTrans->fRadTo = NxMath::degToRad(180.0f);

	}
	else
	{
		m_pMassTrans->fRadFrom = NxMath::degToRad(180.0f);
		m_pMassTrans->fRadTo = NxMath::degToRad(360.0f);

	}
	m_pMassTrans->fVel = NxMath::degToRad(280.0f);
	m_pMassTrans->vMaxDist = NxVec3(.0f, 0.11f, .0f);

}

void ChassisSimulator::UpdateMassTranslation(float dt)
{
	if (m_pMassTrans)
	{
		bool bIsOver = false;
		float fAngleNext = m_pMassTrans->fRadFrom + m_pMassTrans->fVel * dt;

		if (fAngleNext < m_pMassTrans->fRadTo)
		{
			m_pMassTrans->fRadFrom = fAngleNext;
		}
		else
		{
			m_pMassTrans->fRadFrom = m_pMassTrans->fRadTo;
			bIsOver = true;
		}
		NxVec3 vPosLocal = NxMath::sin(m_pMassTrans->fRadFrom)* m_pMassTrans->vMaxDist;
		m_pActorChassis->setCMassOffsetLocalPosition(vPosLocal);

		if (bIsOver)
		{
			delete m_pMassTrans;
			m_pMassTrans = NULL;
		}
	}
}

void ChassisSimulator::OnDriftStart(int nDriftState)
{
//	NewDriftPeriod(nDriftState);
}
void ChassisSimulator::OnDriftEnd(int nDriftState)
{
	//StartMassTranslationWhenDrifting(nDriftState);

	//if (m_eDriftPeriod == DP_DRIFTING)
	//{
	//	SetDriftPeriod(DP_DRIFTEND_FALLING);
	//}

	NewDriftPeriod(nDriftState);
}

void ChassisSimulator::SetDriftPeriod(DRIFTPERIOD eP)
{
	m_eDriftPeriod = eP;
}

void ChassisSimulator::UpdateDrift(float dt)
{
	if (m_bDriftPeriodFinished)	
		return;

	NxVec3 vMassPosLocal(0, 0, 0);

/*	if (m_eDriftPeriod == DP_DRIFTING)
	{
		float fAngleNext = m_massTrans[m_eDriftPeriod].fRadCur + 
			m_massTrans[m_eDriftPeriod].fVel * dt;

		if (fAngleNext < m_massTrans[m_eDriftPeriod].fRadTo)
			m_massTrans[m_eDriftPeriod].fRadCur = fAngleNext;
		else
		{
			m_massTrans[m_eDriftPeriod].fRadCur = m_massTrans[m_eDriftPeriod].fRadTo;
		}
		vMassPosLocal = NxMath::sin(m_massTrans[m_eDriftPeriod].fRadCur)* m_massTrans[m_eDriftPeriod].vMaxDist;
	}
	else*/ if (m_eDriftPeriod == DP_DRIFTEND_FALLING)
	{
		float fAngleNext = m_massTrans[m_eDriftPeriod].fRadCur + 
			m_massTrans[m_eDriftPeriod].fVel * dt;

		if (fAngleNext < m_massTrans[m_eDriftPeriod].fRadTo)
			m_massTrans[m_eDriftPeriod].fRadCur = fAngleNext;
		else
		{
			m_massTrans[m_eDriftPeriod].fRadCur = m_massTrans[m_eDriftPeriod].fRadTo;
			SetDriftPeriod(DP_DRIFTEND_REBOUNCE);//开始漂移反弹
		}
		vMassPosLocal = NxMath::sin(m_massTrans[m_eDriftPeriod].fRadCur)* m_massTrans[m_eDriftPeriod].vMaxDist;
	}
	else if (m_eDriftPeriod == DP_DRIFTEND_REBOUNCE)
	{
		float fAngleNext = m_massTrans[m_eDriftPeriod].fRadCur + 
			m_massTrans[m_eDriftPeriod].fVel * dt;

		if (fAngleNext < m_massTrans[m_eDriftPeriod].fRadTo)
			m_massTrans[m_eDriftPeriod].fRadCur = fAngleNext;
		else
		{
			m_massTrans[m_eDriftPeriod].fRadCur = m_massTrans[m_eDriftPeriod].fRadTo;
			m_bDriftPeriodFinished = true;//结束漂移反弹过程
		}
		vMassPosLocal = NxMath::sin(m_massTrans[m_eDriftPeriod].fRadCur)* m_massTrans[m_eDriftPeriod].vMaxDist;
	}

	m_pActorChassis->setCMassOffsetLocalPosition(vMassPosLocal);
}

void ChassisSimulator::NewDriftPeriod(int nStateCur)
{
	//根据起漂速度来决定各种参数。。。

	m_bDriftPeriodFinished = false;
	SetDriftPeriod(DP_DRIFTEND_FALLING);
	
	float f90 = NxMath::degToRad(90.0f);
	float f180 = f90 * 2.0f;
	float f270 = f90 * 3.0f;
	float f360 = f90 * 4.0f;
	float f450 = f90 * 5.0f;

	////- DP_DRIFTING
	//if (nStateCur == LEFT_DRIFTING)
	//{
	//	m_massTrans[DP_DRIFTING].fRadFrom = f180;
	//	m_massTrans[DP_DRIFTING].fRadCur = f180;
	//	m_massTrans[DP_DRIFTING].fRadTo = f270;

	//}
	//else
	//{
	//	m_massTrans[DP_DRIFTING].fRadFrom = 0.0f;
	//	m_massTrans[DP_DRIFTING].fRadCur = 0.0f;
	//	m_massTrans[DP_DRIFTING].fRadTo = f90;
	//}
	//m_massTrans[DP_DRIFTING].fVel = NxMath::degToRad(280.0f);
	//m_massTrans[DP_DRIFTING].vMaxDist = NxVec3(.0f, 0.11f, .0f);

	//- DP_DRIFTEND_FALLING
	if (nStateCur == LEFT_DRIFTING)
	{
		m_massTrans[DP_DRIFTEND_FALLING].fRadFrom = 0.0f;
		m_massTrans[DP_DRIFTEND_FALLING].fRadCur = 0.0f;
		m_massTrans[DP_DRIFTEND_FALLING].fRadTo = f90;

	}
	else
	{
		m_massTrans[DP_DRIFTEND_FALLING].fRadFrom = f180;
		m_massTrans[DP_DRIFTEND_FALLING].fRadCur = f180;
		m_massTrans[DP_DRIFTEND_FALLING].fRadTo = f270;
	}
	m_massTrans[DP_DRIFTEND_FALLING].fVel = NxMath::degToRad(260.0f);
	m_massTrans[DP_DRIFTEND_FALLING].vMaxDist = NxVec3(.0f, 0.091f, .0f);

	//- DP_DRIFTEND_REBOUNCE
	if (nStateCur == LEFT_DRIFTING)
	{
		m_massTrans[DP_DRIFTEND_REBOUNCE].fRadFrom = f90;
		m_massTrans[DP_DRIFTEND_REBOUNCE].fRadCur = f90;
		m_massTrans[DP_DRIFTEND_REBOUNCE].fRadTo = f180;

	}
	else
	{
		m_massTrans[DP_DRIFTEND_REBOUNCE].fRadFrom = f270;
		m_massTrans[DP_DRIFTEND_REBOUNCE].fRadCur = f270;
		m_massTrans[DP_DRIFTEND_REBOUNCE].fRadTo = f360;
	}
	m_massTrans[DP_DRIFTEND_REBOUNCE].fVel = NxMath::degToRad(300.0f);
	m_massTrans[DP_DRIFTEND_REBOUNCE].vMaxDist = NxVec3(.0f, 0.091f, .0f);

}

float ChassisSimulator::GetDistToGround()
{
	float fDisMin = 9999.9f;

	for (int i = 0; i < WHEELS; i++)
	{
		//float fDist = m_afZofWheels[i] - 
		//	m_fSpecWheelRadius - m_fHeightCur[i];
		float fDist = m_afDistanceToGround[i];

		if (fDist < fDisMin)
		{
			fDisMin = fDist;
		}
	}

	return fDisMin;
}

void ChassisSimulator::SaveWheelsZ()
{
	for (int i = 0; i < WHEELS; i++)
	{
		float a1 = m_pActorWheels[i]->getGlobalPosition().z;
		float a2 = m_pActorBases[i]->getGlobalPosition().z;

		//if (NxMath::abs(a1 - a2) > 0.3)
		//{
		//	m_afZofWheels[i] = m_fHeightCur[i]+ m_fSpecWheelRadius
		//}
		//else
		{
			m_afZofWheels[i] = m_fHeightCur[i]+ 
				m_pActorWheels[i]->getGlobalPosition().z -
				m_pActorBases[i]->getGlobalPosition().z;
		}

	}
}
void ChassisSimulator::UpdateState2(float dt, const NxVec3& velGlobal, const NxVec3& vNormalGrd)
{
	if (!m_bIsFreezed)
	{
		//LogText("当前active, 速度: %f\n", velGlobal.z);
		SaveWheelsZ();
	}
	else
	{
		//LogText("当前freezed, 速度: %f\n", velGlobal.z);
		UpdateWheelsZ(velGlobal.z * dt);
	}

	static float fHeightToFreeze = 0.5f;
	float fDistGround = GetDistToGround();
	float fToFalling = velGlobal.z* dt;

	if (fToFalling < 0.001f)//向下飞
	{
		if (fDistGround < -fToFalling)//这一帧就会落地
		{
			//LogText("距地 %f, 下落距离 %f\n", fDistGround, -fToFalling);
			if (m_bIsFreezed)
			{
				//LogText("\n--------锁定落地瞬间-------");
				MoveBaseWhenTouchGround();
				MeltAllWheels();
				m_bIsFreezed = false;
				NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->SetModalFall();
				LogText("--------解锁-------\n");
			}
		}
		else
		{
			if (!m_bIsFreezed)
			{
				if (fDistGround > fHeightToFreeze)
				{
					//LogText("下落中 。。。距地 %f, 锁定阈值 %f\n", fDistGround, fHeightToFreeze);

					FreezeAllWheels();
					m_bIsFreezed = true;
					LogText("\n--------坠落中,距地超过(0.5)，锁定我！-------\n");
				}

			}
		}

	}
	else
	{
		if (!m_bIsFreezed)
		{
			if (fDistGround > fHeightToFreeze)
			{
				LogText("\n--------向上飞，超过一定高度(0.5)，锁定我！-------\n");
				FreezeAllWheels();
				m_bIsFreezed = true;
			}
		}
	}
	MoveBasesWhenOnEarth();
}
void ChassisSimulator::UpdateWheelsZ(float dist)
{
	for (int i = 0; i < WHEELS; i++)
	{
		m_afZofWheels[i] += dist;
	}
	//LogText("轮子 0 高度：%f\n", m_afZofWheels[0]);
}
void ChassisSimulator::SetWheelsZ(float* zs)
{
	for (int i = 0; i < WHEELS; i++)
	{
		m_afZofWheels[i] = *(zs+i);
	}
	//LogText("轮子 0 高度：%f\n", m_afZofWheels[0]);
}

NxMat33 ChassisSimulator::GetChassisOrientationSmooth()
{
	NiMatrix3 niMat;
	NiPhysXTypes::NxMat33ToNiMatrix3(m_pActorChassis->getGlobalOrientation(), niMat);
	
	float x, y, z;
	float x1, y1;//, z1;
	niMat.ToEulerAnglesXYZ(x, y, z);
	//0.017 = 1 degree
	if (NxMath::abs(x - m_fOrientationEulerXPre) < 0.02)
		x1 = m_fOrientationEulerXPre;
	else
		x1 = m_fOrientationEulerXPre + (x - m_fOrientationEulerXPre)* 0.5f;

	if (NxMath::abs(y - m_fOrientationEulerYPre) < 0.02)
		y1 = m_fOrientationEulerYPre;
	else
		y1 = m_fOrientationEulerYPre + (y - m_fOrientationEulerYPre)* 0.5f;

	niMat.FromEulerAnglesXYZ(x1, y1, 0.0f);
	NxMat33 nxRet;
	NiPhysXTypes::NiMatrix3ToNxMat33(niMat, nxRet);

	m_fOrientationEulerXPre = x1;
	m_fOrientationEulerYPre = y1;
    
	return nxRet;
}
void ChassisSimulator::SetGlobalPosition(NxVec3 pos)
{
	if (!m_pActorChassis)
		return;
	//LogText("\n&*&*&*&*&*&*&*被导弹击中拉！！&*&*&*&*&*&*&\n");
	//外面重新设置了车辆的位置，因此，要更新一下轮子的位置,（这个函数会被调用多次，在这里记录轮子位置会有问题）
	//float fZofChassis = m_fHeightCur[0] + (m_pActorChassis->getGlobalPosition().z - m_pActorBases[0]->getGlobalPosition().z);
	//float fOffset = pos.z - fZofChassis;

	//for (int i = 0; i < WHEELS; i++)
	//{
	//	m_afZofWheels[i] += fOffset;
	//}

	FreezeAllWheels();
	m_bIsFreezed = true;
}

void ChassisSimulator::SetDistanceToGround(float *aDist)
{
	for (int i = 0; i < WHEELS; i++)
	{
		m_afDistanceToGround[i] = *(aDist+i);
	}
}

