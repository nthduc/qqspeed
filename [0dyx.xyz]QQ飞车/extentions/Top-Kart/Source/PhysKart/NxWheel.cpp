#include "NxWheel.h"

#include <cstdio>

#include <NxCooking.h>

#include "NxVehicle.h"
#include "Stream.h"

#define GL_COLOR_TIRE 0.3f,0.3f,0.3f,1.0f
#define GL_COLOR_TIRE_INNER 0.1f,0.1f,0.1f,1.0f

NxWheel* NxWheel::createWheel(NxActor* actor, NxWheelDesc* wheelDesc) 
{
	if(wheelDesc->wheelFlags & NX_WF_USE_WHEELSHAPE)
	{
		return new NxWheel2(actor, wheelDesc);
	}
	
	NxWheel1* wheel = new NxWheel1(&actor->getScene());

	if(wheelDesc->wheelApproximation > 0) 
	{
		NxArray<NxVec3> points;
		NxVec3 center = wheelDesc->position;

		NxVec3 frontAxis(1,0,0);// = wheelDesc->downAxis.cross(wheelDesc->wheelAxis);
		NxVec3 downAxis(0,-1,0);//  = wheelDesc->downAxis;
		NxVec3 wheelAxis(0,0,-1);// = wheelDesc->wheelAxis;
		//frontAxis.normalize();
		frontAxis *= wheelDesc->wheelRadius;
		//downAxis.normalize();
		downAxis *= wheelDesc->wheelRadius;
		//wheelAxis.normalize();
		wheelAxis *= wheelDesc->wheelWidth;
		NxReal step;
		if(wheelDesc->wheelFlags & NX_WF_BUILD_LOWER_HALF) 
		{
			if((wheelDesc->wheelApproximation & 0x1) == 0)
				wheelDesc->wheelApproximation++;
			step = (NxReal)(NxTwoPi) / (NxReal)(wheelDesc->wheelApproximation*2);
		}
		else 
		{
			step = (NxReal)(NxTwoPi) / (NxReal)(wheelDesc->wheelApproximation);
		}
		for(NxU32 i = 0; i < wheelDesc->wheelApproximation; i++) 
		{
			NxReal iReal;
			if(wheelDesc->wheelFlags & NX_WF_BUILD_LOWER_HALF) 
			{
				iReal = (i > (wheelDesc->wheelApproximation >> 1))?(NxReal)(i+wheelDesc->wheelApproximation):(NxReal)i;
			} 
			else 
			{
				iReal = (NxReal)i;
			}
			NxReal Sin, Cos;
			NxMath::sinCos(step * iReal, Sin, Cos);
			NxVec3 insPoint = (downAxis * -Cos) + (frontAxis * Sin);
			points.pushBack(insPoint + wheelAxis);
			points.pushBack(insPoint - wheelAxis);
		}

		NxConvexMeshDesc convexDesc;
		convexDesc.numVertices			= points.size();
		convexDesc.pointStrideBytes		= sizeof(NxVec3);
		convexDesc.points				= &points[0].x;
		convexDesc.flags				= NX_CF_COMPUTE_CONVEX;

		// Cooking from memory
		MemoryWriteBuffer buf;
		if(NxCookConvexMesh(convexDesc, buf)) 
			{
			NxConvexShapeDesc convexShapeDesc;
			convexShapeDesc.meshData = actor->getScene().getPhysicsSDK().createConvexMesh(MemoryReadBuffer(buf.data));
			convexShapeDesc.localPose.t = center;
			convexShapeDesc.localPose.M.setColumn(0, NxVec3( 1, 0, 0));
			convexShapeDesc.localPose.M.setColumn(1, NxVec3( 0,-1, 0));
			convexShapeDesc.localPose.M.setColumn(2, NxVec3( 0, 0, -1));
			if(convexShapeDesc.meshData != NULL) 
			{
				NxU32 shapeNumber = actor->getNbShapes();
				wheel->wheelConvex = actor->createShape(convexShapeDesc)->isConvexMesh();
				wheel->wheelConvex->userData = wheel;
			}
		}

		else 
		{
			delete wheel;
			return NULL;
		}
	}
	else 
	{
		wheel->wheelConvex = NULL;
	}

	NxVec3 frontAxis(1,0,0);
	NxVec3 downAxis(0,-1,0);
	NxVec3 wheelAxis(0,0,-1);
	//NxVec3 downAxis  = wheelDesc->downAxis; downAxis.normalize();
	//NxVec3 wheelAxis  = wheelDesc->wheelAxis; wheelAxis.normalize();
	//NxVec3 frontAxis  = downAxis.cross(wheelAxis); frontAxis.normalize();

	// This assures a height of 1 for every capsule
	NxReal heightModifier = (wheelDesc->wheelSuspension + wheelDesc->wheelRadius) / wheelDesc->wheelSuspension;
	//if (wheelDesc->wheelSuspension < 1)
	//	heightModifier = 1.f / wheelDesc->wheelSuspension;

	NxSpringDesc wheelSpring;
	wheelSpring.spring					= wheelDesc->springRestitution*heightModifier;
	wheelSpring.damper					= wheelDesc->springDamping*heightModifier;
	wheelSpring.targetValue				= wheelDesc->springBias*heightModifier;

	NxMaterialDesc materialDesc;
	materialDesc.restitution			= 0.0f;
	materialDesc.dynamicFriction		= wheelDesc->frictionToSide;
	materialDesc.staticFriction			= 2.0f;
	materialDesc.staticFrictionV		= wheelDesc->frictionToFront*4;
	materialDesc.dynamicFrictionV		= wheelDesc->frictionToFront;
	materialDesc.dirOfAnisotropy		= frontAxis;
	materialDesc.frictionCombineMode	= NX_CM_MULTIPLY;
	materialDesc.flags					|=(NX_MF_SPRING_CONTACT | NX_MF_ANISOTROPIC);
	materialDesc.spring					= &wheelSpring;

	wheel->_frictionToFront = wheelDesc->frictionToFront;
	wheel->_frictionToSide = wheelDesc->frictionToSide;

	wheel->material = actor->getScene().createMaterial(materialDesc);

	NxCapsuleShapeDesc capsuleShape;
	capsuleShape.radius = 0.1f;
	capsuleShape.height = wheelDesc->wheelSuspension + wheelDesc->wheelRadius;
	capsuleShape.flags = NX_SWEPT_SHAPE;
	//capsuleShape.localPose.M.setColumn(0, NxVec3( 1, 0, 0));
	//capsuleShape.localPose.M.setColumn(1, NxVec3( 0,-1, 0));
	//capsuleShape.localPose.M.setColumn(2, NxVec3( 0, 0,-1));	//rotate 180 degrees around x axis to cast downward!
	capsuleShape.localPose.M.setColumn(0, frontAxis);
	capsuleShape.localPose.M.setColumn(1, downAxis);
	capsuleShape.localPose.M.setColumn(2, wheelAxis);
	if(wheelDesc->wheelSuspension >= 1) 
	{
		capsuleShape.localPose.t = wheelDesc->position + downAxis*(wheelDesc->wheelRadius);
	}
	else 
	{
		capsuleShape.localPose.t = wheelDesc->position + downAxis*((wheelDesc->wheelRadius + wheelDesc->wheelSuspension)*0.5f);
	}
	capsuleShape.materialIndex = wheel->material->getMaterialIndex();
	capsuleShape.userData = wheelDesc->userData;

	NxU32 shapeNumber = actor->getNbShapes();
	wheel->wheelCapsule = actor->createShape(capsuleShape)->isCapsule();

	if(wheel->wheelCapsule == NULL) 
	{
		delete wheel;
		return NULL;
	}
	wheel->wheelCapsule->userData = wheel;
	wheel->userData = wheelDesc->userData;
	wheel->wheelCapsule->userData = wheel;
	wheel->wheelFlags = (wheelDesc->wheelFlags & NX_WF_ALL_WHEEL_FLAGS);
	wheel->_turnAngle = 0;
	wheel->_turnVelocity = 0;
	wheel->_radius = wheelDesc->wheelRadius;
	wheel->_perimeter = wheel->_radius * NxTwoPi;
	wheel->_maxSuspension = wheelDesc->wheelSuspension;
	wheel->_wheelWidth = wheelDesc->wheelWidth;
	wheel->_maxPosition = wheelDesc->position;
	return wheel;
}

