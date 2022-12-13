#include "Top-KartPCH.h"
#include "../PhysKart/NxAllVehicles.h"
#include "../Utility.h"
#include "../EventContainer.h"

#include "KartStatusManager.h"
#include "KartPlayback.h"
#include "../nxplaymgr.h"
#include "../NxSceneMgr.h"
#include "../SceneLevel.h"
#include "../levelmanager.h"

CKartPlayBack::CKartPlayBack(const KartInfo & kKartInfo)
: m_kKartInfo(kKartInfo)
, m_uiCurPos(0)
, m_bStartRecord(false)
, m_bPlayBack(false)
, m_fStartRecordTime(0.0f)
, m_fStartPlayBackTime(0.0f)
, m_fCurTime(0.0f)
{	
	CLevelManager::Get()->GetLinks()->Register(this);
}

CKartPlayBack::~CKartPlayBack()
{
	CLevelManager::Get()->GetLinks()->UnRegister(this);
	Clear();	
}

void CKartPlayBack::StartRecord(float fTime)
{
	m_fStartRecordTime = fTime;
	m_bStartRecord = true;
	m_bPlayBack = false;
	InputManager::Get()->Enable(true);
}

void CKartPlayBack::StopRecord(float fTime)
{
	m_bStartRecord = false;
}

void CKartPlayBack::OnKeyDown(const NiInputKeyboard::KeyCode keyCode, int status, 
							  float fTime)//const NxMat34 & kKartPose, 
{	
	if (m_bStartRecord)
	{
		//NxMat34 kKartPose = NxPlayMgr::GetHostPlay()->GetKart()->GetKartPose();
		//NxVec3  kKartSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getActor()->getLinearVelocity();
		//NxVec3  kKartAngleSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getActor()->getAngularVelocity();
		//Key * pKey = new Key;
		//pKey->enKeyCode  = keyCode;
		//pKey->iStatus	 = status;
		//pKey->fPressTime = fTime - m_fStartRecordTime;
		//pKey->kKartPose  = kKartPose;
		//pKey->kSpeed     = kKartSpeed;
		//pKey->kAngleSpeed= kKartAngleSpeed;
		//m_arrKeyRedord.push_back(pKey);
	}
}

bool CKartPlayBack::Save(const char* pcFilePath)
{
	if (m_bPlayBack)
		return false;
	FILE * fp = fopen(
		NiApplication::ConvertMediaFilename(pcFilePath), "w");

	if (fp)
	{
		WriteKartInfo(fp);
		WriteKeyRecord(fp);

		fclose(fp);
		fp = NULL;
		return true;
	}
	
	return false;
}

bool CKartPlayBack::Load(const char * filename, const char * fCameraFile)
{
	Clear();
	FILE * fp = fopen(
		NiApplication::ConvertMediaFilename(filename), "r");
	if (fp)
	{	
		ReadKartInfo(fp);
		ReadKeyRecord(fp);

		fclose(fp);
		fp = NULL;
		
	}
	else
        return false;

	NiStream kStream;

	if (!kStream.Load(NiApplication::ConvertMediaFilename(fCameraFile))) 
	{
		assert(0 && "Load Playback camera nif failed!!");
		return false;
	}

	NiNodePtr p = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	if (!p) {
		assert(0);
		return false;
	}
	FindAndAddCamera(p);

	return true;
}

void CKartPlayBack::Clear()
{
	memset(&m_kKartInfo, 0, sizeof(KartInfo));
	
	unsigned int uiNum = m_arrKeyRedord.size();
	for (unsigned int i = 0; i < uiNum; ++i) 
	{
		delete m_arrKeyRedord[i];
	}
	m_arrKeyRedord.clear();

	unsigned int uiSize = m_arrCamera.GetEffectiveSize();

	for (unsigned int i = 0; i < uiSize; ++i)
	{
		CameraPlayBack * pCamPlayBack = m_arrCamera.GetAt(i);
		assert(pCamPlayBack);
		pCamPlayBack->arrCheckPointName.clear();
		pCamPlayBack->spCamera = NULL;

		SAFE_NIDELETE(pCamPlayBack);
	}

	m_arrCamera.RemoveAll();
}

