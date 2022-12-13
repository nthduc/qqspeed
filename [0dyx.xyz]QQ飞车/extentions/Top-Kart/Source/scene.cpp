#include "Top-KartPCH.h"
#include "scene.h"
#include "nifop.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	Unload();
}

bool CScene::Load(LPCSTR pstrNIFFile)
{
	assert(pstrNIFFile);
	m_strNIFFile = pstrNIFFile;
	NiStream kStream;
	if ( !kStream.Load(pstrNIFFile) )
	{
		return false;
	}
	m_spScene = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
	assert(m_spScene != NULL);
	m_spScene->UpdateProperties();
	m_spScene->UpdateEffects();
	m_spScene->Update(0);
	NiTimeController::StartAnimations(m_spScene);
	return true;
}

bool CScene::Reload()
{
	if ( m_strNIFFile.size() == 0 )
		return false;
	return Load(m_strNIFFile.c_str());
}

void CScene::Unload()
{
	m_spCamera = NULL;
	m_spScene = NULL;
}

bool CScene::SetCamera(LPCSTR pstrCameraFile, LPCSTR pstrCameraName)
{
	if ( pstrCameraFile )
		m_strCameraFile = pstrCameraFile;
	if ( pstrCameraName )
		m_strCameraName = pstrCameraName;
	//如果pstrCameraFile为空,从当前模型寻找相机,否则从pstrCameraFile指定的文件寻找相机
	if ( pstrCameraFile )
	{
		NiStream kStream;
		NiNode* pkNode = NULL;
		if ( kStream.Load(pstrCameraFile) )
		{
			pkNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));
		}
		pkNode->UpdateProperties();
		pkNode->UpdateEffects();
		pkNode->Update(0);
		NiTimeController::StartAnimations(pkNode);
		m_spCamera = CNifOp::FindCameraByName(pkNode, pstrCameraName);
	}
	else
	{
		m_spCamera = CNifOp::FindCameraByName(m_spScene, pstrCameraName);
	}
	assert(m_spCamera != NULL);
	return m_spCamera != NULL;
}

void CScene::SetCamera(NiCamera* pkCamera)
{
	m_spCamera = pkCamera;
}

NiNode*	CScene::GetScene()
{
	return m_spScene;
}

NiCamera* CScene::GetCamera()
{
	return m_spCamera;
}

void CScene::Update(float fAccuTime)
{
	if ( m_spScene )
	{
		m_spScene->Update(fAccuTime);
	}
	if ( m_spCamera )
	{
		m_spCamera->Update(fAccuTime);
	}
}

void CScene::Render()
{
	//NiDrawScene(m_spCamera, m_spScene, GetMainApp()->GetDefaultCuller());
}

void CScene::CacheRender()
{
	//nothing to cacherender
}

void CScene::Draw()
{
	//nothing to draw
}

NiCamera * CScene::GetSceneCamera(LPCSTR pstrCameraName)
{
	return CNifOp::FindCameraByName(m_spScene, pstrCameraName);
}

bool CKartScene::AddKartToPoint(const char * pcName, Play * pPlay)
{
	if (!pPlay || !m_spScene)
		return false;

	NiNode * pkNode = pPlay->GetKartNIFRoot();

	NiNode * pkAttachPoint = NiDynamicCast(NiNode, m_spScene->GetObjectByName(pcName));

	if (pkAttachPoint)
	{
		pkNode->SetTranslate(0, 0, 0);
	/*	NiNode * pkParent = pkNode->GetParent();
		if (pkParent)
			pkParent->DetachChild(pkNode);*/
		pkAttachPoint->AttachChild(pkNode);
		return true;
	}
	return false;
}

bool CKartScene::AddRoleToPoint(const char * pcName, Play * pPlay)
{
	if (!pPlay || !m_spScene)
		return false;

	NiNode * pkNode = pPlay->GetCharacterNIFRoot();

	NiNode * pkAttachPoint = NiDynamicCast(NiNode, m_spScene->GetObjectByName(pcName));

	if (pkAttachPoint)
	{		
		/*NiNode * pkParent = pkNode->GetParent();
		if (pkParent)
			pkParent->DetachChild(pkNode);*/
		pkAttachPoint->AttachChild(pkNode);
		return true;
	}

	return false;
}

bool CKartScene::RemoveKartFromPoint(const char * pcName, Play * pPlay)
{
	if (!pPlay || !m_spScene)
		return false;

	NiNode * pkNode = pPlay->GetKartNIFRoot();

	NiNode * pkAttachPoint = NiDynamicCast(NiNode, m_spScene->GetObjectByName(pcName));

	if (pkAttachPoint)
	{
		pkAttachPoint->DetachChild(pkNode);
		/*NiNode * pkParent = pkNode->GetParent();
		if (pkParent)
			pkParent->DetachChild(pkNode);*/
		return true;
	}

	return false;
}

bool CKartScene::RemoveRoleFromPoint(const char * pcName, Play * pPlay)
{
	if (!pPlay || !m_spScene)
		return false;

	NiNode * pkNode = pPlay->GetCharacterNIFRoot();

	NiNode * pkAttachPoint = NiDynamicCast(NiNode, m_spScene->GetObjectByName(pcName));

	if (pkAttachPoint)
	{
		pkAttachPoint->DetachChild(pkNode);
	/*	NiNode * pkParent = pkNode->GetParent();
		if (pkParent)
			pkParent->DetachChild(pkNode);*/
		return true;
	}

	return false;
}