NxWheel1::NxWheel1(NxScene * s)  : scene(s)
{ 
}

NxWheel1::~NxWheel1() 
{
	scene->releaseMaterial(*material);
}


void NxWheel1::tick(bool handBrake, NxReal motorTorque, NxReal brakeTorque, NxReal dt)
{
	if(getWheelFlag(NX_WF_ACCELERATED)) 
	{
		if (handBrake && getWheelFlag(NX_WF_AFFECTED_BY_HANDBRAKE)) 
		{
			// Handbrake, blocking!
		}
		else if (hasGroundContact()) 
			{
			// Touching, force applies
			NxVec3 steeringDirection;
			getSteeringDirection(steeringDirection);
			steeringDirection.normalize();
			NxReal localTorque = motorTorque;
			NxReal wheelForce = localTorque / _radius;
			steeringDirection *= wheelForce;
			wheelCapsule->getActor().addForceAtPos(steeringDirection, contactInfo.contactPosition);
			if(contactInfo.otherActor->isDynamic())
				contactInfo.otherActor->addForceAtPos(-steeringDirection, contactInfo.contactPosition);
			} 
	}

	NxReal OneMinusBreakPedal = 1-brakeTorque;
	if(handBrake && getWheelFlag(NX_WF_AFFECTED_BY_HANDBRAKE)) 
	{
		material->setDynamicFrictionV(1);
		material->setStaticFrictionV(4);
		material->setDynamicFriction(0.4f);
		material->setStaticFriction(1.0f);
	} 
	else 
	{
		NxReal newv = OneMinusBreakPedal * _frictionToFront + brakeTorque;
		NxReal newv4= OneMinusBreakPedal * _frictionToFront + brakeTorque*4;
		material->setDynamicFrictionV(newv);
		material->setDynamicFriction(_frictionToSide);
		material->setStaticFrictionV(newv*4);
		material->setStaticFriction(2);
	}

	if(!hasGroundContact())
		updateContactPosition();
	updateAngularVelocity(dt, handBrake);


	contactInfo.reset();
}

