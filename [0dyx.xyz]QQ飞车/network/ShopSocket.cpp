// ShopSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "ShopSocket.h"
#include "EncryptUtil.h"

// CShopSocket

CShopSocket::CShopSocket()
{
}

CShopSocket::~CShopSocket()
{
}

bool CShopSocket::Init(ShopLoginInfo* LoginInfo)
{
	ASSERT(LoginInfo);
	m_LoginInfo = *LoginInfo;
	return true;
}

BOOL CShopSocket::RequestShop(UINT MsgID, UINT MsgType, void* Data)
{
	BYTE byBuffer[NORMAL_DATA_LEN];
	int nBufferLen = NORMAL_DATA_LEN;
	if ( !CEncryptUtil::Pack(byBuffer, nBufferLen, m_LoginInfo.uin, MsgID, (LPBYTE)Data, 0,
		m_LoginInfo.byKey, m_LoginInfo.byGameSignature, m_LoginInfo.byGameSignatureLen, 
		MsgID, MsgType, FE_CLIENT, FE_SHOPSVRD, 0, GetSocketInfo()->uServerID) )
		return FALSE;
	return SendData(byBuffer, nBufferLen);	
}

void CShopSocket::OnMyReceive(BYTE* Data, int DataLen)
{
	CMsgHead aMsgHead;
	CCSHead aCSHead;
	static BYTE byBody[MAX_STACKDATASIZE];
	int nBodyLen = 0;
	if ( !CEncryptUtil::Unpack(Data, DataLen, m_LoginInfo.byKey, &aCSHead, &aMsgHead, byBody, nBodyLen) )
		goto EXIT;
	m_pSinkProxy->OnServerMessage(eShopServer, aMsgHead.m_sMsgID, aMsgHead.m_sMsgType, byBody, nBodyLen, (PVOID)&aCSHead, (PVOID)&aMsgHead);
EXIT:
	;
}

void CShopSocket::OnMyConnect(int nErrorCode)
{
	LogText(nErrorCode == 0 ? "Connected to ShopServer.\n" : "Unable to connnect to ShopServer.\n");
	m_pSinkProxy->OnConnect(eShopServer, nErrorCode);
}

void CShopSocket::OnMyClose(int nErrorCode)
{
	LogText("ShopSocket closed.\n");
	m_pSinkProxy->OnClose(eShopServer, nErrorCode);
}

void CShopSocket::OnMySend(int nErrorCode)
{
	m_pSinkProxy->OnSend(eShopServer, nErrorCode);
}
