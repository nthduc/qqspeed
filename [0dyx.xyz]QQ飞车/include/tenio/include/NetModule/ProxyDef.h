#ifndef __PROXY_DEF_H_GONGWEI__
#define __PROXY_DEF_H_GONGWEI__

/**
 * @file ProxyDef.h
 */
#include "Sock5Def.h"

	/**
	 * @brief 代理服务器类型
	 */
	typedef enum enumProxyType
	{
		NO_PROXY_TYPE	        = 0,
		SOCK5_PROXY_TYPE	    = 1,
		HTTP_PROXY_TYPE		    = 3,
		UDP_NORMAL_PROXY_TYPE	= 4,
		UDP_SOCK5_PROXY_TYPE	= 5
	}PROXY_TYPE;
	
	/**
	 * @brief 代理服务器信息
	 */
	typedef struct tagProxyParam
	{
		int     nProxyType;				///< 代理类型，参见 PROXY_TYPE
		TCHAR	szProxyIP[32];			///< 代理服务器IP，e.g. ""172.30.11.11"
		USHORT	usProxyPort;			///< 代理服务器端口
		TCHAR	szUserName[MAX_PATH];	///< 代理服务器用户名
		TCHAR	szPassword[MAX_PATH];	///< 用户密码
		TCHAR   szDomain[MAX_PATH];     ///< http 代理服务器，“域”的取值
		int		nSpc;					///< proxy type,SPC_CONNECT for tcp,SPC_UDP_ASSOCIATE UDP
		int		nSpa;					///< must be SPA_IPV4
	}PROXYPARAM, *PPROXYPARAM, HTTPPROXYPARAM, *PHTTPPROXYPARAM, SOCK5PROXYPARAM, *PSOCK5PROXYPARAM;	


#endif // __HTTP_DEF_H_GONGWEI__