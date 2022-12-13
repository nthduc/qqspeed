/*
	author: rayhuang
	date:	2007/1/18
	desc:	动作控制，优先级逻辑
*/



#ifndef	ANIMEVENTMGR_H
#define ANIMEVENTMGR_H

#include <queue>
#include "NiActorManager.h"
#include <map>
#include <string>
using namespace std;

#define MAKE_EVENT_ID(nNum,nFlag)  ( (nNum << EAET_MASK) | (nFlag) )

typedef map<string, int>	StringToIntMap;

class CAnimEventMgr : public NiMemObject
{
public:
	typedef enum enmAminEventType			//动作类型
	{
		//优先级
		EAET_PRIORITY_1			= 1,
		EAET_PRIORITY_2			= 2,
		EAET_PRIORITY_3			= 4,
		EAET_PRIORITY_4			= 8,
		
		//角色动作事件或车动作事件
		EAET_CHARACTER			= 16,
		EAET_KART				= 32,

		//类型覆盖的位数
		EAET_MASK				= 6,

		//优先级覆盖的位数并全部置为1，用于优先级判断比较
		EAET_PRIORITY_COVER		= 15,
		
		//动作事件类型
		EAET_IDLE				= MAKE_EVENT_ID(1, EAET_PRIORITY_4|EAET_CHARACTER|EAET_KART),
		EAET_TURNLEFT			= MAKE_EVENT_ID(2, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_TURNRIGHT			= MAKE_EVENT_ID(3, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_TURNBACK			= MAKE_EVENT_ID(4, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_SPPEDUP			= MAKE_EVENT_ID(5, EAET_PRIORITY_2|EAET_CHARACTER|EAET_KART),
		EAET_HIT				= MAKE_EVENT_ID(6, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_FINISH				= MAKE_EVENT_ID(7, EAET_PRIORITY_1|EAET_CHARACTER),
		EAET_UNFINISH			= MAKE_EVENT_ID(8, EAET_PRIORITY_1|EAET_CHARACTER),
		EAET_WIN				= MAKE_EVENT_ID(9, EAET_PRIORITY_1|EAET_CHARACTER),	
		
		EAET_BANANA				= MAKE_EVENT_ID(10, EAET_PRIORITY_1|EAET_CHARACTER|EAET_KART),
		EAET_SEELEFT			= MAKE_EVENT_ID(11, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_SEERIGHT			= MAKE_EVENT_ID(12, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_CRY				= MAKE_EVENT_ID(13, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_USEITEM			= MAKE_EVENT_ID(14, EAET_PRIORITY_3|EAET_CHARACTER),
		EAET_STILL				= MAKE_EVENT_ID(15, EAET_PRIORITY_4|EAET_CHARACTER|EAET_KART),	
		EAET_TURNLEFTBIG		= MAKE_EVENT_ID(16, EAET_PRIORITY_4|EAET_CHARACTER),
		EAET_TURNRIGHTBIG		= MAKE_EVENT_ID(17, EAET_PRIORITY_4|EAET_CHARACTER),

	} ENMANIMEVENTTYPE;

public:
	CAnimEventMgr(Play* pPlay);
	~CAnimEventMgr();
	void PushAnimEvent(ENMANIMEVENTTYPE enAnimEvent);	//puah 动作事件
	ENMANIMEVENTTYPE GetCurAnimEvent();					//获得当前的事件
	void ExecuteEvent();								//执行事件队列

private:

	std::queue< ENMANIMEVENTTYPE >	m_queueAnimEvent;

	ENMANIMEVENTTYPE				m_enmCurEvnet;		//当前事件
	ENMANIMEVENTTYPE				m_enmNextEvent;		//队列的下一事件

	//NiActorManagerPtr				m_spCharacter;		//角色
	//NiActorManagerPtr				m_spKart;			//车
	Play*							m_pPlayer;			//玩家

	StringToIntMap					m_mapCharacterAnim;	//角色动作图
	StringToIntMap					m_mapKartAnim;		//车动作图

};

#endif