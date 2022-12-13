// P2PAdapterImpl.cpp : Implementation of CP2PAdapterImpl
#include "stdafx.h"
#include "P2PAdapter.h"
#include "P2PAdapterImpl.h"
#include "QQTppp_i.c"
#include "network/msgheader.h"
#include "Network/Network_i.h"
#include "Network/Network_i.c"
#include "log.h"
#include "mycom.h"
#include "fileop.h"

#define P2P_MODULE "QQTPPP2ND"

/////////////////////////////////////////////////////////////////////////////
// CP2PAdapterImpl

CP2PAdapterImpl::CP2PAdapterImpl():
m_pApp(NULL),
m_pQQTppp(NULL),
m_pNetwork(NULL),
m_dwCookie(0)
{
}

STDMETHODIMP CP2PAdapterImpl::AppInit(IGameApp* App)
{
	assert(App);
	char szModulePath[MAX_PATH] = "";
	GetModulePath(NULL, szModulePath);
	char szFileName[MAX_PATH] = "";
	_makepath(szFileName, NULL, szModulePath, P2P_MODULE, NULL);
	CreateObjectFromFile(szFileName, CLSID_CP2PModule, IID_IQQTppp, (void**)&m_pQQTppp);
	assert(m_pQQTppp);
	if ( !m_pQQTppp )
	{
		return E_FAIL;
	}
	m_pQQTppp->Initialize();
	m_pQQTppp->SetSink((IQQTpppSink*)this);

	m_pApp = App;
	CComQIPtr<INetwork> spNetwork;
	HRESULT hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
	if ( hr != S_OK )
		return E_FAIL;

	m_pNetwork = spNetwork;

	hr = AtlAdvise(spNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
	if ( hr != S_OK )
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CP2PAdapterImpl::AppUnInit()
{
	if ( m_pQQTppp )
	{
		m_pQQTppp->Finalize();
		RELEASE(m_pQQTppp);
		m_pQQTppp = NULL;
	}
	return S_OK;
}

STDMETHODIMP CP2PAdapterImpl::OnResponseLogin(ServerEntity, LoginResult)
{
	return S_OK;
}

STDMETHODIMP CP2PAdapterImpl::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, 
											  PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity != eGameServer )
		return S_OK;
	switch(MsgID)
	{
	case ID_SMC_NOTIFYUDPOK:
		OnNotifyUDPOK(Data);
		break;
	}
	ProcessP2PData(Data, DataLen, csHead, msgHead);
	return S_OK;
}

STDMETHODIMP CP2PAdapterImpl::OnClose(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CP2PAdapterImpl::OnConnect(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CP2PAdapterImpl::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CP2PAdapterImpl::OnStartLogin(ServerEntity, int nErrorCode)
{
	return S_OK;
}

void CP2PAdapterImpl::OnNotifyUDPOK(PBYTE Data)
{
	CNotifyUDPOKPara* pPara = (CNotifyUDPOKPara*)Data;
	if ( pPara )
	{
		if ( pPara->m_szInfo[0] )
		{
			ULONG nIP = ntohl(*(ULONG*)(&pPara->m_szInfo[1]));
			USHORT nPort = ntohs(*(USHORT*)(&pPara->m_szInfo[5]));
			m_pQQTppp->ConfirmReceiveData(pPara->m_nSrcDlg, pPara->m_unSrcUin, nIP, nPort);
		}
		else
		{
			m_pQQTppp->ConfirmTransferIPPort(pPara->m_nSrcDlg, pPara->m_unSrcUin);
		}
	}
}

void CP2PAdapterImpl::ProcessP2PData(PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	CCSHead* pcsHead = (CCSHead*)csHead;
	CMsgHead* pmsgHead = (CMsgHead*)msgHead;
	if (pcsHead->m_byOptLength >=2 && ID_TCP_HELP_P2P_RECV_MSG == ::ntohs(*((short*)&pcsHead->m_abyOption[0])))
	{
		if (pcsHead->m_byOptLength != 6)
		{
			assert(FALSE);
			return;
		}
		DWORD dwUin = ::ntohl(*((int*)&pcsHead->m_abyOption[2]));
		m_pQQTppp->OnRecvP2pTcpData(-1, dwUin, DataLen, Data);
	}
}


STDMETHODIMP CP2PAdapterImpl::OnUdpLoginOk()
{
	LOGLINE("OnUdpLoginOk");
	return Fire_OnUdpLoginOk();
}

STDMETHODIMP CP2PAdapterImpl::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	LOGLINE("OnRecvP2PDataFromSvr");
	return Fire_OnRecvP2PDataFromSvr(shPlayerID, nBodyLen, pBody);
}

STDMETHODIMP CP2PAdapterImpl::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	LOGLINE("OnGetPlayerInfoFromSvr");
	HRESULT hr = S_OK;
	QQUSERDATA* pQQData = (QQUSERDATA*)m_pNetwork->GetQQUserData();
	
	CRequestTransferUDPOKPara aPara;
	aPara.m_unUin = pQQData->Uin;
	aPara.m_unTime = GetTickCount();
	aPara.m_nDstDlg = shPlayerID;
	aPara.m_unDstUin = nUin;
	aPara.m_nInfoLength = 1;
	aPara.m_szInfo[0] = 0;
	hr = m_pNetwork->RequestGameServer(ID_CMS_REQUESTUDPOK, (PBYTE)&aPara, 0, FE_PLAYER, 0);
	assert(hr == S_OK);

	return Fire_OnGetPlayerInfoFromSvr(shPlayerID, nUin);
}

STDMETHODIMP CP2PAdapterImpl::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	LOGLINE("OnRecvClientTry2ConnectData");
	HRESULT hr = S_OK;
	QQUSERDATA* pQQData = (QQUSERDATA*)m_pNetwork->GetQQUserData();

	CRequestTransferUDPOKPara aPara;
	aPara.m_unUin = pQQData->Uin;
	aPara.m_unTime = GetTickCount();
	aPara.m_nDstDlg = shPlayerID;
	aPara.m_unDstUin = nUin;
	aPara.m_nInfoLength = 7;
	aPara.m_szInfo[0] = 1;
	*(ULONG*)(&aPara.m_szInfo[1]) = htonl(ulTestMyIP);
	*(USHORT*)(&aPara.m_szInfo[5]) = htons(usTestMyPort);
	hr = m_pNetwork->RequestGameServer(ID_CMS_REQUESTUDPOK, (PBYTE)&aPara, 0, FE_PLAYER, 0);
	assert(hr == S_OK);
	
	return Fire_OnRecvClientTry2ConnectData(shPlayerID, nUin, ulTestMyIP, usTestMyPort);
}

STDMETHODIMP CP2PAdapterImpl::OnRecvP2PGameDataFromClient(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
//	LOGLINE("OnRecvP2PGameDataFromClient");
	return Fire_OnRecvP2PGameDataFromClient(shSocketPlayerID, nSocketPlayerUin, shPlayerID, nBodyLen, pBody);
}

STDMETHODIMP CP2PAdapterImpl::SendTcpP2pData(int PlayerNum, short *PlayerIDs, int *PlayerUins, int DataLen, BYTE *Data)
{
	LOGLINE("SendTcpP2pData");
	HRESULT hr = m_pNetwork->SendTcpP2pData(0, PlayerNum, PlayerIDs, (UINT*)PlayerUins, Data, DataLen);
	return S_OK;
}
