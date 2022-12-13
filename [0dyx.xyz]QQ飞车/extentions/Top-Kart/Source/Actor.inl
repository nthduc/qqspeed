

//#include "LevelManager.h"

//---------------------------------------------------------------------------
inline const NiPoint3& Actor::GetLenguinTranslate() const
{
    return m_spKartAM->GetNIFRoot()->GetTranslate();
}
//---------------------------------------------------------------------------
inline const NiPoint2 Actor::Get2DLenguinTranslate() const
{
    return NiPoint2(m_spKartAM->GetNIFRoot()->GetTranslate().x, 
        m_spKartAM->GetNIFRoot()->GetTranslate().y);
}
//---------------------------------------------------------------------------
inline const NiPoint2 Actor::GetLenguinDirection() const
{
    return NiPoint2(m_spKartAM->GetNIFRoot()->GetRotate().GetEntry(0,0),
        m_spKartAM->GetNIFRoot()->GetRotate().GetEntry(0,1));
}
//---------------------------------------------------------------------------
inline void Actor::SetElevation(float fElevation)
{
    m_fElevation = fElevation;
}
//---------------------------------------------------------------------------
inline float Actor::GetMinColliderDistanceSQRD() const
{
    return m_fMinColliderDistanceSQRD;
}
//---------------------------------------------------------------------------
inline float Actor::GetMinColliderDistance() const
{
    return m_fMinColliderDistance;
}
//---------------------------------------------------------------------------
inline void Actor::SetMinColliderDistance(float fDistance)
{
    m_fMinColliderDistance = fDistance;
    m_fMinColliderDistanceSQRD = fDistance * fDistance;
}
//---------------------------------------------------------------------------
inline float Actor::GetAvoidCollisionsFactor() const
{
    return m_fAvoidCollisionsFactor;
}
//---------------------------------------------------------------------------
inline void Actor::SetAvoidCollisionsFactor(float fFactor)
{
    m_fAvoidCollisionsFactor = fFactor;
}
//---------------------------------------------------------------------------
inline NiNode* Actor::GetBoneRoot() const
{
    assert(NiIsKindOf(NiNode, m_spKartAM->GetControllerManager()->GetTarget()));
    return (NiNode*)m_spKartAM->GetControllerManager()->GetTarget();
}
//---------------------------------------------------------------------------
/*
修改人：ray，日期：1/15/2007，人物模型和车模型分离。
得到角色根结点和车根结点
*/
inline NiNode* Actor::GetCharacterNIFRoot() const
{
    assert(NiIsKindOf(NiNode, m_spCharacterAM->GetNIFRoot()));
    return (NiNode*)m_spCharacterAM->GetNIFRoot();
}
//---------------------------------------------------------------------------
inline NiNode* Actor::GetKartNIFRoot() const
{
	assert(NiIsKindOf(NiNode, m_spKartAM->GetNIFRoot()));
	return (NiNode*)m_spKartAM->GetNIFRoot();
}
//---------------------------------------------------------------------------
inline NiNode* Actor::GetSkin() const
{
    return m_spSkin;
}
//---------------------------------------------------------------------------
inline void Actor::SetEyesStripified(bool bStripified)
{
    ms_bEyesStripified = bStripified;
}
//---------------------------------------------------------------------------
inline void Actor::SetSkinStripified(bool bStripified)
{
    ms_bSkinStripified = bStripified;
}
//---------------------------------------------------------------------------
inline bool Actor::ColorIsWalkable(const NiColorA &kColor)
{
    return kColor.r < 0.9f;
}
//---------------------------------------------------------------------------
//inline float Actor::GetSpeedMultiplier()
//{
//    return m_fSpeedMultiplier;
//}
//---------------------------------------------------------------------------
//inline void Actor::SetSpeedMultiplier(float fNewSpeedMultiplier)
//{
//    m_fSpeedMultiplier = fNewSpeedMultiplier;
//}
//---------------------------------------------------------------------------
inline NiActorManager::SequenceID Actor::GetTargetAnimation()
{
    return m_spCharacterAM->GetTargetAnimation();
}
//---------------------------------------------------------------------------
inline void Actor::Damage(float fDamage)
{
    m_fHealth -= fDamage;
}
//---------------------------------------------------------------------------
inline float Actor::GetHealth()
{
    return m_fHealth;
}
//---------------------------------------------------------------------------
inline NiPoint3 Actor::GetLocation()
{
    return m_spKartAM->GetNIFRoot()->GetWorldTranslate();
}
//---------------------------------------------------------------------------
inline void Actor::AttachDummy(NiFixedString name, NiNodePtr spNode)
{
	NiNode * pkHint = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName(name));

	if (!pkHint)
	{
		pkHint = NiDynamicCast(NiNode, m_spCharacterAM->GetNIFRoot()->GetObjectByName(name));
	}

	assert(pkHint);
	if(pkHint)
	{
		//pkHint->SetSelectiveUpdate(true);
		pkHint->SetSelectiveUpdateTransforms(true);
		pkHint->SetSelectiveUpdatePropertyControllers(true);
		pkHint->SetSelectiveUpdateRigid(true);
		pkHint->AttachChild(spNode);
		pkHint->UpdateNodeBound();

		NiTimeController::StartAnimations(spNode, GetMainApp()->GetAccumTime());			

		pkHint->UpdateNodeBound();
		//pkHint->UpdateControllers(GetMainApp()->GetAccumTime());
		pkHint->UpdateProperties();
		pkHint->UpdateEffects();
		pkHint->Update(GetMainApp()->GetAccumTime());
	}	
}

