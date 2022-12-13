// QQTP2pModule.cpp : implementation file
//

#include "stdafx.h"
#include "QQTPPP2ND.h"
#include "QQTP2pModule.h"
#include "QQTPPP_i.c"
#include "UnkHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQQTP2pModule

IMPLEMENT_DYNCREATE(CQQTP2pModule, CCmdTarget)
IMPLEMENT_IUNKNOWN(CQQTP2pModule, QQTP2PModule)
IMPLEMENT_OLECREATE(
	CQQTP2pModule,
	"CQQTP2pModule",	
	0x17ADB553, 0x1AA5, 0x46F4, 0xAD, 0x9D, 0x03, 0x28, 0xCE, 0x7A, 0xFD, 0x1E
)

BEGIN_INTERFACE_MAP(CQQTP2pModule, CCmdTarget)
	INTERFACE_PART(CQQTP2pModule, IID_IQQTppp, QQTP2PModule)	
END_INTERFACE_MAP()

CQQTP2pModule::CQQTP2pModule()
{
	m_Timer.HookEvent(P2P_TIMER_EVENT, this, &CQQTP2pModule::OnTimer);
	m_ServerSocket.SetSvrCallBack(this);
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		m_ClientSocket[i].SetConnect(this, &m_ServerSocket, i);
	}
	m_byRoundMark = 1;
}

CQQTP2pModule::~CQQTP2pModule()
{
	m_pP2pSink = NULL;
}


BEGIN_MESSAGE_MAP(CQQTP2pModule, CCmdTarget)
	//{{AFX_MSG_MAP(CQQTP2pModule)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQTP2pModule message handlers
