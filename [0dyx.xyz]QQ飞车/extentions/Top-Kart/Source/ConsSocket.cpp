// ConsSocket.cpp: implementation of the CConsSocket class.
//
//////////////////////////////////////////////////////////////////////
#include "Top-KartPCH.h"
#include "ConsSocket.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WSADATA CConsSocket::m_stWsaData;
char*   CConsSocket::m_scLocalIP="127.0.0.1";
int     CConsSocket::m_nPort=6668;

CConsSocket::CConsSocket()
{
 
}

CConsSocket::~CConsSocket()
{

}

void CConsSocket::StartUp()
{
	int rt = WSAStartup(0x0101,&CConsSocket::m_stWsaData);	
}

const char* CConsSocket::GetLocalIP()
{
	return CConsSocket::m_scLocalIP;
}

int CConsSocket::GetLocalPort()
{
	return CConsSocket::m_nPort;
}

void CConsSocket::ShutDown()
{
	WSACleanup();
}

WSADATA* CConsSocket::GetWsaData()
{
	return &CConsSocket::m_stWsaData;
}
