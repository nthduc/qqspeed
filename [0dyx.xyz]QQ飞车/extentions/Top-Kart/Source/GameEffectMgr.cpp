#include "Top-KartPCH.h"
#include "GameEffectMgr.h"
#include "./PhysKart/NxAllVehicles.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "RMOperator.h"
#include "./Track/KartStatusManager.h"
//#include "SoundManager.h"
#include "soundmgr.h"
#include "NiObjectNET.h"
#include "NiPSysEmitterCtlr.h"
#include "SpeedLine.h"


//	NxSceneMgr::Get()->GetActiveSceneLevel()->LightObject(m_spEffectNode);
#define FORMAT_NODE(m_spEffectNode)\
	m_spEffectNode->UpdateProperties();\
	m_spEffectNode->UpdateEffects();\
	m_spEffectNode->Update(0);\


void IGameEffect::Start()
{
	if(m_spEffectNode){NiTimeController::StartAnimations(m_spEffectNode);};
	m_fStartTime = GetMainApp()->GetAccumTime();
	m_bActive = true;
};

void IGameEffect::Stop()
{
	if(m_spEffectNode){NiTimeController::StopAnimations(m_spEffectNode);};
	m_bActive = false;
};

void IGameEffect::Update(float fTime,NiPoint3* pPos)
{
	UpdateActive();
	CoreUpdate(fTime,pPos);
}

void IGameEffect::UpdateActive()
{
	if(GetType() & ET_LOOP)
	{
		//
	}
	else
	{
		if(m_bActive)
		{
			float sfTime = m_sInfo.GetMaxTime() - m_sInfo.GetMinTime();
			sfTime += m_fStartTime;

			if(sfTime <= GetMainApp()->GetAccumTime())
			{
				m_bActive = false;
			}
		}
	}
}


void IGameEffect::Load(NiNodePtr spNode)
{		
	PreLoad();
	CoreLoad(spNode); 
	PostLoad();
}

extern void FindAndAdd(NiObjectNET * pkRoot);

void CGameEffectMgr::ActiveEffect(IGameEffect::EffectType eType,NiPoint3* pPos)
{
	assert(CGameEffectMgr::Get()->m_pkBaseParticles[GET_EFFECT_INDEX(eType)]);

	IGameEffect* gaft;

	if(eType & IGameEffect::ET_ONLY)
	{	
		gaft = NULL;

		NiTListIterator kPos = CGameEffectMgr::Get()->m_kActiveParticles.GetHeadPos();
		while (kPos)
		{
			IGameEffect* pkPE = CGameEffectMgr::Get()->m_kActiveParticles.GetNext(kPos);

			if(pkPE->GetType() == eType)
			{
				gaft = pkPE;
				if(!(pkPE->GetType() & IGameEffect::ET_REVERT))
				{
					return;
				}
			}
		}

		if(!gaft)
		{
			gaft = CGameEffectMgr::Get()->m_pkBaseParticles[GET_EFFECT_INDEX(eType)];
			CGameEffectMgr::Get()->m_kActiveParticles.AddTail(gaft);
		}


	}
	else
	{
		if ( CGameEffectMgr::Get()->m_kInActiveParticles[GET_EFFECT_INDEX(eType)].GetSize() > 0)
		{
			gaft =  CGameEffectMgr::Get()->m_kInActiveParticles[GET_EFFECT_INDEX(eType)].RemoveHead();
		}
		else
		{
			gaft = CGameEffectMgr::Get()->m_pkBaseParticles[GET_EFFECT_INDEX(eType)];
			gaft = gaft->Clone();
		}
		CGameEffectMgr::Get()->m_kActiveParticles.AddTail(gaft);
	}


	if(gaft)
	{
		gaft->SetPos(pPos);
		gaft->Start();
	}
	
}

void CGameEffectMgr::DeActiveEffect(IGameEffect::EffectType eType)
{
	NiTListIterator kPos = CGameEffectMgr::Get()->m_kActiveParticles.GetHeadPos();
	while (kPos)
	{
		IGameEffect* pkPE = CGameEffectMgr::Get()->m_kActiveParticles.GetNext(kPos);

		if(pkPE->GetType() == eType)
		{
			pkPE->Stop();
		}
	}
}
#define PUSH_EFFECT(T) \
{\
	T * pE = NiNew T ; \
	pE->Load(NULL); \
	m_pkBaseParticles[pE->GetIndex()] = pE; \
}\

CGameEffectMgr::CGameEffectMgr()
{
	for (int i = 0; i < IGameEffect::MAX_EFFECT_NUM; ++i)
	{
		m_pkBaseParticles[i] = NULL;
	}
	InitEffect();
}

CGameEffectMgr::~CGameEffectMgr()
{	
	DestroyEffect();
}

