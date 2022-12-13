// UdpHostMgr.h: interface for the CUdpHostMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDPHOSTMGR_H__E50DBF49_DFCF_4A06_BEC4_07D0AC4F5F6B__INCLUDED_)
#define AFX_UDPHOSTMGR_H__E50DBF49_DFCF_4A06_BEC4_07D0AC4F5F6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PLAYER_UDP_INFO		32

class CUdpHostMgr  
{
public:
	typedef struct _player_udp_info
	{
		BOOL bInUse;		
		DWORD dwUin;
		short sPlayerID;
		ULONG ulOuterIP;
		USHORT usOuterPort;
		ULONG ulInnerIP;
		USHORT usInnerPort;
	} PLAYER_UDP_INFO;
public:
	CUdpHostMgr();
	virtual ~CUdpHostMgr();

	void AddPlayerUdpInfo(short sPlayerID, DWORD dwUin, ULONG ulOuterIP, USHORT usOuterPort, ULONG ulInnerIP, USHORT usInnerPort);
	_player_udp_info * GetPlayerUdpInfo(short shPlayerID, DWORD dwUin);
	void ClearAllUdpInfo();
	void RemoveUdpInfo(short sPlayerID, DWORD dwUin);
private:
	_player_udp_info m_UdpInfo[MAX_PLAYER_UDP_INFO];

};

#endif // !defined(AFX_UDPHOSTMGR_H__E50DBF49_DFCF_4A06_BEC4_07D0AC4F5F6B__INCLUDED_)
