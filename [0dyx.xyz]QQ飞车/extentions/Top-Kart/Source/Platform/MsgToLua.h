////////////////////////////////////////////////
//     FileName:    MsgToLua.h
//     Author:      Fish
//     Date:        2006-10-19 10:32:13
//     Description: []
////////////////////////////////////////////////
#pragma once

#include "network/msg.h"
#include "datadef.h"
#include "../Avatar/ItemDefine.h"
#include "../PlayerRank.h"

/*
inline void set_field(lua_State* ls, LPCSTR index, INT value)
{
	lua_pushstring(ls, index);
	lua_pushnumber(ls, value);
	lua_settable(ls, -3);
}

inline void set_field(lua_State* ls, LPCSTR index, LPCSTR value)
{
	lua_pushstring(ls, index);
	lua_pushstring(ls, value);
	lua_settable(ls, -3);
}

inline void set_field(lua_State* ls, INT index, INT value)
{
	lua_pushnumber(ls, index);
	lua_pushnumber(ls, value);
	lua_settable(ls, -3);
}

inline void set_field(lua_State* ls, INT index, LPCSTR value)
{
	lua_pushnumber(ls, index);
	lua_pushstring(ls, value);
	lua_settable(ls, -3);
}
*/

#define MSG_DEFINE(msgst, msg)		inline void \
									MsgToLua(lua_State* ls, msgst* msg, BOOL Embeded = FALSE, INT index = -1)	\
									{	\
										if ( Embeded && index != -1 )	\
										{	\
											lua_pushnumber(ls, index);	\
											lua_newtable(ls);			\
										}				\
										if ( !Embeded )	\
											lua_newtable(ls);
#define MSG_FIELD(name, value)			SetTableElement(ls, name, value);
#define MSG_FIELD_LOOP(c, name, value)	{lua_pushstring(ls, name);	\
										lua_newtable(ls);	\
										int index = 1;	\
										for ( int i = 0; i < c; i++ )	\
										{	\
											SetTableElement(ls, index++, value[i]);	\
										}	\
										lua_settable(ls, -3);	\
										}
#define EMBED_MSG(c, fldname, fld)		lua_pushstring(ls, fldname);	\
										lua_newtable(ls);	\
										for ( int i = 0; i < c; i++ ){	\
											MsgToLua(ls, &msg->fld[i], TRUE, i+1); lua_settable(ls, -3); }	\
										lua_settable(ls, -3);
#define EMBED_MSG_NOLOOP(fldname, fld)	lua_pushstring(ls, fldname);	\
										lua_newtable(ls);	\
										MsgToLua(ls, &msg->fld, TRUE, -1); 	\
										lua_settable(ls, -3);
#define MSG_END()						if ( !Embeded ) lua_setglobal(ls, "Param");	\
										}
#define DEFINE_MSGTOLUA(ls, msg)		MsgToLua(ls, msg, TRUE);
#define DEFINE_MSGTOLUA_IDX(ls, msg, i) MsgToLua(ls, msg, TRUE, i);
#define DEFINE_MSGTOLUA_PARAM			MsgToLua(ls, msg);
/*
//玩家数据库基本信息
struct STPlayerDBBaseInfo
{
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
	UCHAR	m_bGender;   		 				//玩家性别，0表示女，1表示男
	UCHAR	m_bCountry;	                                             //玩家国家
	USHORT 	m_unExperience;		       //经验
	SHORT 	m_iMoney;			       //金钱
	USHORT 	m_unWinNum;			//胜利次数
	USHORT 	m_unTotalNum;			//比赛总数
	USHORT  m_unHonor;                              //功勋值
};
*/

