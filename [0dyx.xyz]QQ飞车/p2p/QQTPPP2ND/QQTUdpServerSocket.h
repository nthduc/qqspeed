// QQTUdpServerSocket.h: interface for the CQQTUdpServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QQTUDPSERVERSOCKET_H__0077BA9B_8842_436B_8566_22986EF21243__INCLUDED_)
#define AFX_QQTUDPSERVERSOCKET_H__0077BA9B_8842_436B_8566_22986EF21243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "QQTUdpProtocolSocket.h"

#define UDP_LOGIN_RESEND_TIME	5
#define UDP_SERVER_HELLO_TIME_LENGTH	20

class CQQTUdpServerSocket : public CQQTUdpProtocolSocket::IQQTUdpProtocolSocketReceiver
{
public:
	class IUdpSvrCallBack
	{
	public:
		virtual void OnLoginOk(ULONG ulOuterIP) PURE;		
		virtual void OnRecvP2PDataFromSvr(short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen) PURE;
		virtual void OnGetPlayerInfoFromSvr(short shPlayerID, int nUin, ULONG ulOuterIP, USHORT usOuterPort, ULONG ulInnerIP, USHORT usInnerPort) PURE;
		virtual void OnSvrClose() PURE;
	};
public:
	CQQTUdpServerSocket();
	virtual ~CQQTUdpServerSocket();

	//////////////////////////////////////////////////////////////////////////
	// CQQTUdpServerSocket operaiton
	void Initialize();
	void SetServerAddr(ULONG ulSvrIP, USHORT usSvrPort);
	void SetMyInfo(DWORD dwUin, short sPlayerID);
	void Login();
	void OnTimer();
	void SetSvrCallBack(IUdpSvrCallBack *pCallBack)			{	m_pSvrCallBack = pCallBack;	}
	void BroadCastData(const byte *pBuffer, int nLen, short shPlayerID[8], unsigned int nUin[8], int nNumberOfPlayer);
protected:
	//////////////////////////////////////////////////////////////////////////	
	//implementation of CQQTUdpProtocolSocket::IQQTUdpProtocolSocketReceiver
	virtual void OnClose();
	virtual void OnLoginResponse(ULONG ulSvrIP, ULONG ulOuterIP);
	virtual void OnGetPlayerInfoFromSvr(ULONG ulSvrIP, short shSrcPlayerID, int nSrcUin, ULONG ulSrcOuterIP, USHORT usSrcOuterPort, ULONG ulSrcInerIP, USHORT usSrcInnerPort);
	virtual void OnRecvP2PGameData(ULONG ulSrcIP, short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen);
	virtual void OnRecvP2PTestConnectData(ULONG ulIP, USHORT usPort, ULONG ulTestMyIP, USHORT usTestMyPort, byte byCmd){ASSERT(FALSE);}
#ifdef _ENABLE_QQT_LOG_
	virtual void LogString(const char *pFormat, ...);
	virtual void LogBin(int nLen, const char *pBuf);
	virtual void Log2Win(char* pFormat, ...);
	char m_szLogPath[MAX_PATH];
#endif
private:
	//login data
	enum QQT_UDP_Login_Status
	{
		QQT_UDP_Login_Status_No_Start = 0,
		QQT_UDP_Login_Status_Send_No_Receive,
		QQT_UDP_Login_Status_Ok
	};
	
	QQT_UDP_Login_Status m_QQTUdpLoginStatus;
	int m_iLoginRetryTimer;

	int m_iHelloTimer;

	CQQTUdpProtocolSocket m_ProtocolSocket;
	IUdpSvrCallBack *m_pSvrCallBack;
};

#endif // !defined(AFX_QQTUDPSERVERSOCKET_H__0077BA9B_8842_436B_8566_22986EF21243__INCLUDED_)
