#include "Top-KartPCH.h"
#include ".\messagehouse.h"

#include "network/msg.h"
#include "common.h"
#include "zlib.h"
#include "MovePkgMgr.h"
#include "./Track/CountDownTask.h"
#include "./Platform/GlobalInterface.h"
#include "GameAward.h"
#include "GameOverCDTast.h"
#include "PlayerRank.h"
#include "gpcontrol.h"
#include "logtime.h"
#include "PlayerRank.h"

//#include "network/UdpMsg.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
//#pragma comment(lib, "zdll")

CMessageHouse::CMessageHouse(void):
m_pApp(NULL),
m_pNetwork(NULL),
m_dwCookie(0)
{
}

CMessageHouse::~CMessageHouse(void)
{
	AppUnInit();
}


HRESULT CMessageHouse::AppInit(IGameApp* App)
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

HRESULT CMessageHouse::AppUnInit()
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


STDMETHODIMP CMessageHouse::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	if ( Entity == eGameServer && LOGINSUCCESSED(Result) )
	{
		CKartScene::Create();
		CKartScene::Get()->Load(NiApplication::ConvertMediaFilename("scene/scene.nif"));
		CKartScene::Get()->SetCamera(NiApplication::ConvertMediaFilename("scene/cam1.nif"), NULL);
	}
	return S_OK;
}


BEGIN_MESSAGE_MAP_QQKART(CMessageHouse)
ON_COMMAND_QQKART( ID_SMC_NOTIFYGAMEBEGIN, CMessageHouse::OnNotifyGameBegin)
ON_COMMAND_QQKART( ID_SMC_NOTIFYRACEOVER,  CMessageHouse::OnNotifyRaceOver)
ON_COMMAND_QQKART( ID_SMC_NOTIFYRACEBEGIN, CMessageHouse::OnNotifyRaceBegin)
ON_COMMAND_QQKART( ID_SMC_NOTIFYWAITUNREADYPLAYERS,	CMessageHouse::OnNotifyWaitUnReadyPlayers)
ON_COMMAND_QQKART( ID_SMC_NOTIFYGAMEOVER,  CMessageHouse::OnNotifyGameOver)
ON_COMMAND_QQKART( ID_SMC_NOTIFYKICKFROMSERVER,  CMessageHouse::OnNotifyKickFromServer)
ON_COMMAND_QQKART( ID_SMC_NOTIFYRACESHOW,  CMessageHouse::OnNotifyRaceShow)
ON_COMMAND_QQKART( ID_SMC_NOTIFYCOUNTDOWN,  CMessageHouse::OnNotifyRaceCountDown)
ON_COMMAND_QQKART( ID_SMC_NOTIFYPLAYERFINISHRACE, CMessageHouse::OnNotifyPlayerFinishRace)
END_MESSAGE_MAP_QQKART



STDMETHODIMP CMessageHouse::OnServerMessage(ServerEntity Entity,
											UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity != eGameServer )
		return S_OK;

	DISPATCH_MESSAGE_QQKART( MsgID, Data)

		switch(MsgID)
	{
		case ID_CMS_NOTIFY_ANTI_BOT:
			//{
			//	NOTIFY_ANTI_BOT* pParam = (NOTIFY_ANTI_BOT*)Data;
			//	OnNotifyAntiBot(pParam);
			//}
			break;
		case ID_SMC_NOTIFYGAMEBEGIN:
			{
				break;
			}

	}
	return S_OK;
}

void CMessageHouse::OnNotifyWaitUnReadyPlayers( void * Data)
{
	CNotifyWaitUnReadyPlayersPara * aData = ( CNotifyWaitUnReadyPlayersPara * ) Data;

	return;
}
void CMessageHouse::OnNotifyRaceBegin( void * Data )
{
	if (Data == NULL)
		return;

	XTrace("-------------OnNotifyRaceBegin--------------");
	XTrace("receive OnNotifyRaceBegin : %d", GetTickCount());
	XTrace("Scheduler time: %f", Scheduler::Get()->GetClock().GetTime());


	CNotifyRaceBeginPara * pPara = (CNotifyRaceBeginPara*)Data;
	//CountDownTask * pkCountDownTask = NiNew CountDownTask(pPara->m_unCountDownTime / 1000);
	CCountDownTask::Create();
//	pkCountDownTask->SetParam(CountDownTask::CDM_BEGIN,(float)pPara->m_unCountDownTime / 1000 );
//	Scheduler::Get()->Schedule(pkCountDownTask);
	

	return;
}

