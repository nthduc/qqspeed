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

#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include "ParticleEffect.h"
class NiCamera;

class EffectManager : public NiMemObject
{
public:
    EffectManager();
    ~EffectManager();

    void Initialize();
    void Update(float fTime);
    
	ParticleEffect* AddEffect(ParticleEffect::EffectType eType, NiPoint3* pkLocation);
	void DelEffect(ParticleEffect* pEffect);

    void Draw(NiCamera* pkCamera);

private:
    
    NiNodePtr m_spBaseParticles[ParticleEffect::MAX_EFFECT_TYPES];
    NiTPointerList<ParticleEffect*> m_kActiveParticles;
    NiTPointerList<ParticleEffect*> 
        m_kInActiveParticles[ParticleEffect::MAX_EFFECT_TYPES];

};

#endif // EFFECTMANAGER_H
