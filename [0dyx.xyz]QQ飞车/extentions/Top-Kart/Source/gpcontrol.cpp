////////////////////////////////////////////////
//     FileName:    gpcontrol.cpp
//     Author:      Fish
//     Date:        2007-1-18 14:37:50
//     Description: ���������߼�ʵ���ļ�
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

	//������׼��
	if ( m_pAimer )
	{
		m_pAimer->Update(dwTime);
	}

	//ѭ������ÿ�����󣬸���֮
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
	//����û�ж���������ĵ��ߣ�ͨ���ڵ����ϣ�
	_UpdateOrphans();
}

/*
_UpdateTarget:
���µ���Ŀ��
*/
void CGPControl::_UpdateTarget(IGPTarget* pTarget)
{
	if ( !pTarget )
	{
		return;
	}

	DWORD dwNow = GetHighResolutionTime();
	GP_DATA_LIST& listData = pTarget->GetAffectGPs();

	//Ŀ��û�����κε��ߣ����ü�������
	if ( listData.size() <= 0 )
	{
		return;
	}

	//һ���˿������˶�����ߣ������Լ��õķ����͵���
	GP_DATA_LIST_Iterator it = listData.begin();	
	for ( ; it != listData.end(); ++it )
	{
		//��ǰ��������
		LPGP_DATA pData = (LPGP_DATA)&*it;	
		//��ǰ���ߵ�����
		LPGP_CONFIG pConfig = FindConfig(pData->dwID);	
		if ( !pConfig )
		{
			XWaring("FindConfig(%d) failed", pData->dwID);
			continue;
		}

		//��ǰ���ߵ������б�
		GP_AFFECTDATA_LIST& listAffectData = pData->listAffect;	
		DWORD dwOriginTime = pData->dwUseTime;
		DWORD dwElapse = dwNow - dwOriginTime;
		DWORD dwReadyTime = pConfig->dwReadyTime;
		DWORD dwTriggerTime = pConfig->dwTriggerTime;

		//�˶��켣
		_UpdateGPMove(pData, dwElapse);

		//���ʱ���Ƿ��������
		if ( dwElapse < dwReadyTime )	
		{
		}
		//������ڻ��й�����
		else if ( dwElapse < dwReadyTime + dwTriggerTime )
		{
			_EnsureLifeState(pData, ELS_Trigger);
		}
		//����ǻ��к��ˣ�����ǰ��
		else
		{
			if ( _CanStop(pTarget, pData->dwID) )
			{
				XTrace("��ֹ�������ˣ����ϱ�־");
				pData->dwLifeState = ELS_Die;
			}

			//һ�����ߣ���Ӧ���Affect
			GP_AFFECTDATA_LIST_Iterator itAffectData = listAffectData.begin();
			for ( ; itAffectData != listAffectData.end(); ++itAffectData )
			{
				//��ǰ���ߵĵ�ǰ��������
				LPGP_AFFECTDATA pAffectData = (LPGP_AFFECTDATA)&*itAffectData; 
				//��ǰ���ߵĵ�ǰ��������
				LPGP_AFFECT pAffectConfig = pAffectData->pAffectConfig;	
				DWORD dwTimeBegin = dwReadyTime + dwTriggerTime + pAffectConfig->dwStartTime;
				DWORD dwTimeEnd = dwTimeBegin + pAffectConfig->dwLifeTime;
				if ( dwElapse < dwTimeBegin )	//�ֵ�����������
				{
					//���ʱ���Ѿ����˷������ˣ������û��Trigger�Ļ����ô�����
					if ( ELS_Trigger != _EnsureLifeState(pData, ELS_Trigger) 
						|| pConfig->dwTriggerTime != 0 )
					{	
						//���еĻ�����Ҫ�ж϶�������Ƿ񻥳�
						_UpdateBlendState(pTarget, listData, it);
					}
				}
				else if ( dwElapse < dwTimeEnd )
				{
					//���ʱ���Ѿ����˷������ˣ������û��Trigger�Ļ����ô�����
					if ( ELS_Trigger != _EnsureLifeState(pData, ELS_Trigger)
						|| pConfig->dwTriggerTime != 0 )
					{
						//���еĻ�����Ҫ�ж϶�������Ƿ񻥳�
						_UpdateBlendState(pTarget, listData, it);
					}

					//�����ڱ�Affect���÷�Χ��
					//ֻ��ELS_Trigger״̬�Ĳ�������������Ϊ��Щ���߻ᱻ�ų����ΪELS_Disable
					if ( m_bAffectEnabled == TRUE
						&& pData->dwLifeState == ELS_Trigger 
						&& !pAffectData->pAffect->IsPlaying() )
					{
						XTrace("��ʼ��������(%d)��Ч��(%d)", pData->dwID, pAffectConfig->dwID);
						//�ϱ��������������ˣ�0��ʾ�����õ���(ֻ��Ҫ�ϱ�һ��)(�Լ����ϱ�)
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
						XTrace("���ڱ���ĵ����ų⣬ֹͣ����(%d)��Ч��(%d)", pData->dwID, pAffectConfig->dwID);
						pAffectData->pAffect->Stop();
						pAffectData->dwState = EAS_Idle;
					}
					else if ( pData->dwLifeState == ELS_Die 
						&& pAffectData->pAffect->IsPlaying() )
					{
						XTrace("�������������㣬ֹͣ����(%d)��Ч��(%d)", pData->dwID, pAffectConfig->dwID);
						pAffectData->pAffect->Stop();
						pAffectData->dwState = EAS_Idle;
					}
					//ֻ�����������õ�ʱ��Ÿ���
					if ( pAffectData->pAffect->IsPlaying() )
					{
						pAffectData->pAffect->Update(dwElapse - dwTimeBegin);
					}
				}
				//���ʱ����߸��ս�������������
				else // dwElapse >= dwTimeEnd
				{
					//��Affect���ս���
					if ( pAffectData->pAffect->IsPlaying() )
					{
						XTrace("ֹͣ�˵���(%d)��Ч��(%d)", pData->dwID, pAffectConfig->dwID);
						pAffectData->pAffect->Stop();
					}

					//�������Affect��ʱ�䶼��ʱ�ˣ���ʱ��ɾ�����������
					if ( dwElapse >= dwReadyTime + dwTriggerTime + _GetAffectsTotalTime(pConfig->listAffect) 
						&& pData->dwLifeState != ELS_Die )
					{
						XTrace("���һ�����õ�ʱ��Ҳ���ˣ����ϱ�־�Ա��´η����ʱ��ɾ��������߰�");
						pData->dwLifeState = ELS_Die;
					}
				
				}
			}
	
		}
	}

	//ɾ�����ڵĵ��ߣ�һ��ɾ��һ�����Է�����ָ�뷢����
	it = listData.begin();
	for ( ; it != listData.end(); ++it )
	{
		LPGP_DATA pData = (LPGP_DATA)&*it;
		LPGP_CONFIG pConfig = pData->pConfig;
		if ( pData->dwLifeState == ELS_Die ||
			pData->dwLifeState == ELS_Disbled )
		{
			//ֹͣ��������
			GP_AFFECTDATA_LIST& listAffectData = pData->listAffect;
			GP_AFFECTDATA_LIST_Iterator itAffectData = listAffectData.begin();
			for ( ; itAffectData != listAffectData.end(); ++itAffectData )
			{
				//��ǰ���ߵĵ�ǰ��������
				LPGP_AFFECTDATA pAffectData = (LPGP_AFFECTDATA)&*itAffectData; 
				if ( pAffectData->pAffect->IsPlaying() )
				{
					pAffectData->pAffect->Stop();
				}
			}

			//�ϱ��������˵���û�в���Ч�������������߱��ų⣩,1��ʾδ������
			if ( pData->pConfig->dwAffectType == EAT_Attack &&
				pData->dwLifeState == ELS_Disbled 
				&& GetTargetByID(pData->dwDestID) == m_pTargetIter->GetHost() )
			{
				CGPNetIntercommn::RequestPropEffectResult(pData->dwID, pData->dwIndex, EPER_OUTOFATTACK, 0);
			}

			XTrace("�´���һ��ɾ���˵���(%d)������", pData->dwID);
			pTarget->RemoveAffectGP(pData->dwIndex);
			break;
		}
	}
	return;
}

