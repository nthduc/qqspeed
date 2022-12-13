#pragma once

#include "IGameSystem.h"
#include "iplayerdata.h"
#include "msg.h"

#define INVALID_TEAMID -1
#define INVALID_PLAYERUIN -1

inline const QQUSERDATA* GetLocalUserInfo(IGameSystem* pApp)
{
	return pApp->GetNetMgr()->GetUserData();
}

/*
inline ITeamData* GetTeamByPlayerUin(IGameSystem* pApp, UINT Uin)
{
    UINT aUin = GetLocalUserInfo(pApp)->Uin;
	IPlayerData * pPlayerData = GetPlayerByUin(pApp->GetPlayerDatas(), aUin);
	if ( !pPlayerData )
		return NULL;

	UINT nTeamID = pPlayerData->GetPlayerBaseInfo()->nTeamID;
	return GetTeamByTeamID(pApp->GetTeamDatas(), nTeamID);
}
*/
