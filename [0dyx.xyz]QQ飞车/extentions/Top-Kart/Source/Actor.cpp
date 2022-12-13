

#include "Top-KartPCH.h"

#include <NiFloatExtraData.h>
#include <NiTriShape.h>
#include <NiTriStrips.h>
#include <NiVertexColorProperty.h>
#include <NxMath.h>

#include "CollisionManager.h"
#include "Actor.h"
//#include "LevelManager.h"
//#include "ShadowGeometry.h"
#include "NxSceneMgr.h"
#include "Scheduler.h"
#include "Footprints.h"
#include "PhysXUserData.h"
#include "Utility.h"
#include "LM_Anim.h"
#include "LB_Anim.h"
#include "left_Anim.h"
#include "EventContainer.h"
#include "SceneLevel.h"
#include "Profiling.h"
#include "screenobjects.h"

#include "NiTriShapeData.h"
#include "TerrainManager.h"
#include "Platform/GlobalInterface.h"
#include "framework/GameApp_i.h"
#include "IDisplayD3D9/Texture9.h"


const float Actor::m_fMaxHealth = 100.0f;
const float Actor::m_sfZeroVelocityMaximum = 0.1f * 0.1f;
const float Actor::ms_fJumpSpeed = 180.0f;
bool Actor::ms_bEyesStripified;
bool Actor::ms_bSkinStripified;
NiCriticalSection Actor::m_kCriticalSection;

static const NiColorA gs_cBKColor = NiColorA(0.0f, 0.0f, 0.0f, 0.0f);

//---------------------------------------------------------------------------
void Actor::Initialize(unsigned int uiShadowDetail, const char * pcName)
{
  	m_bFirstRender = true;

	NiNodePtr pkPlayNode = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("playermode"));
	if(!pkPlayNode)
	{
		return;
	}
	pkPlayNode->AttachChild(m_spCharacterAM->GetNIFRoot());
	m_spKartAM->GetNIFRoot()->UpdateNodeBound();
	m_spKartAM->GetNIFRoot()->UpdateEffects();
	m_spKartAM->GetNIFRoot()->UpdateProperties();
	m_spKartAM->GetNIFRoot()->Update(0);
	
    NiAVObject * pkSkin = m_spKartAM->GetNIFRoot();

	
	m_spSkin =NiDynamicCast(NiNode, pkSkin);
	m_spLowResSkin = NULL;

	m_spPlayer = NiDynamicCast(NiNode, m_spCharacterAM->GetNIFRoot()->GetObjectByName("Avatar"));
	assert(m_spPlayer);


	NiTriBasedGeom* pkFaceShape = NiDynamicCast(NiTriBasedGeom, m_spPlayer->GetObjectByName("face"));
	m_spOriginFaceTexProp = (NiTexturingProperty*) pkFaceShape->GetProperty(NiProperty::TEXTURING);

	m_spOriginFaceTex = m_spOriginFaceTexProp->GetBaseTexture();

   //角色朝向
   m_spLookAt = NiDynamicCast(NiNode, GetKartNIFRoot()->GetObjectByName("Scene Root"));

    assert(m_spLookAt);

    m_spKartAM->GetNIFRoot()->GetRotate(m_kBipQuat);
    m_spLookAt->GetRotate(m_kLookAtQuat);

    m_kBipQuat.SetX(0.0f);
    m_kBipQuat.SetY(0.0f);
    m_kLookAtQuat.SetX(0.0f);
    m_kLookAtQuat.SetY(0.0f);

   
    bool bSelectedUpdate = true;
    bool bRigid = false;
    m_spKartAM->GetNIFRoot()->SetSelectiveUpdateFlags(bSelectedUpdate, true, 
        bRigid);

    m_spKartAM->GetNIFRoot()->UpdateProperties();
    m_spKartAM->GetNIFRoot()->UpdateEffects();
    m_spKartAM->GetNIFRoot()->Update(0.0f);
    m_spKartAM->GetNIFRoot()->UpdateNodeBound();

	InitializeLights(NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()));

    InitializeRandomIdle();

	InitializeName(pcName);

	ShowName(true);

	m_uiShadowDetail = 0;
}
//---------------------------------------------------------------------------
Actor::Actor() : m_fElevation(0.0f), 
     m_pkShadowGeometry(0),
    m_bIsIdling(false), m_fHealth(m_fMaxHealth),
    m_fMinColliderDistance(50.0f),m_fMinColliderDistanceSQRD(50.0f*50.0f),
    m_fAvoidCollisionsFactor(0.5f), m_uiCurrentFurLOD(0), m_spPlayer(0), 
	/*m_bPortraitVisible(false),*/ m_pkNameFace(NULL), m_bShowName(false), 
	m_nPlayNameResID(-1), m_spkNameBoard(0)
{
}
//---------------------------------------------------------------------------
Actor::~Actor()
{	
	
	Release();
}

