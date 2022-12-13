#pragma once

#include "Network/Network_i.h"
#include "TcpSocket.h"
#include "MultiSvrTcpSocket.h"
#include "common.h"
#include "network/msg.h"

#pragma pack(1)
typedef struct tagDirLoginInfo
{
	UINT uin;
	BYTE byKey[KEY_LEN];
	BYTE byGameSignatureLen;
	BYTE byGameSignature[GAME_SIGNATURE_LEN];
	DWORD dwTime;
	DWORD dwVersion;
	DWORD dwLuVersion;
	DWORD dwLuOptVersion;
	DWORD dwWorldID;
	UCHAR  szFileHash[FILE_HASH_LEN];
}DirLoginInfo;
#pragma pack()

//CDirSocket
class CDirSocket : //public CTcpSocket
	public CMultiSvrTcpSocket
{
public:
	CDirSocket(): m_dwWorldID(0){}

	bool Init(const DirLoginInfo* LoginInfo);
	void SetActiveWorld(DWORD dwWorldID);
	DirResponseInfo*	GetResponseInfo() 
	{ 
		if ( GetResponse() ) 
			return &m_ResponseInfo; 
		else
			return NULL;
	}

	//CMultiSvrTcpSocket
	virtual bool Login();
	virtual ServerEntity GetEntity() { return eDirServer; }
	virtual void OnMyConnect(int nErrorCode);
	virtual void OnMyReceive(BYTE *Data, int DataLen);
	virtual void OnMyClose(int nErrorCode);
	virtual void OnMySend(int nErrorCode);

protected:
	DirLoginInfo	m_LoginInfo;
	DirResponseInfo	m_ResponseInfo;
	DWORD			m_dwWorldID;
};


