#pragma once

#include "NiPoint3.h"
#include "network/msgheader.h"
#include "stdlib.h"
#define min__(a,b)    (((a) < (b)) ? (a) : (b))
/*
struct KPlayBaseInfo
{
	UCHAR	m_bLevel;
	LONG	m_iNextLevelExamInfo;
	ULONG	m_unRaceModeExp;
	ULONG	m_unItemModeExp;
	LONG	m_iMoney;
	ULONG	m_unWinNum;
	ULONG	m_unFirstNum;
	ULONG	m_unTotalNum;
public:
	void clear()
	{
		m_bLevel = 0;
		m_iNextLevelExamInfo= 0;
		m_unRaceModeExp= 0;
		m_unItemModeExp= 0;
		m_iMoney= 0;
		m_unWinNum= 0;
		m_unFirstNum= 0;
		m_unTotalNum= 0;
	}
	KPlayBaseInfo()
	{
		m_bLevel = 0;
		m_iNextLevelExamInfo= 0;
		m_unRaceModeExp= 0;
		m_unItemModeExp= 0;
		m_iMoney= 0;
		m_unWinNum= 0;
		m_unFirstNum= 0;
		m_unTotalNum= 0;
	}
	KPlayBaseInfo(const KPlayBaseInfo & BaseInfo)
	{
		m_bLevel= 				BaseInfo.m_bLevel;
		m_iNextLevelExamInfo= 				BaseInfo.m_iNextLevelExamInfo;
		m_unRaceModeExp= 				BaseInfo.m_unRaceModeExp;
		m_unItemModeExp= 				BaseInfo.m_unItemModeExp;
		m_iMoney= 				BaseInfo.m_iMoney;
		m_unWinNum= 				BaseInfo.m_unWinNum;
		m_unFirstNum= 				BaseInfo.m_unFirstNum;
		m_unTotalNum= 				BaseInfo.m_unTotalNum;
	}
	KPlayBaseInfo & operator= ( const KPlayBaseInfo & BaseInfo)
	{
		m_bLevel= 				BaseInfo.m_bLevel;
		m_iNextLevelExamInfo= 				BaseInfo.m_iNextLevelExamInfo;
		m_unRaceModeExp= 				BaseInfo.m_unRaceModeExp;
		m_unItemModeExp= 				BaseInfo.m_unItemModeExp;
		m_iMoney= 				BaseInfo.m_iMoney;
		m_unWinNum= 				BaseInfo.m_unWinNum;
		m_unFirstNum= 				BaseInfo.m_unFirstNum;
		m_unTotalNum= 				BaseInfo.m_unTotalNum;
		return *this;
	}

};


struct KPlayItemInfo
{
public:
	void clear()
	{
		m_iItemID=  0;
		m_nItemNum=  0;
		m_iAvailPeriod=  0;
		m_bStatus=  0;
		m_unObtainTime=  0;
		m_iOtherAttribute=  0;
	}
	KPlayItemInfo()
	{
		m_iItemID=  0;
		m_nItemNum=  0;
		m_iAvailPeriod=  0;
		m_bStatus=  0;
		m_unObtainTime=  0;
		m_iOtherAttribute=  0;

	}
	KPlayItemInfo( const KPlayItemInfo & ItemInfo)
	{
		m_iItemID=				ItemInfo.m_iItemID;
		m_nItemNum=				ItemInfo.m_nItemNum;
		m_iAvailPeriod=				ItemInfo.m_iAvailPeriod;
		m_bStatus=				ItemInfo.m_bStatus;
		m_unObtainTime=				ItemInfo.m_unObtainTime;
		m_iOtherAttribute=				ItemInfo.m_iOtherAttribute;

	}

	KPlayItemInfo & operator= (const KPlayItemInfo & ItemInfo)
	{
		m_iItemID=				ItemInfo.m_iItemID;
		m_nItemNum=				ItemInfo.m_nItemNum;
		m_iAvailPeriod=				ItemInfo.m_iAvailPeriod;
		m_bStatus=				ItemInfo.m_bStatus;
		m_unObtainTime=				ItemInfo.m_unObtainTime;
		m_iOtherAttribute=				ItemInfo.m_iOtherAttribute;
		return *this;
	}

	LONG	m_iItemID;
	USHORT	m_nItemNum;
	LONG	m_iAvailPeriod;
	UCHAR	m_bStatus;
	ULONG	m_unObtainTime;
	LONG	m_iOtherAttribute;
};

*/

