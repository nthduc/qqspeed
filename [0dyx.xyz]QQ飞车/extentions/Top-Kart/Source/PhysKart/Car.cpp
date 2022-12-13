
#include <NxCooking.h>

#include "NxVehicle.h"

#include "NxAllVehicles.h"

#include "Stream.h"

#define GL_COLOR_WHITE		1.0f,1.0f,1.0f,1.0f
#define GL_COLOR_RED		1.0f,0.0f,0.0f,1.0f
#define GL_COLOR_GREEN		0.0f,1.0f,0.0f,1.0f
#define GL_COLOR_BLUE		0.0f,0.0f,1.0f,1.0f
#define GL_COLOR_LIGHT_BLUE 0.5f,0.5f,1.0f,1.0f
#define GL_COLOR_LIGHT_GREY 0.7f,0.7f,0.7f,1.0f
#define GL_COLOR_BLACK		0.0f,0.0f,0.0f,1.0f

NxScene*	gScene = NULL;
/*
const bool frontWheelIsPowered = true;
const bool rearWheelIsPowered = false;
const NxReal steeringDelta = 0.05;
const NxReal maxMotorPower = 40.0f;
const NxReal maxSteeringAngle = NxMath::degToRad(45.0f);
const NxReal suspSpring = 70.0f; //70.0f;
const NxReal suspDamping = 5.0f; //5.0f; 
const NxReal suspBias = -0.25f; 
const NxReal carMass = 30; 


NxActor * lastCar = 0;

NxMaterialIndex raycastWheelMaterialIndex;


struct CarWheelContact
	{
	NxActor * car;
	NxShape * wheel;
	NxVec3 contactPoint;
	};

NxArray<CarWheelContact> wheelContactPoints;


struct WheelShapeUserData
	{
	bool frontWheel;
	}
wheelUserDatas[4];


NxMaterialDesc raycastWheelMaterial;


NxSpringDesc wheelSpring;

struct CarData
	{
	NxReal _;	//add stuff later maybe.
	} carData;

bool isCar(NxActor * actor)
	{
	return ((&carData) == (CarData *)(actor->userData));
	}
*/
class MyContactReport : public NxUserContactReport
{
public:
	virtual void  onContactNotify(NxContactPair& pair, NxU32 events)
	{
		NxI32 carIndex = -1;
		NxAllVehicles::handlePair(pair, events);
		/*
		if(isCar(pair.actors[0]))
			carIndex = 0;
		else if(isCar(pair.actors[1]))
			carIndex = 1;
		else
			return;
		//ignore the 'both are cars' case for now.


		// Iterate through contact points
		NxContactStreamIterator i(pair.stream);
		//user can call getNumPairs() here
		while(i.goNextPair())
			{
			//user can also call getShape() and getNumPatches() here
			NxShape * s = i.getShape(carIndex);
			while(i.goNextPatch())
				{
				//user can also call getPatchNormal() and getNumPoints() here
				const NxVec3& contactNormal = i.getPatchNormal();
				while(i.goNextPoint())
					{
					//user can also call getPoint() and getSeparation() here
					const NxVec3& contactPoint = i.getPoint();

					//add forces:

					//assuming front wheel drive we need to apply a force at the wheels.
					if (s->is(NX_SHAPE_CAPSULE))		//assuming only the wheels of the car are capsules, otherwise we need more checks.
														//this branch can't be pulled out of loops because we have to do a full iteration through the stream
						{
						CarWheelContact cwc;
						cwc.car = pair.actors[carIndex];
						cwc.wheel = s;
						cwc.contactPoint = contactPoint;
						wheelContactPoints.pushBack(cwc);
						//#error too bad this is illegal (reentry) and also technically busted because the accumulators get zeroed after this returns.
						//pair.actors[carIndex]->addForceAtPos(NxVec3(100,0,0),contactPoint);
						}
					}
				}		
			}

*/
	}

}	carContactReportObj;

