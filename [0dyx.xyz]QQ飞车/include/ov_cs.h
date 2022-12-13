#ifndef OV_CS_H
#define OV_CS_H

#include "ov_os.h"
#include "ov_dr.h"
#include "ov_cproto.h"

#define CS_MAKE_VERSION(m,n)	((m)*100 + (n))

#define CS_VERSION			CS_MAKE_VERSION(CS_MAJOR,CS_MINOR)


#define CS_SVR				"CSSvrPkgBody"
#define CS_CLT				"CSCltPkgBody"
#define CS_QQ				"CSQQInfo"

/* used for qqinfo. */

#define CSQQE_ERROR			-1
#define CSQQE_CONN			-2
#define CSQQE_DECRYPT		-3
#define CSQQE_PASSWD		-4

#define CS_DNS_MAJOR		"tcpconn.tencent.com"
#define CS_DNS_MINOR		"tcpconn%d.tencent.com"

#define CS_CHARS			"ABCDEFGHIJKMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz23456789"

#define CS_MAX_TIME_LEN		64

#define CS_MAX_QQINFO_SIZE	256
#define CS_BUFF_SIZE		65536

#define CS_PWORLD_BASE		CS_PWORLD_MAPID_BASE
#define CS_PWORLD_MAPID		100

#define CS_GET_PWORLDID(MapID)	\
	((MapID) & 0xffff) < CS_PWORLD_BASE ? 0 : ( (((MapID) & 0xffff) - CS_PWORLD_BASE)/CS_PWORLD_MAPID )

#define CS_MAKE_MAPINDEX(MapID, PworldInst)		( (MapID) | ((PworldInst)<<16) )
#define CS_GET_MAPID(MapIndex)					( (MapIndex) & 0xffff )
#define CS_GET_PWORLDINST(MapIndex)				( (MapIndex) >> 16 )

#define CS_INIT_HEAD(pstHead)		memset((pstHead), 0, sizeof(CSPKGHEAD))
#define CS_GET_PKG_TYPE(pstPkg)		(pstPkg)->stHead.nCmd

typedef int (*LPFNCHGKEY)(CSCHGKEYS* pstChgKeyS, char* pszBuff, void* pvArg);

struct tagCSMapper
{
	int iMax;
	LPDRMETA* ppstMapper;
};

typedef struct tagCSMapper	CSMAPPER;
typedef struct tagCSMapper	*LPCSMAPPER;

struct tagCSDesc
{
	DRMETALIB* pstLib;
	DRMETA* pstQQ;
	DRMETA* pstSinfo;
	CSMAPPER stSvr;
	CSMAPPER stClt;
};

typedef struct tagCSDesc	CSDESC;
typedef struct tagCSDesc	*LPCSDESC;

struct tagCSAccount
{
	long lUin;
};

typedef struct tagCSAccount	CSACCOUNT;
typedef struct tagCSAccount*	LPCSACCOUNT;

struct tagCSConn
{
	int s;			/* socket. */
	int iData;
	int iHeadLen;
	int iBodyLen;
	char szKey[CS_KEY_LEN];
	char szBuff[CS_BUFF_SIZE];/* buffer for recv data. */
	int iVersion;
	LPFNCHGKEY pfnChgKey;
	void* pvArg;
};

typedef struct tagCSConn	CSCONN;
typedef struct tagCSConn	*LPCSCONN;

struct tagIdx2Class
{
	int iType;
	int iClass;
	int iSub;
	int iMin;
	int iMax;
	int iCol;
};

typedef struct tagIdx2Class		IDX2CLASS;
typedef struct tagIdx2Class		*LPIDX2CLASS;

#ifdef __cplusplus
extern "C"
{
#endif

IDX2CLASS* GetClassFromIndex(int iIndex);

int cs_load_file(const char* pszFile);
int cs_load_fp(FILE* fp);

int cs_unload(void);

DRMETA* cs_get_meta_by_name(const char* pszName);
DRMETA* cs_get_meta_by_id(int iID);

#define cs_pack				dr_hton
#define cs_unpack			dr_ntoh

/*
int cs_pack(DRMETA* pstMeta, DRDATA* pstNet, DRDATA* pstHost, int iVersion);
int cs_unpack(DRMETA* pstMeta, DRDATA* pstHost, DRDATA* pstNet, int iVersion);
*/

int cs_get_siginfo_by_uri(CSSIGINFO* pstSigInfo, const char* pszUri, char* pszErr, int iBuf, int iTimeout, void(*pCallBack)(int));
int cs_get_siginfo(CSSIGINFO* pstSigInfo, char* pszErr, int iBuf, int iTimeout, void(*pCallBack)(int) );
int cs_set_siginfo_ident(CSSIGINFO* pstSigInfo, char* pchIdent, int iLen);

/* for server package, return -1 or size. */
int cs_spack(int iMsg, char* pszNet, void* pvHost, int iVersion);
int cs_sunpack(int iMsg, void* pvHost, char* pszNet, int iNet, int iVersion);
int cs_sfprintf(int iMsg, void* pvHost, FILE* fp, int iVersion);
int cs_ssprintf(int iMsg, void* pvHost, char* pszBuff, int* piBuff, int iVersion);
int cs_soutput(int iMsg, void* pvHost, char* pszXml, int* piXml, int iVersion);

/* for client package, return -1 or size. */
int cs_cpack(int iMsg, char* pszNet, void* pvHost, int iVersion);
int cs_cunpack(int iMsg, void* pvHost, char* pszNet, int iNet, int iVersion);
int cs_cfprintf(int iMsg, void* pvHost, FILE* fp, int iVersion);
int cs_csprintf(int iMsg, void* pvHost, char* pszBuff, int* piBuff, int iVersion);
int cs_coutput(int iMsg, void* pvHost, char* pszXml, int* piXml, int iVersion);

int cs_get_pkglen(CSSVRPKG* pstPkg);
LPCSSVRPKGBODY cs_get_pkgbody(CSSVRPKG* pstPkg);

int cs_connect(LPCSCONN* ppstConn, const char* pszUri, int iTimeout); /* tcp://172.16.4.128:6610 */
int cs_disconn(LPCSCONN pstConn);

void cs_set_key(LPCSCONN pstConn, const char* pszKey);

int cs_cmd_login(LPCSCONN pstConn, CSSIGINFO* pstSigInfo, int iTimeout, unsigned char ucCltFlag);
int cs_cmd_chgzone(LPCSCONN pstConn, CSSIGINFO* pstSigInfo, CSCHGZONEC* pstChgZone, int iTimeout);

int cs_send_cmd(LPCSCONN pstConn, int iCmd, CSCLTPKGBODY* pstBody, int iTimeout, int iOpSec, int iOpUSec);
int cs_send_cmd_plain(LPCSCONN pstConn, int iCmd, CSCLTPKGBODY* pstBody, int iTimeout, int iOpSec, int iOpUSec);
int cs_recv_cmd(LPCSCONN pstConn, CSSVRPKG* pstPkg, int iTimeout);
int cs_recv_cmd_plain(LPCSCONN pstConn, CSSVRPKG* pstPkg, int iTimeout);

#ifdef __cplusplus
}
#endif

#endif /* OV_CS_H */
