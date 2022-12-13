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
		USHORT ushStructSize;	// 结构体大小
		DWORD  dwUIN;			// QQ号码
		DWORD  dwLastLoginTime;	// 最后登录时间
		BOOL   bIncog;			// 是否隐身
		BOOL   bPasswordValid;	// 下面的arbyPasswordHashed是否有效
		BYTE   arbyPasswordHashed[TENIO_QQLOGINCONFIG_HASHLEN]; // 经过MD5 Hash的密码
	}USERLOGINCONFIG, *PUSERLOGINCONFIG;

	typedef struct _tagUSERLOGINCONFIGLIST
	{
		USHORT ushStructSize;
		USHORT ushLoginInfoCount;	// 用户信息个数, 每个用户的信息保存在下面的 arstLoginInfos
		USERLOGINCONFIG arstLoginInfos[USERLOGINCONFIG_HISTORYCOUNT];
	}USERLOGINCONFIGLIST, *PUSERLOGINCONFIGLIST;
};

#endif