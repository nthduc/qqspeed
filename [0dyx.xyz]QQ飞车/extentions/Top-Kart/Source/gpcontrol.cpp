////////////////////////////////////////////////
//     FileName:    gpcontrol.cpp
//     Author:      Fish
//     Date:        2007-1-18 14:37:50
//     Description: 道具整体逻辑实现文件
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "gpcontrol.h"
#include "GPSettings.h"
#include "gprender.h"
#include "gpmath.h"
#include "gpscript.h"
#include "common.h"
#include "script.h"
#include "AdvTime.h"
#include "nxscenemgr.h"
#include "gpcolumnmgr.h"
#include "GPPoint.h"
#include "gpeffect.h"
#include "GPNetInterCommn.h"
#include "track/CheckEditor.h"
#include "platform/GlobalInterface.h"
#include "RenderBase.h"
#include "soundmgr.h"
#include "EventViewer.h"
#include "Track/KartStatusManager.h"
#include "NxMath.h"
#include "fileop.h"
#include "search.h"
#include "nifop.h"

#define MAX_CUSTOMGPINDEX	0x8000000

class ScaleController : public NiTimeController
{
	NiDeclareRTTI;
public:

	ScaleController(float fSpeed, float fScaleBase = 0.5, float fScaleRange = 0.25)
	{
		m_fSpeed = fSpeed;
		m_fScaleBase = fScaleBase;
		m_fScaleRange = fScaleRange;
		m_fLastUpdateTime = 0;
	}
	~ScaleController()
	{
	}

	virtual void Update(float fTime)
	{
		float fDeltaTime = NiAbs(fTime - m_fLastUpdateTime);
		if ( m_pkTarget && GetActive() )    // if there is an object, rotate it
		{
			NiAVObject *pkObject = (NiAVObject*)m_pkTarget;
			assert(NiIsKindOf(NiAVObject, pkObject));
			float fScale = m_fScaleBase + m_fScaleRange * sin(m_fSpeed * fTime);
			pkObject->SetScale(fScale);
			XTrace("Scale:%f", fScale);
		}
		m_fLastUpdateTime = fTime;
	}

protected:

	virtual bool TargetIsRequiredType () const
	{
		return NiIsKindOf(NiAVObject,m_pkTarget);
	}

	float m_fSpeed;
	float m_fScaleBase;
	float m_fScaleRange;
	float m_fLastUpdateTime;
};
NiImplementRTTI(ScaleController, NiTimeController);


class RotateController : public NiTimeController
{
	NiDeclareRTTI;
public:

	RotateController(float fSpeed, NiPoint3 kVec)
	{
		m_fSpeed = fSpeed;
		m_kVec = kVec;
		m_fLastUpdateTime = 0;
	}

	~RotateController()
	{
	}
	virtual void Update(float fTime)
	{
		float fDeltaTime = NiAbs(fTime - m_fLastUpdateTime);
		if ( m_pkTarget && GetActive() )    // if there is an object, rotate it
		{
			NiAVObject *pkObject = (NiAVObject*)m_pkTarget;
			assert(NiIsKindOf(NiAVObject, pkObject));
			float fAngle = fmod((double)m_fSpeed * fDeltaTime, (double)3.14);
			NiMatrix3 kRot = pkObject->GetRotate();
			NiMatrix3 kRotDelta;
			kRotDelta.MakeIdentity();
			kRotDelta.MakeXRotation(fAngle);
			kRot = kRot * kRotDelta;
			pkObject->SetRotate(kRot);
			//XTrace("Rotate:%f", fAngle);
		}
		m_fLastUpdateTime = fTime;
	}

protected:

	virtual bool TargetIsRequiredType () const
	{
		return NiIsKindOf(NiAVObject,m_pkTarget);
	}

	float m_fSpeed;
	NiPoint3 m_kVec;
	float m_fLastUpdateTime;
};
NiImplementRTTI(RotateController, NiTimeController);

CGPControl::CGPControl():
m_pAimer(NULL),
m_pTargetIter(NULL),
m_pColumnMgr(NULL),
m_bEnd(FALSE)
{
	AppInit();
}

CGPControl::~CGPControl()
{
	AppUnInit();
}

BOOL CGPControl::AppInit()
{
	CGPEffectMgr::Create();
	CSoundMgr::Create();

	BOOL bRet = FALSE;
	bRet = _LoadConfig();
	KK_CHECK_BOOL(bRet);
	bRet = _InitObjects();
KK_EXIT:
	return bRet;
}

BOOL CGPControl::AppUnInit()
{
	if ( !m_bEnd )
	{
		End();
	}
	_UnInitObjects();
	_UnloadConfig();
	CGPEffectMgr::Destroy();
	return TRUE;
}

void CGPControl::Draw(NiCamera* pCamera)
{
}

void CGPControl::Start()
{
	m_pColumnMgr->SetActivity(true);
}

void CGPControl::End()
{
	CALL(m_pColumnMgr, SetActivity(false));
	CALL(m_pTargetIter, GetHost()->GetGPs().clear());

	if ( m_pTargetIter )
	{
		for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
		{
			IGPTarget* pTarget = m_pTargetIter->GetTarget(i);
			if ( pTarget )
			{
				pTarget->ClearAffectGPs();
				pTarget->GetGPs().clear();
			}
		}
	}
	_RemoveOrphans();
	m_bEnd = TRUE;
}


void CGPControl::Update(DWORD dwTime)
{
	if ( !m_pTargetIter )
	{
		return;
	}

	//更新瞄准境
	if ( m_pAimer )
	{
		m_pAimer->Update(dwTime);
	}

	//循环遍历每个对象，更新之
	for ( int i = 0; i < m_pTargetIter->GetTargetCount(); i++ )
	{
		IGPTarget* pTarget = m_pTargetIter->GetTarget(i);
		if ( pTarget )
		{
			_UpdateTarget(pTarget);
		}
		else
		{
			XWaring("m_pTargetIter->GetTarget(%d) failed", i);
		}
	}
	IGPTarget* pDummy = m_pTargetIter->GetDummy();
	if ( pDummy )
	{
		_UpdateTarget(pDummy);
	}
	//更新没有对象可依附的道具（通常在地面上）
	_UpdateOrphans();
}

/*
_UpdateTarget:
更新单个目标
*/
void CGPControl::_UpdateTarget(IGPTarget* pTarget)
{
	if ( !pTarget )
	{
		return;
	}

	DWORD dwNow = GetHighResolutionTime();
	GP_DATA_LIST& listData = pTarget->GetAffectGPs();

	//目标没有中任何道具，不用继续处理
	if ( listData.size() <= 0 )
	{
		return;
	}

	//一个人可能中了多个道具，包括自己用的防护型道具
	GP_DATA_LIST_Iterator it = listData.begin();	
	for ( ; it != listData.end(); ++it )
	{
		//当前道具数据
		LPGP_DATA pData = (LPGP_DATA)&*it;	
		//当前道具的配置
		LPGP_CONFIG pConfig = FindConfig(pData->dwID);	
		if ( !pConfig )
		{
			XWaring("FindConfig(%d) failed", pData->dwID);
			continue;
		}

		//当前道具的作用列表
		GP_AFFECTDATA_LIST& listAffectData = pData->listAffect;	
		DWORD dwOriginTime = pData->dwUseTime;
		DWORD dwElapse = dwNow - dwOriginTime;
		DWORD dwReadyTime = pConfig->dwReadyTime;
		DWORD dwTriggerTime = pConfig->dwTriggerTime;

		//运动轨迹
		_UpdateGPMove(pData, dwElapse);

		//这个时候还是发射过程中
		if ( dwElapse < dwReadyTime )	
		{
		}
		//这个是在击中过程中
		else if ( dwElapse < dwReadyTime + dwTriggerTime )
		{
			_EnsureLifeState(pData, ELS_Trigger);
		}
		//这个是击中后了（结束前）
		else
		{
			if ( _CanStop(pTarget, pData->dwID) )
			{
				XTrace("中止条件到了，设上标志");
				pData->dwLifeState = ELS_Die;
			}

			//一个道具，对应多个Affect
			GP_AFFECTDATA_LIST_Iterator itAffectData = listAffectData.begin();
			for ( ; itAffectData != listAffectData.end(); ++itAffectData )
			{
				//当前道具的当前作用数据
				LPGP_AFFECTDATA pAffectData = (LPGP_AFFECTDATA)&*itAffectData; 
				//当前道具的当前作用配置
				LPGP_AFFECT pAffectConfig = pAffectData->pAffectConfig;	
				DWORD dwTimeBegin = dwReadyTime + dwTriggerTime + pAffectConfig->dwStartTime;
				DWORD dwTimeEnd = dwTimeBegin + pAffectConfig->dwLifeTime;
				if ( dwElapse < dwTimeBegin )	//轮到道具作用了
				{
					//这个时间已经过了发射期了，如果还没有Trigger的话，该触发了
					if ( ELS_Trigger != _EnsureLifeState(pData, ELS_Trigger) 
						|| pConfig->dwTriggerTime != 0 )
					{	
						//击中的话，需要判断多个道具是否互斥
						_UpdateBlendState(pTarget, listData, it);
					}
				}
				else if ( dwElapse < dwTimeEnd )
				{
					//这个时间已经过了发射期了，如果还没有Trigger的话，该触发了
					if ( ELS_Trigger != _EnsureLifeState(pData, ELS_Trigger)
						|| pConfig->dwTriggerTime != 0 )
					{
						//击中的话，需要判断多个道具是否互斥
						_UpdateBlendState(pTarget, listData, it);
					}

					//正好在本Affect作用范围内
					//只有ELS_Trigger状态的才运行启动，因为有些道具会被排斥而成为ELS_Disable
					if ( m_bAffectEnabled == TRUE
						&& pData->dwLifeState == ELS_Trigger 
						&& !pAffectData->pAffect->IsPlaying() )
					{
						XTrace("开始启动道具(%d)的效果(%d)", pData->dwID, pAffectConfig->dwID);
						//上报服务器被作用了，0表示被作用到了(只需要上报一次)(自己才上报)
						if ( pData->pConfig->dwAffectType == EAT_Attack && 
							itAffectData == listAffectData.begin() &&
							GetTargetByID(pData->dwDestID) == m_pTargetIter->GetHost() )
						{
							int nResultID = EPER_ATTACKED;
							if ( pData->dwOriginID != 0 )
								nResultID = EPER_OUTOFATTACK_BECAUSEOFAVATAR;
							CGPNetIntercommn::RequestPropEffectResult(pData->dwID, pData->dwIndex, nResultID, 0);
						}

						IGPTarget* pSrcTarget = NULL;
						IGPTarget* pDestTarget = GetTargetByID(pData->dwDestID);
						if ( pData->dwSrcID )
						{
							pSrcTarget = GetTargetByID(pData->dwSrcID);
						}
						GP_ID kGPID(pData->dwID, pData->dwIndex, 0);
						pAffectData->pAffect->Init(this, kGPID);
						pAffectData->pAffect->SetTarget(pSrcTarget, pDestTarget);
						pAffectData->pAffect->SetLifeTime(pAffectData->pAffectConfig->dwLifeTime);
						pAffectData->pAffect->SetParam(pAffectData->pAffectConfig->fParam);
						pAffectData->pAffect->SetOption(pAffectData->pAffectConfig->dwOption);
						pAffectData->pAffect->Start();
						pAffectData->dwState = EAS_Playing;
					}
					else if ( pData->dwLifeState == ELS_Disbled 
						&& pAffectData->pAffect->IsPlaying() )
					{
						XTrace("由于被别的道具排斥，停止道具(%d)的效果(%d)", pData->dwID, pAffectConfig->dwID);
						pAffectData->pAffect->Stop();
						pAffectData->dwState = EAS_Idle;
					}
					else if ( pData->dwLifeState == ELS_Die 
						&& pAffectData->pAffect->IsPlaying() )
					{
						XTrace("由于条件不满足，停止道具(%d)的效果(%d)", pData->dwID, pAffectConfig->dwID);
						pAffectData->pAffect->Stop();
						pAffectData->dwState = EAS_Idle;
					}
					//只有在正常作用的时候才更新
					if ( pAffectData->pAffect->IsPlaying() )
					{
						pAffectData->pAffect->Update(dwElapse - dwTimeBegin);
					}
				}
				//这个时候道具该终结它的生命期了
				else // dwElapse >= dwTimeEnd
				{
					//本Affect该终结了
					if ( pAffectData->pAffect->IsPlaying() )
					{
						XTrace("停止了道具(%d)的效果(%d)", pData->dwID, pAffectConfig->dwID);
						pAffectData->pAffect->Stop();
					}

					//如果所有Affect的时间都超时了，是时候删除这个道具了
					if ( dwElapse >= dwReadyTime + dwTriggerTime + _GetAffectsTotalTime(pConfig->listAffect) 
						&& pData->dwLifeState != ELS_Die )
					{
						XTrace("最后一个作用的时间也到了，打上标志以便下次方便的时候删除这个道具吧");
						pData->dwLifeState = ELS_Die;
					}
				
				}
			}
	
		}
	}

	//删除过期的道具（一次删除一个，以防错误指针发生）
	it = listData.begin();
	for ( ; it != listData.end(); ++it )
	{
		LPGP_DATA pData = (LPGP_DATA)&*it;
		LPGP_CONFIG pConfig = pData->pConfig;
		if ( pData->dwLifeState == ELS_Die ||
			pData->dwLifeState == ELS_Disbled )
		{
			//停止所有作用
			GP_AFFECTDATA_LIST& listAffectData = pData->listAffect;
			GP_AFFECTDATA_LIST_Iterator itAffectData = listAffectData.begin();
			for ( ; itAffectData != listAffectData.end(); ++itAffectData )
			{
				//当前道具的当前作用数据
				LPGP_AFFECTDATA pAffectData = (LPGP_AFFECTDATA)&*itAffectData; 
				if ( pAffectData->pAffect->IsPlaying() )
				{
					pAffectData->pAffect->Stop();
				}
			}

			//上报服务器此道具没有产生效果（被防御或者被排斥）,1表示未被作用
			if ( pData->pConfig->dwAffectType == EAT_Attack &&
				pData->dwLifeState == ELS_Disbled 
				&& GetTargetByID(pData->dwDestID) == m_pTargetIter->GetHost() )
			{
				CGPNetIntercommn::RequestPropEffectResult(pData->dwID, pData->dwIndex, EPER_OUTOFATTACK, 0);
			}

			XTrace("下达了一个删除了道具(%d)的命令", pData->dwID);
			pTarget->RemoveAffectGP(pData->dwIndex);
			break;
		}
	}
	return;
}