MSG_DEFINE(STPlayerDBBaseInfo, msg)
MSG_FIELD("nickname",		msg->m_szNickname)
MSG_FIELD("gender",			msg->m_bGender)
MSG_FIELD("country",	msg->m_bCountry)
MSG_FIELD("exp",		msg->m_unExperience)
MSG_FIELD("money",		msg->m_iMoney)
MSG_FIELD("winnum",		msg->m_unWinNum)
MSG_FIELD("totnum",		msg->m_unTotalNum)
MSG_FIELD("honor",		msg->m_unHonor)
MSG_END()
/*
//玩家属性信息
struct STPlayerDBAttributeInfo
{
	USHORT m_nSpeed;               //速度
	USHORT m_nExplosion;         //爆发
	USHORT m_nGathering;         //集气
	USHORT m_nSpout;                //喷射
	USHORT m_nMiniSpout;        //小喷
	USHORT m_unReserve;             //未分配
	UCHAR  m_bStrategy;            //自动分配策略
};
*/

MSG_DEFINE(STPlayerDBAttributeInfo, msg)
MSG_FIELD("speedprop",		msg->m_nSpeed)
MSG_FIELD("explosionprop",	msg->m_nExplosion)
MSG_FIELD("gatherprop",		msg->m_nGathering)
MSG_FIELD("spoutprop",	msg->m_nSpout)
MSG_FIELD("minispotprop",	msg->m_nMiniSpout)
MSG_FIELD("reserveprop",	msg->m_unReserve)
MSG_FIELD("strategyprop",	msg->m_bStrategy)
MSG_END()

MSG_DEFINE(STPlayerDBItemInfo, msg)
MSG_FIELD("itemid",		msg->m_iItemID)
MSG_FIELD("itemnum",	msg->m_iItemNum)
MSG_FIELD("availperiod",msg->m_iAvailPeriod)
MSG_FIELD("status",		msg->m_bStatus)
MSG_FIELD("obtaintime",	msg->m_unObtainTime)
MSG_FIELD("reserved",	msg->m_iOtherAttribute)
MSG_END()

/*
//回复登陆游戏
class CResponseLoginPara :public CMsgPara
{
SHORT 				m_nResultID;                
UCHAR 		m_bKeyGameDataLength;
CHAR m_szKeyGameData[MAXCSGAMEKEYLEN]; 
USHORT 		m_unUin;
USHORT 		m_unIdentity; 						 	//玩家身份
USHORT 		m_nPlayerID;
USHORT		m_nRoleID;                                    //角色id，暂定
USHORT		m_nKartID; //车ID		
USHORT 		m_unGameSvrdIdentity;  					//一些gsvrd的信息，如是否免费等
STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;					//玩家基本信息
STPlayerDBAttributeInfo     m_stPlayerDBAttributeInfo;                                 //玩家属性信息
USHORT		m_nItemNum;
STPlayerDBItemInfo	m_astPlayerDBItemInfo[MAXITEMNUMPERPLAYER];	
UCHAR 		m_bReasonLen;
CHAR 	m_szReason[MAXATTACHCONTENTLEN];	
};
*/

MSG_DEFINE(CResponseLoginPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("uin",		msg->m_unUin)
MSG_FIELD("identity",	msg->m_unIdentity)
MSG_FIELD("pid",		msg->m_nPlayerID)
MSG_FIELD("logintype",	msg->m_uiLoginType)
MSG_FIELD("gsvrflag",	msg->m_unGameSvrdIdentity)
EMBED_MSG_NOLOOP("dbinfo", m_stPlayerDBBaseInfo)
EMBED_MSG_NOLOOP("attribinfo", m_stPlayerDBAttributeInfo)
MSG_FIELD("itemnum",	msg->m_nItemNum)
EMBED_MSG(msg->m_nItemNum, "items", m_astPlayerDBItemInfo)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseRegisterPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("uin",		msg->m_unUin)
MSG_FIELD("additemnum", msg->m_nAddItemNum);
EMBED_MSG(msg->m_nAddItemNum, "items", m_stAddItemInfo)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseCreateRoomPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("roomid",		msg->m_nRoomID)
MSG_FIELD("gamemodebase",	msg->m_sGameModeBase)
MSG_FIELD("gamemodesub",	msg->m_sGameModeSub)
MSG_FIELD("roomname",	msg->m_szRoomName)
MSG_FIELD("seatnum",	msg->m_bSeatNum)
MSG_FIELD("flag",		msg->m_bFlag)
MSG_FIELD("seatid",		msg->m_bSeatID)
MSG_FIELD("teamid",		msg->m_bTeamID)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseChangeMapPara, msg)
MSG_FIELD("resultid",	msg->m_sResultID)
MSG_FIELD("mapid",		msg->m_iNewMapID)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CNotifyChangeMapPara, msg)
MSG_FIELD("uin",	msg->m_unUin)
MSG_FIELD("mapid",		msg->m_iNewMapID)
MSG_END()

