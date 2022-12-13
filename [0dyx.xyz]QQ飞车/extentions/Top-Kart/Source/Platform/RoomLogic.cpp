#include "Top-KartPCH.h"
#include ".\roomlogic.h"
#include "common.h"
#include "network/msg.h"
#include "QQTppp_i.c"
#include "../GameStatusManager.h"
#include "../SoundManager.h"
#include "../NxSceneMgr.h"

CRoomLogic::CRoomLogic(void):
m_pApp(NULL),
m_pNetwork(NULL),
m_pQQTppp(NULL),
m_dwCookie(0),
m_dwP2PCookie(0)
{
}

CRoomLogic::~CRoomLogic(void)
{
}

STDMETHODIMP CRoomLogic::Load(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;

		CComPtr<IQQTppp> spQQTppp;
		hr = m_pApp->GetService(IID_IQQTppp, (void**)&spQQTppp);
		KK_CHECK(hr);
		m_pQQTppp = spQQTppp;

		Active();
		hr = S_OK;
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CRoomLogic::Active()
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

STDMETHODIMP CRoomLogic::DeActive()
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

STDMETHODIMP CRoomLogic::UnLoad()
{
	DeActive();
	m_pApp = NULL;
	m_pNetwork = NULL;
	m_pQQTppp = NULL;
	return S_OK;
}

STDMETHODIMP CRoomLogic::GetName(LPCSTR* Name)
{
	static LPCSTR s_myName = "room";
	KK_CHECK_POINTER(Name);
	*Name = s_myName;
KK_EXIT:
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity == eGameServer )
	{
		switch(MsgID)
		{
		case ID_CMS_REQUESTCREATEROOM:
			{
				CResponseCreateRoomPara* pPara = (CResponseCreateRoomPara*)Data;
				if ( pPara->m_nResultID != S_OK )
				{
					NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_ROOM);
					break;
				}
			}
		case ID_CMS_REQUESTENTERROOM:
			{
				CResponseEnterRoomPara* pPara = (CResponseEnterRoomPara*)Data;
				if ( pPara->m_nResultID != S_OK )
				{
					NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_ROOM);
					break;
				}
			}
		case ID_CMS_REQUESTQUICKENTERROOM:
			{
				CResponseQuickEnterRoomPara* pPara = (CResponseQuickEnterRoomPara*)Data;
				if ( pPara->m_nResultID != S_OK )
				{
					break;
				}
			}
			{
				QQUSERDATA* pPara = (QQUSERDATA*)m_pNetwork->GetQQUserData();
				GameResponseInfo* pGameInfo = m_pNetwork->GetGameInfo();
				LPSOCKET_CREATE_INFO pSocketInfo = (LPSOCKET_CREATE_INFO)m_pNetwork->GetSocketInfo(eGameServer);
				ULONG nIP = inet_addr(pSocketInfo->szHostAddr);
				USHORT nPort = pSocketInfo->uUDPPorts[0];
				HRESULT hr = m_pQQTppp->Initialize();
				ASSERT(hr == S_OK);

				hr = m_pQQTppp->LoginLocalPlayer(pGameInfo->m_nPlayerID, pPara->Uin, nIP, nPort, -1, 0);
			}
			{
				if ( MsgID == ID_CMS_REQUESTENTERROOM )
				{	
					CResponseEnterRoomPara* pPara = (CResponseEnterRoomPara*)Data;
					for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
					{
						STPlayerRoomVisibleInfo& pi = pPara->m_stPlayerInfo[i];
						if ( pi.m_unUin != 0 )
						{
							m_pQQTppp->AddHost(pi.m_nPlayerID, pi.m_unUin, -1, 0);
						}
					}
				}
				if ( MsgID == ID_CMS_REQUESTQUICKENTERROOM )
				{	
					CResponseQuickEnterRoomPara* pPara = (CResponseQuickEnterRoomPara*)Data;
					for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
					{
						STPlayerRoomVisibleInfo& pi = pPara->m_stPlayerInfo[i];
						if ( pi.m_unUin != 0 )
						{
							m_pQQTppp->AddHost(pi.m_nPlayerID, pi.m_unUin, -1, 0);
						}
					}
				}
			}
			NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_ROOM);
			break;
		case ID_SMC_NOTIFYENTERROOM:
			{
				CNotifyEnterRoomPara* pPara = (CNotifyEnterRoomPara*)Data;
				STPlayerRoomVisibleInfo& pi = pPara->m_stNewPlayerInfo;
				m_pQQTppp->AddHost(pi.m_nPlayerID, pi.m_unUin, -1, 0);
			}
			break;
		case ID_CMS_REQUESTKICKFROMROOM:
			{
				CResponseKickFromRoomPara* pPara = (CResponseKickFromRoomPara*)Data;
				if ( pPara->m_nResultID == S_OK )
				{
					m_pQQTppp->DelHost(0, pPara->m_unDstUin);
				}
			}
			break;
		case ID_SMC_NOTIFYLEAVEROOM:
			{
				CNotifyLeaveRoomPara* pPara = (CNotifyLeaveRoomPara*)Data;
				QQUSERDATA* pQQData = (QQUSERDATA*)m_pNetwork->GetQQUserData();
				if ( pPara->m_unUin == pQQData->Uin )
				{
					//NxSceneMgr::Get()->GetSoundManager()->StopAllSound();
					CSoundMgr::Get()->StopAllSound();

					NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_ROOM);
					m_pQQTppp->RemoveAllRemotePlayers();
					m_pQQTppp->Finalize();
				}
				else
					m_pQQTppp->DelHost(0, pPara->m_unUin);

			}
			break;
		case ID_SMC_NOTIFYKICKFROMROOM:
			{
				CNotifyKickFromRoomPara* pPara = (CNotifyKickFromRoomPara*)Data;
				QQUSERDATA* pQQData = (QQUSERDATA*)m_pNetwork->GetQQUserData();
				if ( pPara->m_unDstUin == pQQData->Uin )
				{
					m_pQQTppp->RemoveAllRemotePlayers();
					m_pQQTppp->Finalize();
				}
				else
					m_pQQTppp->DelHost(0, pPara->m_unDstUin);
			}
			break;
		case ID_CMS_REQUESTLEAVEROOM:
			{
				CResponseLeaveRoomPara* pPara = (CResponseLeaveRoomPara*)Data;
				if ( pPara->m_nResultID == S_OK )
				{
					//NxSceneMgr::Get()->GetSoundManager()->StopAllSound();
					CSoundMgr::Get()->StopAllSound();
					NxSceneMgr::Get()->GetGameStatusManager()->SetGameState(GameStatusManager::GST_ROOM);
					m_pQQTppp->RemoveAllRemotePlayers();
					m_pQQTppp->Finalize();
				}
			}
			break;
		case ID_CMS_REQUESTLOGOUT:
			{
				CResponseLogoutPara* pPara = (CResponseLogoutPara*)Data;
				if ( pPara->m_nResultID == S_OK )
				{
					m_pQQTppp->RemoveAllRemotePlayers();
					m_pQQTppp->Finalize();
				}
			}
			break;
		case ID_SMC_NOTIFYKICKFROMSERVER:
			{
				m_pQQTppp->RemoveAllRemotePlayers();
				m_pQQTppp->Finalize();
			}
			break;
		}
	}
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnClose(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnConnect(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnStartLogin(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnUdpLoginOk()
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::OnRecvP2PGameDataFromClient(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CRoomLogic::SendTcpP2pData(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData)
{
	return S_OK;
}