/*
_EnsureLifeState:
如果状态需要切换到特定状态，需要满足一定的路线规则，该触发的要触发
*/
ENMLIFESTATE CGPControl::_EnsureLifeState(LPGP_DATA pData, ENMLIFESTATE NewState)
{
	ENMLIFESTATE enmOldState = (ENMLIFESTATE)pData->dwLifeState;
	if ( NewState == enmOldState )
	{
		return enmOldState;
	}

	switch ( enmOldState )
	{
	case ELS_Throw:
		{
			switch ( NewState )
			{
			case ELS_Trigger:
				{
					OnTrigger(pData);
					pData->dwLifeState = ELS_Trigger;
				}
				break;
			case ELS_Disbled:
				{
					_EnsureLifeState(pData, ELS_Trigger);
					OnDisabled(pData);
					pData->dwLifeState = ELS_Disbled;
				}
				break;
			case ELS_Die:
				{
					_EnsureLifeState(pData, ELS_Trigger);
					OnDie(pData);
					pData->dwLifeState = ELS_Die;
				}
				break;
			}
		}
		break;
	case ELS_Trigger:
		{
			switch ( NewState )
			{
			case ELS_Disbled:
				{
					OnDisabled(pData);
					pData->dwLifeState = ELS_Disbled;
				}
				break;
			case ELS_Die:
				{
					OnDie(pData);
					pData->dwLifeState = ELS_Die;
				}
				break;
			}
		}
		break;
	case ELS_Disbled:
		{
			switch ( NewState )
			{
			case ELS_Die:
				{
					OnDie(pData);
					pData->dwLifeState = ELS_Die;
				}
				break;
			}
		}
		break;
	}
	return enmOldState;
}

/*
_UpdateGPMove:
更新运动轨迹,dwTime是自道具启动时间到现在的时间间隔
*/
void CGPControl::_UpdateGPMove(LPGP_DATA pData, DWORD dwTime)
{
	if ( !pData )
	{
		XWaring("_UpdateGPMove() invalid args");
		return;
	}

	LPGP_CONFIG pConfig = FindConfig(pData->dwID);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", pData->dwID);
		return;
	}

	IGPMovementControl* pMoveCtrl = pData->pMoveCtrl;
	if ( pMoveCtrl )
	{
		LPGP_MOVECTRL pMoveInfo = &pConfig->stMoveCtrl;
		if ( !pMoveCtrl->IsPlaying() 
			&& dwTime >= pMoveInfo->dwStartTime
			&& dwTime < pMoveInfo->dwStartTime + pMoveInfo->dwLifeTime )
		{
			XTrace("启动道具(%d)的运行轨迹", pData->dwID);
			pMoveCtrl->SetTarget(GetTargetByID(pData->dwSrcID), GetTargetByID(pData->dwDestID));
			GP_POSITION& kSrcPos = pConfig->stMoveCtrl.stSrcPosition;
			GP_POSITION& kDestPos = pConfig->stMoveCtrl.stDestPosition;
			pMoveCtrl->SetPos(NiPoint3(kSrcPos.x, kSrcPos.y, kSrcPos.z), NiPoint3(kDestPos.x, kDestPos.y, kDestPos.z));
			pMoveCtrl->SetParam(CGPEffectMgr::Get()->NewEffect(pMoveInfo->dwMoveNode), pMoveInfo->dwLifeTime);
			pMoveCtrl->Start();
		}
		else if ( pMoveCtrl->IsPlaying() && dwTime >= pMoveInfo->dwStartTime + pMoveInfo->dwLifeTime )
		{
			XTrace("停止了道具(%d)的运行轨迹", pData->dwID);			
			pMoveCtrl->Stop();
		}
		if ( dwTime >= pMoveInfo->dwStartTime
			&& dwTime < pMoveInfo->dwStartTime + pMoveInfo->dwLifeTime )
		{
			pMoveCtrl->Update(dwTime - pMoveInfo->dwStartTime);
		}
	}
}

/*
_UpdateOrphans:
更新游离道具（在地上的，比如香蕉皮、大水泡、云雾）
*/
void CGPControl::_UpdateOrphans()
{
	if ( m_listOrphanGPs.size() <= 0 )
	{
		return;
	}

	DWORD dwNow = GetHighResolutionTime();
	GP_DATA_LIST_Iterator it = m_listOrphanGPs.begin();
	for ( ; it != m_listOrphanGPs.end(); ++it )
	{
		LPGP_DATA pData = &*it;
		LPGP_CONFIG pConfig = FindConfig(pData->dwID);
		DWORD dwElapse = dwNow - pData->dwUseTime;
		//触发之
		if ( dwElapse >= pConfig->dwBirthTime )
		{
			CGPPoint* pPoint = (CGPPoint*)pData->dwData;
			if ( pPoint && !pPoint->GetActive() )
			{
				pPoint->SetActive(true);
				//需要的话，显示监视镜头
				IGPTarget* pTarget = m_pTargetIter->GetHost();
				if ( pConfig->dwCameraID && pData->dwSrcID == pTarget->GetID() )
				{
					NiNodePtr spNode = CGPEffectMgr::Get()->NewEffect(pConfig->dwCameraID, FALSE);
					NiCameraPtr spCamera = CNifOp::FindCamera(spNode);
					if ( spCamera )
					{
						NiNodePtr spParent = pPoint->GetParam()->m_pkBox;
						if ( spParent )
						{
							spParent->AttachChild(spNode);
							EventViewerMgr::StartItemEvent(spCamera);
						}
					}
				}
			}
		}
		//删除过期的道具，一次删除一个
		if ( dwElapse >= pConfig->dwBirthTime + pConfig->dwStayTime )
		{
			_RemoveOrphan(&*it);
			m_listOrphanGPs.erase(it);
			break;
		}
	}
}

/*
_RemoveOrphan:
删除游离道具
*/
void CGPControl::_RemoveOrphan(LPGP_DATA pData)
{
	LPGP_CONFIG pConfig = FindConfig(pData->dwID);
	CGPPoint* pPoint = (CGPPoint*)pData->dwData;
	if ( pPoint )
	{
		pPoint->SetActive(false);
		//需要的话，销毁监视镜头
		IGPTarget* pTarget = m_pTargetIter->GetHost();
		if ( pConfig->dwCameraID && pData->dwSrcID == pTarget->GetID() )
		{
			EventViewerMgr::CloseItemEvent();
		}
		CGPPointMgr::RemovePoint(pPoint->GetID());
	}
}