struct KPlayInfo
{

	ULONG					m_unUin;
	USHORT					m_nPlayerID;
	USHORT					m_nRoleID;
	USHORT					m_nTeamID;
	USHORT					m_nKartID;
	STPlayerDBBaseInfo		m_stPlayerDBBaseInfo;
	USHORT					m_nItemNum;
	STPlayerDBItemInfo		m_stPlayerDBItemInfo[MAXITEMNUMPERPLAYER];
	
	INT						m_nKartType;
	NiPoint3				m_SpawnPoint;
	CHAR				m_szQQNickname[MAXNICKNAMELEN];

	void Clear()
	{
		m_unUin = 0;
		m_nPlayerID = 0;
		m_nRoleID = 0;
		m_nTeamID = 0;
		m_nKartID = 0;
		//m_stPlayerDBBaseInfo.clear();
		ZeroMemory(&m_stPlayerDBBaseInfo, sizeof(m_stPlayerDBBaseInfo));
		m_nItemNum = 0;
		for ( int i = 0; i < MAXITEMNUMPERPLAYER; i++){
			//m_stPlayerDBItemInfo[i].clear();
			ZeroMemory(&m_stPlayerDBItemInfo[i], sizeof(STPlayerDBItemInfo));
		}
		m_nKartType = 0;
		m_SpawnPoint.x = 0;
		m_SpawnPoint.y = 0;
		m_SpawnPoint.z = 0;
		memset(m_szQQNickname,0,sizeof(CHAR)*MAXNICKNAMELEN);
	}

	KPlayInfo()
	{
		m_unUin = 0;
		m_nPlayerID = 0;
		m_nRoleID = 0;
		m_nTeamID = 0;
		m_nKartID = 0;
		//m_stPlayerDBBaseInfo.clear();
		ZeroMemory(&m_stPlayerDBBaseInfo, sizeof(m_stPlayerDBBaseInfo));
		m_nItemNum = 0;
		for ( int i = 0; i < MAXITEMNUMPERPLAYER; i++){
			//m_stPlayerDBItemInfo[i].clear();
			ZeroMemory(&m_stPlayerDBItemInfo[i], sizeof(STPlayerDBItemInfo));
		}
		m_nKartType = 0;
		m_SpawnPoint.x = 0;
		m_SpawnPoint.y = 0;
		m_SpawnPoint.z = 0;
		memset(m_szQQNickname,0,sizeof(CHAR)*MAXNICKNAMELEN);
	}
	KPlayInfo(const KPlayInfo & Info)
	{
		m_unUin = Info.m_unUin;
		m_nPlayerID = Info.m_nPlayerID;
		m_nRoleID = Info.m_nRoleID;
		m_nTeamID = Info.m_nTeamID;
		m_nKartID = Info.m_nKartID;
		m_stPlayerDBBaseInfo= Info.m_stPlayerDBBaseInfo;
		m_nItemNum = Info.m_nItemNum;
		for ( int i = 0; i < min__(Info.m_nItemNum,MAXITEMNUMPERPLAYER); i++){
			m_stPlayerDBItemInfo[i]= Info.m_stPlayerDBItemInfo[i];
		}
		m_nKartType = Info.m_nKartType;
		m_SpawnPoint = Info.m_SpawnPoint;
		memcpy(m_szQQNickname,Info.m_szQQNickname,sizeof(CHAR)*MAXNICKNAMELEN);
	}
	KPlayInfo & operator= (const KPlayInfo & Info)
	{
		m_unUin = Info.m_unUin;
		m_nPlayerID = Info.m_nPlayerID;
		m_nRoleID = Info.m_nRoleID;
		m_nTeamID = Info.m_nTeamID;
		m_nKartID = Info.m_nKartID;
		m_stPlayerDBBaseInfo= Info.m_stPlayerDBBaseInfo;
		m_nItemNum = Info.m_nItemNum;
		for ( int i = 0; i < min__(Info.m_nItemNum,MAXITEMNUMPERPLAYER); i++){
			m_stPlayerDBItemInfo[i]= Info.m_stPlayerDBItemInfo[i];
		}
		m_nKartType = Info.m_nKartType;
		m_SpawnPoint = Info.m_SpawnPoint;
		memcpy(m_szQQNickname,Info.m_szQQNickname,sizeof(CHAR)*MAXNICKNAMELEN);
		return *this;
	}
};