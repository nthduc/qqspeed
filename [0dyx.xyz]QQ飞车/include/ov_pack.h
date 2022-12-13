#ifndef OV_PACK_H
#define OV_PACK_H

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <netinet/in.h>
#endif

#define HTONS(x)		htons((x))
#define HTONL(x)		htonl((x))
#define NTOHS(x)		ntohs((x))
#define NTOHL(x)		ntohl((x))

#define PACK_TYPE_UCHAR		0x01
#define PACK_TYPE_USHORT	0x02
#define PACK_TYPE_ULONG		0x04
#define PACK_TYPE_STR		0x10

struct tagPackItem
{
	int iOffset;
	int iType;
	int iLen;
};

typedef struct tagPackItem	PACKITEM;

struct tagPackInfo
{
	int iCount;
	PACKITEM stItems[1];
};

typedef struct tagPackInfo	PACKINFO;

#ifdef __cplusplus
extern "C"
{
#endif

int ov_pack(char* pszPack, int iPackLen, PACKINFO* pstInfo, char* pszUnpack, int iUnpackLen);
int ov_unpack(char* pszUnpack, int iUnpackLen, PACKINFO* pstInfo, char* pszPack, int iPackLen);

#ifdef __cplusplus
}
#endif

#endif /* OV_PACK_H */
