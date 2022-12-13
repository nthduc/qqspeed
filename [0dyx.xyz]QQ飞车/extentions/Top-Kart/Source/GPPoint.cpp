#include "Top-KartPCH.h"
#include "GPPoint.h"
#include "gpeffect.h"
#include "physxmanager.h"
#include "common.h"
#include "utility.h"
#include "physxtask.h"

std::map<USHORT,CGPPoint*>	CGPPointMgr::ms_arrPropPoint;
USHORT	CGPPointMgr::ms_nLastPtIdx=0;
UINT CGPPointMgr::ms_nReCreateTime=conStaticBoxReCreateTime;
CRITICAL_SECTION	CGPPointMgr::critSec;
UINT	CGPPointMgr::ms_nLastUpdateTime;
CGPPointMgr::GPPointReport	CGPPointMgr::ms_kPointReport;

CGPPointMgr::tagParam::tagParam()
{
	memset(this,0,sizeof(CGPPointMgr::tagParam));
}

CGPPointMgr::CGPPointMgr()
{

}

CGPPointMgr::~CGPPointMgr()
{

}

CGPPoint::CGPPoint()
:m_nIndexId(0),
m_bActive(false),
m_nLastStaChgTime(0),
m_hActor(INVALID_HACTOR),
m_bTriggered(false)
{
}

CGPPoint::~CGPPoint()
{
	if ( m_stParam.m_pkBox )
	{
		CLevelManager::Get()->DetachObject(m_stParam.m_pkBox);
		m_stParam.m_pkBox = NULL;
	}
	if( m_hActor != INVALID_HACTOR )
	{
		NxScene* pScene = CLevelManager::Get()->GetPhysXManager()->GetPhyScene();
		CPhysXTask::Get()->releaseActor(pScene, m_hActor);
	}
	
	if (m_stParam.m_pkBox)
	{
		CLevelManager::Get()->DetachObject(m_stParam.m_pkBox);
		m_stParam.m_pkBox=0;
	}
}

void	CGPPointMgr::Init(void)
{
	InitializeCriticalSection(&critSec);
}

void CGPPointMgr::Destroy(void)
{
	//�ͷŵ��߷��õ�
	CleanAllPoint();
	DeleteCriticalSection(&critSec);
}

void CGPPointMgr::CleanAllPoint(void)
{
	//�ͷŵ��߷��õ�
	CGPPoint* pPropPt=NULL;
	EnterCriticalSection( &critSec );
	while(!ms_arrPropPoint.empty())
	{
		pPropPt=(ms_arrPropPoint.begin())->second;
		NiDelete pPropPt;
		ms_arrPropPoint.erase(ms_arrPropPoint.begin());
	}
	ms_nLastPtIdx=0;
	LeaveCriticalSection( &critSec );
}

CGPPointMgr::GPPointReport*	CGPPointMgr::GetReportTrigger(void)
{
	return &ms_kPointReport;
}

void	CGPPointMgr::UpdateStaticBox(void)
{
	UINT nNow=::GetTickCount();
	if ((nNow-ms_nLastUpdateTime)<conPropPointUpdateStepTime)
	{
	}
	else
	{
		ms_nLastUpdateTime=nNow;
		CGPPoint* pPropPt=NULL;
		std::map<USHORT,CGPPoint*>::iterator it=ms_arrPropPoint.begin();
		for (;it!=ms_arrPropPoint.end();++it)
		{
			pPropPt=it->second;
			//ֻ�о�̬���ߺ�����ʧʱ�䳬���趨ֵ�Ŵ���
			if (CGPPointMgr::EM_STATIC_BOX==pPropPt->GetParam()->m_nType &&
				(nNow-pPropPt->GetLastStaChgTime())>ms_nReCreateTime)
			{//���¼���
				pPropPt->SetActive(true);
			}
		}
	}
}

CGPPoint*	CGPPointMgr::CreatePoint(CGPPointMgr::Param* pParam, bool bActive)
{
	_ASSERTE(pParam && pParam->m_pkBox && pParam->m_fRadius>0.005); //ԭֵ̫��0.01f );
	_ASSERTE(EM_DYNAMIC_PROP==pParam->m_nType || EM_STATIC_BOX==pParam->m_nType);
	EnterCriticalSection( &critSec );
	CGPPoint* pRes=NiNew CGPPoint;
	pRes->m_nIndexId=ms_nLastPtIdx;
	ms_nLastPtIdx++;
	pRes->m_stParam=*pParam;
	ms_arrPropPoint[pRes->m_nIndexId]=pRes;
	//ģ�ʹ���
	pRes->m_stParam.m_pkBox->SetRotate(pRes->m_stParam.m_kRotate);
	pRes->m_stParam.m_pkBox->SetTranslate(pRes->m_stParam.m_kTranslate);
	pRes->m_stParam.m_pkBox->UpdateProperties();
	pRes->m_stParam.m_pkBox->Update(0.0f);
	//�������ϼ�����ײģ��
	//pRes->m_hActor = NxSceneMgr::Get()->GetActiveSceneLevel()->GetPhysXManager()->CreateGameProp(\
		pRes->m_stParam.m_kTranslate,pRes->m_stParam.m_fRadius,(LPVOID)pRes->m_nIndexId);
	CLevelManager::Get()->AttachObject(pRes->m_stParam.m_pkBox);
	pRes->SetActive(bActive);
	LeaveCriticalSection( &critSec );
	return pRes;
}

