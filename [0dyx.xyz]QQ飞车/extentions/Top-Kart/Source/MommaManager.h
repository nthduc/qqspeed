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

#ifndef MOMMAMANAGER_H
#define MOMMAMANAGER_H

#include "LenguinManager.h"

class MommaUpdateAnimationsTask;

class MommaManager : public LenguinManager
{

public:
    MommaManager();
    ~MommaManager();

    bool Initialize(const NiPoint3& kSpawn, unsigned int uiMommaID,
        unsigned int uiPadID);
    void Update(float fTime, float fFrameTime, const NiPoint2& kDir, 
        const float& fStrafeDir, bool bAnimationUpdate);

    void AttractBabies();
    void DrawFootprints(NiCamera* pkCamera);
    void DrawParticles(NiCamera* pkCamera);
    void Jump();
    void BreakIceAttack();
    bool IsExcutingAction();
    void Restart();
    NiPoint3 GetSpawnPoint();

    void SetAutoModeTime(float fTime);
    float GetAutoModeTime() const;

    // These next two indices are not the same
    // 
    // The array index in the array of Mommas
    unsigned int GetMommaID() const;
    // The pad index in the array of 
    unsigned int GetPadID() const;

    // NiActorManager::CallbackObject overrides
    virtual void AnimActivated(NiActorManager* pkManager,
        NiActorManager::SequenceID eSequenceID, float fCurrentTime,
        float fEventTime);

    virtual void EndOfSequence(NiActorManager *pkManager, 
        NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
        float fEventTime);


protected:
    virtual void HandleTextKeyEvent();
    virtual NiActorManager::EventCode InterpretInput(const NiPoint2& kDir,
        const float& fStrafeDir, float fTime);
    virtual void InitializeRandomIdle();
    virtual void InitializePhysics();
    const NiPoint2 AutoDir(float fTime);
    void ComputeAutoGoal();

    NiPoint3 m_kSpawn;

    unsigned int m_uiMommaID;
    unsigned int m_uiPadID;
    bool m_bRumble;
    bool m_bTriggerJump;
    bool m_bTriggerIceBreak;
    bool m_bExcutingAction;

    NiActorManager::SequenceID m_ePostActionAnimation;

    //Particle Emmitters
    NiGeometryPtr   spLeftFootEmitter;
    NiGeometryPtr   spRightFootEmitter;
    NiGeometryPtr   spFleeEmitter;

    // auto mode
    float m_fLastInputTime;
    float m_fAutoModeTime;
    NiPoint2 m_kAutoGoal;
    float m_fPauseTime;
    float m_fPausedTime;
    bool m_bAutoWalk;
    float m_fMaxAutoGoalSeekTime;
};

#include "MommaManager.inl"    

#endif // #ifndef MOMMAMANAGER_H
