/*------this file was generate by autocoder tool, please do not modify it!-----*/
#include <wtypes.h>

#pragma comment(lib, "ws2_32.lib")

#ifndef __code_h__
#define __code_h__

struct CCSHead
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iTotalLength;
	SHORT	m_nVer;
	SHORT	m_nDialogID;
	LONG	m_iSeq;
	ULONG	m_iUin;
	UCHAR	m_byBodyFlag;
	UCHAR	m_byOptLength;
	UCHAR	m_abyOption[0];
};

struct CMsgHead
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_sMsgID;
	SHORT	m_sMsgType;
	LONG	m_iMsgSeq;
	CHAR	m_bSrcFE;
	CHAR	m_bDstFE;
	SHORT	m_sSrcID;
	SHORT	m_sDstID;
};

struct CTransferHead
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nTotalLength;
	UCHAR	m_cVer;
	LONG	m_iSeq;
	SHORT	m_nDialogID;
	ULONG	m_unUin;
	SHORT	m_nTransferCmd;
	SHORT	m_nCheckSum;
	UCHAR	m_byOptLength;
	UCHAR	m_abyOption[MAXOPTLENGTH];
};

struct STUdpPack
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	MsgID;
};

struct STUDPPlayMove : public STUdpPack
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	UCHAR	m_cHasKeyChange;
	UCHAR	m_cArrKey[MAX_KEY_NUM];
	ULONG	m_unPlayAnimID;
	ULONG	m_unKartAnimID;
	LONG	m_unLinearVelocity[3];
	LONG	m_unArrPosition[3];
	LONG	m_unArrMat[9];
	LONG	m_unOffSetZ;
	LONG	m_unAngle;
	LONG	m_nRestDistance;
	ULONG	m_unPackageIndex;
	ULONG	m_unTimeStamp;
	UCHAR	m_cisDrifting;
};

struct STUDPPlayCollision : public STUdpPack
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unSelfUin;
	ULONG	m_unOtherUin;
	LONG	m_nSelfLinearVel[3];
	LONG	m_nOtherLinearVel[3];
	LONG	m_nSelfPos[3];
	LONG	m_nOtherPos[3];
	ULONG	m_unTime;
	LONG	buf[50];
};

struct CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
};

struct CRequestPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CResponsePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_sResultID;
};

struct STRoomInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nRoomID;
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
	UCHAR	m_bCurrentPlayerNum;
	UCHAR	m_bTotalSeatNum;
	UCHAR	m_bStatus;
	LONG	m_iMapID;
	UCHAR	m_bFlag;
	CHAR	m_szRoomName[MAXROOMNAMELEN];
};

struct STPlayerDBBaseInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];
	UCHAR	m_bGender;
	UCHAR	m_bCountry;
	ULONG	m_unExperience;
	LONG	m_iMoney;
	ULONG	m_unWinNum;
	ULONG	m_unTotalNum;
	ULONG	m_unHonor;
};

struct STPlayerDBAttributeInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	USHORT	m_nSpeed;
	USHORT	m_nExplosion;
	USHORT	m_nGathering;
	USHORT	m_nSpout;
	USHORT	m_nMiniSpout;
	ULONG	m_unReserve;
	UCHAR	m_bStrategy;
};

struct STPlayerDBOtherInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iRightFlag;
	LONG	m_iAvailPeriod;
};

struct STPlayerDBItemInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iItemID;
	LONG	m_iItemNum;
	LONG	m_iAvailPeriod;
	UCHAR	m_bStatus;
	ULONG	m_unObtainTime;
	LONG	m_iOtherAttribute;
};

struct STPlayerRoomVisibleInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unIdentity;
	UCHAR	m_bSeatID;
	UCHAR	m_bTeamID;
	USHORT	m_nPlayerID;
	USHORT	m_nRoleID;
	USHORT	m_nKartID;
	UCHAR	m_bStatus;
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;
	USHORT	m_nEquipedItemNum;
	STPlayerDBItemInfo	m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];
};

struct STGameInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;
	STPlayerDBAttributeInfo	m_stPlayerDBAttributeInfo;
	USHORT	m_nItemNum;
	STPlayerDBItemInfo	m_stPlayerDBItemInfo[MAXITEMNUMPERPLAYER];
	STPlayerDBOtherInfo	m_stPlayerDBOtherInfo;
};

struct STPlayerDBTaskInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iTaskID;
	SHORT	m_shOptionID;
	CHAR	m_cStatus;
	UCHAR	m_bPrevStatus;
	LONG	m_iProgressRate;
	LONG	m_iOther;
	LONG	m_iTakeTime;
};

struct STPrice
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iQBPrice;
	LONG	m_iQPPrice;
	LONG	m_iKBPrice;
};

struct STType
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shBase;
	SHORT	m_shSub;
};

struct STDescription
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	CHAR	m_szGeneralDesc[MAX_COMMODITY_DESCLEN];
	CHAR	m_szAttributeDesc[MAX_COMMODITY_DESCLEN];
};

struct STRebate
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shMemeber;
	SHORT	m_shCommon;
};

struct STLimit
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iLevelLimit;
	LONG	m_iHonorLimit;
	LONG	m_iOtherLimit;
};

struct STItemInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iItemID;
	LONG	m_shAvailPeriod;
	SHORT	m_shItemNum;
};

struct STCommodity
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iID;
	CHAR	m_szName[MAX_COMMODITY_NAMELEN];
	STType	m_stType;
	STDescription	m_stDescription;
	STRebate	m_stRebate;
	STLimit	m_stLimit;
	STPrice	m_stPrice;
	LONG	m_iDateSaleLimit;
	LONG	m_iClientVersion;
	LONG	m_iPriority;
	LONG	m_iSaleTag;
	SHORT	m_shItemCount;
	STItemInfo	m_astItemInfo[MAX_ITEM_PERCOMMODITY];
	LONG	m_iDateSold;
};

struct CRequestLoginPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unClientVersion;
	CHAR	m_szQQNickname[MAXNICKNAMELEN];
	UCHAR	m_bGender;
	UCHAR	m_bIconID;
	ULONG	m_unAttachIdentify;
	ULONG	m_uiLoginType;
};

struct CResponseLoginPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bKeyGameDataLength;
	CHAR	m_szKeyGameData[MAXCSGAMEKEYLEN];
	ULONG	m_unUin;
	ULONG	m_unIdentity;
	USHORT	m_nPlayerID;
	ULONG	m_uiLoginType;
	USHORT	m_nRoleID;
	USHORT	m_nKartID;
	ULONG	m_unGameSvrdIdentity;
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;
	STPlayerDBAttributeInfo	m_stPlayerDBAttributeInfo;
	USHORT	m_nItemNum;
	STPlayerDBItemInfo	m_astPlayerDBItemInfo[MAXITEMNUMPERPLAYER];
	SHORT	m_ushTaskNum;
	STPlayerDBTaskInfo	m_astPlayerDBTaskInfo[MAXTASKNUMPERPLAYER];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestLogoutPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CResponseLogoutPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestRegisterPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];
	UCHAR	m_bGender;
	LONG	m_nFaceID;
	LONG	m_nHairID;
	UCHAR	m_bDressIndex;
	LONG	m_nColorID;
	UCHAR	m_bCountry;
};

struct CResponseRegisterPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];
	UCHAR	m_bGender;
	UCHAR	m_bCountry;
	USHORT	m_nAddItemNum;
	STPlayerDBItemInfo	m_stAddItemInfo[MAXREGISTERADDITEMNUM];
	USHORT	m_ushAddTaskNum;
	STPlayerDBTaskInfo	m_astAddTaskInfo[MAXREGISTERADDTASKNUM];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestGetRoomListPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_nStartRoomNum;
	SHORT	m_nGetRoomNum;
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
};

struct CResponseGetRoomListPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	SHORT	m_nCurTotalRoomNum;
	SHORT	m_nStartIndex;
	SHORT	m_nRoomNum;
	STRoomInfo	m_stRooms[MAXCLIENTGETROOMNUM];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
};

struct CRequestCreateRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
	CHAR	m_szRoomName[MAXROOMNAMELEN];
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag;
	CHAR	m_szPassword[MAXROOMPASSWORDLEN];
};

struct CResponseCreateRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	SHORT	m_nRoomID;
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
	CHAR	m_szRoomName[MAXROOMNAMELEN];
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag;
	UCHAR	m_bSeatID;
	UCHAR	m_bTeamID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestEnterRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_nRoomID;
	CHAR	m_szPassword[MAXROOMPASSWORDLEN];
};

struct CResponseEnterRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	SHORT	m_nRoomID;
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
	CHAR	m_szRoomName[MAXROOMNAMELEN];
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag;
	UCHAR	m_bSeatID;
	UCHAR	m_bTeamID;
	LONG	m_nMapID;
	SHORT	m_nRoomOwnerID;
	UCHAR	m_bSeatStatus[MAXSEATNUMINROOM];
	UCHAR	m_bPlayerCount;
	STPlayerRoomVisibleInfo	m_stPlayerInfo[MAXSEATNUMINROOM];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyEnterRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nRoomID;
	STPlayerRoomVisibleInfo	m_stNewPlayerInfo;
};

struct CRequestLeaveRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CResponseLeaveRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyLeaveRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nRoomID;
	ULONG	m_unUin;
	UCHAR	m_bSeatID;
	CHAR	m_cIsChangeRoomOwner;
	SHORT	m_nNewRoomOwnerID;
};

struct CRequestTransferUDPOKPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_nDstDlg;
	ULONG	m_unDstUin;
	SHORT	m_nInfoLength;
	CHAR	m_szInfo[32];
};

struct CResponseTransferUDPOKPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
};

struct CNotifyUDPOKPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_nSrcDlg;
	ULONG	m_unSrcUin;
	SHORT	m_nInfoLength;
	CHAR	m_szInfo[32];
};

struct CRequestQuickEnterRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
};

struct CResponseQuickEnterRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	SHORT	m_nRoomID;
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
	CHAR	m_szRoomName[MAXROOMNAMELEN];
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag;
	UCHAR	m_bSeatID;
	UCHAR	m_bTeamID;
	LONG	m_nMapID;
	SHORT	m_nRoomOwnerID;
	UCHAR	m_bSeatStatus[MAXSEATNUMINROOM];
	UCHAR	m_bPlayerCount;
	STPlayerRoomVisibleInfo	m_stPlayerInfo[MAXSEATNUMINROOM];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestChangeSeatStatePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_nRoomID;
	UCHAR	m_bSeatID;
	UCHAR	m_bSeatState;
};

struct CResponseChangeSeatStatePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bSeatID;
	UCHAR	m_bSeatState;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyChangeSeatStatePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	UCHAR	m_bSeatID;
	UCHAR	m_bSeatState;
};

struct CRequestKickFromRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unDstUin;
};

struct CResponseKickFromRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unDstUin;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyKickFromRoomPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nRoomID;
	ULONG	m_unDstUin;
};

struct CRequestChangeReadyStatePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	UCHAR	m_bReadyState;
};

struct CResponseChangeReadyStatePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bReadyState;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyChangeReadyStatePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	UCHAR	m_bReadyState;
};

struct CRequestTalkPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	UCHAR	m_bType;
	ULONG	m_unDestPlayerUin;
	SHORT	m_nContentLength;
	CHAR	m_szContent[MAXCHATCONTENTLEN];
};

struct CResponseTalkPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bType;
	ULONG	m_unDestPlayerUin;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyTalkPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unSrcUin;
	CHAR	m_szSrcNickName[MAXNICKNAMELEN];
	UCHAR	m_bGender;
	UCHAR	m_bType;
	ULONG	m_unDestPlayerUin;
	SHORT	m_nContentLength;
	CHAR	m_szContent[MAXCHATCONTENTLEN];
};

struct CRequestHelloPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	UCHAR	m_bInfoLength;
	CHAR	m_abyInfo[MAXATTACHCONTENTLEN];
};

struct CResponseHelloPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyTestNetSpeedPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unStartSec;
	ULONG	m_unUSec;
};

struct CAckTestNetSpeedPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unStartSec;
	ULONG	m_unUSec;
};

struct CNotifyKickFromServerPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nReasonID;
	ULONG	m_unSrcUin;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct STFriendGroupInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	CHAR	m_szFriendGroupName[MAXFRIENDGROUPNAMELEN];
	SHORT	m_nFriendGroupID;
	SHORT	m_nCurrentFriendNum;
	ULONG	m_unFriendUin[MAXFRIENDNUMPERGROUP];
};

struct STPlayerInfoForFriend
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	CHAR	m_szNickName[MAXNICKNAMELEN];
};

struct CRequestGetFriendListPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CResponseGetFriendListPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	SHORT	m_nMaxFriendNum;
	SHORT	m_nTotalFriendNum;
	SHORT	m_nFriendGroupNum;
	STFriendGroupInfo	m_astFriendGroupInfo[MAXGROUPNUMPERPLAYER];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestAddFriendPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unDstUin;
	SHORT	m_nWordLength;
	CHAR	m_szWords[MAXWORDSLEN];
};

struct CResponseAddFriendPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	ULONG	m_unDstUin;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestDeleteFriendPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unFriendUin;
	SHORT	m_nFriendGroupID;
};

struct CResponseDeleteFriendPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	ULONG	m_unFriendUin;
	SHORT	m_nFriendGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestAddFriendGroupPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	UCHAR	m_bGroupNameLen;
	CHAR	m_szGroupName[MAXFRIENDGROUPNAMELEN];
};

struct CResponseAddFriendGroupPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	UCHAR	m_bGroupNameLen;
	CHAR	m_szGroupName[MAXFRIENDGROUPNAMELEN];
	SHORT	m_nFriendGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestDeleteFriendGroupPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_nFriendGroupID;
};

struct CResponseDeleteFriendGroupPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	SHORT	m_nFriendGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestMoveFriendToSomeGroupPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unFriendUin;
	SHORT	m_nSrcGroupID;
	SHORT	m_nDstGroupID;
};

struct CResponseMoveFriendToSomeGroupPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	ULONG	m_unFriendUin;
	SHORT	m_nSrcGroupID;
	SHORT	m_nDstGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyBeAddedAsFriendPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unSrcUin;
	ULONG	m_unDstUin;
	STPlayerInfoForFriend	m_stSrcPlayerInfo;
	SHORT	m_nWordLength;
	CHAR	m_szWords[MAXWORDSLEN];
};

struct CAckBeAddedAsFriendPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unSrcUin;
	ULONG	m_unDstUin;
	SHORT	m_nWordLength;
	CHAR	m_szWords[MAXWORDSLEN];
};

struct CNotifyAddFriendResultPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unSrcUin;
	ULONG	m_unDstUin;
	SHORT	m_nWordLength;
	CHAR	m_szWords[MAXWORDSLEN];
};

struct CRequestFindPlayerByQQPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unDstUin;
};

struct STFindPlayerInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unIdentity;
	USHORT	m_nPlayerID;
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;
	USHORT	m_nEquipedItemNum;
	STPlayerDBItemInfo	m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];
};

struct STPositionInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	USHORT	m_nStatus;
	LONG	m_iServerID;
	STRoomInfo	m_stRoomInfo;
};

struct CResponseFindPlayerByQQPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	STFindPlayerInfo	m_stPlayerInfo;
	UCHAR	m_bIsOnLine;
	STPositionInfo	m_stPositionInfo;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct NOTIFY_ANTI_BOT
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_dwUin;
	SHORT	m_nBufferLen;
	CHAR	m_szBuffer[32000];
};

struct CRequestStartGamePara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CResponseStartGamePara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyGameBeginPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unNormalLoadTime;
	LONG	m_nMapID;
	UCHAR	m_szMapHash[MAP_HASH_CODE];
	ULONG	m_unGameID;
	ULONG	m_unSeed;
	CHAR	m_bCurrentPlayerNum1;
	ULONG	m_anRaceTrackOrders[MAXSEATNUMINROOM];
	SHORT	m_nLap;
	ULONG	m_unPropUpdateInterval;
};

struct CNotifyGameOverPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
};

struct CRequestPrepareReadyPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CNotifyWaitUnReadyPlayersPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unWaitLoadTime;
};

struct CNotifyRaceBeginPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unCountDownTime;
};

struct CRequestPlayerFinishRacePara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unFinUin;
	ULONG	m_unFinResult;
};

struct CNotifyPlayerFinishRacePara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
};

struct CNotifyCountDownPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unCountDownTime;
	ULONG	m_unUin;
};

struct STRaceScore
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unFinTime;
	LONG	m_nTP;
	LONG	m_nMoneyInc;
	LONG	m_nTotalMoney;
	LONG	m_nExpInc;
	LONG	m_nTotalExp;
	LONG	m_nAttrResInc;
	LONG	m_nTotalAttrRes;
	CHAR	m_bAwardNum;
	LONG	m_nSpecialAward[MAXAWORDSIZE];
};

struct CNotifyRaceOverPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	CHAR	m_bCurrentPlayerNum;
	STRaceScore	m_astRaceScore[MAXSEATNUMINROOM];
	CHAR	m_bWinTeamID;
	LONG	m_iWinType;
	ULONG	m_unReceiveHonorTime;
};

struct CNotifyRaceShowPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unRaceShowTime;
};

struct CRequestReportCurrentInfoPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_iCurrentStatus;
	LONG	m_iCurrenRate[3];
	LONG	m_iCurrentPosition[3];
	UCHAR	m_bPassedCheckPointNum;
	USHORT	m_nPassedCheckPointIDs[MAXCHECKPOINTNUM];
};

struct CRequestReportDriftPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_aiSpeedOfDriftStart[3];
	LONG	m_aiSpeedOfDriftStop[3];
	LONG	m_aiPositonOfDirftStart[3];
	LONG	m_aiPositionOfDriftStop[3];
	LONG	m_iDriftFlag;
	LONG	m_iAccelerateFuelInc;
};

struct CResponseReportDriftPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_sResultID;
	SHORT	m_sPropID;
	LONG	m_iPropIndex;
	LONG	m_iCurrentAccelerateFuel;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestGetPropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sPropPositionNO;
	UCHAR	m_bCurrentRank;
	LONG	m_aiCurrentPosition[3];
	LONG	m_iPreviousInterval;
};

struct CResponseGetPropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	SHORT	m_sPropID;
	LONG	m_iPropIndex;
};

struct CNotifyGetPropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	SHORT	m_sPropID;
	LONG	m_iPropIndex;
	SHORT	m_sPropPositionNO;
};

struct CRequestUsePropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sPropID;
	ULONG	m_unDstUin;
	LONG	m_aiCurrentPosition[3];
	LONG	m_iPropIndex;
	SHORT	m_sNewID;
};

struct CResponseUsePropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_unUin;
	SHORT	m_sPropID;
	ULONG	m_unDstUin;
	LONG	m_aiPropPosition[3];
	LONG	m_iPropIndex;
};

struct CNotifyUsePropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	SHORT	m_sPropID;
	ULONG	m_unDstUin;
	LONG	m_aiPropPosition[3];
	LONG	m_iPropIndex;
	SHORT	m_sNewID;
};

struct CRequestPropEffectResultPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sPropID;
	LONG	m_iPropIndex;
	SHORT	m_nEffectResult;
	LONG	m_iItemID;
};

struct CNotifyPropEffectResultPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	SHORT	m_sPropID;
	LONG	m_iPropIndex;
};

struct CRequestChangeAimStatePara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unDstUin;
	UCHAR	m_bAimState;
};

struct CNotifyChangeAimStatePara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unDstUin;
	UCHAR	m_bAimState;
};

struct CRequestChangeMapPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_iNewMapID;
};

struct CResponseChangeMapPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_sResultID;
	LONG	m_iNewMapID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyChangeMapPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	LONG	m_iNewMapID;
};

struct CRequestChangeTeamPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	UCHAR	m_bNewTeamID;
};

struct CResponseChangeTeamPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	UCHAR	m_bNewTeamID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyPlayerChangeTeamPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	UCHAR	m_bNewTeamID;
};

struct CNotifyChangeRoomOwnerPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_sRoomID;
	SHORT	m_nNewRoomOwnerID;
};

struct STReplacePropInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iReplaceFlag;
	LONG	m_iPropIndex;
	SHORT	m_sPropID;
};

struct STRacerReplacePropInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	STReplacePropInfo	m_astReplaceProp[MAXPROPBLANK];
};

struct CNotifyReplacePropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_sPlayerNum;
	STRacerReplacePropInfo	m_astRacerReplacePropInfo[MAXSEATNUMINROOM];
	LONG	m_iItemID;
};

struct CNotifyAddPropPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	SHORT	m_sPropID;
	LONG	m_iPropIdx;
	LONG	m_iItemID;
};

struct CNotifyReportDriftPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iTeamAccelerateFule;
};

struct STItemStatus
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iItemID;
	UCHAR	m_bNewStatus;
};

struct CRequestChangeItemStatus
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sItemNum;
	STItemStatus	m_stItemStatus[MAXCHANGEITEMSTATUSNUM];
};

struct CResponseChangeItemStatus : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	SHORT	m_sItemNum;
	STItemStatus	m_stItemStatus[MAXCHANGEITEMSTATUSNUM];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyChangeItemStatusPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	SHORT	m_sItemNum;
	STPlayerDBItemInfo	m_stItemStatus[MAXCHANGEITEMSTATUSNUM];
};

struct CRequestAssignAttributePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	STPlayerDBAttributeInfo	m_stPlayerDBAttributeInfo;
};

struct CResponseAssignAttributePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	STPlayerDBAttributeInfo	m_stPlayerDBAttributeInfo;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestClearAttributePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CResponseClearAttributePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	STPlayerDBAttributeInfo	m_stPlayerDBAttributeInfo;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestCheckNamePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];
};

