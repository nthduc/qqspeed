
#ifndef _OG_PKG_H
#define _OG_PKG_H

#define MAX_CLD_PKG_DATA 16384
#define MAX_SVR_PKG_DATA 49152
#define MAX_CHANNEL_PKG_DATA 65536

typedef struct {
	char sHeadLen[2];
	char sBodyLen[2];
	char sCmd[2];
	char sReserve[8];
	char sHeadCheckSum[2];
} CldPkgHead;

typedef struct {
	CldPkgHead	stHead;
	char sData[MAX_CLD_PKG_DATA];
} CldPkg;

typedef struct {
	CldPkgHead	stHead;
	char sData[MAX_SVR_PKG_DATA];
} SvrPkg;

typedef struct {
	char sHeadLen[2];
	char sBodyLen[2];
	char sCmd[2];
	char sSeq[4];
	char sReserve[4];
	char sHeadCheckSum[2];
} ChannelPkgHead;

typedef struct {
	ChannelPkgHead stHead;
	char sData[MAX_CHANNEL_PKG_DATA];
} ChannelPkg;

#endif