void CGPControl::_RemoveOrphans()
{
	GP_DATA_LIST_Iterator it = m_listOrphanGPs.begin();
	for ( ; it != m_listOrphanGPs.end(); ++it )
	{
		_RemoveOrphan(&*it);
	}
	m_listOrphanGPs.clear();
}

/*
_UpdateBlendState:
通过互斥条件来调整状态
最后被排斥的道具被置上状态ELS_Disbled
*/
void CGPControl::_UpdateBlendState(IGPTarget* pTarget, GP_DATA_LIST& listData, GP_DATA_LIST_Iterator& itTarget)
{
	//只有一个道具不需要Blend
	if ( listData.size() <= 1 )
	{
		return;
	}

	//一个人可能中了多个道具，包括自己用的防护型道具
//	GP_DATA_LIST_Iterator it = listData.begin();	
//	for ( ; it != listData.end(); ++it )
	do
	{
		GP_DATA_LIST_Iterator it = itTarget;
		//当前道具数据
		LPGP_DATA pData = (LPGP_DATA)&*it;	
		//当前道具的配置
		LPGP_CONFIG pConfig = FindConfig(pData->dwID);	
		if ( !pConfig )
		{
			XWaring("FindConfig(%d) failed", pData->dwID);
			break;
		}

		//这是多个道具同时作用的情况，而且当前不是第一个道具，需要跟前面的做Blend
		//XTrace("遇到多个道具同时作用了，下面小心处理");
		GP_DATA_LIST_Iterator itDataTmp = listData.begin();
		for ( ; itDataTmp != it; ++itDataTmp )
		{
			LPGP_DATA pDataTmp = (LPGP_DATA)&*itDataTmp;
			//只有前一个道具还在作用的时候才需要Blend
			if ( pDataTmp->dwLifeState == ELS_Trigger )
			{
				LPGP_CONFIG pConfigTmp = FindConfig(pDataTmp->dwID);
				if ( !pConfigTmp )
				{
					XWaring("FindConfig(%d) failed", pDataTmp->dwID);
					continue;
				}
				/*防御型道具
				需要处理：
				1、Disable后面的攻击型道具
				2、防御次数达到上限的时候自动销毁
				*/
				if ( pConfigTmp->dwAffectType == EAT_Protect )
				{
					switch ( pConfigTmp->dwProtectType )
					{
					case EPT_AllAttack:
						if ( pConfig->dwAffectType == EAT_Attack 
							&& pData->dwLifeState != ELS_Disbled )
						{
							XTrace("被前面一个防御型的道具阻止了");
							pDataTmp->dwUseCount++;
							pData->dwLifeState = ELS_Disbled;
							OnProtect(pDataTmp->dwID, pTarget->GetID());
							OnProtect(pData->dwID, pTarget->GetID());
						}
						break;
					case EPT_All:
						if ( pData->dwLifeState != ELS_Disbled )
						{
							XTrace("被前面一个防御型的道具阻止了");
							pDataTmp->dwUseCount++;
							pData->dwLifeState = ELS_Disbled;
							OnProtect(pDataTmp->dwID, pTarget->GetID());
							OnProtect(pData->dwID, pTarget->GetID());
						}
						break;
					case EPT_Specified:
						if ( pData->dwID == pDataTmp->dwID 
							&& pData->dwLifeState != ELS_Disbled )
						{
							XTrace("被前面一个防御型的道具阻止了");
							pDataTmp->dwUseCount++;
							pData->dwLifeState = ELS_Disbled;
							OnProtect(pDataTmp->dwID, pTarget->GetID());
							OnProtect(pData->dwID, pTarget->GetID());
						}
						break;
					}
					if ( pDataTmp->dwUseCount >= pConfigTmp->dwProtectCount 
						&& pConfigTmp->dwProtectCount > 0 )
					{
						XTrace("有一个防御型的道具(%d)耐久度变为0了，该删除了", pDataTmp->dwID);
						pDataTmp->dwLifeState = ELS_Die;
					}
				}
				/*攻击型道具
				需要处理：
				1、混合操作
				*/
				else if ( pConfigTmp->dwAffectType == EAT_Attack )
				{
					//如果是治疗型道具
					if ( pConfig->dwAffectType == EAT_Cure )
					{
						if ( pConfigTmp->dwID == pConfig->dwCureID ) 
						{
							if ( pDataTmp->dwLifeState == ELS_Trigger )
							{
								XTrace("前面一个道具%d被治疗作用了", pConfigTmp->dwID);
								pDataTmp->dwLifeState = ELS_Disbled;
								pData->dwLifeState = ELS_Die;
								OnCure(pData->dwID, pTarget->GetID());
							}
						}
					}
					else
					{
						DWORD dwBlendMode = pConfigTmp->dwBlendMode;
						//如果前面是攻击型道具，我是非攻击型道具，那我都能使用，无视攻击型道具的排他性
						if ( pConfig->dwAffectType != EAT_Attack )
						{
							dwBlendMode = EBM_AddNext;
						}

						switch ( dwBlendMode )
						{
						case EBM_UseNext:
							if ( pData->dwLifeState != ELS_Trigger )
							{
								XTrace("哈哈，太好了！前面这个攻击型的道具把位置让给了我");
								pDataTmp->dwLifeState = ELS_Disbled;
								pData->dwLifeState = ELS_Trigger;
							}
							break;
						case EBM_AddNext:
							if ( pData->dwLifeState != ELS_Trigger )
							{
								XTrace("还好，前面这个攻击型的道具并不排斥我");
								pData->dwLifeState = ELS_Trigger;
							}
							break;
						case EBM_IgnoreNext:
							if ( pConfig->dwBlendMode == EBM_ForceReplaceOld )
							{
								if ( pData->dwLifeState != ELS_Trigger )
								{
									XTrace("嘻嘻，前面的道具排斥我，不过我可以强行，哈哈");
									pDataTmp->dwLifeState = ELS_Disbled;
									pData->dwLifeState = ELS_Trigger;
								}
							}
							else
							{
								if ( pData->dwLifeState != ELS_Disbled )
								{
									XTrace("不好， 我被前面的道具排斥了");
									pData->dwLifeState = ELS_Disbled;
								}
							}
							break;
						case EBM_ForceReplaceOld:
							break;
						}
					}
				}

				//道具生成型的
				else if ( pConfigTmp->dwAffectType == EAT_Generator )
				{
				}
				else
				{
					XWaring("不支持的道具类型");
				}
			}
		}
	}while(0);
}

/*
ToggleAffect:
打开或关闭作用开关，如果作用开关被关闭，所有效果将被禁止
*/
BOOL CGPControl::ToggleAffect(BOOL bEnable)
{
	BOOL bOld = m_bAffectEnabled;
	if ( bOld != bEnable )
	{
		m_bAffectEnabled = bEnable;
	}
	return bOld;
}

/*
ToggleOp：
打开或关闭操作开关，如果操作开关被关闭，所有操作将被禁止
*/
BOOL CGPControl::ToggleOp(BOOL bEnable)
{
	BOOL bOld = m_bOpEnabled;
	if ( bOld != bEnable )
	{
		m_bOpEnabled= bEnable;
	}
	return bOld;
}

IGPTargetIterator* CGPControl::GetTargetIterator()
{
	return m_pTargetIter;
}

/*
GenerateNewGP:
生成新的道具（道具生成型道具使用）
*/
BOOL CGPControl::ProcessGeneratorGP(GP_ID id, IGPTarget* pSrcTarget)
{
	LPGP_CONFIG pConfig = FindConfig(id.dwID);
	if ( pConfig->dwAffectType != EAT_Generator )
	{
		return FALSE;
	}

	DWORD dwNewID = pConfig->dwNewID;
	return GenerateNewGP(id, dwNewID, pSrcTarget);
}

BOOL	CGPControl::GenerateNewGP(GP_ID id, DWORD dwNewID, IGPTarget* pSrcTarget)
{
	if ( !pSrcTarget )
	{
		XWaring("GenerateNewGP() bad arg");
		return FALSE;
	}

	LPGP_CONFIG pConfigNew = FindConfig(dwNewID);
	if ( !pConfigNew )
	{
		XWaring("FindConfig(%d) failed", dwNewID);
		return FALSE;
	}

	GP_POSITION kOffset = pConfigNew->stPosition;
	GP_BOUND kBound = pConfigNew->stBound;

	CLinks* pLinks = CLevelManager::Get()->GetLinks();
	if ( pLinks )
	{
		NiPoint3 kPos = pSrcTarget->GetPos();
		NiPoint3 kNewPos = kPos;
		NiPoint3 kNewDir(1.0, 0, 0);
		bool bFind = TRUE;
		//如果太近的话，就直接用车的位置了
		if ( abs(kOffset.x) > 0.000001 )
		{
			pLinks->GetForwardPoint(kNewPos, kNewDir, kPos, kOffset.x);
			//			CLinks * pLinks = NxSceneMgr::Get()->GetActiveSceneLevel()->GetLinks();
			//			CCheckPoint * pChkPt = pLinks->GetLastRightCheckPoint(false);
			//			kNewDir = CKartStatusManager::GetRightDir(pChkPt, pLinks->GetCorrectRunDir());
		}
		if ( bFind )
		{
			XTrace("找到了生成点，放置道具(%d)", dwNewID);
			CGPPointMgr::Param kParam;
			kParam.m_fRadius = kBound.x;
			kParam.m_kRotate = NiMatrix3::IDENTITY;
			//	修改人:	wincoqin
			//  时间:	2007/02/09
			//  目的:	修改显示的姿态和镜头（大糖泡的镜头）			
			kParam.m_kRotate.SetCol(0, kNewDir.x, kNewDir.y, kNewDir.z);
			NiPoint3 kRightDir = NiPoint3::UNIT_Z.Cross(kNewDir);
			kRightDir.Unitize();
			kParam.m_kRotate.SetCol(1, kRightDir);
			kParam.m_kRotate.SetCol(2, NiPoint3::UNIT_Z);
			// 修改结束
			kParam.m_kTranslate = kNewPos;
			kParam.m_nType = CGPPointMgr::EM_DYNAMIC_PROP;
			kParam.m_dwGPID = dwNewID;
			kParam.m_dwGPIndex = id.dwIndex;
			kParam.m_pGPReport = this;
			kParam.m_pkBox = CGPEffectMgr::Get()->NewEffect(pConfigNew->dwStayNode);
			CGPPoint* pPoint = CGPPointMgr::CreatePoint(&kParam, false);
			if ( !pPoint )
			{
				XWaring("CGPPointMgr::CreatePoint failed");
			}
			GP_DATA kData;
			kData.dwID = dwNewID;
			kData.dwIndex = id.dwIndex;
			kData.dwSrcID = pSrcTarget->GetID();
			kData.dwDestID = 0;
			kData.dwUseTime = GetHighResolutionTime();
			kData.dwUseCount = 0;
			kData.dwData = (DWORD)pPoint;
			m_listOrphanGPs.push_back(kData);
		}
		else
		{
			XWaring("在当前位置无法找到生成点，无法放置道具");
		}
	}
	return TRUE;
}