NxUserContactReport * carContactReport = &carContactReportObj;
/*

void tickCar()
	{
	NxReal motorForce;
	static NxReal steeringValue = 0;


	if (keyDown['w'])	motorForce = maxMotorPower;
	else if (keyDown['s'])	motorForce = -maxMotorPower;
	else motorForce = 0;


	if (keyDown['a'])
		{
		if (steeringValue > (-1.0f + steeringDelta) )	steeringValue -= steeringDelta;
		}
	else if (keyDown['d'])
		{
		if (steeringValue < (1.0f - steeringDelta))	steeringValue += steeringDelta;
		}
	else 
		steeringValue = 0;

	NxReal steeringAngle = steeringValue * maxSteeringAngle;




	NxArray<CarWheelContact>::iterator i = wheelContactPoints.begin();
	while(i != wheelContactPoints.end())
		{
		CarWheelContact & cwc = *i;

		WheelShapeUserData * wheelData = (WheelShapeUserData *)(cwc.wheel->userData);

		//apply to powered wheels only.
		if (wheelData->frontWheel)
			{
			//steering:
			NxMat33 wheelOrientation = cwc.wheel->getLocalOrientation();
			wheelOrientation.setColumn(0,  NxVec3(NxMath::cos(steeringAngle), 0,  NxMath::sin(steeringAngle) ));
			wheelOrientation.setColumn(2,  NxVec3(NxMath::sin(steeringAngle), 0, -NxMath::cos(steeringAngle) ));
			cwc.wheel->setLocalOrientation(wheelOrientation);



			if (frontWheelIsPowered)
				{
				//get the world space orientation:
				wheelOrientation = cwc.wheel->getGlobalOrientation();
				NxVec3 steeringDirection;
				wheelOrientation.getColumn(0, steeringDirection);


				//the power direction of the front wheel is the wheel's axis as it is steered.
				if (motorForce)
					cwc.car->addForceAtPos(steeringDirection * motorForce,cwc.contactPoint);


				}
			}
		if (!wheelData->frontWheel && rearWheelIsPowered)
			{
			//get the orientation of this car:
			NxMat33 m = cwc.car->getGlobalOrientation();
			NxVec3 carForwardAxis;
			m.getColumn(0, carForwardAxis);
			//the power direction of the rear wheel is always the car's length axis.
			cwc.car->addForceAtPos(carForwardAxis * motorForce,cwc.contactPoint);
			}
		i++;
		}

	wheelContactPoints.clear();

	}
*/
void createCarWithDesc(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, bool corvette, bool monsterTruck, bool oldStyle, NxPhysicsSDK* physicsSDK)
{
	NxVehicleDesc vehicleDesc;
	NxBoxShapeDesc boxShapes[2];
	NxConvexShapeDesc carShape[2];
	if(corvette)
	{
		NxArray<NxVec3> points;
		NxArray<NxVec3> points2;
		NxReal halfWidth = 1.3f;
		NxReal halfLength = 2.4f;
		NxReal halfHeight = 0.7;
		points.pushBack().set(halfLength,-halfHeight * 0.1f, 0);
		points.pushBack().set(halfLength * 0.7f, 0, 0);
		points.pushBack().set(0.2f * halfLength, halfHeight * 0.2f, 0);
		points.pushBack().set(-halfLength, halfHeight * 0.2f, 0);
		points.pushBack().set(0.1*halfLength, halfHeight * 0.2f, halfWidth * 0.9f);
		points.pushBack().set(0.1*halfLength, halfHeight * 0.2f,-halfWidth * 0.9f);
		points.pushBack().set(-0.8*halfLength, halfHeight * 0.2f, halfWidth * 0.9f);
		points.pushBack().set(-0.8*halfLength, halfHeight * 0.2f,-halfWidth * 0.9f);

		points.pushBack().set(halfLength * 0.9f,-halfHeight * 0.25f, halfWidth * 0.8f);
		points.pushBack().set(halfLength * 0.9f,-halfHeight * 0.25f,-halfWidth * 0.8f);
		points.pushBack().set(0,-halfHeight * 0.2f, halfWidth);
		points.pushBack().set(0,-halfHeight * 0.2f,-halfWidth);
		points.pushBack().set(-halfLength * 0.9f,-halfHeight * 0.2f, halfWidth * 0.9f);
		points.pushBack().set(-halfLength * 0.9f,-halfHeight * 0.2f,-halfWidth * 0.9f);

		points.pushBack().set(halfLength * 0.8f, -halfHeight, halfWidth * 0.79f);
		points.pushBack().set(halfLength * 0.8f, -halfHeight,-halfWidth * 0.79f);
		points.pushBack().set(-halfLength * 0.8f, -halfHeight, halfWidth * 0.79f);
		points.pushBack().set(-halfLength * 0.8f, -halfHeight,-halfWidth * 0.79f);

		for(NxU32 i = 2; i < 8; i++)
		{
			points2.pushBack(points[i]);
		}

		points2.pushBack().set(-0.5*halfLength, halfHeight*0.8f, halfWidth*0.7f);
		points2.pushBack().set(-0.5*halfLength, halfHeight*0.8f,-halfWidth*0.7f);
		points2.pushBack().set(-0.7*halfLength, halfHeight*0.7f, halfWidth*0.7f);
		points2.pushBack().set(-0.7*halfLength, halfHeight*0.7f,-halfWidth*0.7f);


		static NxConvexMeshDesc convexMesh;
		convexMesh.numVertices = points.size();
		convexMesh.points = &(points[0].x);
		convexMesh.pointStrideBytes = sizeof(NxVec3);
		convexMesh.flags |= NX_CF_COMPUTE_CONVEX|NX_CF_USE_LEGACY_COOKER;

		MemoryWriteBuffer buf;
		bool status = NxCookConvexMesh(convexMesh, buf);
		if(status)
		{
			carShape[0].meshData = physicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
			vehicleDesc.carShapes.pushBack(&carShape[0]);
		}

		static NxConvexMeshDesc convexMesh2;
		convexMesh2.numVertices = points2.size();
		convexMesh2.points = (&points2[0].x);
		convexMesh2.pointStrideBytes = sizeof(NxVec3);
		convexMesh2.flags = NX_CF_COMPUTE_CONVEX|NX_CF_USE_LEGACY_COOKER;

		MemoryWriteBuffer buf2;
		status = NxCookConvexMesh(convexMesh2, buf2);
		if(status)
		{
			carShape[1].meshData = physicsSDK->createConvexMesh(MemoryReadBuffer(buf2.data));
			vehicleDesc.carShapes.pushBack(&carShape[1]);
		}

	} else {
		boxShapes[0].dimensions.set(2.5f, 0.4f, 1.2f);
		boxShapes[1].dimensions.set(1.f, 0.3f, 1.1f);
		boxShapes[1].localPose.t.set(-0.3f, 0.7f, 0.f);
		vehicleDesc.carShapes.pushBack(&boxShapes[0]);
		vehicleDesc.carShapes.pushBack(&boxShapes[1]);
	}

	vehicleDesc.position				= pos;
	vehicleDesc.mass					= 1200;//monsterTruck ? 12000 : 
	vehicleDesc.digitalSteeringDelta	= 0.04f;
	vehicleDesc.steeringMaxAngle		= 30.f;
	vehicleDesc.motorForce				= 3500.f;//monsterTruck?180.f:
	vehicleDesc.maxVelocity				= 30.f;//(monsterTruck)?20.f:
	vehicleDesc.cameraDistance			= 8.0f;

	NxVehicleMotorDesc motorDesc;
	NxVehicleGearDesc gearDesc;
	NxReal wheelRadius = 0.4f;
	if(corvette)
	{
		vehicleDesc.maxVelocity = (monsterTruck)?40.f:80.f;
		motorDesc.setToCorvette();
		vehicleDesc.motorDesc = &motorDesc;
		gearDesc.setToCorvette();
		vehicleDesc.gearDesc = &gearDesc;
		vehicleDesc.differentialRatio = 3.42f;
		if(monsterTruck)
		{
			vehicleDesc.differentialRatio*= 6.f;
			vehicleDesc.mass *= 2;
		}
		//vehicleDesc.differentialRatio = 5.f;
		wheelRadius = 0.33f;
		vehicleDesc.centerOfMass.set(0,-0.5f,0);
	}
	else
		vehicleDesc.centerOfMass.set(0,monsterTruck?-2:0,0);


	NxWheelDesc wheelDesc[4];
	for(NxU32 i=0;i<4;i++)
	{
		//wheelDesc[i].wheelAxis.set(0,0,1);
		//wheelDesc[i].downAxis.set(0,-1,0);
		wheelDesc[i].wheelApproximation = 10;
		//wheelDesc[i].wheelFlags |= NX_WF_BUILD_LOWER_HALF;
		wheelDesc[i].wheelRadius = (monsterTruck)?wheelRadius*3.f:wheelRadius;
		wheelDesc[i].wheelWidth = (monsterTruck)?0.3f:0.1f;
		wheelDesc[i].wheelSuspension = (monsterTruck)?0.6f:0.2f;
		wheelDesc[i].springRestitution = monsterTruck?(corvette?5000:4000):7000;
		wheelDesc[i].springDamping = 800;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = monsterTruck?0.5f:1.f;
		if (oldStyle)
			{
			wheelDesc[i].frictionToFront = 0.1f;
			wheelDesc[i].frictionToSide = corvette ? 0.1f : 0.99f;
			}
		else
			{
			wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
			}
		vehicleDesc.carWheels.pushBack(&wheelDesc[i]);
	}
	NxReal widthPos = (monsterTruck)?1.4f:1.09f;
	NxReal heightPos = -0.4f;	//(monsterTruck)?1.f:
	wheelDesc[0].position.set( 1.8f, heightPos, widthPos);
	wheelDesc[1].position.set( 1.8f, heightPos,-widthPos);
	wheelDesc[2].position.set(-1.5f, heightPos, widthPos);
	wheelDesc[3].position.set(-1.5f, heightPos,-widthPos);
	NxU32 flags = NX_WF_BUILD_LOWER_HALF;
	wheelDesc[0].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[1].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[2].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;
	wheelDesc[3].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;

	vehicleDesc.steeringSteerPoint.set(1.8, 0, 0);
	vehicleDesc.steeringTurnPoint.set(-1.5, 0, 0);

	NxVehicle* vehicle = NxVehicle::createVehicle(gScene, &vehicleDesc);
	NxQuat q;
	q.fromAngleAxis(180.0f, NxVec3(0.0f, 1.0f, 0.0f));
	vehicle->getActor()->setGlobalOrientationQuat(q);
			
}

