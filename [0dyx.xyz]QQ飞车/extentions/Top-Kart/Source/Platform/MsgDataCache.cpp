#include "Top-KartPCH.h"
#include ".\msgdatacache.h"
#include "common.h"
#include "log.h"
#include "playerdata.h"
#include "stdlib.h"
#include "globalinterface.h"
#include "../Avatar/AvatarMgr.h"
#include "../Avatar/ShopMgr.h"
#include "../Task/TaskMgr.h"
#include "../Task/TaskNet.h"
#include "scriptutils.h"

CMsgDataCache::CMsgDataCache(void):
m_pApp(NULL),
m_pNetwork(NULL),
m_dwCookie(0)
{
	ZeroMemory(&m_kEnterRoomTask, sizeof(m_kEnterRoomTask));
}

CMsgDataCache::~CMsgDataCache(void)
{
	AppUnInit();
}

STDMETHODIMP CMsgDataCache::AppUnInit()
{
	if ( m_dwCookie )
	{
		AtlUnadvise(m_pNetwork, IID_INetworkSink, m_dwCookie);
		m_dwCookie = 0;
	}
	m_pApp = NULL;
	m_pNetwork = NULL;
	return S_OK;
}

STDMETHODIMP CMsgDataCache::AppInit(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;

		hr = AtlAdvise(m_pNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);

		InitData();
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CMsgDataCache::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	if ( Entity == eGameServer && LOGINSUCCESSED(Result) )
	{
		GameResponseInfo* pData = m_pNetwork->GetGameInfo();
		//加入任务信息
		for(int i = 0; i < pData->m_ushTaskNum; ++i)
		{
			TASKINFO kTask;
			kTask.dwID = pData->m_astPlayerDBTaskInfo[i].m_iTaskID;
			if(pData->m_astPlayerDBTaskInfo[i].m_cStatus == 0)
			{
				kTask.dwAccessState = ETAS_Available;
				kTask.dwAchieveState = ETST_Unfinish;
			}
			else if(pData->m_astPlayerDBTaskInfo[i].m_cStatus == 1)
			{
				kTask.dwAccessState = ETAS_Unavailable;
				kTask.dwAchieveState = ETST_Finish;
			}

			CTaskMgr::Get()->AddTaskInfo(kTask);
		}
	}
	return S_OK;
}

