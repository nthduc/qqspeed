#include "StdAfx.h"
#include "network/msgheader.h"
#include "network/consts.h"
#include "network/QQCrypt.h"
#include "common.h"
#include "encryptutil.h"
#include "Encoder.h"
#include "xmemguard.h"

void MD5HashString(const BYTE* Src, int HashType, BYTE* Dest) //MD5_DIGEST_LENGTH
{
	switch(HashType)
	{
	case 0:
		{
			BYTE byHash1[MD5_DIGEST_LENGTH];
			Md5HashBuffer(byHash1, Src, byteslen(Src, MD5_DIGEST_LENGTH));
			Md5HashBuffer(Dest, byHash1, MD5_DIGEST_LENGTH);		
		}
		break;
	case 1:
		Md5HashBuffer(Dest, Src, MD5_DIGEST_LENGTH);
		break;
	case 2:
		::memcpy(Dest, Src, MD5_DIGEST_LENGTH);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

UINT NextSeq()
{
	static UINT nSeq = 0;
	return ++nSeq;
}

void CEncryptUtil::PackQQSendData(UINT Uin, const BYTE* Pwd, int PwdType,
								  int Cmd, USHORT Seq,
								  BYTE* OutBuffer, short& OutBufferLen, BYTE* Key)
{
	MD5HashString(Pwd, PwdType, Key);
	static BYTE byExchangeKey[] = "hi!!Fish!!gogogo";
	//把Pwd做两次Md5hash之后作为密钥
	//加密一个空串
	BYTE byTempBuf[64];
	int nTempLen = ::oi_symmetry_encrypt2_len(0);
	oi_symmetry_encrypt2((BYTE*)"", 0, Key, byTempBuf, &nTempLen);

	//在空串加密的结果前面加上QQ堂标志
	BYTE byCryptBuf[NORMAL_DATA_LEN];  //所带的消息流中的加密部分的明文
	byCryptBuf[0] = 0x01;//QQT!!!
	memcpy(byCryptBuf + 1, byTempBuf, nTempLen);

	//用交换密钥加密之前的结果
	int nCyrptOKBufLen = ::oi_symmetry_encrypt2_len(nTempLen + 1);
	BYTE byCryptOKBuffer[NORMAL_DATA_LEN];
	::oi_symmetry_encrypt2(byCryptBuf, nTempLen + 1, byExchangeKey, byCryptOKBuffer, &nCyrptOKBufLen);

	QQ_SEND_HEAD aSendHead;
	aSendHead.nCmd = htons(Cmd);
	aSendHead.Uin = htonl(Uin);
	aSendHead.bySTX = 0x02;
	aSendHead.byVersion[0] = 10; //GetQQMainVer();
	aSendHead.byVersion[1] = 9; //::GetQQSubVer();
	aSendHead.nSeq = htons(Seq);
	OutBufferLen = sizeof(QQ_SEND_HEAD) + MD5_DIGEST_LENGTH + nCyrptOKBufLen + 1;
	aSendHead.nLength = htons(OutBufferLen);	

	//最后的包是：发送头＋交换密钥＋加密数据＋结束标志（0x03）
	BYTE* byCryptData = OutBuffer;
	::memcpy(byCryptData , &aSendHead , sizeof(QQ_SEND_HEAD));
	::memcpy(byCryptData + sizeof(QQ_SEND_HEAD) , byExchangeKey, MD5_DIGEST_LENGTH);//密钥	
	::memcpy(byCryptData + sizeof(QQ_SEND_HEAD) + MD5_DIGEST_LENGTH, byCryptOKBuffer, nCyrptOKBufLen);
	BYTE byOver=0x03;
	::memcpy(byCryptData + sizeof(QQ_SEND_HEAD) + MD5_DIGEST_LENGTH + nCyrptOKBufLen, &byOver, 1);	
}

BOOL CEncryptUtil::UnPackQQRecvData(const BYTE* Data, int DataLen, 
						const BYTE* Key, 
						QQ_RECV_HEAD* RecvHead,
						BYTE* OutBuffer, int& OutBufferLen)
{
	if(DataLen < sizeof(QQ_RECV_HEAD) || NULL == Data)
	{
		return FALSE;
	}

	memcpy(RecvHead, Data, sizeof(QQ_RECV_HEAD));
	RecvHead->Decode();

	int nCyrptBufLen = RecvHead->nLength - sizeof(QQ_RECV_HEAD) - 1;
	const BYTE* byCryptBuf = Data + sizeof(QQ_RECV_HEAD);

	//使用Hash作为密钥解开 表明密码验证成功
	if( ::oi_symmetry_decrypt2(byCryptBuf, nCyrptBufLen, Key, OutBuffer, &OutBufferLen) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CEncryptUtil::Pack(BYTE* OutBuffer, int& OutBufferLen, UINT Uin, int DataID, BYTE* Data, DWORD Seq,
						const BYTE* Key, const BYTE* Sign, BYTE SignLen, 
						short MsgID, short MsgType, char SrcFE, char DestFE, short SrcID, short DestID)
{
	ASSERT(Data);

	BYTE byPackBuffer[NORMAL_DATA_LEN];
	UINT nPackLen = 0;
	if ( !EncodeData(DataID, Data, byPackBuffer, &nPackLen) )
		return FALSE;

	//格式化消息头
	CMsgHead aMsgHead;
	aMsgHead.m_sMsgID = MsgID;
	aMsgHead.m_sMsgType = MsgType;
	aMsgHead.m_iMsgSeq = Seq;
	aMsgHead.m_bSrcFE = SrcFE;
	aMsgHead.m_bDstFE = DestFE;
	aMsgHead.m_sSrcID = SrcID;
	aMsgHead.m_sDstID = DestID;
	int nLen = 0;
	aMsgHead.Encode((char*)&aMsgHead, &nLen, TRUE);

	//格式化消息体
	BYTE byMsgBuffer[NORMAL_DATA_LEN];
	int nMsgLen = nPackLen + sizeof(CMsgHead);
	if( nMsgLen > NORMAL_DATA_LEN )
	{
		ASSERT(FALSE);
		return FALSE;
	}

	::memcpy(byMsgBuffer, &aMsgHead, nLen);
	::memcpy(byMsgBuffer + nLen, byPackBuffer, nPackLen);

	BYTE byCryptOKBuffer[NORMAL_DATA_LEN];
	int nCryptOKLen = ::oi_symmetry_encrypt2_len(nMsgLen);
	::oi_symmetry_encrypt2(byMsgBuffer, nMsgLen, Key, byCryptOKBuffer,&nCryptOKLen);

	CCSHead aCSHead;
	aCSHead.m_iTotalLength = sizeof(CCSHead) + SignLen + nCryptOKLen;
	aCSHead.m_nVer = 0;
	aCSHead.m_nDialogID = SrcID;
	aCSHead.m_iSeq = NextSeq();
	aCSHead.m_iUin = Uin;
	aCSHead.m_byBodyFlag = 1;
	aCSHead.m_byOptLength = SignLen;
	int nHeadLen = 0;
	aCSHead.Encode((char*)OutBuffer, &nHeadLen, TRUE);
	memcpy(OutBuffer + sizeof(CCSHead), Sign, SignLen);
	memcpy(OutBuffer + sizeof(CCSHead) + SignLen, byCryptOKBuffer, nCryptOKLen);
	OutBufferLen = sizeof(CCSHead) + SignLen + nCryptOKLen;
	return TRUE;

}

BOOL CEncryptUtil::Unpack(const BYTE* Data, int DataLen, const BYTE* Key,
				   CCSHead* csHead, CMsgHead* MsgHead, BYTE* Body, int& BodyLen)
{
	static BYTE byMsgData[MAX_STACKDATASIZE];

	// 解出协议头
#ifdef _DEBUG
	MemGuard_MonitorNegative(x)
#endif
	CCSHead aCSHead;
#ifdef _DEBUG
	MemGuard_MonitorPositive(x)
#endif
	aCSHead.Decode((char*)Data, DataLen, TRUE);
	int nHeadLen = sizeof(CCSHead);

	// 得到消息体的数据
	BYTE * pMessageBodyData = const_cast<unsigned char *>(Data) + nHeadLen;
	int nMsgLen = DataLen - nHeadLen;
	int nMsgDataLen = 18000000;

	//这里需要check 使用何种加密算法，由于目前只有1种方式，故不做判断
	if(!::oi_symmetry_decrypt2(pMessageBodyData, nMsgLen, Key, byMsgData, &nMsgDataLen))
	{
		ASSERT(0);
		return FALSE;
	}
	//End Add

	CMsgHead aMsgHead;
	aMsgHead.Decode(byMsgData, nMsgDataLen, TRUE);

	memcpy(MsgHead, &aMsgHead, sizeof(CMsgHead));
	memcpy(csHead, &aCSHead, sizeof(CCSHead));

	CHAR * pParam = (CHAR *)byMsgData + sizeof(CMsgHead);
	int nParamLen = DataLen - nHeadLen - sizeof(CMsgHead);

	BodyLen = nParamLen;
//	GetDefaultEncoder()->Unpack(CmdID2DataID(aMsgHead.m_sMsgID), (CHAR*)Body, (CHAR*)pParam, nParamLen, 1);
	if ( !DecodeData(aMsgHead.m_sMsgID, (BYTE*)pParam, nParamLen, Body, (UINT*)&BodyLen) )
		return FALSE;
	return TRUE;
}

BOOL CEncryptUtil::Pack_TcpP2pData(BYTE* OutBuffer, int& OutBufferLen, UINT Uin, BYTE* Data, INT DataLen, 
							DWORD Seq, const BYTE* Key, const BYTE* Sign, BYTE SignLen,	
							INT NumPlayer, UINT* PlayerUins, short SrcID, short DestID)
{
	ASSERT(Data);

	//格式化消息头
	CMsgHead aMsgHead;
	aMsgHead.m_sMsgID = 0;
	aMsgHead.m_sMsgType = Request;
	aMsgHead.m_iMsgSeq = Seq;
	aMsgHead.m_bSrcFE = FE_CLIENT;
	aMsgHead.m_bDstFE = FE_GAMESVRD;
	aMsgHead.m_sSrcID = SrcID;
	aMsgHead.m_sDstID = DestID;
	int nLen = 0;
	aMsgHead.Encode((char*)&aMsgHead, &nLen, TRUE);

	//格式化消息体
	BYTE byMsgBuffer[NORMAL_DATA_LEN];
	int nMsgLen = DataLen + sizeof(CMsgHead);
	if( nMsgLen > NORMAL_DATA_LEN )
	{
		ASSERT(FALSE);
		return FALSE;
	}

	::memcpy(byMsgBuffer, &aMsgHead, nLen);
	::memcpy(byMsgBuffer + nLen, Data, DataLen);

	BYTE byCryptOKBuffer[NORMAL_DATA_LEN];
	int nCryptOKLen = ::oi_symmetry_encrypt2_len(nMsgLen);
	::oi_symmetry_encrypt2(byMsgBuffer, nMsgLen, Key, byCryptOKBuffer,&nCryptOKLen);

	INT nOptLen = 3 + NumPlayer * sizeof(int);
	CCSHead aCSHead;
	aCSHead.m_iTotalLength = sizeof(CCSHead) + nOptLen;
	aCSHead.m_nVer = 0;
	aCSHead.m_nDialogID = SrcID;
	aCSHead.m_iSeq = NextSeq();
	aCSHead.m_iUin = Uin;
	aCSHead.m_byBodyFlag = 1;
	aCSHead.m_byOptLength = nOptLen;

	*((short*)(&aCSHead.m_abyOption[0])) = ::htons(ID_TCP_HELP_P2P_SEND_MSG);
	aCSHead.m_abyOption[2] = (char)NumPlayer;
	for (int i = 0; i < NumPlayer; ++i)
	{
		*((int*)(&aCSHead.m_abyOption[3 + i * sizeof(int)])) = ::htonl(PlayerUins[i]);
	}

	int nHeadLen = 0;
	aCSHead.Encode((char*)OutBuffer, &nHeadLen, TRUE);
	memcpy(OutBuffer + sizeof(CCSHead), Sign, SignLen);
	memcpy(OutBuffer + sizeof(CCSHead) + SignLen, byCryptOKBuffer, nCryptOKLen);
	OutBufferLen = sizeof(CCSHead) + SignLen + nCryptOKLen;
	return TRUE;
}