/*
_FindOrphan:
通过ID找到一个对应的游离道具
*/
GP_DATA_LIST_Iterator CGPControl::_FindOrphan(GP_ID id)
{
	GP_DATA_LIST_Iterator it = m_listOrphanGPs.begin();
	for ( ; it != m_listOrphanGPs.end(); ++it )
	{
		LPGP_DATA pData = &*it;
		if ( pData && pData->dwID == id.dwID && pData->dwIndex == id.dwIndex )
		{
			return it;
		}
	}
	return m_listOrphanGPs.end();
}

/*
OnTrigger:
物理碰撞回调（撞到地面上的道具）
*/
int CGPControl::OnTrigger(LPVOID lpData, LPVOID lpHitObjData)
{
	CGPPoint* pPoint = CGPPointMgr::GetPoint((USHORT)lpData);
	if ( !pPoint || !pPoint->GetActive() || pPoint->GetTriggered() )
	{
		return 0;
	}

	const CGPPointMgr::Param* pParam = pPoint->GetParam();
	Play* pPlay = (Play*)lpHitObjData;
	if ( !pParam || !pPlay || !pPlay->IsHost() )
	{
		return 0;
	}

	if ( pParam->m_nType == CGPPointMgr::EM_DYNAMIC_PROP )
	{
		KK_CHECK_POINTER(m_pTargetIter);
		IGPTarget* pHost = m_pTargetIter->GetHost();
		KK_CHECK_POINTER(pHost);

		DWORD dwID = pParam->m_dwGPID;
		DWORD dwIndex = pParam->m_dwGPIndex;
		GP_ID id(dwID, dwIndex, 0);
		OnTriggerByGP(id, pHost->GetID());
		pPoint->SetTriggered(true);
		//这里不用上报了，Update里面统一做了
		//CGPNetIntercommn::RequestPropEffectResult(id.dwID, id.dwIndex, 0, 0);
	}

KK_EXIT:
	return 0;
}


void CGPControl::OnTriggerByGP(GP_ID id, DWORD dwDestID)
{
	{
		GP_DATA_LIST_Iterator it = _FindOrphan(id);
		if ( it == m_listOrphanGPs.end() )
		{
			XWaring("_FindOrphan(%d, %d) failed", id.dwID, id.dwIndex);
			goto KK_EXIT;
		}

		LPGP_DATA pData = &*it;
		IGPTarget* pSrcTarget = GetTargetByID(pData->dwSrcID);
		_ApplyGP(id, pSrcTarget, GetTargetByID(dwDestID));
		
		LPGP_CONFIG pConfig = FindConfig(id.dwID);
		if ( !pConfig )
		{
			XWaring("FindConfig(%d) failed", id.dwID);
			goto KK_EXIT;
		}

		pData->dwUseCount++;
		if ( pConfig->dwPassCount > 0 && pData->dwUseCount >= pConfig->dwPassCount )
		{
			_RemoveOrphan(&*it);
			m_listOrphanGPs.erase(it);
		}
	}
KK_EXIT:
	return;
}

/*
GetTargetByID:
通过ID找到目标
*/
IGPTarget*	CGPControl::GetTargetByID(DWORD id)
{
	if ( !m_pTargetIter )
	{
		XWaring("GetTargetByID() invalid initialization");
		return NULL;
	}

	for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
	{
		IGPTarget* pTarget = m_pTargetIter->GetTarget(i);
		if ( id == pTarget->GetID() )
		{
			return pTarget;
		}
	}

	XWaring("GetTargetByID(%d) failed", id);
	return NULL;
}

/*
_GetTargetByIndex:
通过名次取到Target
*/
IGPTarget*	CGPControl::_GetTargetByIndex(DWORD dwIndex)
{
	if ( !m_pTargetIter )
	{
		XWaring("_GetTargetByIndex() invalid initialization");
		return NULL;
	}

	for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
	{
		IGPTarget* pTarget = m_pTargetIter->GetTarget(i);
		if ( dwIndex == pTarget->GetIndex() )
		{
			return pTarget;
		}
	}

	XWaring("_GetTargetByIndex(%d) failed", dwIndex);
	return NULL;
}

/*
_FetchTargetList:
搜集打击目标
*/
GPTARGET_LIST CGPControl::_FetchTargetList(DWORD id, IGPTarget* pSrcTarget)
{
	GPTARGET_LIST listTarget;
	if ( !pSrcTarget )
	{
		XWaring("_FetchTargetList() bad arg");
		goto KK_EXIT;
	}
	DWORD dwSrcTeamID = pSrcTarget->GetTeam();
	DWORD dwSrcIndex = pSrcTarget->GetIndex();

	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		goto KK_EXIT;
	}

	//道具生成型的，直接处理
	if ( pConfig->dwAffectType == EAT_Generator )
	{
		goto KK_EXIT;	//适当的用goto对程序是有好处的，有不同意见欢迎探讨
	}

	switch ( pConfig->dwSelectTargetType )
	{
	case ESTT_Select:
		break;
	default:	//ESTT_NoSelect
		{
			if ( pConfig->dwAffectType == EAT_Attack || pConfig->dwAffectType == EAT_Cure )
			{
				if ( pConfig->bAffectSelf 
					&& !pSrcTarget->GetInvalidTarget() )
				{
					XTrace("这种道具同时会作用到自己的（很可能是磁铁）");
					listTarget.push_back(pSrcTarget);
				}
				switch ( pConfig->dwTargetType )
				{
				case ETT_Play:
					for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
					{
						IGPTarget* pTarget = m_pTargetIter->GetTarget(i);
						if ( !pTarget->GetInvalidTarget() 
							&& pTarget != pSrcTarget )
						{
							listTarget.push_back(pTarget);
						}
					}
					break;
				case ETT_Enemy:
					for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
					{
						IGPTarget* pTarget = m_pTargetIter->GetTarget(i);
						if ( !pTarget->GetInvalidTarget() 
							&& pTarget != pSrcTarget 
							&& pTarget->GetTeam() != dwSrcTeamID )
						{
							listTarget.push_back(pTarget);
						}
					}
					break;
				case ETT_FirstPlay:
					for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
					{
						IGPTarget* pTarget = _GetTargetByIndex(i);
						if ( pTarget && !pTarget->GetInvalidTarget() )
						{
							if ( pTarget != pSrcTarget )
							{
								listTarget.push_back(pTarget);
							}
							break;
						}

					}
					break;
				case ETT_PrevPlay:
					for ( int i = dwSrcIndex - 1; i >= 0; --i )
					{
						IGPTarget* pTarget = _GetTargetByIndex(i);
						if ( pTarget && pTarget != pSrcTarget 
							&& !pTarget->GetInvalidTarget() )
						{
							listTarget.push_back(pTarget);
							break;
						}
					}
					break;
				case ETT_PrevEnemy:
					for ( int i = dwSrcIndex - 1; i >= 0; --i )
					{
						IGPTarget* pTarget = _GetTargetByIndex(i);
						if ( pTarget && pTarget != pSrcTarget 
							&& !pTarget->GetInvalidTarget() 
							&& pTarget->GetTeam() != dwSrcTeamID )
						{
							listTarget.push_back(pTarget);
							break;
						}
					}
					break;
				case ETT_Me:
					{
						IGPTarget* pTarget = pSrcTarget;
						if ( !pTarget->GetInvalidTarget() )
						{
							listTarget.push_back(pTarget);
						}
					}
					break;
				}
			}
			if ( pConfig->dwAffectType == EAT_Protect || pConfig->dwAffectType == EAT_Cure )
			{
				switch ( pConfig->dwProtectTarget )
				{
				case EPTT_Me:
					{
						if ( !pSrcTarget->GetInvalidTarget() )
						{
							listTarget.push_back(pSrcTarget);
						}
					}
					break;
				case EPTT_Team:
					for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
					{
						IGPTarget* pTarget = m_pTargetIter->GetTarget(i);
						if ( pTarget && pTarget->GetTeam() == dwSrcTeamID 
							&& !pTarget->GetInvalidTarget() )
						{
							listTarget.push_back(pTarget);
						}
					}
					break;
				}
			}
		}
		break;
	}
KK_EXIT:
	return listTarget;
}

/*
_GetIndexInTeam:
获得目标在自己队伍内的名次
*/
DWORD CGPControl::_GetIndexInTeam(IGPTarget* pTarget)
{
	if ( !pTarget )
	{
		XWaring("_GetIndexInTeam(): bad arg");
		return 0;
	}

	DWORD dwIndex = pTarget->GetIndex();
	DWORD dwTeamID = pTarget->GetTeam();
	DWORD dwIndexInTeam = 0;

	for ( int i = 0; i < m_pTargetIter->GetTargetCount(); ++i )
	{
		IGPTarget* pTmpTarget = m_pTargetIter->GetTarget(i);
		if ( pTmpTarget && pTmpTarget != pTarget && pTmpTarget->GetTeam() == dwTeamID )
		{
			if ( pTmpTarget->GetIndex() < dwIndex )
			{
				++dwIndexInTeam;
			}
		}
	}
	return dwIndexInTeam;
}

/*
_GetAffectsTotalTime:
取得道具作用列表总的时间
*/
DWORD CGPControl::_GetAffectsTotalTime(GP_AFFECT_LIST& listAffect)
{
	DWORD dwMaxTime = 0;
	GP_AFFECT_LIST_Iterator it = listAffect.begin();
	for ( ; it != listAffect.end(); ++it )
	{
		LPGP_AFFECT pAffectConfig = &*it;
		DWORD dwTime = pAffectConfig->dwStartTime + pAffectConfig->dwLifeTime;
		if ( dwTime > dwMaxTime )
		{
			dwMaxTime = dwTime;
		}
	}
	return dwMaxTime;
}