void createCart(const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, bool oldStyle)
{
	NxVehicleDesc cartDesc;
	NxBoxShapeDesc boxShapes[5];
	NxCapsuleShapeDesc capsuleShapes[1];

	bool corvette = false;

	boxShapes[0].dimensions.set(0.8f, 0.02f, 0.5f);
	boxShapes[0].localPose.t.set(0,0,0);
	
	boxShapes[1].dimensions. set( 0.8f, 0.05f, 0.02f);
	boxShapes[1].localPose.t.set( 0.0f, 0.07f, 0.48f);
	
	boxShapes[2].dimensions. set( 0.8f, 0.05f, 0.02f);
	boxShapes[2].localPose.t.set( 0.0f, 0.07f,-0.48f);
	
	boxShapes[3].dimensions. set(0.02f, 0.05f, 0.46f);
	boxShapes[3].localPose.t.set(0.78f, 0.07f,-0.0f);
	
	boxShapes[4].dimensions. set(0.02f, 0.05f, 0.46f);
	boxShapes[4].localPose.t.set(-0.78f, 0.07f,-0.0f);

	capsuleShapes[0].height = 0.1f;
	capsuleShapes[0].radius = 0.4f;
	capsuleShapes[0].localPose.t.set(0,0.4f,0);
    
	cartDesc.carShapes.pushBack(&boxShapes[0]);
	cartDesc.carShapes.pushBack(&boxShapes[1]);
	cartDesc.carShapes.pushBack(&boxShapes[2]);
	cartDesc.carShapes.pushBack(&boxShapes[3]);
	cartDesc.carShapes.pushBack(&boxShapes[4]);
	cartDesc.carShapes.pushBack(&capsuleShapes[0]);

	cartDesc.position				= pos;
	cartDesc.mass					= 200;
	cartDesc.digitalSteeringDelta	= 0.1f;
	cartDesc.steeringMaxAngle		= 30.f;
	cartDesc.motorForce				= 3000.f; //3000.f;
	cartDesc.centerOfMass.set(0,0,0);
	cartDesc.maxVelocity			= 30.f; //30.f;
	cartDesc.cameraDistance		= 3.0f;

	NxVehicleMotorDesc motorDesc;
	NxVehicleGearDesc gearDesc;
	NxReal wheelRadius = 0.1f;
	if(corvette)
	{
		motorDesc.setToCorvette();
		cartDesc.motorDesc = &motorDesc;
		gearDesc.setToCorvette();
		cartDesc.gearDesc = &gearDesc;
		cartDesc.differentialRatio = 3.42f;
		//cartDesc.differentialRatio = 5.f;
		wheelRadius = 0.33f;
	}

	NxWheelDesc wheelDesc[4];
	for(NxU32 i = 0; i < 4; i++)
	{
		wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelRadius = wheelRadius;;
		wheelDesc[i].wheelWidth = 0.08f;
		wheelDesc[i].wheelSuspension = 0.1f;
		wheelDesc[i].springRestitution = 500200;
		wheelDesc[i].springDamping = 80;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 1;
		if (oldStyle)
			{
			wheelDesc[i].frictionToFront = 0.1f;
			wheelDesc[i].frictionToSide = 0.99f;
			}
		if (!oldStyle)
			{
			wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
			}
		cartDesc.carWheels.pushBack(&wheelDesc[i]);
	}
	NxReal frontPos = 0.7f;
	NxReal widthPos = 0.7f;
	NxReal heightPos = -0.0f;
	wheelDesc[0].position.set( frontPos, heightPos, widthPos);
	wheelDesc[1].position.set( frontPos, heightPos,-widthPos);
	wheelDesc[2].position.set(-frontPos, heightPos, widthPos);
	wheelDesc[3].position.set(-frontPos, heightPos,-widthPos);
	NxU32 flags = 0;
	wheelDesc[0].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[1].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[2].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;
	wheelDesc[3].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;

	cartDesc.steeringSteerPoint.set(frontPos, 0, 0);
	cartDesc.steeringTurnPoint.set(-frontPos, 0, 0);

	NxVehicle* vehicle = NxVehicle::createVehicle(gScene, &cartDesc);
	NxQuat q;
	q.fromAngleAxis(180.0f, NxVec3(0.0f, 1.0f, 0.0f));
	vehicle->getActor()->setGlobalOrientationQuat(q);

}