void CKartPlayBack::WriteKartInfo(FILE *& fp)
{
	assert(fp);
}

void CKartPlayBack::ReadKartInfo(FILE *& fp)
{
	assert(fp);
}

void CKartPlayBack::WriteKeyRecord(FILE *& fp)
{
	assert(fp);

	unsigned int uiNum = m_arrKeyRedord.size();
	for (unsigned int i = 0; i < uiNum; ++i) 
	{
		Key * pKey = m_arrKeyRedord[i];
		fprintf(fp, "%d %d %f ", (int)pKey->enKeyCode, pKey->iStatus, pKey->fPressTime);
		fprintf(fp, "%f %f %f ", pKey->kKartPose.t.x, pKey->kKartPose.t.y, pKey->kKartPose.t.z);
		NxVec3 vec;
		for (int j = 0; j < 3; ++j)
		{
			pKey->kKartPose.M.getColumn(j, vec);
			fprintf(fp, "%f %f %f ", vec.x, vec.y, vec.z);
		}
		fprintf(fp, "%f %f %f ", pKey->kSpeed.x, pKey->kSpeed.y, pKey->kSpeed.z);
		fprintf(fp, "%f %f %f ", pKey->kAngleSpeed.x, pKey->kAngleSpeed.y, pKey->kAngleSpeed.z);
	}
}

int CKartPlayBack::ReadKeyStroke(FILE *& fp, int & key, int & status, float & fPresstime)
{
	return fscanf(fp, "%d %d %f ", &key, &status, &fPresstime);
}

int CKartPlayBack::ReadKartPose(FILE *& fp, NxMat34 & kKartPose)
{
	int ret = EOF;
	ret = fscanf(fp, "%f %f %f ", &kKartPose.t.x, &kKartPose.t.y, &kKartPose.t.z);
	if (ret == EOF) 
	{
		return ret;
	}
	NxVec3 vec;
	for (int j = 0; j < 3; ++j)
	{
		ret = fscanf(fp, "%f %f %f ", &vec.x, &vec.y, &vec.z);
		if (ret == EOF) 
		{
			return ret;
		}
		kKartPose.M.setColumn(j, vec);
	}

	return ret;
}

void CKartPlayBack::ReadKeyRecord(FILE *& fp)
{
	assert(fp);
	int key = 0;
	int status;
	float fPressTime = 0.0f;
	NxMat34 kKartPose;
	NxVec3 kKartSpeed;
	NxVec3 kKartAngleSpeed;
	while (EOF != ReadKeyStroke(fp, key, status, fPressTime)
		&& EOF != ReadKartPose(fp, kKartPose)
		&& EOF != ReadKartSpeed(fp, kKartSpeed, kKartAngleSpeed)) 
	{
		Key * pKey = new Key;
		pKey->enKeyCode  = (NiInputKeyboard::KeyCode)key;
		pKey->iStatus	 = status;
		pKey->fPressTime = fPressTime;
		pKey->kKartPose  = kKartPose;
		pKey->kSpeed     = kKartSpeed;
		pKey->kAngleSpeed= kKartAngleSpeed;
		m_arrKeyRedord.push_back(pKey);
	}	
}

int CKartPlayBack::ReadKartSpeed(FILE *& fp, NxVec3 & kKartSpeed, NxVec3 & kKartAngleSpeed)
{
	assert(fp);

	int ret = fscanf(fp, "%f %f %f ", &kKartSpeed.x, &kKartSpeed.y, &kKartSpeed.z);

	ret = fscanf(fp, "%f %f %f ", &kKartAngleSpeed.x, &kKartAngleSpeed.y, &kKartAngleSpeed.z);

	return ret;
}

