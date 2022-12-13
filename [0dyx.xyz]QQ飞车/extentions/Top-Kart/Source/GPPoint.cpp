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
	//释放道具放置点
	CleanAllPoint();
	DeleteCriticalSection(&critSec);
}

void CGPPointMgr::CleanAllPoint(void)
{
	//释放道具放置点
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
			//只有静态道具盒且消失时间超过设定值才处理
			if (CGPPointMgr::EM_STATIC_BOX==pPropPt->GetParam()->m_nType &&
				(nNow-pPropPt->GetLastStaChgTime())>ms_nReCreateTime)
			{//重新激活
				pPropPt->SetActive(true);
			}
		}
	}
}

CGPPoint*	CGPPointMgr::CreatePoint(CGPPointMgr::Param* pParam, bool bActive)
{
	_ASSERTE(pParam && pParam->m_pkBox && pParam->m_fRadius>0.005); //原值太大0.01f );
	_ASSERTE(EM_DYNAMIC_PROP==pParam->m_nType || EM_STATIC_BOX==pParam->m_nType);
	EnterCriticalSection( &critSec );
	CGPPoint* pRes=NiNew CGPPoint;
	pRes->m_nIndexId=ms_nLastPtIdx;
	ms_nLastPtIdx++;
	pRes->m_stParam=*pParam;
	ms_arrPropPoint[pRes->m_nIndexId]=pRes;
	//模型处理
	pRes->m_stParam.m_pkBox->SetRotate(pRes->m_stParam.m_kRotate);
	pRes->m_stParam.m_pkBox->SetTranslate(pRes->m_stParam.m_kTranslate);
	pRes->m_stParam.m_pkBox->UpdateProperties();
	pRes->m_stParam.m_pkBox->Update(0.0f);
	//在赛道上加入碰撞模型
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
		//判断是否需要在场景中detach
		if(pResPt->GetActive())
		{
			pResPt->SetActive(false);
		}
		ms_arrPropPoint.erase(ushId);
		SAFE_NIDELETE(pResPt);
	}
	else
	{
		_ASSERTE(0);//不应该出现没有道具点
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
	//这里找不到是正常的，因为物理盒子的删除放到了后面
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
		bRes=false;	//状态相同下作为设定失败处理,方便发现逻辑错误
	}
	else
	{//可以设置
		m_bActive=bAct;
		m_nLastStaChgTime=::GetTickCount();
		//处理道具盒图形效果
		if (m_bActive)
		{
			//NxSceneMgr::Get()->GetActiveSceneLevel()->AttachObjectToScene(m_stParam.m_pkBox);
			m_stParam.m_pkBox->SetAppCulled(false);
			m_stParam.m_pkBox->SetTranslate(m_stParam.m_kTranslate);
			m_stParam.m_pkBox->SetRotate(m_stParam.m_kRotate);
			//创建碰撞球，在赛道上加入碰撞模型
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
				//删除碰撞球
				CPhysXTask::Get()->raiseActorFlag(m_hActor, NX_AF_DISABLE_COLLISION);
				NxScene* pScene = CLevelManager::Get()->GetPhysXManager()->GetPhyScene();
				CPhysXTask::Get()->releaseActor(pScene, m_hActor);
				m_hActor=INVALID_HACTOR; //避免重复删除
			}
		}
		//处理道具盒物理效果
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
	//道具盒碰撞功能
	CGPPoint* pGpTmp=CGPPointMgr::GetPoint((USHORT)lpGPData);
	//如果有设定指定的回调函数则调用
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