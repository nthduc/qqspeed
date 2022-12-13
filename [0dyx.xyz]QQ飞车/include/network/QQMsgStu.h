//MsgStruct.H	,消息结构头文件
#pragma once

#pragma  warning(disable:4200)
#pragma  warning(disable:4244)

#define BITMAPLEN 12

const int CMD_LOGIN4			=	0x22;
const int CMD_GET_INFO			=	0x06;
const int CMD_GET_SIGNATURE		=	0x1d;
const int CMD_LOGOUT			=	0x01;
const int CMD_LIST2				=	0x18;
const int CMD_QQ_VALIDATA		=	0x37;

const int CMD_RE_LOGIN4			=	0x22;
const int CMD_RE_GET_INFO		=	0x06;
const int CMD_RE_GET_SIGNATURE	=	0x1d;
const int CMD_RE_LOGOUT			=	0x01;

#pragma pack( push, 1)

typedef struct tagQQSENDHEAD
{
	short	nLength;		//整个协议包的长度
	BYTE	bySTX;			//值为X'02'的一个字节。
	BYTE	byVersion[2];	//协议版本号
	short	nCmd;			//命令号
	short	nSeq;			//包序号
	UINT	Uin;			//QQ号码
}QQ_SEND_HEAD;				//Client->QQ Server 的协议包的头部

typedef struct tagQQ_RECV_HEAD
{
	short	nLength;		//整个协议包的长度
	BYTE	bySTX;			//值为X'02'的一个字节。
	BYTE	byVersion[2];	//协议版本号
	short	nCmd;			//命令号
	short	nSeq;			//包序号

	void Decode()
	{
		nLength = ntohs(nLength);
		nCmd = ntohs(nCmd);
		nSeq = ntohs(nSeq);
	}
}QQ_RECV_HEAD;				//Client->QQ Server 的协议包的头部

typedef struct tagQQ_RELOGIN_HEAD
{
	UINT	Uin;
	DWORD	dwServerIP;
	WORD	nServerPort;
}QQ_RELOGIN_HEAD;			//重新Login QQ的信息            

typedef struct tagQQ_REQUEST_GETSIGN
{
	BYTE	byKey[16];				//Login密钥
	BYTE	byUserServiceBitmap[12];//用户身份标识，表示会员等
	BYTE	byGameSignatureLen;		//签名长度
	BYTE	byGameSignature[128];	//签名
}QQ_REQUEST_GETSIGN;				//取得签名的结构

typedef struct tagQQ_RESPONSE_USERINFO
{
	USHORT	nFace;			//头像 
	BYTE	byAge;			//年龄
	BYTE	byGender;		//性别
	BYTE	byNickLen;		//尼称长度 
	char	szNick[0];		//你称, MAX=32

	void Decode()
	{
		nFace = ntohs(nFace);
	}
}QQ_RESPONSE_USERINFO;

typedef struct tagQQ_RESPONSE_GETSIGN
{
	BYTE	byKey[16];                //Login密钥
	BYTE	byUserServiceBitmap[BITMAPLEN];     //用户身份标识，表示会员等
	DWORD	dwUinFlag;
	BYTE	byGameSignatureLen;        //签名长度
	BYTE	byGameSignature[0];       //签名,MAX=128

	void Decode()
	{
		dwUinFlag = ntohl(dwUinFlag);
	}
}QQ_RESPONSE_GETSIGN;

typedef struct tagQQ_LOGINPARA
{
	BYTE	byPwdHash2[16];         //密码的两次Hash
	BYTE	bySecond;				//为0表示第一次登录,为1表示第二次登录,为2表示仅检查密码正确与否
	ULONG	nLanUdpIP;         //qq serverIP
	WORD	usanUdpPort;        //QQ Server Port
    DWORD	dwBuildVer;
	DWORD	dwOEM;
	DWORD	dwLanguage;
	BYTE	byClientExeHash[16];
	BYTE	byLocalOIcqCount;   //本地已运行OICQ的个数
//	BYTE	byteLocalOIcqIndex;   //该OICQ是本地的第几个OICQ
	BYTE	byOnlineStat;       //登陆后该用户的状态
	BYTE	byComputerGUID[16]; //运行QQ-Client电脑的GUID
}QQ_LOGINPARA;              //Login QQ的参数结构

typedef struct tagQQ_RECMDLOGIN4SUCCESS
{
	BYTE	bySessionKey[16];
	UINT	Uin;
	DWORD	dwClientIP;
	WORD	wClientPort;
	DWORD	dwMsgCenterIP;
	WORD	wMsgCenterPort;
	DWORD	dwServerTime;
	BYTE	charClientToClientMinVerMain;
	BYTE	charClientToClientMinVerSub;
	BYTE	arSignature[24];
	DWORD	dwChannelServerIP;//DWORD dwInfoCenterIP;
	WORD	wChannelServerPort;//WORD wInfoCenterPort;
	DWORD	dwMsgDlgAdServerIP;
	WORD	wMsgDlgAdServerPort;
	//BYTE	charUinLevel;
	WORD	wUinLevel;

	BYTE	arValueAddedSignature[32]; //v7.20新增
	BYTE	arValueAddedUserBitmap[12]; //v7.20新增
	DWORD	dwLastLoginIP;
	DWORD	dwLastLoginTime;
	DWORD	dwLastModifyInfoTime;
	DWORD	dwLastModifyListTime;
}QQ_RECMDLOGIN4SUCCESS;

typedef struct tagQQ_REPONSE_MSGHEAD
{
	BYTE byType; 
	BYTE byErrorCode;
}QQ_REPONSE_MSGHEAD;

#pragma pack(pop)
	