/********************************************************************
	作者:	　wincoqin
	时间:	  2007/04/13
    文件名:   BaseTerrainEffect.cpp
	说明：     []
*********************************************************************/
// precompile header
#include "Top-KartPCH.h"
// libfile
// userfile
#include "BaseTerrainEffect.h"
#include "gpscript.h"
#include "GPJetAffect.h"
#include "GPNo2Affect.h"

#pragma warning( disable : 4800)
#pragma warning( disable : 4805)

typedef enum enm_TerrainPropEffectType
{
	ETPET_INVALID			= 0,
	ETPET_EFFECT			= 1,
	ETPET_EFFECT_OTHER		= 2,
	ETPET_SOUND				= 3,
	ETPET_ACCEL				= 4,
	ETPET_JUMP				= 5,
	ETPET_TRANSFORM			= 6,
	ETPET_ACCELSPEED		= 7,
	ETPET_EFFECT_NUM,
}ENM_TPETYPE;


CTerrainSoundEffect::CTerrainSoundEffect()
: CBaseTerrainPropEffect(ETPET_SOUND)
{

}


ITerrainPropEffect * CBaseTerrainPropEffect::CreateTerrainPropEffect(int nTerrainPropEffectType, 
											 TerrainPropEffect * pTerrainPropEffect)
{
	ITerrainPropEffect * pkTerrainPropEffect = NULL;
	switch(nTerrainPropEffectType) 
	{
	case ETPET_EFFECT:
		pkTerrainPropEffect = new CTerrainAnimationEffect();
		break;
	case ETPET_EFFECT_OTHER:
		pkTerrainPropEffect = new CTerrainSoundEffect();
		break;
	case ETPET_SOUND:
		pkTerrainPropEffect = new CTerrainSoundEffect();
		break;
	case ETPET_ACCEL:
		pkTerrainPropEffect = new CTerrainAccelEffect();
		break;
	case ETPET_JUMP:
		pkTerrainPropEffect = new CTerrainAccelEffect();
		break;
	case ETPET_TRANSFORM:
		pkTerrainPropEffect = new CTerrainTransformEffect();
		break;
	case ETPET_ACCELSPEED:
		pkTerrainPropEffect = new CTerrainAccelSpeedEffect();
		break;
	default:
		assert(0);
	}

	if (!pkTerrainPropEffect || !pkTerrainPropEffect->InitPropEffect(pTerrainPropEffect) ) 
	{
		SAFE_DELETE(pkTerrainPropEffect);
	}

	return pkTerrainPropEffect;
}

bool CTerrainSoundEffect::InitPropEffect(TerrainPropEffect * pTerrainPropEffect)
{
	
	bool bSuccess = true;
	if (pTerrainPropEffect)
	{
		assert(pTerrainPropEffect->m_nTerrainPropEffectType == m_nTerrainEffectType);
		bSuccess &= _ReadNumFromVar(&m_nSoundID, pTerrainPropEffect->m_pkPropVarTable->Element("param0"));
		bSuccess &= _ReadNumFromVar(&m_nCycleType, pTerrainPropEffect->m_pkPropVarTable->Element("param1"));
	}

	return bSuccess;
}

void CTerrainSoundEffect::Update(float fDeltaTime)
{

}
void CTerrainSoundEffect::StartTerrainEffect()
{
	CBaseTerrainPropEffect::StartTerrainEffect();
}
void CTerrainSoundEffect::StopTerrainEffect()
{
	CBaseTerrainPropEffect::StopTerrainEffect();
}


CTerrainAnimationEffect::CTerrainAnimationEffect()
: CBaseTerrainPropEffect(ETPET_EFFECT)
{
}