NxVehicleDesc* createTruckPullerDesc(const NxVec3& pos, NxU32 nbGears, bool oldStyle)
{
	static NxVehicleDesc truckDesc;
	static NxBoxShapeDesc boxShapes[3];

	truckDesc.setToDefault();

	boxShapes[0].dimensions. set(2.f, 0.2f, 1.2f);
	boxShapes[0].localPose.t.set(0,0,0);

	boxShapes[1].dimensions. set( 0.8f, 1.2f, 1.25f);
	boxShapes[1].localPose.t.set( 1.4f, 1.4f, 0.0f);
	
	boxShapes[2].dimensions. set( 0.8f, 0.1f, 0.5f);
	boxShapes[2].localPose.t.set(-1.2f, 0.3f, 0.0f);

	truckDesc.carShapes.pushBack(&boxShapes[0]);
	truckDesc.carShapes.pushBack(&boxShapes[1]);
	truckDesc.carShapes.pushBack(&boxShapes[2]);

	truckDesc.position				= pos;
	truckDesc.mass					= 2000;
	truckDesc.digitalSteeringDelta	= 0.1f;
	truckDesc.steeringMaxAngle		= 30.f;
	truckDesc.motorForce				= 200.f;
	truckDesc.centerOfMass.set(0,0,0);
	truckDesc.maxVelocity			= 30.f;
	truckDesc.cameraDistance		= 20.0f;

	static NxVehicleMotorDesc motorDesc;
	static NxVehicleGearDesc gearDesc;

	motorDesc.minRpmToGearDown	= 1500;
	motorDesc.maxRpmToGearUp	= 4000;
	motorDesc.minRpm			= 1000;
	motorDesc.maxRpm			= 5000;
	motorDesc.torqueCurve.insert(1000, 400);
	motorDesc.torqueCurve.insert(3000, 500);
	motorDesc.torqueCurve.insert(5000, 300);
	truckDesc.motorDesc = &motorDesc;

	if(nbGears > gearDesc.getMaxNumOfGears())
	{
		return NULL;
	}

	for(NxU32 i = 1; i <= nbGears; i++)
	{
		gearDesc.forwardGearRatios[i-1] = nbGears / (NxReal)i;
	}
	gearDesc.nbForwardGears = nbGears;
	gearDesc.backwardGearRatio = -11;
	truckDesc.gearDesc = &gearDesc;
	truckDesc.differentialRatio = 3.42f;

	static NxWheelDesc wheelDesc[4];
	for(NxU32 i = 0; i < 4; i++)
	{
		wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelRadius = 0.4f;
		wheelDesc[i].wheelWidth = i>1?0.2f:0.15f;
		wheelDesc[i].wheelSuspension = 0.2f;
		wheelDesc[i].springRestitution = 8000;
		wheelDesc[i].springDamping = 500;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 1;

		if(oldStyle)
			{
			wheelDesc[i].frictionToFront = 0.1f;
			wheelDesc[i].frictionToSide = 0.99f;
			}
		else
			{
			wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
			}
		truckDesc.carWheels.pushBack(&wheelDesc[i]);
	}
	NxReal frontPos = 1.4f;
	NxReal widthPos = 1.01f;
	NxReal heightPos = -0.1f;
	wheelDesc[0].position.set( frontPos, heightPos, widthPos);
	wheelDesc[1].position.set( frontPos, heightPos,-widthPos);
	wheelDesc[2].position.set(-frontPos, heightPos, widthPos);
	wheelDesc[3].position.set(-frontPos, heightPos,-widthPos);
	NxU32 flags = 0;
	bool frontWheelDrive = true;
	bool backWheelDrive = true;
	wheelDesc[0].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[1].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_STEERABLE_INPUT | flags;
	wheelDesc[2].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;
	wheelDesc[3].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | NX_WF_AFFECTED_BY_HANDBRAKE | flags;

	truckDesc.steeringSteerPoint.set(frontPos, 0, 0);
	truckDesc.steeringTurnPoint.set(-frontPos, 0, 0);

	return &truckDesc;
	//NxVehicle* vehicle = NxVehicle::createVehicle(gScene, truckDesc);
	//return vehicle;
}

