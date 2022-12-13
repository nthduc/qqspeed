#pragma once

#include "Network/Network_i.h"
#include "TcpSocket.h"
#include "MultiSvrTcpSocket.h"
#include "common.h"
#include "network/msg.h"

typedef struct tagGameLoginInfo
{
	UINT uin;
	BYTE byKey[KEY_LEN];
	BYTE byGameSignatureLen;
	BYTE byGameSignature[GAME_SIGNATURE_LEN];
	UINT nTime;
	UINT nVersion;
	CHAR szNickName[NICK_NAME_LEN];
	BYTE byGender;
	BYTE byIconID;
	UINT nIdentity;
}GameLoginInfo;

// CGameSocket 
class CGameSocket : // public CTcpSocket
	public CMultiSvrTcpSocket
{
public:
	CGameSocket():	m_nLoginType(ELT_Single) {}
	bool Init(const GameLoginInfo*);
	UINT SetLoginType(UINT);
	GameResponseInfo* GetResponseInfo() 
	{ 
		if ( GetResponse() )
			return &m_ResponseInfo; 
		else
			return NULL;
	}
	
	BOOL RequestGameServer(UINT MsgID, void* Data, UINT PlayerID, BYTE DestFE, SHORT DestID, UINT MsgType);
	BOOL SendTcpP2pData(SHORT PlayerID, INT PlayerNum, SHORT* PlayerIDs, UINT* PlayerUins, PBYTE Data, INT DataLen);

	//CMultiSvrTcpSocket
	virtual bool Login();
	virtual ServerEntity GetEntity() { return eGameServer; }
	virtual void OnMyReceive(BYTE* Data, int DataLen);
	virtual void OnMyConnect(int nErrorCode);
	virtual void OnMyClose(int nErrorCode);
	virtual void OnMySend(int nErrorCode);

protected:
	GameLoginInfo		m_LoginInfo;
	GameResponseInfo	m_ResponseInfo;
	UINT				m_nLoginType;
};