bool CTerrainAnimationEffect::InitPropEffect(TerrainPropEffect * pTerrainPropEffect)
{

	bool bSuccess = true;
	if (pTerrainPropEffect)
	{
		assert(pTerrainPropEffect->m_nTerrainPropEffectType == m_nTerrainEffectType);
		bSuccess &= _ReadNumFromVar(&m_nAnimationPos, pTerrainPropEffect->m_pkPropVarTable->Element("param0"));
		std::string sString;
		bSuccess &= _ReadStringFromVar(sString, pTerrainPropEffect->m_pkPropVarTable->Element("param1"));
		m_kAnimation = sString.data();
		bSuccess &= _ReadNumFromVar(&m_nCycleType, pTerrainPropEffect->m_pkPropVarTable->Element("param2"));
	}
	m_bStart = false;
	NiStream kStream;

	if (bSuccess &= kStream.Load(NiApplication::ConvertMediaFilename(m_kAnimation)))
	{
		m_spEffect = (NiAVObject*)kStream.GetObjectAt(0);
		m_spEffect->UpdateNodeBound();
		m_spEffect->UpdateProperties();
		m_spEffect->UpdateEffects();
		m_spEffect->Update(0.0f);
	}	

	return bSuccess;
}
void CTerrainAnimationEffect::Update(float fDeltaTime)
{
	CTencentCar * pCar = NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle();
	if (pCar && NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * GameSetting::SpeedDispCoef > 20.0f
		&& pCar->GetContactStatus() != CTencentCar::ECS_NONECONTACT && pCar->GetContactStatus() != CTencentCar::ECS_INAIR)
	{
		if (!m_bStart)
			StartTerrainEffect();
	}
	else
	{
		if (m_bStart)
			StopTerrainEffect();
	}
}
void CTerrainAnimationEffect::StartTerrainEffect()
{
	CBaseTerrainPropEffect::StartTerrainEffect();
	if (m_spEffect)
		NxPlayMgr::GetHostPlay()->AttachDummy("dummy_back", NiDynamicCast(NiNode, m_spEffect));
}
void CTerrainAnimationEffect::StopTerrainEffect()
{
	CBaseTerrainPropEffect::StopTerrainEffect();
	if (m_spEffect)
		NxPlayMgr::GetHostPlay()->DetachDummy("dummy_back", NiDynamicCast(NiNode, m_spEffect));
}


CTerrainAccelEffect::CTerrainAccelEffect()
: CBaseTerrainPropEffect(ETPET_EFFECT_OTHER)
, m_pJetAffect(NULL)
{

}

bool CTerrainAccelEffect::InitPropEffect(TerrainPropEffect * pTerrainPropEffect)
{

	bool bSuccess = true;
	if (pTerrainPropEffect)
	{
		assert(pTerrainPropEffect->m_nTerrainPropEffectType == m_nTerrainEffectType);
		bSuccess &= _ReadNumFromVar(&m_fXForce, pTerrainPropEffect->m_pkPropVarTable->Element("param0"));
		bSuccess &= _ReadNumFromVar(&m_fForceTime, pTerrainPropEffect->m_pkPropVarTable->Element("param1"));		
	}

	return bSuccess;
}

void CTerrainAccelEffect::Update(float fDeltaTime)
{
	
	if ( m_fForceTime >= 0.1f && GetMainApp()->GetAccumTime() - m_fStartTime >= m_fForceTime)
		return;

	Play * pPlay = NxPlayMgr::GetHostPlay();

	if (pPlay) 
	{
		float fForce[3];
		fForce[0] = m_fXForce;
		NxVec3 kVecLocal = pPlay->GetKart()->GetVelocityLocal();
		CLinks * pLinks = CLevelManager::Get()->GetLinks();
		assert(pLinks);
		/*CCheckPoint * pChkPt = pLinks->GetLastRightCheckPoint(false);
		// 路面朝向
		NiPoint3 kDir = CKartStatusManager::GetRightDir(pChkPt, pLinks->GetCorrectRunDir());

		NxVec3 kLocal;
		NxVec3 kGlobal(kDir.x, kDir.y, kDir.z);
		pPlay->GetKart()->GetVehicle()->GlobalToLocal(kGlobal, kLocal);
		kLocal *= m_fXForce;
		fForce[0] = kLocal.x;
		fForce[1] = kLocal.y;
		fForce[2] = kLocal.z;
		*/
		//if (kVecLocal.x * GameSetting::SpeedDispCoef > 1.0f)
		{
			if (!m_pNo2Affect->IsPlaying()) 
			{
				m_pNo2Affect->Start();
				CCameraBase * pkCamera = NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_RACE);
				pkCamera->SetForceSpeedUp(true);
			}
			pPlay->AddForce(fForce, true);
			pPlay->PlayAnimation(1306, TRUE);
		}
	/*	else
		{
			if (m_pNo2Affect->IsPlaying()) 
			{
				m_pNo2Affect->Stop();
				CCameraBase * pkCamera = NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_RACE);
				pkCamera->SetForceSpeedUp(false);
			}
		}*/
	}	
}
void CTerrainAccelEffect::StartTerrainEffect()
{
	/*
	m_pJetAffect = new CGPJectAffect();
	m_pJetAffect->SetTarget(NULL, NxPlayMgr::GetHostPlay());
	m_pJetAffect->Start();*/
	m_fStartTime = GetMainApp()->GetAccumTime();

	m_pNo2Affect = new CGPNo2Affect;
	m_pNo2Affect->SetTarget(NULL, NxPlayMgr::GetHostPlay());
	if (m_fForceTime <= 0.000001)
		m_pNo2Affect->SetLifeTime(1000000);
	else
		m_pNo2Affect->SetLifeTime(m_fForceTime);

	CCameraBase * pkCamera = NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_RACE);
	pkCamera->SetForceSpeedUp(true);
	
	m_pNo2Affect->Start();
	CBaseTerrainPropEffect::StartTerrainEffect();
}
void CTerrainAccelEffect::StopTerrainEffect()
{
	/*m_pJetAffect->Stop();
	SAFE_DELETE(m_pJetAffect);*/
	m_pNo2Affect->Stop();
	CCameraBase * pkCamera = NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_RACE);
	pkCamera->SetForceSpeedUp(false);
	SAFE_DELETE(m_pNo2Affect);
	CBaseTerrainPropEffect::StopTerrainEffect();
}