void CMessageHouse::OnNotifyPlayerFinishRace(void * Data)
{
	CNotifyPlayerFinishRacePara* pPara = (CNotifyPlayerFinishRacePara*) Data;
	if(NxPlayMgr::GetPlay(pPara->m_unUin))
		NxPlayMgr::GetPlay(pPara->m_unUin)->SetInvalidTarget(true);
	if(CPlayerRankMgr::Get())
	{
		CPlayerRankMgr::Get()->SetPlayerFinish(pPara->m_unUin);
	}
}



void CMessageHouse::OnNotifyRaceCountDown(void * Data)
{
	CNotifyCountDownPara *pData = (CNotifyCountDownPara *)Data;
	//CGameOverCountDnMgr::StartCountDownTast();
	if(!NxPlayMgr::GetHostPlay()->IsFinish()){
		CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_OVERCOUNTDOWN,0));
	}
	if(CPlayerRankMgr::Get())
	{
		CPlayerRankMgr::Get()->SetPlayerFinish(pData->m_unUin);
	}

}



// 开始展示时间
void CMessageHouse::OnNotifyRaceShow( void * Data)
{
	CNotifyRaceShowPara * aData = ( CNotifyRaceShowPara*)Data;

	if ( !NxPlayMgr::GetHostPlay()->IsFinish())
	{
		CGameOverCountDnMgr::StopCountDownTast();
		NxPlayMgr::GetHostPlay()->SetFinish(false);
		NxPlayMgr::GetHostPlay()->SetWin(false);		
	}
	
	CALL(NxPlayMgr::GetHostPlay()->GetKart(), SetMovable(false));
}

void CMessageHouse::OnNotifyRaceOver( void * Data)
{
	CNotifyRaceOverPara * pData = ( CNotifyRaceOverPara *) Data;
	CGameAward::Create();
	CALL(CGameAward::Get(), Init(pData));

	NxGamePkgMgr::Destroy();
	//NxPlayMgr::StopAllPhysicPlay();
	if (CEagleEyeMap::Get())
		CEagleEyeMap::Get()->Show(false);		

	if ( CGPControl::Get() )
	{
		CGPControl::Get()->ToggleOp(FALSE);
		CGPControl::Get()->End();
	}
}


void CMessageHouse::OnNotifyGameOver( void * Data)
{
	CGameAward::Destroy();
	//if(NxSceneMgr::Get()->EnterSceneLevel(SCENELEVEL_CARSTORAGE))
	//{
	//	NxSceneMgr::Get()->GetGameStatusManager()->RestartLevel();
	//}
	CLevelManager::Destroy();
	CCountDownTask::Destroy();
	CALL(CKartScene::Get(), Reload());
	CALL(CKartScene::Get(), SetCamera(NiApplication::ConvertMediaFilename("scene/cam1.nif"), NULL));

	GlobalInterface::m_pScriptState->DoString("BindSelfKartToScene('ROOT');	BindSelfRoleToScene('playermode');");
	GlobalInterface::m_pQQTppp->SetGameStatus(PPPSetGameStatus_NotInGame,1);
}


void CMessageHouse::OnNotifyKickFromServer( void * Data)
{
	TerminateGame();
}

//static bRecived = false;

