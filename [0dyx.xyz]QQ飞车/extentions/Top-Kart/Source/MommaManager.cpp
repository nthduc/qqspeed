

#include "Top-KartPCH.h"

#include "MommaManager.h"
#include "MommaTasks.h"
#include "Scheduler.h"
#include "TerrainManager.h"
#include "LM_Anim.h"
#include <NiApplication.h>
#include "Footprints.h"
#include "SoundManager.h"
#include "PhysXUserData.h"
#include <NiPhysX.h>

#if defined(WIN32)
#include "Lenguins.h"
#endif

//---------------------------------------------------------------------------
MommaManager::MommaManager() :  m_bTriggerJump(false),
    m_fLastInputTime(0.0f), m_fAutoModeTime(15.0f), 
    m_kAutoGoal(NiPoint2::ZERO), m_uiMommaID(INT_MAX), m_uiPadID(INT_MAX),
    m_bRumble(true), m_fPausedTime(0.0f),
    m_fPauseTime(15.0f), m_kSpawn(NiPoint3::ZERO), m_bAutoWalk(false),
    m_fMaxAutoGoalSeekTime(60.0f), m_bTriggerIceBreak(false), 
    m_bExcutingAction(false)
{
}
//---------------------------------------------------------------------------
MommaManager::~MommaManager()
{

}
//---------------------------------------------------------------------------
bool MommaManager::Initialize(const NiPoint3& kSpawn, unsigned int uiMommaID,
    unsigned int uiPadID)
{   
    m_kSpawn = kSpawn;
    m_uiMommaID = uiMommaID;
    m_uiPadID = uiPadID;
    m_fBodyFollowResponse = 9.0f;
    m_fLookAtResponse = 9.0f;
    m_fSpeedMultiplier = 6.5f;

    NiActorManager* pkAM;

    if (LevelManager::GetUseShaders())
    {
        pkAM = NiActorManager::Create(
            NiApplication::ConvertMediaFilename("MOMMA/LM-Shader.KFM"));
    }
    else
    {
        pkAM = NiActorManager::Create(
            NiApplication::ConvertMediaFilename("MOMMA/LM.KFM"));
    }

    if (!pkAM)
    {
        NiOutputDebugString("MommaManager::Initialize -- "
            "Momma KFM Load Failed\n");
        return false;
    }

    LevelManager::Get()->LightObject((NiNode*)pkAM->GetNIFRoot());
    pkAM->GetNIFRoot()->UpdateProperties();

    // Prepack and add to static geometry group.
    // Note - We don't want to prepack the eyes because they are morphed.
    // However, there is no NiGeomMorpherController on the eyes
    // at this point -- it is added by the 
    // NiActorManager/NiControllerManager later, as the animations
    // are activated.  Therefore, just pack the skin.

    NiAVObject* pkSkin = pkAM->GetNIFRoot()->GetObjectByName("LM01-HIGHRES");

#ifdef WIN32
    NiGeometryGroup* pkStaticGeomGroup = 
        NiGeometryGroupManager::GetManager()->CreateGroup(
        NiGeometryGroupManager::STATIC);

    LevelManager::RecursiveAddToStaticGeometryGroup(pkSkin, pkStaticGeomGroup);
#endif

    LevelManager::RecursivePrepack(pkAM->GetNIFRoot(), true);
    LevelManager::RecursivePrepack(pkSkin, false);

    NiStream kStream;
    if(!kStream.Load(NiApplication::ConvertMediaFilename(
        "LEVELS/LM-FP01.NIF")))
    {
        NiOutputDebugString("MommaManager::Initialize -- "
            "Footprint NIF Load Failed\n");
        return false;
    }
 
    NiAVObject* pkFootprint = ((NiNode*)kStream.GetObjectAt(0))->GetAt(0);
    NiAVObject* pkBoneRoot = (NiAVObject*)pkAM->GetControllerManager()->
        GetTarget();
    NiAVObject* pkRightFoot = pkBoneRoot->
        GetObjectByName("node-RightFoot");
    NiAVObject* pkLeftFoot = pkBoneRoot->
        GetObjectByName("node-LeftFoot");
    // setup callbacks for footsteps
    // Note: Using NiActorManager::ANY_EVENT_CODE would be convenient here,
    // but it is less efficient than requesting callbacks for only the
    // sequences we know will have such events.
    pkAM->SetCallbackObject(this);
    pkAM->RegisterCallback(NiActorManager::TEXT_KEY_EVENT, 
        LM_Anim::WALKFORWARD, "rfd");
    pkAM->RegisterCallback(NiActorManager::TEXT_KEY_EVENT, 
        LM_Anim::WALKFORWARD, "lfd");
    pkAM->RegisterCallback(NiActorManager::TEXT_KEY_EVENT, 
        LM_Anim::RUN_V2, "rfd");
    pkAM->RegisterCallback(NiActorManager::TEXT_KEY_EVENT, 
        LM_Anim::RUN_V2, "lfd");
    pkAM->RegisterCallback(NiActorManager::ANIM_ACTIVATED,
        LM_Anim::JUMP01);
    pkAM->RegisterCallback(NiActorManager::ANIM_ACTIVATED,
        LM_Anim::JUMPLEFT);
    pkAM->RegisterCallback(NiActorManager::ANIM_ACTIVATED,
        LM_Anim::JUMPRIGHT);
    pkAM->RegisterCallback(NiActorManager::END_OF_SEQUENCE,
        LM_Anim::JUMP01);
    pkAM->RegisterCallback(NiActorManager::END_OF_SEQUENCE,
        LM_Anim::JUMPLEFT);
    pkAM->RegisterCallback(NiActorManager::END_OF_SEQUENCE,
        LM_Anim::JUMPRIGHT);
    pkAM->RegisterCallback(NiActorManager::END_OF_SEQUENCE,
        LM_Anim::ICEBREAK01);

    pkAM->SetTargetAnimation(LM_Anim::IDLE01BASE);
    pkAM->Update(0.0f);


    pkAM->GetNIFRoot()->SetTranslate(m_kSpawn);

    LenguinManager::Initialize(pkAM, pkFootprint, pkRightFoot, pkLeftFoot, 
        9);


    NiAVObject* pkEmitter = m_spAM->GetNIFRoot()->
        GetObjectByName("SS-LeftFoot-Emitter");
    assert(pkEmitter);
    spLeftFootEmitter = (NiGeometry*)LevelManager::GetObjectByType(pkEmitter, 
        &NiGeometry::ms_RTTI);
    assert(spLeftFootEmitter);

    pkEmitter = m_spAM->GetNIFRoot()->GetObjectByName("SS-RightFoot-Emitter");
    assert(pkEmitter);
    spRightFootEmitter = (NiGeometry*)LevelManager::GetObjectByType(pkEmitter, 
    &NiGeometry::ms_RTTI);
    assert(spRightFootEmitter);

    pkEmitter = m_spAM->GetNIFRoot()->GetObjectByName("SS-Fleas");
    assert(pkEmitter);
    spFleeEmitter = (NiGeometry*)LevelManager::GetObjectByType(pkEmitter, 
        &NiGeometry::ms_RTTI);
    assert(spFleeEmitter);

    LevelManager::RecursivePrepack(spLeftFootEmitter, false);
    LevelManager::RecursivePrepack(spRightFootEmitter, false);
    LevelManager::RecursivePrepack(spFleeEmitter, false);


    // Create the task to update Momma's animations -- because this is a 
    // persistent TYPE_FRAME task, MommaManager doesn't need a reference
    // to it.  It is owned (and destroyed) by the Scheduler as necessary
    MommaUpdateAnimationsTask* pkUpdateAnimsTask = 
        NiNew MommaUpdateAnimationsTask();

    if (!pkUpdateAnimsTask)
        return false;

    Scheduler::Get()->Schedule(pkUpdateAnimsTask);

    return true;
}
//---------------------------------------------------------------------------
void MommaManager::InitializePhysics()
{
    m_bPhysicsControl = false;
    
    // Create a kinematic actor for Momma
    NiPhysXScene* pkScene = LevelManager::Get()->GetPhysXScene();
    NxScene* pkNxScene = pkScene->GetPhysXScene();
    NiPhysXManager* pkPhysXManager = NiPhysXManager::GetPhysXManager();
    
    NxCapsuleShapeDesc kCapsuleShape;
    kCapsuleShape.radius = 33.0f;
    kCapsuleShape.height = 45.0f;
    kCapsuleShape.localPose.t.z =
        kCapsuleShape.radius + 0.5f * kCapsuleShape.height // midpt wrt root
        - 41.0f; // hip wrt root
    NiQuaternion kLocalRot(NI_PI * 0.5f, NiPoint3::UNIT_X);
    NiPhysXTypes::NiQuaternionToNxMat33(kLocalRot, kCapsuleShape.localPose.M);

    NxCapsuleShapeDesc kTriggerShape;
    kTriggerShape.radius = 35.0f;
    kTriggerShape.height = 47.0f;
    kTriggerShape.localPose.t.z =
        kTriggerShape.radius + 0.5f * kTriggerShape.height // midpt wrt root
        - 41.0f; // hip wrt root
    NiPhysXTypes::NiQuaternionToNxMat33(kLocalRot, kTriggerShape.localPose.M);
    kTriggerShape.shapeFlags |= NX_TRIGGER_ON_ENTER;        
    kTriggerShape.shapeFlags |= NX_TRIGGER_ON_LEAVE;        
    
    NxBodyDesc kBodyDesc;
    
    NxActorDesc kActorDesc;
    kActorDesc.body = &kBodyDesc;
    kActorDesc.density = 1.0f;
    kActorDesc.shapes.pushBack(&kCapsuleShape);
    kActorDesc.shapes.pushBack(&kTriggerShape);
    kActorDesc.userData = NiNew PhysXUserData(PhysXUserData::OBJECTTYPE_MOMMA,
        (void*)this);
    
    pkPhysXManager->WaitSDKLock();
    m_pkPhysXActor = pkNxScene->createActor(kActorDesc);
    m_pkPhysXActor->raiseBodyFlag(NX_BF_KINEMATIC);
    pkPhysXManager->ReleaseSDKLock();
    
    NiAVObject* pkHip =
        ((NiNode*)m_spAM->GetNIFRoot())->GetObjectByName("BoxBone-hip");
    assert(pkHip);
    NiPhysXKinematicSrc* pkSrc =
        NiNew NiPhysXKinematicSrc(pkHip, m_pkPhysXActor);
    pkSrc->SetActive(true);
    pkSrc->SetInterpolate(true);
    pkScene->AddSource(pkSrc);
}
//---------------------------------------------------------------------------
void MommaManager::Update(float fTime, float fFrameTime, const NiPoint2& kDir,
    const float& fStrafeDir, bool bAnimationUpdate)
{
  // m_pkDynaQuad->ResetQuads();
 
    if (kDir.SqrLength() > m_sfZeroVelocityMaximum)
        m_fLastInputTime = fTime;

    NiPoint2 kMommaDir;
#if defined(AUTODRIVEMOMMA)
    if (fTime > m_fLastInputTime + m_fAutoModeTime)
    {
        // auto drive
        // turn off rumble
        m_bRumble = false;
        kMommaDir = AutoDir(fTime);
    }
    else
#endif
    {
        m_bRumble = true;
        kMommaDir = kDir;
    }

    LenguinManager::Update(fTime, fFrameTime, kMommaDir, fStrafeDir, 
        bAnimationUpdate);
}
//---------------------------------------------------------------------------
NiActorManager::EventCode MommaManager::InterpretInput(const NiPoint2& kDir,
    const float& fStrafeDir, float fTime)
{
    float fLength = kDir.SqrLength();

    if (m_bTriggerJump)
    {
        NiActorManager::SequenceID eSequence = m_spAM->GetTargetAnimation();

        if (fLength < m_sfZeroVelocityMaximum && fStrafeDir == 0)
            m_bIsIdling = true;
        else
            m_bIsIdling = false;

        //Make sure if Momma is already jumping that we continue to use the 
        //same jump animation. If Momma is not already jumping then select
        //the jump animation based on her movement direction.
        if (eSequence == LM_Anim::JUMPLEFT ||
            eSequence == LM_Anim::JUMPRIGHT ||
            eSequence == LM_Anim::JUMP01)
        {
            return eSequence;
        }
        else
        {
            if (fStrafeDir < 0)
                return LM_Anim::JUMPRIGHT;
            else if (fStrafeDir > 0)
                return LM_Anim::JUMPLEFT;
            else
                return LM_Anim::JUMP01;
        }

    }

    if (m_bTriggerIceBreak)
    {
        return LM_Anim::ICEBREAK01;
    }

    NiActorManager::EventCode eMove;

    if (fLength < m_sfZeroVelocityMaximum && fStrafeDir == 0)
    {
        // No significant movement
        float fEndTime = m_spAM->GetNextEventTime(
            NiActorManager::TEXT_KEY_EVENT, m_spAM->GetTargetAnimation(), 
            "end");
        if (fEndTime < fTime || !m_bIsIdling)
        {
            // done with current idle loop, or not yet idling at all
            float fRandomIdle = NiUnitRandom() * m_kRandomIdle.GetSize();
            if (fRandomIdle == (float)m_kRandomIdle.GetSize())
                fRandomIdle = (float)(m_kRandomIdle.GetSize() - 1);

            unsigned int uiWhichIdle = (unsigned int)fRandomIdle;
            assert(uiWhichIdle < m_kRandomIdle.GetSize());

            eMove = m_kRandomIdle.GetAt(uiWhichIdle);
        }
        else
        {
            // already idling, but not yet done with current loop
            eMove = m_spAM->GetTargetAnimation();
        }
        m_bIsIdling = true;
    }
    else
    {
        if (fStrafeDir < 0)
        {
            eMove = LM_Anim::STRAIF_RIGHT;            
        }
        else if (fStrafeDir > 0)
        {
            eMove = LM_Anim::STRAIF_LEFT;            
        }
        else
        {
            if (fLength < 0.45f)
            {
                eMove = LM_Anim::WALKFORWARD;
            }
            else
            {
                eMove = LM_Anim::RUN_V2;
            }
        }
        m_bIsIdling = false;
    }

    return eMove;

}
//---------------------------------------------------------------------------
void MommaManager::InitializeRandomIdle()
{
    m_kRandomIdle.SetSize(10);
    m_kRandomIdle.SetAt(0, LM_Anim::IDLE01BASE);
    m_kRandomIdle.SetAt(1, LM_Anim::IDLE01BASE);
    m_kRandomIdle.SetAt(2, LM_Anim::IDLE01BASE);
    m_kRandomIdle.SetAt(3, LM_Anim::IDLE01BASE);
    m_kRandomIdle.SetAt(4, LM_Anim::IDLE01BASE);
    m_kRandomIdle.SetAt(5, LM_Anim::IDLE01BASE);
    m_kRandomIdle.SetAt(6, LM_Anim::IDLE02LOOKBACK);
    m_kRandomIdle.SetAt(7, LM_Anim::IDLE03SHIVER);
    m_kRandomIdle.SetAt(8, LM_Anim::IDLE04ITCH);
    m_kRandomIdle.SetAt(9, LM_Anim::IDLE05TURNAROUND);
}
//---------------------------------------------------------------------------
void MommaManager::HandleTextKeyEvent()
{
}
//---------------------------------------------------------------------------
void MommaManager::AttractBabies()
{
//    LevelManager::GetFlockManager()->AddAttractor(m_spAM->GetNIFRoot());
}
//---------------------------------------------------------------------------
const NiPoint2 MommaManager::AutoDir(float fTime)
{
    bool bTimeExpired = 
        (fTime - m_fPausedTime > m_fMaxAutoGoalSeekTime);
#ifdef _DEBUG
    if (bTimeExpired)
        NiOutputDebugString("AutoDir time expired, changing goals...\n");
#endif

    if ((m_kAutoGoal - Get2DLenguinTranslate()).Length() < 1000.0f ||
        bTimeExpired)
    {
        // pause for a moment between going from goal to goal
        if (fTime - m_fPausedTime < m_fPauseTime)
        {
            return NiPoint2::ZERO;
        }
        else
        {
            m_fPausedTime = fTime;
        }

        ComputeAutoGoal();
        float fHeight;
        NiColorA kColor;

        while (LevelManager::GetTerrainManager()->GetHeightAndColor(
            m_kAutoGoal.x, m_kAutoGoal.y, fHeight, kColor) &&
            !ColorIsWalkable(kColor))
        {
            ComputeAutoGoal();
        }
        m_bAutoWalk = (NiUnitRandom() < 0.5f);
    }

    NiPoint2 kAutoDir = m_kAutoGoal - Get2DLenguinTranslate();
    kAutoDir.Unitize();
    if (m_bAutoWalk)
        kAutoDir *= 0.4f;
    return kAutoDir;
}
//---------------------------------------------------------------------------
void MommaManager::ComputeAutoGoal()
{
    float fMaxX = LevelManager::GetTerrainManager()->GetMaxX();
    float fMaxY = LevelManager::GetTerrainManager()->GetMaxY();
    float fMinX = LevelManager::GetTerrainManager()->GetMinX();
    float fMinY = LevelManager::GetTerrainManager()->GetMinY();

    m_kAutoGoal.x = (NiUnitRandom() * (fMaxX - fMinX)) + fMinX;
    m_kAutoGoal.y = (NiUnitRandom() * (fMaxY - fMinY)) + fMinY;
}
//---------------------------------------------------------------------------
void MommaManager::AnimActivated(NiActorManager* pkManager,
    NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
    float fEventTime)
{
    switch (eSequenceID)
    {
    case LM_Anim::JUMP01:
        LevelManager::Get()->GetSoundManager()->PlayMommaJump();
        break;
    case LM_Anim::JUMPLEFT:
        LevelManager::Get()->GetSoundManager()->PlayMommaJumpLeft();
        break;
    case LM_Anim::JUMPRIGHT:
        LevelManager::Get()->GetSoundManager()->PlayMommaJumpRight();
        break;
    }
}
//---------------------------------------------------------------------------
void MommaManager::EndOfSequence(NiActorManager *pkManager, 
                   NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
                   float fEventTime)
{
    if ( eSequenceID == LM_Anim::JUMP01 ||
         eSequenceID == LM_Anim::JUMPLEFT ||
         eSequenceID == LM_Anim::JUMPRIGHT)
    {
        m_bTriggerJump = false;
    }
    else if (eSequenceID == LM_Anim::ICEBREAK01)
    {
        m_bTriggerIceBreak = false;
    }

    m_bExcutingAction = false;

    if (m_bIsIdling)
        m_spAM->SetTargetAnimation( m_kRandomIdle.GetAt(0) ); 
    else
        m_spAM->SetTargetAnimation( m_ePostActionAnimation ); 
}
//---------------------------------------------------------------------------
void MommaManager::DrawParticles(NiCamera* pkCamera)
{
    LevelManager::Get()->PartialRender(pkCamera, spLeftFootEmitter);
    LevelManager::Get()->PartialRender(pkCamera, spRightFootEmitter);
    LevelManager::Get()->PartialRender(pkCamera, spFleeEmitter);   
}
//---------------------------------------------------------------------------
void MommaManager::Restart()
{
    m_spAM->GetNIFRoot()->SetTranslate(m_kSpawn);
    m_spAM->GetNIFRoot()->SetRotate( NiQuaternion(0,0,0,0) );
    m_kBipQuat = NiQuaternion(0,0,0,0);
    m_kLookAtQuat = NiQuaternion(0,0,0,0);
}
//---------------------------------------------------------------------------


