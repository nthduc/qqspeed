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
#include "Top-KartPCH.h"

#include <NiRenderedTexture.h>
#include <NiRenderTargetGroup.h>
#include <NiDrawSceneUtility.h>

#include "PerspectiveMirror.h"
#include "common.h"
#include <NiCamera.h>

//#include "CameraManager.h"


//---------------------------------------------------------------------------
NiMatrix3 MakeCameraMatrix(float fPitch, float fYaw)
{
	NiMatrix3 kCameraRotation, kTemp;
	//Rotate and orient the camera so that Z is up.
	kCameraRotation.MakeXRotation(-1.0f * NI_HALF_PI);
	//Pitch around Y which is the current right vector.
	kTemp.MakeYRotation(fPitch);
	kCameraRotation = kTemp * kCameraRotation;

	//Make the Z rotation here around world Z since we've defined world Z
	//as up for our application.
	kTemp.MakeZRotation(fYaw);
	kCameraRotation = kTemp * kCameraRotation;
	return kCameraRotation;
}
//--------------------------------------------------------------------------
inline void CalculatePitchAndYaw(NiCamera* pkCamera, float & fPitch, float & fYaw)
{
	//Extract the camera's pitch and yaw from the view direction of the
	//camera.
	NiPoint3 kViewDirection = pkCamera->GetWorldDirection();

	fPitch = NiASin(kViewDirection.z);

	kViewDirection.z = 0;
	kViewDirection.Unitize();
	fYaw = NiATan2(kViewDirection.y, kViewDirection.x);
	fYaw *= -1.0f;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
PerspectiveMirror::PerspectiveMirror(NiRenderer* pkRenderer, NiNode* pkScene, NiNode * pkMirror,									 
									 const NiFixedString & kMirrorNodeName, const NiFixedString & kMirrorGeoName)
    : m_kVisible(128, 128)
	, m_kCuller(&m_kVisible)
	, m_spMirrorEnvMap(0)
	, m_kName("")
	, m_bShow(true)
	, m_pkMirror(NULL)
{
    m_spCamera = NiNew NiCamera;
    m_spScene = pkScene;

    m_spRenderer = pkRenderer;

    m_spRenderedTexture = NiRenderedTexture::Create(512, 512, pkRenderer);
    assert(m_spRenderedTexture);
    m_spRenderTargetGroup = NiRenderTargetGroup::Create(
        m_spRenderedTexture->GetBuffer(), pkRenderer, true, true);
    assert(m_spRenderTargetGroup);
    m_kCenter = NiPoint3::ZERO;
    m_kDir = NiPoint3::UNIT_X; 
    m_kUp = NiPoint3::UNIT_Y;
    m_kRight = NiPoint3::UNIT_Z;


	NiNode* pkNode = (NiNode*)pkMirror->GetObjectByName(kMirrorNodeName);
	NiTriShape* pkMirrorGeo;
	//NiTriStrips;
	NiNode* pkMirrorNode;
	if (pkNode)
	{
		pkMirrorNode = pkNode;
		m_pkMirror = pkNode;
		pkMirrorGeo = (NiTriShape*)pkNode->GetObjectByName(kMirrorGeoName);
	}
	else
	{
		NiMessageBox("Failed to find node for reflection attachment.", 
			"Reflection Failure");		
		return;
	}

	// Set the texture
	NiTexturingProperty* pkTP = (NiTexturingProperty*)pkMirrorGeo->
		GetProperty(NiProperty::TEXTURING);
	if (!pkTP)
	{
		LogText("No Texturing Property found for Mirror!");
		NiMessageBox("No Texturing Property found for Mirror!", 
			"Reflection Failure");	
		return;
	}

	//Clone the texturing property since there may be other items using it.
	NiTexturingProperty* pkNewProperty = (NiTexturingProperty*)pkTP->Clone();

	// Attach the mirror texture as a statically projected environment map
	m_spMirrorEnvMap = NiNew NiTextureEffect;
	m_spMirrorEnvMap->SetEffectTexture(m_spRenderedTexture);
	m_spMirrorEnvMap->SetTextureType(NiTextureEffect::ENVIRONMENT_MAP);
	m_spMirrorEnvMap->SetTextureFilter(NiTexturingProperty::FILTER_BILERP);
	m_spMirrorEnvMap->SetTextureClamp(NiTexturingProperty::WRAP_S_WRAP_T);
	m_spMirrorEnvMap->SetTextureCoordGen(NiTextureEffect::WORLD_PERSPECTIVE);
	
	m_kProjectMatrix.SetRow(0, 0.5f, 0.0f, 0.5f / 0.697f);
	m_kProjectMatrix.SetRow(1, 0.5f, 0.5f / 0.523f , 0.0f);
	m_kProjectMatrix.SetRow(2, 1.0f, 0.0f, 0.0f);
	m_spMirrorEnvMap->SetModelProjectionMatrix(m_kProjectMatrix);
	m_spMirrorEnvMap->SetModelProjectionTranslation(
		NiPoint3(0.0f, 0.0f, 0.0f));
	m_spMirrorEnvMap->AttachAffectedNode(pkNode);
	m_spMirrorEnvMap->SetSwitch(true);
	m_spMirrorEnvMap->Update(0.0f);

	pkMirrorGeo->RemoveProperty(NiProperty::TEXTURING);
	pkMirrorGeo->AttachProperty(pkNewProperty);	

	pkMirrorGeo->Update(0.0f);
}
//---------------------------------------------------------------------------
PerspectiveMirror::~PerspectiveMirror()
{
	m_spMirrorEnvMap = 0;
	m_spRenderTargetGroup = 0;
	m_spRenderer = 0;
}
//---------------------------------------------------------------------------
NiTexture* PerspectiveMirror::GetImage()
{
    return NiSmartPointerCast(NiTexture,m_spRenderedTexture);
}
//---------------------------------------------------------------------------
void PerspectiveMirror::Update(NiCamera* pkCam)
{
	if (!m_bShow) 
	{
		return;
	}

	

    if (!m_spRenderedTexture || !m_spMirrorEnvMap) 
	{
		LogText("PerspectiveMirror::Update. m_spRenderedTexture or m_spMirrorEnvMap was NULL\n");
		return;
	}

	m_pkMirror->SetAppCulled(true);

	NiMatrix3 kWorldRotation;
	kWorldRotation.SetCol(0, pkCam->GetWorldDirection());
	kWorldRotation.SetCol(1, pkCam->GetWorldUpVector());
	kWorldRotation.SetCol(2, pkCam->GetWorldRightVector());
	m_spMirrorEnvMap->SetRotate(kWorldRotation);
	m_spMirrorEnvMap->SetTranslate(pkCam->GetWorldLocation());

	m_spMirrorEnvMap->Update(0.0f);
	
	NiFrustum kFrustum = pkCam->GetViewFrustum();
	NiMatrix3 kProjection;
	kProjection.SetRow(0, 0.5f, 0.0f, 0.5f / kFrustum.m_fRight);
	kProjection.SetRow(1, 0.5f, 0.5f / kFrustum.m_fTop, 0.0f);
	kProjection.SetRow(2, 1.0f, 0.0f, 0.0f);
	m_spMirrorEnvMap->SetModelProjectionMatrix(kProjection);


    // set the mirror camera's rotation matrix
    NiMatrix3 kRot;

    //if (!m_pkCameraManager->IsCloned())
	float fPitch = 0.0f, fYaw = 0.0f;
    CalculatePitchAndYaw(pkCam, fPitch, fYaw);

    //Invert the pitch here since we will reflect the camera across the plane.
    //Yaw should remain the same.
    kRot = MakeCameraMatrix(-1.0f * fPitch, fYaw);

    m_spCamera->SetRotate(kRot);

    NiPoint3 kLoc = pkCam->GetWorldTranslate();

    const NiPoint3 kLmC = kLoc - m_kCenter;

    // reflect loc across the plane = L - 2((L - C) DOT N) N
    kLoc -= 2.0f * m_kUp * (m_kUp * kLmC);

    m_spCamera->SetTranslate(kLoc);

    NiFrustum kFr;

    kFr = pkCam->GetViewFrustum();

    m_spCamera->SetViewFrustum(kFr);

    //Render the mirrored scene
    m_spCamera->Update(0.0f);

    m_spRenderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup,
        NiRenderer::CLEAR_ALL);
    
    NiDrawScene(m_spCamera, m_spScene, m_kCuller);
    
    m_spRenderer->EndUsingRenderTargetGroup();

	m_pkMirror->SetAppCulled(false);
}
//---------------------------------------------------------------------------
void PerspectiveMirror::SetPlane(const NiPoint3& kCenter, 
    const NiPoint3& kDir, const NiPoint3& kUp)
{
    m_kCenter = kCenter;
    m_kDir = kDir; 
    m_kUp = kUp;
    m_kRight = m_kDir.UnitCross(m_kUp);		
}
//---------------------------------------------------------------------------