NxVehicleDesc* createTruckTrailer1(const NxVec3& pos, NxReal length, bool oldStyle)
{
	static NxVehicleDesc truckTrailerDesc;
	static NxBoxShapeDesc boxShapes[2];

	boxShapes[0].dimensions. set(length/2.f, 1.4f, 1.2f);
	boxShapes[0].localPose.t.set(0,1.9f,0);

	boxShapes[1].dimensions. set( 1.5f, 0.1f, 1.2f);
	boxShapes[1].localPose.t.set(-length/2.f+1.5f, 0.4f, 0.0f);

	truckTrailerDesc.carShapes.pushBack(&boxShapes[0]);
	truckTrailerDesc.carShapes.pushBack(&boxShapes[1]);

	truckTrailerDesc.position				= pos;
	truckTrailerDesc.mass					= 12000;
	//truckTrailerDesc.digitalSteeringDelta	= 0.1f;
	//truckTrailerDesc.steeringMaxAngle		= 30.f;
	//truckTrailerDesc.motorForce				= 200.f;
	truckTrailerDesc.centerOfMass.set(0,0,0);
	truckTrailerDesc.maxVelocity			= 30.f;

	static NxWheelDesc wheelDesc[4];
	for(NxU32 i = 0; i < 4; i++){
		wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelRadius = 0.4f;
		wheelDesc[i].wheelWidth = i>1?0.2f:0.15f;
		wheelDesc[i].wheelSuspension = 0.2f;
		wheelDesc[i].springRestitution = truckTrailerDesc.mass *4.0f/3.0f;
		wheelDesc[i].springDamping = truckTrailerDesc.mass / 6.0f;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 1;
		if(oldStyle)
			{
			wheelDesc[i].frictionToFront = 0.1f;
			wheelDesc[i].frictionToSide = 0.99f;
			}
		else
			{
			wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
			}
		truckTrailerDesc.carWheels.pushBack(&wheelDesc[i]);
	}
	NxReal frontOffset = -length / 2.f + 1.5f;
	NxReal frontPos = 1.0f;
	NxReal widthPos = 1.0f;
	NxReal heightPos = -0.1f;
	wheelDesc[0].position.set(frontOffset + frontPos, heightPos, widthPos);
	wheelDesc[1].position.set(frontOffset + frontPos, heightPos,-widthPos);
	wheelDesc[2].position.set(frontOffset - frontPos, heightPos, widthPos);
	wheelDesc[3].position.set(frontOffset - frontPos, heightPos,-widthPos);
	NxU32 flags = 0;
	bool frontWheelDrive = false;
	bool backWheelDrive = false;
	wheelDesc[0].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | flags;
	wheelDesc[1].wheelFlags |= (frontWheelDrive?NX_WF_ACCELERATED:0) | flags;
	wheelDesc[2].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | flags;
	wheelDesc[3].wheelFlags |= (backWheelDrive?NX_WF_ACCELERATED:0) | flags;

	truckTrailerDesc.steeringSteerPoint.set(frontPos, 0, 0);
	truckTrailerDesc.steeringTurnPoint.set(-frontPos, 0, 0);

	return &truckTrailerDesc;}

NxVehicle* createTruckPuller(const NxVec3& pos, NxU32 nbGears, bool oldStyle)
{
	NxVehicleDesc* truckPullerDesc = createTruckPullerDesc(pos, nbGears, oldStyle);
	NxVehicle * vehicle = NxVehicle::createVehicle(gScene, truckPullerDesc);
	NxQuat q;
	q.fromAngleAxis(180.0f, NxVec3(0.0f, 1.0f, 0.0f));
	vehicle->getActor()->setGlobalOrientationQuat(q);
	return vehicle;

}

NxVehicle* createTruckWithTrailer1(const NxVec3& pos, NxU32 nbGears, bool oldStyle)
{
	NxVehicleDesc* truckPullerDesc = createTruckPullerDesc(pos, 32, oldStyle);

	NxReal length = NxMath::rand(6.f, 10.f);
	NxReal jointXPos = -1.0f;

	NxVehicleDesc* truckTrailerDesc = createTruckTrailer1(pos+NxVec3(1.f+jointXPos-length/2,0,0), length, oldStyle);

	if(truckPullerDesc && truckTrailerDesc)
	{
		truckPullerDesc->carWheels[0]->springRestitution *= 1.5f;
		truckPullerDesc->carWheels[1]->springRestitution *= 1.5f;
		truckPullerDesc->carWheels[2]->springRestitution = 2*truckTrailerDesc->carWheels[0]->springRestitution;
		truckPullerDesc->carWheels[3]->springRestitution = 2*truckTrailerDesc->carWheels[0]->springRestitution;
		truckPullerDesc->carWheels[2]->springDamping = 2*truckTrailerDesc->carWheels[0]->springDamping;
		truckPullerDesc->carWheels[3]->springDamping = 2*truckTrailerDesc->carWheels[0]->springDamping;
		//truckPullerDesc->carWheels[0]->wheelSuspension = 0.4f;
		//truckPullerDesc->carWheels[1]->wheelSuspension = 0.4f;
		//truckPullerDesc->carWheels[2]->wheelSuspension = 0.4f;
		//truckPullerDesc->carWheels[3]->wheelSuspension = 0.4f;

		truckPullerDesc->cameraDistance = length + 4.f;
		truckPullerDesc->children.pushBack(truckTrailerDesc);
	}


	NxVehicle* truck = NxVehicle::createVehicle(gScene, truckPullerDesc);

	if(truck->nbChildren() > 0)
	{
#if 1
		static NxD6JointDesc joint;
		joint.actor[0] = truck->getActor();
		joint.actor[1] = truck->getChild(0)->getActor();

		joint.setGlobalAnchor(pos + NxVec3(jointXPos,0.5f,0));
		joint.setGlobalAxis(NxVec3(1,0,0));

		joint.xMotion = NX_D6JOINT_MOTION_LOCKED;
		joint.yMotion = NX_D6JOINT_MOTION_LOCKED;
		joint.zMotion = NX_D6JOINT_MOTION_LOCKED;
		joint.swing1Motion = NX_D6JOINT_MOTION_LIMITED;
		joint.swing2Motion = NX_D6JOINT_MOTION_LIMITED;
		joint.twistMotion = NX_D6JOINT_MOTION_LIMITED;

		joint.swing1Limit.value = 0.3f;
		joint.swing2Limit.value = 1.8f;
		joint.twistLimit.low.value = -0.1f;
		joint.twistLimit.high.value = 0.1f;
#else
		static NxRevoluteJointDesc joint;
		joint.actor[0] = truck->getActor();
		joint.actor[1] = truck->getChild(0)->getActor();

		joint.setGlobalAnchor(pos + NxVec3(jointXPos,0.5f,0));
		joint.setGlobalAxis(NxVec3(0,1,0));

		joint.flags |= NX_RJF_LIMIT_ENABLED;

		joint.limit.low.value = -3.14159f/2.f;
		joint.limit.high.value = 3.14159f/2.f;
#endif
		gScene->createJoint(joint);

	}
	return truck;
}

