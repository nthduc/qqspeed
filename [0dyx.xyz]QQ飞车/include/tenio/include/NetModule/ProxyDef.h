#ifndef __PROXY_DEF_H_GONGWEI__
#define __PROXY_DEF_H_GONGWEI__

/**
 * @file ProxyDef.h
 */
#include "Sock5Def.h"

	/**
	 * @brief �������������
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
	 * @brief �����������Ϣ
	 */
	typedef struct tagProxyParam
	{
		int     nProxyType;				///< �������ͣ��μ� PROXY_TYPE
		TCHAR	szProxyIP[32];			///< ���������IP��e.g. ""172.30.11.11"
		USHORT	usProxyPort;			///< ����������˿�
		TCHAR	szUserName[MAX_PATH];	///< ����������û���
		TCHAR	szPassword[MAX_PATH];	///< �û�����
		TCHAR   szDomain[MAX_PATH];     ///< http ��������������򡱵�ȡֵ
		int		nSpc;					///< proxy type,SPC_CONNECT for tcp,SPC_UDP_ASSOCIATE UDP
		int		nSpa;					///< must be SPA_IPV4
	}PROXYPARAM, *PPROXYPARAM, HTTPPROXYPARAM, *PHTTPPROXYPARAM, SOCK5PROXYPARAM, *PSOCK5PROXYPARAM;	


#endif // __HTTP_DEF_H_GONGWEI__