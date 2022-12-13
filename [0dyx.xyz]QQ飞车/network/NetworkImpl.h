////////////////////////////////////////////////
//     FileName:    NetworkImpl.h
//     Author:      Fish
//     Date:        2006-9-18 15:55:53
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "QQLoginSocket.h"
#include "DirSocket.h"
#include "GameSocket.h"
#include "ShopSocket.h"
#include "Network/Network_i.h"
#include "framework/GameApp_i.h"
#include "INetworkSink_CP.H"
#include "resource.h"
#include "PingMgr_i.h"
#include "vector"
using namespace std;

typedef struct tagAppInfo
{
	HWND	hWnd;
	DWORD	Version;
	DWORD	luVersion;
	DWORD	luoptVersion;
}AppInfo;

#define MAX_PINGCOUNT	3
typedef struct tagPingTaskInfo
{
	DWORD	dwChannel;
	DWORD	dwLocID;
	UINT	nSvrIP;
	USHORT	nSvrPort;
	INT		nPingIndex;
	INT		nPingValue;
	INT		nPingCount;
	bool	bStoped;

	int operator < (tagPingTaskInfo& rhs)
	{
		return nPingValue < rhs.nPingValue;
	}
}PingTaskInfo;
typedef vector<PingTaskInfo>	PingTaskInfo_List;

class ATL_NO_VTABLE CNetworkImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CNetworkImpl, &CLSID_NetworkImpl>,
	public IConnectionPointContainerImpl<CNetworkImpl>,
	public INetwork,
	public IGameService,
	public CQQLoginSocket, 
	public CDirSocket, 
	public CGameSocket, 
	public CShopSocket,
	public ISocketSinkProxy,
	public IPingSink,
	public CProxyINetworkSink<CNetworkImpl>,
	public IEventImpl<IVersionUpdaterEvent, 1>
{
public:
	CNetworkImpl();
	virtual ~CNetworkImpl(void);

	//IGameService
	STDMETHOD(AppInit)(IGameApp*);
	STDMETHOD(AppUnInit)();

	//INetwork
	STDMETHOD(SetActiveWorld)(DWORD dwWorldID);
	STDMETHOD(SetLoginType)(UINT);
	STDMETHOD(StartLogin)(UINT Uin, PBYTE Pwd);
	STDMETHOD(CancelLogin)();
	STDMETHOD(StartReLogin)();
	STDMETHOD(LoginGameServer)(DWORD dwCountry, DWORD dwChannel, DWORD dwSvrID);

	STDMETHOD(RequestGameServer)(UINT MsgID, PBYTE Data, UINT PlayerID, BYTE DestFE, SHORT DestID, UINT MsgType) ;
	STDMETHOD(RequestShopServer)(UINT MsgID, PBYTE Data, UINT MsgType);
	STDMETHOD(SendTcpP2pData)(SHORT PlayerID, INT PlayerNum, SHORT* PlayerIDs, UINT* PlayerUins, PBYTE Data, INT DataLen);

	QQUSERDATA*	GetQQUserData();
	DirResponseInfo* GetDirInfo();
	GameResponseInfo* GetGameInfo();
	SOCKET_CREATE_INFO*	GetSocketInfo(ServerEntity);

	virtual int oi_symmetry_encrypt2_len(int nInBufLen);
	virtual void oi_symmetry_encrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);
	virtual BOOL oi_symmetry_decrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen);

	//ISocketSinkProxy
	STDMETHOD(OnStartLogin)(ServerEntity Entity, int nErrorCode);
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult);
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode);
	STDMETHOD(OnReceive)(ServerEntity, int nErrorCode);
	STDMETHOD(OnSend)(ServerEntity, int nErrorCode);
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode);
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PVOID Data, INT, PVOID, PVOID);

	//IPingSink
	STDMETHOD(NotifyPingResult)(int nIndex, int nTTL, float fLossRate, int nAverageime);

	// IVersionUpdaterEvent Handler
	virtual void OnUpdateSuccess();
	virtual void OnUpdateFail();
	virtual void OnIgnoreUpdate();
	virtual void OnAlreadyNewest();
	virtual void OnNeedUpdate();

	DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY1)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CNetworkImpl)
		COM_INTERFACE_ENTRY(IGameService)
		COM_INTERFACE_ENTRY(INetwork)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IPingMgr, m_pPingMgr)
	END_COM_MAP()
	BEGIN_CONNECTION_POINT_MAP(CNetworkImpl)
		CONNECTION_POINT_ENTRY(__uuidof(INetworkSink))
	END_CONNECTION_POINT_MAP()

private:
	BOOL InitLoginQQ();
	BOOL InitLoginDir();
	BOOL InitLoginGame();
	BOOL InitLoginShop();

	BOOL LoginDir();
	BOOL LoginNormal(UINT Uin, const BYTE* Pwd, INT HashType = 0);
	BOOL LoginFromQQGame();
	BOOL LoginFromQQ();
	void SetLoginPlayer(int nUin, const BYTE* byPwd, int iHashType);

	void CheckLoginFromLoader();

	STGameServerInfo* FindSvrInfo(DWORD dwSvrID);
	STChannelInfo* FindChannelInfo(DWORD dwCountry, DWORD dwChannel);
	void PingChannel(STChannelInfo* pChannelInfo);
	void StopPing();
	DWORD FindBestLocID(DWORD dwChannel);
	DWORD RandCountry();
	DWORD RandChannel(DWORD dwCountry);
	void LoginGameServer(DWORD dwCountry, DWORD dwChannel, DWORD dwLocID, bool bLoc);
	void InitGameSocket();

	void GetLUVersion(DWORD& dwVersion, DWORD& dwOptVersion);
	void SaveDirConfig(DirResponseInfo*);
	void SaveLastLoginSettings(UINT ServerID);
	bool CheckUpdate();

private:
	IGameApp*		m_pApp;
	LoginType		m_eLoginType;
	IPingMgr*		m_pPingMgr;
	HWND			m_hWnd;
	AppInfo			m_AppInfo;
	DWORD			m_dwWorldID;
	PingTaskInfo_List	m_listPingTask;
	DWORD			m_dwCountry;
	DWORD			m_dwChannel;
	HWND			m_hWndLoader;
	// Note: 使用IVersionUpdate组件来获取版本升级服务
	TTenioPtr<IVersionUpdater> m_spVersionUpdater;
};

