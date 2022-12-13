#include "../Play.h"
#include "../PhysKart/kart.h"
#include "../NxSceneMgr.h"
#include "../SceneLevel.h"
#include "../GameStatusManager.h"
#include "CheckEditor.h"
#include "EagleEye.h"
#include "KartStatusManager.h"
#include "NxPlayMgr.h"



inline CPlayerPosUpdateTask::CPlayerPosUpdateTask(void)
{
	ms_bCreated = true;
	m_dwWrongwayShowTime = 0;
	Task::SetType(Task::TYPE_TIME);
	SetTimeParams(0.0f, 0.05f, 0.0f);
}

inline CPlayerPosUpdateTask::~CPlayerPosUpdateTask()
{
	ms_bCreated = false;
}



