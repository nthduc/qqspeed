#include "StdAfx.h"
#include "assert.h"
#include "encoder.h"
#include "network/msg.h"
#include "common.h"
//#include "Encoder_i.c"
//#include "comhelp.h"
/*
CEncoder::CEncoder(void)
{
	CreateObjectFromFile("Encoder.dll", CLSID_CEncoder, IID_IEncoder, (void**)&m_pEncoder);
	if ( !m_pEncoder )
	{
		LogText("Create Encode Interface Error!\n");
		return;
	}
	if ( S_OK != m_pEncoder->LoadMetaLib("QQTMsgData.bin") )
	{
		LogText("LoadMetaLib Failed!\n");
		return;
	}
}

CEncoder::~CEncoder(void)
{
	if ( m_pEncoder )
	{
//		m_pEncoder->Release(); //Ææ¹Ö£¬ÕâÀï±¨´í
		m_pEncoder = NULL;
		LogText("Encode Interface Released!\n");
	}
}

CEncoder g_Encoder;
IEncoder* GetDefaultEncoder()
{
	return g_Encoder.GetEncoder();
}
*/
//////////////////////////////////////////////////////////////////////////
//Encode Macros
#define BEGIN_ENCODE_DATA(DataID)	\
	switch(DataID)	\
	{	\
	case 0:	\
	return FALSE
#define ADD_ENCODE_DATA(DataID, Data, Type, Buffer, OutLen)	\
	case DataID:	\
	{	\
		LogText("----%s-----\n", #Type);	\
		Type* pData = (Type*)Data;	\
		pData->Encode((LPSTR)Buffer, (int*)OutLen);	\
	}	\
	break
#define END_ENCODE_DATA	\
	default:	\
		assert(0);	\
		return FALSE;	\
	}	\
	return TRUE

//////////////////////////////////////////////////////////////////////////
//Decode Macros
#define BEGIN_DECODE_DATA(DataID)	\
	switch(DataID)	\
	{	\
	case 0:	\
		return FALSE
#define ADD_DECODE_DATA(DataID, Data, DataLen, Type, Buffer, OutLen)	\
	case DataID:	\
	{	\
		if ( sizeof(Type) > 1024*1024 )	\
		return FALSE;	\
		LogText("----%s-----\n", #Type);	\
		Type* pData = new Type;	\
		UINT nSize = sizeof(Type); \
		memset(pData, 0, sizeof(Type));	\
		pData->Decode((void*)Data, DataLen);	\
		*OutLen = nSize;	\
		memcpy(Buffer, pData, nSize);	\
		delete pData;	\
	}	\
	break
#define END_DECODE_DATA	\
	default:	\
		assert(0);\
		return FALSE;	\
	}	\
	return TRUE

BOOL EncodeData(UINT DataID, const BYTE* Data, BYTE* Buffer, UINT* OutLen)
{
	BEGIN_ENCODE_DATA(DataID);
	ADD_ENCODE_DATA(ID_CMD_REQGETHALLINFO,			Data, CRequestGetWorldInfoPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTLOGIN,			Data, CRequestLoginPara,			Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTREGISTER,			Data, CRequestRegisterPara,			Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTLOGOUT,			Data, CRequestLogoutPara,			Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTGETROOMLIST,		Data, CRequestGetRoomListPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCREATEROOM,		Data, CRequestCreateRoomPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTENTERROOM,		Data, CRequestEnterRoomPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTLEAVEROOM,		Data, CRequestLeaveRoomPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTQUICKENTERROOM,	Data, CRequestQuickEnterRoomPara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCHANGESEATSTATE,	Data, CRequestChangeSeatStatePara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTKICKFROMROOM,		Data, CRequestKickFromRoomPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCHANGEREADYSTATE,	Data, CRequestChangeReadyStatePara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTSTARTGAME,		Data, CRequestStartGamePara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTPREPAREREADY,		Data, CRequestPrepareReadyPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTREPORTCURRENTINFO,Data, CRequestReportCurrentInfoPara,Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCHANGEMAP,		Data, CRequestChangeMapPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCHANGETEAM,		Data, CRequestChangeTeamPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCHANGEITEMSTATUS,	Data, CRequestChangeItemStatus,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_SMC_NOTIFYPLAYERCHANGEITEMSTATUS,	Data, CNotifyChangeItemStatusPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTFINDPLAYERBYQQ,	Data, CRequestFindPlayerByQQPara,		Buffer, OutLen);

	ADD_ENCODE_DATA(ID_CMS_REQUESTTALK,				Data, CRequestTalkPara,				Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTHELLO,			Data, CRequestHelloPara,			Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTUDPOK,			Data, CRequestTransferUDPOKPara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_SMC_NOTIFYTESTNETSPEED,		Data, CNotifyTestNetSpeedPara,		Buffer, OutLen);

	ADD_ENCODE_DATA(ID_CMS_REQUESTDIRFT,			Data, CRequestReportDriftPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTGETPROP,			Data, CRequestGetPropPara,			Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTUSEPROP,			Data, CRequestUsePropPara,			Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTPROPEFFECTRESULT,	Data, CRequestPropEffectResultPara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCHANGEAIMSTATE,	Data, CRequestChangeAimStatePara,	Buffer, OutLen);

	ADD_ENCODE_DATA(ID_CMS_REQUESTASSIGNATTRIBUTE,	Data, CRequestAssignAttributePara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCLEARATTRIBUTE,	Data, CRequestClearAttributePara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTCHECKNAME,		Data, CRequestCheckNamePara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTBUYCOMMODITY,		Data, CRequestBuyCommodityPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTGETCOMMODITYLIST,	Data, CRequestGetCommodityListPara,	Buffer, OutLen);

	ADD_ENCODE_DATA(ID_CMS_REQUESTGETTASKLIST,		Data, CRequestGetTaskListPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTTAKETASK,			Data, CRequestTakeTaskPara,			Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTDELETETASK,		Data, CRequestDeleteTaskPara,		Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTSTARTSINGLETASK,	Data, CRequestStartSingleTaskPara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTREPORTCURRENTOPT,	Data, CRequestReportCurrentOptPara,	Buffer, OutLen);
	ADD_ENCODE_DATA(ID_CMS_REQUESTSKIPTASK,			Data, CRequestSkipTaskPara,			Buffer, OutLen);
	END_ENCODE_DATA;
}

BOOL DecodeData(UINT DataID, const BYTE* Data, UINT DataLen, BYTE* Buffer, UINT* OutLen)
{
	BEGIN_DECODE_DATA(DataID);
	ADD_DECODE_DATA(ID_CMD_REQGETHALLINFO,			Data, DataLen, CResponseGetWorldInfoPara,	Buffer, OutLen);//dir
	ADD_DECODE_DATA(ID_CMS_REQUESTLOGIN,			Data, DataLen, CResponseLoginPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTREGISTER,			Data, DataLen, CResponseRegisterPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTLOGOUT,			Data, DataLen, CResponseLogoutPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTGETROOMLIST,		Data, DataLen, CResponseGetRoomListPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCREATEROOM,		Data, DataLen, CResponseCreateRoomPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTENTERROOM,		Data, DataLen, CResponseEnterRoomPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYENTERROOM,			Data, DataLen, CNotifyEnterRoomPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTLEAVEROOM,		Data, DataLen, CResponseLeaveRoomPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYLEAVEROOM,			Data, DataLen, CNotifyLeaveRoomPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYCHANGESEATSTATE,	Data, DataLen, CNotifyChangeSeatStatePara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYKICKFROMROOM,		Data, DataLen, CNotifyKickFromRoomPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYCHANGEREADYSTATE,	Data, DataLen, CNotifyChangeReadyStatePara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYTALK,				Data, DataLen, CNotifyTalkPara,				Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYTESTNETSPEED,		Data, DataLen, CNotifyTestNetSpeedPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTQUICKENTERROOM,	Data, DataLen, CResponseQuickEnterRoomPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCHANGESEATSTATE,	Data, DataLen, CResponseChangeSeatStatePara,Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTKICKFROMROOM,		Data, DataLen, CResponseKickFromRoomPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCHANGEREADYSTATE,	Data, DataLen, CResponseChangeReadyStatePara,Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTSTARTGAME,		Data, DataLen, CResponseStartGamePara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYGAMEBEGIN,			Data, DataLen, CNotifyGameBeginPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYWAITUNREADYPLAYERS,Data, DataLen, CNotifyWaitUnReadyPlayersPara,Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYRACEBEGIN,			Data, DataLen, CNotifyRaceBeginPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYCOUNTDOWN,			Data, DataLen, CNotifyCountDownPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYRACEOVER,			Data, DataLen, CNotifyRaceOverPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYGAMEOVER,			Data, DataLen, CNotifyGameOverPara,         Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYRACESHOW,			Data, DataLen, CNotifyRaceShowPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYKICKFROMSERVER,	Data, DataLen, CNotifyKickFromServerPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYPLAYERFINISHRACE,	Data, DataLen, CNotifyPlayerFinishRacePara,	Buffer, OutLen);


	ADD_DECODE_DATA(ID_CMS_REQUESTREPORTCURRENTINFO,Data, DataLen, CRequestReportCurrentInfoPara,Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTPREPAREREADY	,	Data, DataLen, CRequestPrepareReadyPara,    Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCHANGEMAP		,	Data, DataLen, CResponseChangeMapPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYCHANGEMAP		,	Data, DataLen, CNotifyChangeMapPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCHANGETEAM	,	Data, DataLen, CResponseChangeTeamPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCHANGEITEMSTATUS,	Data, DataLen, CResponseChangeItemStatus,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYCHANGETEAM		,	Data, DataLen, CNotifyPlayerChangeTeamPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYCHANGEROOMOWNER,	Data, DataLen, CNotifyChangeRoomOwnerPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYREPLACEPROP,		Data, DataLen, CNotifyReplacePropPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYDRIFT,				Data, DataLen, CNotifyReportDriftPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYADDPROP,			Data, DataLen, CNotifyAddPropPara,			Buffer, OutLen);

	ADD_DECODE_DATA(ID_CMS_REQUESTTALK,				Data, DataLen, CResponseTalkPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTHELLO,			Data, DataLen, CResponseHelloPara,			Buffer, OutLen);

	ADD_DECODE_DATA(ID_CMS_REQUESTUDPOK,			Data, DataLen, CResponseTransferUDPOKPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYUDPOK,				Data, DataLen, CNotifyUDPOKPara,			Buffer, OutLen);

	ADD_DECODE_DATA(ID_CMS_REQUESTDIRFT,			Data, DataLen, CResponseReportDriftPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTGETPROP,			Data, DataLen, CResponseGetPropPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYGETPROP,			Data, DataLen, CNotifyGetPropPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTUSEPROP,			Data, DataLen, CResponseUsePropPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYUSEPROP,			Data, DataLen, CNotifyUsePropPara,			Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYPROPEFFECTRESULT,	Data, DataLen, CNotifyPropEffectResultPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYCHANGEAIMSTATE,	Data, DataLen, CNotifyChangeAimStatePara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYPLAYERCHANGEITEMSTATUS,	Data, DataLen, CNotifyChangeItemStatusPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTFINDPLAYERBYQQ,	Data, DataLen, CResponseFindPlayerByQQPara,	Buffer, OutLen);

	ADD_DECODE_DATA(ID_CMS_REQUESTASSIGNATTRIBUTE,	Data, DataLen, CResponseAssignAttributePara,Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCLEARATTRIBUTE,	Data, DataLen, CResponseClearAttributePara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTCHECKNAME,		Data, DataLen, CResponseCheckNamePara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTBUYCOMMODITY,		Data, DataLen, CResponseBuyCommodityPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTGETCOMMODITYLIST,	Data, DataLen, CResponseGetCommodityListPara,Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYADDITEM,			Data, DataLen, CNotifyClientAddItemPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYUPDATEMONEY,	Data, DataLen, CNotifyClientUpdateMoneyPara,	Buffer, OutLen);

	ADD_DECODE_DATA(ID_CMS_REQUESTGETTASKLIST,		Data, DataLen, CResponseGetTaskListPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTTAKETASK,			Data, DataLen, CResponseTakeTaskPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTDELETETASK,		Data, DataLen, CResponseDeleteTaskPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTSTARTSINGLETASK,	Data, DataLen, CResponseStartSingleTaskPara,Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYSINGLEGAMEBEGIN,	Data, DataLen, CNotifySingleGameBeginPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYSINGLERACEBEGIN,	Data, DataLen, CNotifySingleRaceBeginPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYSINGLERACEFINISH,	Data, DataLen, CNotifySingleRaceFinishPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYSINGLERACEOVER,	Data, DataLen, CNotifySingleRaceOverPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYSINGLEGAMEOVER,	Data, DataLen, CNotifySingleGameOverPara,	Buffer, OutLen);
	ADD_DECODE_DATA(ID_CMS_REQUESTSKIPTASK,			Data, DataLen, CResponseSkipTaskPara,		Buffer, OutLen);
	ADD_DECODE_DATA(ID_SMC_NOTIFYTASKAWARD,			Data, DataLen, CNotifyTaskAwardPara,		Buffer, OutLen);

	END_DECODE_DATA;
}