NxVehicleDesc* createFullTruckDesc(const NxVec3& pos, NxReal length, NxU32 nbGears, bool has4Axes, bool oldStyle)
{
	static NxVehicleDesc truckDesc;

	truckDesc.setToDefault();
	static NxBoxShapeDesc boxShapes[3];

	truckDesc.setToDefault();

	boxShapes[0].dimensions. set(length/2.f, 0.2f, 1.2f);
	boxShapes[0].localPose.t.set(0,0,0);

	boxShapes[1].dimensions. set( 0.8f, 1.2f, 1.25f);
	boxShapes[1].localPose.t.set( length/2.f - 0.6f, 1.4f, 0.0f);

	boxShapes[2].dimensions. set( length/2.f -0.7f, 1.3f, 1.3f);
	boxShapes[2].localPose.t.set(-0.8f, 1.5f, 0.0f);

	truckDesc.carShapes.pushBack(&boxShapes[0]);
	truckDesc.carShapes.pushBack(&boxShapes[1]);
	truckDesc.carShapes.pushBack(&boxShapes[2]);

	truckDesc.position				= pos;
	truckDesc.mass					= 10000;
	truckDesc.digitalSteeringDelta	= 0.1f;
	truckDesc.steeringMaxAngle		= 30.f;
	truckDesc.motorForce				= 200.f;
	truckDesc.centerOfMass.set(0,1,0);
	truckDesc.maxVelocity			= 30.f;
	truckDesc.cameraDistance		= 20.0f;

	static NxVehicleMotorDesc motorDesc;
	static NxVehicleGearDesc gearDesc;

	motorDesc.minRpmToGearDown	= 1500;
	motorDesc.maxRpmToGearUp	= 4000;
	motorDesc.minRpm			= 1000;
	motorDesc.maxRpm			= 5000;
	motorDesc.torqueCurve.insert(1000, 400);
	motorDesc.torqueCurve.insert(3000, 500);
	motorDesc.torqueCurve.insert(5000, 300);
	truckDesc.motorDesc = &motorDesc;

	if(nbGears > gearDesc.getMaxNumOfGears())
	{
		return NULL;
	}
	
	for(NxU32 i = 1; i <= nbGears; i++)
	{
		gearDesc.forwardGearRatios[i-1] = nbGears / (NxReal)i;
	}
	gearDesc.nbForwardGears = nbGears;
	gearDesc.backwardGearRatio = -11;
	truckDesc.gearDesc = &gearDesc;
	truckDesc.differentialRatio = 3.42f;

	static NxWheelDesc wheelDesc[8];
	NxU32 axes = has4Axes?4:2;
	for(NxU32 i = 0; i < axes*2; i++)
	{
		wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelRadius = 0.4f;
		wheelDesc[i].wheelWidth = i>1?0.2f:0.15f;
		wheelDesc[i].wheelSuspension = 0.2f;
		wheelDesc[i].springRestitution = 160000 / (NxReal)axes;
		wheelDesc[i].springDamping = 10000 / (NxReal)axes;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 1;
		if(oldStyle)
			{
			wheelDesc[i].frictionToFront = 0.1f;
			wheelDesc[i].frictionToSide = 0.99f;
			}
		else
			{
			wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
			}
		truckDesc.carWheels.pushBack(&wheelDesc[i]);
	}
	NxReal frontOffset = 0.4f;
	NxReal frontPos = length / 3.2f;
	NxReal widthPos = 1.2f;
	NxReal heightPos = -0.1f;

	NxReal back = frontOffset - frontPos;
	NxReal front = frontOffset + frontPos;
	NxReal raxes = has4Axes?(NxReal)axes:(NxReal)(axes-1);
	for(NxU32 i = 0; i < axes; i++)
	{
		NxU32 wheel = i*2;
		NxReal front01 = (NxReal)(i>1?i+1:i) / raxes;
		wheelDesc[wheel+0].position.set(front01 * back + (1-front01)*front, heightPos, widthPos);
		wheelDesc[wheel+1].position.set(front01 * back + (1-front01)*front, heightPos,-widthPos);
		NxU32 flags = 0;
		if(i < 2)
		{
			flags |= NX_WF_ACCELERATED | NX_WF_STEERABLE_INPUT;
		} else {
			flags |= NX_WF_AFFECTED_BY_HANDBRAKE | NX_WF_ACCELERATED | NX_WF_STEERABLE_INPUT;
		}
		wheelDesc[wheel+0].wheelFlags |= flags;
		wheelDesc[wheel+1].wheelFlags |= flags;
	}
	
	truckDesc.steeringSteerPoint.set(frontOffset + frontPos, 0, 0);
	truckDesc.steeringTurnPoint .set(frontOffset - frontPos, 0, 0);

	truckDesc.cameraDistance = length;

	return &truckDesc;
}
NxVehicleDesc* createTwoAxisTrailer(const NxVec3& pos, NxReal length, bool oldStyle)
{
	static NxVehicleDesc trailerDesc;

	trailerDesc.setToDefault();
	static NxBoxShapeDesc boxShape;

	trailerDesc.setToDefault();

	boxShape.dimensions. set(length/2.f, 1.4f, 1.2f);
	boxShape.localPose.t.set(0,1.4f,0);

	trailerDesc.carShapes.pushBack(&boxShape);

	trailerDesc.position				= pos;
	trailerDesc.mass					= 10000;
	//trailerDesc.digitalSteeringDelta	= 0.1f;
	//trailerDesc.steeringMaxAngle		= 30.f;
	//trailerDesc.motorForce				= 200.f;
	trailerDesc.centerOfMass.set(0,1,0);
	//trailerDesc.maxVelocity			= 30.f;
	trailerDesc.cameraDistance		= 20.0f;

	static NxWheelDesc wheelDesc[4];
	for(NxU32 i = 0; i < 4; i++)
	{
		wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelRadius = 0.4f;
		wheelDesc[i].wheelWidth = i>1?0.2f:0.15f;
		wheelDesc[i].wheelSuspension = 0.2f;
		wheelDesc[i].springRestitution = 80000;
		wheelDesc[i].springDamping = 5000;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 1;
		if(oldStyle)
			{
			wheelDesc[i].frictionToFront = 0.1f;
			wheelDesc[i].frictionToSide = 0.99f;
			}
		else
			{
			wheelDesc[i].wheelFlags |= NX_WF_USE_WHEELSHAPE;
			}
		trailerDesc.carWheels.pushBack(&wheelDesc[i]);
	}
	NxReal frontOffset = 0.0f;
	NxReal frontPos = length / 2.5f;
	NxReal widthPos = 1.2f;
	NxReal heightPos = -0.1f;

	NxReal back = frontOffset - frontPos;
	NxReal front = frontOffset + frontPos;
	wheelDesc[0].position.set(front, heightPos, widthPos);
	wheelDesc[1].position.set(front, heightPos,-widthPos);
	wheelDesc[2].position.set(back, heightPos, widthPos);
	wheelDesc[3].position.set(back, heightPos,-widthPos);
	wheelDesc[0].wheelFlags |= NX_WF_STEERABLE_AUTO;
	wheelDesc[1].wheelFlags |= NX_WF_STEERABLE_AUTO;

	trailerDesc.steeringSteerPoint.set(frontOffset + frontPos, 0, 0);
	trailerDesc.steeringTurnPoint .set(frontOffset - frontPos, 0, 0);

	trailerDesc.cameraDistance = length;

	return &trailerDesc;
}

