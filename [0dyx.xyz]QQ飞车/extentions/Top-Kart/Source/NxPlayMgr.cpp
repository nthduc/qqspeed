#include "Top-KartPCH.h"
#include ".\nxplaymgr.h"
#include "Avatar/AvatarMgr.h"

//-----------------------------------------------------------------------
//            Create by Hoverzhao	
//-----------------------------------------------------------------------

std::set< Play * > NxPlayMgr::m_AllPlay;
Play * NxPlayMgr::m_pHostPlay = NULL;
std::set< Play * > NxPlayMgr::m_AllRemotePlay;
NxPlayMgr * NxPlayMgr::ms_pNxPlayMgr = NULL;


//-----------------------------------------------------------------------
float NxPlayMgr::UpdatePlayTProc::ms_fTime = 0.0f;
float NxPlayMgr::UpdatePlayTProc::ms_fFrameTime = 0.0f;
bool  NxPlayMgr::UpdatePlayTProc::ms_bAnimationUpdate = false;
NiCamera * NxPlayMgr::DrawPlayTProc::ms_pCamera = NULL;
unsigned int NxPlayMgr::ms_uiCfgThreads = 0;

//-----------------------------------------------------------------------
bool NxPlayMgr::Init()
{
	ms_pNxPlayMgr = new NxPlayMgr();
	return ms_pNxPlayMgr->InitThreads();
}

void NxPlayMgr::Destory() 
{ 
	ClearAllPlay() ; 
	if ( ms_pNxPlayMgr ) 
	{ 
		ms_pNxPlayMgr->DestoryThreads(); 
		delete ms_pNxPlayMgr; 
		ms_pNxPlayMgr = NULL; 
	}
} 

//-----------------------------------------------------------------------
NxPlayMgr::NxPlayMgr():
m_uiMaxThreads(1),
m_uiNumThreads(1)
{
	m_AllPlay.clear();
}

NxPlayMgr::~NxPlayMgr()
{
}

//-----------------------------------------------------------------------
bool NxPlayMgr::InitThreads()
{
	/*m_uiMaxThreads = NiSystemDesc::GetSystemDesc()
		.GetLogicalProcessorCount();*/
	
	m_uiMaxThreads = ms_uiCfgThreads;

	if (m_uiMaxThreads > NxPlayMgr::MAX_THREADS)
		m_uiMaxThreads = NxPlayMgr::MAX_THREADS;
	m_uiNumThreads = m_uiMaxThreads;

	for (unsigned int uiT = 0; uiT < m_uiMaxThreads; uiT++)
	{
		m_apkUpdateThreads[uiT] = NiLoopedThread::Create(
			&m_akUpdateTProcs[uiT]);

		m_apkUpdateThreads[uiT]->SetPriority(NiThread::BELOW_NORMAL);
		m_apkUpdateThreads[uiT]->Resume();
	}
	for (unsigned int uiT = 0; uiT < m_uiMaxThreads; uiT++)
	{
		m_apkDrawThreads[uiT] = NiLoopedThread::Create(
			&m_akDrawTProcs[uiT]);

		m_apkDrawThreads[uiT]->SetPriority(NiThread::BELOW_NORMAL);
		m_apkDrawThreads[uiT]->Resume();
	}
	return true;
}

//-----------------------------------------------------------------------
void NxPlayMgr::DestoryThreads()
{
	for (unsigned int uiT = 0; uiT < m_uiMaxThreads; uiT++)
	{
		NiDelete m_apkUpdateThreads[uiT];
		NiDelete m_apkDrawThreads[uiT];
	}
	return;
}

//-----------------------------------------------------------------------
void NxPlayMgr::UpdateAllPlay(float fTime, float fFrameTime, bool bAnimationUpdate)
{
	std::set<Play*>::iterator it = m_AllPlay.begin();

	if ( ms_pNxPlayMgr && ms_uiCfgThreads > 0 ) 
	{
		ms_pNxPlayMgr->UpdateAllPlayThreads(fTime, fFrameTime, bAnimationUpdate );			
	}
	else
	{
		for(; it != m_AllPlay.end(); ++it)
		{
			(*it)->Update(fTime, fFrameTime, InputManager::Get()->GetRelativeDir(), 
				InputManager::Get()->GetStrafeDir(), bAnimationUpdate);			
		}	
	}
}

