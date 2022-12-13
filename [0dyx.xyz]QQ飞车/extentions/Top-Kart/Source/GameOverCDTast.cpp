#include "Top-KartPCH.h"
#include "GameOverCDTast.h"
#include "./track/KartStatusManager.h"
#include "Utility.h"

extern void FindAndAdd(NiObjectNET * pkRoot);

CGameOverCDTast * CGameOverCountDnMgr::ms_pGameoverTask = NULL;

CGameOverCDTast::CGameOverCDTast():
m_nExcuteStartTimes(0),
m_nExcuteTimes(0),
m_nSoundTimes(0),
m_bIsAttach(false),
m_bIsOver(false),
m_spCamera(0),
m_fStartTime(0.0f)
{

	/*
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/caidai.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		//m_spCamera = NxSceneMgr::Get()->GetCameraController()->GetCamera();
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
	*/




	//m_spCountDown = RMOperator::OpenNifAndPrepack("Map/endCountDown.nif");
	//FindAndAdd(m_spCountDown);

	//if( m_spCountDown ) {
	//	m_spCamera = NiDynamicCast(NiCamera, m_spCountDown->GetObjectByName("Camera01"));
	//	m_spCountDown->UpdateEffects();
	//	m_spCountDown->UpdateProperties();
	//	m_spCountDown->Update(0.0f);
	//	m_sInfo.CollectData(m_spCountDown);
	//}

/*
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename("Map/endCountDown.nif"))) 
	{
		assert(0.0f);
	}

	m_spCountDown = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	FindAndAdd(m_spCountDown);
	m_spCountDown->Update(0.0f);

*/


	//m_nExcuteStartTimes = ::GetTickCount();
	//m_nExcuteTimes =m_nExcuteStartTimes + 10000;
	//m_nSoundTimes = ::GetTickCount();



	//if(m_spCountDown){NiTimeController::StartAnimations(m_spCountDown);};
	//m_fStartTime = GetMainApp()->GetAccumTime();


}


CGameOverCDTast::~CGameOverCDTast()
{
	/*
	if( NxPlayMgr::GetHostPlay())
	{
		NxPlayMgr::GetHostPlay()->DetachHint(m_spCountDown);
		m_spCountDown = 0;
	}*/

//	if(m_spCountDown){NiTimeController::StopAnimations(m_spCountDown);};
//
//	m_spCamera = 0;
//	m_spCountDown = 0;
}


void CGameOverCDTast::Update(float fTime)
{
	/*
	if( ::GetTickCount() > m_nExcuteStartTimes && !m_bIsAttach)
	{
		NxPlayMgr::GetHostPlay()->AttachHint(m_spCountDown);
		m_bIsAttach = true;
	}
	*/


	/*if(m_spCountDown) {
		m_spCountDown->Update(fTime);
	}

	if ( ::GetTickCount() - m_nSoundTimes > 1000 )
	{
		NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
		m_nSoundTimes = ::GetTickCount();
	}


	if ( ::GetTickCount() > m_nExcuteTimes && !m_bIsOver)
	{
		CKartStatusManager::Get()->OnTriggerRaceFinish(RACE_FINISH_TIMEOUT, 0);
		m_bIsOver = true;
	}*/

}

void CGameOverCDTast::Render()
{
	/*NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera, m_spCountDown);*/
}



void CGameOverCountDnMgr::StartCountDownTast()
{
	/*if(!NxPlayMgr::GetHostPlay()->IsFinish())
	{

		if ( !ms_pGameoverTask )
		{
			ms_pGameoverTask = new CGameOverCDTast();
		}
	}*/
}

void CGameOverCountDnMgr::StopCountDownTast()
{
	//if ( ms_pGameoverTask )
	//{
	//	delete ms_pGameoverTask;
	//	ms_pGameoverTask = NULL;
	//}
}

void CGameOverCountDnMgr::UpdateCountDownTast(float fTime)
{
	/*if ( ms_pGameoverTask)
		ms_pGameoverTask->Update(fTime);*/
}

void CGameOverCountDnMgr::RenderCountDownTast()
{
	//if ( ms_pGameoverTask)
	//	ms_pGameoverTask->Render(fTime);
}