//////////////////////////////////////////////////////////////////////////
// implementation of inner class
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::Initialize()
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->Initialize();
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::LoginLocalPlayer( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ unsigned int uiServerIP, /*[in]*/ unsigned short usSvrPort, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->LoginLocalPlayer(shPlayerID, nUin, uiServerIP, usSvrPort, shOtherPlayerID, nOtherUin);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::RemoveAllRemotePlayers()
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->RemoveAllRemotePlayers();
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::AddHost( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->AddHost(shPlayerID, nUin, shOtherPlayerID, nOtherUin);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::DelHost( /*[in]*/ short shPlayerID, /*[in]*/ int nUin)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->DelHost(shPlayerID, nUin);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::BroadCast( /*[in]*/ unsigned int uiLen, /*[size_is][in]*/ unsigned char *pBuf, /*[in]*/int iIsGameData)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->BroadCast(uiLen, pBuf, iIsGameData);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::ConfirmTransferIPPort( /*[in]*/ short shPlayerID, /*[in]*/ int nUin)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->ConfirmTransferIPPort(shPlayerID, nUin);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::ConfirmReceiveData( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, ULONG ulIP, USHORT usPort)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->ConfirmReceiveData(shPlayerID, nUin, ulIP, usPort);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::SendP2pData2Player(/*[in]*/short shPlayerID, /*[in]*/unsigned int uiUin, /*[in]*/unsigned int uiLen, /*[in, size_is(uiLen)]*/byte *pBuf, /*[in]*/int iIsGameData)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->SendP2pData2Player(shPlayerID, uiUin, uiLen, pBuf, iIsGameData);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::SetSink( /*[in]*/ IQQTpppSink *pQQTpppSink)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->SetSink(pQQTpppSink);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::SetGameStatus(/*[in]*/int iGameStatus, /*[in]*/unsigned int uiRoundMark)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->SetGameStatus(iGameStatus, uiRoundMark);
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::CancelSink()
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->CancelSink();
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::Finalize()
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->Finalize();
}
STDMETHODIMP CQQTP2pModule::XQQTP2PModule::OnRecvP2pTcpData(/*[in]*/short sPlayerID, /*[in]*/int iUin, /*[in]*/int iLen, /*[in, size_is(iLen)]*/BYTE *pData)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->OnRecvP2pTcpData(sPlayerID, iUin, iLen, pData);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// implemetaion of outter class
STDMETHODIMP CQQTP2pModule::Initialize()
{
	if(!m_Timer.IsRunning())
	{
		m_Timer.Create();
		m_Timer.StartTimer();
	}
	
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		m_ClientSocket[i].Initialize();
	}	
	m_ServerSocket.Initialize();

	m_dwMyUin = 0;
	m_sMyPlayerID = -1;
	m_ulSvrIP = 0;
	m_usSvrPort = 0;
	m_ulMyOuterIP = 0;
	m_UdpBufferMgr.Initialize();
	m_HostMgr.ClearAllUdpInfo();
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::LoginLocalPlayer( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ unsigned int uiServerIP, /*[in]*/ unsigned short usSvrPort, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin)
{
	ASSERT(shPlayerID > -1 && nUin > 10000 && usSvrPort && usSvrPort);
	uiServerIP = ::ntohl(uiServerIP);//Dir中保存的是网络序，这里转成本地序

	m_dwMyUin = (DWORD)nUin;
	m_sMyPlayerID = shPlayerID;
	m_ulSvrIP = uiServerIP;
	m_usSvrPort = usSvrPort;

	m_ServerSocket.SetMyInfo(nUin, shPlayerID);
	m_ServerSocket.SetServerAddr(uiServerIP, usSvrPort);
	m_ServerSocket.Login();
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::RemoveAllRemotePlayers()
{
	m_HostMgr.ClearAllUdpInfo();
	m_UdpBufferMgr.Initialize();
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		m_ClientSocket[i].Initialize();
	}
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::AddHost( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, /*[in]*/ int shOtherPlayerID, /*[in]*/ int nOtherUin)
{
	PPPTRACE("AddHost Uin %d, Player %d\n", nUin, shPlayerID);

	ASSERT(nUin > 10000 && shPlayerID > -1);
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer((DWORD)nUin))
		{
			ASSERT(FALSE);
			return S_OK;
		}
	}
	for(i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer(0))
		{
			m_ClientSocket[i].SetMyInfo(m_dwMyUin, m_sMyPlayerID);
			m_ClientSocket[i].SetDestPlayerInfo((DWORD)nUin, shPlayerID);
			m_ClientSocket[i].SetServerAddr(m_ulSvrIP, m_usSvrPort);
			m_ClientSocket[i].StartConnectPlayer();
			CUdpHostMgr::_player_udp_info *pUdpInfo = m_HostMgr.GetPlayerUdpInfo(shPlayerID, nUin);
			if(pUdpInfo)
			{
				PPPTRACE("Find Player Info in HostMgr\n");

				m_ClientSocket[i].SetDestPlayerAddr(pUdpInfo->ulOuterIP, pUdpInfo->usOuterPort, pUdpInfo->ulInnerIP, pUdpInfo->usInnerPort);
				m_HostMgr.RemoveUdpInfo(shPlayerID, nUin);
				if(m_pP2pSink)
				{
					m_pP2pSink->OnGetPlayerInfoFromSvr(shPlayerID, nUin);
				}
			}
			return S_OK;
		}
	}
	ASSERT(FALSE);
	return S_FALSE;
}
STDMETHODIMP CQQTP2pModule::DelHost( /*[in]*/ short shPlayerID, /*[in]*/ int nUin)
{
	PPPTRACE("DelHost Uin %d, PlayerID %d\n", nUin, shPlayerID);

	m_HostMgr.RemoveUdpInfo(shPlayerID, nUin);
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer((DWORD)nUin))
		{
			m_ClientSocket[i].Initialize();
			return S_OK;
		}
	}
	ASSERT(FALSE);
	return S_FALSE;
}
STDMETHODIMP CQQTP2pModule::BroadCast( /*[in]*/ unsigned int uiLen, /*[size_is][in]*/ unsigned char *pBuf, /*[in]*/int iResendLevel)
{
	LOG_TIME(a1, "CQQTP2pModule::BroadCast", 30);

	BOOL bKeyData = (iResendLevel > 1) ? TRUE : FALSE;

	int nPlayerCount = 0;
	UINT auiUin[8] = {0};
	short ashPlayerID[8] = {0};
	DWORD dwUin;
	short shPlayerID;
	int nCliIdx = 0;	//找到一个可用的ClientSocket
	
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(!m_ClientSocket[i].IsDestPlayer(0))
		{
			m_ClientSocket[i].SendP2PGameData(uiLen, pBuf, FALSE);

			if(bKeyData)
			{
				dwUin = 0;
				shPlayerID = 0;
				m_ClientSocket[i].GetDestPlayerInfo(dwUin, shPlayerID);
				if(dwUin)
				{
					auiUin[nPlayerCount] = dwUin;
					ashPlayerID[nPlayerCount] = shPlayerID;
					nPlayerCount++;
					nCliIdx = i;
				}
			}
		}
	}

	if(bKeyData)
	{	LOG_TIME(a3, "CQQTP2pModule:SendTcpDataToServer()", 30);
		m_ClientSocket[nCliIdx].SendTcpDataToServer(uiLen, pBuf, ashPlayerID, auiUin, nPlayerCount);
	}
	
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::ConfirmTransferIPPort( /*[in]*/ short shPlayerID, /*[in]*/ int nUin)
{
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer(nUin))
		{
			m_ClientSocket[i].SetTransmitOK();
			break;
		}
	}
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::ConfirmReceiveData( /*[in]*/ short shPlayerID, /*[in]*/ int nUin, ULONG ulIP, USHORT usPort)
{
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer(nUin))
		{
			m_ClientSocket[i].SetCanPass(ulIP, usPort);
			break;
		}
	}
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::SendP2pData2Player(/*[in]*/short shPlayerID, /*[in]*/unsigned int uiUin, /*[in]*/unsigned int uiLen, /*[in, size_is(uiLen)]*/byte *pBuf, /*[in]*/int iIsGameData)
{
//	CQQTUdpBufferMgr::IMsgBuffer *pMsgBuffer = m_UdpBufferMgr.AddMsgBuffer(pBuf, uiLen, iIsGameData ? TRUE : FALSE);
	BOOL bKeyData = (iIsGameData > 1) ? TRUE : FALSE;
//	if(pMsgBuffer)
	{
		for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
		{
			if(m_ClientSocket[i].IsDestPlayer(uiUin))
			{
				m_ClientSocket[i].SendP2PGameData(uiLen, pBuf, bKeyData);
//				pMsgBuffer->Release();
				return S_OK;
			}
		}
		ASSERT(FALSE);
//		pMsgBuffer->Release();
	}	
	return S_FALSE;
}
STDMETHODIMP CQQTP2pModule::SetSink( /*[in]*/ IQQTpppSink *pQQTpppSink)
{
	m_pP2pSink = pQQTpppSink;
//	ASSERT(m_pP2pSink)
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::SetGameStatus(/*[in]*/int iGameStatus, /*[in]*/unsigned int uiRoundMark)
{
	m_byRoundMark = (byte)uiRoundMark;


	m_UdpBufferMgr.Initialize();
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(!m_ClientSocket[i].IsDestPlayer(0))
		{
			m_ClientSocket[i].SetGameStatus(iGameStatus);
		}
	}
	m_iBoardCaseTag = 0;
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::CancelSink()
{
	m_pP2pSink = NULL;
	return S_OK;
}
STDMETHODIMP CQQTP2pModule::Finalize()
{
	Initialize();
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
void CQQTP2pModule::OnTimer()
{
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		m_ClientSocket[i].OnTimer();
	}	
	m_ServerSocket.OnTimer();
}

// 进入小区时和server建立的UDP连接可获得我的OuterIP
void CQQTP2pModule::OnLoginOk(ULONG ulOuterIP)
{
	m_ulMyOuterIP = ulOuterIP;
	if(m_pP2pSink)
	{
		m_pP2pSink->OnUdpLoginOk();
	}
}
void CQQTP2pModule::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin, ULONG ulOuterIP, USHORT usOuterPort, ULONG ulInnerIP, USHORT usInnerPort)
{	
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer(nUin))
		{
			m_ClientSocket[i].SetDestPlayerAddr(ulOuterIP, usOuterPort, ulInnerIP, usInnerPort);
			if(m_pP2pSink)
			{
				m_pP2pSink->OnGetPlayerInfoFromSvr(shPlayerID, nUin);
			}
			return;
		}
	}

	PPPTRACE("GetPlayerInfoFromSvr, ClientSocket not Found: Uin %d, PlayerID %d, OuterAddr %d.%d.%d.%d:%d, InnerAddr %d.%d.%d.%d:%d\n",
		nUin, shPlayerID, (ulOuterIP>>24)&0xFF, (ulOuterIP>>16)&0xFF, (ulOuterIP>>8)&0xFF, (ulOuterIP)&0xFF, usOuterPort,
		(ulInnerIP>>24)&0xFF, (ulInnerIP>>16)&0xFF, (ulInnerIP>>8)&0xFF, (ulInnerIP)&0xFF, usInnerPort);

	//add to host mgr
	m_HostMgr.AddPlayerUdpInfo(shPlayerID, nUin, ulOuterIP, usOuterPort, ulInnerIP, usInnerPort);
}
ULONG CQQTP2pModule::GetMyOutterIP()
{
	return m_ulMyOuterIP;
}
byte CQQTP2pModule::GetRoundMark()
{
	return m_byRoundMark;
}
void CQQTP2pModule::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	if(m_pP2pSink)
	{
		m_pP2pSink->OnRecvClientTry2ConnectData(shPlayerID, nUin, ulTestMyIP, usTestMyPort);
	}
}
void CQQTP2pModule::OnRecvP2PDataFromSvr(short shPlayerID, unsigned int uiUin, BYTE *pBody, int nBodyLen)
{
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer(uiUin))
		{
			m_ClientSocket[i].OnRecvP2PGameData(0, shPlayerID, uiUin, pBody, nBodyLen);
			break;
		}
	}
}
void CQQTP2pModule::OnRecvP2PGameDataFromClient(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, BYTE *pBody, int nBodyLen)
{
	//LOGBIN(TRUE, "RecvData", nBodyLen, (const char*)pBody);
	if(m_pP2pSink)
	{
		m_pP2pSink->OnRecvP2PGameDataFromClient(shSocketPlayerID, nSocketPlayerUin, shPlayerID, nBodyLen, pBody);
	}
}
CQQTUdpBufferMgr::IMsgBuffer* CQQTP2pModule::GetPreBuffer(int iClientTag)
{
	return m_UdpBufferMgr.GetLastBuffer(iClientTag);
}
void CQQTP2pModule::SetLastBuffer(int iClientTag, CQQTUdpBufferMgr::IMsgBuffer* pMsgBuffer)
{
	m_UdpBufferMgr.SetLastBuffer(iClientTag, pMsgBuffer);
}
STDMETHODIMP CQQTP2pModule::OnRecvP2pTcpData(/*[in]*/short sPlayerID, /*[in]*/int iUin, /*[in]*/int iLen, /*[in, size_is(iLen)]*/BYTE *pData)
{
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer(iUin))
		{
			m_ClientSocket[i].OnGetTcpP2pGameData(sPlayerID, iUin, pData, iLen);
		}
	}
	return S_OK;
}
void CQQTP2pModule::SendTcpP2pData(int nPlayerCount, short *asPlayerID, DWORD * auiUin, int iLen, byte *pBuffer)
{
	if(m_pP2pSink)
	{
		m_pP2pSink->SendTcpP2pData(nPlayerCount, asPlayerID, (int*)auiUin, iLen, pBuffer);
	}
}