//-----------------------------------------------------------------------
void NxPlayMgr::UpdateAllPlayThreads(float fTime, float fFrameTime, bool bAnimationUpdate)
{

	if ( m_AllPlay.empty() )
		return;

	NiRenderer::GetRenderer()->LockRenderer();

	std::set<Play*>::iterator kPos = m_AllPlay.begin();

	unsigned int  uiRemaining  = m_AllPlay.size();
	unsigned int  uiNumThreads = uiRemaining >= m_uiNumThreads ? 
m_uiNumThreads : uiRemaining;

	if ( uiNumThreads > 0 )
	{
		unsigned int uiDivSize = uiRemaining / uiNumThreads;

		UpdatePlayTProc::ms_fTime = fTime;
		UpdatePlayTProc::ms_fFrameTime = fFrameTime;
		UpdatePlayTProc::ms_bAnimationUpdate = bAnimationUpdate;

		for (unsigned int uiT = 0; uiT < uiNumThreads - 1; uiT++)
		{
			m_akUpdateTProcs[uiT].m_kPos = kPos;
			m_akUpdateTProcs[uiT].m_uiChildren = uiDivSize;
			uiRemaining -= uiDivSize;
			m_apkUpdateThreads[uiT]->DoLoop();

			for (unsigned int uiChild = 0; uiChild < uiDivSize; uiChild++)
			{
				kPos++;
			}
		}
		unsigned int uiLast = uiNumThreads - 1;
		m_akUpdateTProcs[uiLast].m_kPos = kPos;
		m_akUpdateTProcs[uiLast].m_uiChildren = uiRemaining;
		m_apkUpdateThreads[uiLast]->DoLoop();
	}

	if (uiNumThreads > 0)
	{
		for (unsigned int uiTWait = 0; uiTWait < uiNumThreads; uiTWait++)
		{
			m_apkUpdateThreads[uiTWait]->WaitForLoopCompletion();
		}
	}

	NiRenderer::GetRenderer()->UnlockRenderer();
}

//-----------------------------------------------------------------------
bool NxPlayMgr::UpdatePlayTProc::LoopedProcedure(void* pvArg)
{
	for (unsigned int uiC = 0; uiC < m_uiChildren; uiC++)
	{
		(*m_kPos)->Update( ms_fTime, ms_fFrameTime, InputManager::Get()->GetRelativeDir(), 
			InputManager::Get()->GetStrafeDir(),ms_bAnimationUpdate);		
		m_kPos++;
	}
	return true;
}

////-----------------------------------------------------------------------
//void NxPlayMgr::DrawAllPlay(NiCamera * pkCamera)
//{
//	std::set<Play*>::iterator it = m_AllPlay.begin();
//
//	if (NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == 
//		GameStatusManager::GST_RACING ||NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == 
//		GameStatusManager::GST_STAR_RACE)
//	{	
//
//		if ( ms_pNxPlayMgr && ms_uiCfgThreads > 0 )
//		{
//			ms_pNxPlayMgr->DrawAllPlayThreads(pkCamera);
//		}
//		else
//		{
//			for(; it != m_AllPlay.end(); ++it)
//			{
//				(*it)->Draw(pkCamera);
//			}
//		}
//	}
//	else if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == 
//		GameStatusManager::GST_ROOM)
//	{
//		if( m_pHostPlay)
//			m_pHostPlay->Draw( pkCamera);
//	}
//}

//-----------------------------------------------------------------------
void NxPlayMgr::DrawAllPlayThreads(NiCamera * pkCamera)
{
	if ( m_AllPlay.empty() )
		return;

	NiRenderer::GetRenderer()->LockRenderer();

	std::set<Play*>::iterator kPos = m_AllPlay.begin();

	unsigned int  uiRemaining  = m_AllPlay.size();
	unsigned int  uiNumThreads = uiRemaining >= m_uiNumThreads ? 
m_uiNumThreads : uiRemaining;

	if ( uiNumThreads > 0 )
	{
		unsigned int uiDivSize = uiRemaining / uiNumThreads;

		DrawPlayTProc::ms_pCamera = pkCamera;

		for (unsigned int uiT = 0; uiT < uiNumThreads - 1; uiT++)
		{
			m_akDrawTProcs[uiT].m_kPos = kPos;
			m_akDrawTProcs[uiT].m_uiChildren = uiDivSize;
			uiRemaining -= uiDivSize;
			m_apkDrawThreads[uiT]->DoLoop();

			for (unsigned int uiChild = 0; uiChild < uiDivSize; uiChild++)
			{
				kPos++;
			}
		}
		unsigned int uiLast = uiNumThreads - 1;
		m_akDrawTProcs[uiLast].m_kPos = kPos;
		m_akDrawTProcs[uiLast].m_uiChildren = uiRemaining;
		m_apkDrawThreads[uiLast]->DoLoop();
	}

	if (uiNumThreads > 0)
	{
		for (unsigned int uiTWait = 0; uiTWait < uiNumThreads; uiTWait++)
		{
			m_apkDrawThreads[uiTWait]->WaitForLoopCompletion();
		}
	}

	NiRenderer::GetRenderer()->UnlockRenderer();

}