NxVehicle* createFullTruck(const NxVec3& pos, NxU32 nbGears, bool has4Axes, bool oldStyle)
{
	NxReal length = NxMath::rand(5.f,8.f);
	NxVehicleDesc* fullTruck = createFullTruckDesc(pos, length, nbGears, has4Axes, oldStyle);
	NxVehicle* vehicle = NxVehicle::createVehicle(gScene, fullTruck);
	NxQuat q;
	q.fromAngleAxis(180.0f, NxVec3(0.0f, 1.0f, 0.0f));
	vehicle->getActor()->setGlobalOrientationQuat(q);
//	NxAllVehicles::AddVehicle(vehicle);
	return vehicle;
}

NxVehicle* createFullTruckWithTrailer2(const NxVec3& pos, NxU32 nbGears, bool oldStyle)
{
	NxReal length = NxMath::rand(5.f, 8.f);
	NxReal intermediateLength = NxMath::rand(1.0f, 4.0f);
	NxVehicleDesc* fullTruck = createFullTruckDesc(pos, length, nbGears, false, oldStyle);

	NxVehicleDesc* trailer = createTwoAxisTrailer(pos - NxVec3(length+intermediateLength,0,0), length, oldStyle);

	if(fullTruck != NULL && trailer != NULL)
	{
		NxActorDesc trailerAxis;
		NxBoxShapeDesc boxShape;
		boxShape.dimensions.set(intermediateLength/2.0f, 0.05f,0.05f);
		trailerAxis.shapes.pushBack(&boxShape);
		NxBodyDesc bodyDesc;
		bodyDesc.mass = 1000;
		trailerAxis.body = &bodyDesc;
		trailerAxis.globalPose.t = pos - NxVec3((length+intermediateLength)/2.f,0,0);
		NxActor* aTrailerAxis = gScene->createActor(trailerAxis);

		fullTruck->children.pushBack(trailer);
		fullTruck->cameraDistance += length;

		NxVehicle* vehicle = NxVehicle::createVehicle(gScene, fullTruck);

		NxD6JointDesc joints[2];
		joints[0].actor[0] = vehicle->getActor();
		joints[0].actor[1] = aTrailerAxis;
		joints[1].actor[0] = aTrailerAxis;
		joints[1].actor[1] = vehicle->getChild(0)->getActor();
		joints[0].setGlobalAnchor(pos - NxVec3(length/2.0f, 0, 0));
		joints[1].setGlobalAnchor(pos - NxVec3(length/2.0f+intermediateLength, 0, 0));
		joints[0].setGlobalAxis(NxVec3(1,0,0));
		joints[1].setGlobalAxis(NxVec3(1,0,0));
		
		joints[0].xMotion		= joints[1].xMotion			= NX_D6JOINT_MOTION_LOCKED;
		joints[0].yMotion		= joints[1].yMotion			= NX_D6JOINT_MOTION_LOCKED;
		joints[0].zMotion		= joints[1].zMotion			= NX_D6JOINT_MOTION_LOCKED;
		joints[0].swing1Motion	= joints[1].swing1Motion	= NX_D6JOINT_MOTION_LIMITED;
		joints[0].swing2Motion	= joints[1].swing2Motion	= NX_D6JOINT_MOTION_LIMITED;
		joints[0].twistMotion	= joints[1].twistMotion		= NX_D6JOINT_MOTION_LIMITED;
		
		joints[0].swing1Limit.value		= joints[1].swing1Limit.value		= 0.3f;
		joints[0].swing2Limit.value		= joints[1].swing2Limit.value		= 0.8f;
		joints[0].twistLimit.low.value	= joints[1].twistLimit.low.value	=-0.1f;
		joints[0].twistLimit.high.value	= joints[1].twistLimit.high.value	= 0.1f;

		/*
		joints[0].projectionMode		= joints[1].projectionMode		= NX_JPM_POINT_MINDIST;
		joints[0].projectionDistance	= joints[1].projectionDistance	= 0.1f;
		joints[0].projectionAngle		= joints[1].projectionAngle		= 0.1f;
		*/
		
		gScene->createJoint(joints[0]);
		gScene->createJoint(joints[1]);

		return vehicle;
	} else {
		delete fullTruck;
		delete trailer;
		return NULL;
	}
}

