/********************************************************************
	作者:	　wincoqin
	时间:	2007/01/22
	目的:	
*********************************************************************/

#include "Top-KartPCH.h"
#include "PlayerPosUpdateTask.h"
#include "../levelmanager.h"
#define MIN_SHOW_WRONGWAY_TIMES 40
bool CPlayerPosUpdateTask::ms_bCreated = false;

void CPlayerPosUpdateTask::Execute(unsigned int uiFrameCount, float fTime)
{	
	GameStatusManager * pkGameStatusMgr = NxSceneMgr::Get()->GetGameStatusManager();
	if (!CLevelManager::Get()->IsEnableCheckPoint()) 
	{
		return;
	}
	// 修改人: wincoqin
	// 时间:   2007/1/22
	// 目的: 添加指针判空
	if (pkGameStatusMgr && GameStatusManager::GST_RACING == pkGameStatusMgr->GetGameState())
	{
		Play * pPlay = NULL;
		static std::set< Play * > & allPlayer = NxPlayMgr::GetAllPlay();

		for (std::set<Play*>::iterator iter = allPlayer.begin(); iter != allPlayer.end(); ++iter)
		{
			Play * pPlay = (*iter);

			if (pPlay) 
			{
#ifndef _XCART_
				CKart * pKart = pPlay->GetKart();
#else
				XCart* pKart = pPlay->GetKart();
#endif
				CLinks	* pkLinks	= CLevelManager::Get()->GetLinks(); 

				if (pkLinks && pKart) 
				{
					NiPoint3 kPos = pKart->GetKartTranslate();
					// 更新小车位于Check Point中的信息



					// 获得小车的姿势矩阵以获得其朝向
					NiPoint3 kKartDir;
					NiMatrix3 kKartRotate = pPlay->GetKartNIFRoot()->GetRotate();

					// 更新小地图
					if ( CEagleEyeMap::Get())
					{
						if (pPlay->IsHost()) 
						{
							
							pkLinks->UpdatePos(kPos);
							CEagleEyeMap::Get()->UpdateMap(kPos, kKartRotate);
						}
						else
						{

							CEagleEyeMap::Get()->UpdateMap(kPos, kKartRotate, pPlay->GetPlayUin(), false);
						}

					}

					//	修改人:	wincoqin
					//  时间:	2007/02/06
					//  目的:	修改了反向提示

					if (!pPlay->IsHost()) 
					{
						continue;
					}

					CLinks * pLinks = CLevelManager::Get()->GetLinks();
					/*
					CCheckPoint * pChkPt = pLinks->GetLastRightCheckPoint(false);

					// 路面朝向
					NiPoint3 kDir = CKartStatusManager::GetRightDir(pChkPt, pLinks->GetCorrectRunDir());
					*/
					_ASSERTE(pLinks->m_kLastPair.pPtFrom);
					NiPoint3 kDir=pLinks->m_kLastPair.pPtTo->kPoint - pLinks->m_kLastPair.pPtFrom->kPoint;
					NiPoint3::UnitizeVector(kDir);

					CTencentCar * pCar = pKart->GetVehicle();
					NxVec3 kartUp = pCar->GetUpGlobal();
					NxVec3 _kKartDir = pCar->GetVecGlobal();
					_kKartDir.normalize();
					NiPhysXTypes::NxVec3ToNiPoint3(_kKartDir, kKartDir);
					NxVec3 terrainNormal;
					NiPhysXTypes::NiPoint3ToNxVec3(CLevelManager::Get()->GetPhysXManager()->m_kNormalCarHitTerrain, terrainNormal);
					if (kartUp.dot(terrainNormal) < -0.50f) 
					{
						CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);							
						m_dwWrongwayShowTime = 0;
						return;
					}

					NiPoint3 kTmpNiPt3;
					NiPhysXTypes::NxVec3ToNiPoint3(pCar->GetForwardGlobal(),kTmpNiPt3);
					float fDirKartVec = kDir.Dot(kKartDir);
					float fDirKartBody= kDir.Dot(kTmpNiPt3);

					// 反向了
					if (fDirKartVec < -0.4f && fDirKartBody< -0.4f &&
						(pKart->GetVelocity() * GameSetting::SpeedDispCoef > 8.0f))
					{
						CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_WRONGWAY, NULL);
					}	
					else
					{
						CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_NONE, NULL);
					}		
					/*
					else if (pKart->GetVehicle()->IsInHell())
					{
					CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);

					float f = kDir.Dot(kKartDir);

					// 反向了
					if (f < -0.3f)
					{
					CKartStatusManager::Get()->OnTriggerShowWrongWay(true);
					}	
					else
					{
					CKartStatusManager::Get()->OnTriggerShowWrongWay(false);
					}			
					return;
					}

					*/

				}
			}

		}

	}

}