/*
_EnsureLifeState:
���״̬��Ҫ�л����ض�״̬����Ҫ����һ����·�߹��򣬸ô�����Ҫ����
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
�����˶��켣,dwTime���Ե�������ʱ�䵽���ڵ�ʱ����
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
			XTrace("��������(%d)�����й켣", pData->dwID);
			pMoveCtrl->SetTarget(GetTargetByID(pData->dwSrcID), GetTargetByID(pData->dwDestID));
			GP_POSITION& kSrcPos = pConfig->stMoveCtrl.stSrcPosition;
			GP_POSITION& kDestPos = pConfig->stMoveCtrl.stDestPosition;
			pMoveCtrl->SetPos(NiPoint3(kSrcPos.x, kSrcPos.y, kSrcPos.z), NiPoint3(kDestPos.x, kDestPos.y, kDestPos.z));
			pMoveCtrl->SetParam(CGPEffectMgr::Get()->NewEffect(pMoveInfo->dwMoveNode), pMoveInfo->dwLifeTime);
			pMoveCtrl->Start();
		}
		else if ( pMoveCtrl->IsPlaying() && dwTime >= pMoveInfo->dwStartTime + pMoveInfo->dwLifeTime )
		{
			XTrace("ֹͣ�˵���(%d)�����й켣", pData->dwID);			
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
����������ߣ��ڵ��ϵģ������㽶Ƥ����ˮ�ݡ�����
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
		//����֮
		if ( dwElapse >= pConfig->dwBirthTime )
		{
			CGPPoint* pPoint = (CGPPoint*)pData->dwData;
			if ( pPoint && !pPoint->GetActive() )
			{
				pPoint->SetActive(true);
				//��Ҫ�Ļ�����ʾ���Ӿ�ͷ
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
		//ɾ�����ڵĵ��ߣ�һ��ɾ��һ��
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
ɾ���������
*/
void CGPControl::_RemoveOrphan(LPGP_DATA pData)
{
	LPGP_CONFIG pConfig = FindConfig(pData->dwID);
	CGPPoint* pPoint = (CGPPoint*)pData->dwData;
	if ( pPoint )
	{
		pPoint->SetActive(false);
		//��Ҫ�Ļ������ټ��Ӿ�ͷ
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
ͨ����������������״̬
����ų�ĵ��߱�����״̬ELS_Disbled
*/
void CGPControl::_UpdateBlendState(IGPTarget* pTarget, GP_DATA_LIST& listData, GP_DATA_LIST_Iterator& itTarget)
{
	//ֻ��һ�����߲���ҪBlend
	if ( listData.size() <= 1 )
	{
		return;
	}

	//һ���˿������˶�����ߣ������Լ��õķ����͵���
//	GP_DATA_LIST_Iterator it = listData.begin();	
//	for ( ; it != listData.end(); ++it )
	do
	{
		GP_DATA_LIST_Iterator it = itTarget;
		//��ǰ��������
		LPGP_DATA pData = (LPGP_DATA)&*it;	
		//��ǰ���ߵ�����
		LPGP_CONFIG pConfig = FindConfig(pData->dwID);	
		if ( !pConfig )
		{
			XWaring("FindConfig(%d) failed", pData->dwID);
			break;
		}

		//���Ƕ������ͬʱ���õ���������ҵ�ǰ���ǵ�һ�����ߣ���Ҫ��ǰ�����Blend
		//XTrace("�����������ͬʱ�����ˣ�����С�Ĵ���");
		GP_DATA_LIST_Iterator itDataTmp = listData.begin();
		for ( ; itDataTmp != it; ++itDataTmp )
		{
			LPGP_DATA pDataTmp = (LPGP_DATA)&*itDataTmp;
			//ֻ��ǰһ�����߻������õ�ʱ�����ҪBlend
			if ( pDataTmp->dwLifeState == ELS_Trigger )
			{
				LPGP_CONFIG pConfigTmp = FindConfig(pDataTmp->dwID);
				if ( !pConfigTmp )
				{
					XWaring("FindConfig(%d) failed", pDataTmp->dwID);
					continue;
				}
				/*�����͵���
				��Ҫ����
				1��Disable����Ĺ����͵���
				2�����������ﵽ���޵�ʱ���Զ�����
				*/
				if ( pConfigTmp->dwAffectType == EAT_Protect )
				{
					switch ( pConfigTmp->dwProtectType )
					{
					case EPT_AllAttack:
						if ( pConfig->dwAffectType == EAT_Attack 
							&& pData->dwLifeState != ELS_Disbled )
						{
							XTrace("��ǰ��һ�������͵ĵ�����ֹ��");
							pDataTmp->dwUseCount++;
							pData->dwLifeState = ELS_Disbled;
							OnProtect(pDataTmp->dwID, pTarget->GetID());
							OnProtect(pData->dwID, pTarget->GetID());
						}
						break;
					case EPT_All:
						if ( pData->dwLifeState != ELS_Disbled )
						{
							XTrace("��ǰ��һ�������͵ĵ�����ֹ��");
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
							XTrace("��ǰ��һ�������͵ĵ�����ֹ��");
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
						XTrace("��һ�������͵ĵ���(%d)�;öȱ�Ϊ0�ˣ���ɾ����", pDataTmp->dwID);
						pDataTmp->dwLifeState = ELS_Die;
					}
				}
				/*�����͵���
				��Ҫ����
				1����ϲ���
				*/
				else if ( pConfigTmp->dwAffectType == EAT_Attack )
				{
					//����������͵���
					if ( pConfig->dwAffectType == EAT_Cure )
					{
						if ( pConfigTmp->dwID == pConfig->dwCureID ) 
						{
							if ( pDataTmp->dwLifeState == ELS_Trigger )
							{
								XTrace("ǰ��һ������%d������������", pConfigTmp->dwID);
								pDataTmp->dwLifeState = ELS_Disbled;
								pData->dwLifeState = ELS_Die;
								OnCure(pData->dwID, pTarget->GetID());
							}
						}
					}
					else
					{
						DWORD dwBlendMode = pConfigTmp->dwBlendMode;
						//���ǰ���ǹ����͵��ߣ����Ƿǹ����͵��ߣ����Ҷ���ʹ�ã����ӹ����͵��ߵ�������
						if ( pConfig->dwAffectType != EAT_Attack )
						{
							dwBlendMode = EBM_AddNext;
						}

						switch ( dwBlendMode )
						{
						case EBM_UseNext:
							if ( pData->dwLifeState != ELS_Trigger )
							{
								XTrace("������̫���ˣ�ǰ����������͵ĵ��߰�λ���ø�����");
								pDataTmp->dwLifeState = ELS_Disbled;
								pData->dwLifeState = ELS_Trigger;
							}
							break;
						case EBM_AddNext:
							if ( pData->dwLifeState != ELS_Trigger )
							{
								XTrace("���ã�ǰ����������͵ĵ��߲����ų���");
								pData->dwLifeState = ELS_Trigger;
							}
							break;
						case EBM_IgnoreNext:
							if ( pConfig->dwBlendMode == EBM_ForceReplaceOld )
							{
								if ( pData->dwLifeState != ELS_Trigger )
								{
									XTrace("������ǰ��ĵ����ų��ң������ҿ���ǿ�У�����");
									pDataTmp->dwLifeState = ELS_Disbled;
									pData->dwLifeState = ELS_Trigger;
								}
							}
							else
							{
								if ( pData->dwLifeState != ELS_Disbled )
								{
									XTrace("���ã� �ұ�ǰ��ĵ����ų���");
									pData->dwLifeState = ELS_Disbled;
								}
							}
							break;
						case EBM_ForceReplaceOld:
							break;
						}
					}
				}

				//���������͵�
				else if ( pConfigTmp->dwAffectType == EAT_Generator )
				{
				}
				else
				{
					XWaring("��֧�ֵĵ�������");
				}
			}
		}
	}while(0);
}

/*
ToggleAffect:
�򿪻�ر����ÿ��أ�������ÿ��ر��رգ�����Ч��������ֹ
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
ToggleOp��
�򿪻�رղ������أ�����������ر��رգ����в���������ֹ
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
�����µĵ��ߣ����������͵���ʹ�ã�
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
		//���̫���Ļ�����ֱ���ó���λ����
		if ( abs(kOffset.x) > 0.000001 )
		{
			pLinks->GetForwardPoint(kNewPos, kNewDir, kPos, kOffset.x);
			//			CLinks * pLinks = NxSceneMgr::Get()->GetActiveSceneLevel()->GetLinks();
			//			CCheckPoint * pChkPt = pLinks->GetLastRightCheckPoint(false);
			//			kNewDir = CKartStatusManager::GetRightDir(pChkPt, pLinks->GetCorrectRunDir());
		}
		if ( bFind )
		{
			XTrace("�ҵ������ɵ㣬���õ���(%d)", dwNewID);
			CGPPointMgr::Param kParam;
			kParam.m_fRadius = kBound.x;
			kParam.m_kRotate = NiMatrix3::IDENTITY;
			//	�޸���:	wincoqin
			//  ʱ��:	2007/02/09
			//  Ŀ��:	�޸���ʾ����̬�;�ͷ�������ݵľ�ͷ��			
			kParam.m_kRotate.SetCol(0, kNewDir.x, kNewDir.y, kNewDir.z);
			NiPoint3 kRightDir = NiPoint3::UNIT_Z.Cross(kNewDir);
			kRightDir.Unitize();
			kParam.m_kRotate.SetCol(1, kRightDir);
			kParam.m_kRotate.SetCol(2, NiPoint3::UNIT_Z);
			// �޸Ľ���
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
			XWaring("�ڵ�ǰλ���޷��ҵ����ɵ㣬�޷����õ���");
		}
	}
	return TRUE;
}

/*
_FindOrphan:
ͨ��ID�ҵ�һ����Ӧ���������
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
������ײ�ص���ײ�������ϵĵ��ߣ�
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
		//���ﲻ���ϱ��ˣ�Update����ͳһ����
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
ͨ��ID�ҵ�Ŀ��
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
ͨ������ȡ��Target
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
�Ѽ����Ŀ��
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

	//���������͵ģ�ֱ�Ӵ���
	if ( pConfig->dwAffectType == EAT_Generator )
	{
		goto KK_EXIT;	//�ʵ�����goto�Գ������кô��ģ��в�ͬ�����ӭ̽��
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
					XTrace("���ֵ���ͬʱ�����õ��Լ��ģ��ܿ����Ǵ�����");
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
���Ŀ�����Լ������ڵ�����
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
ȡ�õ��������б��ܵ�ʱ��
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
Ŀ�걻�����ˣ��ѵ��߸�������
*/
void CGPControl::_AddAffectGP(IGPTarget* pTarget, GP_ID id, DWORD dwSrcID)
{
	if ( !pTarget )
	{
		XWaring("_AddAffectGP(): bad arg");
		return;
	}

	/*
	//����Լ��������ˣ��ϱ�֮
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
	//��Ҫָ��Ŀ��ĵ��ߣ����û����׼�κ�Ŀ�������£��ͷŵ���ʧ��
	if ( !pTarget )
	{
		XWaring("û����׼�κ�Ŀ�꣬����ʹ��ʧ����");
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
		XTrace("�����˽ű������ļ�����ʼ��ȡ����...");
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
		XTrace("���ؽű��ļ�ʧ����(config/gpconfig.lua)");
	}
	//��ȡ����
	NxSceneMgr::Get()->GetScriptVM()->ExecuteScriptFile("Scripts/GPSettings.lua", false);
	//��������ʧ���˲�Ӱ��������У�����TRUE���͵�ʲô����Ҳû��
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
�ҵ���Ӧ���ߵ�������Ϣ
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
�ж�Ŀ�굱ǰ�ĵ����Ƿ����ʹ��
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

//����������...
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

		//�����ӿ�ʵ��
		LPGP_CONFIG pConfig = FindConfig(id.dwID);
		if ( !pConfig )
		{
			XWaring("�Ҳ���IDΪ%d�ĵ������ã��޷�ʹ��.", id.dwID);
			KK_CHECK_POINTER(pConfig);
		}
		//����ת��
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

		//���������͵ģ����⴦��
		if ( pConfig->dwAffectType == EAT_Generator )
		{
			bRet = ProcessGeneratorGP(id, pSrcTarget);
			goto KK_EXIT;
		}

		//���ָ���˹���Ŀ��(��׼�����)������֮��������Ҫ��ȡ���Թ�����Ŀ���б�
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
OnUseGP��
CNotifyUsePropPara��Ϣ���˵����
*/
void CGPControl::OnUseGP(GP_ID id, DWORD dwSrcID, DWORD dwDestID /* = 0 */, bool bSelfDefineTarget /* = false */,IGPTarget * pTarget /* = NULL  */)
{
	//destid�п�����srcid
	XTrace("OnUseGP(id=%d, srcid=%d, destid=%d)", id, dwSrcID, dwDestID);
	
	OnThrow(id.dwID, dwSrcID);
	if ( bSelfDefineTarget && pTarget )
		_ApplyGP(id, GetTargetByID(dwSrcID), pTarget);
	else
		_ApplyGP(id, GetTargetByID(dwSrcID), GetTargetByID(dwDestID));
	return;
}


/*
SetAimed��
��Ŀ�괦�ڱ���׼״̬
*/
void CGPControl::SetAimed(DWORD dwDestID, BOOL bSet)
{
	if ( m_pAimer )
	{
		m_pAimer->SetAimed(dwDestID, bSet);
	}
}

/*
OnGotten��
�õ��˵��ߣ��������Ქ�ųԵ�����Ч
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
�ӳ����ߣ��������Ქ����Ч֮��
bThrowΪFALSE����Ҫ��׼�ĵ���ʹ��ǰ����Ϊ
*/
void CGPControl::OnThrow(DWORD id, DWORD dwSrcID, BOOL bThrow)
{
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("����(id=%d)ʹ���˲����ڵĵ���(id=%d)", dwSrcID, id);
		return;
	}
	
	IGPTarget* pSrcTarget = GetTargetByID(dwSrcID);

	if ( !pSrcTarget )
	{
		XWaring("GetTargetByID(%d)ʧ��", dwSrcID);
		return;
	}

	//����Ҫ����һ���ӳ��Ķ���
	pSrcTarget->PlayAnimation(GPSettings::throwanimation, TRUE);
	if ( bThrow )
	{
		_PlaySpecialEffect(pSrcTarget, pConfig->listThrowEffect);
	}
}

/*
OnTrigger:
���߻�����Ŀ�꣬�������Ქ����Ч֮��
*/
void CGPControl::OnTrigger(LPGP_DATA pData)
{
	LPGP_CONFIG pConfig = FindConfig(pData->dwID);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", pData->dwID);
		return;
	}

	XTrace("����(%d)������Ŀ��(%d)", pData->dwID, pData->dwDestID);
	_PlaySpecialEffect(GetTargetByID(pData->dwDestID), pConfig->listTriggerEffect);
}

/*
OnDisbled:
���߱��ų��ˣ���������ɱ��
*/
void CGPControl::OnDisabled(LPGP_DATA pData)
{
}

/*
OnDie:
���������ڽ����ˣ���������ɱ��
*/
void CGPControl::OnDie(LPGP_DATA pData)
{
}

/*
OnProtect:
���߷�����һ�ι��������ܻ����ŵ�Ч��
*/
void CGPControl::OnProtect(DWORD id, DWORD dwSrcID)
{
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return;
	}

	XTrace("����(%d)������һ�ι������߱�������", id);
	_PlaySpecialEffect(GetTargetByID(dwSrcID), pConfig->listProtectEffect);
}


/*
OnCure:
���߷�����һ�ι��������ܻ����ŵ�Ч��
*/
void CGPControl::OnCure(DWORD id, DWORD dwSrcID)
{
	LPGP_CONFIG pConfig = FindConfig(id);
	if ( !pConfig )
	{
		XWaring("FindConfig(%d) failed", id);
		return;
	}

	XTrace("����(%d)�ɹ�����", id);
	_PlaySpecialEffect(GetTargetByID(dwSrcID), pConfig->listCureEffect);
}

/*
_PlaySpecialEffect:
������Ч
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
��ӦWindows��Ϣ
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
			//�����ε�
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
			XWaring("CreateGPShelterIteratorʧ�ܣ�������֧��������");
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

	//���Ŀ���жϸ���ǰ��׼�ߵľ���
	GPTARGET_LIST	listTarget;
	ENUMSHOOTSCOPE enmDistance = ESS_Far;
	for ( int i = 0; i < pTargetIter->GetTargetCount(); ++i )
	{
		IGPTarget* pTarget = pTargetIter->GetTarget(i);
		if ( !pTarget || pTarget == pHost )	//�ò��������Լ�
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
	//�ۼ���׼ʱ��
	if ( listTarget.size() > 0 )
	{
		if ( m_pTarget )
		{
			GPTARGET_LIST_Iterator it = find(listTarget.begin(), listTarget.end(), m_pTarget);
			if ( it != listTarget.end() )
			{
				m_dwTargetTime += dwDeltaTime;
				//�����һ��Ŀ�����׼ʱ�䳬��Ԥ����С����Ҫ�л�����һ��Ŀ��
				if ( m_dwTargetTime > GPSettings::exchangetime )
				{
					++it;
					//�������һ���ˣ���ͷ��ʼ
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
	//��ʾ�Լ�����׼��ͷ
	ShowAim(enmDistance, m_pTarget);
	//��ʾĿ�����ϵ���׼��ͷ
	//�����Ŀ���ϵ���׼��־
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

//ȡ�õ�ǰ������׼��Ŀ��
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

//��ʼ��׼��
void CGPAimer::_StartAim()
{
	m_bAimming = TRUE;
	CGPNetIntercommn::RequestChangeAimState(0, 1);//Ҫ��1
}

//������׼
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

//�жϵ��ֱ�ߵ����λ��
CGPAimer::ENUMSHOOTSCOPE CGPAimer::_JudgeTargetScope(NiPoint3 kOrigin, NiPoint3 kDir, NiPoint3 kTarget)
{
	//��㵽ֱ�ߵľ���
	double d = abs(GPMath::ComputeDistanceFromPointToLine(kOrigin, kDir, kTarget));
	//�Ȳ������ϰ�������

	//����н�
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

//��Ŀ������ʾ/������׼ͼ��
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

//��ʾ/�����Լ�����׼��ͼ��
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

//ȡ����׼��������Լ���λ��
NiPoint3 CGPAimer::_GetAimLocalOriginPos(IGPTarget* pTarget)
{
	return NiPoint3(GPSettings::aimoffsetx, GPSettings::aimoffsety, GPSettings::aimoffsetz);
}

//ȡ����׼������������
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
//ȡ����׼���򣨵�ͬ�ھ�ͷ����
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