void CGameEffectMgr::InitEffect()
{
	PUSH_EFFECT(IGameFinishEffect);
	PUSH_EFFECT(IGameUnFinishEffect);
	PUSH_EFFECT(IGameStartCountdownEffect);
	PUSH_EFFECT(IGameOverCountdownEffect);

	PUSH_EFFECT(IFirstLapEffect);
	PUSH_EFFECT(ISecondLapEffect);
	PUSH_EFFECT(ILastLapEffect);
	//PUSH_EFFECT(IWrongWayEffect);
	//PUSH_EFFECT(CAngleDrifting);
	//PUSH_EFFECT(CAngleDriftingCenter);

	PUSH_EFFECT(CStopShow);
	PUSH_EFFECT(ISnowEffect);
	PUSH_EFFECT(ICrashEffect);
	PUSH_EFFECT(IAccEffect);
	PUSH_EFFECT(IBackFireEffect);
	PUSH_EFFECT(ICaiDaiEffect);
	PUSH_EFFECT(ISmokeEffect);
	PUSH_EFFECT(IGamePerfectEffect);
	PUSH_EFFECT(ISpeedEffect);
	PUSH_EFFECT(CPressUP_ET);
	PUSH_EFFECT(CPressDown_ET);
	PUSH_EFFECT(CPressLeft_ET);
	PUSH_EFFECT(CPressRight_ET);
	PUSH_EFFECT(CTaskFinish_ET);
	PUSH_EFFECT(CDriftTask_Step1);
	PUSH_EFFECT(CDriftTask_Step2);
	PUSH_EFFECT(CDriftTask_Step3);
	PUSH_EFFECT(CDriftTask_Step4);
}

void CGameEffectMgr::DestroyEffect()
{
	ResetEffect();

	for(unsigned int ui = 0; ui < IGameEffect::MAX_EFFECT_NUM; ui++)
	{
		NiTListIterator kPos = m_kInActiveParticles[ui].GetHeadPos();

		while (kPos)
		{
			IGameEffect* pkPE = m_kInActiveParticles[ui].GetNext(kPos);
			NiDelete pkPE;
		}        
		m_kInActiveParticles[ui].RemoveAll();
	
		SAFE_NIDELETE(m_pkBaseParticles[ui]);			
	}
}

void CGameEffectMgr::ResetEffect()
{
	NiTListIterator kPos = m_kActiveParticles.GetHeadPos();
	while (kPos)
	{
		IGameEffect* pkPE = m_kActiveParticles.GetNext(kPos);

		pkPE->Stop();
		m_kActiveParticles.Remove(pkPE);
	}
}

void CGameEffectMgr::Update(float fTime)
{
	NiTListIterator kPos = m_kActiveParticles.GetHeadPos();
	while (kPos)
	{
		IGameEffect* pkPE = m_kActiveParticles.GetNext(kPos);

		if (!pkPE->IsActive())
		{
			m_kActiveParticles.Remove(pkPE);

			if(!(pkPE->GetType() & IGameEffect::ET_ONLY))
			{
				m_kInActiveParticles[pkPE->GetIndex()].AddTail(pkPE);
			}

		}
		else
		{
			pkPE->Update(fTime,NULL);
		}
	}        
}

void CGameEffectMgr::Render(NiCamera* pkCamera)
{
	NiTListIterator kPos = m_kActiveParticles.GetHeadPos();
	while (kPos)
	{
		IGameEffect* pkPE = m_kActiveParticles.GetNext(kPos);

		if(!(pkPE->GetType() & IGameEffect::ET_SCREEN))
		{
			pkPE->Render(pkCamera);
		}
	}    
}

void CGameEffectMgr::ScreenRender(NiCamera* pkCamera)
{
	NiRenderer * pkRenderer = NiRenderer::GetRenderer();
	if (!pkRenderer) 
	{
		return;
	}

	pkRenderer->ClearBuffer(NULL, NiRenderer::CLEAR_ZBUFFER);

	NiTListIterator kPos = m_kActiveParticles.GetHeadPos();
	while (kPos)
	{
		IGameEffect* pkPE = m_kActiveParticles.GetNext(kPos);

		if((pkPE->GetType() & IGameEffect::ET_SCREEN))
		{
			pkPE->Render(pkCamera);
		}
	}
}


//////////////////////////////////////////////////////////////////////////

void ISnowEffect::Render(NiCamera* spCamera)
{
	return;
	if(m_bShow)
	{
		//NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode);
		NiDrawScene(spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
	}
}

IGameEffect* ISnowEffect::Clone()
{
	return this;
}


void ISnowEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = 
		RMOperator::OpenNifAndPrepack("effect/SnowSplash.nif");

	FORMAT_NODE(m_spEffectNode);

	m_spCamera = NULL;
}

void ISnowEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	return;
	if(m_bShow)
	{
		float sfTime = GetMainApp()->GetCurrentTime() - m_fStartTime;

		if(sfTime >= 30.0f)
		{
			m_bShow = false;
			m_fStartTime = GetMainApp()->GetCurrentTime();
		}
	}
	else
	{
		float sfTime = GetMainApp()->GetCurrentTime() - m_fStartTime;

		if(sfTime >= 60.0f)
		{
			m_bShow = true;
			m_fStartTime = GetMainApp()->GetCurrentTime();
		}
	}


	if(m_bShow)
	{
		NiPoint3 np3 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetTranslate();
		if(m_spEffectNode)
		{
			m_spEffectNode->SetTranslate(np3 + NiPoint3(0.0f,0.0f,20.0f));

			m_spEffectNode->Update(fTime);
		}
	}

}



