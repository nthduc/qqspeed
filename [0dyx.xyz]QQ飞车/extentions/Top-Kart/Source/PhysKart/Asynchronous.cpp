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

#include "..\Top-KartPCH.h"
#include <NiMain.h>

#include "Asynchronous.h"
unsigned char g_keys[256];
//---------------------------------------------------------------------------
NiApplication* NiApplication::Create()
{
	return NiNew Asynchronous;
}
//---------------------------------------------------------------------------
Asynchronous::Asynchronous() :
NiApplication("Gamebryo PhysX Tutorial 4", 640, 480)
{
	// Tell Gamebryo where to find its data files.
#if defined(WIN32) && !defined(_XBOX)
	SetMediaPath("../../../Data/Win32/");   
#elif defined(_XBOX)
	SetMediaPath("D:/DATA/Win32/");
#endif // defined(WIN32) && !defined(_XBOX)

	m_spTrnNode = 0;
	m_spRotNode = 0;

	// Initialize smart pointers to zero. In the case of early termination
	// this avoid errors.
	m_spMasterScene = 0;
	m_spBallScene = 0;
	m_spBodyDest = 0;
	m_spCarRoot = 0;
}
//---------------------------------------------------------------------------
Asynchronous::~Asynchronous()
{
}
//---------------------------------------------------------------------------
bool Asynchronous::Initialize()
{
	// Save a pointer to the NiPhysXManager object. This is Gamembryo's
	// manager for all PhysX global functionality. The file "NiPhysX.h"
	// must be included for this object to exist.
	m_pkPhysManager = NiPhysXManager::GetPhysXManager();

	// Try to initialize the PhysX SDK. By default, we are setting up
	// Gamebryo's memory manager and debug output interfaces for use
	// with PhysX.
	if (!m_pkPhysManager->Initialize())
		return false;

	// The manager contains a public pointer to the PhysX SDK object,
	// m_pkPhysXSDK. Here we use it to set some global SDK parameters.
	// See the PhysX documentation for an explanation of these settings.
	m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_SKIN_WIDTH, 0.01f);
	m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_BOUNCE_TRESHOLD, -0.5f);
	m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZATION_SCALE, 0.0f);
	m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZE_BODY_AXES, 0.2f);
	m_pkPhysManager->m_pkPhysXSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES,
		1.0f);

	if (!NiApplication::Initialize())
		return false;

	// Set up camera control
	SetTurretControls();

	// Update the scene graph before rendering begins.
	m_spScene->UpdateProperties();
	m_spScene->UpdateEffects();
	m_spScene->Update(0.0f);

	// Get simulation started. We give a small timestep to avoid zero length
	// steps, which cause problems with PhysX hardware in v2.3.2
	m_spMasterScene->UpdateSources(0.001f);
	m_spMasterScene->Simulate(0.001f);
	fLastSimTime = 0.001f;

	return true;
}
//---------------------------------------------------------------------------
void Asynchronous::Terminate()
{
	m_spTrnNode = 0;
	m_spRotNode = 0;

	// The PhysX scenes should be deleted before the SDK is shut down.
	m_spMasterScene = 0;
	m_spBallScene = 0;

	// The PhysX manager must be shut down after all PhysX content has been
	// deleted and before the application terminates.
	m_pkPhysManager->Shutdown();

	NiApplication::Terminate();
}
//---------------------------------------------------------------------------
bool Asynchronous::CreateScene()
{
	if(!NiApplication::CreateScene())
		return false;

	// Set the background color
	NiColor kColor(0.5f, 0.6f, 1.0f);
	m_spRenderer->SetBackgroundColor(kColor);

	// Load the box scene, but this time there is an animated paddle also.
	NiStream kStream;
	if (!kStream.Load(ConvertMediaFilename("paodao.nif")))
	{
		assert(0 && "Couldn't load nif file\n");
		NiMessageBox::DisplayMessage(
			"Could not load BoxAndPaddle.nif. Aborting\n",
			"Missing nif file.");

		return false;
	}

	// We know that this NIF file has the scene graph at location 0.
	assert(NiIsKindOf(NiNode, kStream.GetObjectAt(0)));
	m_spScene = (NiNode*)kStream.GetObjectAt(0);

	// Look for a camera and the PhysX content.
	int i = kStream.GetObjectCount();
	for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
	{
		if (NiIsKindOf(NiCamera, kStream.GetObjectAt(ui)))
		{
			m_spCamera = (NiCamera*)kStream.GetObjectAt(ui);
		}
		else if (NiIsKindOf(NiPhysXScene, kStream.GetObjectAt(ui)))
		{
			// We have found the PhysX content in the NIF.
			m_spMasterScene = (NiPhysXScene*)kStream.GetObjectAt(ui);
		}
	}

	//m_spCamera = NiNew NiCamera();
	//m_spCamera.
	// After loading the PhysX content, we must instantiate it. That
	// means creating the PhysX actors and connecting them to Gamebryo.
	// It is done with a single call:
	m_spMasterScene->CreateSceneFromSnapshot();

	// This scene now contains the paddle, which is a source of pose
	// information for PhysX. We need to enable updates of sources in the
	// scene. Individual sources also must be activated, but they are by
	// default.
	m_spMasterScene->SetUpdateSrc(true);

	// Repeat the process with the ball.
	kStream.RemoveAllObjects();
	if (!kStream.Load(ConvertMediaFilename("car.nif")))
	{
		assert(0 && "Couldn't load nif file\n");
		NiMessageBox::DisplayMessage("Could not load Ball.nif. Aborting\n",
			"Missing nif file.");

		return false;
	}

	// We know that this NIF file has the ball at location 0. Attach the
	// ball to the scene graph.
	assert(NiIsKindOf(NiAVObject, kStream.GetObjectAt(0)));
	m_spScene->AttachChild((NiAVObject*)kStream.GetObjectAt(0));

	// Look for the PhysX content.
	for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
	{
		if (NiIsKindOf(NiCamera, kStream.GetObjectAt(ui)))
		{
 			//m_spCamera = (NiCamera*)kStream.GetObjectAt(ui);
		}
		else if (NiIsKindOf(NiPhysXScene, kStream.GetObjectAt(ui)))
		{
			// We have found the PhysX content in the NIF.
			m_spBallScene = (NiPhysXScene*)kStream.GetObjectAt(ui);
		}
	}


	// Now we want the ball to be part of the same PhysX scene as the box.
	// This is done by slaving the ball to the box scene, before the
	// PhysX content is instantiated. We specify that we want the identity
	// as the relative transformation between the two scenes.
	m_spBallScene->SetSlaved(m_spMasterScene, NiPhysXTypes::NXMAT34_ID);

	// Now instantiate the ball. It gets created in the same scene as
	// the box.
	m_spBallScene->CreateSceneFromSnapshot();

	// The ball is a destination object - the Gamebryo scene graph ball
	// receives pose information from the PhysX actor. We need to enable
	// the updating of destinations to start the transfer of information.
	// Each objct needs to be enabled and the whole scene. By default the
	// object is enabled, but the scene is not. Even though it is slaved,
	// the NiPhysXScene that contains the objects remains responsible for
	// its destinations and sources.
	m_spBallScene->SetUpdateDest(true);

	//Kart
	NxReal xpos = 0;
	NxReal ypos = 2.0f;
	NxReal zdist = 5.0f;
	NxReal zpos = 0.0f;

	m_spCarRoot = (NiAVObject*)kStream.GetObjectAt(0);

	m_kart.CreateSimpleCart((NiAVObject*)kStream.GetObjectAt(0),
		NxVec3(xpos,ypos,zpos + zdist * 3), false, true, false,
		m_spMasterScene->GetPhysXScene());//cart rw

	//m_kart.CreateSimpleCart((NiAVObject*)kStream.GetObjectAt(0),
	//	NxVec3(xpos,ypos,zpos + zdist * 3), false, true, false,
	//	m_spMasterScene->GetPhysXScene());//cart rw

	//createCart((NiAVObject*)kStream.GetObjectAt(0),NxVec3(xpos,ypos,zpos + zdist * 3), false, true, false,m_spMasterScene->GetPhysXScene());//cart rw


	//NiAVObject* pCarRoot = (NiAVObject*)kStream.GetObjectAt(0);
	//NiAVObject* pChassis = pCarRoot->GetObjectByName("chassis");
	//if (!pChassis)
	//	return false;

	//NxActor* pActorBody = NxAllVehicles::getActiveVehicle()->getActor();
	//if (!pActorBody)
	//	return false;

	//m_spBodyDest = NiNew NiPhysXTransformDest(pChassis, pActorBody,NULL);
	//m_spBallScene->AddDestination(m_spBodyDest);

	NiAVObject* pObStart = m_spScene->GetObjectByName("KartStart");
	if (pObStart)
	{
		NiPoint3 ptStart = pObStart->GetTranslate();
		NxVec3 pos(ptStart.x-50, ptStart.y, ptStart.z+1.20f);
		NxAllVehicles::getActiveVehicle()->getActor()->setGlobalPosition(pos);
		NxMat33 mx33;
		mx33.rotX(90.0f/180.0f*3.14159);
		//NxAllVehicles::getActiveVehicle()->getActor()
		//NxAllVehicles::getActiveVehicle()->getActor()->setGlobalOrientation(mx33);
		//NxAllVehicles::getActiveVehicle()->getActor()->setLinearVelocity(NxVec3(0,0,0));
		//NxAllVehicles::getActiveVehicle()->getActor()->setAngularVelocity(NxVec3(0,0,0));


		NxMat33 mx333;
		mx333.rotZ(90.0f/180.0f*3.14159);
		NxWheel2* pW2 = (NxWheel2*) NxAllVehicles::getActiveVehicle()->getWheel(0);
		//pW2->wheelShape->setGlobalOrientation(mx333);
		//NxAllVehicles::getActiveVehicle()->standUp();
	}



	
m_spCamera->Update(0);


	return true;
}
//---------------------------------------------------------------------------
void Asynchronous::ProcessInput()
{
	NiApplication::ProcessInput();
	NiInputKeyboard* pkKeyboard = GetInputSystem()->GetKeyboard();
	if (pkKeyboard)
	{
		if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_R))
		{
			// Reset the ball's position
			ResetBall();
		}
		if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_V))
		{
			// Toggle debug rendering
			ToggleDebug();
		}

		if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_I))
		{
			g_keys[NiInputKeyboard::KEY_I] = 1;
		}
		if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_K))
		{
			g_keys[NiInputKeyboard::KEY_K] = 1;
		}
		if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_J))
		{
			g_keys[NiInputKeyboard::KEY_J] = 1;
		}
		if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_L))
		{
			g_keys[NiInputKeyboard::KEY_L] = 1;
		}
		if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_SPACE))
		{
			g_keys[NiInputKeyboard::KEY_L] = 1;
		}

		if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_I))
		{
			g_keys[NiInputKeyboard::KEY_I] = 0;
		}
		if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_K))
		{
			g_keys[NiInputKeyboard::KEY_K] = 0;
		}
		if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_J))
		{
			g_keys[NiInputKeyboard::KEY_J] = 0;
		}
		if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_L))
		{
			g_keys[NiInputKeyboard::KEY_L] = 0;
		}
		if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_SPACE))
		{
			g_keys[NiInputKeyboard::KEY_L] = 0;
		}

		if (pkKeyboard->KeyWasReleased(NiInputKeyboard::KEY_F))
		{
			NxAllVehicles::getActiveVehicle()->standUp();
		}




		bool left = (g_keys[NiInputKeyboard::KEY_J] == 1);
		bool right = (g_keys[NiInputKeyboard::KEY_L] == 1);
		bool forward = (g_keys[NiInputKeyboard::KEY_I] == 1);
		bool backward = (g_keys[NiInputKeyboard::KEY_K] == 1);
		NxReal steering = 0;
		if (left && !right) steering = -1;
		else if (right && !left) steering = 1;
		NxReal acceleration = 0;
		if (forward && !backward) acceleration = 1;
		else if (backward && !forward) acceleration = -1;
		bool handbrake = g_keys[NiInputKeyboard::KEY_SPACE];
		if(NxAllVehicles::getActiveVehicle())
			//NxAllVehicles::getActiveVehicle()->control(1, false, 1, false, false);
			NxAllVehicles::getActiveVehicle()->control(steering, false, acceleration, false, handbrake);
		NxAllVehicles::updateAllVehicles(1.0f/60.f);

	}

	NiInputGamePad* pkGamePad;
	for (unsigned int uiPort = 0; uiPort < NiInputSystem::MAX_GAMEPADS; 
		uiPort++)
	{
		pkGamePad = m_spInputSystem->GetGamePad(uiPort);
		if (pkGamePad)
		{
			if (pkGamePad->ButtonWasPressed(NiInputGamePad::NIGP_START))
			{
				// Reset the ball's position
				ResetBall();
			}
			if (pkGamePad->ButtonWasPressed(NiInputGamePad::NIGP_A))
			{
				// Reset the ball's position
				ToggleDebug();
			}
		}
	}

}
//---------------------------------------------------------------------------
void Asynchronous::UpdateFrame()
{
	// Simulation has been going on between frames. Now we get the
	// results. We still block because we need the answers. We have to do
	// this before we do anything that might touch the PhysX SDK, because
	// it s unsafe to try any SDK operations while simulating.
	m_spMasterScene->FetchResults(fLastSimTime, true);
	m_spBallScene->UpdateDestinations(fLastSimTime);

	NiApplication::UpdateFrame(); // Calls process input

//	5.
	// Update the camera. This uses global time.
	if (m_kTurret.Read())
		m_spTrnNode->Update(m_fAccumTime);

	// We update the scene graph.
	m_spScene->Update(m_fAccumTime);

	// Now we start the next step, giving a time that will actually be
	// in te past by the time we get the results.
	m_spMasterScene->UpdateSources(m_fAccumTime);
	m_spMasterScene->Simulate(m_fAccumTime);
	fLastSimTime = m_fAccumTime;


	m_kart.Update(0);
}
//---------------------------------------------------------------------------
void Asynchronous::ResetBall()
{
	// To restore the scene to its initial conditions, we use the data
	// stored in the snapshot that was originally loaded from the NIF file.
	// By default, that is snapshot 0.
	m_spBallScene->RestoreSnapshotState(0);
}
//---------------------------------------------------------------------------
void Asynchronous::ToggleDebug()
{
	if (m_spMasterScene->GetDebugRender())
	{
		// Turn off debug rendering for the scene.
		m_spMasterScene->SetDebugRender(false);

		// We also have to tell the PhysX SDK to stop debug rendering
		m_pkPhysManager->m_pkPhysXSDK->setParameter(
			NX_VISUALIZATION_SCALE, 0.0f);
	}
	else
	{
		// Turn on debug rendering for the scene. We must give it a Gamebryo
		// node to attach the debug geometry to, in this case the scene graph.
		m_spMasterScene->SetDebugRender(true, m_spScene);

		// We also have to tell the PhysX SDK to do debug rendering
		m_pkPhysManager->m_pkPhysXSDK->setParameter(
			NX_VISUALIZATION_SCALE, 1.0f);
	}
}
//---------------------------------------------------------------------------
void Asynchronous::SetTurretControls()
{   
	m_spCamera->Update(0.0f);
	m_spTrnNode = NiNew NiNode();
	m_spTrnNode->SetTranslate(m_spCamera->GetWorldTranslate());
	m_spCamera->SetTranslate(NiPoint3::ZERO);
	m_spRotNode = NiNew NiNode();
	m_spTrnNode->AttachChild(m_spRotNode);
	m_spRotNode->SetRotate(m_spCamera->GetWorldRotate());
	m_spCamera->SetRotate(NiMatrix3::IDENTITY);
	m_spRotNode->AttachChild(m_spCamera);
	m_spTrnNode->Update(0.0f);

	float fTrnSpeed = 0.05f;
	float fRotSpeed = 0.005f;

	m_kTurret.SetStandardTrn(fTrnSpeed, m_spTrnNode);
	m_kTurret.SetStandardRot(fRotSpeed, m_spTrnNode, m_spRotNode);
	NiMatrix3 kRot;
	kRot.SetCol(0, 1.0f, 0.0f, 0.0f);
	kRot.SetCol(1, 0.0f, 0.0f, 1.0f);
	kRot.SetCol(2, 0.0f, -1.0f, 0.0f);
	m_kTurret.SetAxes(kRot);

	if (m_kTurret.GetInputDevice() == NiTurret::TUR_KEYBOARD)
	{
		m_kTurret.SetTrnButtonsKB(0,
			NiInputKeyboard::KEY_Z, NiInputKeyboard::KEY_X);
		m_kTurret.SetTrnButtonsKB(1,
			NiInputKeyboard::KEY_S, NiInputKeyboard::KEY_W);
		m_kTurret.SetTrnButtonsKB(2,
			NiInputKeyboard::KEY_A, NiInputKeyboard::KEY_D);

		//m_kTurret.SetRotButtonsKB(1,
		//	NiInputKeyboard::KEY_J, NiInputKeyboard::KEY_L);
		//m_kTurret.SetRotButtonsKBzz(2,
		//	NiInputKeyboard::KEY_M, NiInputKeyboard::KEY_K);
	}
	else if (m_kTurret.GetInputDevice() == NiTurret::TUR_GAMEPAD)
	{
		m_kTurret.SetTrnButtonsStickDirGP(0, 
			NiInputGamePad::NIGP_STICK_LEFT, 
			NiInputGamePad::NIGP_STICK_AXIS_V);
		m_kTurret.SetTrnButtonsGP(1, 
			NiInputGamePad::NIGP_L1, 
			NiInputGamePad::NIGP_R1);
		m_kTurret.SetTrnButtonsStickDirGP(2, 
			NiInputGamePad::NIGP_STICK_LEFT, 
			NiInputGamePad::NIGP_STICK_AXIS_H);

		m_kTurret.SetRotButtonsGP(0, NiInputGamePad::NIGP_NONE,
			NiInputGamePad::NIGP_NONE);
		m_kTurret.SetRotButtonsStickDirGP(1, 
			NiInputGamePad::NIGP_STICK_RIGHT, 
			NiInputGamePad::NIGP_STICK_AXIS_H);
		m_kTurret.SetRotButtonsStickDirGP(2, 
			NiInputGamePad::NIGP_STICK_RIGHT, 
			NiInputGamePad::NIGP_STICK_AXIS_V);
	}
}
//kart
void Asynchronous::ExtractShapeFromNif(NxVehicleDesc* cartDesc, NiAVObject* pNif)
{
	//if (!pNif)
	//	return;

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

	cartDesc->carShapes.pushBack(&boxShapes[0]);
	cartDesc->carShapes.pushBack(&boxShapes[1]);
	cartDesc->carShapes.pushBack(&boxShapes[2]);
	cartDesc->carShapes.pushBack(&boxShapes[3]);
	cartDesc->carShapes.pushBack(&boxShapes[4]);
	cartDesc->carShapes.pushBack(&capsuleShapes[0]);


}
void Asynchronous::createCart(const NiAVObject* pNif, const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, bool oldStyle, NxScene* gScene)
{
	NxVehicleDesc cartDesc;
	NxBoxShapeDesc boxShapes[5];
	NxCapsuleShapeDesc capsuleShapes[1];

	bool corvette = false;

	//NiAVObject* pPhsChassis = pNif->GetObjectByName("phs_chassis");
	//if (pPhsChassis)
	//{
	//	if (p)
	//}


	boxShapes[0].dimensions.set(60.0f, 10.0f, 40.0f);
	boxShapes[0].localPose.t.set(0,-20,0);

	//boxShapes[1].dimensions. set( 0.8f, 0.05f, 0.02f);
	//boxShapes[1].localPose.t.set( 0.0f, 0.07f, 0.48f);

	//boxShapes[2].dimensions. set( 0.8f, 0.05f, 0.02f);
	//boxShapes[2].localPose.t.set( 0.0f, 0.07f,-0.48f);

	//boxShapes[3].dimensions. set(0.02f, 0.05f, 0.46f);
	//boxShapes[3].localPose.t.set(0.78f, 0.07f,-0.0f);

	//boxShapes[4].dimensions. set(0.02f, 0.05f, 0.46f);
	//boxShapes[4].localPose.t.set(-0.78f, 0.07f,-0.0f);

	//capsuleShapes[0].height = 0.1f;
	//capsuleShapes[0].radius = 0.4f;
	//capsuleShapes[0].localPose.t.set(0,0.4f,0);

	cartDesc.carShapes.pushBack(&boxShapes[0]);
	//cartDesc.carShapes.pushBack(&boxShapes[1]);
	//cartDesc.carShapes.pushBack(&boxShapes[2]);
	//cartDesc.carShapes.pushBack(&boxShapes[3]);
	//cartDesc.carShapes.pushBack(&boxShapes[4]);
	//cartDesc.carShapes.pushBack(&capsuleShapes[0]);

	cartDesc.position				= pos;
	cartDesc.mass					= 1000;
	cartDesc.digitalSteeringDelta	= 0.1f;
	cartDesc.steeringMaxAngle		= 30.f;
	cartDesc.motorForce				= 500.f;
	cartDesc.centerOfMass.set(0,0,0);
	cartDesc.maxVelocity			= 60.f;
	cartDesc.cameraDistance		= 3.0f;

	NxVehicleMotorDesc motorDesc;
	NxVehicleGearDesc gearDesc;
	NxReal wheelRadius = 10.9f;
	if(corvette)
	{
		motorDesc.setToCorvette();
		cartDesc.motorDesc = &motorDesc;
		gearDesc.setToCorvette();
		cartDesc.gearDesc = &gearDesc;
		cartDesc.differentialRatio = 3.42f;
		//cartDesc.differentialRatio = 5.f;
		wheelRadius = 20.45f;
	}

	NxWheelDesc wheelDesc[4];
	for(NxU32 i = 0; i < 4; i++)
	{
		wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelRadius = wheelRadius;;
		wheelDesc[i].wheelWidth = 2.08f;
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
	NxReal frontPos = 40.7f;
	NxReal widthPos = 42.7f;
	NxReal heightPos = -5.0f;
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

/*
NxVehicleDesc cartDesc;
NxBoxShapeDesc boxShapes[5];
NxCapsuleShapeDesc capsuleShapes[1];

bool corvette = false;

//NiAVObject* pPhsChassis = pNif->GetObjectByName("phs_chassis");
//if (pPhsChassis)
//{
//	if (p)
//}


boxShapes[0].dimensions.set(0.8f, 0.02f, 0.5f);
boxShapes[0].localPose.t.set(0,0,0);

//boxShapes[1].dimensions. set( 0.8f, 0.05f, 0.02f);
//boxShapes[1].localPose.t.set( 0.0f, 0.07f, 0.48f);

//boxShapes[2].dimensions. set( 0.8f, 0.05f, 0.02f);
//boxShapes[2].localPose.t.set( 0.0f, 0.07f,-0.48f);

//boxShapes[3].dimensions. set(0.02f, 0.05f, 0.46f);
//boxShapes[3].localPose.t.set(0.78f, 0.07f,-0.0f);

//boxShapes[4].dimensions. set(0.02f, 0.05f, 0.46f);
//boxShapes[4].localPose.t.set(-0.78f, 0.07f,-0.0f);

//capsuleShapes[0].height = 0.1f;
//capsuleShapes[0].radius = 0.4f;
//capsuleShapes[0].localPose.t.set(0,0.4f,0);

cartDesc.carShapes.pushBack(&boxShapes[0]);
//cartDesc.carShapes.pushBack(&boxShapes[1]);
//cartDesc.carShapes.pushBack(&boxShapes[2]);
//cartDesc.carShapes.pushBack(&boxShapes[3]);
//cartDesc.carShapes.pushBack(&boxShapes[4]);
//cartDesc.carShapes.pushBack(&capsuleShapes[0]);

cartDesc.position				= pos;
cartDesc.mass					= 1000;
cartDesc.digitalSteeringDelta	= 0.1f;
cartDesc.steeringMaxAngle		= 30.f;
cartDesc.motorForce				= 500.f;
cartDesc.centerOfMass.set(0,0,0);
cartDesc.maxVelocity			= 30.f;
cartDesc.cameraDistance		= 3.0f;

NxVehicleMotorDesc motorDesc;
NxVehicleGearDesc gearDesc;
NxReal wheelRadius = 10.9f;
if(corvette)
{
	motorDesc.setToCorvette();
	cartDesc.motorDesc = &motorDesc;
	gearDesc.setToCorvette();
	cartDesc.gearDesc = &gearDesc;
	cartDesc.differentialRatio = 3.42f;
	//cartDesc.differentialRatio = 5.f;
	wheelRadius = 20.45f;
}

NxWheelDesc wheelDesc[4];
for(NxU32 i = 0; i < 4; i++)
{
	wheelDesc[i].wheelApproximation = 10;
	wheelDesc[i].wheelRadius = wheelRadius;;
	wheelDesc[i].wheelWidth = 2.08f;
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
NxReal frontPos = 40.7f;
NxReal widthPos = 42.7f;
NxReal heightPos = 5.0f;
//wheelDesc[0].position.set( frontPos, heightPos, widthPos);
//wheelDesc[1].position.set( frontPos, heightPos,-widthPos);
//wheelDesc[2].position.set(-frontPos, heightPos, widthPos);
//wheelDesc[3].position.set(-frontPos, heightPos,-widthPos);
wheelDesc[0].position.set( frontPos, widthPos, heightPos);
wheelDesc[1].position.set( frontPos,-widthPos, heightPos);
wheelDesc[2].position.set(-frontPos, widthPos, heightPos);
wheelDesc[3].position.set(-frontPos,-widthPos, heightPos);

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
*/

}