//MsgStruct.H	,��Ϣ�ṹͷ�ļ�
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
	short	nLength;		//����Э����ĳ���
	BYTE	bySTX;			//ֵΪX'02'��һ���ֽڡ�
	BYTE	byVersion[2];	//Э��汾��
	short	nCmd;			//�����
	short	nSeq;			//�����
	UINT	Uin;			//QQ����
}QQ_SEND_HEAD;				//Client->QQ Server ��Э�����ͷ��

typedef struct tagQQ_RECV_HEAD
{
	short	nLength;		//����Э����ĳ���
	BYTE	bySTX;			//ֵΪX'02'��һ���ֽڡ�
	BYTE	byVersion[2];	//Э��汾��
	short	nCmd;			//�����
	short	nSeq;			//�����

	void Decode()
	{
		nLength = ntohs(nLength);
		nCmd = ntohs(nCmd);
		nSeq = ntohs(nSeq);
	}
}QQ_RECV_HEAD;				//Client->QQ Server ��Э�����ͷ��

typedef struct tagQQ_RELOGIN_HEAD
{
	UINT	Uin;
	DWORD	dwServerIP;
	WORD	nServerPort;
}QQ_RELOGIN_HEAD;			//����Login QQ����Ϣ            

typedef struct tagQQ_REQUEST_GETSIGN
{
	BYTE	byKey[16];				//Login��Կ
	BYTE	byUserServiceBitmap[12];//�û���ݱ�ʶ����ʾ��Ա��
	BYTE	byGameSignatureLen;		//ǩ������
	BYTE	byGameSignature[128];	//ǩ��
}QQ_REQUEST_GETSIGN;				//ȡ��ǩ���Ľṹ

typedef struct tagQQ_RESPONSE_USERINFO
{
	USHORT	nFace;			//ͷ�� 
	BYTE	byAge;			//����
	BYTE	byGender;		//�Ա�
	BYTE	byNickLen;		//��Ƴ��� 
	char	szNick[0];		//���, MAX=32

	void Decode()
	{
		nFace = ntohs(nFace);
	}
}QQ_RESPONSE_USERINFO;

typedef struct tagQQ_RESPONSE_GETSIGN
{
	BYTE	byKey[16];                //Login��Կ
	BYTE	byUserServiceBitmap[BITMAPLEN];     //�û���ݱ�ʶ����ʾ��Ա��
	DWORD	dwUinFlag;
	BYTE	byGameSignatureLen;        //ǩ������
	BYTE	byGameSignature[0];       //ǩ��,MAX=128

	void Decode()
	{
		dwUinFlag = ntohl(dwUinFlag);
	}
}QQ_RESPONSE_GETSIGN;

typedef struct tagQQ_LOGINPARA
{
	BYTE	byPwdHash2[16];         //���������Hash
	BYTE	bySecond;				//Ϊ0��ʾ��һ�ε�¼,Ϊ1��ʾ�ڶ��ε�¼,Ϊ2��ʾ�����������ȷ���
	ULONG	nLanUdpIP;         //qq serverIP
	WORD	usanUdpPort;        //QQ Server Port
    DWORD	dwBuildVer;
	DWORD	dwOEM;
	DWORD	dwLanguage;
	BYTE	byClientExeHash[16];
	BYTE	byLocalOIcqCount;   //����������OICQ�ĸ���
//	BYTE	byteLocalOIcqIndex;   //��OICQ�Ǳ��صĵڼ���OICQ
	BYTE	byOnlineStat;       //��½����û���״̬
	BYTE	byComputerGUID[16]; //����QQ-Client���Ե�GUID
}QQ_LOGINPARA;              //Login QQ�Ĳ����ṹ

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

	BYTE	arValueAddedSignature[32]; //v7.20����
	BYTE	arValueAddedUserBitmap[12]; //v7.20����
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
	