/*
_AddAffectGP:
目标被击中了，把道具给他加上
*/
void CGPControl::_AddAffectGP(IGPTarget* pTarget, GP_ID id, DWORD dwSrcID)
{
	if ( !pTarget )
	{
		XWaring("_AddAffectGP(): bad arg");
		return;
	}

	/*
	//如果自己被暗算了，上报之
	if ( m_pTargetIter && pTarget == m_pTargetIter->GetHost() )
	{
		CGPNetIntercommn::RequestPropEffectResult(id.dwIndex, 0);
	}
	*/
	

	LPGP_CONFIG pConfig = FindConfig(id.dwID);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return;
	}

	GP_DATA gpData;
	gpData.dwID = id.dwID;
	gpData.dwIndex = id.dwIndex;
	gpData.dwOriginID = id.dwOriginID;
	gpData.dwSrcID = dwSrcID;
	gpData.dwDestID = pTarget->GetID();
	gpData.dwUseTime = GetHighResolutionTime();
	gpData.dwUseCount = 0;
	gpData.dwLifeState=  ELS_Throw;
	gpData.pMoveCtrl = NULL;
	gpData.pConfig = pConfig;
	if ( pConfig->stMoveCtrl.dwMoveType != EMT_None )
	{
		IGPMovementControl* pMoveCtrl = NULL;
		BOOL bRet = CreateGPMovementControl((ENMMOVETYPE)pConfig->stMoveCtrl.dwMoveType, &pMoveCtrl);
		if ( bRet )
		{
			gpData.pMoveCtrl = pMoveCtrl;
		}
		else
		{
			XWaring("CreateGPMovementControl(%d) failed, no movement will be supported", pConfig->stMoveCtrl.dwMoveType);
		}
	}

	GP_AFFECT_LIST& listAffect = pConfig->listAffect;
	GP_AFFECT_LIST_Iterator it = listAffect.begin();
	for ( ; it != listAffect.end(); ++it )
	{
		LPGP_AFFECT pAffectConfig = &*it;
		IGPAffect* pAffect = NULL;
		BOOL bRet = CreateGPAffect((ENMAFFECTID)pAffectConfig->dwID, &pAffect);
		if ( bRet )
		{
			GP_AFFECTDATA gpAffectData;
			gpAffectData.pAffectConfig = pAffectConfig;
			gpAffectData.dwState = EAS_Idle;
			gpAffectData.pAffect = pAffect;
			gpData.listAffect.push_back(gpAffectData);
		}
		else
		{
			XWaring("CreateGPAffect(%d) failed", pAffectConfig->dwID);
		}
	}
	pTarget->AddAffectGP(&gpData);
}

void CGPControl::OnCtrlPress()
{
	BOOL bRet = FALSE;
	{
		if ( !m_bOpEnabled )
		{
			goto KK_EXIT;
		}

		IGPTarget* pHost = m_pTargetIter->GetHost();
		KK_CHECK_POINTER(pHost);
		DWORD dwHostID = pHost->GetID();

		GP_ID_LIST& listID = pHost->GetGPs();
		if ( listID.size() <= 0 )
		{
			goto KK_EXIT;
		}

		m_idCurrentHold = *listID.begin();
		DWORD dwID = m_idCurrentHold.dwID;
		if ( !_CanUse(pHost, dwID) )
		{
			goto KK_EXIT;
		}

		pHost->RemoveGP(dwID);
		LPGP_CONFIG pConfig = FindConfig(dwID);
		if ( !pConfig )
		{
			XWaring("FindConfig(%d) failed", dwID);
			goto KK_EXIT;
		}
		
		if ( pConfig->dwSelectTargetType == ESTT_Select 
			&& m_pAimer != NULL )
		{
			m_pAimer->OnCtrlPress();
			OnThrow(dwID, dwHostID, FALSE);
			goto KK_EXIT;
		}

		OnThrow(dwID, dwHostID);
		DWORD dwIDConv = 0;
		bRet = _ApplyGP(m_idCurrentHold, pHost, NULL, &dwIDConv);
		DWORD dwIndex = m_idCurrentHold.dwIndex > MAX_CUSTOMGPINDEX ? 0 : m_idCurrentHold.dwIndex;
		CGPNetIntercommn::RequestUseProp(m_idCurrentHold.dwID, dwIndex, 0, dwIDConv);
	}
KK_EXIT:
	return;
}

void CGPControl::OnCtrlRelease()
{
	if ( !m_bOpEnabled )
	{
		return;
	}

	if ( m_pAimer )
	{
		m_pAimer->OnCtrlRelease();
	}
}

void CGPControl::OnAimFinished(IGPTarget* pTarget)
{
	//需要指定目标的道具，如果没有瞄准任何目标的情况下，释放道具失败
	if ( !pTarget )
	{
		XWaring("没有瞄准任何目标，道具使用失败罗");
		DWORD dwIndex = m_idCurrentHold.dwIndex > MAX_CUSTOMGPINDEX ? 0 : m_idCurrentHold.dwIndex;
		CGPNetIntercommn::RequestUseProp(m_idCurrentHold.dwID, dwIndex, 0, 0);
		return;
	}

	BOOL bRet = FALSE;
	{
		IGPTarget* pHost = m_pTargetIter->GetHost();
		KK_CHECK_POINTER(pHost);
		DWORD dwHostID = pHost->GetID();

		OnThrow(m_idCurrentHold.dwID, pHost->GetID(), TRUE);

		DWORD dwIDConv = 0;//m_idCurrentHold.dwID;
		_ApplyGP(m_idCurrentHold, pHost, pTarget, &dwIDConv);
		DWORD dwIndex = m_idCurrentHold.dwIndex > MAX_CUSTOMGPINDEX ? 0 : m_idCurrentHold.dwIndex;
		CGPNetIntercommn::RequestUseProp(m_idCurrentHold.dwID, dwIndex, pTarget->GetID(), dwIDConv);
	}
KK_EXIT:
	return;
}

void CGPControl::OnTargetAimed(DWORD dwDestID, BOOL bSet)
{
	if ( m_pAimer )
	{
		m_pAimer->SetAimed(dwDestID, bSet);
	}
}

BOOL CGPControl::_LoadConfig()
{
	CScriptState ls; // = GlobalInterface::m_pScriptState;
	if (0 == ls.DoFile("config/gpconfig.lua") )
	{
		m_ConfigList.clear();
		XTrace("加载了脚本配置文件，开始读取配置...");
		CVarArray varRoot("GPConfig");
		varRoot.GetFromScript(&ls);
		for ( int i = 0; i < varRoot.GetNum(); ++i )
		{
			GP_CONFIG gpConfig;
			CVarTable* pVar = (CVarTable*)varRoot.Element(i);
			if ( pVar )
			{
				_ReadNumFromVar(&gpConfig.dwID, pVar->Element("id"));
				_ReadNumFromVar(&gpConfig.dwParentID, pVar->Element("parentid"));
				_ReadStringFromVar(gpConfig.strName, pVar->Element("name"));
				_ReadNumFromVar(&gpConfig.dwCategory, pVar->Element("category"));
				_ReadNumFromVar(&gpConfig.dwUseType, pVar->Element("usetype"));
				_ReadNumFromVar(&gpConfig.dwAffectType, pVar->Element("affectype"));
				_ReadNumFromVar(&gpConfig.dwBlendMode, pVar->Element("blendmode"));
				_ReadNumFromVar(&gpConfig.bAffectSelf, pVar->Element("affectself"));
				_ReadNumFromVar(&gpConfig.dwCannotUseCondition, pVar->Element("cannotusecondition"));
				_ReadNumFromVar(&gpConfig.dwStopCondition, pVar->Element("stopcondition"));

				_ReadEffectFromVar(pVar, "origineffect", gpConfig.listOriginEffect);
				_ReadEffectFromVar(pVar, "eatteneffect", gpConfig.listEattenEffect);
				_ReadEffectFromVar(pVar, "throweffect", gpConfig.listThrowEffect);
				_ReadEffectFromVar(pVar, "triggereffect", gpConfig.listTriggerEffect);
				_ReadEffectFromVar(pVar, "protecteffect", gpConfig.listProtectEffect);
				_ReadEffectFromVar(pVar, "cureeffect", gpConfig.listCureEffect);

				_ReadMoveCtrlFromVar(pVar, "movectrl", &gpConfig.stMoveCtrl);

				_ReadNumFromVar(&gpConfig.dwStayNode, pVar->Element("staynode"));
				_ReadNumFromVar(&gpConfig.dwStayTime, pVar->Element("staytime"));

				_ReadNumFromVar(&gpConfig.dwTargetType, pVar->Element("targettype"));
				_ReadNumFromVar(&gpConfig.dwSelectTargetType, pVar->Element("selecttargettype"));
				_ReadNumFromVar(&gpConfig.bIgnoreProtect, pVar->Element("ignoreprotect"));

				_ReadNumFromVar(&gpConfig.dwProtectType, pVar->Element("protecttype"));
				_ReadNumFromVar(&gpConfig.dwProtectCount, pVar->Element("protectcount"));
				_ReadNumFromVar(&gpConfig.dwProtectIndex, pVar->Element("protectindex"));
				_ReadNumFromVar(&gpConfig.dwProtectTarget, pVar->Element("protecttarget"));

				_ReadNumFromVar(&gpConfig.dwCureID, pVar->Element("cureid"));

				_ReadBoundFromVar(pVar, "bound", &gpConfig.stBound);
				_ReadPositionFromVar(pVar, "position", &gpConfig.stPosition);
				_ReadNumFromVar(&gpConfig.dwPassCount, pVar->Element("passcount"));

				_ReadNumFromVar(&gpConfig.dwStaticProp, pVar->Element("staticprop"));
				_ReadNumFromVar(&gpConfig.dwNewID, pVar->Element("newid"));
				_ReadNumFromVar(&gpConfig.dwReadyTime, pVar->Element("readytime"));
				_ReadNumFromVar(&gpConfig.dwBirthTime, pVar->Element("birthtime"));
				_ReadNumFromVar(&gpConfig.dwTriggerTime, pVar->Element("triggertime"));

				_ReadStringFromVar(gpConfig.strIconTex, pVar->Element("icontex"));
				_ReadNumFromVar(&gpConfig.dwCameraID, pVar->Element("camera"));

				_ReadAffectsFromVar(pVar, "affects", gpConfig.listAffect);
			}
			m_ConfigList.push_back(gpConfig);
		}
	}
	else
	{
		XTrace("加载脚本文件失败了(config/gpconfig.lua)");
	}
	//读取设置
	NxSceneMgr::Get()->GetScriptVM()->ExecuteScriptFile("Scripts/GPSettings.lua", false);
	//加载配置失败了不影响程序运行，返回TRUE，就当什么道具也没有
	return TRUE;
}

void CGPControl::_UnloadConfig()
{
}