STDMETHODIMP CMsgDataCache::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity != eGameServer )
		return S_OK;
	switch(MsgID)
	{
	case ID_CMS_REQUESTLOGIN:
		{
			CResponseLoginPara * pData = (CResponseLoginPara*)Data;
			memset(&m_RoomList, 0, sizeof(m_RoomList));
			m_RoomList.m_nResultID = E_FAIL;

			if( pData->m_nResultID != S_OK && pData->m_nResultID != 1 )
			{
				return S_FALSE;
			}

			if ( pData->m_uiLoginType == ELT_Multi )
			{//多人登录不用创建玩家,检查有没有追踪任务
				if ( m_kEnterRoomTask.dwSvrID != 0 )
				{
					XTrace("有一个追踪任务，处理之(roomid=%d)", m_kEnterRoomTask.dwRoomID);
					CRequestEnterRoomPara kPara;
					memset(&kPara, 0, sizeof(kPara));
					kPara.m_unUin = pData->m_unUin;
					kPara.m_unTime = GetTickCount();
					kPara.m_nRoomID = m_kEnterRoomTask.dwRoomID;
					GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTENTERROOM, (PBYTE)&kPara, 0, FE_ROOM, m_kEnterRoomTask.dwRoomID);
					m_kEnterRoomTask.dwSvrID = 0;
				}
				return S_OK;
			}

			Play * pPlay = NxPlayMgr::GetHostPlay();
			if (!pPlay || pPlay->GetPlayUin() != pData->m_unUin)
			{
				NxPlayMgr::ClearAllPlay();
				KPlayInfo PlayInfo;

				PlayInfo.m_unUin = pData->m_unUin;
				PlayInfo.m_nPlayerID =  pData->m_nPlayerID;
				PlayInfo.m_nRoleID = pData->m_nRoleID;
				PlayInfo.m_nTeamID = 0;
				PlayInfo.m_nKartID = pData->m_nKartID;
				//memcpy(&PlayInfo.m_stPlayerDBBaseInfo,&pData->m_stPlayerDBBaseInfo,sizeof(KPlayBaseInfo));
				PlayInfo.m_stPlayerDBBaseInfo = pData->m_stPlayerDBBaseInfo;

				//	PlayInfo.m_stPlayerDBBaseInfo = (KPlayBaseInfo)pData->m_stPlayerDBBaseInfo;
				PlayInfo.m_nItemNum = min__(pData->m_nItemNum , MAXITEMNUMPERPLAYER);

				for( int i = 0 ; i < PlayInfo.m_nItemNum;  i++){
					//memcpy(&PlayInfo.m_stPlayerDBItemInfo[i], &pData->m_astPlayerDBItemInfo[i], sizeof(KPlayItemInfo));
					PlayInfo.m_stPlayerDBItemInfo[i] = pData->m_astPlayerDBItemInfo[i];

				}
				QQUSERDATA* pUser = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
				strncpy(PlayInfo.m_szQQNickname,pUser->szNickName,MAXNICKNAMELEN);
				//设置角色avatar信息
				SetAvatarInfo(pData->m_unUin,pData->m_nItemNum,pData->m_astPlayerDBItemInfo,&PlayInfo);
				Play * pPlay = NxPlayMgr::CreatePlay(PlayInfo,true);
				//SetAvatarInfo(pData->m_unUin,pData->m_nItemNum,pData->m_astPlayerDBItemInfo, true);
				///载入Shop数据
				CShopMgr::Get()->LoadShopData();

				INetwork* pNetwork = GlobalInterface::m_pNetwork;
				SOCKET_CREATE_INFO* pSocketInfo = pNetwork->GetSocketInfo(eGameServer);
				assert(pSocketInfo);
			}
			else
			{
				NxPlayMgr::ClearAllRemotePlay();
			}
			CALL(NxPlayMgr::GetHostPlay(), ChangeTeam(0));
		}
		break;
	case ID_CMS_REQUESTREGISTER:
		{
			CResponseRegisterPara* pPara = (CResponseRegisterPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				GameResponseInfo* pGameInfo = m_pNetwork->GetGameInfo();
				pGameInfo->m_stPlayerDBBaseInfo.m_bCountry = pPara->m_bCountry;
				strncpy(pGameInfo->m_stPlayerDBBaseInfo.m_szNickname, pPara->m_szNickname, MAXQQKARTNICKNAMELEN);
				pGameInfo->m_stPlayerDBBaseInfo.m_bGender = pPara->m_bGender;
				CAvatarMgr::Get()->GetItemDataMgr()->CleanMainAvatarData(pPara->m_unUin);
				SetAvatarInfo(pPara->m_unUin, pPara->m_nAddItemNum, pPara->m_stAddItemInfo,NULL);
				//SetAvatarInfo(pPara->m_unUin, pPara->m_nAddItemNum, pPara->m_stAddItemInfo, true);
				//加入新手教学
				if(pPara->m_ushAddTaskNum > 0)
				{
					for(int i = 0; i < pPara->m_ushAddTaskNum; ++i)
					{
						TASKINFO kTask;
						kTask.dwID = pPara->m_astAddTaskInfo[i].m_iTaskID;
						kTask.dwAccessState = ETAS_Available;
						kTask.dwAchieveState = ETST_Unfinish;
						CTaskMgr::Get()->AddTaskInfo(kTask);
					}
					/*DWORD dwUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
					DWORD dwTaskId = pPara->m_astAddTaskInfo[0].m_iTaskID;
					PTASKCONFIG pTaskCfg = CTaskMgr::Get()->GetTaskCfg(dwTaskId);
					DWORD dwMapId = pTaskCfg->dwMapID;
					CTaskNet::RequestStartSingleTask(dwUin, dwTaskId, dwMapId);*/
				}
			}
			else
			{
				if (GlobalInterface::m_pScriptState)
				{
					char cmd[256];
					_snprintf(cmd, 255, "Win_ShowMsgBox('%s');", pPara->m_szReason);
					cmd[255] = 0;
					GlobalInterface::m_pScriptState->DoString(cmd);
				}
			}
		}
		break;
	case ID_CMS_REQUESTGETROOMLIST:
		{
			CResponseGetRoomListPara* pPara = (CResponseGetRoomListPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				m_RoomList = *pPara;
			}
		}
		break;
	case ID_CMS_REQUESTCREATEROOM:
		{
			CResponseCreateRoomPara* pPara = (CResponseCreateRoomPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				m_RoomDesc.Reset();
				m_RoomDesc.ValidFlag = 1;
				m_RoomDesc.RoomID = pPara->m_nRoomID;
				m_RoomDesc.GameModeBase = pPara->m_sGameModeBase;
				m_RoomDesc.GameModeSub = pPara->m_sGameModeSub;
				strncpy(m_RoomDesc.RoomName, pPara->m_szRoomName, MAXROOMNAMELEN);
				m_RoomDesc.SeatNum = pPara->m_bSeatNum;
				m_RoomDesc.Flag = pPara->m_bFlag;
				m_RoomDesc.SeatID = pPara->m_bSeatID;
				m_RoomDesc.TeamID=  pPara->m_bTeamID;
				m_RoomDesc.MapID = 0;
				m_RoomDesc.RoomOwnerID = -1;
				m_RoomDesc.PlayerCount = 0;
				for ( int i = pPara->m_bSeatNum; i < MAXSEATNUMINROOM; i++ )
				{
					m_RoomDesc.SeatStatus[i] = Seat_Closed;
				}
				CALL(NxPlayMgr::GetHostPlay(), ChangeTeam(pPara->m_bTeamID));
			}
		}
		break;
	case ID_CMS_REQUESTENTERROOM:
		{
			CResponseEnterRoomPara* pPara = (CResponseEnterRoomPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				m_RoomDesc.Reset();
				m_RoomDesc.ValidFlag = 1;
				m_RoomDesc.RoomID = pPara->m_nRoomID;
				m_RoomDesc.GameModeBase = pPara->m_sGameModeBase;
				m_RoomDesc.GameModeSub = pPara->m_sGameModeSub;
				strncpy(m_RoomDesc.RoomName, pPara->m_szRoomName, MAXROOMNAMELEN);
				m_RoomDesc.SeatNum = pPara->m_bSeatNum;
				m_RoomDesc.Flag = pPara->m_bFlag;
				m_RoomDesc.SeatID = pPara->m_bSeatID;
				m_RoomDesc.TeamID = pPara->m_bTeamID;
				m_RoomDesc.MapID = pPara->m_nMapID;
				m_RoomDesc.RoomOwnerID = pPara->m_nRoomOwnerID;
				memcpy(&m_RoomDesc.SeatStatus[0], &pPara->m_bSeatStatus[0], MAXSEATNUMINROOM * sizeof(char));
				m_RoomDesc.PlayerCount = pPara->m_bPlayerCount;

				for ( int i = 0; i < pPara->m_bPlayerCount; i++ )
				{
					STPlayerRoomVisibleInfo& refPi = pPara->m_stPlayerInfo[i];
					m_RoomDesc.m_stPlayerInfo[refPi.m_bSeatID] = refPi;
				}
				CALL(NxPlayMgr::GetHostPlay(), ChangeTeam(pPara->m_bTeamID));
			}
			
			//-----------------------------------------------------------------------
			if( pPara->m_nResultID == S_FALSE)
			{
				return S_FALSE;
			}
			for ( int j = 0; j < m_RoomDesc.PlayerCount; j++ )
			{
				STPlayerRoomVisibleInfo& refPi = pPara->m_stPlayerInfo[j];
				KPlayInfo PlayInfo;
				PlayInfo.m_unUin = refPi.m_unUin;
				PlayInfo.m_nPlayerID =  refPi.m_nPlayerID;
				PlayInfo.m_nRoleID = refPi.m_nRoleID;
				PlayInfo.m_nTeamID = refPi.m_bTeamID;
				PlayInfo.m_nKartID = refPi.m_nKartID;
//				PlayInfo.m_stPlayerDBBaseInfo.m_bLevel = refPi.m_bLevel;
//				PlayInfo.m_stPlayerDBBaseInfo.m_iNextLevelExamInfo = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unRaceModeExp = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unItemModeExp = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_iMoney = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unWinNum = refPi.m_stPlayerDBBaseInfo.m_unWinNum;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unFirstNum = refPi.m_unFirstNum;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unTotalNum = refPi.m_stPlayerDBBaseInfo.m_unTotalNum;
				PlayInfo.m_stPlayerDBBaseInfo = refPi.m_stPlayerDBBaseInfo;

				PlayInfo.m_nItemNum =  min__(refPi.m_nEquipedItemNum , MAXITEMNUMPERPLAYER);

				for ( int k = 0 ; k < PlayInfo.m_nItemNum; k++){
					PlayInfo.m_stPlayerDBItemInfo[k] = refPi.m_stPlayerItemInfo[k];
					//memcpy(&PlayInfo.m_stPlayerDBItemInfo[k], &refPi.m_stPlayerItemInfo[k], sizeof(KPlayItemInfo));

				}
				strncpy(PlayInfo.m_szQQNickname,refPi.m_stPlayerDBBaseInfo.m_szNickname,MAXNICKNAMELEN);
				//设置角色avatar
				SetAvatarInfo(pPara->m_stPlayerInfo[j].m_unUin,
					pPara->m_stPlayerInfo[j].m_nEquipedItemNum,
					pPara->m_stPlayerInfo[j].m_stPlayerItemInfo,&PlayInfo);
				NxPlayMgr::CreatePlay(PlayInfo, false);
				/*SetAvatarInfo(pPara->m_stPlayerInfo[j].m_unUin,
					pPara->m_stPlayerInfo[j].m_nEquipedItemNum,
					pPara->m_stPlayerInfo[j].m_stPlayerItemInfo, true);
					*/
			}
		}
		break;
	case ID_SMC_NOTIFYENTERROOM:   // 房间中的人会收到这个消息， 进入方面的玩家会在  ResponseEnterRoom 消息中中收到其他玩家的信息（不包含自己的）
		{
			CNotifyEnterRoomPara* pPara = (CNotifyEnterRoomPara*)Data;
			if ( pPara->m_nRoomID == m_RoomDesc.RoomID )
			{
				STPlayerRoomVisibleInfo& refPi = pPara->m_stNewPlayerInfo;
				m_RoomDesc.PlayerCount++;
				m_RoomDesc.m_stPlayerInfo[pPara->m_stNewPlayerInfo.m_bSeatID] = refPi;
			}
			//-----------------------------------------------------------------------
			
			STPlayerRoomVisibleInfo& refPi = pPara->m_stNewPlayerInfo;
			
			KPlayInfo PlayInfo;
			PlayInfo.m_unUin = refPi.m_unUin;
			PlayInfo.m_nPlayerID =  refPi.m_nPlayerID;
			PlayInfo.m_nRoleID = refPi.m_nRoleID;
			PlayInfo.m_nTeamID = refPi.m_bTeamID;
			PlayInfo.m_nKartID = refPi.m_nKartID;
	//		PlayInfo.m_stPlayerDBBaseInfo.m_bLevel = refPi.m_bLevel;
//			PlayInfo.m_stPlayerDBBaseInfo.m_iNextLevelExamInfo = 0;
//			PlayInfo.m_stPlayerDBBaseInfo.m_unRaceModeExp = 0;
//			PlayInfo.m_stPlayerDBBaseInfo.m_unItemModeExp = 0;
//			PlayInfo.m_stPlayerDBBaseInfo.m_iMoney = 0;
//			PlayInfo.m_stPlayerDBBaseInfo.m_unWinNum = refPi.m_stPlayerDBBaseInfo.m_unWinNum;
//			PlayInfo.m_stPlayerDBBaseInfo.m_unFirstNum = refPi.m_unFirstNum;
//			PlayInfo.m_stPlayerDBBaseInfo.m_unTotalNum = refPi.m_stPlayerDBBaseInfo.m_unTotalNum;
			PlayInfo.m_stPlayerDBBaseInfo = refPi.m_stPlayerDBBaseInfo;
			PlayInfo.m_nItemNum =  min__(refPi.m_nEquipedItemNum , MAXITEMNUMPERPLAYER);

			for ( int k = 0 ; k < PlayInfo.m_nItemNum; k++){
				PlayInfo.m_stPlayerDBItemInfo[k] = refPi.m_stPlayerItemInfo[k];
				//memcpy(&PlayInfo.m_stPlayerDBItemInfo[k], &refPi.m_stPlayerItemInfo[k], sizeof(KPlayItemInfo));

			}
			strncpy(PlayInfo.m_szQQNickname,refPi.m_stPlayerDBBaseInfo.m_szNickname,MAXNICKNAMELEN);
			//设置角色avatar
			SetAvatarInfo(pPara->m_stNewPlayerInfo.m_unUin,
				pPara->m_stNewPlayerInfo.m_nEquipedItemNum,
				pPara->m_stNewPlayerInfo.m_stPlayerItemInfo,&PlayInfo);
			NxPlayMgr::CreatePlay(PlayInfo, false);
			/*
			SetAvatarInfo(pPara->m_stNewPlayerInfo.m_unUin,
				pPara->m_stNewPlayerInfo.m_nEquipedItemNum,
				pPara->m_stNewPlayerInfo.m_stPlayerItemInfo, true);
				*/
		}
		break;
	case ID_SMC_NOTIFYLEAVEROOM:
		{
			CNotifyLeaveRoomPara* pPara = (CNotifyLeaveRoomPara*)Data;
			if ( pPara->m_nRoomID == m_RoomDesc.RoomID )
			{
				for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
				{
					if ( m_RoomDesc.m_stPlayerInfo[i].m_unUin == pPara->m_unUin )
					{
						memset(&m_RoomDesc.m_stPlayerInfo[i], 0, sizeof(STPlayerRoomVisibleInfo));
						m_RoomDesc.PlayerCount--;
						break;
					}
				}
				if ( pPara->m_cIsChangeRoomOwner )
				{
					m_RoomDesc.RoomOwnerID = pPara->m_nNewRoomOwnerID;

					assert(m_pNetwork);
					CResponseLoginPara* pPara = m_pNetwork->GetGameInfo();
					if ( m_RoomDesc.RoomOwnerID == pPara->m_nPlayerID )
					{
						m_RoomDesc.Ready = State_UnReadied;
					}
				}
			}
			
			//-----------------------------------------------------------------------
			if( NxPlayMgr::GetHostPlay()->GetPlayUin() == pPara->m_unUin) 
			{
				NxPlayMgr::ClearAllRemotePlay();
			}
			else
			{ 
				NxPlayMgr::RemovePlay(pPara->m_unUin);
			}
				
		}
		break;
	case ID_CMS_REQUESTQUICKENTERROOM:
		{
			CResponseQuickEnterRoomPara* pPara = (CResponseQuickEnterRoomPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				m_RoomDesc.Reset();
				m_RoomDesc.ValidFlag = 1;
				m_RoomDesc.RoomID = pPara->m_nRoomID;
				m_RoomDesc.GameModeBase = pPara->m_sGameModeBase;
				m_RoomDesc.GameModeSub = pPara->m_sGameModeSub;
				strncpy(m_RoomDesc.RoomName, pPara->m_szRoomName, MAXROOMNAMELEN);
				m_RoomDesc.SeatNum = pPara->m_bSeatNum;
				m_RoomDesc.Flag = pPara->m_bFlag;
				m_RoomDesc.SeatID = pPara->m_bSeatID;
				m_RoomDesc.TeamID = pPara->m_bTeamID;
				m_RoomDesc.MapID = pPara->m_nMapID;
				m_RoomDesc.RoomOwnerID = pPara->m_nRoomOwnerID;
				memcpy(&m_RoomDesc.SeatStatus[0], &pPara->m_bSeatStatus[0], MAXSEATNUMINROOM * sizeof(char));
				m_RoomDesc.PlayerCount = pPara->m_bPlayerCount;

				for ( int i = 0; i < pPara->m_bPlayerCount; i++ )
				{
					STPlayerRoomVisibleInfo& refPi = pPara->m_stPlayerInfo[i];
					m_RoomDesc.m_stPlayerInfo[refPi.m_bSeatID] = refPi;
				}
				CALL(NxPlayMgr::GetHostPlay(), ChangeTeam(pPara->m_bTeamID));
			}

			//-----------------------------------------------------------------------
			
			if( pPara->m_nResultID == S_FALSE)
			{
				return S_FALSE;
			}
			for ( int j = 0; j < m_RoomDesc.PlayerCount; j++ )
			{
				STPlayerRoomVisibleInfo& refPi = pPara->m_stPlayerInfo[j];
				
				KPlayInfo PlayInfo;
				PlayInfo.m_unUin = refPi.m_unUin;
				PlayInfo.m_nPlayerID =  refPi.m_nPlayerID;
				PlayInfo.m_nRoleID = refPi.m_nRoleID;
				PlayInfo.m_nTeamID = refPi.m_bTeamID;
				PlayInfo.m_nKartID = refPi.m_nKartID;
				//PlayInfo.m_stPlayerDBBaseInfo.m_bLevel = refPi.m_bLevel;
				//PlayInfo.m_stPlayerDBBaseInfo.m_iNextLevelExamInfo = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unRaceModeExp = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unItemModeExp = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_iMoney = 0;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unWinNum = refPi.m_stPlayerDBBaseInfo.m_unWinNum;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unFirstNum = refPi.m_unFirstNum;
//				PlayInfo.m_stPlayerDBBaseInfo.m_unTotalNum = refPi.m_stPlayerDBBaseInfo.m_unTotalNum;
				PlayInfo.m_stPlayerDBBaseInfo = refPi.m_stPlayerDBBaseInfo;
				PlayInfo.m_nItemNum =  min__(refPi.m_nEquipedItemNum , MAXITEMNUMPERPLAYER);

				for ( int k = 0 ; k < PlayInfo.m_nItemNum; k++){
					PlayInfo.m_stPlayerDBItemInfo[k] = refPi.m_stPlayerItemInfo[k];
					//memcpy(&PlayInfo.m_stPlayerDBItemInfo[k], &refPi.m_stPlayerItemInfo[k], sizeof(KPlayItemInfo));
				}
				strncpy(PlayInfo.m_szQQNickname,refPi.m_stPlayerDBBaseInfo.m_szNickname,MAXNICKNAMELEN);

				//设置角色avatar
				SetAvatarInfo(refPi.m_unUin,
					refPi.m_nEquipedItemNum,
					refPi.m_stPlayerItemInfo,&PlayInfo);
				NxPlayMgr::CreatePlay(PlayInfo, false);
			}
		}
		break;
	case ID_CMS_REQUESTKICKFROMROOM:
		{
			CResponseKickFromRoomPara* pPara = (CResponseKickFromRoomPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
				{
					if ( m_RoomDesc.m_stPlayerInfo[i].m_unUin == pPara->m_unDstUin )
					{
						memset(&m_RoomDesc.m_stPlayerInfo[i], 0, sizeof(STPlayerRoomVisibleInfo));
						m_RoomDesc.PlayerCount--;
						break;
					}
				}
			}

			//-----------------------------------------------------------------------
			
			if ( pPara->m_nResultID == S_FALSE)
			{
				return S_FALSE;
			}

			if( NxPlayMgr::GetHostPlay()->GetPlayUin() != pPara->m_unDstUin) 
			{
				NxPlayMgr::RemovePlay(pPara->m_unDstUin);
			}
		}
		break;
	case ID_SMC_NOTIFYKICKFROMROOM:  // 
		{
			QQUSERDATA* pUser = (QQUSERDATA*)m_pNetwork->GetQQUserData();
			CNotifyKickFromRoomPara* pPara = (CNotifyKickFromRoomPara*)Data;

			if ( pPara->m_unDstUin ==  pUser->Uin )
			{
				CALL(NxPlayMgr::GetHostPlay(), ChangeTeam(0));
				m_RoomDesc.Reset();
			}
			else
			{
				for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
				{
					if ( m_RoomDesc.m_stPlayerInfo[i].m_unUin == pPara->m_unDstUin )
					{
						memset(&m_RoomDesc.m_stPlayerInfo[i], 0, sizeof(STPlayerRoomVisibleInfo));
						m_RoomDesc.PlayerCount--;
						break;
					}
				}
			}	

			if( NxPlayMgr::GetHostPlay()->GetPlayUin() == pPara->m_unDstUin) 
			{
				NxPlayMgr::ClearAllRemotePlay();
			}
			else
			{ 
				NxPlayMgr::RemovePlay(pPara->m_unDstUin);
			}
		}
		break;
	case ID_SMC_NOTIFYCHANGESEATSTATE:
		{
			CNotifyChangeSeatStatePara* pPara = (CNotifyChangeSeatStatePara*)Data;
			m_RoomDesc.SeatStatus[pPara->m_bSeatID] = pPara->m_bSeatState;
		}
		break;
	case ID_SMC_NOTIFYCHANGEREADYSTATE:
		{
			CNotifyChangeReadyStatePara* pPara = (CNotifyChangeReadyStatePara*)Data;
			for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
			{
				if ( m_RoomDesc.m_stPlayerInfo[i].m_unUin == pPara->m_unUin )
				{
					if ( pPara->m_bReadyState == State_Ready )
						m_RoomDesc.ReadyStatus[i] = State_Readied;
					else
						m_RoomDesc.ReadyStatus[i] = State_UnReadied;
					break;
				}
			}
		}
	case ID_CMS_REQUESTCHANGEREADYSTATE:
		{
			CResponseChangeReadyStatePara* pPara = (CResponseChangeReadyStatePara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{	
				if ( pPara->m_bReadyState == State_Ready )
					m_RoomDesc.Ready = State_Readied;
				else
					m_RoomDesc.Ready = State_UnReadied;
			}
		}
		break;
	case ID_CMS_REQUESTCHANGESEATSTATE:
		{
			CResponseChangeSeatStatePara* pPara = (CResponseChangeSeatStatePara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				m_RoomDesc.SeatStatus[pPara->m_bSeatID] = pPara->m_bSeatState;
			}
		}
		break;
	case ID_CMS_REQUESTLEAVEROOM:  // 
		{
			CResponseLeaveRoomPara* pPara = (CResponseLeaveRoomPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				m_RoomDesc.Reset();
			}
			CALL(NxPlayMgr::GetHostPlay(), ChangeTeam(0));
			NxPlayMgr::ClearAllRemotePlay();
		}
		break;
	case ID_CMS_REQUESTLOGOUT:
		{
			CResponseLoginPara* pPara = (CResponseLoginPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				memset(&m_RoomList, 0, sizeof(m_RoomList));
				m_RoomDesc.Reset();
				NxPlayMgr::ClearAllPlay();
			}
		}
		break;

	case ID_SMC_NOTIFYGAMEOVER:
		{
			m_RoomDesc.Ready = State_UnReadied;
			CNotifyGameOverPara * pPara = (CNotifyGameOverPara*)Data;
			for ( int i = 0 ; i < MAXSEATNUMINROOM; i++)
			{
				m_RoomDesc.ReadyStatus[i] = State_UnReadied;
				break;
			}
		}
		break;
	case ID_SMC_NOTIFYCHANGEMAP:
		{
			CNotifyChangeMapPara* pPara = (CNotifyChangeMapPara*)Data;
			m_RoomDesc.MapID = pPara->m_iNewMapID;
		}
		break;
	case ID_CMS_REQUESTCHANGEMAP:
		{
			CResponseChangeMapPara* pPara = (CResponseChangeMapPara*)Data;
			if ( pPara->m_sResultID == S_OK )
			{
				m_RoomDesc.MapID = pPara->m_iNewMapID;
			}
		}
		break;
	case ID_CMS_REQUESTCHANGETEAM:
		{
			CResponseChangeTeamPara* pPara = (CResponseChangeTeamPara*)Data;
			if ( pPara->m_nResultID == S_OK )
			{
				m_RoomDesc.TeamID = pPara->m_bNewTeamID;
				CALL(NxPlayMgr::GetHostPlay(), ChangeTeam(pPara->m_bNewTeamID));
			}
		}
		break;
	case ID_SMC_NOTIFYCHANGETEAM:
		{
			CNotifyPlayerChangeTeamPara* pPara = (CNotifyPlayerChangeTeamPara*)Data;
			for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
			{
				if ( m_RoomDesc.m_stPlayerInfo[i].m_unUin == pPara->m_unUin )
				{
					m_RoomDesc.m_stPlayerInfo[i].m_bTeamID = pPara->m_bNewTeamID;
					CALL(NxPlayMgr::GetPlay(pPara->m_unUin), ChangeTeam(pPara->m_bNewTeamID));
					break;
				}
			}
		}
		break;
	case ID_SMC_NOTIFYCHANGEROOMOWNER:
		{
			CNotifyChangeRoomOwnerPara* pPara = (CNotifyChangeRoomOwnerPara*)Data;
			if ( pPara->m_sRoomID == m_RoomDesc.RoomID )
			{
				m_RoomDesc.RoomOwnerID = pPara->m_nNewRoomOwnerID;
			}
		}
		break;
	case ID_SMC_NOTIFYPLAYERCHANGEITEMSTATUS:
		{
			CNotifyChangeItemStatusPara* pPara = (CNotifyChangeItemStatusPara*)Data;
			CRequestChangeItemStatus	insQu;
			insQu.m_unUin=pPara->m_unUin;
			insQu.m_sItemNum=pPara->m_sItemNum;
			insQu.m_unTime=::GetTickCount();
			UINT nCnt=0;
			for (;nCnt<insQu.m_sItemNum;++nCnt)
			{
				insQu.m_stItemStatus[nCnt].m_iItemID=pPara->m_stItemStatus[nCnt].m_iItemID;
				insQu.m_stItemStatus[nCnt].m_bNewStatus=pPara->m_stItemStatus[nCnt].m_bStatus;
			}
			SetAvatarInfo(pPara->m_unUin,pPara->m_sItemNum,pPara->m_stItemStatus,NULL);
			CAvatarMgr::Get()->ChangeItemStatus(&insQu,true);
			CALL(NxPlayMgr::GetPlay(pPara->m_unUin), ChangeTeam());
			CStringEx str;
			str.Format("%d", pPara->m_unUin);
			CallScript(GlobalInterface::m_pScriptState, "UI.children.room.OnNotifyChangeItemStatus", str.data());
		}
		break;
	case ID_SMC_NOTIFYADDITEM:
		{
			CNotifyClientAddItemPara* pPara = (CNotifyClientAddItemPara*)Data;
			//如果是正式装备还要更新后备列表
			SetAvatarInfo(pPara->m_unUin,pPara->m_sItemNum,pPara->m_stItemInfo,NULL);
		}
		break;
	case ID_SMC_NOTIFYUPDATEMONEY:
		{
			CNotifyClientUpdateMoneyPara* pPara = (CNotifyClientUpdateMoneyPara*)Data;
			//更新酷币
			CResponseLoginPara* pInfo = GlobalInterface::m_pNetwork->GetGameInfo();
			pInfo->m_stPlayerDBBaseInfo.m_iMoney=pPara->m_iCurrentMoney;
			TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyUpdateMoney");
			XTrace("商店购买后更新酷币总值=%u",pPara->m_iCurrentMoney);
		}
		break;
	case ID_SMC_NOTIFYGAMEBEGIN:
		m_RoomDesc.Ready = State_UnReadied;
		for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
		{
			m_RoomDesc.ReadyStatus[i] = State_UnReadied;
		}
		break;
	case ID_CMS_REQUESTCHECKNAME:
		break;
	case ID_CMS_REQUESTASSIGNATTRIBUTE:
		{
			CResponseAssignAttributePara * pPara = (CResponseAssignAttributePara*)Data;
			CResponseLoginPara* pLoginInfo = m_pNetwork->GetGameInfo();
			assert(pPara);
			assert(pLoginInfo);
			pLoginInfo->m_stPlayerDBAttributeInfo = pPara->m_stPlayerDBAttributeInfo;
		}
		break;

	case ID_CMS_REQUESTCLEARATTRIBUTE:
		{
			CResponseClearAttributePara * pPara = (CResponseClearAttributePara*)Data;
			CResponseLoginPara* pLoginInfo = m_pNetwork->GetGameInfo();
			assert(pPara);
			assert(pLoginInfo);
			if ( pPara->m_nResultID == S_OK )
			{
				pLoginInfo->m_stPlayerDBAttributeInfo = pPara->m_stPlayerDBAttributeInfo;
			}
		}
		break;
	case ID_SMC_NOTIFYRACEOVER:
		{
			CNotifyRaceOverPara* pPara = (CNotifyRaceOverPara*)Data;
			CResponseLoginPara* pResponseLoginPara = m_pNetwork->GetGameInfo();

			for (int i = 0; i < pPara->m_bCurrentPlayerNum; ++i)
			{

				// 如果有升级更新玩家的属性
				if (pPara->m_astRaceScore[i].m_unUin == pResponseLoginPara->m_unUin)
				{
					if (pPara->m_astRaceScore[i].m_nAttrResInc)
						pResponseLoginPara->m_stPlayerDBAttributeInfo.m_unReserve = pPara->m_astRaceScore[i].m_nTotalAttrRes;
					pResponseLoginPara->m_stPlayerDBBaseInfo.m_unExperience = pPara->m_astRaceScore[i].m_nTotalExp;
					pResponseLoginPara->m_stPlayerDBBaseInfo.m_iMoney       = pPara->m_astRaceScore[i].m_nTotalMoney;	
				}
				// 更新经验值和金钱
				STPlayerRoomVisibleInfo* pPlayerInfo = FindPlayerInfo(pPara->m_astRaceScore[i].m_unUin);
				if (pPlayerInfo)
				{
					pPlayerInfo->m_stPlayerDBBaseInfo.m_unExperience = pPara->m_astRaceScore[i].m_nTotalExp;
					pPlayerInfo->m_stPlayerDBBaseInfo.m_iMoney       = pPara->m_astRaceScore[i].m_nTotalMoney;						
				}				
			}
		}
		break;
	}
	return S_OK;
}

