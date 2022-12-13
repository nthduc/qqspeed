#ifndef __SOCKET_OBJECT_DEFINE_H_GONGWEI__
#define __SOCKET_OBJECT_DEFINE_H_GONGWEI__

//////////////////////////////////////////////////////////////////////////
// define
//////////////////////////////////////////////////////////////////////////

// default socket buffer length
#define DEFAULT_SOCKET_BUFFER_LENGTH				1024

// socket event ID
#define SOCKET_EVENT								(0x3333 + 0x11)

// �ӳٶ�ģʽ
#define SOCKETMODE_DELAYRECEIVE						(0x3333 + 0x11)

// socket windows class name
#define SOCKET_CLASSNAME							_T("QQGAME_SOCKET_BASE_CLASS_NAME")

//////////////////////////////////////////////////////////////////////////
// typedef
//////////////////////////////////////////////////////////////////////////

// socket status
typedef enum enumSocketStatus
{
	ESS_NOTPREPARED		= 0,
	ESS_NOTCONNECTED	= 1,
	ESS_CONNECTTING		= 2,
	ESS_ISCONNECTED		= 3,
	ESS_DISCONNECTTING	= 4
}ENUMSOCKETSTATUS;

/*
typedef enum enumNETWORKPROXYTYPE
{
	EPT_NOTSET = 0,
	EPT_SOCKS5PROXY = 1,
	EPT_HTTPPROXY = 2
}ENUMNETWORKPROXYTYPE;

typedef struct _tagGLOBALNETWORKPROXYSETTING
{
	USHORT ushStructSize;

	BOOL bUseProxy;
	ENUMNETWORKPROXYTYPE nProxyType;
	
	USHORT ushProxyPort;
	TCHAR szProxyHost[QQLO_MAXPROXYHOSTLEN];
	TCHAR szProxyUserName[QQLO_MAXPROXYUSERNAMELEN];
	TCHAR szProxyPassword[QQLO_MAXPROXYPASSWORDLEN];
	TCHAR szProxyDomain[QQLO_MAXPROXYDOMAINLEN];

}GLOBALNETWORKPROXYSETTING, *PGLOBALNETWORKPROXYSETTING;
*/

// socket type
typedef enum enumSocketType
{
	EST_RAWTCP					= 0,
	EST_RAWUDP					= 1,
	EST_SOCK5TCPPROXY			= 2,
	EST_SOCK5UDPPROXY			= 3,
	EST_HTTPPROXY				= 4,
	EST_SPEEDLIMITRAWTCP		= 5,
	EST_SPEEDLIMITSOCK5PROXYTCP	= 6,
	EST_SPEEDLIMITHTTPPROXYTCP	= 7
	
}ENMSOCKETTYPE;

#define SOCKETTYPE_TYPECOUNT				5
#define SOCKETTYPE_RAWTCP					1
#define SOCKETTYPE_RAWUDP					2
#define SOCKETTYPE_SOCK5PROXYTCP			3
#define SOCKETTYPE_SOCK5PROXYUDP			4
#define SOCKETTYPE_HTTPPROXYTCP				5
// #define SOCKETTYPE_SPEEDLIMITSOCK5PROXYTCP	6
// #define SOCKETTYPE_SPEEDLIMITHTTPPROXYTCP	7
// #define SOCKETTYPE_SPEEDLIMITRAWTCP			8



// socket param for ISocketObject::Create()
typedef struct tagSocketParam
{
	ENMSOCKETTYPE		eSocketType;
	LPVOID				lpProxyParam;
}SOCKETPARAM;

#endif // __SOCKET_OBJECT_DEFINE_H_GONGWEI__