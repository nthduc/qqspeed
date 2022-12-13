// P2PAdapterImpl.h : Declaration of the CP2PAdapterImpl

#ifndef __P2PADAPTER_H_
#define __P2PADAPTER_H_

#include "resource.h"       // main symbols
#include "framework/GameApp_i.h"
#include "QQTppp_i.h"
#include "Network/Network_i.h"
#include "IQQTpppSink_CP.H"

/////////////////////////////////////////////////////////////////////////////
// CP2PAdapterImpl
class ATL_NO_VTABLE CP2PAdapterImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CP2PAdapterImpl, &CLSID_P2PAdapter>,
	public IGameService,
	public INetworkSink,
	public IQQTpppSink,
	public IConnectionPointContainerImpl<CP2PAdapterImpl>,
	public CProxyIQQTpppSink<CP2PAdapterImpl>
{
public:
	CP2PAdapterImpl();

	//IGameService
	STDMETHOD(AppInit)(IGameApp*);
	STDMETHOD(AppUnInit)();

	//INetworkSink
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult);
	STDMETHOD(OnServerMessage)(ServerEntity, UINT, UINT, PBYTE, INT, PBYTE, PBYTE);
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode);
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode);
	STDMETHOD(OnCreate)(ServerEntity, int nErrorCode);
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode);
	
	//IQQTpppSink
	STDMETHOD(OnUdpLoginOk)();
	STDMETHOD(OnRecvP2PDataFromSvr)(short shPlayerID, int nBodyLen, BYTE *pBody);
	STDMETHOD(OnGetPlayerInfoFromSvr)(short shPlayerID, int nUin);
	STDMETHOD(OnRecvClientTry2ConnectData)(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort);
	STDMETHOD(OnRecvP2PGameDataFromClient)(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody);
	STDMETHOD(SendTcpP2pData)(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData);

	DECLARE_REGISTRY_RESOURCEID(IDR_P2PADAPTER)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CP2PAdapterImpl)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IGameService)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IQQTppp, m_pQQTppp)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

protected:
	void OnNotifyUDPOK(PBYTE);
	void ProcessP2PData(PBYTE, INT, PBYTE, PBYTE);

private:
	IGameApp*	m_pApp;
	IQQTppp*	m_pQQTppp;
	INetwork*	m_pNetwork;
	DWORD		m_dwCookie;
	INT			m_nRoomID;
public:
	BEGIN_CONNECTION_POINT_MAP(CP2PAdapterImpl)
		CONNECTION_POINT_ENTRY(__uuidof(IQQTpppSink))
	END_CONNECTION_POINT_MAP()
};

#endif //__P2PADAPTER_H_