struct CResponseCheckNamePara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestBuyCommodityPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_iReqBuySeq;
	LONG	m_iCommodityID;
	LONG	m_iPrice;
	LONG	m_iPayType;
	ULONG	m_unDstUin;
	SHORT	m_nAttachInfoLen;
	UCHAR	m_abyAttachInfo[MAXATTACHINFOLEN];
};

struct CResponseBuyCommodityPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_nResultID;
	ULONG	m_uiUin;
	LONG	m_iReqBuySeq;
	LONG	m_iCommodityID;
	LONG	m_iPayType;
	LONG	m_iPrice;
	ULONG	m_uiDstUin;
	USHORT	m_ushReasonLen;
	CHAR	m_szReason[MAX_REASON_LEN];
};

struct CRequestGetCommodityListPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unTime;
};

struct CResponseGetCommodityListPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shResultID;
	CHAR	m_cEndFlag;
	SHORT	m_shCommodityNum;
	STCommodity	m_astCommodity[MAX_COMMODITY_NUM];
	USHORT	m_ushReasonLen;
	CHAR	m_szReason[MAX_REASON_LEN];
};

struct CNotifyClientAddItemPara : public CMsgPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_unUin;
	ULONG	m_unSrcUin;
	SHORT	m_sItemNum;
	STPlayerDBItemInfo	m_stItemInfo[MAXITEMNUMONCEADD];
	SHORT	m_sAttachLen;
	UCHAR	m_szAttachInfo[MAXATTACHINFOLEN];
};

struct STDownloadServerInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiIPAddr;
	USHORT	m_ushPort;
	USHORT	m_ushLocationID;
};

struct STDirServerInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iServerID;
	USHORT	m_ushLocationID;
	ULONG	m_uiServerIP;
	USHORT	m_aushServerPort[MAX_DIR_LISTEN_PORT_NUM];
};

struct STWorldBaseInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	UCHAR	m_byWorldID;
	UCHAR	m_byNameLen;
	CHAR	m_szName[MAX_WORLD_NAME_LEN];
	UCHAR	m_byDirNum;
	STDirServerInfo	m_astDirServerInfo[MAX_WORLD_DIR_SERVER_NUM];
};

struct STServerBaseInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iServerID;
	USHORT	m_ushLocationID;
	ULONG	m_uiServerIP;
	USHORT	m_ushServerPort;
};

struct STGameServerInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iServerID;
	ULONG	m_uiServerIP;
	USHORT	m_ushTcpPort;
	USHORT	m_ushUdpPort;
	USHORT	m_ushMaxPlayerNum;
	USHORT	m_ushCurrentPlayerNum;
};

struct STCommGameServerInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	STGameServerInfo	m_stServerInfo;
	USHORT	m_ushLocationID;
};

struct STIDCServerInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	USHORT	m_ushLocationID;
	UCHAR	m_byGameServerNum;
	STGameServerInfo	m_astGameServerInfo[MAX_IDC_GANE_SERVER_NUM];
};

struct STChannelInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	UCHAR	m_byChannelID;
	UCHAR	m_byIDCNum;
	STIDCServerInfo	m_astIDCServerInfo[MAX_IDC_SERVER_NUM];
};

struct STCountryInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	UCHAR	m_byCountryID;
	UCHAR	m_byChannelNum;
	STChannelInfo	m_astChannelInfo[MAX_COUNTRY_CHANNEL_NUM];
};

struct CRequestGetWorldInfoPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	ULONG	m_uiTime;
	ULONG	m_uiClientVersion;
	ULONG	m_uiMasterVersion;
	ULONG	m_uiSlaveVersion;
	UCHAR	m_byWorldID;
	UCHAR	m_szFileHash[HASH_CODE_LENGTH];
};

struct CResponseGetWorldInfoPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	USHORT	m_ushResultID;
	ULONG	m_uiLastClientVersion;
	ULONG	m_uiLastMasterVersion;
	ULONG	m_uiLastSlaveVersion;
	UCHAR	m_byDownloadServerNum;
	STDownloadServerInfo	m_astDownloadServerInfo[MAX_DOWNSVR_NUMBER];
	UCHAR	m_byRootDirectionLen;
	CHAR	m_szRootDirection[MAX_DIRECTION_LENGTH];
	ULONG	m_uiMasterFullVersion;
	ULONG	m_uiSlaveFullVersion;
	USHORT	m_ushP2POption;
	UCHAR	m_byWorldCount;
	STWorldBaseInfo	m_astWorldBaseInfo[MAX_WORLD_NUM];
	UCHAR	m_byCountryCount;
	STCountryInfo	m_astCountryInfo[MAX_COUNTRY_NUM];
	UCHAR	m_byShopServerCount;
	STServerBaseInfo	m_astShopServerInfo[MAX_SHOP_SERVER_NUM];
	UCHAR	m_byBorderServerCount;
	STCommGameServerInfo	m_astBorderServerInfo[MAX_BORDER_SERVER_NUM];
	UCHAR	m_byMatchServerCount;
	STCommGameServerInfo	m_astMatchServerInfo[MAX_MATCH_SERVER_NUM];
	UCHAR	m_bNetAdminServerCount;
	STServerBaseInfo	m_astNetAdminServerInfo[MAX_NETADMIN_SERVER_NUM];
	USHORT	m_ushReasonLen;
	CHAR	m_szReason[MAX_REASON_LEN];
};

struct STTaskStatusInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iTaskID;
	CHAR	m_cStatus;
	LONG	m_iProgressRate;
	LONG	m_iOther;
};

struct STTaskAward
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iAwardMoney;
	LONG	m_iTotalMoney;
	LONG	m_iAwardExp;
	LONG	m_iTotalExp;
	LONG	m_iAwardItemNum;
	STPlayerDBItemInfo	m_astAwardItemInfo[MAXAWARDITEMNUM];
};

struct STTaskPrevStatusInfo
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	LONG	m_iTaskID;
	UCHAR	m_bPrevStatus;
};

struct CNotifySingleGameBeginPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiNormalLoadTime;
	LONG	m_iTaskID;
	LONG	m_iMapID;
	UCHAR	m_szMapHash[MAP_HASH_CODE];
	ULONG	m_uiGameID;
	ULONG	m_uiSeed;
	SHORT	m_shTotalMapRound;
	ULONG	m_uiTimeLimit;
};

struct CNotifySingleRaceBeginPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiCountDownTime;
};

struct CNotifySingleRaceFinishPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	USHORT	m_shResultID;
};

struct CNotifySingleRaceOverPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	STTaskStatusInfo	m_stTaskStatusInfo;
	ULONG	m_uiFinTime;
	STTaskAward	m_stTaskAward;
};

struct CNotifySingleGameOverPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shResultID;
};

struct CRequestStartSingleTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	ULONG	m_uiTime;
	LONG	m_iTaskID;
	LONG	m_iMapID;
};

struct CResponseStartSingleTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shResultID;
	ULONG	m_uiUin;
	LONG	m_iTaskID;
	LONG	m_iMapID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestReportCurrentOptPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	ULONG	m_uiTime;
	LONG	m_iOptID;
};

struct CRequestGetTaskListPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	ULONG	m_uiTime;
};

struct CResponseGetTaskListPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shResultID;
	ULONG	m_uiUin;
	SHORT	m_shTaskNum;
	STPlayerDBTaskInfo	m_astPlayerDBTaskInfo[MAXTASKNUMPERPLAYER];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestTakeTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	ULONG	m_uiTime;
	LONG	m_iTaskID;
	SHORT	m_shTakeOptionID;
};

struct CResponseTakeTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shResultID;
	ULONG	m_uiUin;
	STPlayerDBTaskInfo	m_stPlayerDBTaskInfo;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestDeleteTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	ULONG	m_uiTime;
	LONG	m_iTaskID;
};

struct CResponseDeleteTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shResultID;
	ULONG	m_uiUin;
	LONG	m_iTaskID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CRequestSkipTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	ULONG	m_uiTime;
	LONG	m_iTaskID;
};

struct CResponseSkipTaskPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	SHORT	m_shResultID;
	ULONG	m_uiUin;
	LONG	m_iTaskID;
	STTaskAward	m_stTaskAward;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

struct CNotifyClientUpdateMoneyPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	LONG	m_iCurrentMoney;
};

struct CNotifyTaskAwardPara
{

	int Encode(void*,int*,BOOL bNest=FALSE);

	int Decode(void*,int,BOOL bNest=FALSE);
	ULONG	m_uiUin;
	LONG	m_iTaskID;
	STTaskAward	m_stTaskAward;
};

