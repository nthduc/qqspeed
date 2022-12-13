// UdpHostMgr.cpp: implementation of the CUdpHostMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QQTPPP2ND.h"
#include "UdpHostMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUdpHostMgr::CUdpHostMgr()
{
	ClearAllUdpInfo();
}

CUdpHostMgr::~CUdpHostMgr()
{

}

void CUdpHostMgr::AddPlayerUdpInfo(short sPlayerID, DWORD dwUin, ULONG ulOuterIP, USHORT usOuterPort, ULONG ulInnerIP, USHORT usInnerPort)
{
	//to look for if udp info of this player has in array
	PPPTRACE("CUdpHostMgr::AddPlayerUdpInfo Uin %d, PlayerID %d\n", dwUin, sPlayerID);

	int iTag = -1;;
	for(int i = 0; i < MAX_PLAYER_UDP_INFO; i ++)
	{
		if(!m_UdpInfo[i].bInUse)
		{
			if(-1 == iTag)
			{
				iTag = i;
			}
		}
		else if(m_UdpInfo[i].dwUin == dwUin)
		{
			m_UdpInfo[i].sPlayerID = sPlayerID;
			m_UdpInfo[i].ulOuterIP = ulOuterIP;
			m_UdpInfo[i].usOuterPort = usOuterPort;
			m_UdpInfo[i].ulInnerIP = ulInnerIP;
			m_UdpInfo[i].usInnerPort = usInnerPort;
			return;
		}		
	}

	if(-1 == iTag)
	{
		iTag = ((rand() % MAX_PLAYER_UDP_INFO) + MAX_PLAYER_UDP_INFO) % MAX_PLAYER_UDP_INFO;
	}
	m_UdpInfo[iTag].bInUse = TRUE;
	m_UdpInfo[iTag].dwUin = dwUin;
	m_UdpInfo[iTag].sPlayerID = sPlayerID;
	m_UdpInfo[iTag].ulOuterIP = ulOuterIP;
	m_UdpInfo[iTag].usOuterPort = usOuterPort;
	m_UdpInfo[iTag].ulInnerIP = ulInnerIP;
	m_UdpInfo[iTag].usInnerPort = usInnerPort;
	return;
}
CUdpHostMgr::_player_udp_info* CUdpHostMgr::GetPlayerUdpInfo(short sPlayerID, DWORD dwUin)
{
	PPPTRACE("CUdpHostMgr::GetPlayerUdpInfo Uin %d, PlayerID %d\n", dwUin, sPlayerID);

	for(int i = 0; i < MAX_PLAYER_UDP_INFO; i ++)
	{
		if(m_UdpInfo[i].bInUse)
		{
			if(sPlayerID == m_UdpInfo[i].sPlayerID && dwUin == m_UdpInfo[i].dwUin)
			{
				return &m_UdpInfo[i];
			}
		}
	}
	return NULL;
}
void CUdpHostMgr::ClearAllUdpInfo()
{
	::ZeroMemory(&m_UdpInfo, sizeof(_player_udp_info) * MAX_PLAYER_UDP_INFO);
}
void CUdpHostMgr::RemoveUdpInfo(short sPlayerID, DWORD dwUin)
{
	for(int i = 0; i < MAX_PLAYER_UDP_INFO; i ++)
	{
		if(m_UdpInfo[i].bInUse)
		{
			if(sPlayerID == m_UdpInfo[i].sPlayerID && dwUin == m_UdpInfo[i].dwUin)
			{
				::ZeroMemory(&m_UdpInfo[i], sizeof(_player_udp_info));
				return;
			}
		}
	}
}

