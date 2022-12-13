// QQLoginSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "QQLoginSocket.h"
#include "EncryptUtil.h"
#include "Network/QQMsgStu.h"

bool CQQLoginSocket::Init(const QQLoginInfo* LoginInfo)
{
	Reset();
	assert(LoginInfo);
	memcpy(&m_LoginInfo, LoginInfo, sizeof(QQLoginInfo));
	m_QQUserData.Uin = LoginInfo->uin;
	memcpy(m_QQUserData.byPwd, LoginInfo->byPwd, MD5_DIGEST_LENGTH);
	return true;
}

bool CQQLoginSocket::Login()
{
	BYTE byBuffer[NORMAL_DATA_LEN];
	short nBufferLen;
	CEncryptUtil::PackQQSendData(m_LoginInfo.uin, m_LoginInfo.byPwd, m_LoginInfo.nHashType, CMD_QQ_VALIDATA, 0, byBuffer, nBufferLen, m_QQUserData.byPwdHash2);
	SendData(byBuffer, nBufferLen);
	m_pSinkProxy->OnStartLogin(eQQServer, 0);
	return true;
}

void CQQLoginSocket::OnMyConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_pSinkProxy->OnConnect(eQQServer, nErrorCode);
	LogText(nErrorCode == 0 ? "Connect to QQ Server OK!\n" : "Connect to QQ Server Failed( After 3 times)!\n");
}

void CQQLoginSocket::OnMyReceive(BYTE *Data, int DataLen)
{
	// TODO: 在此添加专用代码和/或调用基类
	QQ_RECV_HEAD aRecvHead;
	BYTE byBuffer[NORMAL_DATA_LEN];
	int nBufferLen = NORMAL_DATA_LEN;
	LoginResult lr = eLoginFailed;
	if ( !CEncryptUtil::UnPackQQRecvData(Data, DataLen, m_QQUserData.byPwdHash2, &aRecvHead, byBuffer, nBufferLen) )
		goto EXIT;

	LPBYTE byData = byBuffer;
	QQ_REPONSE_MSGHEAD* pMsgHead = (QQ_REPONSE_MSGHEAD*)byData;
	byData += sizeof(QQ_REPONSE_MSGHEAD);

	if( CMD_QQ_VALIDATA == aRecvHead.nCmd )
	{
		if ( pMsgHead->byType == 1)  //cType=1 表示是取Game签名，等于请求消息
		{
			if( 0 == pMsgHead->byErrorCode )  // 0 表示成功
			{
				//签名信息
				QQ_RESPONSE_GETSIGN SignInfo;
				::memcpy(&SignInfo, byData, sizeof(QQ_RESPONSE_GETSIGN));
				byData += sizeof(QQ_RESPONSE_GETSIGN);

				BYTE byGameSignature[GAME_SIGNATURE_LEN] = { 0 };
				memcpy(byGameSignature, byData, SignInfo.byGameSignatureLen);
				SignInfo.Decode();

				byData += SignInfo.byGameSignatureLen;
				QQ_RESPONSE_USERINFO UserInfo;
				::memcpy(&UserInfo, byData, sizeof(QQ_RESPONSE_USERINFO));
				UserInfo.Decode();
				byData += sizeof(QQ_RESPONSE_USERINFO);
				BYTE byNickName[NICK_NAME_LEN] = { 0 };
				memcpy(byNickName, byData, UserInfo.byNickLen);

				// copy to data member
				m_QQUserData.byAge = UserInfo.byAge;
				memcpy(m_QQUserData.byKey, SignInfo.byKey, KEY_LEN);
				memcpy(m_QQUserData.byGameSignature, byGameSignature, SignInfo.byGameSignatureLen);
				m_QQUserData.byGameSignatureLen = SignInfo.byGameSignatureLen;
				m_QQUserData.byGender = UserInfo.byGender;
				m_QQUserData.byNickNameLen = UserInfo.byNickLen;
				memcpy(m_QQUserData.byUserServiceBitmap, SignInfo.byUserServiceBitmap, BITMAPLEN);
				m_QQUserData.dwUinFlag = SignInfo.dwUinFlag;
				m_QQUserData.nFace = UserInfo.nFace;
				memset(m_QQUserData.szNickName, 0, NICK_NAME_LEN);
				strncpy(m_QQUserData.szNickName, (char*)byNickName, UserInfo.byNickLen);

				lr = eLoginSuccessed;
			}
		}//取签名完成
	}
EXIT:
	m_pSinkProxy->OnResponseLogin(eQQServer, lr);
}

void CQQLoginSocket::OnMyClose(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	m_pSinkProxy->OnClose(eQQServer, nErrorCode);
}

void CQQLoginSocket::OnMySend(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	m_pSinkProxy->OnSend(eQQServer, nErrorCode);
}
