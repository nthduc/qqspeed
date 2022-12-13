#pragma once

#include "objbase.h"
#include "Network/Network_i.h"
#include "Network/QQMsgStu.h"
#include "Network/consts.h"

#define MAX_PORTS_COUNT	3

enum ConnectState
{
	eConnectReady = -1,	//未连接
	eConnecting,
	eConnectOK,	//连接成功
	eConnectRetrying,//正在重试
	eConnectClosed, //连接被关闭
	eConnectError, //连接失败
};

#define RCV_BUF_LEN	(1024*500)
#define SND_BUF_LEN	(1024*500)

interface ISocketSinkProxy
{
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult) PURE;
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnReceive)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnSend)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PVOID Data, INT DataLen, PVOID, PVOID) PURE;
};

typedef struct tagQQGAMELOGINFILEDATA
{
	UINT	Uin;
	char	szNick[NICK_NAME_LEN];
	int		nGender;
	int		nAge;
	int		nFaceID;
	short	nKeyLen;
	BYTE	byKey[GAME_SIGNATURE_LEN];      
	short	nGameSignatureLen;  
	BYTE	byGameSignature[GAME_SIGNATURE_LEN];  
	int		nIdentity;
	BYTE	byUserServiceBitmap[BITMAPLEN]; 
	DWORD	dwUinFlag;
}QQGAMELOGINFILEDATA;

// QQ Start
#define	WM_QQ_REQUEST_PLAY_GAME		1016
#define	WM_GAME_NOTIFY_PLAY_GAME	1014
#define WM_QQ_REQUEST_ACCEPT_INVITE	1018

typedef struct tagQQ_REQUEST_PLAY_GAME
{
	short m_shGameID;
	CHAR m_chKey[KEY_LEN];
}QQ_REQUEST_PLAY_GAME;

typedef struct tagQQ_REQUEST_ACCEPT_INVITE
{
	short m_shServerID;
	short m_shSectionID;
	short m_shRoomID;
	short m_shSeatID;
	short m_shGameID;
	short m_TheStep;
	short m_TheRequest;
	CHAR m_chKey[KEY_LEN];
}QQ_REQUEST_ACCEPT_INVITE;

typedef struct tagGAME_NOTIFY_PLAY_GAME
{
	short m_shServerID;
	short m_shSectionID;
	short m_shRoomID;
	BYTE m_bySeatID;
	short m_shGameID;
	BYTE m_bystatus;
	BYTE m_byRequest;
	CHAR m_chKey[KEY_LEN];
}GAME_NOTIFY_PLAY_GAME;