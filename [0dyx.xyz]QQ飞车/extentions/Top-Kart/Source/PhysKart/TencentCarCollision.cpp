/********************************************************************
	作者:	　wincoqin
	时间:	  2007/01/27
    文件名:   TencentCarCollision.cpp
	目的:   	
*********************************************************************/
#include "Top-KartPCH.h"
// libfile
#include "NxController.h"

// userfile
#include "../NxPlayMgr.h"
#include "TencentCarCollision.h"
#include "CollisionBase.h"
#include "../PowerController.h"



NxControllerAction CCollisionCenter::onShapeHit(const NxControllerShapeHit& hit)
{
	Play * pPlay = NULL;
	CollisionInfo kCollisionInfo;
	NxShape * pShape = hit.shape;


	if (!pShape)
		return NX_ACTION_NONE;

	XUserData * pUserData = (XUserData*)pShape->getActor().userData;

	if (!pUserData) 
	{
		return NX_ACTION_NONE;
	}

	//不是自己
	if (hit.controller != NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getBoxController()) 
	{
		return NX_ACTION_NONE;
	}
	pPlay = NxPlayMgr::GetHostPlay();

	kCollisionInfo.collisionShape = hit.shape;
	kCollisionInfo.dir			  = hit.dir;
	kCollisionInfo.fLength		  = hit.length;
	kCollisionInfo.unId			  = hit.id;
	kCollisionInfo.worldNormal	  = hit.worldNormal;
	kCollisionInfo.worldPos		  = hit.worldPos;
	
	NxShape * pkHitShape = kCollisionInfo.collisionShape;

	if (!pPlay || !pkHitShape) 
		return NX_ACTION_NONE;

	CTencentCar * pCar = pPlay->GetKart()->GetVehicle();

	if (!pCar)
		return NX_ACTION_NONE;

	NxVec3 vPos = pCar->GetPosGlobal();

	if (IsKindOfCollisionType((XUserData::ACTORTYPE)pUserData->_actorType, XUserData::AT_TERRAIN))
	{
//		pCar->SetTerrainNormal(hit.worldNormal);
		return NX_ACTION_NONE;
	}
		

	if (!IsKindOfCollisionType((XUserData::ACTORTYPE)pUserData->_actorType, 
		XUserData::AT_TERRAIN | XUserData::AT_GAMEPROP))
	{
		NxVec3 vVelo = pCar->GetVecGlobal();

		if (NxMath::abs(vVelo.dot(kCollisionInfo.worldNormal)) * GameSetting::SpeedDispCoef
			>= GameSetting::SpeedLineShowVelo * 0.2f && (NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * GameSetting::SpeedDispCoef >= 22.0f))
		{
			//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_HIT);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_HIT);
			CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_CRASH, NULL));
			CALL(CPowerController::Get(), GetSinglePower()->AccumulateFail());
			pPlay->SetCharaterAnimation(1307);// “HURT”动作
		}
		/*else
		{
		pPlay->PushPlayerAnimEvent(CAnimEventMgr::EAET_HIT);
		}*/
	}

	CollisionIter iterEnd = m_vecCollisionPool.end();

	for (CollisionIter iter = m_vecCollisionPool.begin(); iter != iterEnd; ++iter)
	{
		if (IsKindOfCollisionType((XUserData::ACTORTYPE)pUserData->_actorType, (*iter)->GetCallbackType())
			&& !IsKindOfCollisionType(XUserData::AT_WHEELLESSCAR, (*iter)->GetCallbackType()))
		{
			// TODO: 这个接口还要考虑
			(*iter)->OnTrigger(pPlay, &kCollisionInfo, pUserData->_actorType);
		}
	}

	return NX_ACTION_NONE;
}

NxControllerAction CCollisionCenter::onControllerHit(const NxControllersHit& hit)
{
	if (hit.controller != NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getBoxController()) 
	{
		return NX_ACTION_NONE;
	}

	Play * pSelf = NxPlayMgr::GetHostPlay();

	Play * pOther = NxPlayMgr::GetNearPlayFromHostPlay();
	
	if ( !pOther )
		return NX_ACTION_NONE;

	CollisionIter iterEnd = m_vecCollisionPool.end();

	for (CollisionIter iter = m_vecCollisionPool.begin(); iter != iterEnd; ++iter)
	{
		if (IsKindOfCollisionType(XUserData::AT_WHEELLESSCAR, (*iter)->GetCallbackType()))
		{
			// TODO: 这个接口得重写
			(*iter)->OnTrigger(pSelf, pOther, XUserData::AT_WHEELLESSCAR);
		}
	}

	return NX_ACTION_NONE;
}

bool CCollisionCenter::RegisterCollisionCallback(ICollisionCallback * pCollisionCallback)
{
	assert(pCollisionCallback);

	if (!pCollisionCallback) 
	{
		return false;
	}

	CollisionIter iterEnd = m_vecCollisionPool.end();
	// 防止重复注册
	for (CollisionIter iter = m_vecCollisionPool.begin(); iter != iterEnd; ++iter)
	{
		// 找到了
		if ((*iter) == pCollisionCallback) 
		{
			return true;
		}
	}

	m_vecCollisionPool.push_back(pCollisionCallback);

	return true;
}


bool CCollisionCenter::UnRegisterCollisionCallBack(ICollisionCallback * pCollisionCallback)
{
	CollisionIter iterEnd = m_vecCollisionPool.end();
	
	for (CollisionIter iter = m_vecCollisionPool.begin(); iter != iterEnd; ++iter)
	{
		// 找到了
		if ((*iter) == pCollisionCallback) 
		{
			m_vecCollisionPool.erase(iter);
			return true;
		}
	}

	return false;
}