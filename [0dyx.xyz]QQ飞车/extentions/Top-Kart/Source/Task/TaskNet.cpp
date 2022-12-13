#include "Top-KartPCH.h"
#include "TaskNet.h"
#include "../network/msgheader.h"
#include "../Platform/GlobalInterface.h"
#include "network/msg.h"
#include "common.h"
#include "zlib.h"
#include "../scene.h"
#include "../NxSceneMgr.h"
#include "TaskMgr.h"
#include "../Track/CountDownTask.h"
#include "../Platform/GlobalInterface.h"
#include "framework/gui_i.h"
#include "AwardMgr.h"
CTaskNet::CTaskNet(void):
m_pApp(NULL),
m_pNetwork(NULL),
m_dwCookie(0),
m_dwP2PCookie(0),
m_bFirst(true)
{
}

CTaskNet::~CTaskNet(void)
{
	AppUnInit();
}


HRESULT CTaskNet::AppInit(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;

		hr = AtlAdvise(spNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);

		CComPtr<IQQTppp> spQQTppp;
		hr = m_pApp->GetService(IID_IQQTppp, (void**)&spQQTppp);
		KK_CHECK(hr);
		m_pQQTppp = spQQTppp;

		CComPtr<IGameService> spService;
		hr = m_pApp->GetService(IID_IQQTppp, &spService);
		KK_CHECK(hr);

		hr = AtlAdvise(spService, (IQQTpppSink*)this, IID_IQQTpppSink, &m_dwP2PCookie);
		KK_CHECK(hr);


	}
KK_EXIT:
	return hr;
}

HRESULT CTaskNet::AppUnInit()
{
	HRESULT hr = S_OK;
	if ( m_dwCookie )
	{
		AtlUnadvise(m_pNetwork, IID_INetworkSink, m_dwCookie);
		m_dwCookie = 0;
	}
	if ( m_dwP2PCookie )
	{
		CComPtr<IGameService> spService;
		hr = m_pApp->GetService(IID_IQQTppp, &spService);
		KK_CHECK(hr);

		hr = AtlUnadvise(spService, IID_IQQTpppSink, m_dwP2PCookie);
		m_dwP2PCookie = 0;
	}
	m_pNetwork = NULL;
	m_pQQTppp = NULL;
	m_pApp = NULL;
	return S_OK;

KK_EXIT:
	return hr;
}


STDMETHODIMP CTaskNet::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	return S_OK;
}


STDMETHODIMP CTaskNet::OnServerMessage(ServerEntity Entity,
											   UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity != eGameServer )
		return S_OK;

	DISPATCH_MESSAGE_QQKART( MsgID, Data)

		return S_OK;
}

STDMETHODIMP CTaskNet::OnClose(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}


STDMETHODIMP CTaskNet::OnConnect(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CTaskNet::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CTaskNet::OnStartLogin(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CTaskNet::OnUdpLoginOk()
{
	return S_OK;
}

STDMETHODIMP CTaskNet::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CTaskNet::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	return S_OK;
}

STDMETHODIMP CTaskNet::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	return S_OK;
}

STDMETHODIMP CTaskNet::OnRecvP2PGameDataFromClient
(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CTaskNet::SendTcpP2pData(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData)
{
	return S_OK;
}


//根据消息映射到相应函数

BEGIN_MESSAGE_MAP_QQKART(CTaskNet)
ON_COMMAND_QQKART(ID_CMS_REQUESTGETTASKLIST,		CTaskNet::OnResponseGetTaskList)
ON_COMMAND_QQKART(ID_CMS_REQUESTTAKETASK,			CTaskNet::OnResponseTakeTask)
ON_COMMAND_QQKART(ID_CMS_REQUESTDELETETASK,			CTaskNet::OnResponseDeleteTask)
ON_COMMAND_QQKART(ID_CMS_REQUESTSTARTSINGLETASK,	CTaskNet::OnResponseStartSingleTask)
ON_COMMAND_QQKART(ID_SMC_NOTIFYSINGLEGAMEBEGIN,		CTaskNet::OnNotifySingleGameBegin)
ON_COMMAND_QQKART(ID_SMC_NOTIFYSINGLERACEBEGIN,		CTaskNet::OnNotifySingleRaceBegin)
ON_COMMAND_QQKART(ID_SMC_NOTIFYSINGLERACEFINISH,	CTaskNet::OnNotifySingleRaceFinish)
ON_COMMAND_QQKART(ID_SMC_NOTIFYSINGLERACEOVER,		CTaskNet::OnNotifySingleRaceOver)
ON_COMMAND_QQKART(ID_SMC_NOTIFYSINGLEGAMEOVER,		CTaskNet::OnNotifySingleGameOver)
ON_COMMAND_QQKART(ID_CMS_REQUESTSKIPTASK,			CTaskNet::OnResponseSkipTask)
ON_COMMAND_QQKART(ID_SMC_NOTIFYTASKAWARD,			CTaskNet::OnNotifyTaskAward)
END_MESSAGE_MAP_QQKART


void CTaskNet::RequestGetTaskList(DWORD dwUin)
{
	CRequestGetTaskListPara aData;
	aData.m_uiUin = dwUin;
	aData.m_uiTime = GetTickCount();
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTGETTASKLIST, (PBYTE) &aData, 0, FE_GAMELOGIC, NxSceneMgr::Get()->GetGameSvrID());

}