void NxWheel1::getSteeringDirection(NxVec3& dir) 
{
	if(wheelFlags & (NX_WF_STEERABLE_INPUT | NX_WF_STEERABLE_AUTO)) 
	{
		wheelCapsule->getGlobalOrientation().getColumn(0, dir);
	} 
	else 
	{
		wheelCapsule->getActor().getGlobalOrientation().getColumn(0, dir);
	}
}

void NxWheel1::updateAngularVelocity(NxReal lastTimeStepSize, bool handbrake) 
{
	if((wheelFlags & NX_WF_AFFECTED_BY_HANDBRAKE) && handbrake) 
	{
		_turnVelocity = 0;
	} 
	else if (contactInfo.isTouching()) 
		{
		NxReal wheelPerimeter = NxTwoPi * _radius;
		NxReal newTurnVelocity = contactInfo.relativeVelocity / wheelPerimeter;
//		if (NxMath::abs(_turnVelocity - newTurnVelocity) > 1.f)
//			printf("pfff\n");
		_turnVelocity = newTurnVelocity;
		_turnAngle += _turnVelocity * lastTimeStepSize * NxTwoPi;
		} 
	else 
	{
		_turnVelocity *= 0.99f;
		_turnAngle += _turnVelocity;
	}
	while(_turnAngle >= NxTwoPi)
		_turnAngle -= NxTwoPi;
	while (_turnAngle < 0)
		_turnAngle += NxTwoPi;

	//printf("Wheel turn angle: %2.3f (%s) (%x)\n", _turnAngle, contactInfo.isTouching()?"true":"false", (void*)&wheelCapsule->getActor());
	}

