#ifndef GameOverCDTast_H
#define GameOverCDTast_H

#include "Task.h"

class CGameOverCDTast//: public Task
{
public:
	CGameOverCDTast();
	 ~CGameOverCDTast();
	 void Update(float fTime );
	 void Render();

	
private:
	NiNodePtr m_spCountDown;
	DWORD	m_nExcuteTimes;
	DWORD   m_nExcuteStartTimes;
	DWORD   m_nSoundTimes;
	bool	m_bIsAttach;
	bool    m_bIsOver;

	NiCameraPtr m_spCamera;
	float		m_fStartTime;

	CNifAnimationInfo m_sInfo;


};


class CGameOverCountDnMgr
{
public:

	static void StartCountDownTast();
	static void StopCountDownTast();
	static void UpdateCountDownTast(float fTime);
	static void RenderCountDownTast();
	static CGameOverCDTast * ms_pGameoverTask;
};

#endif