STDMETHODIMP CQQTP2pModule::XQQTP2PModule::CanPass(unsigned int nUin)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->CanPass(nUin);
}

STDMETHODIMP CQQTP2pModule::XQQTP2PModule::GetP2pNetSpeed(unsigned int nUin, int* Speed)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->GetP2pNetSpeed(nUin, Speed);
}

STDMETHODIMP CQQTP2pModule::XQQTP2PModule::BroadCastDataViaServer(int nPlayerCount, short shPlayerIDs[8], unsigned int uiUins[8], unsigned int uiLen, byte *pBuf)
{
	METHOD_PROLOGUE(CQQTP2pModule, QQTP2PModule);
	return pThis->BroadCastDataViaServer(nPlayerCount, shPlayerIDs, uiUins, uiLen, pBuf);
}

STDMETHODIMP CQQTP2pModule::CanPass(unsigned int uiUin)
{
	for(int i = 0; i < MAX_P2P_DEST_PLAYER; i ++)
	{
		if(m_ClientSocket[i].IsDestPlayer(uiUin))
		{
			bool bCanPass = m_ClientSocket[i].CanPass();
			//				pMsgBuffer->Release();
			return bCanPass ? S_OK : S_FALSE;
		}
	}
	return S_FALSE;
}

STDMETHODIMP CQQTP2pModule::GetP2pNetSpeed(unsigned int nUin, int* Speed)
{
	for ( int i = 0; i < MAX_P2P_DEST_PLAYER; i++ )
	{
		if ( m_ClientSocket[i].IsDestPlayer(nUin) )
		{
			*Speed = m_ClientSocket[i].GetNetSpeed();
			return S_OK;
		}
	}
	return E_FAIL;
}

STDMETHODIMP CQQTP2pModule::BroadCastDataViaServer(int nPlayerCount, short shPlayerIDs[8], unsigned int uiUins[8], unsigned int uiLen, byte *pBuf)
{
	for ( int i = 0; i < MAX_P2P_DEST_PLAYER; i++ )
	{
//		if ( !m_ClientSocket[i].CanPass() )
		{
			m_ClientSocket[i].SendP2PGameData(uiLen, pBuf, FALSE, shPlayerIDs, uiUins, nPlayerCount, TRUE);
			return S_OK;
		}
	}
	assert(0);	//p2p都是通的，不应该到这里
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
// temp codes
void CQQTP2pModule::OnSvrClose(){}
void CQQTP2pModule::OnClientClose(short shPlayerID, int nUin){}

