#include "stdafx.h"
#include "CPing.h"
#include <mmsystem.h>

HANDLE CPing::s_hndlIcmp = NULL;
int CPing::s_nObjectCount = 0;

#pragma comment(lib, "Winmm.lib")


static HANDLE (WINAPI *pIcmpCreateFile)(VOID);
static BOOL (WINAPI *pIcmpCloseHandle)(HANDLE);
static DWORD (WINAPI *pIcmpSendEcho)(HANDLE,DWORD,LPVOID,WORD,PIPINFO,LPVOID,DWORD,DWORD);

CPing::CPing()
{
	if (0 == s_nObjectCount++)
	{	
		// Dynamically load the ICMP.DLL
		s_hndlIcmp = LoadLibrary("ICMP.DLL");
		if (s_hndlIcmp == NULL)
		{
			::MessageBox(NULL, "Could not load ICMP.DLL", "Error:", MB_OK);
			return;
		}
		// Retrieve ICMP function pointers
		pIcmpCreateFile  = (HANDLE (WINAPI *)(void))
			GetProcAddress((HMODULE)s_hndlIcmp,"IcmpCreateFile");
		pIcmpCloseHandle = (BOOL (WINAPI *)(HANDLE))
			GetProcAddress((HMODULE)s_hndlIcmp,"IcmpCloseHandle");
		pIcmpSendEcho = (DWORD (WINAPI *)
			(HANDLE,DWORD,LPVOID,WORD,PIPINFO,LPVOID,DWORD,DWORD))
			GetProcAddress((HMODULE)s_hndlIcmp,"IcmpSendEcho");
		// Check all the function pointers
		if (pIcmpCreateFile == NULL		|| 
			pIcmpCloseHandle == NULL	||
			pIcmpSendEcho == NULL)
		{
			::MessageBox(NULL, "Error loading ICMP.DLL", "Error:", MB_OK);
			FreeLibrary((HMODULE)s_hndlIcmp);
			return;
		}
	}
}

CPing::~CPing()
{
	if (0 == --s_nObjectCount)
	{
		FreeLibrary((HMODULE)s_hndlIcmp);
	}
}

void CPing::GetPingResult(int &nTTL, int &nRTT)
{
	nTTL = m_nTTL; 
	nRTT = m_nRTT; 
}

int CPing::Ping(char *szIP , int nLimitTime)
{
	TRACE("\nping host = %s ", szIP);
//	struct in_addr iaDest;		// Internet address structure
//  LPHOSTENT pHost;			// Pointer to host entry structure
	DWORD dwAddress;			// IP Address
	IPINFO ipInfo;				// IP Options structure
	ICMPECHO icmpEcho;			// ICMP Echo reply buffer
	HANDLE hndlFile;			// Handle for IcmpCreateFile()
/*	
    iaDest.s_addr = inet_addr(strHost);
	if (iaDest.s_addr == INADDR_NONE)
        pHost = gethostbyname(strHost);
    else
        pHost = gethostbyaddr((const char *)&iaDest, 
                        sizeof(struct in_addr), AF_INET);
	if (pHost == NULL)
	{
		return FALSE;
	}

	// Copy the IP address
	dwAddress = (DWORD *)(*pHost->h_addr_list);
*/
//	dwAddress = ::inet_addr("218.30.13.40");
//	wAddress = ::inet_addr("192.168.35.222");
//	dwAddress = ::inet_addr("192.168.35.103");
	dwAddress = ::inet_addr(szIP);

	// Get an ICMP echo request handle        
	hndlFile = pIcmpCreateFile();

	m_nRTT = nLimitTime;
	m_nTTL = 0;

	
	// Set some reasonable default values
	ipInfo.Ttl = 255;
	ipInfo.Tos = 0;
	ipInfo.IPFlags = 0;
	ipInfo.OptSize = 0;
	ipInfo.Options = NULL;
	icmpEcho.Status = 0;
	// Reqest an ICMP echo

	int startTime = timeGetTime();

	DWORD dwRetVal = 
	pIcmpSendEcho(
		hndlFile,		// Handle from IcmpCreateFile()
		dwAddress,		// Destination IP address
		NULL,			// Pointer to buffer to send
		0,				// Size of buffer in bytes
		&ipInfo,		// Request options
		&icmpEcho,		// Reply buffer
		sizeof(struct tagICMPECHO),
		nLimitTime);		// Time to wait in milliseconds
	int endTime = timeGetTime();
	int intervalTime = endTime - startTime;

	if (icmpEcho.Status)
	{
//		ASSERT(FALSE);
		TRACE("\nicmpEcho.Status");
		return FALSE; 
	}

	TRACE("\ndwRetVal = %d", dwRetVal);
	if (dwRetVal)
	{
		m_nRTT = icmpEcho.RTTime;
		m_nTTL = icmpEcho.ipInfo.Ttl;
	}
	TRACE("\nRTT = %d, TTL = %d, interval time = %d", m_nRTT, m_nTTL, intervalTime);

	// Close the echo request file handle
	pIcmpCloseHandle(hndlFile);
	return TRUE;
}