MSG_DEFINE(STRoomInfo, msg)
MSG_FIELD("roomid",		msg->m_nRoomID)
MSG_FIELD("gamemodebase",	msg->m_sGameModeBase)
MSG_FIELD("gamemodesub",	msg->m_sGameModeSub)
MSG_FIELD("pnum",		msg->m_bCurrentPlayerNum)
MSG_FIELD("seatnum",	msg->m_bTotalSeatNum)
MSG_FIELD("status",		msg->m_bStatus)
MSG_FIELD("mapid",		msg->m_iMapID)
MSG_FIELD("flag",		msg->m_bFlag)
MSG_FIELD("roomname",	msg->m_szRoomName)
MSG_END()

MSG_DEFINE(STPositionInfo, msg)
MSG_FIELD("status",		msg->m_nStatus)
MSG_FIELD("svrid",	msg->m_iServerID)
EMBED_MSG_NOLOOP("roominfo",	m_stRoomInfo)
MSG_END()

MSG_DEFINE(STPlayerRoomVisibleInfo, msg)
MSG_FIELD("uin",		msg->m_unUin)
MSG_FIELD("identity",	msg->m_unIdentity)
MSG_FIELD("seatid",		msg->m_bSeatID)
MSG_FIELD("teamid",		msg->m_bTeamID)
MSG_FIELD("pid",		msg->m_nPlayerID)
MSG_FIELD("rid",		msg->m_nRoleID)
MSG_FIELD("kartid",		msg->m_nKartID)
MSG_FIELD("status",		msg->m_bStatus)
EMBED_MSG_NOLOOP("playerdbinfo", m_stPlayerDBBaseInfo)
/*MSG_FIELD("nickname",	msg->m_stPlayerDBBaseInfo.m_szNickname)
MSG_FIELD("gender",		msg->m_stPlayerDBBaseInfo.m_bGender)
MSG_FIELD("expn",		msg->m_stPlayerDBBaseInfo.m_unExperience)
MSG_FIELD("winnum",		msg->m_stPlayerDBBaseInfo.m_unWinNum)
MSG_FIELD("totnum",	msg->m_stPlayerDBBaseInfo.m_unTotalNum)*/
MSG_FIELD("itemnum",	msg->m_nEquipedItemNum)
EMBED_MSG(msg->m_nEquipedItemNum, "items", m_stPlayerItemInfo)
MSG_END()

MSG_DEFINE(STPrice, msg)
MSG_FIELD("qbi",	msg->m_iQBPrice)
MSG_FIELD("qdian",	msg->m_iQPPrice)
MSG_FIELD("kbi",	msg->m_iKBPrice)
MSG_END()

MSG_DEFINE(STDescription, msg)
MSG_FIELD("generaldesc",	msg->m_szGeneralDesc)
MSG_FIELD("attributedesc",	msg->m_szAttributeDesc)
MSG_END()

MSG_DEFINE(STCommodity, msg)
MSG_FIELD("itemid",		msg->m_iID)
MSG_FIELD("itemname",	msg->m_szName)
MSG_FIELD("saletag",	msg->m_iSaleTag)
EMBED_MSG_NOLOOP("price", m_stPrice)
EMBED_MSG_NOLOOP("desc", m_stDescription)
MSG_END()