void NxWheel1::drawWheel(NxReal approx, bool debug) const
{
	/*
	if(debug) 
	{
		glPointSize(5.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		NxVec3 vec1(getWheelPos());
		glColor4f(1,0,0,1);
		glVertexPointer(3, GL_FLOAT, 0, &vec1);
		glDrawArrays(GL_POINTS, 0, 1);

		NxVec3 vec2(contactInfo.contactPositionLocal);
		glColor4f(1,0,1,1);
		glVertexPointer(3, GL_FLOAT, 0, &vec2);
		glDrawArrays(GL_POINTS, 0, 1);

		NxVec3 wheelpos = contactInfo.contactPositionLocal + NxVec3(0.f, _radius, 0.f);
		glColor4f(1,1,1,1);
		glVertexPointer(3, GL_FLOAT, 0, &wheelpos);
		glDrawArrays(GL_POINTS, 0, 1);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

#if !defined(__CELLOS_LV2__) && !defined(_XBOX)

	static GLUquadricObj *quadric = NULL;
	if(quadric == NULL) 
	{
		quadric = gluNewQuadric();
	}
	
	if(debug) 
	{
		gluQuadricDrawStyle(quadric, GLU_LINE);
	} 
	else 
	{
		gluQuadricDrawStyle(quadric, GLU_FILL);
	}

	NxVec3 wheelpos = (contactInfo.contactPositionLocal + NxVec3(0.f, _radius, 0.f));
	NxReal angle = NxMath::radToDeg(_turnAngle);
	
	glPushMatrix();
	
	if(debug) 
	{
		NxF32 red = getWheelFlag(NX_WF_ACCELERATED)?1.f:0.f;
		NxF32 green = contactInfo.isTouching()?1:0.5f;
		NxF32 blue = 0.5f;
		glColor4f(red, green, blue, 1.0f);
	} else {
		glColor4f(GL_COLOR_TIRE);
	}
	NxU32 slices = 20;
	GLfloat mat[16]; mat[3] = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0; mat[15] = 1;
	wheelCapsule->getLocalOrientation().getColumnMajorStride4(mat);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(NxVec3), &_maxPosition.x);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableClientState(GL_VERTEX_ARRAY);

	glTranslatef(wheelpos.x, wheelpos.y, wheelpos.z);
	glMultMatrixf(mat);
	glRotatef(angle, 0,0,1);
	glTranslatef(0,0, -_wheelWidth);
	gluCylinder(quadric, _radius, _radius, _wheelWidth*2, slices, 1);

	if(!debug) 
	{
		glTranslatef(0,0, 2*_wheelWidth);
		gluDisk(quadric, _radius/2.f, _radius, slices, 1);
		glColor4f(GL_COLOR_TIRE_INNER);
		gluDisk(quadric, 0, _radius/2.f, slices, 1);
		glTranslatef(0,0, -2*_wheelWidth);
		glRotatef(180,1,0,0);
		glColor4f(GL_COLOR_TIRE);
		gluDisk(quadric, _radius/2.f, _radius, slices, 1);
		glColor4f(GL_COLOR_TIRE_INNER);
		gluDisk(quadric, 0, _radius/2.f, slices, 1);
	}

	glPopMatrix();
#endif
	*/
}

void NxWheel1::updateContactPosition() 
{
	contactInfo.contactPositionLocal = _maxPosition - NxVec3(0, _maxSuspension+_radius, 0);
}

void NxWheel1::setAngle(NxReal angle) 
{
	_angle = angle;

	NxReal Cos, Sin;
	NxMath::sinCos(_angle, Sin, Cos);
	NxMat33 wheelOrientation = wheelCapsule->getLocalOrientation();
	wheelOrientation.setColumn(0,  NxVec3( Cos, 0, Sin ));
	wheelOrientation.setColumn(2,  NxVec3( Sin, 0,-Cos ));
	setWheelOrientation(wheelOrientation);
}