inline int CCSHead::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iTotalLength);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nVer);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nDialogID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iSeq);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_byBodyFlag,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_byOptLength,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_abyOption[0], m_byOptLength);
	tmpsize += m_byOptLength;
	pTmp += m_byOptLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CMsgHead::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_sMsgID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sMsgType);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iMsgSeq);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bSrcFE,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bDstFE,1);
	tmpsize += 1;
	pTmp += 1;
	{USHORT tmpshort = htons(m_sSrcID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sDstID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CTransferHead::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nTotalLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_cVer,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_iSeq);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nDialogID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nTransferCmd);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nCheckSum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_byOptLength,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_abyOption[0], m_byOptLength);
	tmpsize += m_byOptLength;
	pTmp += m_byOptLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STUdpPack::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(MsgID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STUDPPlayMove::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_cHasKeyChange,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_cArrKey[0], MAX_KEY_NUM);
	tmpsize += MAX_KEY_NUM;
	pTmp += MAX_KEY_NUM;
	{ULONG tmplong = htonl(m_unPlayAnimID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unKartAnimID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_unLinearVelocity[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_unArrPosition[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 9; i++ ) 
	{
		ULONG tmplong = htonl(m_unArrMat[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 9;
	{ULONG tmplong = htonl(m_unOffSetZ);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unAngle);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nRestDistance);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unPackageIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTimeStamp);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_cisDrifting,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STUDPPlayCollision::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unSelfUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unOtherUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_nSelfLinearVel[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_nOtherLinearVel[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_nSelfPos[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_nOtherPos[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 50; i++ ) 
	{
		ULONG tmplong = htonl(buf[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 50;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CMsgPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponsePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_sResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STRoomInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bCurrentPlayerNum,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bTotalSeatNum,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bStatus,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_iMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bFlag,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szRoomName[0], MAXROOMNAMELEN);
	tmpsize += MAXROOMNAMELEN;
	pTmp += MAXROOMNAMELEN;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPlayerDBBaseInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_szNickname[0], MAXQQKARTNICKNAMELEN);
	tmpsize += MAXQQKARTNICKNAMELEN;
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(pTmp, &m_bGender,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bCountry,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_unExperience);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iMoney);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unWinNum);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTotalNum);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unHonor);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPlayerDBAttributeInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nSpeed);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nExplosion);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nGathering);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nSpout);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nMiniSpout);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unReserve);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bStrategy,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPlayerDBOtherInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iRightFlag);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iAvailPeriod);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPlayerDBItemInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iItemID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iItemNum);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iAvailPeriod);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bStatus,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_unObtainTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iOtherAttribute);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPlayerRoomVisibleInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unIdentity);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	{USHORT tmpshort = htons(m_nPlayerID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nRoleID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nKartID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bStatus,1);
	tmpsize += 1;
	pTmp += 1;
	{int tmplen = 0;
	m_stPlayerDBBaseInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{USHORT tmpshort = htons(m_nEquipedItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nEquipedItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stPlayerItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STGameInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{int tmplen = 0;
	m_stPlayerDBBaseInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{int tmplen = 0;
	m_stPlayerDBAttributeInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{USHORT tmpshort = htons(m_nItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stPlayerDBItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{int tmplen = 0;
	m_stPlayerDBOtherInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPlayerDBTaskInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_shOptionID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_cStatus,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bPrevStatus,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_iProgressRate);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iOther);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTakeTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPrice::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iQBPrice);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iQPPrice);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iKBPrice);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STType::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_shSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STDescription::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_szGeneralDesc[0], MAX_COMMODITY_DESCLEN);
	tmpsize += MAX_COMMODITY_DESCLEN;
	pTmp += MAX_COMMODITY_DESCLEN;
	memcpy(pTmp, &m_szAttributeDesc[0], MAX_COMMODITY_DESCLEN);
	tmpsize += MAX_COMMODITY_DESCLEN;
	pTmp += MAX_COMMODITY_DESCLEN;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STRebate::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shMemeber);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_shCommon);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STLimit::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iLevelLimit);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iHonorLimit);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iOtherLimit);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STItemInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iItemID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_shAvailPeriod);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_shItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STCommodity::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szName[0], MAX_COMMODITY_NAMELEN);
	tmpsize += MAX_COMMODITY_NAMELEN;
	pTmp += MAX_COMMODITY_NAMELEN;
	{int tmplen = 0;
	m_stType.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{int tmplen = 0;
	m_stDescription.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{int tmplen = 0;
	m_stRebate.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{int tmplen = 0;
	m_stLimit.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{int tmplen = 0;
	m_stPrice.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{ULONG tmplong = htonl(m_iDateSaleLimit);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iClientVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iPriority);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iSaleTag);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_shItemCount);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_shItemCount; i++ ) 
	{
		int tmplen = 0;
		m_astItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{ULONG tmplong = htonl(m_iDateSold);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestLoginPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unClientVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szQQNickname[0], MAXNICKNAMELEN);
	tmpsize += MAXNICKNAMELEN;
	pTmp += MAXNICKNAMELEN;
	memcpy(pTmp, &m_bGender,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bIconID,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_unAttachIdentify);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiLoginType);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseLoginPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bKeyGameDataLength,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szKeyGameData[0], m_bKeyGameDataLength);
	tmpsize += m_bKeyGameDataLength;
	pTmp += m_bKeyGameDataLength;
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unIdentity);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nPlayerID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiLoginType);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nRoleID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nKartID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unGameSvrdIdentity);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stPlayerDBBaseInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{int tmplen = 0;
	m_stPlayerDBAttributeInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{USHORT tmpshort = htons(m_nItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nItemNum; i++ ) 
	{
		int tmplen = 0;
		m_astPlayerDBItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{USHORT tmpshort = htons(m_ushTaskNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_ushTaskNum; i++ ) 
	{
		int tmplen = 0;
		m_astPlayerDBTaskInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestLogoutPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseLogoutPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestRegisterPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szNickname[0], MAXQQKARTNICKNAMELEN);
	tmpsize += MAXQQKARTNICKNAMELEN;
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(pTmp, &m_bGender,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_nFaceID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nHairID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bDressIndex,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_nColorID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bCountry,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseRegisterPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szNickname[0], MAXQQKARTNICKNAMELEN);
	tmpsize += MAXQQKARTNICKNAMELEN;
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(pTmp, &m_bGender,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bCountry,1);
	tmpsize += 1;
	pTmp += 1;
	{USHORT tmpshort = htons(m_nAddItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nAddItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stAddItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{USHORT tmpshort = htons(m_ushAddTaskNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_ushAddTaskNum; i++ ) 
	{
		int tmplen = 0;
		m_astAddTaskInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestGetRoomListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nStartRoomNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nGetRoomNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseGetRoomListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nCurTotalRoomNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nStartIndex);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nRoomNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nRoomNum; i++ ) 
	{
		int tmplen = 0;
		m_stRooms[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestCreateRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szRoomName[0], MAXROOMNAMELEN);
	tmpsize += MAXROOMNAMELEN;
	pTmp += MAXROOMNAMELEN;
	memcpy(pTmp, &m_bSeatNum,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bFlag,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szPassword[0], MAXROOMPASSWORDLEN);
	tmpsize += MAXROOMPASSWORDLEN;
	pTmp += MAXROOMPASSWORDLEN;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseCreateRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szRoomName[0], MAXROOMNAMELEN);
	tmpsize += MAXROOMNAMELEN;
	pTmp += MAXROOMNAMELEN;
	memcpy(pTmp, &m_bSeatNum,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bFlag,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestEnterRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szPassword[0], MAXROOMPASSWORDLEN);
	tmpsize += MAXROOMPASSWORDLEN;
	pTmp += MAXROOMPASSWORDLEN;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseEnterRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szRoomName[0], MAXROOMNAMELEN);
	tmpsize += MAXROOMNAMELEN;
	pTmp += MAXROOMNAMELEN;
	memcpy(pTmp, &m_bSeatNum,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bFlag,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_nMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nRoomOwnerID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bSeatStatus[0], MAXSEATNUMINROOM);
	tmpsize += MAXSEATNUMINROOM;
	pTmp += MAXSEATNUMINROOM;
	memcpy(pTmp, &m_bPlayerCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_bPlayerCount; i++ ) 
	{
		int tmplen = 0;
		m_stPlayerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyEnterRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{int tmplen = 0;
	m_stNewPlayerInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestLeaveRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseLeaveRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyLeaveRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_cIsChangeRoomOwner,1);
	tmpsize += 1;
	pTmp += 1;
	{USHORT tmpshort = htons(m_nNewRoomOwnerID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestTransferUDPOKPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nDstDlg);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nInfoLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szInfo[0], m_nInfoLength);
	tmpsize += m_nInfoLength;
	pTmp += m_nInfoLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseTransferUDPOKPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyUDPOKPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nSrcDlg);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unSrcUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nInfoLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szInfo[0], m_nInfoLength);
	tmpsize += m_nInfoLength;
	pTmp += m_nInfoLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestQuickEnterRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseQuickEnterRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeBase);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sGameModeSub);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szRoomName[0], MAXROOMNAMELEN);
	tmpsize += MAXROOMNAMELEN;
	pTmp += MAXROOMNAMELEN;
	memcpy(pTmp, &m_bSeatNum,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bFlag,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_nMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nRoomOwnerID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bSeatStatus[0], MAXSEATNUMINROOM);
	tmpsize += MAXSEATNUMINROOM;
	pTmp += MAXSEATNUMINROOM;
	memcpy(pTmp, &m_bPlayerCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_bPlayerCount; i++ ) 
	{
		int tmplen = 0;
		m_stPlayerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestChangeSeatStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bSeatState,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseChangeSeatStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bSeatState,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyChangeSeatStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_bSeatID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bSeatState,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestKickFromRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseKickFromRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyKickFromRoomPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestChangeReadyStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReadyState,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseChangeReadyStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReadyState,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyChangeReadyStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReadyState,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestTalkPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bType,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_unDestPlayerUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nContentLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szContent[0], m_nContentLength);
	tmpsize += m_nContentLength;
	pTmp += m_nContentLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseTalkPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bType,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_unDestPlayerUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyTalkPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unSrcUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szSrcNickName[0], MAXNICKNAMELEN);
	tmpsize += MAXNICKNAMELEN;
	pTmp += MAXNICKNAMELEN;
	memcpy(pTmp, &m_bGender,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bType,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_unDestPlayerUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nContentLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szContent[0], m_nContentLength);
	tmpsize += m_nContentLength;
	pTmp += m_nContentLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestHelloPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bInfoLength,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_abyInfo[0], m_bInfoLength);
	tmpsize += m_bInfoLength;
	pTmp += m_bInfoLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseHelloPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyTestNetSpeedPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unStartSec);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unUSec);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CAckTestNetSpeedPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unStartSec);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unUSec);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyKickFromServerPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nReasonID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unSrcUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STFriendGroupInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_szFriendGroupName[0], MAXFRIENDGROUPNAMELEN);
	tmpsize += MAXFRIENDGROUPNAMELEN;
	pTmp += MAXFRIENDGROUPNAMELEN;
	{USHORT tmpshort = htons(m_nFriendGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nCurrentFriendNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nCurrentFriendNum; i++ ) 
	{
		ULONG tmplong = htonl(m_unFriendUin[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * m_nCurrentFriendNum;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPlayerInfoForFriend::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_szNickName[0], MAXNICKNAMELEN);
	tmpsize += MAXNICKNAMELEN;
	pTmp += MAXNICKNAMELEN;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestGetFriendListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseGetFriendListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nMaxFriendNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nTotalFriendNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nFriendGroupNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nFriendGroupNum; i++ ) 
	{
		int tmplen = 0;
		m_astFriendGroupInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestAddFriendPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nWordLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szWords[0], m_nWordLength);
	tmpsize += m_nWordLength;
	pTmp += m_nWordLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseAddFriendPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestDeleteFriendPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unFriendUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nFriendGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseDeleteFriendPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unFriendUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nFriendGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestAddFriendGroupPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bGroupNameLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szGroupName[0], m_bGroupNameLen);
	tmpsize += m_bGroupNameLen;
	pTmp += m_bGroupNameLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseAddFriendGroupPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bGroupNameLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szGroupName[0], m_bGroupNameLen);
	tmpsize += m_bGroupNameLen;
	pTmp += m_bGroupNameLen;
	{USHORT tmpshort = htons(m_nFriendGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestDeleteFriendGroupPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nFriendGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseDeleteFriendGroupPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nFriendGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestMoveFriendToSomeGroupPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unFriendUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nSrcGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nDstGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseMoveFriendToSomeGroupPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unFriendUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nSrcGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nDstGroupID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyBeAddedAsFriendPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unSrcUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stSrcPlayerInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{USHORT tmpshort = htons(m_nWordLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szWords[0], m_nWordLength);
	tmpsize += m_nWordLength;
	pTmp += m_nWordLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CAckBeAddedAsFriendPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unSrcUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nWordLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szWords[0], m_nWordLength);
	tmpsize += m_nWordLength;
	pTmp += m_nWordLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyAddFriendResultPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unSrcUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nWordLength);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szWords[0], m_nWordLength);
	tmpsize += m_nWordLength;
	pTmp += m_nWordLength;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestFindPlayerByQQPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STFindPlayerInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unIdentity);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nPlayerID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{int tmplen = 0;
	m_stPlayerDBBaseInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{USHORT tmpshort = htons(m_nEquipedItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nEquipedItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stPlayerItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STPositionInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nStatus);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iServerID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stRoomInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseFindPlayerByQQPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{int tmplen = 0;
	m_stPlayerInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	memcpy(pTmp, &m_bIsOnLine,1);
	tmpsize += 1;
	pTmp += 1;
	{int tmplen = 0;
	m_stPositionInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int NOTIFY_ANTI_BOT::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_dwUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nBufferLen);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szBuffer[0], m_nBufferLen);
	tmpsize += m_nBufferLen;
	pTmp += m_nBufferLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestStartGamePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseStartGamePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyGameBeginPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unNormalLoadTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szMapHash[0], MAP_HASH_CODE);
	tmpsize += MAP_HASH_CODE;
	pTmp += MAP_HASH_CODE;
	{ULONG tmplong = htonl(m_unGameID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unSeed);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bCurrentPlayerNum1,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < MAXSEATNUMINROOM; i++ ) 
	{
		ULONG tmplong = htonl(m_anRaceTrackOrders[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * MAXSEATNUMINROOM;
	{USHORT tmpshort = htons(m_nLap);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unPropUpdateInterval);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyGameOverPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestPrepareReadyPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyWaitUnReadyPlayersPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unWaitLoadTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyRaceBeginPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unCountDownTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestPlayerFinishRacePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unFinUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unFinResult);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyPlayerFinishRacePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyCountDownPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unCountDownTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STRaceScore::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unFinTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nTP);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nMoneyInc);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nTotalMoney);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nExpInc);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nTotalExp);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nAttrResInc);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_nTotalAttrRes);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bAwardNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_bAwardNum; i++ ) 
	{
		ULONG tmplong = htonl(m_nSpecialAward[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * m_bAwardNum;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyRaceOverPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_bCurrentPlayerNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_bCurrentPlayerNum; i++ ) 
	{
		int tmplen = 0;
		m_astRaceScore[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bWinTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_iWinType);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unReceiveHonorTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyRaceShowPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unRaceShowTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestReportCurrentInfoPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iCurrentStatus);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_iCurrenRate[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_iCurrentPosition[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	memcpy(pTmp, &m_bPassedCheckPointNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_bPassedCheckPointNum; i++ ) 
	{
		USHORT tmpshort = htons(m_nPassedCheckPointIDs[i]);
		memcpy(pTmp, &tmpshort, sizeof(short));
	pTmp+=sizeof(short);
	}
	tmpsize += sizeof(short) * m_bPassedCheckPointNum;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestReportDriftPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiSpeedOfDriftStart[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiSpeedOfDriftStop[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiPositonOfDirftStart[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiPositionOfDriftStop[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	{ULONG tmplong = htonl(m_iDriftFlag);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iAccelerateFuelInc);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseReportDriftPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_sResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iCurrentAccelerateFuel);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestGetPropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropPositionNO);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bCurrentRank,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiCurrentPosition[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	{ULONG tmplong = htonl(m_iPreviousInterval);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseGetPropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyGetPropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropPositionNO);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestUsePropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiCurrentPosition[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sNewID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseUsePropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiPropPosition[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyUsePropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ ) 
	{
		ULONG tmplong = htonl(m_aiPropPosition[i]);
		memcpy(pTmp, &tmplong, sizeof(long));
	pTmp+=sizeof(long);
	}
	tmpsize += sizeof(long) * 3;
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sNewID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestPropEffectResultPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nEffectResult);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iItemID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyPropEffectResultPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestChangeAimStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bAimState,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyChangeAimStatePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bAimState,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestChangeMapPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iNewMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseChangeMapPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_sResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iNewMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyChangeMapPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iNewMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestChangeTeamPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bNewTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseChangeTeamPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_bNewTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyPlayerChangeTeamPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bNewTeamID,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyChangeRoomOwnerPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_sRoomID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_nNewRoomOwnerID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STReplacePropInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iReplaceFlag);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iPropIndex);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STRacerReplacePropInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < MAXPROPBLANK; i++ ) 
	{
		int tmplen = 0;
		m_astReplaceProp[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyReplacePropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_sPlayerNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sPlayerNum; i++ ) 
	{
		int tmplen = 0;
		m_astRacerReplacePropInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{ULONG tmplong = htonl(m_iItemID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyAddPropPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sPropID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_iPropIdx);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iItemID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyReportDriftPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iTeamAccelerateFule);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STItemStatus::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iItemID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bNewStatus,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestChangeItemStatus::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stItemStatus[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseChangeItemStatus::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_sItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stItemStatus[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyChangeItemStatusPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stItemStatus[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestAssignAttributePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stPlayerDBAttributeInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseAssignAttributePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{int tmplen = 0;
	m_stPlayerDBAttributeInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestClearAttributePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseClearAttributePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{int tmplen = 0;
	m_stPlayerDBAttributeInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestCheckNamePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szNickname[0], MAXQQKARTNICKNAMELEN);
	tmpsize += MAXQQKARTNICKNAMELEN;
	pTmp += MAXQQKARTNICKNAMELEN;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseCheckNamePara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szNickname[0], MAXQQKARTNICKNAMELEN);
	tmpsize += MAXQQKARTNICKNAMELEN;
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestBuyCommodityPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iReqBuySeq);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iCommodityID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iPrice);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iPayType);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_nAttachInfoLen);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_abyAttachInfo[0], m_nAttachInfoLen);
	tmpsize += m_nAttachInfoLen;
	pTmp += m_nAttachInfoLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseBuyCommodityPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_nResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iReqBuySeq);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iCommodityID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iPayType);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iPrice);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiDstUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_ushReasonLen);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szReason[0], m_ushReasonLen);
	tmpsize += m_ushReasonLen;
	pTmp += m_ushReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestGetCommodityListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseGetCommodityListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_cEndFlag,1);
	tmpsize += 1;
	pTmp += 1;
	{USHORT tmpshort = htons(m_shCommodityNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_shCommodityNum; i++ ) 
	{
		int tmplen = 0;
		m_astCommodity[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{USHORT tmpshort = htons(m_ushReasonLen);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szReason[0], m_ushReasonLen);
	tmpsize += m_ushReasonLen;
	pTmp += m_ushReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyClientAddItemPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_unUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_unSrcUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_sItemNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ ) 
	{
		int tmplen = 0;
		m_stItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{USHORT tmpshort = htons(m_sAttachLen);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szAttachInfo[0], m_sAttachLen);
	tmpsize += m_sAttachLen;
	pTmp += m_sAttachLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STDownloadServerInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiIPAddr);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_ushPort);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_ushLocationID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STDirServerInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iServerID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_ushLocationID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiServerIP);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < MAX_DIR_LISTEN_PORT_NUM; i++ ) 
	{
		USHORT tmpshort = htons(m_aushServerPort[i]);
		memcpy(pTmp, &tmpshort, sizeof(short));
	pTmp+=sizeof(short);
	}
	tmpsize += sizeof(short) * MAX_DIR_LISTEN_PORT_NUM;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STWorldBaseInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_byWorldID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_byNameLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szName[0], m_byNameLen);
	tmpsize += m_byNameLen;
	pTmp += m_byNameLen;
	memcpy(pTmp, &m_byDirNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byDirNum; i++ ) 
	{
		int tmplen = 0;
		m_astDirServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STServerBaseInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iServerID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_ushLocationID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiServerIP);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_ushServerPort);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STGameServerInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iServerID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiServerIP);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_ushTcpPort);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_ushUdpPort);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_ushMaxPlayerNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{USHORT tmpshort = htons(m_ushCurrentPlayerNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STCommGameServerInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{int tmplen = 0;
	m_stServerInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{USHORT tmpshort = htons(m_ushLocationID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STIDCServerInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_ushLocationID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_byGameServerNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byGameServerNum; i++ ) 
	{
		int tmplen = 0;
		m_astGameServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STChannelInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_byChannelID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_byIDCNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byIDCNum; i++ ) 
	{
		int tmplen = 0;
		m_astIDCServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STCountryInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	memcpy(pTmp, &m_byCountryID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_byChannelNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byChannelNum; i++ ) 
	{
		int tmplen = 0;
		m_astChannelInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestGetWorldInfoPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiClientVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiMasterVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiSlaveVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_byWorldID,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szFileHash[0], HASH_CODE_LENGTH);
	tmpsize += HASH_CODE_LENGTH;
	pTmp += HASH_CODE_LENGTH;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseGetWorldInfoPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_ushResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiLastClientVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiLastMasterVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiLastSlaveVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_byDownloadServerNum,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byDownloadServerNum; i++ ) 
	{
		int tmplen = 0;
		m_astDownloadServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_byRootDirectionLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szRootDirection[0], m_byRootDirectionLen);
	tmpsize += m_byRootDirectionLen;
	pTmp += m_byRootDirectionLen;
	{ULONG tmplong = htonl(m_uiMasterFullVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiSlaveFullVersion);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_ushP2POption);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_byWorldCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byWorldCount; i++ ) 
	{
		int tmplen = 0;
		m_astWorldBaseInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_byCountryCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byCountryCount; i++ ) 
	{
		int tmplen = 0;
		m_astCountryInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_byShopServerCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byShopServerCount; i++ ) 
	{
		int tmplen = 0;
		m_astShopServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_byBorderServerCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byBorderServerCount; i++ ) 
	{
		int tmplen = 0;
		m_astBorderServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_byMatchServerCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_byMatchServerCount; i++ ) 
	{
		int tmplen = 0;
		m_astMatchServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bNetAdminServerCount,1);
	tmpsize += 1;
	pTmp += 1;
	for ( int i = 0; i < m_bNetAdminServerCount; i++ ) 
	{
		int tmplen = 0;
		m_astNetAdminServerInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	{USHORT tmpshort = htons(m_ushReasonLen);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	memcpy(pTmp, &m_szReason[0], m_ushReasonLen);
	tmpsize += m_ushReasonLen;
	pTmp += m_ushReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STTaskStatusInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_cStatus,1);
	tmpsize += 1;
	pTmp += 1;
	{ULONG tmplong = htonl(m_iProgressRate);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iOther);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STTaskAward::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iAwardMoney);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTotalMoney);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iAwardExp);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTotalExp);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iAwardItemNum);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	for ( int i = 0; i < m_iAwardItemNum; i++ ) 
	{
		int tmplen = 0;
		m_astAwardItemInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int STTaskPrevStatusInfo::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bPrevStatus,1);
	tmpsize += 1;
	pTmp += 1;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifySingleGameBeginPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiNormalLoadTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_szMapHash[0], MAP_HASH_CODE);
	tmpsize += MAP_HASH_CODE;
	pTmp += MAP_HASH_CODE;
	{ULONG tmplong = htonl(m_uiGameID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiSeed);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_shTotalMapRound);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiTimeLimit);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifySingleRaceBeginPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiCountDownTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifySingleRaceFinishPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifySingleRaceOverPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stTaskStatusInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	{ULONG tmplong = htonl(m_uiFinTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stTaskAward.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifySingleGameOverPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestStartSingleTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseStartSingleTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iMapID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestReportCurrentOptPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iOptID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestGetTaskListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseGetTaskListPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_shTaskNum);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_shTaskNum; i++ ) 
	{
		int tmplen = 0;
		m_astPlayerDBTaskInfo[i].Encode(pTmp, &tmplen, TRUE);
		tmpsize += tmplen;
	pTmp+=tmplen;
	}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestTakeTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{USHORT tmpshort = htons(m_shTakeOptionID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseTakeTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stPlayerDBTaskInfo.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestDeleteTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseDeleteTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CRequestSkipTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_uiTime);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CResponseSkipTaskPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{USHORT tmpshort = htons(m_shResultID);
	memcpy(pTmp, &tmpshort, sizeof(short));}
	tmpsize += sizeof(short);
	pTmp += sizeof(short);
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stTaskAward.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	memcpy(pTmp, &m_bReasonLen,1);
	tmpsize += 1;
	pTmp += 1;
	memcpy(pTmp, &m_szReason[0], m_bReasonLen);
	tmpsize += m_bReasonLen;
	pTmp += m_bReasonLen;
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyClientUpdateMoneyPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iCurrentMoney);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CNotifyTaskAwardPara::Encode(void* pBuf, int* pSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf + (bNest ? 0 : sizeof(short));
	int tmpsize=(bNest ? 0 : sizeof(short));
	{ULONG tmplong = htonl(m_uiUin);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{ULONG tmplong = htonl(m_iTaskID);
	memcpy(pTmp, &tmplong, sizeof(long));}
	tmpsize += sizeof(long);
	pTmp += sizeof(long);
	{int tmplen = 0;
	m_stTaskAward.Encode(pTmp, &tmplen, TRUE);
	tmpsize += tmplen;
	pTmp+=tmplen;}
	*pSize = tmpsize;
	if(!bNest)
	{
		pTmp = (char*)pBuf;
	tmpsize=htons(tmpsize);
	memcpy(pTmp, &tmpsize, sizeof(short));
	}
	return tmpsize;
}

inline int CCSHead::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iTotalLength = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nVer = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nDialogID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iSeq = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_byBodyFlag, pTmp, 1);
	pTmp+=1;
	memcpy(&m_byOptLength, pTmp, 1);
	pTmp+=1;
	memset(&m_abyOption[0], 0, 0);
	memcpy(&m_abyOption[0], pTmp, m_byOptLength);
	pTmp += m_byOptLength;
	return pTmp-(char*)pBuf;
}