//////////////////////////////////////////////////////////////////////////
ISpeedEffect::~ISpeedEffect()
{
	if (m_pSpeedLine)
	{
		delete m_pSpeedLine;
		m_pSpeedLine = NULL;
	}
}

void ISpeedEffect::Render(NiCamera* spCamera)
{
	if(m_bShow)
	{
//		NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera,m_spEffectNode);
		NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
	}
}

IGameEffect* ISpeedEffect::Clone()
{
	return this;
}

void ISpeedEffect::CoreLoad(NiNodePtr spNode)
{
//	m_spCamera = NxSceneMgr::Get()->GetCameraController()->GetForeCam()->GetCamera();

	m_pSpeedLine = new CSpeedLine(8, NULL);

	//CLevelManager::Get()->GetRootNode()->AttachChild(m_pSpeedLine->GetParticleRoot());
	m_spEffectNode = m_pSpeedLine->GetParticleRoot();
		//RMOperator::OpenNifAndPrepack("effect/acc_line.nif");

	//FindAndAdd(m_spEffectNode);

	FORMAT_NODE(m_spEffectNode);

	

	//m_spCamera = (NiCamera*) ((NiNode*)m_spEffectNode->GetObjectByName("Camera01"))->GetAt(0);

}
void ISpeedEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	//if (!m_spEffectNode->GetParent())
	//{
	//	CLevelManager::Get()->GetRootNode()->AttachChild(m_spEffectNode);
	//}
	float speed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * GameSetting::SpeedDispCoef;

	float fSpeedX = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocityLocal().x;
//TODO:....
	//NxWheel2* pxW0 = (NxWheel2*)NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getWheel(0);
	//if (pxW0)
	//{

	//	NxReal fRoundAngle = pxW0->wheelShape->getAxleSpeed();
	//	NxReal fRadius = pxW0->wheelShape->getRadius();
	//	speed = fRadius* fRoundAngle*5;
	//}

	//if(speed >= GameSetting::SpeedLineShowVelo)
	//{
	//	m_bShow = true;
	//}
	//else
	//{
	//	m_bShow = false;
	//}

	m_bShow = true;

	if(m_bShow)
	{
		if(m_spEffectNode)
		{
			m_spCamera = NxSceneMgr::Get()->GetCameraController()->GetForeCam()->GetCamera();
			m_pSpeedLine->Update(fTime, m_spCamera, fSpeedX);
			m_spEffectNode->Update(fTime);
		}
	}
}



CAngleDrifting::CAngleDrifting()
:IGameEffect(ET_ANGLEDRIFT)
{
	m_fLiveingTime = 5000.0f;
	m_fDriftingStartTime = 0.0f;
}

CAngleDrifting::~CAngleDrifting()
{
	m_spEffectNode = 0;
}

void CAngleDrifting::Render( NiCamera * spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode);
	NiDrawScene(spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
IGameEffect * CAngleDrifting::Clone()
{
	return this;
}

void CAngleDrifting::CoreUpdate( float fTime, NiPoint3* pPos)
{
	NiPoint3 np3 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetTranslate();


	if(m_spEffectNode)
	{
		m_spEffectNode->SetTranslate(np3);
		m_spEffectNode->Update(fTime);
	}
	if( ::GetTickCount() - m_fDriftingStartTime > m_fLiveingTime)
	{
		//CGameEffectMgr::DeActiveEffect(ET_ANGLEDRIFT);
		m_fDriftingStartTime = 0.0f;
		return;
	}
}

void CAngleDrifting::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("effect/AngleDrifting.nif");
	FindAndAdd(m_spEffectNode);
	FORMAT_NODE(m_spEffectNode);
	m_spEffectNode->SetScale(0.01f);
	m_spCamera = NULL;
}
void CAngleDrifting::Start()
{
	m_spEffectNode->SetTranslate(m_nPos);
	m_spEffectNode->Update(0);
	m_fDriftingStartTime = ::GetTickCount();
	IGameEffect::Start();
}

CAngleDriftingCenter::CAngleDriftingCenter()
:IGameEffect(ET_ANGLEDRIFTCENTER)
{
	m_fLiveingTime = 10000.0f;
	m_fDriftingStartTime = 0.0f;
}

CAngleDriftingCenter::~CAngleDriftingCenter()
{
	m_spEffectNode = 0 ;
}
void CAngleDriftingCenter::Render( NiCamera * spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode);
	NiDrawScene(spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
IGameEffect * CAngleDriftingCenter::Clone()
{
	return this;
}

void CAngleDriftingCenter::CoreUpdate( float fTime, NiPoint3* pPos)
{
	if(m_spEffectNode)
	{
		m_spEffectNode->Update(fTime);
		if( ::GetTickCount() - m_fDriftingStartTime > m_fLiveingTime)
		{
			//CGameEffectMgr::DeActiveEffect(ET_ANGLEDRIFT);
			m_fDriftingStartTime = 0.0f;
			return;
		}
	}
}

void CAngleDriftingCenter::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("effect/AngleDriftingCenter.nif");
	FindAndAdd(m_spEffectNode);
	FORMAT_NODE(m_spEffectNode);
	m_spEffectNode->SetScale(0.01f);
	m_spCamera = NULL;
}
void CAngleDriftingCenter::Start()
{
	m_spEffectNode->SetTranslate(m_nPos);
	m_spEffectNode->Update(0);
	m_fDriftingStartTime = ::GetTickCount();
	IGameEffect::Start();
}

