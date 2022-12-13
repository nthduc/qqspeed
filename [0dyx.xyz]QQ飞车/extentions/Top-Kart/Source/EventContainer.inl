#include "EventContainer.h"


inline EventContainer* EventContainer::Get()
{
	assert(ms_pkEnventContainer);
	return ms_pkEnventContainer;
}


inline void EventContainer::PushAnimateEvent(AnimateEvent event )
{
	m_queueAnimEvent.push(event);
}


inline EventContainer::AnimateEvent EventContainer::PopAnimateEvent()
{
	EventContainer::AnimateEvent temp;
	if(m_queueAnimEvent.empty()) return ANIM_NONE;
	temp = m_queueAnimEvent.front();
	m_queueAnimEvent.pop();
	return temp;

}

inline EventContainer::AnimateEvent EventContainer::GetAnimateEvent()
{
	if(m_queueAnimEvent.empty()) return ANIM_NONE;
	return m_queueAnimEvent.front();
}


/*
inline void EventContainer::PushInputEvent(InputEvent event)
{
	m_queueInputEvent.push(event);
}

inline EventContainer::InputEvent EventContainer::PopInputEvent()
{
	EventContainer::InputEvent temp;
	if(m_queueInputEvent.empty()) return INPUT_NONE;
	temp = m_queueInputEvent.front();
	m_queueInputEvent.pop();
	return temp;

}
*/
inline void EventContainer::PushPhysicEvent(PhysicEvent event )
{
	m_queuePhysicEvent.push(event);
}


inline EventContainer::PhysicEvent EventContainer::PopPhysicEvent()
{
	EventContainer::PhysicEvent temp;
	if(m_queuePhysicEvent.empty()) return PHYSIC_NONE;
	temp = m_queuePhysicEvent.front();
	m_queuePhysicEvent.pop();
	return temp;
}

inline void EventContainer::PushSoundEvent(SoundEvent event)
{
	m_queueSoundEvent.push(event);
}

inline EventContainer::SoundEvent EventContainer::PopSoundEvent()
{
	EventContainer::SoundEvent temp;
	if(m_queueSoundEvent.empty()) return SOUND_NONE;
	temp = m_queueSoundEvent.front();
	m_queueSoundEvent.pop();
	return temp;
}

/*
inline void EventContainer::SetPlayerAnimEvent(AnimateEvent event)
{
	m_playerAnimEvent = event;
}

inline EventContainer::AnimateEvent EventContainer::GetPlayAnimEvent()
{
	return m_playerAnimEvent;
}
*/

