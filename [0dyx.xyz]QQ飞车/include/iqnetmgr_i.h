/* -------------------------------------------------------------------------
//	�ļ���		��	iqnetmgr.h
//	������		��	FishHong
//	����ʱ��	��	2006-3-16 16:58:58
//	��������	��	
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __IQNETMGR_H__
#define __IQNETMGR_H__
// -------------------------------------------------------------------------
#include "objbase.h"
#include "BaseInterface.h"
#include "IBaseListener.h"
#include "IBaseProducer.h"
#include "dirmsg.h"

//��������ԭ��
//extern "C" HRESULT CreateIQNetMgr(IGameApp*, IIQNetMgr**);

#define NORMAL_DATA_LEN		2048
#define MAX_DATA_LEN		1024*500
#define FILE_HASH_LEN		32
#define KEY_LEN				16
#define GAME_SIGNATURE_LEN	128
#define NICK_NAME_LEN		32
#define MAX_CONNECT_COUNT	3
#define PWD_LEN				16
#define MAX_DECRPYT_BUFFER_LENGTH 307200

enum ServerEntity
{
	eQQServer = 0,
	eDirServer = 1,
	eGameServer = 2,
	eShopServer = 3
};

enum LoginResult
{
	eLoginSuccessed = 0,
	eLoginFailed = 1,
	eLoginTimeout = 2,
	eLoginRequireReg = 3,
	eLoginCanceled =4,
	eLoginIdentityLimit,
	eLoginVersionLimit,
	eLoginVersionRecommend,
	eLoginFileCracked
};

enum LoginType
{
	eLoginNormal = 0,
	eLoginFromQQ = 1,
	eLoginFromQQGame = 2
};

typedef struct  tagQQUSERDATA
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

interface IIQNetSink : IBaseListener
{
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult){ return S_OK;	}
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, const void* Data){ return S_OK;}
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode){ return S_OK;}
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode){ return S_OK;}
	STDMETHOD(OnSocketCreate)(ServerEntity, int nErrorCode){ return S_OK;}
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode){ return S_OK;}
};

interface IPingProxySink : IBaseListener
{
	STDMETHOD(NotifyPingResult)(int nIndex, int nTTL, float fLossRate, int nAverageime) { return S_OK; }
};

typedef IBaseEventListenerContainer<IIQNetSink>	INetSinkCP;
typedef IBaseEventListenerContainer<IPingProxySink>	IPingProxySinkCP;

interface IPingMgrProxy;

interface IIQNetMgr : BaseInterface
{
	//StartLogin��������½���̣�����QQ���������
	//����Ǵ�QQ��Ϸ����QQ��½��Uin = 0
	//���������ȵ�½QQȡǩ����Ȼ���½Dir��������ȡServer��Ϣ
	STDMETHOD(StartLogin)(UINT Uin, const BYTE* Pwd)	PURE;
	STDMETHOD(CancelLogin)() PURE;
	STDMETHOD(StartReLogin)() PURE;
	STDMETHOD(LoginGameServer)(INT ServerID) PURE;

	//ѡ�������Ϸ������
	//�Զ�ѡ��ServerID= -1��>=0��ֵ��ʾ����������
	//���¶��Ǹ���Ϸ�������Ľ���
	STDMETHOD(RequestEnterGameServer)(int ServerID, UINT Version)	PURE;
	STDMETHOD(RequestLeaveGameServer)(UINT PlayerID) PURE;

	STDMETHOD(RequestGameServer)(UINT MsgID, void* Data, UINT PlayerID, UINT MsgType = Request) PURE;
	STDMETHOD(RequestPlayer)(UINT MsgID, void* Data, UINT PlayerID, UINT MsgType = Request) PURE;
	STDMETHOD(RequestTeam)(UINT MsgID, void* Data, UINT PlayerID, UINT TeamID, UINT MsgType = Request) PURE;
	STDMETHOD(RequestGameLogic)(UINT MsgID, void* Data, UINT PlayerID, UINT GameID, UINT MsgType = Request) PURE;

	//�̵����
	STDMETHOD(RequestShopServer)(UINT MsgID, void* Data, UINT MsgType = Request) PURE;

	//����������
	STDMETHOD_(IPingMgrProxy*, GetPingMgr)() PURE;
	//ȡ�óɹ���½֮��ӷ�������õ���Ϣ
	STDMETHOD_(const QQUSERDATA*, GetUserData)() PURE;
	STDMETHOD_(const CResponseGetHallInfoPara*, GetHallInfo)() PURE;
	STDMETHOD_(const CResponseLoginPara*, GetGameUserData)() PURE;

	//�¼��ص�֧��
	STDMETHOD_(INetSinkCP*, GetEventListenerContainer)() PURE;

	STDMETHOD(RequestLoginDir)() PURE;
	STDMETHOD_(void*, GetHashString)() PURE;
};


interface IPingMgrProxy : BaseInterface
{
	STDMETHOD(PingServer)(unsigned long ulSvrIP, unsigned short usPort, int __RPC_FAR *pnIndex,	int nLimitTime) PURE;
	STDMETHOD(CancelPing)(int nIndex) PURE;
	STDMETHOD_(IPingProxySinkCP*, GetEventListenerContainer)() PURE;
};


// -------------------------------------------------------------------------
//	$Log: $

#endif /* __IQNETMGR.H__ */