//////////////////////////////////////////////////////////////////////////

void ICrashEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode);
	NiDrawScene(spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}

IGameEffect* ICrashEffect::Clone()
{
	return this;
}

void ICrashEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = 
		RMOperator::OpenNifAndPrepack("effect/xing.nif");


	FindAndAdd(m_spEffectNode);

//	NxSceneMgr::Get()->GetActiveSceneLevel()->LightObject(
//		m_spEffectNode);
	m_spEffectNode->UpdateProperties();
	m_spEffectNode->UpdateEffects();
	m_spEffectNode->Update(0);

	m_spCamera = NULL;
}

void ICrashEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	NiPoint3 np3 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetTranslate();

	if(m_spEffectNode)
	{
		m_spEffectNode->SetTranslate(np3);
		m_spEffectNode->Update(fTime);
	}
}


//////////////////////////////////////////////////////////////////////////

void IAccEffect::Render(NiCamera* spCamera)
{

	//NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode);
	NiDrawScene(spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}

IGameEffect* IAccEffect::Clone()
{
	return this;
}

void IAccEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = 
		RMOperator::OpenNifAndPrepack("effect/acc_show.nif");


	FindAndAdd(m_spEffectNode);

	//m_spEffectNode->SetScale(0.15f);
	FORMAT_NODE(m_spEffectNode);

	m_spCamera = NULL;
}

void IAccEffect::Start()
{
	//LevelManager::Get()->GetPlay()->GetNIFRoot()->AttachChild(m_spEffectNode);
	//FORMAT_NODE(LevelManager::Get()->GetPlay()->GetNIFRoot());
	IGameEffect::Start();
}

void IAccEffect::Stop()
{
	//LevelManager::Get()->GetPlay()->GetNIFRoot()->DetachChild(m_spEffectNode);
	IGameEffect::Stop();
}

void IAccEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode)
	{
		NiNode * pkNode = NxPlayMgr::GetHostPlay()->GetKartNIFRoot();
		NiPoint3  kTranslate = pkNode->GetTranslate();
		NiMatrix3 kRotate    = pkNode->GetRotate();
		NiPoint3 kVehicleDir;
		kRotate.GetCol(1, kVehicleDir);
		m_spEffectNode->SetTranslate(kTranslate - kVehicleDir * 0.5f);
		m_spEffectNode->SetRotate(pkNode->GetRotate());
		m_spEffectNode->Update(fTime);
	}

/*
	NiPoint3 n1 = NxPlayMgr::GetHostPlay()->GetNIFRoot()->GetTranslate();

	if(m_spEffectNode)
	{
		m_spEffectNode->SetTranslate(n1);
		m_spEffectNode->SetRotate(NxPlayMgr::GetHostPlay()->GetNIFRoot()->GetRotate());
		m_spEffectNode->Update(fTime);
	}
*/
}


//////////////////////////////////////////////////////////////////////////

IBackFireEffect::IBackFireEffect()  : IGameEffect(ET_BACKFIRE) 
{
	m_spEffectNode2 = NULL;
	m_spRoot = NiNew NiNode;

	m_spAmbientLight = NiNew NiAmbientLight;

	m_spAmbientLight->SetAmbientColor(NiColor::WHITE);
	m_spAmbientLight->SetDiffuseColor(NiColor::WHITE);
	m_spAmbientLight->SetSpecularColor(NiColor::WHITE);


	m_spAmbientLight->AttachAffectedNode(m_spRoot);


	m_spRoot->AttachChild(m_spAmbientLight);

	m_spRoot->UpdateEffects();
	m_spRoot->UpdateProperties();
	m_spRoot->Update(0.0f);

}
void IBackFireEffect::Render(NiCamera* spCamera)
{
	//NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode);
	//NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode2);
	//NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode3);
	//NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera,m_spEffectNode4);
	NiDrawScene(spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
	NiDrawScene(spCamera,m_spEffectNode2, GetMainApp()->GetDefaultCuller());
	NiDrawScene(spCamera,m_spEffectNode3, GetMainApp()->GetDefaultCuller());
	NiDrawScene(spCamera,m_spEffectNode4, GetMainApp()->GetDefaultCuller());
}

IGameEffect* IBackFireEffect::Clone()
{
	return this;
}

void IBackFireEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("effect/backfire_big.nif");


//	m_spEffectNode->SetScale(2.0f);

	m_spEffectNode2 = (NiNode*) m_spEffectNode->Clone();

//	m_spEffectNode2->SetScale(2.0f);

	m_spEffectNode3 = RMOperator::OpenNifAndPrepack("effect/blueflame.nif");

	m_spEffectNode4 = (NiNode*) m_spEffectNode3->Clone();

//	m_spEffectNode3->SetScale(2.0f);

//	m_spEffectNode4->SetScale(2.0f);



	m_spCamera = NULL;

	m_spRoot->AttachChild(m_spEffectNode);
	m_spRoot->AttachChild(m_spEffectNode2);
	m_spRoot->AttachChild(m_spEffectNode3);
	m_spRoot->AttachChild(m_spEffectNode4);

	m_spRoot->UpdateEffects();
	m_spRoot->UpdateProperties();
	m_spRoot->Update(0.0f);
}

void IBackFireEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{

	NiTransform n1 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_1")->GetWorldTransform();
	NiTransform n2 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_2")->GetWorldTransform();

	NiTransform n3 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_3")->GetWorldTransform();
	NiTransform n4 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_4")->GetWorldTransform();



	if(m_spEffectNode)
	{
		m_spEffectNode->SetTranslate(n1.m_Translate);
		m_spEffectNode->SetRotate(n1.m_Rotate);

		m_spEffectNode2->SetTranslate(n2.m_Translate);
		m_spEffectNode2->SetRotate(n2.m_Rotate);


		m_spEffectNode3->SetTranslate(n3.m_Translate);
		m_spEffectNode3->SetRotate(n3.m_Rotate);

		m_spEffectNode4->SetTranslate(n4.m_Translate);
		m_spEffectNode4->SetRotate(n4.m_Rotate);

		m_spRoot->Update(GetMainApp()->GetAccumTime() - m_fStartTime);
	}
}

void IBackFireEffect::Start()
{
	NiTimeController::StartAnimations(m_spEffectNode2);
	NiTimeController::StartAnimations(m_spEffectNode3);
	NiTimeController::StartAnimations(m_spEffectNode4);


	m_spEffectNode->Update(0.0f);
	m_spEffectNode2->Update(0.0f);
	m_spEffectNode3->Update(0.0f);
	m_spEffectNode4->Update(0.0f);

	IGameEffect::Start();

}

IGameUnFinishEffect::IGameUnFinishEffect()
:IGameEffect(ET_UNFINISHTEXT) 
{
}
IGameUnFinishEffect::~IGameUnFinishEffect()
{
	m_spEffectNode =  0;
}

IGameEffect * IGameUnFinishEffect::Clone()
{
	return this;
}
void IGameUnFinishEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/unfinishtitle.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}
void IGameUnFinishEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}

void IGameUnFinishEffect::Render(NiCamera * spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera, m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}

//-----------------------------------------------------------------------

IGameFinishEffect::IGameFinishEffect()
:IGameEffect(ET_FINISHTEXT)
{
}
IGameFinishEffect::~IGameFinishEffect()
{
	m_spEffectNode = 0;
}
IGameEffect * IGameFinishEffect::Clone()
{
	return this;
}
void IGameFinishEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/finishtitle.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void IGameFinishEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void IGameFinishEffect::Render(NiCamera * spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera, m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-----------------------------------------------------------------------

IGamePerfectEffect::IGamePerfectEffect()
:IGameEffect(ET_PERFECT)
{
}
IGamePerfectEffect::~IGamePerfectEffect()
{
	m_spEffectNode = 0;
}
IGameEffect * IGamePerfectEffect::Clone()
{
	return this;
}
void IGamePerfectEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/perfect.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void IGamePerfectEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void IGamePerfectEffect::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-----------------------------------------------------------------------

IGameOverCountdownEffect::IGameOverCountdownEffect()
:IGameEffect(ET_OVERCOUNTDOWN),
m_bStarting(false)
{
	m_nSoundTimes = ::GetTickCount();
	
}

IGameOverCountdownEffect::~IGameOverCountdownEffect()
{
	m_spEffectNode =0;
}
IGameEffect * IGameOverCountdownEffect::Clone()
{
	return this;
}
void IGameOverCountdownEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/enddaoshu.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}
void IGameOverCountdownEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{

	if ( m_bActive && !m_bStarting){
		m_bStarting = true;

	}

	if ( m_bActive ) {
		if(m_spEffectNode) {
			m_spEffectNode->Update(fTime);
		}

		if ( ::GetTickCount() - m_nSoundTimes > 1000 ) {
			//NxSceneMgr::Get()->GetSoundManager()->StopSound(SoundManager::ST_GO);
			//NxSceneMgr::Get()->GetSoundManager()->SetPlaybackRate(SoundManager::ST_GO, 25500);
			//NxSceneMgr::Get()->GetSoundManager()->PlaySound(SoundManager::ST_GO);
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_GO);
			CSoundMgr::Get()->SetPlaybackRate(CSoundMgr::ST_GO, 25500);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_GO);
			
			m_nSoundTimes = ::GetTickCount();
		}
	}

	// 临时这么做将来会加入 动画播放完毕的回调处理
	if ( !m_bActive && m_bStarting )
	{
		if (!NxPlayMgr::GetHostPlay()->IsWin())
		{
			CKartStatusManager::Get()->OnTriggerRaceFinish(RACE_FINISH_TIMEOUT, 0);
			m_bStarting = false;
			m_nSoundTimes = ::GetTickCount();
		}
	}
}

void IGameOverCountdownEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera, m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}

//-----------------------------------------------------------------------