CTerrainJumpEffect::CTerrainJumpEffect()
: CBaseTerrainPropEffect(ETPET_JUMP)
{

}

bool CTerrainJumpEffect::InitPropEffect(TerrainPropEffect * pTerrainPropEffect)
{

	bool bSuccess = true;
	if (pTerrainPropEffect)
	{
		assert(pTerrainPropEffect->m_nTerrainPropEffectType == m_nTerrainEffectType);
		bSuccess &= _ReadNumFromVar(&m_kDeltaVelocity.x, pTerrainPropEffect->m_pkPropVarTable->Element("param0"));
		bSuccess &= _ReadNumFromVar(&m_kDeltaVelocity.y, pTerrainPropEffect->m_pkPropVarTable->Element("param1"));
		bSuccess &= _ReadNumFromVar(&m_kDeltaVelocity.z, pTerrainPropEffect->m_pkPropVarTable->Element("param2"));
	}
	Play * pPlay = NxPlayMgr::GetHostPlay();
	if ( pPlay && pPlay->GetKart() && pPlay->GetKart()->GetVehicle())
	{
		pPlay->GetKart()->GetVehicle()->TirggerJumpMotion();
	}
	return bSuccess;
}

CTerrainTransformEffect::CTerrainTransformEffect()
: CBaseTerrainPropEffect(ETPET_TRANSFORM)
{

}

bool CTerrainTransformEffect::InitPropEffect(TerrainPropEffect * pTerrainPropEffect)
{

	bool bSuccess = true;
	if (pTerrainPropEffect)
	{
		assert(pTerrainPropEffect->m_nTerrainPropEffectType == m_nTerrainEffectType);
		std::string kString;
		bSuccess &= _ReadStringFromVar(kString, pTerrainPropEffect->m_pkPropVarTable->Element("param0"));
		m_kTransformAimedName = kString.data();
		bSuccess &= _ReadStringFromVar(kString, pTerrainPropEffect->m_pkPropVarTable->Element("param1"));
		m_kAnimationName      = kString.data();
		bSuccess &= _ReadNumFromVar(&m_fAnimationTime, pTerrainPropEffect->m_pkPropVarTable->Element("param2"));
	}

	return bSuccess;
}

void CTerrainTransformEffect::Update(float fDeltaTime)
{
	
}
void CTerrainTransformEffect::StartTerrainEffect()
{
	CBaseTerrainPropEffect::StartTerrainEffect();
}
void CTerrainTransformEffect::StopTerrainEffect()
{
	CBaseTerrainPropEffect::StopTerrainEffect();
}

//---------------------------------------------------------------------------------------------------
CTerrainAccelSpeedEffect::CTerrainAccelSpeedEffect()
:CBaseTerrainPropEffect(ETPET_ACCELSPEED)
{
	m_bIsDriftModifyGravity = false;
	m_fModifyRate = 1.0f;
}

void CTerrainAccelSpeedEffect::Update(float fDeltaTime)
{
	CTencentCar * pCar = NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle();

	if (!m_bIsDriftModifyGravity && pCar && pCar->IsDrifting())
	{
		m_bIsDriftModifyGravity = true;
		m_fOldGravityValue = pCar->GetGravityValue();
		pCar->SetGravityValue(m_fOldGravityValue * m_fModifyRate);
	}
}

bool CTerrainAccelSpeedEffect::InitPropEffect(TerrainPropEffect * pTerrainPropEffect)
{
	bool bSuccess = true;
	if (pTerrainPropEffect)
	{
		assert(pTerrainPropEffect->m_nTerrainPropEffectType == m_nTerrainEffectType);
		bSuccess &= _ReadNumFromVar(&m_fModifyRate, pTerrainPropEffect->m_pkPropVarTable->Element("param0"));
	}

	return bSuccess;
}

void CTerrainAccelSpeedEffect::StartTerrainEffect()
{
	CTencentCar * pCar = NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle();

	if (pCar && pCar->IsDrifting())
	{
		m_bIsDriftModifyGravity = true;
		m_fOldGravityValue = pCar->GetGravityValue();
		pCar->SetGravityValue(m_fOldGravityValue * m_fModifyRate);
	}
}

void CTerrainAccelSpeedEffect::StopTerrainEffect()
{
	CTencentCar * pCar = NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle();

	if (m_bIsDriftModifyGravity && pCar)
	{
		pCar->SetGravityValue(m_fOldGravityValue);
	}
}