BOOL CGPControl::_InitObjects()
{
	m_bAffectEnabled = TRUE;
	m_bOpEnabled = TRUE;
	BOOL bRet = FALSE;
	if ( !m_pAimer )
	{
		m_pAimer = new CGPAimer;
		KK_CHECK_POINTER(m_pAimer);
		bRet = m_pAimer->Init(this);
	}
	if ( !m_pTargetIter )
	{
		bRet = CreateGPTargetIterator(&m_pTargetIter);
		KK_CHECK_POINTER(m_pTargetIter);
	}
	if ( !m_pColumnMgr )
	{
		m_pColumnMgr = NiNew CGPColumnMgr;
	}

KK_EXIT:
	return bRet;
}

void CGPControl::_UnInitObjects()
{

	if ( m_pAimer )
	{
		m_pAimer->UnInit();
		delete m_pAimer;
		m_pAimer = NULL;
	}
	if ( m_pTargetIter )
	{
		DestroyGPTargetIterator(m_pTargetIter);
		m_pTargetIter = NULL;
	}

	if ( m_pColumnMgr )
	{
		NiDelete m_pColumnMgr;
		m_pColumnMgr = NULL;
	}
}

/*
FindConfig: 
找到对应道具的配置信息
*/
LPGP_CONFIG CGPControl::FindConfig(DWORD id)
{
	GP_CONFIG_LIST_Iterator it = m_ConfigList.begin();
	for ( ; it != m_ConfigList.end(); ++it )
	{
		if ( id == it->dwID)
		{
			return &(*it);
		}
	}
	return NULL;
}

bool IsKeyDownAsyn( int vkey );

/*
_CanUse:
判断目标当前的道具是否可以使用
*/
BOOL CGPControl::_CanUse(IGPTarget* pTarget, DWORD id)
{
	if ( !pTarget )
	{
		XWaring("_CanUse() invalid args");
		return FALSE;
	}
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return FALSE;
	}
	return _IsTrueCondition(pTarget, id, pConfig->dwCannotUseCondition);
}

BOOL CGPControl::_CanStop(IGPTarget* pTarget, DWORD id)
{
	if ( !pTarget )
	{
		XWaring("_CanUse() invalid args");
		return FALSE;
	}
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return FALSE;
	}
	return !_IsTrueCondition(pTarget, id, pConfig->dwStopCondition);
}

BOOL CGPControl::_IsTrueCondition(IGPTarget* pTarget, DWORD id, DWORD dwCondition)
{
	if ( !pTarget )
	{
		XWaring("_IsTrueCondition() bad args");
		return FALSE;
	}
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return FALSE;
	}

	if ( dwCondition == ECUC_None )
	{
		return TRUE;
	}
	if ( dwCondition & ECUC_OneExists )
	{
		GP_DATA_LIST listData = pTarget->GetAffectGPs();
		GP_DATA_LIST_Iterator it = listData.begin();
		for ( ; it != listData.end(); ++it )
		{
			LPGP_DATA pData = &*it;
			if ( pData && (pData->dwID == id ||
				(pConfig->dwCategory != 0
				&& pData->pConfig->dwCategory == pConfig->dwCategory)) )
			{
				return FALSE;
			}
		}
	}
	if ( dwCondition & ECUC_NoAccel )
	{
		if ( !IsKeyDownAsyn(VK_UP) && !IsKeyDownAsyn('w') && !IsKeyDownAsyn('W'))
		{
			return FALSE;
		}
	}
	return TRUE;
}

//启动道具了...
BOOL CGPControl::_ApplyGP(GP_ID id, IGPTarget* pSrcTarget, IGPTarget* pTarget, DWORD* pdwIDConv)
{
	BOOL bRet = FALSE;
	KK_CHECK_POINTER(m_pTargetIter);
	KK_CHECK_POINTER(pSrcTarget);
	{
		DWORD dwDestUin = 0;

		if (pTarget)
		{
			//dwDestUin = pTarget->GetID();
		}

		IGPTarget* pHost = m_pTargetIter->GetHost();
		DWORD dwSrcID = pSrcTarget->GetID();		

		//创建接口实例
		LPGP_CONFIG pConfig = FindConfig(id.dwID);
		if ( !pConfig )
		{
			XWaring("找不到ID为%d的道具配置，无法使用.", id.dwID);
			KK_CHECK_POINTER(pConfig);
		}
		//道具转换
		if(pTarget)
		{
			if (pSrcTarget == pHost)
			{
				GPINFO tagGPInfo = {id.dwID, 0, 0, 0};
				NxPlayMgr::GetPlay(pTarget->GetID())->OnAttackByGP(tagGPInfo);
				if ( tagGPInfo.dwID != id.dwID )
				{
					*pdwIDConv = tagGPInfo.dwID;
				}
				id.dwID = tagGPInfo.dwID;
			}
		}

		//道具生成型的，特殊处理
		if ( pConfig->dwAffectType == EAT_Generator )
		{
			bRet = ProcessGeneratorGP(id, pSrcTarget);
			goto KK_EXIT;
		}

		//如果指定了攻击目标(瞄准的情况)，攻击之，否则，需要获取可以攻击的目标列表
		if ( pTarget )
		{
			_AddAffectGP(pTarget, id, dwSrcID);
			if ( pConfig->bAffectSelf )
			{
				_AddAffectGP(pSrcTarget, id, dwSrcID);
			}
			bRet = TRUE;
			goto KK_EXIT;
		}

		GPTARGET_LIST listTarget = _FetchTargetList(id.dwID, pSrcTarget);
		if ( listTarget.size() > 0 )
		{
			GPTARGET_LIST_Iterator it = listTarget.begin();
			for ( ; it != listTarget.end(); ++it )
			{
				IGPTarget* pTmpTarget = *it;
				_AddAffectGP(pTmpTarget, id, dwSrcID);
			}
		}

		bRet = TRUE;
	}
KK_EXIT:
	return bRet;
}

/*
OnUseGP：
CNotifyUsePropPara消息来了调这个
*/
void CGPControl::OnUseGP(GP_ID id, DWORD dwSrcID, DWORD dwDestID /* = 0 */, bool bSelfDefineTarget /* = false */,IGPTarget * pTarget /* = NULL  */)
{
	//destid有可能是srcid
	XTrace("OnUseGP(id=%d, srcid=%d, destid=%d)", id, dwSrcID, dwDestID);
	
	OnThrow(id.dwID, dwSrcID);
	if ( bSelfDefineTarget && pTarget )
		_ApplyGP(id, GetTargetByID(dwSrcID), pTarget);
	else
		_ApplyGP(id, GetTargetByID(dwSrcID), GetTargetByID(dwDestID));
	return;
}


/*
SetAimed：
让目标处于被瞄准状态
*/
void CGPControl::SetAimed(DWORD dwDestID, BOOL bSet)
{
	if ( m_pAimer )
	{
		m_pAimer->SetAimed(dwDestID, bSet);
	}
}

/*
OnGotten：
得到了道具，给个机会播放吃到的特效
*/
void CGPControl::OnGotten(DWORD id, DWORD dwSrcID)
{
	IGPTarget* pTarget = GetTargetByID(dwSrcID);
	if ( !pTarget )
	{
		XWaring("OnGotten(): cannot find target(%d)", dwSrcID);
		return;
	}
	
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return;
	}

	CSoundMgr::Get()->PlaySound(GPSettings::eatensound);
	NiNodePtr spNode = CGPEffectMgr::Get()->NewEffect(GPSettings::gottenshownode);
	if ( spNode )
	{
		NiAVObjectPtr spTri = spNode->GetObjectByName("Plane");
		if ( spTri )
		{
			NiTexturingPropertyPtr spProperty = NiDynamicCast(NiTexturingProperty, 
				spTri->GetProperty(NiProperty::TEXTURING));
			if ( spProperty )
			{
				NiSourceTexturePtr spTexture = NiSourceTexture::Create(
					NiApplication::ConvertMediaFilename(pConfig->strIconTex.c_str()));
				spProperty->SetBaseTexture(spTexture);

				pTarget->Attach(spNode);
			}
		}
	}
}

/*
OnThrow:
扔出道具，给个机会播放特效之类
bThrow为FALSE是需要瞄准的道具使用前的行为
*/
void CGPControl::OnThrow(DWORD id, DWORD dwSrcID, BOOL bThrow)
{
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("有人(id=%d)使用了不存在的道具(id=%d)", dwSrcID, id);
		return;
	}
	
	IGPTarget* pSrcTarget = GetTargetByID(dwSrcID);

	if ( !pSrcTarget )
	{
		XWaring("GetTargetByID(%d)失败", dwSrcID);
		return;
	}

	//这里要播放一个扔出的动作
	pSrcTarget->PlayAnimation(GPSettings::throwanimation, TRUE);
	if ( bThrow )
	{
		_PlaySpecialEffect(pSrcTarget, pConfig->listThrowEffect);
	}
}

/*
OnTrigger:
道具击中了目标，给个机会播放特效之类
*/
void CGPControl::OnTrigger(LPGP_DATA pData)
{
	LPGP_CONFIG pConfig = FindConfig(pData->dwID);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", pData->dwID);
		return;
	}

	XTrace("道具(%d)击中了目标(%d)", pData->dwID, pData->dwDestID);
	_PlaySpecialEffect(GetTargetByID(pData->dwDestID), pConfig->listTriggerEffect);
}

/*
OnDisbled:
道具被排斥了，给个机会干别的
*/
void CGPControl::OnDisabled(LPGP_DATA pData)
{
}

/*
OnDie:
道具生命期结束了，给个机会干别的
*/
void CGPControl::OnDie(LPGP_DATA pData)
{
}

/*
OnProtect:
道具防御了一次攻击，可能会搞点炫的效果
*/
void CGPControl::OnProtect(DWORD id, DWORD dwSrcID)
{
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return;
	}

	XTrace("道具(%d)防御了一次攻击或者被防御了", id);
	_PlaySpecialEffect(GetTargetByID(dwSrcID), pConfig->listProtectEffect);
}


/*
OnCure:
道具防御了一次攻击，可能会搞点炫的效果
*/
void CGPControl::OnCure(DWORD id, DWORD dwSrcID)
{
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return;
	}

	XTrace("道具(%d)成功治疗", id);
	_PlaySpecialEffect(GetTargetByID(dwSrcID), pConfig->listCureEffect);
}

