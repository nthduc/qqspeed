#if !defined(AFX_QQTP2PMODULE_H__BA78BD81_F45D_45DD_8E03_12FE344CE53C__INCLUDED_)
#define AFX_QQTP2PMODULE_H__BA78BD81_F45D_45DD_8E03_12FE344CE53C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QQTP2pModule.h : header file
//
#include "IQQTPPP.h"
#include "QQTHelper.h"
#include "QQTUdpClientSocket.h"
#include "AVTimer.h"
#include "QQTUdpBuffermgr.h"
#include "UdpHostMgr.h"


/////////////////////////////////////////////////////////////////////////////
// CQQTP2pModule command target

class CQQTP2pModule : public CCmdTarget, public CQQTUdpServerSocket::IUdpSvrCallBack, public CQQTUdpClientSocket::IUdpClientCallBack
{
	DECLARE_DYNCREATE(CQQTP2pModule)
	DECLARE_OLECREATE(CQQTP2pModule)
	DECLARE_EVENT_RECEIVER(CQQTP2pModule)

	CQQTP2pModule();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQTP2pModule)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CQQTP2pModule();

	// Generated message map functions
	//{{AFX_MSG(CQQTP2pModule)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(QQTP2PModule, IQQTppp)
	INIT_INTERFACE_PART(CP2PModule, QQTP2PModule)
		STDMETHOD(Initialize)();
        STDMETHOD(LoginLocalPlayer)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ unsigned int uiServerIP, /*[in]*/ unsigned short usSvrPort, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin);
		STDMETHOD(RemoveAllRemotePlayers)();
        STDMETHOD(AddHost)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin);
        STDMETHOD(DelHost)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin);
        STDMETHOD(BroadCast)( /*[in]*/ unsigned int uiLen, /*[size_is][in]*/ unsigned char *pBuf, /*[in]*/int iIsGameData);
        STDMETHOD(ConfirmTransferIPPort)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin);
		STDMETHOD(ConfirmReceiveData)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, ULONG ulIP, USHORT usPort);
		STDMETHOD(SendP2pData2Player)(/*[in]*/short shPlayerID, /*[in]*/unsigned int uiUin, /*[in]*/unsigned int uiLen, /*[in, size_is(uiLen)]*/byte *pBuf, /*[in]*/int iIsGameData);
        STDMETHOD(SetSink)( /*[in]*/ IQQTpppSink *pQQTpppSink);
		STDMETHOD(SetGameStatus)(/*[in]*/int iGameStatus, /*[in]*/unsigned int uiRoundMark);
        STDMETHOD(CancelSink)();
		STDMETHOD(Finalize)();
		STDMETHOD(OnRecvP2pTcpData)(/*[in]*/short sPlayerID, /*[in]*/int iUin, /*[in]*/int iLen, /*[in, size_is(iLen)]*/BYTE *pData);
		STDMETHOD(CanPass)(unsigned int nUin);
		STDMETHOD(GetP2pNetSpeed)(unsigned int nUin, int* Speed);
		STDMETHOD(BroadCastDataViaServer)(int nPlayerCount, short shPlayerIDs[8], unsigned int uiUins[8], unsigned int uiLen, byte *pBuf);		
	END_INTERFACE_PART(QQTP2PModule)

	STDMETHOD(Initialize)();
    STDMETHOD(LoginLocalPlayer)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ unsigned int uiServerIP, /*[in]*/ unsigned short usSvrPort, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin);
	STDMETHOD(RemoveAllRemotePlayers)();
    STDMETHOD(AddHost)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin);
    STDMETHOD(DelHost)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin);
    STDMETHOD(BroadCast)( /*[in]*/ unsigned int uiLen, /*[size_is][in]*/ unsigned char *pBuf, /*[in]*/int iIsGameData);
    STDMETHOD(ConfirmTransferIPPort)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin);
	STDMETHOD(ConfirmReceiveData)( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, ULONG ulIP, USHORT usPort);
	STDMETHOD(SendP2pData2Player)(/*[in]*/short shPlayerID, /*[in]*/unsigned int uiUin, /*[in]*/unsigned int uiLen, /*[in, size_is(uiLen)]*/byte *pBuf, /*[in]*/int iIsGameData);
    STDMETHOD(SetSink)( /*[in]*/ IQQTpppSink *pQQTpppSink);
	STDMETHOD(SetGameStatus)(/*[in]*/int iGameStatus, /*[in]*/unsigned int uiRoundMark);
    STDMETHOD(CancelSink)();
	STDMETHOD(Finalize)();
	STDMETHOD(OnRecvP2pTcpData)(/*[in]*/short sPlayerID, /*[in]*/int iUin, /*[in]*/int iLen, /*[in, size_is(iLen)]*/BYTE *pData);
	STDMETHOD(CanPass)(unsigned int nUin);
	STDMETHOD(GetP2pNetSpeed)(unsigned int nUin, int* Speed);
	STDMETHOD(BroadCastDataViaServer)(int nPlayerCount, short* shPlayerIDs, unsigned int* uiUins, unsigned int uiLen, byte *pBuf);		
private:
	//////////////////////////////////////////////////////////////////////////
	// Implementation of CQQTUdpServerSocket::IUdpSvrCallBack
	virtual void OnLoginOk(ULONG ulOuterIP);
	virtual void OnRecvP2PDataFromSvr(short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen);
	virtual void OnGetPlayerInfoFromSvr(short shPlayerID, int nUin, ULONG ulOuterIP, USHORT usOuterPort, ULONG ulInnerIP, USHORT usInnerPort);
	virtual void OnSvrClose();
	//////////////////////////////////////////////////////////////////////////
	// implementation of CQQTUdpClientSocket::IUdpClientCallBack
	virtual void OnClientClose(short shPlayerID, int nUin);
	virtual void OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort);
	virtual void OnRecvP2PGameDataFromClient(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, BYTE *pBody, int nBodyLen);
	virtual ULONG GetMyOutterIP();
	virtual byte GetRoundMark();
	virtual CQQTUdpBufferMgr::IMsgBuffer* GetPreBuffer(int iClientTag);
	virtual void SetLastBuffer(int iClientTag, CQQTUdpBufferMgr::IMsgBuffer* pMsgBuffer);
	virtual void SendTcpP2pData(int nPlayerCount, short *asPlayerID, DWORD *auiUin, int iLen, byte *pBuffer);

	void OnTimer();
	CAVTimer m_Timer;
private:
	CQQTWiseComPtr<IQQTpppSink> m_pP2pSink;
	CQQTUdpClientSocket m_ClientSocket[MAX_P2P_DEST_PLAYER];
	CQQTUdpServerSocket m_ServerSocket;

	//////////////////////////////////////////////////////////////////////////
	DWORD m_dwMyUin;
	short m_sMyPlayerID;
	ULONG m_ulSvrIP;
	USHORT m_usSvrPort;
	ULONG m_ulMyOuterIP;

	//////////////////////////////////////////////////////////////////////////
	byte m_byRoundMark;
	int m_iBoardCaseTag;

	//////////////////////////////////////////////////////////////////////////
	CQQTUdpBufferMgr m_UdpBufferMgr;
	CUdpHostMgr m_HostMgr;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQTP2PMODULE_H__BA78BD81_F45D_45DD_8E03_12FE344CE53C__INCLUDED_)