void CKartPlayBack::PlayBack(float fTime)
{
	DisableInput();
	Clear();
	Load("play.txt", "Map/Common Map/Map01/CamPlayBack.nif");
	m_bPlayBack = true;
	m_bStartRecord = false;
	m_uiCurPos = 0;
	m_fStartPlayBackTime = fTime;
}


void CKartPlayBack::DisableInput()
{
	InputManager::Get()->Enable(false);
}



void CKartPlayBack::Update(float fTime)
{
	if (!m_bPlayBack) 
	{
		return;
	}

	unsigned int uiSize = m_arrKeyRedord.size();

	if (uiSize == 0 || m_uiCurPos >= uiSize) 
	{
		return;
	}

	for (unsigned int i = 0; i < m_uiCurPos && i < uiSize; ++i)
	{
		Key * pKey = m_arrKeyRedord[i];
		InputManager::keys[pKey->enKeyCode] = false;
	}
	Key * pKey = m_arrKeyRedord[m_uiCurPos];
	float fPlayTime = fTime - m_fStartPlayBackTime;
	
	if (pKey && fPlayTime >= pKey->fPressTime)
		m_fCurTime = pKey->fPressTime;
	else
		return;
	while (m_uiCurPos < uiSize && pKey && fPlayTime >= (pKey->fPressTime))
	{		
		if (pKey->fPressTime > m_fCurTime) 
		{
			break;
		}
		
		switch(pKey->enKeyCode) {
		case NiInputKeyboard::KEY_F:
			{
				CKartStatusManager::Get()->OnTriggerExceptionEvent(KART_EXCEPTSTATE_RESET, NULL);
			}
			break;
		case NiInputKeyboard::KEY_W:
			{
				//EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_TURNLEFT);
			}
			break;
		case NiInputKeyboard::KEY_A:
			{
//				EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_TURNLEFT);
				if(NxPlayMgr::GetHostPlay())
					NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNLEFT);
			}
			break;
		case NiInputKeyboard::KEY_S:
			{
//				EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_TURNBACK);
				if(NxPlayMgr::GetHostPlay())
					NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNBACK);
			}
			break;
		case NiInputKeyboard::KEY_D:
			{
//				EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_TURNRIGHT);
				if(NxPlayMgr::GetHostPlay())
					NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_TURNRIGHT);
			}
			break;
		default:
			break;
		}
		
		// 模拟这个按键：）
		//NxPlayMgr::GetHostPlay()->GetKart()->SetKartPose(pKey->kKartPose);
		//NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getActor()->setLinearVelocity(pKey->kSpeed);
		//NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getActor()->setAngularVelocity(pKey->kAngleSpeed);
		//InputManager::keys[pKey->enKeyCode] = true;
		//++m_uiCurPos;
		//if (m_uiCurPos < uiSize)
		//	pKey = m_arrKeyRedord[m_uiCurPos];
	};	
}

float CKartPlayBack::GetPlayBackTime()
{
	return m_fCurTime;
}

bool CKartPlayBack::IsPlayBack()
{
	return m_bPlayBack;
}

unsigned int CKartPlayBack::GetCurKeyIndex()
{
	return m_uiCurPos;
}

void CKartPlayBack::OnLapUp()
{

}

bool CKartPlayBack::IsPlayBackCamera(NiCamera * pkCamera)
{
	unsigned int uiSize = m_arrCamera.GetEffectiveSize();

	for (unsigned int i = 0; i < uiSize; ++i)
	{
		CameraPlayBack * pCamPlayBack = m_arrCamera.GetAt(i);
		if (pCamPlayBack->spCamera == pkCamera) 
		{
			return true;
		}
	}

	return false;
}