/*
_PlaySpecialEffect:
播放特效
*/
void CGPControl::_PlaySpecialEffect(IGPTarget* pTarget, GP_SPECIAL_EFFECT_LIST& listEffect)
{
	if ( !pTarget )
	{
		XWaring("_PlaySpecialEffect() bad arg");
		return;
	}

	GP_SPECIAL_EFFECT_LIST_Iterator it = listEffect.begin();
	for ( ; it != listEffect.end(); ++it )
	{
		LPGP_SPECIAL_EFFECT pEffect = (LPGP_SPECIAL_EFFECT)&*it;
		switch( pEffect->dwType )
		{
		case ESE_None:
			{
			}
			break;
		case ESE_Effect:
			{
				NiNodePtr pNode = CGPEffectMgr::Get()->NewEffect(pEffect->dwParam);
				pTarget->Attach(pNode);
			}
			break;
		case ESE_Sound:
			{
				IGPTarget* pHost = m_pTargetIter ? m_pTargetIter->GetHost() : NULL;
				if ( pHost == pTarget )
				{
					CSoundMgr::Get()->PlaySound(pEffect->dwParam, TRUE);
				}
			}
			break;
		case ESE_Animation:
			{
				pTarget->PlayAnimation(pEffect->dwParam, TRUE);
			}
			break;
		}
	}
}

/*
WndProc:
响应Windows消息
*/
STDMETHODIMP CGPControl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		if ( (lParam & ( 1 << 30 )) != 0 )
		{
			break;
		}
		if ( wParam == VK_CONTROL )
		{
			OnCtrlPress();
		}
		if ( wParam >= '0' && wParam <= '9' && FileExists("key") )
		{
			IGPTarget* pHost = m_pTargetIter->GetHost();
			if ( pHost )
			{
				DWORD dwID = wParam - '0';
				if ( dwID == 0 )
				{
					dwID = 10;		
				}
				if ( IsKeyDownAsyn(VK_SHIFT) )
					dwID += 10;
				static s_gpindex = MAX_CUSTOMGPINDEX;
				pHost->AddGP(dwID, ++s_gpindex);
				OnGotten(dwID, pHost->GetID());
			}
		}
		if ( wParam == VK_INSERT )
		{
			_LoadConfig();
		}
		if ( wParam == VK_TAB )
		{
			//先屏蔽掉
			//m_pColumnMgr->UseExchanger();
		}
		break;
	case WM_KEYUP:
		if ( wParam == VK_CONTROL )
		{
			OnCtrlRelease();
		}
		break;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////CGPAimer////////////////////////////////////////

CGPAimer::CGPAimer():
m_bAimming(FALSE),
m_bAimShow(FALSE),
m_ShootScope(ESS_None),
m_pTarget(NULL),
m_pControl(NULL),
m_dwLastTime(0),
m_pShelterIter(NULL),
m_dwTargetTime(0),
m_spBillboard1(NULL),
m_spBillboard2(NULL),
m_pkRotateController(NULL),
m_pkScaleController(NULL),
m_pkMeScaleController(NULL)
{
	for ( int i = 0; i < AIM_MAX_EFFECTS; i++ )
	{
		m_pTextures[i] = NULL;
	}
}

CGPAimer::~CGPAimer()
{
	for ( int i = 0; i < AIM_MAX_EFFECTS; i++ )
	{
		if ( m_pTextures[i] )
		{
			m_pTextures[i] = NULL;
		}
	}
	if ( m_pShelterIter )
	{
		DestroyGPShelterIterator(m_pShelterIter);
		m_pShelterIter = NULL;
	}
	
	assert(m_pControl);
	IGPTargetIterator* pIter = m_pControl->GetTargetIterator();
	assert(pIter);
	for ( int i = 0; i < pIter->GetTargetCount(); i++ )
	{
		IGPTarget* pTarget = pIter->GetTarget(i);
		pTarget->Detach(m_spBillboard1);
		pTarget->Detach(m_spBillboard2);
	}
}

BOOL CGPAimer::Init(CGPControl* pControl)
{
	m_pControl = pControl;
	m_bAimming = FALSE;

	if ( !m_pShelterIter )
	{
		if ( !CreateGPShelterIterator(&m_pShelterIter) )
		{
			XWaring("CreateGPShelterIterator失败，将不会支持迷雾功能");
		}
	}

	_PrepareEffect();
	return TRUE;
}

BOOL CGPAimer::UnInit()
{
	m_bAimming = FALSE;
	return TRUE;
}

void CGPAimer::Update(DWORD dwTime)
{
	if ( !m_bAimming || !m_pControl )
	{
		return;
	}

	IGPTargetIterator* pTargetIter =  m_pControl->m_pTargetIter;
	if ( !pTargetIter )
	{
		return;
	}

	IGPTarget* pHost = pTargetIter->GetHost();
	if ( !pHost )
	{
		XWaring("pTargetIter->GetHost failed");
		return;
	}

	//逐个目标判断跟当前瞄准线的距离
	GPTARGET_LIST	listTarget;
	ENUMSHOOTSCOPE enmDistance = ESS_Far;
	for ( int i = 0; i < pTargetIter->GetTargetCount(); ++i )
	{
		IGPTarget* pTarget = pTargetIter->GetTarget(i);
		if ( !pTarget || pTarget == pHost )	//拿不到或者自己
		{
			continue;
		}

		ENUMSHOOTSCOPE enmScope = _JudgeTargetScope(
									_GetAimWorldOriginPos(pHost), 
									_GetAimWorldDirection(pHost), 
									_GetAimedWorldOriginPos(pTarget));
		switch(enmScope)
		{
		case ESS_Far:
			break;
		case ESS_Mid:
			if ( enmDistance == ESS_Far )
			{
				enmDistance = ESS_Mid;
			}
			break;
		case ESS_Near:
			listTarget.push_back(pTarget);
			if ( enmDistance != ESS_Near )
			{
				enmDistance = ESS_Near;
			}
			break;
		}
	}

	if ( m_dwLastTime == 0 )
	{
		m_dwLastTime = GetHighResolutionTime();
	}
	DWORD dwCurTime = GetHighResolutionTime();
	DWORD dwDeltaTime = dwCurTime - m_dwLastTime;
	m_dwLastTime = dwCurTime;

	IGPTarget* pTargetOld = m_pTarget;
	//累计瞄准时间
	if ( listTarget.size() > 0 )
	{
		if ( m_pTarget )
		{
			GPTARGET_LIST_Iterator it = find(listTarget.begin(), listTarget.end(), m_pTarget);
			if ( it != listTarget.end() )
			{
				m_dwTargetTime += dwDeltaTime;
				//如果对一个目标的瞄准时间超过预定大小，需要切换到下一个目标
				if ( m_dwTargetTime > GPSettings::exchangetime )
				{
					++it;
					//到了最后一个了，从头开始
					if ( it == listTarget.end() )
					{
						it = listTarget.begin();
					}
					m_pTarget = *it;
				}
			}
			else
			{
				m_dwTargetTime = 0;
			}
		}
		else
		{
			m_pTarget = *listTarget.begin();
		}
	}
	else
	{
		m_pTarget = NULL;
	}
	//显示自己的瞄准镜头
	ShowAim(enmDistance, m_pTarget);
	//显示目标身上的瞄准镜头
	//清除老目标上的瞄准标志
	if ( pTargetOld && pTargetOld != m_pTarget )
	{
		//SetAimed(pTargetOld->GetID(), FALSE);
		m_dwTargetTime = 0;
		CGPNetIntercommn::RequestChangeAimState(pTargetOld->GetID(), FALSE);
	}
	if ( m_pTarget && pTargetOld != m_pTarget )
	{
		m_dwTargetTime = 0;
		CGPNetIntercommn::RequestChangeAimState(m_pTarget->GetID(), TRUE);
	}
	if ( m_pTarget )
	{
		//SetAimed(m_pTarget->GetID());
	}
}

//取得当前正在瞄准的目标
IGPTarget*	CGPAimer::GetTarget()
{
	return m_pTarget;
}

void CGPAimer::OnCtrlPress()
{
	_StartAim();
}

void CGPAimer::OnCtrlRelease()
{
	_EndAim();
}

//开始瞄准了
void CGPAimer::_StartAim()
{
	m_bAimming = TRUE;
	CGPNetIntercommn::RequestChangeAimState(0, 1);//要发1
}

//结束瞄准
void CGPAimer::_EndAim()
{
	if ( m_bAimming && m_pControl )
	{
		IGPTarget* pHost = NULL;
		IGPTargetIterator* it = m_pControl->m_pTargetIter;
		if ( it )
		{
			 pHost = it->GetHost();
		}
		if ( pHost )
		{
			ShowAim(ESS_Far, NULL, FALSE);
		}
		if ( m_pTarget )
		{
			SetAimed(m_pTarget->GetID(), FALSE);
			CGPNetIntercommn::RequestChangeAimState(m_pTarget->GetID(), FALSE);
		}

		m_pControl->OnAimFinished(m_pTarget);
		m_bAimming = FALSE;
		m_pTarget = NULL;
	}
}

//判断点跟直线的相对位置
CGPAimer::ENUMSHOOTSCOPE CGPAimer::_JudgeTargetScope(NiPoint3 kOrigin, NiPoint3 kDir, NiPoint3 kTarget)
{
	//算点到直线的距离
	double d = abs(GPMath::ComputeDistanceFromPointToLine(kOrigin, kDir, kTarget));
	//先不考虑障碍和迷雾

	//再算夹角
	NiPoint3 kDirTarget = kTarget - kOrigin;
	double fAngel = GPMath::ComputeAngleBetweenLines(kDir, kDirTarget);

	if ( d <= GPSettings::nearDistance && fAngel >= 0 && fAngel <= GPSettings::nearAngel )
	{
		return ESS_Near;
	}
	else if ( d < GPSettings::farDistance && fAngel >= 0 && fAngel <= GPSettings::farAngel )
	{
		return ESS_Mid;
	}
	else
	{
		return ESS_Far;
	}
}

