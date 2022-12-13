

#include "Top-KartPCH.h"

#include "EffectManager.h"
//#include "LevelManager.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "RMOperator.h"
//---------------------------------------------------------------------------
EffectManager::EffectManager()
{
    /* ... */
}
//---------------------------------------------------------------------------
EffectManager::~EffectManager()
{
    // Clean out Active and In-Active Lists
    NiTListIterator kPos = m_kActiveParticles.GetHeadPos();
    while (kPos)
    {
        ParticleEffect* pkPE = m_kActiveParticles.GetNext(kPos);
        NiDelete pkPE;
    }        

    for(unsigned int ui = 0; ui < ParticleEffect::MAX_EFFECT_TYPES; ui++)
    {
        NiTListIterator kPos = m_kInActiveParticles[ui].GetHeadPos();
        while (kPos)
        {
            ParticleEffect* pkPE = m_kInActiveParticles[ui].GetNext(kPos);
            NiDelete pkPE;
        }        

        m_spBaseParticles[ui] = 0;
    }

}
//---------------------------------------------------------------------------
void EffectManager::Initialize()
{
    // Load Base Particles

	//Example For Add A Effect~----------------------------------------
    m_spBaseParticles[ParticleEffect::SNOW_EFFECT] = 
        RMOperator::OpenNifAndPrepack("effect/SnowSplash.nif");

	/*
	NxSceneMgr::Get()->GetActiveSceneLevel()->LightObject(
    m_spBaseParticles[ParticleEffect::SNOW_EFFECT]);
    m_spBaseParticles[ParticleEffect::SNOW_EFFECT]->UpdateProperties();
    m_spBaseParticles[ParticleEffect::SNOW_EFFECT]->UpdateEffects();
    m_spBaseParticles[ParticleEffect::SNOW_EFFECT]->Update(0);
	*/

	m_spBaseParticles[ParticleEffect::ACCLINE_EFFECT] = 
		RMOperator::OpenNifAndPrepack("effect/acc_line.nif");

	/*
	NxSceneMgr::Get()->GetActiveSceneLevel()->LightObject(
		m_spBaseParticles[ParticleEffect::ACCLINE_EFFECT]);
	m_spBaseParticles[ParticleEffect::ACCLINE_EFFECT]->UpdateProperties();
	m_spBaseParticles[ParticleEffect::ACCLINE_EFFECT]->UpdateEffects();
	m_spBaseParticles[ParticleEffect::ACCLINE_EFFECT]->Update(0);
	*/

    //Here We also can  Pre-Cache particles effects for future use
	ParticleEffect* pkEffect;
    ParticleEffect::EffectType eType;
    NiNode* pkClone;
    for(int ui = 0; ui < 3; ui++)
    {
        // Add Snow Effect
        pkEffect = NiNew ParticleEffect();
        eType = ParticleEffect::SNOW_EFFECT;
        pkClone = (NiNode*)m_spBaseParticles[eType]->Clone();
        pkClone->UpdateEffects();
        pkClone->UpdateProperties();
        pkClone->Update(0);

        pkEffect->Initilize(eType, pkClone);
        m_kInActiveParticles[eType].AddTail(pkEffect);



		pkEffect = NiNew ParticleEffect();
		eType = ParticleEffect::ACCLINE_EFFECT;
		pkClone = (NiNode*)m_spBaseParticles[eType]->Clone();
		pkClone->UpdateEffects();
		pkClone->UpdateProperties();
		pkClone->Update(0);

		pkEffect->Initilize(eType, pkClone);
		m_kInActiveParticles[eType].AddTail(pkEffect);
    }
 
}
//---------------------------------------------------------------------------
void EffectManager::Update(float fTime)
{
    NiTListIterator kPos = m_kActiveParticles.GetHeadPos();
    while (kPos)
    {
        ParticleEffect* pkPE = m_kActiveParticles.GetNext(kPos);
        ParticleEffect::EffectType eType = pkPE->GetType();

        if (!pkPE->IsActive())
        {
            // Particle In-Active, so remove from active list and add to 
            // In-Active list
            m_kActiveParticles.Remove(pkPE);
            m_kInActiveParticles[eType].AddTail(pkPE);

        }
        else
        {
            pkPE->Update(fTime);
        }
    }        
}
//---------------------------------------------------------------------------
ParticleEffect* EffectManager::AddEffect(ParticleEffect::EffectType eType,
                              NiPoint3* pkLocation)
{
    ParticleEffect* pkPE;

    // Pull Effect from In-Active list if available
    if (m_kInActiveParticles[eType].GetSize() > 0)
    {
        pkPE = m_kInActiveParticles[eType].RemoveHead();
    }
    else
    {
        // In-Active list is empty so manually create a new effect
        pkPE = NiNew ParticleEffect();
        
        NiNode* pkClone = (NiNode*)m_spBaseParticles[eType]->Clone();
        pkClone->UpdateEffects();
        pkClone->UpdateProperties();
        pkClone->Update(0);

        pkPE->Initilize(eType, pkClone);
    }

    pkPE->Restart(pkLocation);
    m_kActiveParticles.AddTail(pkPE);

	return pkPE;
}

//---------------------------------------------------------------------------
void EffectManager::Draw(NiCamera* pkCamera)
{
    NiTListIterator kPos = m_kActiveParticles.GetHeadPos();
    while (kPos)
    {
        ParticleEffect* pkPE = m_kActiveParticles.GetNext(kPos);
        pkPE->Draw(pkCamera);
    }        

}
//---------------------------------------------------------------------------
void EffectManager::DelEffect(ParticleEffect* pEffect)
{
}