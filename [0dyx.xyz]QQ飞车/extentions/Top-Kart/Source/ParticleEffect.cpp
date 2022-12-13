

#include "Top-KartPCH.h"

#include "ParticleEffect.h"
//#include "LevelManager.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"

//---------------------------------------------------------------------------
ParticleEffect::ParticleEffect() : m_fTime(0)
{
    
}

//---------------------------------------------------------------------------
ParticleEffect::~ParticleEffect()
{
    m_spParticle = 0;
}

//---------------------------------------------------------------------------
void ParticleEffect::Initilize(EffectType eType, NiNodePtr spParticle)
{
    m_eType = eType;
    m_spParticle = spParticle;
	m_nInfo.CollectData(spParticle);

	NiTimeController::StartAnimations(spParticle);
	Update(0.0f);
}

//---------------------------------------------------------------------------
void ParticleEffect::Restart(NiPoint3* pkLocation)
{
	if(pkLocation){ m_spParticle->SetTranslate(*pkLocation); }
    m_spParticle->Update(0);
    m_fTime = 0;
}


//--------------------------------------------------------------------------

void ParticleEffect::RePos(const NiPoint3& pkLocation)
{
	m_spParticle->SetTranslate(pkLocation);
}


//---------------------------------------------------------------------------
void ParticleEffect::Update(float fTime)
{
    m_fTime = 1.0f / 30.0f;
    m_spParticle->Update(GetMainApp()->GetCurrentTime());
}
//---------------------------------------------------------------------------
void ParticleEffect::Draw(NiCamera* pkCamera)
{
	NiDrawScene(pkCamera, m_spParticle, GetMainApp()->GetDefaultCuller());
//	if(NxSceneMgr::Get()->GetActiveSceneLevel())
//		NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(pkCamera,m_spParticle);
}
//---------------------------------------------------------------------------
