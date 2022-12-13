#ifndef __SOCK5_DEF_H_GONGWEI__
#define __SOCK5_DEF_H_GONGWEI__

//////////////////////////////////////////////////////////////////////////
// define
//////////////////////////////////////////////////////////////////////////

namespace Tenio
{
// sizeof(SOCK5PROXYPARAM)
//#define SOCK5_PROXY_PARAM_SIZE		562

// defautl proxy port
#define SOCK5_PROXY_DEFAULT_PORT		1080
		
// Sock5 Proxy Version
#define SPV_SOCK4						0x04
#define SPV_SOCK5						0x05

// Sock5 Proxy RSV
#define SPR_RSV							0x00

// Sock5 Proxy Command
#define SPC_CONNECT						0x01	// Proxy请求TCP connect,通讯以TCP方式实现
#define SPC_BIND						0x02	// Bind
#define SPC_UDP_ASSOCIATE				0x03	// Proxy通讯以UDP方式实现
                                    	
// Sock5 Proxy Atyp                 	
#define SPA_IPV4						0x01	// 0x01	IPv4地址
#define SPA_FQDN						0x03	// 0x03 FQDN(全称域名)
#define SPA_IPV6						0x04	// 0x04 IPv6地址
                                    	
// Sock5 Proxy Method               	
#define SPM_NOAUTHENTICATION			0x00	// 不需要验证
#define SPM_GSSAPI						0x01	// GSSAPI
#define SPM_USERNAMEPASSWORD			0x02	// 口令认证机制
#define SPM_IANA_ASSIGNED_BEGIN			0x03
#define SPM_IANA_ASSIGNED_END			0x7F
#define SPM_PRIVATEMETHODS_BEGIN		0x80
#define SPM_PRIVATEMETHODS_END			0xFE
#define SPM_NOACCEPTABLE				0xFF	// 完全不兼容

// Sock5 Proxy Error Code
#define SPEC_SUCCESS					0x00
#define SPEC_UNKNOW_ERROR				0x01
#define SPEC_COMMUNICATION_EXPECTION	0x02
#define SPEC_USERNAME_PASSWORD_ERROR	0x03
#define SPEC_AUTHEN_FAILED				0x04
#define SPEC_CONNECT_FAILED				0x05
#define SPEC_CREATE_FAILED				0x06

// Sock5 Proxy Reply
#define SPP_SUCCESSED					0x00
#define SPP_GENERALFAIL					0x01
#define SPP_CONNECTION_NOT_ALLOW		0x02
#define SPP_NETWORD_UNREACHABLE			0x03
#define SPP_HOST_UNREACHABLE			0x04
#define SPP_CONNECTION_REFUSED			0x05
#define SPP_TTL_EXPIRED					0x06
#define SPP_CMD_NOT_SUPPORTED			0x07
#define SPP_ADDRESS_TYPE_NOT_SUPPORTED	0x08

// sock5 proxy status
typedef enum enumSock5ProxyStatus
{
	SOCK5_START_STATUS = 0,
	SOCK5_STEP1        = 1,
	SOCK5_STEP2        = 2,
	SOCK5_STEP3        = 3,
	SOCK5_SUCCESS      = 4,

	ESPS_NOTPREPARED	= SOCK5_START_STATUS,
	ESPS_CONNECTED		= 1,
	ESPS_CONFERRED		= 2,
	ESPS_RECVUSEINFO	= 3,
	ESPS_REQUESTED		= 4
}SOCK5PROXYSTATUS;

enum SOCKTYPE
{
	SOCK5_CPROXY_TYPE     = 1,
	SOCK5_SUPERPROXY_TYPE = 2,
};
}


#endif // __SOCK5_DEF_H_GONGWEI__