void CTaskNet::RequestTakeTask(DWORD dwUin, DWORD dwTaskID, DWORD dwOption)
{
	CRequestTakeTaskPara aData;
	aData.m_uiUin = dwUin;
	aData.m_iTaskID = dwTaskID;
	aData.m_uiTime = GetTickCount();
	aData.m_shTakeOptionID = dwOption;
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTTAKETASK, (PBYTE) &aData, 0, FE_GAMELOGIC, NxSceneMgr::Get()->GetGameSvrID());

}

void CTaskNet::RequestDeleteTask(DWORD dwUin, DWORD dwTaskID)
{
	CRequestDeleteTaskPara aData;
	aData.m_uiUin = dwUin;
	aData.m_iTaskID = dwTaskID;
	aData.m_uiTime = GetTickCount();
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTDELETETASK, (PBYTE) &aData, 0, FE_PLAYER, NxSceneMgr::Get()->GetGameSvrID());
}

void CTaskNet::RequestStartSingleTask(DWORD dwUin, DWORD dwTaskID, DWORD dwMapId)
{
	CRequestStartSingleTaskPara aData;
	aData.m_uiUin = dwUin;
	aData.m_iTaskID = dwTaskID;
	aData.m_iMapID = dwMapId;	
	aData.m_uiTime = GetTickCount();
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTSTARTSINGLETASK, (PBYTE) &aData, 0, FE_PLAYER, NxSceneMgr::Get()->GetGameSvrID());
}

void CTaskNet::RequestPrepareReady(DWORD dwUin)
{
	CRequestPrepareReadyPara aData;
	aData.m_unUin = dwUin;
	aData.m_unTime = GetTickCount();
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTPREPAREREADY, (PBYTE) &aData, 0, FE_GAMELOGIC, CLevelManager::Get()->GetGameBeginPara()->m_unGameID);
}

void CTaskNet::RequestReportCurrentOpt(DWORD dwUin, DWORD dwOpt)
{
	CRequestReportCurrentOptPara aData;
	aData.m_uiUin = dwUin;
	aData.m_iOptID = dwOpt;
	aData.m_uiTime = GetTickCount();
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTREPORTCURRENTOPT, (PBYTE) &aData, 0, FE_GAMELOGIC, CLevelManager::Get()->GetGameBeginPara()->m_unGameID);
}

void CTaskNet::RequestReportCheckPoint()
{
	//CLevelManager::Get()->GetLinks()->SimWinGoPath();
}

void CTaskNet::RequestSkipTask(DWORD dwUin, DWORD dwID)
{
	CRequestSkipTaskPara aData;
	aData.m_uiUin = dwUin;
	aData.m_iTaskID = dwID;
	aData.m_uiTime = GetTickCount();
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTSKIPTASK, (PBYTE) &aData, 0, FE_GAMELOGIC, CLevelManager::Get()->GetGameBeginPara()->m_unGameID);
}

//---------------------服务器下发-----------------------------
void CTaskNet::OnResponseGetTaskList(void* pData)
{
	CResponseGetTaskListPara* pGetData = (CResponseGetTaskListPara*)pData;
}

void CTaskNet::OnResponseTakeTask(void* pData)
{
	CResponseTakeTaskPara* pGetData = (CResponseTakeTaskPara*)pData;
}

