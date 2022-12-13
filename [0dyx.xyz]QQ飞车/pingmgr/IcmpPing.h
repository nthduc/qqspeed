// IcmpPingObject.h: interface for the CIcmpPingObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICMPPINGOBJECT_H__E37477CA_455E_4AF5_968B_9694977CC7ED__INCLUDED_)
#define AFX_ICMPPINGOBJECT_H__E37477CA_455E_4AF5_968B_9694977CC7ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxsock.h>
//#include "QQTRawSocket.h"
#include "IPingObject.h"
#include "PingThread.h"
#include <afxtempl.h>

//#define ICMP_ECHO 8 
//#define ICMP_ECHOREPLY 0 
//
//#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header) 
//
//#pragma pack(push)
//#pragma pack(4)
//
//
///* The IP header */ 
//typedef struct iphdr { 
//	unsigned int h_len:4; // length of the header 
//	unsigned int version:4; // Version of IP 
//	unsigned char tos; // Type of service 
//	unsigned short total_len; // total length of the packet 
//	unsigned short ident; // unique identifier 
//	unsigned short frag_and_flags; // flags 
//	unsigned char ttl; 
//	unsigned char proto; // protocol (TCP, UDP etc) 
//	unsigned short checksum; // IP checksum 
//	
//	unsigned int sourceIP; 
//	unsigned int destIP; 
//	
//}IpHeader;
//
//// 
//// ICMP header 
//// 
//typedef struct icmphdr 
//{ 
//	BYTE i_type; 
//	BYTE i_code; /* type sub code */ 
//	USHORT i_cksum; 
//	USHORT i_id; 
//	USHORT i_seq; 
//	/* This is not the std header, but we reserve space for time */ 
//}IcmpHeader;
//
//#pragma  pack(pop)

//struct QQT_PING_MSG
//{
//	IcmpHeader m_ICMPHeader;
//	ULONG m_unTimeStamp; 
//	ULONG m_nSocketIndex;
//};
//
//
//#define STATUS_FAILED 0xFFFF 
//#define DEF_PACKET_SIZE    32
//#define DEF_PACKET_NUMBER  4    /* 发送数据报的个数 */
//#define MAX_PACKET 1024

class CIcmpPingObject : public IPingObject/*, public IRawSocketCallBack*/
{
public:
	CIcmpPingObject();
	virtual ~CIcmpPingObject();
	
	virtual void Ping(unsigned int ulSvrIP, unsigned short usSvrPort, int nPingCount, int nStartTime, int nLimitTime);
	virtual void CancelPing();

	virtual void GetPingResult(int &nTTL, float &fLossRate, int &nAverageRTT);

//	static unsigned short CheckSum(unsigned short *pBuffer, int nSize);

//	void EncodePingMsg(char *pBuffer);
//	BOOL DecodePingMsg(char *pBuffer, int nBytes, int *pRTT, unsigned char *pTTL);

//	CQQTRawSocket m_Socket;

	void StartPings();

	void CheckCanDelete();

	BOOL CanDelete();

	int m_nPingCount;
	int m_nReplyCount;

	int m_nTotalTTL;
	int m_nTotalRTT;
	
	int m_cResult;
	
	int m_nSeq;

	BOOL m_bAllThreadObjectHasReturn;

	// 同步socket 要开线程
	CArray<CPingThread *, CPingThread *> m_PingThreads;
};

#endif // !defined(AFX_ICMPPINGOBJECT_H__E37477CA_455E_4AF5_968B_9694977CC7ED__INCLUDED_)