void Actor::Release()
{
	if(m_pkNameFace)
	{
		m_pkNameFace->DestroyElement(m_nPlayNameResID);
	}
	m_nPlayNameResID = -1;
	m_spkNameBoard = 0;
	SAFE_NIDELETE(m_pkNameFace);
	m_spPlayer = 0;
	m_spOriginFaceTexProp = 0;
	m_arrSceneLights.RemoveAll();
	SAFE_NIDELETE(m_pkShadowGeometry);
	m_spCharacterAM = 0;
	m_spKartAM = 0;
	m_spHairAM = 0;
}

void Actor::ReBindAll()
{
	// 邦定头发到人头上
	// assert(m_spHairAM);
	assert(m_spKartAM);
	assert(m_spCharacterAM);

	if (m_spHairAM)
	{
		NiNodePtr pkHeadNode = NiDynamicCast(NiNode, m_spCharacterAM->GetNIFRoot()->GetObjectByName("head"));
		if (!pkHeadNode)
			return;
		pkHeadNode->AttachChild(m_spHairAM->GetNIFRoot());
	}

	// 绑定人到车上
	NiNodePtr pkPlayNode = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("playermode"));
	if(!pkPlayNode)
	{
		return;
	}
	pkPlayNode->AttachChild(m_spCharacterAM->GetNIFRoot());

	AttachHint(m_spkNameBoard);
	ShowName(m_bShowName);

	m_spSkin = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot());

	// 更新
	m_spSkin->UpdateNodeBound();
	m_spSkin->UpdateEffects();
	m_spSkin->UpdateProperties();
/*	m_spSkin->Update(0);*/	

	Actor::Update(GetMainApp()->GetCurrentTime(), 0, NiPoint2(0, 0),  0, true);
}

void Actor::UnloadKart()
{
	m_spSkin   = NULL;
	m_spKartAM = NULL;
	m_spOriginFaceTexProp = 0;
	SAFE_NIDELETE(m_pkShadowGeometry);
}

void Actor::UnLoadHair()
{
	NiNode * pkParent = NULL;
	if (m_spHairAM)
	{
		pkParent = m_spHairAM->GetNIFRoot()->GetParent();
		if (pkParent)
			pkParent->DetachChild(m_spHairAM->GetNIFRoot());
		m_spHairAM = NULL;
	}
}

void Actor::UnLoadCharacter()
{
	NiNode * pkParent = NULL;
	if (m_spCharacterAM)
	{
		NiNode * pkSelfNode = NiDynamicCast(NiNode, m_spCharacterAM->GetNIFRoot());
		assert(pkSelfNode);
		pkParent = pkSelfNode->GetParent();
		
		if (pkParent)
			pkParent->DetachChild(pkSelfNode);
		if (m_spHairAM)
			pkSelfNode->DetachChild(m_spHairAM->GetNIFRoot());
		
		m_spCharacterAM = NULL;
	}
}