//-----------------------------------------------------------------------
bool NxPlayMgr::DrawPlayTProc::LoopedProcedure(void * pvArg )
{
	for ( unsigned  int uiC = 0; uiC <  m_uiChildren; uiC++ )
	{
		(*m_kPos)->Draw(ms_pCamera);
		m_kPos++;
	}
	return true;
}

//-----------------------------------------------------------------------
Play * NxPlayMgr::CreatePlay(  KPlayInfo & Info ,bool bHostPlay /* = false */)
{
	// 这里需要判断一下 玩家是否已经被创建了

	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it != m_AllPlay.end(); ++it)
	{
		if((*it)->GetPlayUin() == Info.m_unUin)
		{
			return (*it);
		}
	}
	//
	Play * pPlay = NULL;

	if(bHostPlay)
	{
		pPlay = NiNew Play();
		m_pHostPlay = pPlay;
	}
	else
	{
		pPlay = NiNew RemotePlay();
	}

	if(!pPlay->Initialize((Info),0,0))
	{
		NiDelete pPlay; pPlay = NULL;
		if (bHostPlay)
			m_pHostPlay = NULL;
		return NULL;
	}

	m_AllPlay.insert(pPlay);
	//todo:加载avatar模型数据
	CAvatarMgr::Get()->BindRes1st(pPlay->GetPlayInfo().m_unUin);
	return pPlay;

}
//-----------------------------------------------------------------------
/*
void NxPlayMgr::PhysicAllPlay()
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it!= m_AllPlay.end(); ++it)
	{
		//(*it)->EnterPhyScene();
	}

}*/
//-----------------------------------------------------------------------

void NxPlayMgr::UnPhysicAllPlay()
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it!= m_AllPlay.end(); ++it)
	{
		(*it)->LeavePhyScene();
	}
}

void NxPlayMgr::StartAllPhysicPlay()
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it!= m_AllPlay.end(); ++it)
	{
		(*it)->StartPhy();
	}
}
void NxPlayMgr::StopAllPhysicPlay()
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it!= m_AllPlay.end(); ++it)
	{
		(*it)->StopPhy();
	}
}
//-----------------------------------------------------------------------

void NxPlayMgr::UpdateAllPhysicPlay(float fTime)
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it!= m_AllPlay.end(); ++it)
	{
		(*it)->UpdatePhy(fTime);
	}
}
//-----------------------------------------------------------------------
void NxPlayMgr::ClearAllPlay()
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it!= m_AllPlay.end(); ++it)
	{
		SAFE_NIDELETE(*it);
		//(*it) = NULL;
	}
	m_pHostPlay = NULL;
	m_AllPlay.clear();
}

void NxPlayMgr::SetAllPlayAnim(NiFixedString strName)
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it!= m_AllPlay.end(); ++it)
	{
		(*it)->SetCharaterAnimation(strName);
	}
}

std::set<Play*> NxPlayMgr::GetAllRemotePlay()
{
	std::set<Play*> allRemotePlay;
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for( ; it != m_AllPlay.end(); it++)
	{
		if( (*it)->GetPlayUin() != GetHostPlay()->GetPlayUin())
			allRemotePlay.insert(*it);
	}
	return allRemotePlay;
}