IGameStartCountdownEffect::IGameStartCountdownEffect()
: IGameEffect(ET_STARTCOUNTDOWN)
{
}

IGameStartCountdownEffect::~IGameStartCountdownEffect()
{
	m_spEffectNode = 0;
}
void IGameStartCountdownEffect::Render(NiCamera* spCamera)
{
	NiD3DRenderer * pkRender = (NiD3DRenderer*)NiRenderer::GetRenderer();
	pkRender->GetRenderState()->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera,m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
	pkRender->GetRenderState()->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
}
IGameEffect * IGameStartCountdownEffect::Clone()
{
	return this;
}
//	增加人:	wincoqin
//  时间:	2007/02/05
//  目的:	去除ZBufferTest属性
void RecurviseCloseZBufferProp(NiNode * pkNode)
{
	NiZBufferProperty * pkZbufferProp = (NiZBufferProperty *)pkNode->GetProperty(NiProperty::ZBUFFER);

	if (pkZbufferProp) 
	{
		pkZbufferProp->SetZBufferTest(false);
		pkZbufferProp->SetZBufferWrite(false);
	}
	else
	{
		NiZBufferPropertyPtr spZbufferProp = NiNew NiZBufferProperty;
		spZbufferProp->SetZBufferTest(false);
		spZbufferProp->SetZBufferWrite(false);
		pkNode->AttachProperty(spZbufferProp);
		pkNode->UpdateProperties();
	}

	unsigned int unChildNum = pkNode->GetChildCount();

	for (unsigned int i = 0; i < unChildNum; ++i)
	{
		NiAVObject * pkObject = pkNode->GetAt(i);

		if (pkObject) 
		{
			if (NiIsKindOf(NiNode, pkObject))
				RecurviseCloseZBufferProp((NiNode*)pkObject);
			else
			{
				NiZBufferProperty * pkZbufferProp = (NiZBufferProperty *)pkObject->GetProperty(NiProperty::ZBUFFER);

				if (pkZbufferProp) 
				{
					pkZbufferProp->SetZBufferTest(false);
					pkZbufferProp->SetZBufferWrite(false);
				}
				else
				{
					NiZBufferPropertyPtr spZbufferProp = NiNew NiZBufferProperty;
					spZbufferProp->SetZBufferTest(false);
					spZbufferProp->SetZBufferWrite(false);
					pkObject->AttachProperty(spZbufferProp);
					pkObject->UpdateProperties();
				}
			}
		}
	}
}

void IGameStartCountdownEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/321go.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		RecurviseCloseZBufferProp(m_spEffectNode);

		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void IGameStartCountdownEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}


//-----------------------------------------------------------------------

IFirstLapEffect::IFirstLapEffect()
:IGameEffect(ET_FIRSTLAP)
{
}
IFirstLapEffect::~IFirstLapEffect()
{
	m_spEffectNode = 0;
}
void IFirstLapEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera,m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
IGameEffect * IFirstLapEffect::Clone()
{
	return this;
}
void IFirstLapEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/321go.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void IFirstLapEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
//-----------------------------------------------------------------------

ISecondLapEffect::ISecondLapEffect()
:IGameEffect(ET_SECONDLAP)
{
}
ISecondLapEffect::~ISecondLapEffect()
{
	m_spEffectNode = 0;
}
void ISecondLapEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera,m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
IGameEffect * ISecondLapEffect::Clone()
{
	return this;
}
void ISecondLapEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/d2q.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void ISecondLapEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

ILastLapEffect::ILastLapEffect()
:IGameEffect(ET_LASTLAP)
{
}
ILastLapEffect::~ILastLapEffect()
{
	m_spEffectNode = 0;
}
void ILastLapEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera,m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
IGameEffect * ILastLapEffect::Clone()
{
	return this;
}
void ILastLapEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/d3q.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void ILastLapEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}

//-----------------------------------------------------------------------

IWrongWayEffect::IWrongWayEffect()
:IGameEffect(ET_WRONGWAY)
{
}
IWrongWayEffect::~IWrongWayEffect()
{
	m_spEffectNode = 0;
}
void IWrongWayEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera,m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
IGameEffect * IWrongWayEffect::Clone()
{
	return this;
}
void IWrongWayEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/wrongway.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void IWrongWayEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}



//-----------------------------------------------------------------------

ICaiDaiEffect::ICaiDaiEffect()
	: IGameEffect(ET_CAIDAI)
{

}

ICaiDaiEffect::~ICaiDaiEffect()
{
	m_spEffectNode = 0;
}

void ICaiDaiEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera, m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}

IGameEffect * ICaiDaiEffect::Clone()
{
	return this;
}

void ICaiDaiEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/caidai.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}

}

void ICaiDaiEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode)
	{

		m_spEffectNode->Update(fTime);
	}
}