/*---------------------------------------------------------------------------------------*\
|
|Wheel 2 - uses NxWheelShape
|
\*---------------------------------------------------------------------------------------*/
NxWheel2::NxWheel2(NxActor* a, NxWheelDesc* wheelDesc) : actor(a)
{

	NxScene * scene = &actor->getScene();
	//create a shared car wheel material to be used by all wheels
	static NxMaterial * wsm = 0;
	if(!wsm)
	{
		NxMaterialDesc m;
		m.flags |= NX_MF_DISABLE_FRICTION;
		wsm = scene->createMaterial(m);
	}

	NxWheelShapeDesc wheelShapeDesc;

	wheelShapeDesc.localPose.t = wheelDesc->position;
	NxQuat q;
	q.fromAngleAxis(90.0f, NxVec3(0,1,0));
	wheelShapeDesc.localPose.M.fromQuat(q);
	wheelShapeDesc.materialIndex = wsm->getMaterialIndex();
	wheelFlags = wheelDesc->wheelFlags;



	NxReal heightModifier = (wheelDesc->wheelSuspension + wheelDesc->wheelRadius) / wheelDesc->wheelSuspension;

	wheelShapeDesc.suspension.spring = wheelDesc->springRestitution*heightModifier;
	wheelShapeDesc.suspension.damper = 0;//wheelDesc->springDamping*heightModifier;
	wheelShapeDesc.suspension.targetValue = wheelDesc->springBias*heightModifier;

	wheelShapeDesc.radius = wheelDesc->wheelRadius;
	wheelShapeDesc.suspensionTravel = wheelDesc->wheelSuspension; 
	wheelShapeDesc.inverseWheelMass = 0.1f;	//not given!? TODO

	wheelShapeDesc.lateralTireForceFunction.stiffnessFactor *= wheelDesc->frictionToSide;	
	wheelShapeDesc.longitudalTireForceFunction.stiffnessFactor *= wheelDesc->frictionToFront;	

	wheelShape = static_cast<NxWheelShape *>(actor->createShape(wheelShapeDesc));
}

NxWheel2::~NxWheel2()
{
}

void NxWheel2::tick(bool handBrake, NxReal motorTorque, NxReal brakeTorque, NxReal dt)
{
	//motorTorque *= 0.1f;
	brakeTorque *= 500.0f;
	if(handBrake && getWheelFlag(NX_WF_AFFECTED_BY_HANDBRAKE))
		brakeTorque = 1000.0f;

	if(getWheelFlag(NX_WF_ACCELERATED)) 
		wheelShape->setMotorTorque(motorTorque);

	wheelShape->setBrakeTorque(brakeTorque);
}

NxActor* NxWheel2::getTouchedActor() const
{
	NxWheelContactData wcd;
	NxShape * s = wheelShape->getContact(wcd);	
	return s ? &s->getActor() : 0;
}

NxVec3 NxWheel2::getWheelPos() const
{
	return wheelShape->getLocalPosition();
}

void NxWheel2::setAngle(NxReal angle)
{
	wheelShape->setSteerAngle(-angle);
}

void NxWheel2::drawWheel(NxReal approx, bool debug) const
{
	//nothing, taken care of by built in visualization.
	NxWheelContactData wcd;
	NxShape* s = wheelShape->getContact(wcd);	
	if(!s) return;

	//printf(" f = %f %f %f\n",wcd.contactForce, wcd.longitudalSlip, wcd.longitudalImpulse);
/*
	glPushMatrix();
	glLoadIdentity();

	NxVec3 pVecBuffer[4];
	pVecBuffer[0] = wcd.contactPoint;
	pVecBuffer[1] = wcd.contactPoint + wcd.longitudalDirection;
	pVecBuffer[2] = wcd.contactPoint;
	pVecBuffer[3] = wcd.contactPoint + wcd.lateralDirection;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pVecBuffer);
	glDrawArrays(GL_LINES, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
*/
}

NxReal NxWheel2::getRpm() const
{
	return NxMath::abs(wheelShape->getAxleSpeed())/NxTwoPi * 60.0f;
}
