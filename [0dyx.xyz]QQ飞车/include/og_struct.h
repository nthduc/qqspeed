
#ifndef _OG_STRUCT_H
#define _OG_STRUCT_H

#include <time.h>
#include "og_pkg.h"

#define MAX_CLIENT	3010

typedef struct {
	int iSock;
	int iPos;
	int iRecvLen;
	time_t lLost;
	time_t lActive;
	int iID;
	unsigned long lIP;
	unsigned short shPort;
	char cStat;
	char sVersion[2];
	char sKey[16];
	char sReserve[71];
	CldPkg	stPkg;
} Client;

typedef struct {
	int iMaxClient;
	int iFreeHead;
	int iFreeTail;
	int aiFreeClient[MAX_CLIENT];
	char sReserve[1024];
	Client astClient[MAX_CLIENT];
} ZoneConnectShm;

typedef struct {
	char sFace[2];
	char cAge;
	char cGender;
	char cNickLen;
	char sNick[13];
} QQInfo;

#endif
