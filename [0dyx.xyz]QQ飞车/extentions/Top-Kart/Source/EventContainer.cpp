#include "Top-KartPCH.h"
#include "EventContainer.h"


EventContainer * EventContainer::ms_pkEnventContainer = 0;

bool EventContainer::Create()
{
	if(!ms_pkEnventContainer)
	{
		ms_pkEnventContainer = NiNew EventContainer;
		if(!ms_pkEnventContainer) return false;
	}
	return true;

}

void EventContainer::Destroy()
{
	NiDelete ms_pkEnventContainer;
	ms_pkEnventContainer = 0;
}