#include "stdafx.h"
#include "utils.h"
#include "fileop.h"
#include "common.h"

#define MAX_SVRCFG	100

void LoadServerCFGFromFile(const char* szProfile, SOCKET_CREATE_INFO_LIST& listSocketInfo)
{
	const char* szApp = "ServerInfo";
	char strApp[MAX_PATH] = "";
	strncpy(strApp, szApp, MAX_PATH - 1);
	for ( int i = 1; i < MAX_SVRCFG; i++ )
	{
		sprintf(strApp, "%s%d", szApp, i);
		char szHostName[MAX_PATH];
		GetPrivateProfileString(strApp, "Name", "", szHostName, MAX_PATH, szProfile);
		char szHostIP[MAX_PATH];
		GetPrivateProfileString(strApp, "IP", "", szHostIP, MAX_PATH, szProfile);
		char szPorts[MAX_PATH];
		if ( strlen(szHostName) == 0 && strlen(szHostIP) == 0 )
		{
			break;
		}

		GetPrivateProfileString(strApp, "Ports", "", szPorts, MAX_PATH, szProfile);
		UINT uPorts[3] = { 0 };
		if ( strlen(szPorts) > 0 )
		{
			sscanf(szPorts, "%d,%d,%d", &uPorts[0], &uPorts[1], &uPorts[2]);
		}
		char szUDPPorts[MAX_PATH];
		GetPrivateProfileString(strApp, "UDPPorts", "", szUDPPorts, MAX_PATH, szProfile);
		UINT uUDPPorts[3] = { 0 };
		if ( strlen(szUDPPorts) > 0 )
		{
			sscanf(szUDPPorts, "%d,%d,%d", &uUDPPorts[0], &uUDPPorts[1], &uUDPPorts[2]);
		}
		SOCKET_CREATE_INFO kSocketInfo;
		strncpy(kSocketInfo.szHostName, szHostName, 100);
		strncpy(kSocketInfo.szHostAddr, szHostIP, 100);
		kSocketInfo.nRcvBufSize = RCV_BUF_LEN;
		kSocketInfo.nSndBufSize = SND_BUF_LEN;
		memcpy(kSocketInfo.uPorts, uPorts, sizeof(UINT) * 3);
		memcpy(kSocketInfo.uUDPPorts, uUDPPorts, sizeof(UINT) * 3);
		kSocketInfo.uServerID = GetPrivateProfileInt(szApp, "ServerID", 0, szProfile);
		listSocketInfo.push_back(kSocketInfo);

	}
}

BOOL LoadServerCFG(ServerEntity Entity, SOCKET_CREATE_INFO_LIST& listSocketInfo)
{
	char* pszTmp = NULL;
	switch(Entity)
	{
	case eQQServer:
		pszTmp = "QQSvrInfo.ini";
		break;
	case eDirServer:
		pszTmp = "DirSvrInfo.ini";
		break;
	case eGameServer:
		pszTmp = "GameSvrInfo.ini";
		break;
	case eShopServer:
		pszTmp = "ShopSvrInfo.ini";
		break;
	default:
		return FALSE;
	}

	char szProfile[MAX_PATH] = "";
	if ( !_fullpath(szProfile, pszTmp, MAX_PATH) )
		return FALSE;

	if ( !FileExists(szProfile) )
		return FALSE;

	LoadServerCFGFromFile(szProfile, listSocketInfo);
	return TRUE;
}

const char* GetAddrByName(const char* Name)
{
	hostent* ent = gethostbyname(Name);
	if ( !ent )
	{
		LogText("gethostbyname : %s failed!", Name);
		return NULL;
	}
	return inet_ntoa(*(in_addr*)ent->h_addr);
}