bool Actor::InitializeName(const char * pcName)
{
	if (!pcName) 
	{
		assert(pcName);
		return false;
	}

	//m_pkNameFace = NiNew GFX::CUIFace(500, 100) ;
	//修改tnt2显卡兼容性 pighuang 20070313
	m_pkNameFace = NiNew GFX::CUIFace(512, 128) ;

	RECT rcScr, rcDest ;
	rcScr.top = 0;
	rcScr.left = 0;
	rcScr.bottom = 128 ;
	rcScr.right = 512 ;
	rcDest.top = 0 ;
	rcDest.left = 0 ;
	rcDest.bottom = 128 ;
	rcDest.right = 512 ;

	m_nPlayNameResID =  m_pkNameFace->CreateTextElement(pcName, "SystemFont", 0xfff9f0ef, rcDest, 
		NULL,  GUI_DT_VCENTER | GUI_DT_HCENTER );

	NiStream kStream;
	kStream.Load(NiApplication::ConvertMediaFilename("map/nameboard.nif"));

	m_spkNameBoard = NiDynamicCast(NiAVObject, kStream.GetObjectAt(0));


	NiAVObject * pkPlane = m_spkNameBoard->GetObjectByName("Plane");

	NiTexturingProperty * pkProp = (NiTexturingProperty *)pkPlane->GetProperty(NiTexturingProperty::GetType());

	pkProp->SetBaseTexture(m_pkNameFace->GetRenderTarget());

	AttachHint(m_spkNameBoard);

	ShowName(m_bShowName);

	return true;
}

void Actor::OnLostDevice(bool bReset)
{
	if ( bReset && m_pkNameFace)
	{
		m_pkNameFace->MarkUpdate();
	}
}

void Actor::RenderName()
{
	if (m_pkNameFace) 
	{
		m_pkNameFace->UpdateFace();
	}
}

void Actor::ShowName(bool bShow)
{
	assert(m_spkNameBoard);
	if (m_spkNameBoard)
	{
		m_bShowName = bShow;

		m_spkNameBoard->SetAppCulled(!bShow);
	}
}

bool Actor::InitializeLights(NiNode * pkNode)
{
	assert(pkNode);

	bool bSuccess = false;
	if (pkNode)
	{
		m_spLightNode = NiDynamicCast(NiNode, pkNode->GetObjectByName("light"));
		
		if (m_spLightNode) 
		{
			unsigned int unLightNum = m_spLightNode->GetChildCount();

			for (unsigned int i = 0; i < unLightNum; ++i)
			{
				NiLightPtr spLight = NiDynamicCast(NiLight, m_spLightNode->GetAt(i));

				if (spLight)
				{
					m_arrSceneLights.Add(spLight);
				}
			}
			if (m_arrSceneLights.GetEffectiveSize())
				bSuccess = true;
		}
	}

	return bSuccess;
}

void Actor::LightObject(NiNode * pkObject)
{
	assert(pkObject);

	if (pkObject)
	{
		unsigned int unLightNum = m_arrSceneLights.GetEffectiveSize();

		for (unsigned int i = 0; i < unLightNum; ++i)
		{
			NiLight * pkLight = m_arrSceneLights.GetAt(i);
			pkLight->AttachAffectedNode(pkObject);
			pkLight->UpdateEffects();
		}
		pkObject->UpdateEffects();
	}	
}

void Actor::UnLightObject(NiNode * pkObject)
{
	assert(pkObject);

	if (pkObject)
	{
		unsigned int unLightNum = m_arrSceneLights.GetEffectiveSize();

		for (unsigned int i = 0; i < unLightNum; ++i)
		{
			NiLight * pkLight = m_arrSceneLights.GetAt(i);
			pkLight->AttachUnaffectedNode(pkObject);
			pkLight->UpdateEffects();
		}
		pkObject->UpdateEffects();
	}	
}

void Actor::ChangeFaceTexture(const NiSourceTexturePtr& spTex)
{
	//assert(m_spOriginFaceTexProp);
	if ( !m_spOriginFaceTexProp )
	{
		NiTriBasedGeom* pkFaceShape = NiDynamicCast(NiTriBasedGeom, m_spPlayer->GetObjectByName("face"));
		m_spOriginFaceTexProp = (NiTexturingProperty*) pkFaceShape->GetProperty(NiProperty::TEXTURING);
	}
	m_spOriginFaceTexProp->SetBaseTexture(spTex);
}