/*
struct STGameInfo
{
STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;					//玩家数据库基本信息
STPlayerDBAttributeInfo    m_stPlayerDBAttributeInfo;                                    //玩家属性信息
USHORT		m_nItemNum;
STPlayerDBItemInfo	m_stPlayerDBItemInfo[MAXITEMNUMPERPLAYER];
STPlayerDBOtherInfo	m_stPlayerDBOtherInfo;						//其他信息
};
*/

/*
LONG m_iRightFlag;		//是否禁言
LONG m_iAvailPeriod;	//禁言时长	
*/
MSG_DEFINE(STPlayerDBOtherInfo, msg)
MSG_FIELD("forbidspeek", msg->m_iRightFlag)
MSG_FIELD("forbidtime", msg->m_iAvailPeriod)
MSG_END();

MSG_DEFINE(STGameInfo, msg)
EMBED_MSG_NOLOOP("playerdbbaseinfo", m_stPlayerDBBaseInfo)
EMBED_MSG_NOLOOP("playerdbattribinfo", m_stPlayerDBBaseInfo)
MSG_FIELD("itemnum", msg->m_nItemNum)
EMBED_MSG(msg->m_nItemNum, "items", m_stPlayerDBItemInfo)
EMBED_MSG_NOLOOP("playerdbotherinfo", m_stPlayerDBOtherInfo)
MSG_END();

MSG_DEFINE(QueryPlayerAvatarInfo, msg)
MSG_FIELD("itemnum",	msg->m_nItemNum_InOut)
EMBED_MSG(msg->m_nItemNum_InOut, "items", m_stItemInfo_Out)
MSG_END()

