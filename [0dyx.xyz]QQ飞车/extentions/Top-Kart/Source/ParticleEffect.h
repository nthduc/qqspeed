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

#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include "NiSystem.h"
#include "NiNode.h"
#include "NifAnimationInfo.h"

class NiCamera;

class ParticleEffect : public NiMemObject
{
public:
    enum EffectType
    {
        NILL_EFFECT = 0,
		SNOW_EFFECT,
		ACCLINE_EFFECT,
        MAX_EFFECT_TYPES
    };

    ParticleEffect();
    virtual ~ParticleEffect();

    void Initilize(EffectType eType, NiNodePtr spParticle);
    void Restart(NiPoint3* pkLocation);

	void RePos(const NiPoint3& pkLocation);

	CNifAnimationInfo* GetAnimInfo(){return &m_nInfo;};

    bool IsActive();
    EffectType GetType();

    virtual void Update(float fTime);
    virtual void Draw(NiCamera* pkCamera);

private:
    EffectType m_eType;
    NiNodePtr m_spParticle;
    float m_fTime;
	CNifAnimationInfo  m_nInfo;
};

#include "ParticleEffect.inl"

#endif // PARTICLEEFFECT_H
