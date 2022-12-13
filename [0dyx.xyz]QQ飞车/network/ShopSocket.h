#pragma once

#include "Network/Network_i.h"
#include "TcpSocket.h"
#include "MultiSvrTcpSocket.h"
#include "common.h"
#include "network/msg.h"

// CShopSocket 
typedef struct tagShopLoginInfo
{
	UINT uin;
	BYTE byKey[KEY_LEN];
	BYTE byGameSignatureLen;
	BYTE byGameSignature[GAME_SIGNATURE_LEN];
}ShopLoginInfo;

class CShopSocket : //public CTcpSocket
	public CMultiSvrTcpSocket
{
public:
	CShopSocket();
	virtual ~CShopSocket();

	bool Init(ShopLoginInfo*);
	BOOL RequestShop(UINT MsgID, UINT MsgType, void* Data);

protected:
	virtual ServerEntity GetEntity() { return eShopServer; }
	virtual void OnMyReceive(BYTE* Data, int DataLen);
	virtual void OnMyConnect(int nErrorCode);
	virtual void OnMyClose(int nErrorCode);
	virtual void OnMySend(int nErrorCode);

private:
	ShopLoginInfo	m_LoginInfo;
};


