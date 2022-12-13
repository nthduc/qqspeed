#ifndef __QQLOGINSTORAGESTRUCTS_H__
#define __QQLOGINSTORAGESTRUCTS_H__

#include "networkdef.h"

#define TENIO_QQLOGINCONFIG_HASHLEN		16
#define USERLOGINCONFIG_HISTORYCOUNT	20
//#define QQLO_MAXPROXYHOSTLEN			128
//#define QQLO_MAXPROXYUSERNAMELEN		128
//#define QQLO_MAXPROXYPASSWORDLEN		128
//#define QQLO_MAXPROXYDOMAINLEN			128

namespace Tenio
{
	typedef struct _tagQQLOGINCONFIG
	{
		USHORT ushStructSize;	// �ṹ���С
		DWORD  dwUIN;			// QQ����
		DWORD  dwLastLoginTime;	// ����¼ʱ��
		BOOL   bIncog;			// �Ƿ�����
		BOOL   bPasswordValid;	// �����arbyPasswordHashed�Ƿ���Ч
		BYTE   arbyPasswordHashed[TENIO_QQLOGINCONFIG_HASHLEN]; // ����MD5 Hash������
	}USERLOGINCONFIG, *PUSERLOGINCONFIG;

	typedef struct _tagUSERLOGINCONFIGLIST
	{
		USHORT ushStructSize;
		USHORT ushLoginInfoCount;	// �û���Ϣ����, ÿ���û�����Ϣ����������� arstLoginInfos
		USERLOGINCONFIG arstLoginInfos[USERLOGINCONFIG_HISTORYCOUNT];
	}USERLOGINCONFIGLIST, *PUSERLOGINCONFIGLIST;
};

#endif