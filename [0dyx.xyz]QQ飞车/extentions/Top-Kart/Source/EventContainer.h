
#ifndef EVENTCONTAINER_H
#define EVENTCONTAINER_H

#include <NiMemObject.h>

#include "Top-KartPCH.h"
#include <stack>
#include <queue>
#include <list>
using namespace std;

class EventContainer : public NiMemObject
{

public:
/*	enum InputEvent
	{
		
		INPUT_NONE = 0,
		KEYBORAD = 1,
		MOUSE = 2,

		PRESS_A = (1 << 2)| KEYBORAD,
		PRESS_S = (2 << 2) | KEYBORAD,
		PRESS_D = (3 << 2) | KEYBORAD,
		PRESS_W = (4 << 2) | KEYBORAD,
		PRESS_CTRL = (5 << 2) | KEYBORAD,
		PRESS_SHIFT = (6 << 2) | KEYBORAD,
		PRESS_LEFT = (7 << 2) | KEYBORAD,
		PRESS_RIGHT = (8 << 2) | KEYBORAD,
		PRESS_UP = (9 << 2) | KEYBORAD,
		PRESS_DOWN = (10 << 2) | KEYBORAD,
		PRESS_SPACE = (11 << 2) | KEYBORAD,
		PRESS_G = (12 << 2) | KEYBORAD,
		PRESS_F = (13 << 2) | KEYBORAD
	

	};
*/	
	enum AnimateEvent
	{
		ANIM_NONE,
		ANIM_IDLE,
		ANIM_TURNLEFT,
		ANIM_TURNRIGHT,
		ANIM_TURNBACK,
		ANIM_SPPEDUP,
		ANIM_LIGHTHIT,
		ANIM_HEAVYHIT,
		ANIM_FAIL,
		ANIM_WIN
	};


	enum PhysicEvent
	{

		PHYSIC_NONE = 0,
		PHYSIC_HEAVY_HIT,
		PHYSIC_LIGHT_HIT,

	};


	enum SoundEvent
	{
		SOUND_NONE = 0,

	};

	EventContainer(){}
	~EventContainer(){}
	
	static bool Create();
	static void Destroy();
	static EventContainer* Get();


	
	void PushAnimateEvent( AnimateEvent event );
	AnimateEvent PopAnimateEvent();
	AnimateEvent GetAnimateEvent();

/*
	void PushInputEvent( InputEvent event);
	InputEvent PopInputEvent();
*/
	void PushPhysicEvent( PhysicEvent event );
	PhysicEvent PopPhysicEvent();

	void PushSoundEvent(SoundEvent event);
	SoundEvent PopSoundEvent();


protected:
//	AnimateEvent m_playerAnimEvent;
	std::queue<AnimateEvent> m_queueAnimEvent;
//	std::queue<InputEvent> m_queueInputEvent;
	std::queue<PhysicEvent> m_queuePhysicEvent;
	std::queue<SoundEvent> m_queueSoundEvent;

	static EventContainer * ms_pkEnventContainer;


};

#include "EventContainer.inl"

#endif