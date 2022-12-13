#pragma once

#include "../Task.h"
#include "NiNode.h"
#include "KartStatusManager.h"

class TriggerHintTask
	: public Task
{
public:
	TriggerHintTask(CKartStatusManager::enMark mark, float fStart, float fEnd);
	virtual ~TriggerHintTask();	
	virtual void Execute(unsigned int uiFrameCount, float fTime){}
protected:
	CKartStatusManager::enMark m_enMark;
};









#include "TriggerTask.inl"