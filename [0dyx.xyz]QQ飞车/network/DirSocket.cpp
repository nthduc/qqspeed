// DirSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "DirSocket.h"
#include "EncryptUtil.h"
#include "Encoder.h"
#include "network/QQCrypt.h"

bool CDirSocket::Init(const DirLoginInfo* LoginInfo)
{
	assert(LoginInfo);
	memcpy(&m_LoginInfo, LoginInfo, sizeof(DirLoginInfo));
	Reset();
	return true;
}

void CDirSocket::SetActiveWorld(DWORD dwWorldID)
{
	m_dwWorldID = dwWorldID;
}

bool CDirSocket::Login()
{
	CRequestGetWorldInfoPara kPara;
	kPara.m_uiTime = m_LoginInfo.dwTime;
	kPara.m_uiUin = m_LoginInfo.uin;
	kPara.m_uiClientVersion = m_LoginInfo.dwVersion;
	kPara.m_uiMasterVersion = m_LoginInfo.dwLuVersion;
	kPara.m_uiSlaveVersion = m_LoginInfo.dwLuOptVersion;
	kPara.m_byWorldID = m_dwWorldID;
	memcpy(kPara.m_szFileHash, m_LoginInfo.szFileHash, FILE_HASH_LEN);

	BYTE byBuffer[NORMAL_DATA_LEN];
	int nBufferLen = NORMAL_DATA_LEN;
	CEncryptUtil::Pack(byBuffer, nBufferLen, m_LoginInfo.uin, ID_CMD_REQGETHALLINFO, (LPBYTE)&kPara, 0,
				m_LoginInfo.byKey, m_LoginInfo.byGameSignature, m_LoginInfo.byGameSignatureLen, 
				ID_CMD_REQGETHALLINFO, Request, FE_CLIENT, FE_DIRSVRD, -1, GetSocketInfo()->uServerID);
	SendData(byBuffer, nBufferLen);

	m_pSinkProxy->OnStartLogin(eDirServer, 0);
	return true;
}

void CDirSocket::OnMyConnect(int nErrorCode)
{
	m_pSinkProxy->OnConnect(eDirServer, nErrorCode);
	LogText(nErrorCode == 0 ? "Connect to Dir Server OK!\n" : "Connect to Dir Server Failed!\n");
}

void CDirSocket::OnMyReceive(BYTE *Data, int DataLen)
{
	LogText("Receive from Dir Server!\n");
	CMsgHead aMsgHead;
	CCSHead aCSHead;
	static BYTE byBody[MAX_STACKDATASIZE];
	int nBodyLen = 0;
	if ( !CEncryptUtil::Unpack(Data, DataLen, m_LoginInfo.byKey, &aCSHead, &aMsgHead, byBody, nBodyLen) )
		goto EXIT;

	switch(aMsgHead.m_sMsgID)
	{	
		// new version
	case ID_CMD_REQGETHALLINFO:
		{
			CResponseGetWorldInfoPara* pPara = (CResponseGetWorldInfoPara*)byBody;
			m_ResponseInfo = *pPara;
			LoginResult lr;
			switch(pPara->m_ushResultID)
			{
			case EDLR_Succeed:
				lr = eLoginSuccessed;
				break;
			case EDLR_Err_IdentityLimit:
				lr = eLoginIdentityLimit;
				break;
			case EDLR_Err_VersionLimit:
				lr = eLoginVersionLimit;
				break;
			case EDLR_Err_VersionRecommend:
				lr = eLoginVersionRecommend;
				break;
			case EDLR_Err_FileCracked:
			case EDLR_Err_WorldIDError:
				lr = eLoginFileCracked;
				break;
			default:
				lr = eLoginFailed;
				break;
			}
			m_pSinkProxy->OnResponseLogin(eDirServer, lr);
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	m_pSinkProxy->OnServerMessage(eDirServer, aMsgHead.m_sMsgID, aMsgHead.m_sMsgType, byBody, nBodyLen, (PVOID)&aCSHead, (PVOID)&aMsgHead);

EXIT:
	;
}

void CDirSocket::OnMyClose(int nErrorCode)
{
	m_pSinkProxy->OnClose(eDirServer, nErrorCode);
}

void CDirSocket::OnMySend(int nErrorCode)
{
	m_pSinkProxy->OnSend(eDirServer, nErrorCode);
}
