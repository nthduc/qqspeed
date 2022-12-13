#include "Top-KartPCH.h"

#include "CameraController.h"


#define ADD_CAM(ID, T) \
{\
	T * p = NiNew T ; \
	m_apkCamera[ID] = p; \
}\



CCameraController::CCameraController()
{
	m_unForeCamID = 0;
	for(unsigned int i = 0; i < CCameraBase::MAX_CAM_TYPES; ++i)
	{
		m_apkCamera[i] = NULL;
		m_bActiveCam[i] = false;
	}
	Init();
}


CCameraController::~CCameraController()
{
	for(unsigned int i = 0; i < CCameraBase::MAX_CAM_TYPES; ++i)
	{
		SAFE_NIDELETE(m_apkCamera[i]);
	}
}


void CCameraController::Init()
{
	ADD_CAM(CCameraBase::ECT_RACE,		CRaceCamera);
	ADD_CAM(CCameraBase::ECT_MODEL,		CModelCamera);
	ADD_CAM(CCameraBase::ECT_AIRSCAPE,	CAirScapeCamera);
	ADD_CAM(CCameraBase::ECT_EAGLEMAP,	CEagleMapCamera);
}


void CCameraController::AddLogicCamera(unsigned int eCamType, NiCameraPtr spCam)
{
	switch(eCamType)
	{
	case CCameraBase::ECT_RACE:
		{
			if(!m_apkCamera[eCamType])
			{
				m_apkCamera[eCamType] = NiNew CRaceCamera;
			}
			m_apkCamera[eCamType]->SetCamera(spCam);
			break;
		}

	case CCameraBase::ECT_MODEL:
		{
			if(!m_apkCamera[eCamType])
			{
				m_apkCamera[eCamType] = NiNew CModelCamera;
			}
			m_apkCamera[eCamType]->SetCamera(spCam);
			break;
		}

	case CCameraBase::ECT_RADAR:
		{
			if(!m_apkCamera[eCamType])
			{
				m_apkCamera[eCamType] = NiNew CRaceCamera;
			}
			m_apkCamera[eCamType]->SetCamera(spCam);
			break;
		}

	case CCameraBase::ECT_AIRSCAPE:
		{
			if(!m_apkCamera[eCamType])
			{
				m_apkCamera[eCamType] = NiNew CAirScapeCamera;
			}
			m_apkCamera[eCamType]->SetCamera(spCam);
			break;
		}

	case CCameraBase::ECT_EAGLEMAP:
		{
			if(!m_apkCamera[eCamType])
			{
				m_apkCamera[eCamType] = NiNew CEagleMapCamera;
			}
			m_apkCamera[eCamType]->SetCamera(spCam);
			break;
		}


	}
}


void CCameraController::AddLogicCamera(CCameraBase* pkCamBase)
{
	if(!pkCamBase)
	{
		return;
	}

	m_apkCamera[pkCamBase->GetCameraType()] = pkCamBase;
}

CCameraBase* CCameraController::GetLogicCamera(unsigned int eCamType)
{
	return m_apkCamera[eCamType];
}

void CCameraController::SetForeCam(unsigned int eCamType)
{
	m_bActiveCam[m_unForeCamID] = false;
	m_bActiveCam[eCamType] = true;
	m_unForeCamID = eCamType;
}



CCameraBase* CCameraController::GetForeCam()
{
	return m_apkCamera[m_unForeCamID];
}


void CCameraController::ActivateLogicCam(unsigned int eCamType)
{
	m_bActiveCam[eCamType] = true;
}


void CCameraController::DeactivateLogicCam(unsigned int eCamType)
{
	m_bActiveCam[eCamType] = false;
}

void CCameraController::DeactivateAllCam()
{
	for(int i = 0; i < CCameraBase::MAX_CAM_TYPES; ++i)
	{
		m_bActiveCam[i] = false;
	}
}

void CCameraController::Update(float fTime)
{
	for( unsigned int i = 0; i < CCameraBase::MAX_CAM_TYPES; ++i)
	{
		if(m_bActiveCam[i])
		{
			if(m_apkCamera[i])
			{
				m_apkCamera[i]->Update(fTime);
			}			
		}
	}	
}

void CCameraController::LoadConfig()
{
	CScriptState ls;
	if(ls.DoFile("config/CameraCfg.lua") == 0)
	{
		for(int i = 0; i < CCameraBase::MAX_CAM_TYPES; ++i)
		{
			if(m_apkCamera[i])
			{
				m_apkCamera[i]->LoadConfig(&ls);
			}

		}
	}
}

void CCameraController::LoadConfig( const char * pFilePath )
{
	if ( !pFilePath )
	{
		return LoadConfig();
	}
	CScriptState ls;
	if ( ls.DoFile(pFilePath) == 0 )
	{
		for ( int i = 0; i < CCameraBase::MAX_CAM_TYPES; ++i )
		{
			if ( m_apkCamera[i])
			{
				m_apkCamera[i]->LoadConfig(&ls);
			}
		}
	}
}