void CMessageHouse::OnNotifyGameBegin(  void * Data)
{
	LOG_TIME("CMessageHouse::OnNotifyGameBegin");
	if ( Data == NULL)
		return;

	XTrace("--------------loadscene-------------------");
	XTrace("start load : %d", GetTickCount());

	CNotifyGameBeginPara * aData = (CNotifyGameBeginPara*)Data;	

	unsigned int start_time = GetTickCount();
	SHORT  nLap = max( aData->m_nLap,1);

	CKartScene * pkKartScene = CKartScene::Get();
	Play * pSelf = NxPlayMgr::GetHostPlay();
	if (pkKartScene)
	{		
		pkKartScene->RemoveKartFromPoint("ROOT", pSelf);
		pkKartScene->RemoveRoleFromPoint("playermode", pSelf);
		pSelf->ReBindAll();
		pkKartScene->Unload();
	}
	CLevelManager::Create();
	CLevelManager::Get()->Enter(aData);
//	if (NxSceneMgr::Get()->EnterSceneLevel(aData->m_nMapID - 100,nLap))  // bug hovzhao
//	{
//		NxSceneMgr::Get()->GetGameStatusManager()->RestartLevel();
//	}

//	CPlayerRankMgr::Create((unsigned int *)aData->m_anRaceTrackOrders, aData->m_bCurrentPlayerNum1);

	CALL(CAcceleratePanel::Get(), SetActivity(true));
	CALL(CPlayerRankMgr::Get(),	Init((unsigned int *)aData->m_anRaceTrackOrders, aData->m_bCurrentPlayerNum1));
	CALL(CPlayerRankMgr::Get(), SetActivity(true));

	//////////////////////////////////////////////////////////////////////////////////////
	//todo：临时代码，为了渲染游戏中的UI,将playerrank放到队列最后
	CRenderMgr::Get()->TerminateObject2D(CPlayerRankMgr::Get());
	CRenderMgr::Get()->ActivateObject2D(CPlayerRankMgr::Get());
	/////////////////////////////////////////////////////////////////////////////////////

	//NxSceneMgr::Get()->GetActiveSceneLevel()->GetPhysXManager()->Fetch(0.0f);
	
	int nVaildPlayCount = 0;
	//Sleep(100);
	CGPControl::Get()->ToggleOp(FALSE);

	for(int i = 0; i < MAXSEATNUMINROOM; i++)
	{
		if ( aData->m_anRaceTrackOrders[i] > 0 )
		{
			Play * pPlay = NxPlayMgr::GetPlay( aData->m_anRaceTrackOrders[i] );
			
			if( !pPlay) 
			{
				LogText("MessageHouse::NotifyGameBegin() 玩家参数不对");
					continue;
			}
			else
			{
				//修改人：ray    描叙：开始前禁止使用道具
				pPlay->SetInvalidTarget(true);
				NiTransform Trans = CLevelManager::Get()->GetLinks()->GetStartPointTransform(i);
				Trans.m_Translate.z += 0.56f;
				//pPlay->SitPos(Trans);
				pPlay->EnterPhyScene(Trans);
				if (pPlay->GetKart() && pPlay->GetKart()->GetVehicle()) 
				{
					pPlay->GetKart()->SetMovable(false);
				}
				//NxSceneMgr::GetActiveSceneLevel()->GetScene()->Update(0.0);
				nVaildPlayCount++ ;
			}
		}
	}

	NxPlayMgr::StartAllPhysicPlay();
	//NxSceneMgr::Get()->GetActiveSceneLevel()->GetPhysXManager()->Sim(0.00f);
	
	if ( nVaildPlayCount != aData->m_bCurrentPlayerNum1) 
	{
		LogText("MessageHouse::NotifyGameBegin() 玩家参数不对");
		assert(0);
	}

	unsigned int last_time = GetTickCount() - start_time;


	CRequestPrepareReadyPara data;
	data.m_unUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
	data.m_unTime = GetTickCount();

	XTrace("-----------CRequestPrepareReadyPara------------");
	XTrace("load over: %d", GetTickCount());
	XTrace("Scheduler time: %f", Scheduler::Get()->GetClock().GetTime());


	HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTPREPAREREADY,(PBYTE) &data, 0, FE_GAMELOGIC, aData->m_unGameID);
	
	NxSceneMgr::Get()->SetGameSvrID(aData->m_unGameID);

	if(hr != S_OK)
	{
		LogText("MessageHouse::NotifyGameBegin() 发送 ID_CMS_REQUESTPREPAREREADY 失败");
		return;
	}

	NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_STAR_RACE);
	GlobalInterface::m_pQQTppp->SetGameStatus(PPPSetGameStatus_InGame,1);

	XTrace("--------------loadscene-------------------");
	XTrace("start load : %d", GetTickCount());
	return;
}


STDMETHODIMP CMessageHouse::OnClose(ServerEntity Entity, int nErrorCode)
{
	if ( Entity == eGameServer )
	{
		//TerminateGame();
	}
	return S_OK;
}

void CMessageHouse::TerminateGame()
{
	NxGamePkgMgr::Destroy();
	CGameAward::Destroy();

	CLevelManager::Get()->Destroy();
	GlobalInterface::m_pQQTppp->SetGameStatus(PPPSetGameStatus_NotInGame,1);
	if ( NxPlayMgr::GetHostPlay() )
	{
		CALL(NxPlayMgr::GetHostPlay()->GetKart(), SetMovable(false));
	}
}

STDMETHODIMP CMessageHouse::OnConnect(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CMessageHouse::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CMessageHouse::OnStartLogin(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CMessageHouse::OnUdpLoginOk()
{
	return S_OK;
}

STDMETHODIMP CMessageHouse::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	STUdpPack * pData = ( STUdpPack * ) pBody;

	if( NxGamePkgMgr::Get()) 
		NxGamePkgMgr::Get()->ProcessUdpPkg((STUdpPack*)pData);

	return S_OK;
}

STDMETHODIMP CMessageHouse::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	return S_OK;
}

STDMETHODIMP CMessageHouse::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	return S_OK;
}

STDMETHODIMP CMessageHouse::OnRecvP2PGameDataFromClient
(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
	STUdpPack * pData = ( STUdpPack * ) pBody;

	if( NxGamePkgMgr::Get()) 
		NxGamePkgMgr::Get()->ProcessUdpPkg((STUdpPack*)pData);

	return S_OK;
}

STDMETHODIMP CMessageHouse::SendTcpP2pData(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData)
{
	return S_OK;
}