void Actor::ChangeFaceTexture(const NiFixedString & kName)
{
	//NiFixedString kNewName = NiApplication::ConvertMediaFilename(MakeString("Kart/one_Anim1/emotion/%s.dds", (const char *)kName));

	NiSourceTexture * pkBaseTexture = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(kName));
	if ( !m_spOriginFaceTexProp )
	{
		NiTriBasedGeom* pkFaceShape = NiDynamicCast(NiTriBasedGeom, m_spPlayer->GetObjectByName("face"));
		m_spOriginFaceTexProp = (NiTexturingProperty*) pkFaceShape->GetProperty(NiProperty::TEXTURING);
	}

	m_spOriginFaceTexProp->SetBaseTexture(pkBaseTexture);
}

void Actor::RecoverFaceTexture()
{
	if ( !m_spOriginFaceTexProp || !m_spOriginFaceTex )
	{
		NiTriBasedGeom* pkFaceShape = NiDynamicCast(NiTriBasedGeom, m_spPlayer->GetObjectByName("face"));
		m_spOriginFaceTexProp = (NiTexturingProperty*) pkFaceShape->GetProperty(NiProperty::TEXTURING);
		m_spOriginFaceTex = m_spOriginFaceTexProp->GetBaseTexture();
	}
	m_spOriginFaceTexProp->SetBaseTexture(m_spOriginFaceTex);
}

void Actor::StartShadow(unsigned int uiShadowDetail, unsigned int uiMaxShadowTriSize)
{
	SAFE_NIDELETE(m_pkShadowGeometry);
	m_uiShadowDetail = uiShadowDetail;
	if (uiShadowDetail != 0 && CLevelManager::Get() && CLevelManager::Get()->GetPhysXManager()) 
	{
		// 这段代码需要重新考虑
		m_pkShadowGeometry = ShadowGeometry::Create(uiShadowDetail, uiMaxShadowTriSize, "data/win32/textures/car_sh.dds");
		if (!m_pkShadowGeometry)
		{
			Log("BipedCharacter::Initialize -- "
				"KFM Load Failed\n");
			assert(0);
			return ;
		}

		m_pkShadowGeometry->SetCaster(m_spKartAM->GetNIFRoot()->GetObjectByName("carmode"));

		m_pkShadowGeometry->SetGroundSceneGraph(
			CLevelManager::Get()->GetPhysXManager()->m_pTerrainMgr->GetTerrainGraph());
	}
}

void Actor::TerminateShadow()
{
	SAFE_NIDELETE(m_pkShadowGeometry);
}

//---------------------------------------------------------------------------
void Actor::Update(float fTime, float fFrameTime, 
    const NiPoint2& kDir,  const float& fStrafeDir, bool bAnimationUpdate)
{
    // 修改人：wincoqin
	// 时间:   2007/1/22
    // 修改方式：删除
    // 原因： 现在的游戏中不需要这个函数作碰撞判断   
    // NiPoint2 kNewDir = AvoidCollisions(kDir);
	// 修改结束

	fTime = GetMainApp()->GetAccumTime();
	if(bAnimationUpdate)
	{
		m_spCharacterAM->Update(fTime);
		m_spKartAM->Update(fTime);
	}

	// 修改人：wincoqin
	// 时间:   2007/1/22
	// 修改方式：删除
	// 原因： 现在的游戏中不需要这个函数
    //UpdateBipedRootTransforms(kNewDir, fStrafeDir, fFrameTime);
	// 修改结束

	// 更新光效
	if (m_spLightNode)
	{
		m_spLightNode->UpdateEffects();
	}

	m_spKartAM->GetNIFRoot()->Update(fTime);
}