inline int CMsgHead::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_sMsgID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sMsgType = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iMsgSeq = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bSrcFE, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bDstFE, pTmp, 1);
	pTmp+=1;
	m_sSrcID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sDstID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CTransferHead::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nTotalLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_cVer, pTmp, 1);
	pTmp+=1;
	m_iSeq = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nDialogID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nTransferCmd = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nCheckSum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_byOptLength, pTmp, 1);
	pTmp+=1;
	memset(&m_abyOption[0], 0, MAXOPTLENGTH);
	memcpy(&m_abyOption[0], pTmp, m_byOptLength);
	pTmp += m_byOptLength;
	return pTmp-(char*)pBuf;
}

inline int STUdpPack::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	MsgID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STUDPPlayMove::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_cHasKeyChange, pTmp, 1);
	pTmp+=1;
	memset(&m_cArrKey[0], 0, MAX_KEY_NUM);
	memcpy(&m_cArrKey[0], pTmp, MAX_KEY_NUM);
	pTmp += MAX_KEY_NUM;
	m_unPlayAnimID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unKartAnimID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ )
	{
		m_unLinearVelocity[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_unArrPosition[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 9; i++ )
	{
		m_unArrMat[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_unOffSetZ = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unAngle = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nRestDistance = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unPackageIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTimeStamp = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_cisDrifting, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int STUDPPlayCollision::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unSelfUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unOtherUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ )
	{
		m_nSelfLinearVel[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_nOtherLinearVel[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_nSelfPos[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_nOtherPos[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 50; i++ )
	{
		buf[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	return pTmp-(char*)pBuf;
}

inline int CMsgPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	return pTmp-(char*)pBuf;
}

inline int CRequestPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponsePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_sResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STRoomInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bCurrentPlayerNum, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bTotalSeatNum, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bStatus, pTmp, 1);
	pTmp+=1;
	m_iMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bFlag, pTmp, 1);
	pTmp+=1;
	memset(&m_szRoomName[0], 0, MAXROOMNAMELEN);
	memcpy(&m_szRoomName[0], pTmp, MAXROOMNAMELEN);
	pTmp += MAXROOMNAMELEN;
	return pTmp-(char*)pBuf;
}

inline int STPlayerDBBaseInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memset(&m_szNickname[0], 0, MAXQQKARTNICKNAMELEN);
	memcpy(&m_szNickname[0], pTmp, MAXQQKARTNICKNAMELEN);
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(&m_bGender, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bCountry, pTmp, 1);
	pTmp+=1;
	m_unExperience = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iMoney = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unWinNum = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTotalNum = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unHonor = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STPlayerDBAttributeInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nSpeed = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nExplosion = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nGathering = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nSpout = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nMiniSpout = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unReserve = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bStrategy, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int STPlayerDBOtherInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iRightFlag = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iAvailPeriod = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STPlayerDBItemInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iItemID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iItemNum = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iAvailPeriod = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bStatus, pTmp, 1);
	pTmp+=1;
	m_unObtainTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iOtherAttribute = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STPlayerRoomVisibleInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unIdentity = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bTeamID, pTmp, 1);
	pTmp+=1;
	m_nPlayerID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nRoleID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nKartID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bStatus, pTmp, 1);
	pTmp+=1;
	{int stusize = m_stPlayerDBBaseInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_nEquipedItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nEquipedItemNum; i++ )
	{
		int stusize = m_stPlayerItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int STGameInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	{int stusize = m_stPlayerDBBaseInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	{int stusize = m_stPlayerDBAttributeInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_nItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nItemNum; i++ )
	{
		int stusize = m_stPlayerDBItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	{int stusize = m_stPlayerDBOtherInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	return pTmp-(char*)pBuf;
}

inline int STPlayerDBTaskInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_shOptionID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_cStatus, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bPrevStatus, pTmp, 1);
	pTmp+=1;
	m_iProgressRate = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iOther = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTakeTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STPrice::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iQBPrice = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iQPPrice = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iKBPrice = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STType::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_shSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STDescription::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memset(&m_szGeneralDesc[0], 0, MAX_COMMODITY_DESCLEN);
	memcpy(&m_szGeneralDesc[0], pTmp, MAX_COMMODITY_DESCLEN);
	pTmp += MAX_COMMODITY_DESCLEN;
	memset(&m_szAttributeDesc[0], 0, MAX_COMMODITY_DESCLEN);
	memcpy(&m_szAttributeDesc[0], pTmp, MAX_COMMODITY_DESCLEN);
	pTmp += MAX_COMMODITY_DESCLEN;
	return pTmp-(char*)pBuf;
}

inline int STRebate::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shMemeber = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_shCommon = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STLimit::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iLevelLimit = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iHonorLimit = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iOtherLimit = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STItemInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iItemID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_shAvailPeriod = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_shItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STCommodity::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szName[0], 0, MAX_COMMODITY_NAMELEN);
	memcpy(&m_szName[0], pTmp, MAX_COMMODITY_NAMELEN);
	pTmp += MAX_COMMODITY_NAMELEN;
	{int stusize = m_stType.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	{int stusize = m_stDescription.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	{int stusize = m_stRebate.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	{int stusize = m_stLimit.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	{int stusize = m_stPrice.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_iDateSaleLimit = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iClientVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iPriority = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iSaleTag = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_shItemCount = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_shItemCount; i++ )
	{
		int stusize = m_astItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	m_iDateSold = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CRequestLoginPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unClientVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szQQNickname[0], 0, MAXNICKNAMELEN);
	memcpy(&m_szQQNickname[0], pTmp, MAXNICKNAMELEN);
	pTmp += MAXNICKNAMELEN;
	memcpy(&m_bGender, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bIconID, pTmp, 1);
	pTmp+=1;
	m_unAttachIdentify = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiLoginType = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseLoginPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bKeyGameDataLength, pTmp, 1);
	pTmp+=1;
	memset(&m_szKeyGameData[0], 0, MAXCSGAMEKEYLEN);
	memcpy(&m_szKeyGameData[0], pTmp, m_bKeyGameDataLength);
	pTmp += m_bKeyGameDataLength;
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unIdentity = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nPlayerID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiLoginType = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nRoleID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nKartID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unGameSvrdIdentity = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stPlayerDBBaseInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	{int stusize = m_stPlayerDBAttributeInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_nItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nItemNum; i++ )
	{
		int stusize = m_astPlayerDBItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	m_ushTaskNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_ushTaskNum; i++ )
	{
		int stusize = m_astPlayerDBTaskInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestLogoutPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseLogoutPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestRegisterPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szNickname[0], 0, MAXQQKARTNICKNAMELEN);
	memcpy(&m_szNickname[0], pTmp, MAXQQKARTNICKNAMELEN);
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(&m_bGender, pTmp, 1);
	pTmp+=1;
	m_nFaceID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nHairID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bDressIndex, pTmp, 1);
	pTmp+=1;
	m_nColorID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bCountry, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CResponseRegisterPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szNickname[0], 0, MAXQQKARTNICKNAMELEN);
	memcpy(&m_szNickname[0], pTmp, MAXQQKARTNICKNAMELEN);
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(&m_bGender, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bCountry, pTmp, 1);
	pTmp+=1;
	m_nAddItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nAddItemNum; i++ )
	{
		int stusize = m_stAddItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	m_ushAddTaskNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_ushAddTaskNum; i++ )
	{
		int stusize = m_astAddTaskInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestGetRoomListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nStartRoomNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nGetRoomNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CResponseGetRoomListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nCurTotalRoomNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nStartIndex = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nRoomNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nRoomNum; i++ )
	{
		int stusize = m_stRooms[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CRequestCreateRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szRoomName[0], 0, MAXROOMNAMELEN);
	memcpy(&m_szRoomName[0], pTmp, MAXROOMNAMELEN);
	pTmp += MAXROOMNAMELEN;
	memcpy(&m_bSeatNum, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bFlag, pTmp, 1);
	pTmp+=1;
	memset(&m_szPassword[0], 0, MAXROOMPASSWORDLEN);
	memcpy(&m_szPassword[0], pTmp, MAXROOMPASSWORDLEN);
	pTmp += MAXROOMPASSWORDLEN;
	return pTmp-(char*)pBuf;
}

inline int CResponseCreateRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szRoomName[0], 0, MAXROOMNAMELEN);
	memcpy(&m_szRoomName[0], pTmp, MAXROOMNAMELEN);
	pTmp += MAXROOMNAMELEN;
	memcpy(&m_bSeatNum, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bFlag, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bTeamID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestEnterRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szPassword[0], 0, MAXROOMPASSWORDLEN);
	memcpy(&m_szPassword[0], pTmp, MAXROOMPASSWORDLEN);
	pTmp += MAXROOMPASSWORDLEN;
	return pTmp-(char*)pBuf;
}