MSG_DEFINE(CResponseGetRoomListPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("totroomnum",	msg->m_nCurTotalRoomNum)
MSG_FIELD("startfrom",	msg->m_nStartIndex)
MSG_FIELD("getnum",		msg->m_nRoomNum)
EMBED_MSG(msg->m_nRoomNum, "rooms", m_stRooms)
MSG_END()

MSG_DEFINE(CResponseEnterRoomPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("roomid",		msg->m_nRoomID)
MSG_FIELD("gamemodebase",	msg->m_sGameModeBase)
MSG_FIELD("gamemodesub",	msg->m_sGameModeSub)
MSG_FIELD("roomname",	msg->m_szRoomName)
MSG_FIELD("seatnum",	msg->m_bSeatNum)
MSG_FIELD("flag",		msg->m_bFlag)
MSG_FIELD("seatid",		msg->m_bSeatID)
MSG_FIELD("teamid",		msg->m_bTeamID)
MSG_FIELD("mapid",		msg->m_nMapID)
MSG_FIELD("roomerid",	msg->m_nRoomOwnerID)
MSG_FIELD_LOOP(MAXSEATNUMINROOM, "seatstatus",	msg->m_bSeatStatus)
MSG_FIELD("pnum",		msg->m_bPlayerCount)
EMBED_MSG(msg->m_bPlayerCount, "players", m_stPlayerInfo)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseLeaveRoomPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseQuickEnterRoomPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("roomid",		msg->m_nRoomID)
MSG_FIELD("gamemodebase",	msg->m_sGameModeBase)
MSG_FIELD("gamemodesub",	msg->m_sGameModeSub)
MSG_FIELD("roomname",	msg->m_szRoomName)
MSG_FIELD("seatnum",	msg->m_bSeatNum)
MSG_FIELD("flag",		msg->m_bFlag)
MSG_FIELD("seatid",		msg->m_bSeatID)
MSG_FIELD("teamid",		msg->m_bTeamID)
MSG_FIELD("mapid",		msg->m_nMapID)
MSG_FIELD("roomerid",	msg->m_nRoomOwnerID)
MSG_FIELD_LOOP(MAXSEATNUMINROOM, "seatstatus",	msg->m_bSeatStatus)
MSG_FIELD("pnum",		msg->m_bPlayerCount)
EMBED_MSG(msg->m_bPlayerCount, "players", m_stPlayerInfo)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseChangeSeatStatePara, msg)
MSG_FIELD("resultid",		msg->m_nResultID)
MSG_FIELD("seatid",			msg->m_bSeatID)
MSG_FIELD("seatstate",		msg->m_bSeatState)
MSG_FIELD("reasonlen",		msg->m_bReasonLen)
MSG_FIELD("reason",			msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseKickFromRoomPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("uin",		msg->m_unDstUin)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseChangeReadyStatePara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("readystate",	msg->m_bReadyState)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseTalkPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("type",		msg->m_bType)
MSG_FIELD("uin",	msg->m_unDestPlayerUin)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CNotifyEnterRoomPara, msg)
MSG_FIELD("roomid",	msg->m_nRoomID)
EMBED_MSG_NOLOOP("pinfo", m_stNewPlayerInfo)
MSG_END()

MSG_DEFINE(CNotifyLeaveRoomPara, msg)
MSG_FIELD("roomid",	msg->m_nRoomID)
MSG_FIELD("uin",	msg->m_unUin)
MSG_FIELD("seatid",	msg->m_bSeatID)
MSG_FIELD("changeroomer",	msg->m_cIsChangeRoomOwner)
MSG_FIELD("newroomerid",	msg->m_nNewRoomOwnerID)
MSG_END()

MSG_DEFINE(CNotifyChangeSeatStatePara, msg)
MSG_FIELD("seatid",		msg->m_bSeatID)
MSG_FIELD("seatstate",	msg->m_bSeatState)
MSG_END()

MSG_DEFINE(CNotifyKickFromRoomPara, msg)
MSG_FIELD("roomid",	msg->m_nRoomID)
MSG_FIELD("uin",	msg->m_unDstUin)
MSG_END()

MSG_DEFINE(CNotifyChangeReadyStatePara, msg)
MSG_FIELD("uin",	msg->m_unUin)
MSG_FIELD("readystate",	msg->m_bReadyState)
MSG_END()

MSG_DEFINE(CNotifyTalkPara, msg)
MSG_FIELD("uin",		msg->m_unSrcUin)
MSG_FIELD("nickname",	msg->m_szSrcNickName)
MSG_FIELD("gender",		msg->m_bGender)
MSG_FIELD("type",		msg->m_bType)
MSG_FIELD("dstuin",		msg->m_unDestPlayerUin)
MSG_FIELD("contentlen",	msg->m_nContentLength)
MSG_FIELD("content",	msg->m_szContent)
MSG_END()

MSG_DEFINE(CNotifyKickFromServerPara, msg)
MSG_FIELD("reasonid",	msg->m_nReasonID)
MSG_FIELD("uin",		msg->m_unSrcUin)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CRequestTalkPara, msg)
MSG_FIELD("uin",	msg->m_unUin)
MSG_FIELD("type",	msg->m_bType)
MSG_FIELD("dstuin",	msg->m_unDestPlayerUin)
MSG_FIELD("contentlen",	msg->m_nContentLength)
MSG_FIELD("content",	msg->m_szContent)
MSG_END()

MSG_DEFINE(CResponseStartGamePara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CNotifyGameBeginPara, msg)
MSG_FIELD("loadtime",	msg->m_unNormalLoadTime)
MSG_FIELD("mapid",		msg->m_nMapID)
MSG_FIELD("maphash",	(LPCSTR)msg->m_szMapHash)
MSG_FIELD("gameid",		msg->m_unGameID)
MSG_FIELD("seed",		msg->m_unSeed)
MSG_FIELD("pnum",		msg->m_bCurrentPlayerNum1)
MSG_FIELD_LOOP(MAXSEATNUMINROOM, "uins",	msg->m_anRaceTrackOrders)
MSG_END()