//-----------------------------------------------------------------------
void NxPlayMgr::ClearAllRemotePlay()
{
	bool bExist = true;

	while(bExist) {
		bExist = false;
		std::set<Play*>::iterator it = m_AllPlay.begin();
		for(; it!= m_AllPlay.end(); ++it) {
			if( !(*it)->IsHost()) {
				SAFE_NIDELETE (*it);
				m_AllPlay.erase(it);
				bExist = true;
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------
void NxPlayMgr::DrawAllPlay(NiCamera * pkCamera)
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	if (NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == 
		GameStatusManager::GST_RACING ||NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == 
		GameStatusManager::GST_STAR_RACE)
	{
		if ( ms_pNxPlayMgr && ms_uiCfgThreads > 0 )
		{
			ms_pNxPlayMgr->DrawAllPlayThreads(pkCamera);
		}
		else
		{
			for(; it != m_AllPlay.end(); ++it)
			{
				(*it)->Draw(pkCamera);
			}
		}
				
	}
	else if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == 
		GameStatusManager::GST_ROOM)
	{
		if( m_pHostPlay)
			m_pHostPlay->Draw( pkCamera);
	}
}
//-----------------------------------------------------------------------
void NxPlayMgr::RenderAllPlayName()
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it != m_AllPlay.end(); ++it)
	{
		(*it)->RenderName();
	}
}

//-----------------------------------------------------------------------
void NxPlayMgr::OnAllPlayLostDevice(bool bReset)
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it != m_AllPlay.end(); ++it)
	{
		(*it)->OnLostDevice(bReset);
	}
}

void NxPlayMgr::ShowAllPlayName(BOOL bShow)
{
	std::set<Play*>::iterator it = m_AllRemotePlay.begin();
	for(; it != m_AllRemotePlay.end(); ++it)
	{
		(*it)->ShowName(bShow);
	}
}
/*
//-----------------------------------------------------------------------
void NxPlayMgr::RenderAllPlayPortrait()
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it != m_AllPlay.end(); ++it)
	{
		(*it)->RenderPortrait();
	}		
}*/


//-----------------------------------------------------------------------
void NxPlayMgr::RemovePlay(ULONG dwUin)
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it != m_AllPlay.end(); ++it)
	{
		if((*it)->GetPlayUin() == dwUin)
		{
			SAFE_NIDELETE(*it);
			m_AllPlay.erase(it);
			break;
		}
	}
}

//-----------------------------------------------------------------------
Play * NxPlayMgr::GetPlay(ULONG dwUin)
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it != m_AllPlay.end(); ++it)
	{
		if((*it)->GetPlayUin() == dwUin)
		{
			return (*it);
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------
void NxPlayMgr::OnContactNotify(NxContactPair& pair, NxU32 events)
{
	//if (IsSafeVehicle((NxVehicle*)pair.actors[0]->userData)) {
	//	NxVehicle* v = (NxVehicle*)pair.actors[0]->userData;
	//	v->handleContactPair(pair, 0, events);
	//}
	//if (IsSafeVehicle((NxVehicle*)pair.actors[1]->userData)) {
	//	NxVehicle* v = (NxVehicle*)pair.actors[1]->userData;
	//	v->handleContactPair(pair, 1, events);
	//}
}
/*
//-----------------------------------------------------------------------
#ifndef _XCART_
bool NxPlayMgr::IsSafeVehicle(WheellessCar * pVehicle)
#else
bool NxPlayMgr::IsSafeVehicle(NxVehicle* pVehicle)
#endif
{
	std::set<Play*>::iterator it = m_AllPlay.begin();
	for(; it != m_AllPlay.end(); ++it)
	{
		if((*it)->GetKart()->GetVehicle() == pVehicle)
		{
			return true;
		}
	}
	return false;
}*/

//-----------------------------------------------------------------------
// 临时代码 提供碰撞回调的检测
//-----------------------------------------------------------------------

#define MAX_DISTANCE 999999

float ComputeDistance (Play * pPlay1, Play * pPlay2)
{
	if( pPlay1 == NULL || pPlay2 == NULL)
		return MAX_DISTANCE;

	NiPoint3 p1 = pPlay1->GetPos();
	NiPoint3 p2 = pPlay2->GetPos();

	return sqrt(
		(p1.x - p2.x) * (p1.x - p2.x) +
		(p1.y - p2.y) * (p1.y - p2.y) +
		(p1.z - p2.z) * (p1.z - p2.z) );

}


Play * NxPlayMgr::GetNearPlayFromHostPlay()
{
	Play * pPlay = GetHostPlay();
	Play * pTargetPlay = NULL;

	std::set< Play * > _AllRemotePlay = GetAllRemotePlay();
	std::set< Play * >::iterator it = _AllRemotePlay.begin();

	float fDistance = MAX_DISTANCE ;
	
	for ( ; it != _AllRemotePlay.end(); it++)
	{
		float dist =  ComputeDistance(pPlay, (*it));
		
		if ( dist < fDistance)
		{
			fDistance = dist;
			pTargetPlay = *it;
		}
	}

	return pTargetPlay;
}