inline void Actor::DetachDummy(NiFixedString name, NiNodePtr spNode)
{
	if (!spNode) 
	{
		return ;
	}

	NiNode * pkHint = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName(name));
	if (!pkHint)
	{
		pkHint = NiDynamicCast(NiNode, m_spCharacterAM->GetNIFRoot()->GetObjectByName(name));
	}
	assert(pkHint);

	if (pkHint) 
	{
		spNode->Update(0.0f);
		NiTimeController::StopAnimations(spNode);
		pkHint->DetachChild(spNode);
		pkHint->UpdateNodeBound();
		pkHint->UpdateEffects();
		pkHint->UpdateProperties();
		pkHint->UpdateControllers(0.0f);
		pkHint->UpdateNodeBound();
		pkHint->UpdateEffects();
		pkHint->UpdateProperties();
		//pkHint->UpdateControllers(0.0f);

		pkHint->Update(0.0f);
	}
}

inline bool Actor::AttachHint(NiAVObject * spObj)
{
	assert(spObj);
	NiNode * pkHint = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("center"));
	assert(pkHint);

	if (pkHint && spObj->GetParent() != pkHint ) 
	{
		pkHint->AttachChild(spObj);				
		NiTimeController::StartAnimations(spObj, GetMainApp()->GetAccumTime());			

		pkHint->UpdateNodeBound();
		pkHint->UpdateProperties();
		pkHint->UpdateEffects();
		pkHint->Update(GetMainApp()->GetAccumTime());

		return true;
	}

	return false;
}
inline bool Actor::DetachHint(NiAVObject * pObj)
{
	if (!pObj) 
	{
		return true;
	}

	NiNode * pkHint = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("center"));
	assert(pkHint);

	if (pkHint) 
	{
		pObj->Update(0.0f);
		NiTimeController::StopAnimations(pObj);
		pkHint->DetachChild(pObj);
		pkHint->UpdateNodeBound();
		pkHint->UpdateEffects();
		pkHint->UpdateProperties();
		pkHint->Update(0.0f);
		return true;
	}
	
	return false;
}
/*
inline void Actor::SetVisible2DPortrait(bool bShow)
{
	m_bPortraitVisible = bShow;
}
*/