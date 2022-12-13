#include "Top-KartPCH.h"
#include "GameLogic.h"
#include "QQTppp_i.h"
#include "common.h"
#include "framework/gui_i.h"
#include "./Track/CountDownTask.h"

CGameLogic::CGameLogic(void):
m_pApp(0),
m_pNetwork(0),
m_pQQTppp(0),
m_pGUI(0),
m_dwCookie(0),
m_dwP2PCookie(0)
{
}

CGameLogic::~CGameLogic()
{

}

STDMETHODIMP CGameLogic::Load(IGameApp* App)
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

		CComPtr<IGUI> spGUI;
		hr = m_pApp->GetService(IID_IGUI, (void**)&spGUI);
		KK_CHECK(hr);
		m_pGUI = spGUI;

		CComPtr<IQQTppp> spQQTppp;
		hr = m_pApp->GetService(IID_IQQTppp, (void**)&spQQTppp);
		KK_CHECK(hr);
		m_pQQTppp = spQQTppp;

		CComPtr<IGameService> spService;
		hr = m_pApp->GetService(IID_IQQTppp, &spService);
		KK_CHECK(hr);

		hr = AtlAdvise(spService, (IQQTpppSink*)this, IID_IQQTpppSink, &m_dwP2PCookie);
		KK_CHECK(hr);

		hr = S_OK;
	}
KK_EXIT:
	return hr;
	
	
}


STDMETHODIMP CGameLogic::Active()
{
	HRESULT hr = E_FAIL;
	{
		hr = AtlAdvise(m_pNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);

		CComPtr<IGameService> spService;
		hr = m_pApp->GetService(IID_IQQTppp, &spService);
		KK_CHECK(hr);

		hr = AtlAdvise(spService, (IQQTpppSink*)this, IID_IQQTpppSink, &m_dwP2PCookie);
		KK_CHECK(hr);

	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CGameLogic::DeActive()
{
	HRESULT hr = S_OK;
	if ( m_dwCookie )
	{
		hr = AtlUnadvise(m_pNetwork, IID_INetworkSink, m_dwCookie);
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
KK_EXIT:
	return hr;
}

STDMETHODIMP CGameLogic::UnLoad()
{
	m_pApp = NULL;
	m_pNetwork = NULL;
	m_pQQTppp = NULL;
	m_pGUI = NULL;
	return S_OK;
}

STDMETHODIMP CGameLogic::GetName(LPCSTR* Name)
{
	static LPCSTR s_myName = "GameLogic";
	KK_CHECK_POINTER(Name);
	*Name = s_myName;
KK_EXIT:
	return S_OK;
}

STDMETHODIMP CGameLogic::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	return S_OK;
}



STDMETHODIMP CGameLogic::OnServerMessage(ServerEntity entity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if(entity == eGameServer)
	{
		switch(MsgID)
		{
		case ID_SMC_NOTIFYWAITUNREADYPLAYERS:
			{
				CNotifyWaitUnReadyPlayersPara * pPara = (CNotifyWaitUnReadyPlayersPara *)Data;
				m_uiLoadtime += pPara->m_unWaitLoadTime;
			}
			break;

		case ID_SMC_NOTIFYRACEBEGIN:
			{
				CNotifyRaceBeginPara * pPara = (CNotifyRaceBeginPara*)Data;
				CountDownTask * pkCountDownTask = NiNew CountDownTask;
				pkCountDownTask->SetParam(CountDownTask::CDM_BEGIN,(float)pPara->m_unCountDownTime);
				Scheduler::Get()->Schedule(pkCountDownTask);
/*
				int Len; MsgData;

				CComPtr<IQQTppp> spQQTppp;
				HRESULT hr = m_pApp->GetService(IID_IQQTppp, (void**)&spQQTppp);
				KK_CHECK(hr);
				spQQTppp->BroadCast();
*/
			}
			break;
		}
	}
	return S_OK;
}
STDMETHODIMP CGameLogic::Update(DWORD time)
{

	return S_OK;
}


void CGameLogic::StartGameLogic(CNotifyGameBeginPara* pData)
{	
	m_pGameBeginPara = pData;
	unsigned int start_time = GetTickCount();

	LoadScene();
	PlacedPlayers();	

	unsigned int last_time = GetTickCount() - start_time;

	if(last_time < m_uiLoadtime)
	{
		CRequestPrepareReadyPara data;
		data.m_unUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
		data.m_unTime = GetTickCount();
		HRESULT hr = m_pNetwork->RequestGameServer(ID_CMS_REQUESTPREPAREREADY,(PBYTE) &data, 0, FE_GAMESVRD, 0);
		if(hr != S_OK)
			return;
	}	
	NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GAME_START);

}

bool CGameLogic::LoadScene()
{

	if(! NxSceneMgr::Get()->EnterSceneLevel(m_pGameBeginPara->m_nMapID))
		return false;
	return true;

}

void CGameLogic::PlacedPlayers()
{
	NiTransform startTran;
	Play * pPlayer;
	NxMat34 mPose;

	NiMatrix3 kXRot90;
	kXRot90.MakeXRotation(-NI_PI/2.0f);

	NiMatrix3 kYRot90;
	kYRot90.MakeYRotation(NI_PI/2.0f);

	startTran.m_Rotate = startTran.m_Rotate * kXRot90 * kYRot90;

	NiPhysXTypes:: NiTransformToNxMat34(startTran.m_Rotate, startTran.m_Translate, mPose);
	for(int i = 0; i < m_pGameBeginPara->m_bCurrentPlayerNum1; i++)
	{
		pPlayer = NxPlayMgr::GetPlay(m_pGameBeginPara->m_anRaceTrackOrders[i]);
		startTran = NxSceneMgr::GetActiveSceneLevel()->GetLinks()->GetStartPointTransform(i);
		startTran.m_Rotate = startTran.m_Rotate * kXRot90 * kYRot90;
		NiPhysXTypes:: NiTransformToNxMat34(startTran.m_Rotate, startTran.m_Translate, mPose);
		pPlayer->GetKart()->SetKartPose(mPose);
	
	}

}



STDMETHODIMP CGameLogic::OnClose(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CGameLogic::OnConnect(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CGameLogic::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CGameLogic::OnStartLogin(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CGameLogic::OnUdpLoginOk()
{
	return S_OK;
}

STDMETHODIMP CGameLogic::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CGameLogic::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	return S_OK;
}

STDMETHODIMP CGameLogic::OnRecvP2PGameDataFromClient(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
	//处理其它玩家的消息
	/*
	Play* otherPlayer = NxPlayMgr::GetPlay((ULONG) nSocketPlayerUin)；
	*/
	return S_OK;
}

STDMETHODIMP CGameLogic::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	return S_OK;
}


STDMETHODIMP CGameLogic::SendTcpP2pData(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData)
{
	return S_OK;
}

