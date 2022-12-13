////////////////////////////////////////////////
//     FileName:    ScriptEvent.cpp
//     Author:      Fish
//     Date:        2006-10-12 15:04:28
//     Description: [提供让事件回调到脚本的功能]
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "GameUI.h"
#include "ScriptEvent.h"
#include "scriptutils.h"
#include "common.h"
#include "Network/msg.h"
#include "MsgToLua.h"
#include "GlobalInterface.h"

HRESULT CScriptEvent::AppInit(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		hr = AtlAdvise(spNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;
		
		CComPtr<IGameService> spService;
		hr = m_pApp->GetService(IID_IQQTppp, &spService);
		KK_CHECK(hr);

		hr = AtlAdvise(spService, (IQQTpppSink*)this, IID_IQQTpppSink, &m_dwP2PCookie);
		KK_CHECK(hr);
	}
KK_EXIT:
	return hr;
}

HRESULT CScriptEvent::AppUnInit()
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

	m_pApp = NULL;
	m_pNetwork = NULL;
	GlobalInterface::m_pScriptState = NULL;
KK_EXIT:
	return S_OK;
}


STDMETHODIMP CScriptEvent::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	TriggleScriptEventEx(GlobalInterface::m_pScriptState, "OnResponseLogin", "%d,%d", Entity, Result);
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity == eGameServer )
	{
		switch(MsgID)
		{
		case ID_CMS_REQUESTLOGIN:
			{
				CResponseLoginPara* pPara = (CResponseLoginPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseLogin2");
			}
			break;
		case ID_CMS_REQUESTREGISTER:
			{
				CResponseRegisterPara* pPara = (CResponseRegisterPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseRegist");
			}
			break;
		case ID_CMS_REQUESTGETROOMLIST:
			{
				CResponseGetRoomListPara* pPara = (CResponseGetRoomListPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseGetRoomList");
			}
			break;
		case ID_CMS_REQUESTCREATEROOM:
			{
				CResponseCreateRoomPara* pPara = (CResponseCreateRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseCreateRoom");
			}
			break;
		case ID_CMS_REQUESTENTERROOM:
			{
				CResponseEnterRoomPara* pPara = (CResponseEnterRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseEnterRoom");
			}
			break;
		case ID_CMS_REQUESTLEAVEROOM:
			{
				CResponseLeaveRoomPara* pPara = (CResponseLeaveRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseLeaveRoom");
			}
			break;
		case ID_CMS_REQUESTQUICKENTERROOM:
			{
				CResponseQuickEnterRoomPara* pPara = (CResponseQuickEnterRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseQuickEnterRoom");
			}
			break;
		case ID_CMS_REQUESTCHANGESEATSTATE:
			{
				CResponseChangeSeatStatePara* pPara = (CResponseChangeSeatStatePara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseChangeSeatState");
			}
			break;
		case ID_CMS_REQUESTKICKFROMROOM:
			{
				CResponseKickFromRoomPara* pPara = (CResponseKickFromRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseKickFromRoom");
			}
			break;
		case ID_CMS_REQUESTCHANGEREADYSTATE:
			{
				CResponseChangeReadyStatePara* pPara = (CResponseChangeReadyStatePara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseChangeReadyState");
			}
			break;
		case ID_CMS_REQUESTSTARTGAME:
			{
				CResponseStartGamePara* pPara = (CResponseStartGamePara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseStartGame");
			}
			break;
		case ID_CMS_REQUESTTALK:
			{
				CResponseTalkPara* pPara = (CResponseTalkPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseTalk");
			}
			break;

		//notify
		case ID_SMC_NOTIFYENTERROOM:
			{
				CNotifyEnterRoomPara* pPara = (CNotifyEnterRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyEnterRoom");
			}
			break;
		case ID_SMC_NOTIFYLEAVEROOM:
			{
				CNotifyLeaveRoomPara* pPara = (CNotifyLeaveRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyLeaveRoom");
			}
			break;
		case ID_SMC_NOTIFYCHANGESEATSTATE:
			{
				CNotifyChangeSeatStatePara* pPara = (CNotifyChangeSeatStatePara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyChangeSeatState");
			}
			break;
		case ID_SMC_NOTIFYKICKFROMROOM:
			{
				CNotifyKickFromRoomPara* pPara = (CNotifyKickFromRoomPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyKickFromRoom");
			}
			break;
		case ID_SMC_NOTIFYCHANGEREADYSTATE:
			{
				CNotifyChangeReadyStatePara* pPara = (CNotifyChangeReadyStatePara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyChangeReadyState");
			}
			break;
		case ID_SMC_NOTIFYTALK:
			{
				CNotifyTalkPara* pPara = (CNotifyTalkPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyTalk");
			}
			break;
		case ID_SMC_NOTIFYKICKFROMSERVER:
			{
				CNotifyKickFromServerPara* pPara = (CNotifyKickFromServerPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyKickFromServer");
			}
			break;
		case ID_SMC_NOTIFYGAMEBEGIN:
			{
				CNotifyGameBeginPara * pPara = (CNotifyGameBeginPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState,"OnNotifyGameBegin");
			}
			break;
		case ID_SMC_NOTIFYSINGLEGAMEBEGIN:
			{
				CNotifySingleGameBeginPara* pPara = (CNotifySingleGameBeginPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifySingleGameBegin");
			}
			break;
		case ID_SMC_NOTIFYGAMEOVER:
			{
				CNotifyGameOverPara * pPara = (CNotifyGameOverPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyGameOver");
			}
			break;
		case ID_SMC_NOTIFYRACEOVER:
			{
				CNotifyRaceOverPara* pPara = (CNotifyRaceOverPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyRaceOver");
			}
			break;
		case ID_SMC_NOTIFYCHANGEMAP:
			{
				CNotifyChangeMapPara* pPara = (CNotifyChangeMapPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyChangeMap");
			}
			break;
		case ID_CMS_REQUESTCHANGEMAP:
			{
				CResponseChangeMapPara* pPara = (CResponseChangeMapPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseChangeMap");
			}
			break;
		case ID_SMC_NOTIFYCHANGETEAM:
			{
				CNotifyPlayerChangeTeamPara* pPara = (CNotifyPlayerChangeTeamPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyPlayerChangeTeam");
			}
			break;
		case ID_CMS_REQUESTCHANGETEAM:
			{
				CResponseChangeTeamPara* pPara = (CResponseChangeTeamPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseChangeTeam");
			}
			break;
		case ID_SMC_NOTIFYCHANGEROOMOWNER:
			{
				CNotifyChangeRoomOwnerPara* pPara = (CNotifyChangeRoomOwnerPara*)Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyChangeRoomOwner");
			}
			break;
		case ID_CMS_REQUESTCHANGEITEMSTATUS:
			{
				CResponseChangeItemStatus* pPara = (CResponseChangeItemStatus*) Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseChangeItemStatus");
			}
			break;
		case ID_CMS_REQUESTFINDPLAYERBYQQ:
			{
				CResponseFindPlayerByQQPara* pPara = (CResponseFindPlayerByQQPara*) Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseFindPlayer");
			}
			break;
		case ID_CMS_REQUESTCHECKNAME:
			{
				CResponseCheckNamePara* pPara = (CResponseCheckNamePara*) Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseCheckName");
			}
			break;
		case ID_CMS_REQUESTASSIGNATTRIBUTE:
			{
				CResponseAssignAttributePara* pPara = (CResponseAssignAttributePara*) Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseAssignAttribute");
			}
			break;
		case ID_CMS_REQUESTCLEARATTRIBUTE:
			{
				CResponseClearAttributePara* pPara = (CResponseClearAttributePara*) Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseClearAttribute");
			}
			break;
		}
	}
	else if( Entity == eShopServer )
	{
		switch(MsgID)
		{
		case ID_CMS_REQUESTBUYCOMMODITY:
			{
				CResponseBuyCommodityPara* pPara = (CResponseBuyCommodityPara*) Data;
				MsgToLua(GlobalInterface::m_pScriptState->GetSysState(),pPara);
				TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnResponseBuyCommodity");
			}
			break;
		}
	}
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnClose(ServerEntity Entity, int nErrorCode)
{
	TriggleScriptEventEx(GlobalInterface::m_pScriptState, "OnSocketClose", "%d,%d", Entity, nErrorCode);
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnConnect(ServerEntity Entity, int nErrorCode)
{
	TriggleScriptEventEx(GlobalInterface::m_pScriptState, "OnSocketConnect", "%d,%d", Entity, nErrorCode);
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnCreate(ServerEntity Entity, int nErrorCode)
{
	TriggleScriptEventEx(GlobalInterface::m_pScriptState, "OnSocketCreate", "%d,%d", Entity, nErrorCode);
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnStartLogin(ServerEntity Entity, int nErrorCode)
{
	TriggleScriptEventEx(GlobalInterface::m_pScriptState, "OnStartLogin", "%d,%d", Entity, nErrorCode);
	return S_OK;
}


STDMETHODIMP CScriptEvent::OnUdpLoginOk()
{
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	return S_OK;
}

STDMETHODIMP CScriptEvent::OnRecvP2PGameDataFromClient(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
	switch(*(INT*)pBody)
	{
	case ID_CMS_REQUESTTALK:
		{
			CRequestTalkPara aPara;
			aPara.Decode(pBody+sizeof(INT), nBodyLen);
			MsgToLua(GlobalInterface::m_pScriptState->GetSysState(), &aPara);
			TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnRequestTalk_P2P");
		}
		break;
	}
	return S_OK;
}

STDMETHODIMP CScriptEvent::SendTcpP2pData(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData)
{
	return S_OK;
}
