// GameSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "GameSocket.h"
#include "network/msg.h"
#include "EncryptUtil.h"

bool CGameSocket::Init(const GameLoginInfo* LoginInfo)
{
	assert(LoginInfo);
	memcpy(&m_LoginInfo, LoginInfo, sizeof(GameLoginInfo));
	memset(&m_ResponseInfo, 0, sizeof(GameResponseInfo));
	Reset();
	return true;
}

UINT CGameSocket::SetLoginType(UINT nLoginType)
{
	UINT nOldType = m_nLoginType;
	m_nLoginType = nLoginType;
	return nOldType;
}

bool CGameSocket::Login()
{
	CRequestLoginPara kPara;
	kPara.m_unUin = m_LoginInfo.uin;
	kPara.m_unTime = m_LoginInfo.nTime;
	kPara.m_unClientVersion = m_LoginInfo.nVersion;
	strncpy(kPara.m_szQQNickname, m_LoginInfo.szNickName, MAXNICKNAMELEN);
	kPara.m_bGender = m_LoginInfo.byGender;
	kPara.m_bIconID = m_LoginInfo.byIconID;
	kPara.m_unAttachIdentify = m_LoginInfo.nIdentity;
	kPara.m_unClientVersion = m_LoginInfo.nVersion;
	kPara.m_uiLoginType = m_nLoginType;
	BYTE byBuffer[NORMAL_DATA_LEN];
	int nBufferLen = NORMAL_DATA_LEN;
	if ( !CEncryptUtil::Pack(byBuffer, nBufferLen, m_LoginInfo.uin, ID_CMS_REQUESTLOGIN, (LPBYTE)&kPara, 0,
		m_LoginInfo.byKey, m_LoginInfo.byGameSignature, m_LoginInfo.byGameSignatureLen, 
		ID_CMS_REQUESTLOGIN, Request, FE_CLIENT, FE_GAMESVRD, -1, GetSocketInfo()->uServerID) )
		return false;
	SendData(byBuffer, nBufferLen);

	m_pSinkProxy->OnStartLogin(eGameServer, 0);
	return true;
}

void CGameSocket::OnMyReceive(BYTE* Data, int DataLen)
{
	CMsgHead aMsgHead;
	CCSHead aCSHead;
	static BYTE byBody[MAX_STACKDATASIZE];
	int nBodyLen = 0;
	if ( !CEncryptUtil::Unpack(Data, DataLen, m_LoginInfo.byKey, &aCSHead, &aMsgHead, byBody, nBodyLen) )
		goto EXIT;

	if ( aMsgHead.m_sMsgID == ID_CMS_REQUESTLOGIN )
	{
		LoginResult eLoginResult = eLoginFailed;
		CResponseLoginPara* pPara = (CResponseLoginPara*)byBody;
		m_ResponseInfo = *pPara;
		if ( pPara->m_nResultID == 0 )
		{
			eLoginResult = eLoginSuccessed;
			LogText("Login Game Server Succeed.\n");
		}
		else if ( pPara->m_nResultID == 1 )
		{
			eLoginResult = eLoginRequireReg;
			LogText("Login Game Server Succeed. Need Register.\n");
		}
		else
		{
			LogText("Login Failed.\n");
		}
		if ( pPara->m_uiLoginType == ELT_Single )
		{
			m_pSinkProxy->OnResponseLogin(eGameServer, eLoginResult);
		}
	}
	m_pSinkProxy->OnServerMessage(eGameServer, aMsgHead.m_sMsgID, aMsgHead.m_sMsgType, byBody, nBodyLen, (PVOID)&aCSHead, (PVOID)&aMsgHead);
EXIT:
	;
}

//比较弱的服务器消息转发机制，没办法，在客户端帮他们做了吧
BOOL CGameSocket::RequestGameServer(UINT MsgID, void* Data, UINT PlayerID, BYTE DestFE, SHORT DestID, UINT MsgType)
{
	BYTE byBuffer[NORMAL_DATA_LEN];
	int nBufferLen = NORMAL_DATA_LEN;
	if ( DestFE == FE_GAMESVRD && DestID <= 0 )
		DestID = GetSocketInfo()->uServerID;
	if ( !CEncryptUtil::Pack(byBuffer, nBufferLen, m_LoginInfo.uin, MsgID, (LPBYTE)Data, 0,
		m_LoginInfo.byKey, m_LoginInfo.byGameSignature, /*m_LoginInfo.byGameSignatureLen*/0, 
		MsgID, MsgType, FE_CLIENT, DestFE, PlayerID, DestID) )
		return FALSE;
	return SendData(byBuffer, nBufferLen);	
}

BOOL CGameSocket::SendTcpP2pData(SHORT PlayerID, INT PlayerNum, SHORT* PlayerIDs, UINT* PlayerUins, PBYTE Data, INT DataLen)
{
	BYTE byBuffer[NORMAL_DATA_LEN];
	int nBufferLen = NORMAL_DATA_LEN;
	CEncryptUtil::Pack_TcpP2pData(byBuffer, nBufferLen, m_LoginInfo.uin, Data, DataLen, 0,
		m_LoginInfo.byKey, m_LoginInfo.byGameSignature, 0,
		PlayerNum, PlayerUins, PlayerID, GetSocketInfo()->uServerID);
	return SendData(byBuffer, nBufferLen);	
}

 void CGameSocket::OnMyConnect(int nErrorCode)
 {
	 LogText(nErrorCode == 0 ? "Connected to GameServer.\n" : "Unable to connnect to GameServer.\n");
	 m_pSinkProxy->OnConnect(eGameServer, nErrorCode);
 }

 void CGameSocket::OnMyClose(int nErrorCode)
 {
	 LogText("GameSocket closed.\n");
	 m_pSinkProxy->OnClose(eGameServer, nErrorCode);
 }

 void CGameSocket::OnMySend(int nErrorCode)
 {
	 m_pSinkProxy->OnSend(eGameServer, nErrorCode);
 }
