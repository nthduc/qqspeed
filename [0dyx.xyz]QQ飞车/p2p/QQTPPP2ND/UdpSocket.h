#if !defined(AFX_UDPSOCKET_H__EDCCDD41_F2B1_4828_AA76_5C39706E2740__INCLUDED_)
#define AFX_UDPSOCKET_H__EDCCDD41_F2B1_4828_AA76_5C39706E2740__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UdpSocket.h : header file
//

#include "QQTPPP2ND.h"



/////////////////////////////////////////////////////////////////////////////
// CUdpSocket command target

#define MAX_UDP_PACKAGE				1024

class CUdpSocket : public CAsyncSocket
{
// Attributes
public:
	class IUdpSocketCallBack
	{
	public:
		virtual void OnReceive(ULONG ulIP, USHORT usPort, BYTE *pBuffer, int nBufferLen) PURE;
		virtual void OnClose() PURE;
#ifdef _ENABLE_QQT_LOG_ 
		virtual void LogString(const char *pFormat, ...) PURE;
		virtual void LogBin(int nLen, const char *pBuf) PURE;
		virtual void Log2Win(char* pFormat, ...) PURE;
#endif
	};

// Operations
public:
	CUdpSocket();
	virtual ~CUdpSocket();

	//socket operation
	BOOL BindPort(USHORT usPort = 0);
	void CloseSocket();
	void SetUdpSocketCallBack(IUdpSocketCallBack* pQQTUdpSocketCallBack);
	void SetServerAddr(ULONG ulIP, USHORT usPort);
	void SetClientAddr(ULONG ulIP, USHORT usPort);
	void SendUdpDataToServer(int nLen, const byte *pBuffer);
	void SendUdpDataToClient(int nLen, const byte *pBuffer);
	
	//get property
	ULONG GetLocalIP();
	USHORT GetLocalPort() const				{	return m_usLocalPort;	}
	ULONG GetServerIP() const					{	return m_ulServerIP;	}
	USHORT GetServerPort() const				{	return m_usServerPort;}
	ULONG GetClientIP() const				{ return m_ulClientIP; }
	USHORT GetClientPort() const			{ return m_usClientPort; }
	
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdpSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUdpSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	void SendUdpData(int nLen, const byte *pBuffer, ULONG ulIP, USHORT usPort);
	void _internal_socket_close();
private:	
	byte m_byUdpSendBuffer[MAX_UDP_PACKAGE];
	short m_sUdpSendBufferLength;
	IUdpSocketCallBack *m_pUdpSocketCallBack;

	ULONG m_ulServerIP;
	USHORT m_usServerPort;
	
	ULONG m_ulClientIP;
	USHORT m_usClientPort;

	static ULONG m_ulLocalIP;
	USHORT m_usLocalPort;	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPSOCKET_H__EDCCDD41_F2B1_4828_AA76_5C39706E2740__INCLUDED_)
