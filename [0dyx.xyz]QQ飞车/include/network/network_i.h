////////////////////////////////////////////////
//     FileName:    network_i.h
//     Author:      Fish
//     Date:        2006-11-20 15:38:55
//     Description: (]
////////////////////////////////////////////////
#pragma once

#include "framework/comdef.h"
#include "network/consts.h"
#include "msg.h"

typedef enum tagServerEntity
{
	eQQServer	= 0,
	eDirServer	= 1,
	eGameServer = 2,
	eShopServer = 3,
}ServerEntity;

typedef enum tagLoginResult
{
	eLoginSuccessed			= 0,
	eLoginFailed			= 1,
	eLoginTimeout			= 2,
	eLoginRequireReg		= 3,
	eLoginCanceled			= 4,
	eLoginIdentityLimit		= 5,
	eLoginVersionLimit		= 6,
	eLoginVersionRecommend	= 7,
	eLoginFileCracked		= 8,
}LoginResult;
#define LOGINSUCCESSED(hr)	(hr == eLoginSuccessed || hr == eLoginRequireReg || hr == eLoginVersionRecommend)

typedef enum tagLoginType
{
	eLoginNormal		= 0,
	eLoginFromQQ		= 1,
	eLoginFromQQGame	= 2,
}LoginType;

typedef struct tagQQUSERDATA
{
	UINT	Uin;
	BYTE	byPwd[PWD_LEN];
	BYTE	byPwdHash2[KEY_LEN];
	BYTE	byKey[KEY_LEN];
	BYTE	byUserServiceBitmap[NICK_NAME_LEN];
	DWORD	dwUinFlag;
	BYTE	byGameSignatureLen;
	BYTE	byGameSignature[GAME_SIGNATURE_LEN];
	USHORT	nFace;
	BYTE	byAge;
	BYTE	byGender;
	BYTE	byNickNameLen;
	CHAR	szNickName[NICK_NAME_LEN];
}QQUSERDATA;
typedef QQUSERDATA*			LPQQUSERDATA;
typedef const LPQQUSERDATA	LPCQQUSERDATA;

#define MAX_PORTS_COUNT	3
#define MAX_HOSTNAME	100
#define MAX_HOSTADDR	100
typedef struct tagSOCKET_CREATE_INFO
{
	CHAR szHostName[MAX_HOSTNAME];
	CHAR szHostAddr[MAX_HOSTADDR];
	UINT uPorts[MAX_PORTS_COUNT];
	UINT uUDPPorts[MAX_PORTS_COUNT];
	UINT uServerID;
	UINT nSndBufSize;
	UINT nRcvBufSize;
}SOCKET_CREATE_INFO;
typedef SOCKET_CREATE_INFO*			LPSOCKET_CREATE_INFO;
typedef const LPSOCKET_CREATE_INFO	LPCSOCKET_CREATE_INFO;

typedef CResponseGetWorldInfoPara	DirResponseInfo;
typedef CResponseLoginPara	GameResponseInfo;

DECL_IID(INetwork, "7494E55F-85B6-495C-9140-54BA62463604")
struct DECLSPEC_NOVTABLE INetwork : IUnknown
{
	STDMETHOD(SetActiveWorld)(DWORD dwWorldID) PURE;
	STDMETHOD(SetLoginType)(UINT) PURE;
	STDMETHOD(StartLogin)(UINT Uin, PBYTE Pwd) PURE;
	STDMETHOD(CancelLogin)(void) PURE;
	STDMETHOD(StartReLogin)(void) PURE;
	STDMETHOD(LoginGameServer)(DWORD dwCountry, DWORD dwChannel, DWORD dwSvrID = 0) PURE;
	STDMETHOD(RequestGameServer)(UINT MsgID, PBYTE Data, UINT PlayerID, BYTE DestFE, SHORT DestID, UINT MsgType = 0) PURE;
	STDMETHOD(RequestShopServer)(UINT MsgID, PBYTE Data, UINT MsgType = 0) PURE;
	STDMETHOD(SendTcpP2pData)(SHORT PlayerID, INT PlayerNum, SHORT* PlayerIDs, UINT* PlayerUins, PBYTE Data, INT DataLen) PURE;
	
	virtual QQUSERDATA* GetQQUserData() PURE;
	virtual DirResponseInfo* GetDirInfo() PURE;
	virtual GameResponseInfo* GetGameInfo() PURE;

	virtual SOCKET_CREATE_INFO* GetSocketInfo(ServerEntity) PURE;

	virtual int oi_symmetry_encrypt2_len(int nInBufLen) PURE;
	virtual void oi_symmetry_encrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen) PURE;
	virtual BOOL oi_symmetry_decrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen) PURE;
};

DECL_IID(INetworkSink, "AFDDA123-3BD8-4ed3-9DA7-75674D5825C3")
struct DECLSPEC_NOVTABLE INetworkSink : IUnknown
{
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult) PURE;
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead) PURE;
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnCreate)(ServerEntity, int nErrorCode) PURE;
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode) PURE;
};

DECL_CLSID(NetworkImpl)