//---------------------------------------------------------------------------
void Actor::Draw(NiCamera* pkCamera)
{

	NiVisibleArray m_kVisible;
	CCustomCullingProcess  m_kCustomCuller(&m_kVisible);
	NiCullScene(pkCamera, m_spSkin, m_kCustomCuller, m_kVisible );
	if ( m_pkShadowGeometry)
		NiCullScene(pkCamera, m_pkShadowGeometry->GetShadowGeometry(), m_kCustomCuller, m_kVisible,false);
	m_kCriticalSection.Lock();
	NiDrawVisibleArray(pkCamera, m_kVisible);
	m_kCriticalSection.Unlock();


	//Statistics::AddVisibleArray(&kArray, Statistics::GT_CAR);

	//DrawShadow(pkCamera);
}
//---------------------------------------------------------------------------
void Actor::UpdateBipedRootTransforms(NiPoint2& kDir,
    const float& fStrafeDir, float fFrameTime)
{
    NiActorManager::SequenceID eSequence = m_spKartAM->GetTargetAnimation();
}
//---------------------------------------------------------------------------
void Actor::TextKeyEvent(NiActorManager* pkManager,
    NiActorManager::SequenceID eSequenceID, const NiFixedString& pcTextKey,
    const NiTextKeyMatch* pkMatchObject, float fCurrentTime, float fEventTime)
{
  
}
//---------------------------------------------------------------------------
void Actor::HandleTextKeyEvent()
{
}
//---------------------------------------------------------------------------
NiCamera * Actor::GetCamera(const char * szCameraName)
{
	NiAVObjectPtr spRoot = GetKartNIFRoot();
	NiCamera * pkCamera = NULL;
	if (spRoot) 
	{
		NiAVObject * pkNode = spRoot->GetObjectByName(szCameraName);

		if (NiIsKindOf(NiCamera, pkNode)) 
		{
			return (NiCamera*)pkNode;
		}

		if (pkNode)
		{
			pkCamera = NiDynamicCast(NiCamera, (NiDynamicCast(NiNode, pkNode)->GetAt(0)));
		}
		assert(pkCamera);
	}
	return pkCamera;
}

//---------------------------------------------------------------------------
void Actor::RenderShadow(NiCamera* pkSceneCamera)
{
	if (!m_uiShadowDetail)
		return;

	if (m_pkShadowGeometry)
	{
		PROFILE("Actor::RenderShadow");
		// The flags here allow future expansion to support things like LOD'ed
		// shadows that might be turned off when far from the camera, etc.
		bool bShadowVisible = true;
		bool bUpdateShadowImage = true;
		// 只有显示本地玩家的阴影
		Play * pPlay = (Play*)this;
		if ( pPlay->GetKart() )
		{
			NiPoint3 kDir = NiPoint3(0.0f,0.0f, 1.0f);
			CTencentCar * pCar = pPlay->GetKart()->GetVehicle();
			if ( pCar )
			{
				kDir.x = pCar->GetUpGlobal().x;
				kDir.y = pCar->GetUpGlobal().y;
				kDir.z = pCar->GetUpGlobal().z;
				//m_pkShadowGeometry->SetLightDir(-kDir);
			}
		}


		m_pkShadowGeometry->Click(0.0f, pkSceneCamera, bShadowVisible, 
			bUpdateShadowImage);
	}
}
//---------------------------------------------------------------------------
void Actor::ForceRenderShadow()
{
	// Used in Win32 when ALT-TAB occurs.
	if (m_pkShadowGeometry)
	{
		m_pkShadowGeometry->ForceRender();
	}
}
//---------------------------------------------------------------------------
void Actor::DrawShadow(NiCamera* pkCamera)
{
	if (m_pkShadowGeometry)
	{
		CCustomCullingProcess & kCuller = GetMainApp()->GetCuller();
		NiVisibleArray & kArray = GetMainApp()->GetVisibleArray();
		NiCullScene(pkCamera, m_pkShadowGeometry->GetShadowGeometry(), kCuller, kArray);
		NiDrawVisibleArray(pkCamera, kArray);
	}
}

//-----------------------------------------------------------------------
void Actor::ToggleBoneLOD(int nLod)
{
	NiBoneLODController* pkBLOD = 
		m_spCharacterAM->GetBoneLODController();

	if ( nLod < 0 || nLod > pkBLOD->GetNumberOfBoneLODs() 
		|| pkBLOD->GetBoneLOD() == nLod )
		return;

	if (pkBLOD && pkBLOD->GetNumberOfBoneLODs() > 1)
	{
		pkBLOD->SetBoneLOD(nLod);
	}
}
