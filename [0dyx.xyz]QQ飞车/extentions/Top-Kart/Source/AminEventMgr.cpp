/*
author: rayhuang
date:	2007/1/18
desc:	动作控制，优先级逻辑
*/



#include "Top-KartPCH.h"
#include "AminEventMgr.h"
#include "one_Anim.h"
#include "car_Anim.h"



CAnimEventMgr::CAnimEventMgr(Play* pPlay)
{
	//m_spCharacter		= spCharater;
	//m_spKart			= spKart;
	//m_mapCharacterAnim	= mapCharacterAnim;
	//m_mapKartAnim		= mapKartAnim;
	m_pPlayer = pPlay;
	m_mapCharacterAnim  = pPlay->m_stCurPlayerDesc.mapCharacterAnim;
	m_mapKartAnim		= pPlay->m_stCurPlayerDesc.mapKartAnim;
	m_enmCurEvnet		= EAET_IDLE;
	m_enmNextEvent		= EAET_IDLE;

}


CAnimEventMgr::~CAnimEventMgr()
{
	while(!m_queueAnimEvent.empty())
	{
		m_queueAnimEvent.pop();
	}
	//m_spCharacter	= 0;
	//m_spKart		= 0;
}

//push 动作事件
void CAnimEventMgr::PushAnimEvent(CAnimEventMgr::ENMANIMEVENTTYPE enAnimEvent)
{
	m_queueAnimEvent.push(enAnimEvent);
}


//获得当前的事件
CAnimEventMgr::ENMANIMEVENTTYPE CAnimEventMgr::GetCurAnimEvent()
{
	/*
	if(NxPlayMgr::GetHostPlay()->GetTargetAnimation() == one_Anim::STILL
		||NxPlayMgr::GetHostPlay()->GetTargetAnimation() == one_Anim::IDLE
		||NxPlayMgr::GetHostPlay()->GetTargetAnimation() == NiActorManager::INVALID_SEQUENCE_ID	)
	{
		return CAnimEventMgr::EAET_IDLE;
	}
	else
	{
		return m_enmCurEvnet;
	}
	*/
//	unsigned int unAnimType = m_spCharacter->GetTargetAnimation();
	unsigned int unAnimType = m_pPlayer->GetCharacterAnim();
	if(unAnimType == m_mapCharacterAnim["TurnLeft"])
	{
		return EAET_TURNLEFT;
	}
	if(unAnimType == m_mapCharacterAnim["TurnRight"])
	{
		return EAET_TURNRIGHT;
	}
	if(unAnimType ==  m_mapCharacterAnim["TurnBack"])
	{
		return EAET_TURNBACK;
	}
	if(unAnimType ==  m_mapCharacterAnim["SpeedUp"])
	{
		return EAET_SPPEDUP;
	}
	if(unAnimType == m_mapCharacterAnim["Win"])
	{
		return EAET_WIN;
	}
	if(unAnimType == m_mapCharacterAnim["Finish"])
	{
		return EAET_FINISH;
	}
	if(unAnimType == m_mapCharacterAnim["UnFinish"])
	{
		return EAET_UNFINISH;
	}
	if(unAnimType ==  m_mapCharacterAnim["Hurt"])
	{
		return EAET_HIT;
	}
	if(unAnimType == m_mapCharacterAnim["Still"])
	{
		return EAET_IDLE;
	}
	if(unAnimType == m_mapCharacterAnim["Idle"])
	{
		return EAET_IDLE;
	}
	if(unAnimType == m_mapCharacterAnim["TurnLeftBig"])
	{
		return EAET_TURNLEFTBIG;
	}
	if(unAnimType == m_mapCharacterAnim["TurnRightBig"])
	{
		return EAET_TURNRIGHTBIG;
	}
	if(unAnimType == NiActorManager::INVALID_SEQUENCE_ID)
	{
		return EAET_IDLE;
	}
	return EAET_IDLE;

}

//执行事件队列
void CAnimEventMgr::ExecuteEvent()
{
	/*
	if(m_spCharacter->GetTargetAnimation() == m_mapCharacterAnim["Still"]
		||m_spCharacter->GetTargetAnimation() == m_mapCharacterAnim["Idle"]
		||m_spCharacter->GetTargetAnimation() == NiActorManager::INVALID_SEQUENCE_ID	)
	{
		m_enmCurEvnet = EAET_IDLE;
	}
	*/
	m_enmCurEvnet = GetCurAnimEvent();
	while(!m_queueAnimEvent.empty())
	{
		m_enmNextEvent = m_queueAnimEvent.front();
		m_queueAnimEvent.pop();

		if( (m_enmNextEvent&EAET_PRIORITY_COVER) <= (m_enmCurEvnet&EAET_PRIORITY_COVER) )
		{
			m_enmCurEvnet = m_enmNextEvent;

			switch(m_enmCurEvnet)
			{
			case EAET_TURNLEFT:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["TurnLeft"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["TurnLeft"]);
//				m_spCharacter->SetTargetAnimation()
				break;

			case EAET_TURNRIGHT:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["TurnRight"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["TurnRight"]);
				break;

			case EAET_TURNBACK:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["TurnBack"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["TurnBack"]);
				break;

			case EAET_SPPEDUP:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["SpeedUp"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["SpeedUp"]);
//				m_spKart->SetTargetAnimation(m_mapKartAnim["Open"]);
				break;

			case EAET_WIN:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["Win"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["Win"]);
				break;

			case EAET_FINISH:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["Finish"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["Finish"]);
				break;

			case EAET_UNFINISH:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["UnFinish"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["UnFinish"]);
			break;

			case EAET_HIT:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["Hurt"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["Hurt"]);
				break;

			case EAET_IDLE:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["Idle"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["Idle"]);
				break;

			case EAET_STILL:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["Still"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["Still"]);
				break;

			case EAET_SEELEFT:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["SeeLeft"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["SeeLeft"]);
				break;

			case EAET_SEERIGHT:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["SeeRight"]);
//				m_spCharacter->SetTargetAnimation(m_mapCharacterAnim["SeeRight"]);
				break;
			case EAET_TURNRIGHTBIG:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["TurnRightBig"]);
				break;
			case EAET_TURNLEFTBIG:
				m_pPlayer->SetCharaterAnimation(m_mapCharacterAnim["TurnLeftBig"]);
				break;
			}
		}
	}
}