STDMETHODIMP CMsgDataCache::OnClose(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CMsgDataCache::OnConnect(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CMsgDataCache::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CMsgDataCache::OnStartLogin(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STPlayerRoomVisibleInfo* CMsgDataCache::FindPlayerInfo(UINT Uin)
{
	if ( !m_RoomDesc.ValidFlag )
	{
		return NULL;
	}
	for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
	{
		STPlayerRoomVisibleInfo* pInfo = &m_RoomDesc.m_stPlayerInfo[i];
		if ( Uin == pInfo->m_unUin )
		{
			return pInfo;
		}
	}
	return NULL;
}

bool CMsgDataCache::SetAvatarInfo(ULONG dwUin,UINT nNum,STPlayerDBItemInfo* pDbItem,KPlayInfo* pInfo)
{
	bool bRes=true;
	UINT nCntTmp=0;
	UINT nHairId=0;
	ItemStore* pStore=NULL;
	ItemUnit* pItem=NULL;
	if (pInfo)
	{
		pInfo->m_nKartID=0;
		pInfo->m_nRoleID=0;
	}

	for(;nCntTmp<nNum;++nCntTmp)
	{//添加到用户Avatar系统
		pItem=CAvatarMgr::Get()->GetItemDataMgr()->GetItemById(pDbItem[nCntTmp].m_iItemID);
		_ASSERTE(pItem);
		CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(dwUin,&pDbItem[nCntTmp],true);
		if (pInfo)
		{
			if (EAIBT_HAIR==(pItem->nType & 0xff00)  && pDbItem[nCntTmp].m_bStatus)
			{
				nHairId=pDbItem[nCntTmp].m_iItemID;
			}
			else if (EAIBT_ROLE==(pItem->nType & 0xff00) && pDbItem[nCntTmp].m_bStatus)
			{
				pInfo->m_nRoleID=pDbItem[nCntTmp].m_iItemID;
			}
			else if (EAIBT_CAR==(pItem->nType & 0xff00) && pDbItem[nCntTmp].m_bStatus)
			{
				pInfo->m_nKartID=pDbItem[nCntTmp].m_iItemID;
			}
		}

	}
	//
	if (pInfo)
	{//只要要装配一辆车和一个角色
		if(!pInfo->m_nKartID && !pInfo->m_nRoleID)
		{//todo:默认的角色和车辆先在这里设，第一次注册时需要这样处理
			STPlayerDBItemInfo insSta;
			/*
			UINT nRnd=rand();
			switch(nRnd%2)
			{
			case 0:
				pInfo->m_nKartID=10020;
				break;
			case 1:
				pInfo->m_nKartID=10024;
			    break;
			default:
				pInfo->m_nKartID=10020;
			    break;
			}
			*/
			pInfo->m_nKartID=10020;
			pInfo->m_nRoleID=10018;
			nHairId=10021;

			insSta.m_bStatus=1;
			insSta.m_iItemID=pInfo->m_nKartID;
			insSta.m_iItemNum=1;
			CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(dwUin,&insSta,true);
			insSta.m_iItemID=pInfo->m_nRoleID;
			CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(dwUin,&insSta,true);
			//加头发
			/*
			insSta.m_iItemID=nHairId;
			CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(dwUin,&insSta,true);
			*/
		}
		else
		{
			_ASSERTE(pInfo->m_nKartID && pInfo->m_nRoleID);
		}
	}
	
	return bRes;
}

bool CMsgDataCache::AddEnterRoomTask(DWORD dwSvrID, DWORD dwRoomID)
{
	m_kEnterRoomTask.dwSvrID = dwSvrID;
	m_kEnterRoomTask.dwRoomID = dwRoomID;
	return true;
}
