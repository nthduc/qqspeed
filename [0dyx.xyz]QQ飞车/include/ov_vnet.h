#ifndef OV_VNET_H
#define OV_VNET_H

#include "ov_os.h"

#define VNET_WAIT_INFINITE		WAIT_INFINITE

#define VNET_CHAR_ARG			'?'
#define VNET_CHAR_EQ			'='
#define VNET_CHAR_SEP			'&'
#define VNET_CHAR_PORT			':'
#define VNET_CHAR_ESC			'%'

#define VNET_STR_PROTO			"://"
#define VNET_STR_NUMBER			"1234567890."

#define VNET_PROTO_TCP			"tcp"
#define VNET_PROTO_UDP			"udp"
#define VNET_PROTO_UNIX			"unix"
#define VNET_PROTO_PIPE			"pipe"
#define VNET_PROTO_FILE			"file"
#define VNET_PROTO_SHM			"shm"

#define VNET_ID_TCP			1
#define VNET_ID_UDP			2
#define VNET_ID_UNIX			3
#define VNET_ID_PIPE			4
#define VNET_ID_FILE			5
#define VNET_ID_SHM			6

#define VNET_ARG_REUSE			"reuse"
#define VNET_ARG_TYPE			"type"

#define VNET_TYPE_STREAM		"stream"
#define VNET_TYPE_DGRAM			"dgram"
#define VNET_TYPE_SEQPACKET		"seqpacket" 
#define VNET_TYPE_RDM			"rdm"
#define VNET_TYPE_RAW			"raw"
#define VNET_TYPE_PACKET		"packet"

#define VNET_DEF_PROTO			VNET_PROTO_TCP

#define HTONS(x)			htons((x))
#define HTONL(x)			htonl((x))
#define NTOHS(x)			ntohs((x))
#define NTOHL(x)			ntohl((x))

#define VNET_PROTO_LEN			16
#define VNET_ADDR_LEN			128
#define VNET_NAME_LEN			32
#define VNET_VALUE_LEN			256
#define VNET_MAX_ARGS			16
#define VNET_BUFF_LEN			256

struct tagVnetArg
{
	char szName[VNET_NAME_LEN];
	char szValue[VNET_VALUE_LEN];
};

typedef struct tagVnetArg		VNETARG;

struct tagVnetOpt
{
	char szProto[VNET_PROTO_LEN];
	char szAddr[VNET_ADDR_LEN];
	int iArgs;
	VNETARG	args[VNET_MAX_ARGS];
};

typedef struct tagVnetOpt		VNETOPT;

struct tagVnetProtoDef
{
	char* pszName;
	int iID;
	int fSelectable;
	int fAvail;
};

typedef struct tagVnetProtoDef		VNETPROTODEF;

struct tagVnetTypeDef
{
	char* pszType;
	int iType;
};

typedef struct tagVnetTypeDef		VNETTYPEDEF;

union tagVnetAddr
{
	struct sockaddr_in stIn;

#ifndef WIN32
	struct sockaddr_un stUn;
#endif /* WIN32 */
};

typedef union tagVnetAddr		VNETADDR;

#ifdef __cplusplus
extern "C"
{
#endif

int vnet_is_number(const char* pszHost);

int vnet_str2inet(const char* pszAddr, struct sockaddr_in* pstIn);
int vnet_str2unix(const char* pszAddr, struct sockaddr_un* pstUn);

/* helper used internal. */
int vnet_decode(char* pszDst, int* piDst, const char* pszSrc, int iSrc);
int vnet_encode(char* pszDst, int* piDst, const char* pszSrc, int iSrc);

const VNETPROTODEF* vnet_find_proto(const char* pszProto);
const VNETTYPEDEF* vnet_find_type(const char* pszType);
const VNETARG* vnet_find_arg(VNETOPT* pstOpt, const char* pszName);

int vnet_get_opt(VNETOPT* pstOpt, const char* pszUri);		/* get the option from the uri string, this is a helper, needn't use normally. */

/* some flags can set. */
int vnet_set_nonblock(int s, int fNonblock);			/* set the socket to be nonblock or not. */

/* create/destroy the socket. */
int vnet_open(const char* pszUri);				/* create a socket use uri info. */
int vnet_listen(const char* pszUri, int iBacklog);		/* create a socket for listen. */
int vnet_connect(const char* pszUri, int iTimeout);		/* create a socket, and connect to the uri specified. */
int vnet_accept(int s, struct sockaddr* pstAddr, int* piLen, int iTimeout); /* accept a income connection. */
int vnet_close(int s);						/* close a socket. */

/* send/recv socket operations. */
int vnet_send(int s, const char* pszBuff, int iLen, int iTimeout);
int vnet_recv(int s, char* pszBuff, int iLen, int iTimeout);

/* sendall/recvall socket operations. */
int vnet_sendall(int s, const char* pszBuff, int iLen, int iTimeout);
int vnet_recvall(int s, char* pszBuff, int iLen, int iTimeout);

/* read/write operations. */
int vnet_write(int s, const char* pszBuff, int iLen);
int vnet_read(int s, char* pszBuff, int iLen);

#ifdef __cplusplus
}
#endif

#endif /* OV_VNET_H */
