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

#ifndef ASYNCHRONOUS_H
#define ASYNCHRONOUS_H

#include <NiApplication.h>
#include <NiAnimation.h>
#include <NiPhysX.h>

#include "NxVehicleDesc.h"
#include "NxVehicle.h"
#include "NxAllVehicles.h"
#include "kart.h"

class Asynchronous : public NiApplication
{
public:
    Asynchronous();
    ~Asynchronous();
    
    bool Initialize();
    void Terminate();
    void UpdateFrame();
    void ProcessInput();





//kart
void ExtractShapeFromNif(NxVehicleDesc* cartDesc, NiAVObject* pNif);
void createCart(const NiAVObject* pNif, const NxVec3& pos, bool frontWheelDrive, bool backWheelDrive, bool oldStyle, NxScene* gScene);





protected:
    // Create all the contents of the scene
    bool CreateScene();
    
    // Reset the balls position back to its initial conditions.
    void ResetBall();
    
    // Toggle the debug visualization state
    void ToggleDebug();
    
    // Remap the keys used to control the view.
    void SetTurretControls();

    // Physics related members
    NiPhysXManager* m_pkPhysManager;
    NiPhysXScenePtr m_spMasterScene;
    NiPhysXScenePtr m_spBallScene;
    float fLastSimTime;


	CKart m_kart;

    // Keyboard and gamepad handling for the camera.
    NiTurret m_kTurret;
    NiNodePtr m_spTrnNode;
    NiNodePtr m_spRotNode;
	NiAVObjectPtr m_spCarRoot;

	NiPhysXTransformDestPtr m_spBodyDest;
	NiPhysXTransformDestPtr m_spWheelDests[4];
};

#endif // #ifdef ASYNCHRONOUS_H