/*
void renderHUD(OrthographicDrawing& orthoDraw)
{
	NxVehicle* curVehicle = NxAllVehicles::getActiveVehicle();
	if(curVehicle == NULL)
	{
		return;
	}
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	NxReal maxV = curVehicle->getMaxVelocity();
	NxReal velocity = curVehicle->getDriveVelocity();
	NxReal velocity01 = velocity / maxV;

	NxReal radius = 50;
	glColor4f(1,1,1,1);
	NxReal kmh = velocity * 3.6f;
	NxReal maxKmh = maxV * 3.6f;
	NxReal spacing = 10.f;
	float centerX = width-radius-spacing;
	float centerY = height-radius-spacing;
	float centerY2 = height-30.f-3*radius;

	char buf[100];
	sprintf(buf, "km/h: %3d", (int)kmh);
	orthoDraw.drawText(centerX-35, centerY+radius-10.f, buf);

	// RPM
	if(curVehicle->getMotor())
	{
		char buf[100]; sprintf(buf, "Rpm: %4d", (int)curVehicle->getMotor()->getRpm());
		orthoDraw.drawText(centerX-35, centerY2+radius-10.f, buf);
	}

	if(curVehicle->getGears())
	{
		char buf[100]; sprintf(buf, "Gear: %d", curVehicle->getGears()->getGear());
		orthoDraw.drawText(centerX-20.f, centerY2-radius-10.f, buf);
	}

	glScalef(1, -1, 1);
	glTranslatef(0, -orthoDraw.mHeight, 0);

	// Velocity
	float angle = (1-velocity01) * NxMath::degToRad(210.f) + velocity01*NxMath::degToRad(-30.f);
	glColor4f(GL_COLOR_WHITE);
	orthoDraw.drawCircle(centerX , centerY, radius, -30, 210, 10, maxKmh/20.f, maxKmh/10.f);
	glColor4f(GL_COLOR_RED);
	glLineWidth(2.0f);
	orthoDraw.drawLine(centerX, centerY, centerX + NxMath::cos(angle)*radius*0.9f, centerY - NxMath::sin(angle)*radius*0.9f);
	glLineWidth(1.0f);
	

	// RPM
	if(curVehicle->getMotor())
	{
		float rpm = curVehicle->getMotor()->getRpm();
		float maxRpm = curVehicle->getMotor()->getMaxRpm();
		float rpm01 = rpm / maxRpm;
		float angle = (1-rpm01) * NxMath::degToRad(210.f) + rpm01*NxMath::degToRad(-30.f);
		glColor4f(GL_COLOR_WHITE);
		orthoDraw.drawCircle(centerX, centerY2, radius, -30, 210, 10, 5, 10);
		glColor4f(GL_COLOR_RED);
		glLineWidth(2.0f);
		orthoDraw.drawLine(centerX, centerY2, centerX + NxMath::cos(angle)*radius*0.9f, centerY2 - NxMath::sin(angle)*radius*0.9f);
		glLineWidth(1.0f);
		//char buf[100]; sprintf(buf, "Rpm: %4d", (int)rpm);
		//orthoDraw.drawText(centerX-35, centerY+radius-10.f, buf);
	}

	NxReal wheelRpmRadius = 30.f;
	float maxWheelRpm = 1000.f;
	for (NxU32 i = 0; i < curVehicle->getNbWheels(); i++)
	{
		const NxWheel* curWheel = curVehicle->getWheel(i);
		centerX = spacing + wheelRpmRadius;
		centerY = spacing + wheelRpmRadius + i*(2*wheelRpmRadius + spacing);
		if(curWheel->getWheelFlag(NX_WF_ACCELERATED))
		{
			glColor4f(GL_COLOR_WHITE);
		} else {
			glColor4f(GL_COLOR_LIGHT_GREY);
		}
		orthoDraw.drawCircle(centerX, centerY, wheelRpmRadius, -30, 210, 10, 10, 20);

		float rpm = curWheel->getRpm();
		float rpm01 = rpm / maxWheelRpm;
		float angle = (1-rpm01) * NxMath::degToRad(210.f) + rpm01*NxMath::degToRad(-30.f);
		glColor4f(GL_COLOR_RED);
		glLineWidth(2.0f);
		orthoDraw.drawLine(centerX, centerY, centerX + NxMath::cos(angle)*wheelRpmRadius*0.9f, centerY - NxMath::sin(angle)*wheelRpmRadius*0.9f);
		glLineWidth(1.0f);
	}

}
*/