void CGPAimer::_PrepareEffect()
{
	const char* pszMapName[] = {
			"Textures/aim1.tga",
			"Textures/aim2.tga",
			"Textures/aim3.tga",
	};
	
	if ( !m_spBillboard1 )
	{
		m_spBillboard1 = CGPRender::CreateBillboard(GPSettings::aimboardx, GPSettings::aimboardx);
		m_spBillboard1->Update(GetMainApp()->GetAccumTime());
	}
	if ( !m_spBillboard2 )
	{
		m_spBillboard2 = CGPRender::CreateBillboard(GPSettings::aimedboardx, GPSettings::aimedboardx);
		m_spBillboard2->Update(GetMainApp()->GetAccumTime());
	}
	KK_CHECK_POINTER(m_spBillboard1);
	KK_CHECK_POINTER(m_spBillboard2);
	
	if ( !m_pkRotateController )
	{
		m_pkRotateController = NiNew RotateController(GPSettings::aimrotspeed, NiPoint3(1, 0, 0));
		m_pkRotateController->SetCycleType(NiTimeController::LOOP);
		m_pkRotateController->SetTarget(m_spBillboard1->GetAt(1));
		m_pkRotateController->SetActive(false);
	}
	if ( !m_pkScaleController )
	{
		m_pkScaleController = NiNew ScaleController(GPSettings::aimscalespeed, 0.4, 0.20);
		m_pkScaleController->SetCycleType(NiTimeController::LOOP);
		m_pkScaleController->SetTarget(m_spBillboard1);
		m_pkScaleController->SetActive(false);
	}
	if ( !m_pkMeScaleController )
	{
		m_pkMeScaleController = NiNew ScaleController(GPSettings::aimscalespeed, 0.4, 0.20);
		m_pkMeScaleController->SetCycleType(NiTimeController::LOOP);
		m_pkMeScaleController->SetTarget(m_spBillboard2);
	}
	
	for ( int i = 0; i < AIM_MAX_EFFECTS; ++i )
	{
		if ( !m_pTextures[i] )
		{
			m_pTextures[i] = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(pszMapName[i]));
		}
		assert(m_pTextures[i]);
	}

KK_EXIT:
	return;
}

//在目标上显示/隐藏瞄准图标
void CGPAimer::SetAimed(DWORD dwDestID, BOOL bSet)
{
	if ( !m_pControl || dwDestID == 0 )
	{
		return;
	}

	IGPTarget* pTarget = m_pControl->GetTargetByID(dwDestID);
	if ( !pTarget )
	{
		XWaring("GetTargetByID(%d) failed", dwDestID);
		return;
	}

	BOOL bSwitchToSet = FALSE;
	if ( bSet != pTarget->SetAimed(bSet) )
	{
		if ( bSet )
		{
			bSwitchToSet = TRUE;
			CSoundMgr::Get()->PlaySound(1);
		}
		else
		{
			CSoundMgr::Get()->StopSoundByID(1);
		}
	}
	
	if ( bSet )
	{
		if ( bSwitchToSet && pTarget == m_pControl->GetTargetIterator()->GetHost() )
		{
			XTrace("SetAimed(%d, %d)", dwDestID, bSet);
			NiAVObjectPtr pkObject = NiDynamicCast(NiAVObject, m_spBillboard2->GetAt(1));
			KK_CHECK_POINTER(pkObject);

			NiTexturingPropertyPtr spTextureProperty = NiDynamicCast(NiTexturingProperty, pkObject->GetProperty(NiProperty::TEXTURING));
			KK_CHECK_POINTER(spTextureProperty);
			spTextureProperty->SetBaseTexture(m_pTextures[(int)ESS_Near]);

			pTarget->Attach(m_spBillboard2);
			NiPoint3 kLocalPos(GPSettings::aimedmex, GPSettings::aimedmey, GPSettings::aimedmez);
			m_spBillboard2->SetTranslate(kLocalPos.x, kLocalPos.y, kLocalPos.z);
			m_spBillboard2->SetScale(0.5);
		}
	}
	else
	{
		pTarget->Detach(m_spBillboard2);
	}

KK_EXIT:
	return;
}

//显示/隐藏自己的瞄准境图标
void CGPAimer::ShowAim(ENUMSHOOTSCOPE Scope, IGPTarget* pTarget, BOOL bShow)
{
	{
		KK_CHECK_POINTER(m_pControl);
		KK_CHECK_POINTER(m_pControl->m_pTargetIter);
		IGPTarget* pHost = m_pControl->m_pTargetIter->GetHost();
		KK_CHECK_POINTER(pHost);
		KK_CHECK_POINTER(m_spBillboard1);

		BOOL bSwitchToShow = FALSE;
		if ( bShow )
		{
			if ( m_ShootScope != Scope )
			{
				bSwitchToShow = TRUE;
			}
			{
				NiAVObjectPtr pkObject = NiDynamicCast(NiAVObject, m_spBillboard1->GetAt(1));
				KK_CHECK_POINTER(pkObject);

				NiTexturingPropertyPtr spTextureProperty = NiDynamicCast(NiTexturingProperty, pkObject->GetProperty(NiProperty::TEXTURING));
				KK_CHECK_POINTER(spTextureProperty);
				spTextureProperty->SetBaseTexture(m_pTextures[(int)Scope]);

				pHost->Attach(m_spBillboard1);
				if ( Scope == ESS_Near )
				{
					pHost->Attach(m_spBillboard1);
					NiPoint3 kOrigin = pHost->GetPos();
					NiPoint3 kTargetPos = _GetAimedWorldOriginPos(pTarget);
					NiPoint3 kCameraPos = CLevelManager::Get()->GetCamera()->GetTranslate();
					NiPoint3 kDir = kTargetPos - kCameraPos;
					kDir.Unitize();
					NiPoint3 kPoint = GPMath::ComputerNearestPointFromPointToLine(kTargetPos, -kDir, kOrigin);
					NiTransform kTransform = pHost->GetTransform();
					NiTransform kTransformInvert;
					kTransform.Invert(kTransformInvert);
					NiPoint3 kPointLoc = kTransformInvert * kPoint;
					m_spBillboard1->SetTranslate(kPointLoc.x, kPointLoc.y, kPointLoc.z);
					//m_spBillboard1->SetScale(0.5);
					m_pkRotateController->SetActive(false);
					m_pkScaleController->SetActive(true);
				}
				else
				{
					NiPoint3 kOffset = _GetAimLocalOriginPos(pHost);
					m_spBillboard1->SetTranslate(kOffset.x, kOffset.y, kOffset.z);
					m_spBillboard1->SetScale(1.0);
					m_pkRotateController->SetActive(true);
					m_pkScaleController->SetActive(false);
				}
			}
		}
		else
		{
			m_bAimShow = FALSE;
			pHost->Detach(m_spBillboard1);
			CSoundMgr::Get()->StopSoundByID(2);
		}
		m_bAimShow = bShow;
		m_ShootScope = Scope;
		if ( bSwitchToShow )
		{
			CSoundMgr::Get()->PlaySound(2);
		}
	}
KK_EXIT:
	return;
}

//取得瞄准境相对于自己的位置
NiPoint3 CGPAimer::_GetAimLocalOriginPos(IGPTarget* pTarget)
{
	return NiPoint3(GPSettings::aimoffsetx, GPSettings::aimoffsety, GPSettings::aimoffsetz);
}

//取得瞄准境的世界坐标
NiPoint3 CGPAimer::_GetAimWorldOriginPos(IGPTarget* pTarget)
{
	if ( !pTarget )
	{
		XWaring("_GetAimWorldOriginPos(): bad arg");
		return NiPoint3(0, 0, 0);
	}
	NiPoint3 kLocalPos = _GetAimLocalOriginPos(pTarget);
	NiTransform kTrans = pTarget->GetTransform();
	NiPoint3 kWorldPos = kTrans * kLocalPos;
	return kWorldPos;
}

NiPoint3 CGPAimer::_GetAimedLocalOriginPos(IGPTarget*)
{
	return NiPoint3(GPSettings::aimedoffsetx, GPSettings::aimedoffsety, GPSettings::aimedoffsetz);
}

NiPoint3 CGPAimer::_GetAimedWorldOriginPos(IGPTarget* pTarget)
{
	if ( !pTarget )
	{
		XWaring("_GetAimedWorldOriginPos(): bad arg");
		return NiPoint3(0, 0, 0);
	}
	NiPoint3 kLocalPos = _GetAimedLocalOriginPos(pTarget);
	NiTransform kTrans = pTarget->GetTransform();
	NiPoint3 kWorldPos = kTrans * kLocalPos;
	return kWorldPos;
}
//取得瞄准方向（等同于镜头方向）
NiPoint3 CGPAimer::_GetAimWorldDirection(IGPTarget* pTarget)
{
	if ( !pTarget )
	{
		XWaring("_GetAimWorldDirection(): bad arg");
		return NiPoint3(1, 0, 0);
	}
	NiCamera* pCamera = NULL;
	if ( CLevelManager::Get() )
	{
		pCamera = CLevelManager::Get()->GetCamera();
	}
	if ( !pCamera )
	{
		XWaring("GetCameraController()->GetCamera failed");
		return NiPoint3(1, 0, 0);
	}

	/*
#if defined(_DEBUG) || defined(DEBUG)
	NiPoint3 kLoc = pCamera->GetWorldLocation();
	NiPoint3 kRight = pCamera->GetWorldRightVector();
	NiPoint3 kUp = pCamera->GetWorldUpVector();
#endif
	NiPoint3 kForward = pTarget->GetForward();
	kForward.Unitize();
	NiPoint3 kDir = pCamera->GetWorldDirection();
	kDir.Unitize();
	return kDir;
	*/
	NiPoint3 kCameraPos = pCamera->GetWorldLocation();
	NiPoint3 kAimPos = _GetAimWorldOriginPos(pTarget);
	NiPoint3 kDir = kAimPos - kCameraPos;
	kDir.Unitize();
	return kDir;
	/*
	NiPoint3 kOriginWorld = _GetAimWorldOriginPos(pTarget);
	float fx = 0.0, fy = 0.0;
	pCamera->WorldPtToScreenPt(kOriginWorld, fx, fy);
	NiPoint3 kOrigin, kDir;
	pCamera->WindowPointToRay(fx, fy, &kOrigin, &kDir);
	return kDir;
	*/
	/*
	const float* pMatrixData = pCamera->GetWorldToCameraMatrix();
	D3DXMATRIX kMatWC(pMatrixData[0], pMatrixData[1], pMatrixData[2], pMatrixData[3],
				pMatrixData[4], pMatrixData[5], pMatrixData[6], pMatrixData[7],
				pMatrixData[8], pMatrixData[9], pMatrixData[10], pMatrixData[11],
				pMatrixData[12], pMatrixData[13], pMatrixData[14], pMatrixData[15]
				);
	D3DXMATRIX kMatCW;
	float fDeterminant = 0;
	D3DXMatrixInverse(&kMatCW, &fDeterminant, &kMatWC);
	D3DXVECTOR4 kCameraDir(1.0, 0, 0, 0);
	D3DXVECTOR4 kDirWorld;
	D3DXVec4Transform(&kDirWorld, &kCameraDir, &kMatCW);
	return NiPoint3(kDirWorld.x, kDirWorld.y, kDirWorld.z);
	*/
}