MSG_DEFINE(CNotifySingleGameBeginPara, msg)
MSG_FIELD("loadtime",	msg->m_uiNormalLoadTime)
MSG_FIELD("taskid",		msg->m_iTaskID)
MSG_FIELD("mapid",		msg->m_iMapID)
MSG_FIELD("maphash",	(LPCSTR)msg->m_szMapHash)
MSG_FIELD("gameid",		msg->m_uiGameID)
MSG_FIELD("seed",		msg->m_uiSeed)
MSG_FIELD("totalround",	msg->m_shTotalMapRound)
MSG_FIELD("limittime",	msg->m_uiTimeLimit)
MSG_END()

MSG_DEFINE(CNotifyGameOverPara, msg)
MSG_FIELD("resultid",   msg->m_nResultID)
MSG_END()


MSG_DEFINE(CNotifyWaitUnReadyPlayersPara, msg)
MSG_FIELD("waittime", msg->m_unWaitLoadTime)
MSG_END()

MSG_DEFINE(CNotifyRaceBeginPara, msg)
MSG_FIELD("lefttime", msg->m_unCountDownTime)
MSG_END()

MSG_DEFINE(CNotifyCountDownPara, msg)
MSG_FIELD("lefttime", msg->m_unCountDownTime)
MSG_FIELD("firstuin", msg->m_unUin)
MSG_END()

MSG_DEFINE(STRaceScore, msg)
MSG_FIELD("uin",		msg->m_unUin)
MSG_FIELD("fintime",	msg->m_unFinTime)
MSG_FIELD("tp",			msg->m_nTP)
MSG_FIELD("moneyadd",	msg->m_nMoneyInc)
MSG_FIELD("money",		msg->m_nTotalMoney)
MSG_FIELD("expadd",		msg->m_nExpInc)
MSG_FIELD("expn",		msg->m_nTotalExp)
MSG_FIELD("awardnum",	msg->m_bAwardNum)
MSG_FIELD("attribinc",  msg->m_nAttrResInc)
MSG_FIELD("restattrib", msg->m_nTotalAttrRes)
MSG_FIELD_LOOP(msg->m_bAwardNum, "awards",	msg->m_nSpecialAward)
MSG_END()

MSG_DEFINE(CNotifyRaceOverPara, msg)
MSG_FIELD("pnum", msg->m_bCurrentPlayerNum)
EMBED_MSG(msg->m_bCurrentPlayerNum, "scores", m_astRaceScore)
MSG_FIELD("winnerid", msg->m_bWinTeamID)
MSG_FIELD("wintype", msg->m_iWinType)
MSG_END()

MSG_DEFINE(RoomDesc, msg)
MSG_FIELD("valid",		msg->ValidFlag)
MSG_FIELD("roomid",		msg->RoomID)
MSG_FIELD("gamemodebase",	msg->GameModeBase)
MSG_FIELD("gamemodesub",	msg->GameModeSub)
MSG_FIELD("roomname",	msg->RoomName)
MSG_FIELD("seatnum",	msg->SeatNum)
MSG_FIELD("flag",		msg->Flag)
MSG_FIELD("seatid",		msg->SeatID)
MSG_FIELD("teamid",		msg->TeamID)
MSG_FIELD("mapid",		msg->MapID)
MSG_FIELD("readystate",	msg->Ready)
MSG_FIELD("roomerid",	msg->RoomOwnerID)
MSG_FIELD_LOOP(MAXSEATNUMINROOM, "seatstatus",	msg->SeatStatus)
MSG_FIELD_LOOP(MAXSEATNUMINROOM, "readystatus", msg->ReadyStatus)
MSG_FIELD("pnum",		msg->PlayerCount)
EMBED_MSG(MAXSEATNUMINROOM, "players", m_stPlayerInfo)
MSG_END()

