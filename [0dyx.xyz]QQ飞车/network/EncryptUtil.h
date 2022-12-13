#pragma once

#include "network/msg.h"
#include "network/QQMsgStu.h"

struct CMsgHead;
struct CCSHead;
class CEncryptUtil
{
public:
	static void PackQQSendData(UINT Uin, const BYTE* Pwd, int PwdType,
		int Cmd, USHORT Seq,
		BYTE* OutBuffer, short& OutBufferLen, BYTE* Key);

	static BOOL UnPackQQRecvData(const BYTE* Data, int DataLen, 
		const BYTE* Key, 
		QQ_RECV_HEAD* RecvHead,
		BYTE* OutBuffer, int& OutBufferLen);

	static BOOL Pack(BYTE* OutBuffer, int& OutBufferLen, UINT Uin, int DataID, BYTE* Data, DWORD Seq,
		const BYTE* Key, const BYTE* Sign, BYTE SignLen,	
		short MsgID, short MsgType, char SrcFE, char DestFE, short SrcID, short DestID);

	static BOOL Unpack(const BYTE* Data, int DataLen, const BYTE* Key,
		CCSHead* csHead, CMsgHead* MsgHead, BYTE* Body, int& BodyLen);

	static BOOL Pack_TcpP2pData(BYTE* OutBuffer, int& OutBufferLen, UINT Uin, BYTE* Data, INT DataLen, 
		DWORD Seq, const BYTE* Key, const BYTE* Sign, BYTE SignLen,	
		INT NumPlayer, UINT* PlayerUins, short SrcID, short DestID);
};