void CTaskNet::OnResponseDeleteTask(void* pData)
{
	CResponseDeleteTaskPara* pGetData = (CResponseDeleteTaskPara*)pData;
}

void CTaskNet::OnResponseStartSingleTask(void* pData)
{
	CResponseStartSingleTaskPara* pGetData = (CResponseStartSingleTaskPara*)pData;
	
}

void CTaskNet::OnNotifySingleGameBegin(void* pData)
{
	CNotifySingleGameBeginPara* pGetData = (CNotifySingleGameBeginPara*)pData;
	
	
	if(!CTaskMgr::Get()->EnterTask(pGetData))
	{
		XTrace("游戏初始化失败");
		return;
	}

	NiTransform Trans = CLevelManager::Get()->GetLinks()->GetStartPointTransform(4);
	Trans.m_Translate.z += 0.56f;
	Play* pPlay = NxPlayMgr::GetHostPlay();
	if(pPlay)
	{
		//pPlay->SitPos(Trans);
		pPlay->EnterPhyScene(Trans);
		if (pPlay->GetKart() && pPlay->GetKart()->GetVehicle()) 
		{
			//pPlay->GetKart()->SetMovable(false);
			pPlay->GetKart()->SetMovable(true);
			pPlay->GetKart()->SetKartRotate(pPlay->GetKart()->GetKartRotate()*Trans.m_Rotate);
		}
	}
	//上报服务器准备完成
	CTaskNet::RequestPrepareReady(NxPlayMgr::GetHostPlay()->GetPlayUin());

	NxSceneMgr::Get()->SetGameSvrID(pGetData->m_uiGameID);
	NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_STAR_RACE);
	GlobalInterface::m_pQQTppp->SetGameStatus(PPPSetGameStatus_InGame,1);
	
}

void CTaskNet::OnNotifySingleRaceBegin(void* pData)
{
	CNotifySingleRaceBeginPara* pGetData = (CNotifySingleRaceBeginPara*)pData;
	CTaskMgr::Get()->GetCurTask()->Start();

	NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_EAGLEMAP)->SetTarget(NxPlayMgr::GetHostPlay());
	NxSceneMgr::Get()->GetCameraController()->ActivateLogicCam(CCameraBase::ECT_EAGLEMAP);
	NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_RACE)->SetTarget(NxPlayMgr::GetHostPlay());
	NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_RACE);
	NxSceneMgr::Get()->GetCameraController()->GetForeCam()->SetRotate(NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate());

	CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SMOKE, 0));
	NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_RACING);

}

void CTaskNet::OnNotifySingleRaceFinish(void* pData)
{
	CNotifySingleRaceFinishPara* pGetData = (CNotifySingleRaceFinishPara*)pData;
	if(pGetData->m_shResultID == 0)
	{
		CTaskMgr::Get()->GetCurTask()->Finish();
	}
	else
	{
		CTaskMgr::Get()->GetCurTask()->Fail();
	}
}

void CTaskNet::OnNotifySingleRaceOver(void* pData)
{
	CNotifySingleRaceOverPara* pGetData = (CNotifySingleRaceOverPara*)pData;
	//显示结果面板
	CResponseLoginPara* pResponseLoginPara = m_pNetwork->GetGameInfo();
	pResponseLoginPara->m_stPlayerDBBaseInfo.m_unExperience = pGetData->m_stTaskAward.m_iTotalExp;
	pResponseLoginPara->m_stPlayerDBBaseInfo.m_iMoney = pGetData->m_stTaskAward.m_iTotalMoney;
}

void CTaskNet::OnNotifySingleGameOver(void* pData)
{
	CNotifySingleGameOverPara* pGetData = (CNotifySingleGameOverPara*)pData;
	GetMainApp()->ShowPointer();
	GlobalInterface::m_pGUI->EnterUILayout("taskscore");
	//显示完成对话框
}

void CTaskNet::OnResponseSkipTask(void* pData)
{
	CResponseSkipTaskPara* pGetData = (CResponseSkipTaskPara*)pData;
}

void CTaskNet::OnNotifyTaskAward(void* pData)
{
	CNotifyTaskAwardPara* pGetData = (CNotifyTaskAwardPara*)pData;
	if(pGetData->m_uiUin == NxPlayMgr::GetHostPlay()->GetPlayUin())
	{
		CALL(CAwardMgr::Get(), AddAward(pGetData->m_stTaskAward));
	}	
}