inline int CResponseEnterRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szRoomName[0], 0, MAXROOMNAMELEN);
	memcpy(&m_szRoomName[0], pTmp, MAXROOMNAMELEN);
	pTmp += MAXROOMNAMELEN;
	memcpy(&m_bSeatNum, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bFlag, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bTeamID, pTmp, 1);
	pTmp+=1;
	m_nMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nRoomOwnerID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_bSeatStatus[0], 0, MAXSEATNUMINROOM);
	memcpy(&m_bSeatStatus[0], pTmp, MAXSEATNUMINROOM);
	pTmp += MAXSEATNUMINROOM;
	memcpy(&m_bPlayerCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_bPlayerCount; i++ )
	{
		int stusize = m_stPlayerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyEnterRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	{int stusize = m_stNewPlayerInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	return pTmp-(char*)pBuf;
}

inline int CRequestLeaveRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseLeaveRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyLeaveRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_cIsChangeRoomOwner, pTmp, 1);
	pTmp+=1;
	m_nNewRoomOwnerID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CRequestTransferUDPOKPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nDstDlg = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nInfoLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szInfo[0], 0, 32);
	memcpy(&m_szInfo[0], pTmp, m_nInfoLength);
	pTmp += m_nInfoLength;
	return pTmp-(char*)pBuf;
}

inline int CResponseTransferUDPOKPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CNotifyUDPOKPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nSrcDlg = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unSrcUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nInfoLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szInfo[0], 0, 32);
	memcpy(&m_szInfo[0], pTmp, m_nInfoLength);
	pTmp += m_nInfoLength;
	return pTmp-(char*)pBuf;
}

inline int CRequestQuickEnterRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CResponseQuickEnterRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeBase = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sGameModeSub = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szRoomName[0], 0, MAXROOMNAMELEN);
	memcpy(&m_szRoomName[0], pTmp, MAXROOMNAMELEN);
	pTmp += MAXROOMNAMELEN;
	memcpy(&m_bSeatNum, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bFlag, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bTeamID, pTmp, 1);
	pTmp+=1;
	m_nMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nRoomOwnerID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_bSeatStatus[0], 0, MAXSEATNUMINROOM);
	memcpy(&m_bSeatStatus[0], pTmp, MAXSEATNUMINROOM);
	pTmp += MAXSEATNUMINROOM;
	memcpy(&m_bPlayerCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_bPlayerCount; i++ )
	{
		int stusize = m_stPlayerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestChangeSeatStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bSeatState, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CResponseChangeSeatStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bSeatState, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyChangeSeatStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memcpy(&m_bSeatID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bSeatState, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CRequestKickFromRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseKickFromRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyKickFromRoomPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CRequestChangeReadyStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReadyState, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CResponseChangeReadyStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReadyState, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyChangeReadyStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReadyState, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CRequestTalkPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bType, pTmp, 1);
	pTmp+=1;
	m_unDestPlayerUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nContentLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szContent[0], 0, MAXCHATCONTENTLEN);
	memcpy(&m_szContent[0], pTmp, m_nContentLength);
	pTmp += m_nContentLength;
	return pTmp-(char*)pBuf;
}

inline int CResponseTalkPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bType, pTmp, 1);
	pTmp+=1;
	m_unDestPlayerUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyTalkPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unSrcUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szSrcNickName[0], 0, MAXNICKNAMELEN);
	memcpy(&m_szSrcNickName[0], pTmp, MAXNICKNAMELEN);
	pTmp += MAXNICKNAMELEN;
	memcpy(&m_bGender, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bType, pTmp, 1);
	pTmp+=1;
	m_unDestPlayerUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nContentLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szContent[0], 0, MAXCHATCONTENTLEN);
	memcpy(&m_szContent[0], pTmp, m_nContentLength);
	pTmp += m_nContentLength;
	return pTmp-(char*)pBuf;
}

inline int CRequestHelloPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bInfoLength, pTmp, 1);
	pTmp+=1;
	memset(&m_abyInfo[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_abyInfo[0], pTmp, m_bInfoLength);
	pTmp += m_bInfoLength;
	return pTmp-(char*)pBuf;
}

inline int CResponseHelloPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyTestNetSpeedPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unStartSec = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unUSec = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CAckTestNetSpeedPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unStartSec = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unUSec = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyKickFromServerPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nReasonID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unSrcUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int STFriendGroupInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memset(&m_szFriendGroupName[0], 0, MAXFRIENDGROUPNAMELEN);
	memcpy(&m_szFriendGroupName[0], pTmp, MAXFRIENDGROUPNAMELEN);
	pTmp += MAXFRIENDGROUPNAMELEN;
	m_nFriendGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nCurrentFriendNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nCurrentFriendNum; i++ )
	{
		m_unFriendUin[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	return pTmp-(char*)pBuf;
}

inline int STPlayerInfoForFriend::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memset(&m_szNickName[0], 0, MAXNICKNAMELEN);
	memcpy(&m_szNickName[0], pTmp, MAXNICKNAMELEN);
	pTmp += MAXNICKNAMELEN;
	return pTmp-(char*)pBuf;
}

inline int CRequestGetFriendListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseGetFriendListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nMaxFriendNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nTotalFriendNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nFriendGroupNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nFriendGroupNum; i++ )
	{
		int stusize = m_astFriendGroupInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestAddFriendPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nWordLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szWords[0], 0, MAXWORDSLEN);
	memcpy(&m_szWords[0], pTmp, m_nWordLength);
	pTmp += m_nWordLength;
	return pTmp-(char*)pBuf;
}

inline int CResponseAddFriendPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestDeleteFriendPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unFriendUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nFriendGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CResponseDeleteFriendPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unFriendUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nFriendGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestAddFriendGroupPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bGroupNameLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szGroupName[0], 0, MAXFRIENDGROUPNAMELEN);
	memcpy(&m_szGroupName[0], pTmp, m_bGroupNameLen);
	pTmp += m_bGroupNameLen;
	return pTmp-(char*)pBuf;
}

inline int CResponseAddFriendGroupPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bGroupNameLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szGroupName[0], 0, MAXFRIENDGROUPNAMELEN);
	memcpy(&m_szGroupName[0], pTmp, m_bGroupNameLen);
	pTmp += m_bGroupNameLen;
	m_nFriendGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestDeleteFriendGroupPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nFriendGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CResponseDeleteFriendGroupPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nFriendGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestMoveFriendToSomeGroupPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unFriendUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nSrcGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nDstGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CResponseMoveFriendToSomeGroupPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unFriendUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nSrcGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nDstGroupID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyBeAddedAsFriendPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unSrcUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stSrcPlayerInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_nWordLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szWords[0], 0, MAXWORDSLEN);
	memcpy(&m_szWords[0], pTmp, m_nWordLength);
	pTmp += m_nWordLength;
	return pTmp-(char*)pBuf;
}

inline int CAckBeAddedAsFriendPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unSrcUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nWordLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szWords[0], 0, MAXWORDSLEN);
	memcpy(&m_szWords[0], pTmp, m_nWordLength);
	pTmp += m_nWordLength;
	return pTmp-(char*)pBuf;
}

inline int CNotifyAddFriendResultPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unSrcUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nWordLength = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szWords[0], 0, MAXWORDSLEN);
	memcpy(&m_szWords[0], pTmp, m_nWordLength);
	pTmp += m_nWordLength;
	return pTmp-(char*)pBuf;
}

inline int CRequestFindPlayerByQQPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STFindPlayerInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unIdentity = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nPlayerID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	{int stusize = m_stPlayerDBBaseInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_nEquipedItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_nEquipedItemNum; i++ )
	{
		int stusize = m_stPlayerItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int STPositionInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nStatus = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iServerID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stRoomInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	return pTmp-(char*)pBuf;
}

inline int CResponseFindPlayerByQQPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	{int stusize = m_stPlayerInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	memcpy(&m_bIsOnLine, pTmp, 1);
	pTmp+=1;
	{int stusize = m_stPositionInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int NOTIFY_ANTI_BOT::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_dwUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nBufferLen = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szBuffer[0], 0, 32000);
	memcpy(&m_szBuffer[0], pTmp, m_nBufferLen);
	pTmp += m_nBufferLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestStartGamePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseStartGamePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyGameBeginPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unNormalLoadTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szMapHash[0], 0, MAP_HASH_CODE);
	memcpy(&m_szMapHash[0], pTmp, MAP_HASH_CODE);
	pTmp += MAP_HASH_CODE;
	m_unGameID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unSeed = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bCurrentPlayerNum1, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
	{
		m_anRaceTrackOrders[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_nLap = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unPropUpdateInterval = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyGameOverPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CRequestPrepareReadyPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyWaitUnReadyPlayersPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unWaitLoadTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyRaceBeginPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unCountDownTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CRequestPlayerFinishRacePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unFinUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unFinResult = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyPlayerFinishRacePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyCountDownPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unCountDownTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STRaceScore::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unFinTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nTP = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nMoneyInc = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nTotalMoney = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nExpInc = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nTotalExp = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nAttrResInc = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nTotalAttrRes = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bAwardNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_bAwardNum; i++ )
	{
		m_nSpecialAward[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	return pTmp-(char*)pBuf;
}

inline int CNotifyRaceOverPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memcpy(&m_bCurrentPlayerNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_bCurrentPlayerNum; i++ )
	{
		int stusize = m_astRaceScore[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bWinTeamID, pTmp, 1);
	pTmp+=1;
	m_iWinType = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unReceiveHonorTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyRaceShowPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unRaceShowTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CRequestReportCurrentInfoPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iCurrentStatus = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ )
	{
		m_iCurrenRate[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_iCurrentPosition[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	memcpy(&m_bPassedCheckPointNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_bPassedCheckPointNum; i++ )
	{
		m_nPassedCheckPointIDs[i] = ntohs(*(USHORT*)pTmp);
		pTmp += sizeof(short);
	}
	return pTmp-(char*)pBuf;
}

inline int CRequestReportDriftPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ )
	{
		m_aiSpeedOfDriftStart[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_aiSpeedOfDriftStop[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_aiPositonOfDirftStart[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	for ( int i = 0; i < 3; i++ )
	{
		m_aiPositionOfDriftStop[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_iDriftFlag = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iAccelerateFuelInc = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseReportDriftPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_sResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iCurrentAccelerateFuel = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestGetPropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropPositionNO = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bCurrentRank, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < 3; i++ )
	{
		m_aiCurrentPosition[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_iPreviousInterval = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseGetPropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyGetPropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropPositionNO = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CRequestUsePropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ )
	{
		m_aiCurrentPosition[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sNewID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CResponseUsePropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ )
	{
		m_aiPropPosition[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyUsePropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < 3; i++ )
	{
		m_aiPropPosition[i] = ntohl(*(ULONG*)pTmp);
		pTmp += sizeof(long);
	}
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sNewID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CRequestPropEffectResultPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nEffectResult = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iItemID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyPropEffectResultPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CRequestChangeAimStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bAimState, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CNotifyChangeAimStatePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bAimState, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CRequestChangeMapPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iNewMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseChangeMapPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_sResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iNewMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyChangeMapPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iNewMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CRequestChangeTeamPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bNewTeamID, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CResponseChangeTeamPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_bNewTeamID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyPlayerChangeTeamPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bNewTeamID, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CNotifyChangeRoomOwnerPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_sRoomID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_nNewRoomOwnerID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STReplacePropInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iReplaceFlag = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iPropIndex = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STRacerReplacePropInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < MAXPROPBLANK; i++ )
	{
		int stusize = m_astReplaceProp[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int CNotifyReplacePropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_sPlayerNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sPlayerNum; i++ )
	{
		int stusize = m_astRacerReplacePropInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	m_iItemID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyAddPropPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sPropID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_iPropIdx = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iItemID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyReportDriftPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iTeamAccelerateFule = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STItemStatus::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iItemID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bNewStatus, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CRequestChangeItemStatus::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ )
	{
		int stusize = m_stItemStatus[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int CResponseChangeItemStatus::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_sItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ )
	{
		int stusize = m_stItemStatus[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyChangeItemStatusPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ )
	{
		int stusize = m_stItemStatus[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int CRequestAssignAttributePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stPlayerDBAttributeInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	return pTmp-(char*)pBuf;
}

inline int CResponseAssignAttributePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	{int stusize = m_stPlayerDBAttributeInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestClearAttributePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseClearAttributePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	{int stusize = m_stPlayerDBAttributeInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestCheckNamePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szNickname[0], 0, MAXQQKARTNICKNAMELEN);
	memcpy(&m_szNickname[0], pTmp, MAXQQKARTNICKNAMELEN);
	pTmp += MAXQQKARTNICKNAMELEN;
	return pTmp-(char*)pBuf;
}

inline int CResponseCheckNamePara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szNickname[0], 0, MAXQQKARTNICKNAMELEN);
	memcpy(&m_szNickname[0], pTmp, MAXQQKARTNICKNAMELEN);
	pTmp += MAXQQKARTNICKNAMELEN;
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestBuyCommodityPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iReqBuySeq = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iCommodityID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iPrice = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iPayType = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_nAttachInfoLen = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_abyAttachInfo[0], 0, MAXATTACHINFOLEN);
	memcpy(&m_abyAttachInfo[0], pTmp, m_nAttachInfoLen);
	pTmp += m_nAttachInfoLen;
	return pTmp-(char*)pBuf;
}

inline int CResponseBuyCommodityPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_nResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iReqBuySeq = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iCommodityID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iPayType = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iPrice = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiDstUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_ushReasonLen = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szReason[0], 0, MAX_REASON_LEN);
	memcpy(&m_szReason[0], pTmp, m_ushReasonLen);
	pTmp += m_ushReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestGetCommodityListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseGetCommodityListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_cEndFlag, pTmp, 1);
	pTmp+=1;
	m_shCommodityNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_shCommodityNum; i++ )
	{
		int stusize = m_astCommodity[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	m_ushReasonLen = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szReason[0], 0, MAX_REASON_LEN);
	memcpy(&m_szReason[0], pTmp, m_ushReasonLen);
	pTmp += m_ushReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyClientAddItemPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_unUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_unSrcUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_sItemNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_sItemNum; i++ )
	{
		int stusize = m_stItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	m_sAttachLen = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szAttachInfo[0], 0, MAXATTACHINFOLEN);
	memcpy(&m_szAttachInfo[0], pTmp, m_sAttachLen);
	pTmp += m_sAttachLen;
	return pTmp-(char*)pBuf;
}

inline int STDownloadServerInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiIPAddr = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_ushPort = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_ushLocationID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STDirServerInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iServerID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_ushLocationID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiServerIP = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < MAX_DIR_LISTEN_PORT_NUM; i++ )
	{
		m_aushServerPort[i] = ntohs(*(USHORT*)pTmp);
		pTmp += sizeof(short);
	}
	return pTmp-(char*)pBuf;
}

inline int STWorldBaseInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memcpy(&m_byWorldID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_byNameLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szName[0], 0, MAX_WORLD_NAME_LEN);
	memcpy(&m_szName[0], pTmp, m_byNameLen);
	pTmp += m_byNameLen;
	memcpy(&m_byDirNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byDirNum; i++ )
	{
		int stusize = m_astDirServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int STServerBaseInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iServerID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_ushLocationID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiServerIP = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_ushServerPort = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STGameServerInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iServerID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiServerIP = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_ushTcpPort = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_ushUdpPort = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_ushMaxPlayerNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_ushCurrentPlayerNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STCommGameServerInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	{int stusize = m_stServerInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_ushLocationID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int STIDCServerInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_ushLocationID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_byGameServerNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byGameServerNum; i++ )
	{
		int stusize = m_astGameServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int STChannelInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memcpy(&m_byChannelID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_byIDCNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byIDCNum; i++ )
	{
		int stusize = m_astIDCServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int STCountryInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	memcpy(&m_byCountryID, pTmp, 1);
	pTmp+=1;
	memcpy(&m_byChannelNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byChannelNum; i++ )
	{
		int stusize = m_astChannelInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int CRequestGetWorldInfoPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiClientVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiMasterVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiSlaveVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_byWorldID, pTmp, 1);
	pTmp+=1;
	memset(&m_szFileHash[0], 0, HASH_CODE_LENGTH);
	memcpy(&m_szFileHash[0], pTmp, HASH_CODE_LENGTH);
	pTmp += HASH_CODE_LENGTH;
	return pTmp-(char*)pBuf;
}

inline int CResponseGetWorldInfoPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_ushResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiLastClientVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiLastMasterVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiLastSlaveVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_byDownloadServerNum, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byDownloadServerNum; i++ )
	{
		int stusize = m_astDownloadServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_byRootDirectionLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szRootDirection[0], 0, MAX_DIRECTION_LENGTH);
	memcpy(&m_szRootDirection[0], pTmp, m_byRootDirectionLen);
	pTmp += m_byRootDirectionLen;
	m_uiMasterFullVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiSlaveFullVersion = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_ushP2POption = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memcpy(&m_byWorldCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byWorldCount; i++ )
	{
		int stusize = m_astWorldBaseInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_byCountryCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byCountryCount; i++ )
	{
		int stusize = m_astCountryInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_byShopServerCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byShopServerCount; i++ )
	{
		int stusize = m_astShopServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_byBorderServerCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byBorderServerCount; i++ )
	{
		int stusize = m_astBorderServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_byMatchServerCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_byMatchServerCount; i++ )
	{
		int stusize = m_astMatchServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bNetAdminServerCount, pTmp, 1);
	pTmp+=1;
	for ( int i = 0; i < m_bNetAdminServerCount; i++ )
	{
		int stusize = m_astNetAdminServerInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	m_ushReasonLen = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	memset(&m_szReason[0], 0, MAX_REASON_LEN);
	memcpy(&m_szReason[0], pTmp, m_ushReasonLen);
	pTmp += m_ushReasonLen;
	return pTmp-(char*)pBuf;
}

inline int STTaskStatusInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_cStatus, pTmp, 1);
	pTmp+=1;
	m_iProgressRate = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iOther = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int STTaskAward::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iAwardMoney = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTotalMoney = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iAwardExp = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTotalExp = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iAwardItemNum = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	for ( int i = 0; i < m_iAwardItemNum; i++ )
	{
		int stusize = m_astAwardItemInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	return pTmp-(char*)pBuf;
}

inline int STTaskPrevStatusInfo::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bPrevStatus, pTmp, 1);
	pTmp+=1;
	return pTmp-(char*)pBuf;
}

inline int CNotifySingleGameBeginPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiNormalLoadTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memset(&m_szMapHash[0], 0, MAP_HASH_CODE);
	memcpy(&m_szMapHash[0], pTmp, MAP_HASH_CODE);
	pTmp += MAP_HASH_CODE;
	m_uiGameID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiSeed = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_shTotalMapRound = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiTimeLimit = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifySingleRaceBeginPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiCountDownTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifySingleRaceFinishPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CNotifySingleRaceOverPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stTaskStatusInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	m_uiFinTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stTaskAward.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	return pTmp-(char*)pBuf;
}

inline int CNotifySingleGameOverPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CRequestStartSingleTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseStartSingleTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iMapID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestReportCurrentOptPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iOptID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CRequestGetTaskListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseGetTaskListPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_shTaskNum = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	for ( int i = 0; i < m_shTaskNum; i++ )
	{
		int stusize = m_astPlayerDBTaskInfo[i].Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
		pTmp += stusize;
	}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestTakeTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_shTakeOptionID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	return pTmp-(char*)pBuf;
}

inline int CResponseTakeTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stPlayerDBTaskInfo.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestDeleteTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseDeleteTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CRequestSkipTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_uiTime = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CResponseSkipTaskPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_shResultID = ntohs(*(USHORT*)pTmp);
	pTmp += sizeof(short);
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stTaskAward.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	memcpy(&m_bReasonLen, pTmp, 1);
	pTmp+=1;
	memset(&m_szReason[0], 0, MAXATTACHCONTENTLEN);
	memcpy(&m_szReason[0], pTmp, m_bReasonLen);
	pTmp += m_bReasonLen;
	return pTmp-(char*)pBuf;
}

inline int CNotifyClientUpdateMoneyPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iCurrentMoney = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	return pTmp-(char*)pBuf;
}

inline int CNotifyTaskAwardPara::Decode(void* pBuf, int nSize, BOOL bNest)
{
	char* pTmp=(char*)pBuf;
	int tmpsize=0;
	short stusize=0;
	if(!bNest)
	{stusize=ntohs(*(short*)pTmp);
	if ( stusize > nSize )
		return -1;
	pTmp+=sizeof(short);
	}
	m_uiUin = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	m_iTaskID = ntohl(*(ULONG*)pTmp);
	pTmp += sizeof(long);
	{int stusize = m_stTaskAward.Decode(pTmp, nSize + pTmp - (char*)pBuf, TRUE);
	pTmp += stusize;}
	return pTmp-(char*)pBuf;
}

#endif