bool	CGPPointMgr::RemovePoint(USHORT ushId)
{
	bool bRes=true;
	CGPPoint* pResPt=NULL;
	EnterCriticalSection( &critSec );
	if (ms_arrPropPoint.find(ushId)!=ms_arrPropPoint.end())
	{
		pResPt=ms_arrPropPoint[ushId];
		//�ж��Ƿ���Ҫ�ڳ�����detach
		if(pResPt->GetActive())
		{
			pResPt->SetActive(false);
		}
		ms_arrPropPoint.erase(ushId);
		SAFE_NIDELETE(pResPt);
	}
	else
	{
		_ASSERTE(0);//��Ӧ�ó���û�е��ߵ�
		bRes=false;
	}
	LeaveCriticalSection( &critSec );
	return bRes;
}

CGPPoint*	CGPPointMgr::GetPoint(USHORT ushId)
{
	CGPPoint* pResPt=NULL;
	EnterCriticalSection( &critSec );
	if (ms_arrPropPoint.find(ushId)!=ms_arrPropPoint.end())
	{ 
		pResPt=ms_arrPropPoint[ushId];
	}
	//�����Ҳ����������ģ���Ϊ������ӵ�ɾ���ŵ��˺���
	//_ASSERTE(pResPt);
	LeaveCriticalSection( &critSec );
	return pResPt;
}

void	CGPPointMgr::SetReCreateTime(UINT	nStepTi)
{
	EnterCriticalSection( &critSec );
	ms_nReCreateTime=nStepTi;
	LeaveCriticalSection( &critSec );
}

UINT	CGPPointMgr::GetReCreateTime(void)
{
	return	ms_nReCreateTime;
}

bool	CGPPoint::SetActive(bool bAct)
{
	bool bRes=true;
	if(bAct==m_bActive)
	{
		bRes=false;	//״̬��ͬ����Ϊ�趨ʧ�ܴ���,���㷢���߼�����
	}
	else
	{//��������
		m_bActive=bAct;
		m_nLastStaChgTime=::GetTickCount();
		//������ߺ�ͼ��Ч��
		if (m_bActive)
		{
			//NxSceneMgr::Get()->GetActiveSceneLevel()->AttachObjectToScene(m_stParam.m_pkBox);
			m_stParam.m_pkBox->SetAppCulled(false);
			m_stParam.m_pkBox->SetTranslate(m_stParam.m_kTranslate);
			m_stParam.m_pkBox->SetRotate(m_stParam.m_kRotate);
			//������ײ���������ϼ�����ײģ��
			m_hActor = CLevelManager::Get()->GetPhysXManager()->CreateGameProp(\
				m_stParam.m_kTranslate,m_stParam.m_fRadius,(LPVOID)m_nIndexId);
			if ( m_hActor != INVALID_HACTOR )
			{
				CPhysXTask::Get()->clearActorFlag(m_hActor, NX_AF_DISABLE_COLLISION);
			}
		}
		else
		{
			//NxSceneMgr::Get()->GetActiveSceneLevel()->DetachObjectToScene(m_stParam.m_pkBox);
			m_stParam.m_pkBox->SetAppCulled(true);
			if ( m_hActor != INVALID_HACTOR )
			{
				//ɾ����ײ��
				CPhysXTask::Get()->raiseActorFlag(m_hActor, NX_AF_DISABLE_COLLISION);
				NxScene* pScene = CLevelManager::Get()->GetPhysXManager()->GetPhyScene();
				CPhysXTask::Get()->releaseActor(pScene, m_hActor);
				m_hActor=INVALID_HACTOR; //�����ظ�ɾ��
			}
		}
		//������ߺ�����Ч��
	}
	return bRes;
}

bool	CGPPoint::GetActive(void)
{
	return m_bActive;
}

CGamePropCollisionReport*	CGPPoint::GetReport(void)
{
	return m_stParam.m_pGPReport;
}

UINT	CGPPoint::GetLastStaChgTime(void)
{
	return	m_nLastStaChgTime;
}

const CGPPointMgr::Param*	CGPPoint::GetParam(void)
{
	return &m_stParam;
}

USHORT CGPPoint::GetID()
{
	return m_nIndexId;
}

CGPPointMgr::GPPointReport::OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData)
{
	//���ߺ���ײ����
	CGPPoint* pGpTmp=CGPPointMgr::GetPoint((USHORT)lpGPData);
	//������趨ָ���Ļص����������
	if(pGpTmp && pGpTmp->GetReport())
	{
		pGpTmp->GetReport()->OnTrigger(lpGPData,lpHitObjData);
	}
}
bool CGPPoint::GetTriggered()
{
	return m_bTriggered;
}

bool CGPPoint::SetTriggered(bool bSet)
{
	bool bOld = m_bTriggered;
	m_bTriggered = bSet;
	return bOld;
}