MSG_DEFINE(CResponseChangeTeamPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("teamid",		msg->m_bNewTeamID)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CNotifyPlayerChangeTeamPara, msg)
MSG_FIELD("uin",	msg->m_unUin)
MSG_FIELD("teamid",	msg->m_bNewTeamID)
MSG_END()

MSG_DEFINE(CNotifyChangeRoomOwnerPara, msg)
MSG_FIELD("roomid",	msg->m_sRoomID)
MSG_FIELD("roomerid",	msg->m_nNewRoomOwnerID)
MSG_END()

MSG_DEFINE(STItemStatus, msg)
MSG_FIELD("itemid",	msg->m_iItemID)
MSG_FIELD("status",	msg->m_bNewStatus)
MSG_END()

MSG_DEFINE(CResponseChangeItemStatus, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("itemnum",	msg->m_sItemNum)
EMBED_MSG(msg->m_sItemNum,"items",	m_stItemStatus)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(STFindPlayerInfo, msg)
MSG_FIELD("uin", msg->m_unUin)
MSG_FIELD("identity", msg->m_unIdentity)
MSG_FIELD("pid", msg->m_nPlayerID)
EMBED_MSG_NOLOOP("playerdbinfo", m_stPlayerDBBaseInfo)
/*
MSG_FIELD("nickname", msg->m_stPlayerDBBaseInfo.m_szNickname)
MSG_FIELD("gender", msg->m_stPlayerDBBaseInfo.m_bGender)
MSG_FIELD("expn", msg->m_stPlayerDBBaseInfo.m_unExperience)
MSG_FIELD("winnum", msg->m_stPlayerDBBaseInfo.m_unWinNum)
MSG_FIELD("totalnum", msg->m_stPlayerDBBaseInfo.m_unTotalNum)
*/
MSG_FIELD("itemnum",	msg->m_nEquipedItemNum)
EMBED_MSG(msg->m_nEquipedItemNum, "items",	m_stPlayerItemInfo)
MSG_END()

MSG_DEFINE(CResponseFindPlayerByQQPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
EMBED_MSG_NOLOOP("pinfo", m_stPlayerInfo)
MSG_FIELD("isonline",	msg->m_bIsOnLine)
EMBED_MSG_NOLOOP("positioninfo",	m_stPositionInfo)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseCheckNamePara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("nickname",	msg->m_szNickname)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseAssignAttributePara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
EMBED_MSG_NOLOOP("attribinfo",	m_stPlayerDBAttributeInfo)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseClearAttributePara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
EMBED_MSG_NOLOOP("attribinfo",	m_stPlayerDBAttributeInfo)
MSG_FIELD("reasonlen",	msg->m_bReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(CResponseGetWorldInfoPara, msg)
MSG_FIELD("resultid",	msg->m_ushResultID)
MSG_FIELD("reasonlen",	msg->m_ushReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()


MSG_DEFINE(CResponseBuyCommodityPara, msg)
MSG_FIELD("resultid",	msg->m_nResultID)
MSG_FIELD("reasonlen",	msg->m_ushReasonLen)
MSG_FIELD("reason",		msg->m_szReason)
MSG_END()

MSG_DEFINE(PLAY_RANKINFO, msg)
MSG_FIELD("uin", msg->Uin)
MSG_FIELD("teamid", msg->nTeamID)
MSG_FIELD("nickname", msg->szNickName)
MSG_FIELD("netspeed", msg->nNetSpeed)
MSG_FIELD("finish", msg->bFinish)
MSG_FIELD("color", msg->nColor)
MSG_FIELD("gp1", msg->dwGPs[0])
MSG_FIELD("gp2", msg->dwGPs[1])
MSG_END()

MSG_DEFINE(GAME_RANKINFO, msg)
MSG_FIELD("pnum", msg->nPlayNum)
EMBED_MSG(msg->nPlayNum, "ri", ri)
MSG_END()
