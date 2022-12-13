#ifndef __NETHELP_H__
#define __NETHELP_H__

#include "winsock2.h"

inline CString GetLocalIP(DWORD* pIP /*= NULL*/)
{
	CString s = "";
	//char sHostname[256];
	hostent *lpHost;
	SOCKADDR_IN sLocalSockAddr;

	char sHostname[256];
	CString strTempHostName;

	if(gethostname(sHostname,256))
	{
		if (pIP)
			*pIP = 0;
		return s;
	}
	
	strTempHostName = sHostname;
	
	lpHost = gethostbyname(strTempHostName);
	
	if(!lpHost)
	{
		if (pIP)
			*pIP = 0;
		return s;
	}

	int i = 0;
	for(i = 0; lpHost!= NULL && lpHost->h_addr_list[i]!= NULL; i++ );
	if(i > 1)
	{
		for(i = 0; lpHost!= NULL && lpHost->h_addr_list[i]!= NULL; i++ )
		{
			SOCKADDR_IN sLocalSockAddr;
			sLocalSockAddr.sin_addr.s_addr = *((unsigned long *)lpHost->h_addr_list[i]);
			
			s = inet_ntoa(sLocalSockAddr.sin_addr);
			
			if(s == "0.0.0.0" || s.Left(3) == "169")
			{
				//ÅÅ³ý 0.0.0.0 »ò 169.x.x.x
				continue;
			}
			
			if (pIP)
				*pIP = sLocalSockAddr.sin_addr.S_un.S_addr;
			break;
		}
	}
	else
	{
		//just one Ip:
		sLocalSockAddr.sin_addr.s_addr = *((unsigned long *)lpHost->h_addr);
		s = inet_ntoa(sLocalSockAddr.sin_addr);
		
		if (pIP)
			*pIP = sLocalSockAddr.sin_addr.S_un.S_addr;
	}

	return s;
}

#endif