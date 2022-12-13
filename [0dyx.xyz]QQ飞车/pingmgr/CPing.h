#ifndef __CPING_H__
#define __CPING_H__

#include "stdafx.h"
#include "winsock.h"


typedef struct tagIPINFO
{
	u_char Ttl;				// Time To Live
	u_char Tos;				// Type Of Service
	u_char IPFlags;			// IP flags
	u_char OptSize;			// Size of options data
	u_char FAR *Options;	// Options data buffer
}IPINFO;

typedef IPINFO* PIPINFO;

typedef struct tagICMPECHO
{
	u_long Source;			// Source address
	u_long Status;			// IP status
	u_long RTTime;			// Round trip time in milliseconds
	u_short DataSize;		// Reply data size
	u_short Reserved;		// Unknown
	void FAR *pData;		// Reply data buffer
	IPINFO	ipInfo;			// Reply options
}ICMPECHO;

typedef ICMPECHO* PICMPECHO;

class CPing
{
public:
	CPing();
	~CPing();
	int Ping(char *szIP , int nLimitTime);

	void GetPingResult(int &nTTL, int &nRTT);
	int m_nTTL, m_nRTT;

protected:
	// ICMP.DLL Export Function Pointers
	static HANDLE s_hndlIcmp;			
	static int s_nObjectCount;

};


#endif