void CKartPlayBack::OnEnterPoint(CCheckPoint * pChkPt)
{
	if (!m_bPlayBack) 
	{
		return;
	}

	int nIndex = -1;
	NiCameraPtr spCam = GetCamera(pChkPt->kName, nIndex);

	NiCamera * pCam = NxSceneMgr::Get()->GetCameraController()->GetForeCam()->GetCamera();
	assert(pCam);
	if (pCam == spCam) 
	{
		return;
	}
	if (spCam == NULL && IsPlayBackCamera(pCam))
	{
//		CLevelManager::Get()->UseCamera(CLevelManager::GameCamera);			
	}
	else
	{
//		if (nIndex == 0)
//			NxSceneMgr::Get()->GetCameraController()->SetCamera(spCam, CameraController::CCM_PLAYBACK);
		//else
		//	LevelManager::Get()->UseGameCamera();
	}
}

void CKartPlayBack::OnLeavePoint(CCheckPoint * pChkPt)
{

}

void CKartPlayBack::FindAndAddCamera(NiAVObject * pkNode)
{
	if (NiIsKindOf(NiCamera, pkNode))
	{
		CameraPlayBack * pkCamPlayBack = NiNew CameraPlayBack;
		pkCamPlayBack->spCamera = (NiCamera*)pkNode;
		RetriveCheckPoint(pkCamPlayBack);
		m_arrCamera.Add(pkCamPlayBack);
	}
	else if (NiIsKindOf(NiNode, pkNode))
	{
		// NiNodes are the primary structural objects in Gamebryo. They 
		// group other Gamebryo scene objects together under a common parent
		// and coordinate system. NiNodes can have as many children as 
		// necessary, but those children are not guaranteed to be contiguous.

		NiNode * pNode = (NiNode*)pkNode;

		for (unsigned int ui = 0; ui < pNode->GetArrayCount(); ui++)
		{
			FindAndAddCamera(pNode->GetAt(ui));
		}
	}
}

void CKartPlayBack::RetriveCheckPoint(CameraPlayBack *& pCamPlayBack)
{
	//pCamPlayBack->arrCheckPointName.Add()

	int count = pCamPlayBack->spCamera->GetExtraDataSize();
	if (count == 0) 
	{
		assert(0);
		return;
	}
	NiStringExtraData * pData =NiDynamicCast(NiStringExtraData, pCamPlayBack->spCamera->GetExtraDataAt(0));
	NiFixedString kValue = pData->GetValue();

	const char * strText = (const char *) kValue;

	char strTemp[128] = "";

	const char * pCur = strText;
	char * pNext = NULL;
	int pos = 0;
	int step = 0;
	do {
		pNext = strstr(&pCur[pos], ",");
		strncpy(strTemp, &pCur[pos], 128);
		if (pNext)
		{
			step = pNext - (char*)&pCur[pos] + 1;		
			pos += step;

			strTemp[step - 1] = 0;
			pCamPlayBack->arrCheckPointName.push_back((strTemp));
		}		
		else
		{
			int l = strlen(strTemp);
			strTemp[l-1] = 0;
			pCamPlayBack->arrCheckPointName.push_back((strTemp));
		}
		
	} while(pNext);
/*
	int Mark = sscanf(strText, "%s", strTemp);	
	int i = 0;
	while(Mark != EOF)
	{		
		pCamPlayBack->arrCheckPointName.Add(strTemp);
		i += strlen(strTemp) + 1;
		Mark = sscanf(strText + i, "%s", strTemp);			
	}*/
}

NiCameraPtr CKartPlayBack::GetCamera(const NiFixedString & kCheckPointName, int & nIndex)
{
	unsigned int uiSize = m_arrCamera.GetEffectiveSize();

	for (unsigned int i = 0; i < uiSize; ++i)
	{
		CameraPlayBack * pCamPlayBack = m_arrCamera.GetAt(i);
		assert(pCamPlayBack);
		unsigned int uiCheckNum = pCamPlayBack->arrCheckPointName.size();

		for (unsigned int j = 0; j < uiCheckNum; ++j)
		{
			if (!pCamPlayBack->arrCheckPointName[j].compare(kCheckPointName)) 
			{
				nIndex = j;
				return pCamPlayBack->spCamera;
			}
		}
	}

	return NULL;
}