ISmokeEffect::ISmokeEffect():IGameEffect(ET_SMOKE)
{
	m_spSmokeEmitCtr1 = 0;
	m_spSmokeEmitCtr2 = 0;

	m_spSmokeEmitter1 = 0;	
	m_spSmokeEmitter2 = 0;

	m_spSmokeEffNode2 = 0;
	m_spEffectNode = 0;
	m_spRoot = NiNew NiNode;
	m_spAmbientLight = NiNew NiAmbientLight;

	m_spAmbientLight->SetAmbientColor(NiColor::WHITE);
	m_spAmbientLight->SetSpecularColor(NiColor::WHITE);
	m_spAmbientLight->SetDiffuseColor(NiColor::WHITE);

	m_spAmbientLight->AttachAffectedNode(m_spRoot);

	m_spRoot->AttachChild(m_spAmbientLight);

	m_spRoot->UpdateEffects();
	m_spRoot->UpdateProperties();
	m_spRoot->Update(0);
}
ISmokeEffect::~ISmokeEffect()
{
	m_spAmbientLight = 0;

	m_spSmokeEmitCtr1 = 0;
	m_spSmokeEmitCtr2 = 0;

	m_spSmokeEmitter1 = 0;	
	m_spSmokeEmitter2 = 0;

	m_spSmokeEffNode2 = 0;
	m_spEffectNode = 0;
	m_spRoot = 0;
}

IGameEffect* ISmokeEffect::Clone()
{
	return this;

}
void ISmokeEffect::Render(NiCamera* spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera, m_spEffectNode);
//	NxSceneMgr::Get()->GetActiveSceneLevel()->PartialRender(spCamera, m_spSmokeEffNode2);
	NiDrawScene(spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
	NiDrawScene(spCamera,m_spSmokeEffNode2, GetMainApp()->GetDefaultCuller());
}

void ISmokeEffect::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("effect/smoke.nif");
	m_spSmokeEffNode2 = (NiNode*) m_spEffectNode->Clone();
	NiParticleSystemPtr pkSmokePSys = NiDynamicCast(NiParticleSystem, m_spEffectNode->GetObjectByName("SuperSpray01"));
	m_spSmokeEmitter1 = NiDynamicCast(NiPSysBoxEmitter, pkSmokePSys->GetModifierAt(1));
	m_fInitEmitSpeed = m_spSmokeEmitter1->GetSpeed();

	m_spSmokeEmitCtr1 = NiGetController(NiPSysEmitterCtlr, pkSmokePSys);
	m_fInitEmitFrequence = m_spSmokeEmitCtr1->GetFrequency();

	pkSmokePSys =  NiDynamicCast(NiParticleSystem, m_spSmokeEffNode2->GetObjectByName("SuperSpray01"));
	m_spSmokeEmitter2 = NiDynamicCast(NiPSysBoxEmitter, pkSmokePSys->GetModifierAt(1));
	m_spSmokeEmitCtr2 = NiGetController(NiPSysEmitterCtlr, pkSmokePSys);

	m_spRoot->AttachChild(m_spEffectNode);
	m_spRoot->AttachChild(m_spSmokeEffNode2);
	m_spRoot->UpdateNodeBound();

	m_spRoot->UpdateEffects();
	m_spRoot->UpdateProperties();
	m_spRoot->Update(0.0f);

}

void ISmokeEffect::CoreUpdate(float fTime,NiPoint3* pPos)
{
	NiTransform n3 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_3")->GetWorldTransform();
	NiTransform n4 = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetObjectByName("tailpipe_4")->GetWorldTransform();

	m_spEffectNode->SetTranslate(n3.m_Translate);
	m_spEffectNode->SetRotate(n3.m_Rotate);

	m_spSmokeEffNode2->SetTranslate(n4.m_Translate);
	m_spSmokeEffNode2->SetRotate(n4.m_Rotate);

	if(! NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle())
		return ;

	float curSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity() * 5.0f;

	if(curSpeed > 100)
	{
		m_fCurEmitSpeed = 2.0;
	}
	else if( curSpeed > 0)
	{
		m_fCurEmitSpeed = (curSpeed / 100 + 1) * m_fInitEmitSpeed;

	}
	else
	{
		m_fCurEmitSpeed = m_fInitEmitSpeed;
	}

//	m_spSmokeEmitCtr1->SetFrequency(m_fCurEmitSpeed * m_fInitEmitFrequence);
//	m_spSmokeEmitCtr2->SetFrequency(m_fCurEmitSpeed * m_fInitEmitFrequence);
	m_spSmokeEmitter1->SetSpeed(m_fCurEmitSpeed);
	m_spSmokeEmitter2->SetSpeed(m_fCurEmitSpeed);
	m_spRoot->Update(GetMainApp()->GetAccumTime() - m_fStartTime);


}


void ISmokeEffect::Start()
{
	m_bActive = true;
	m_fStartTime = GetMainApp()->GetAccumTime();
	NiTimeController::StartAnimations(m_spEffectNode);
	NiTimeController::StartAnimations(m_spSmokeEffNode2);
	m_spEffectNode->Update(0.0);
	m_spSmokeEffNode2->Update(0.0);
}

CStopShow::CStopShow()
: IGameEffect(ET_STOP)
{
	m_spCamera     = NULL;
	m_spEffectNode = NULL;
}
CStopShow::~CStopShow()
{
	m_spCamera     = NULL;
	m_spEffectNode = NULL;
}

void CStopShow::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/stop.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

IGameEffect * CStopShow::Clone()
{
	return this;
}

void CStopShow::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode)
	{
		m_spEffectNode->Update(fTime);
	}
}

void CStopShow::Render(NiCamera * spCamera)
{
//	NxSceneMgr::Get()->GetActiveSceneLevel()->ImmRender(m_spCamera, m_spEffectNode);
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-----------------------------------------------------------------------------
CPressUP_ET::CPressUP_ET()
:IGameEffect(ET_PRESSUP)
{
}
CPressUP_ET::~CPressUP_ET()
{
	m_spEffectNode = 0;
}
IGameEffect * CPressUP_ET::Clone()
{
	return this;
}
void CPressUP_ET::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/pressup.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_sInfo.CollectData(m_spEffectNode);
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CPressUP_ET::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CPressUP_ET::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//------------------------------------------------------------------------------
CPressDown_ET::CPressDown_ET()
:IGameEffect(ET_PRESSDOWN)
{
}
CPressDown_ET::~CPressDown_ET()
{
	m_spEffectNode = 0;
}
IGameEffect * CPressDown_ET::Clone()
{
	return this;
}
void CPressDown_ET::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/pressdown.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_sInfo.CollectData(m_spEffectNode);
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CPressDown_ET::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CPressDown_ET::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------
CPressLeft_ET::CPressLeft_ET()
:IGameEffect(ET_PRESSLEFT)
{	
}
CPressLeft_ET::~CPressLeft_ET()
{
	m_spEffectNode = 0;
}
IGameEffect * CPressLeft_ET::Clone()
{
	return this;
}
void CPressLeft_ET::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/pressleft.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_sInfo.CollectData(m_spEffectNode);
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CPressLeft_ET::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CPressLeft_ET::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------
CPressRight_ET::CPressRight_ET()
:IGameEffect(ET_PRESSRIGHT)
{
}
CPressRight_ET::~CPressRight_ET()
{
	m_spEffectNode = 0;
}
IGameEffect * CPressRight_ET::Clone()
{
	return this;
}
void CPressRight_ET::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/pressright.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CPressRight_ET::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CPressRight_ET::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------
CTaskFinish_ET::CTaskFinish_ET()
:IGameEffect(ET_TASKFINISH)
{
}
CTaskFinish_ET::~CTaskFinish_ET()
{
	m_spEffectNode = 0;
}
IGameEffect * CTaskFinish_ET::Clone()
{
	return this;
}
void CTaskFinish_ET::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/taskfinish.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CTaskFinish_ET::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CTaskFinish_ET::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------
CDriftTask_Step1::CDriftTask_Step1()
:IGameEffect(ET_DRIFT_STEP1)
{
}
CDriftTask_Step1::~CDriftTask_Step1()
{
	m_spEffectNode = 0;
}
IGameEffect * CDriftTask_Step1::Clone()
{
	return this;
}
void CDriftTask_Step1::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/drifttask_step1.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CDriftTask_Step1::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CDriftTask_Step1::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------
CDriftTask_Step2::CDriftTask_Step2()
:IGameEffect(ET_DRIFT_STEP2)
{
}
CDriftTask_Step2::~CDriftTask_Step2()
{
	m_spEffectNode = 0;
}
IGameEffect * CDriftTask_Step2::Clone()
{
	return this;
}
void CDriftTask_Step2::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/drifttask_step2.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CDriftTask_Step2::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CDriftTask_Step2::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------
CDriftTask_Step3::CDriftTask_Step3()
:IGameEffect(ET_DRIFT_STEP3)
{
}
CDriftTask_Step3::~CDriftTask_Step3()
{
	m_spEffectNode = 0;
}
IGameEffect * CDriftTask_Step3::Clone()
{
	return this;
}
void CDriftTask_Step3::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/drifttask_step3.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CDriftTask_Step3::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CDriftTask_Step3::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------
CDriftTask_Step4::CDriftTask_Step4()
:IGameEffect(ET_DRIFT_STEP4)
{
}
CDriftTask_Step4::~CDriftTask_Step4()
{
	m_spEffectNode = 0;
}
IGameEffect * CDriftTask_Step4::Clone()
{
	return this;
}
void CDriftTask_Step4::CoreLoad(NiNodePtr spNode)
{
	m_spEffectNode = RMOperator::OpenNifAndPrepack("Map/drifttask_step4.nif");

	FindAndAdd(m_spEffectNode);

	assert(m_spEffectNode);
	if (m_spEffectNode)
	{
		m_spCamera = NiDynamicCast(NiCamera, m_spEffectNode->GetObjectByName("Camera01"));
		assert(m_spCamera);

		m_spEffectNode->UpdateEffects();
		m_spEffectNode->UpdateProperties();
		m_spEffectNode->Update(0.0f);
	}
}

void CDriftTask_Step4::CoreUpdate(float fTime,NiPoint3* pPos)
{
	if(m_spEffectNode) {
		m_spEffectNode->Update(fTime);
	}
}
void CDriftTask_Step4::Render(NiCamera * spCamera)
{
	NiDrawScene(m_spCamera,m_spEffectNode, GetMainApp()->GetDefaultCuller());
}
//-------------------------------------------------------------------