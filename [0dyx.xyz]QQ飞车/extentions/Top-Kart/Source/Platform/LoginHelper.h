#ifndef __LOGINHELPER_H__
#define __LOGINHELPER_H__

#include "framework/GameApp_i.h"
#include "Network/Network_i.h"
#include "Network/msg.h"
#include "PingMgr_i.h"
#include "script.h"
#include "list"
#include "map"
using namespace std;

typedef struct tagCommonServerInfo
{
	INT		ID;
	ULONG	IP;
	USHORT	Port;
	CHAR	Name[MAXCHILDLOCATIONNAMELEN];
}CommonServerInfo;

typedef list<CommonServerInfo>				ServerInfoContainer;
typedef map<USHORT, ServerInfoContainer>	RegionContainer;
typedef map<int, int>						IDMap;

class CLoginHelper : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLoginHelper>,
	public INetworkSink,
	public IPingSink
{
public:
	CLoginHelper();
	~CLoginHelper();

	HRESULT AppInit(IGameApp*);
	HRESULT AppUnInit();

	INT		GetRegionPingValue(int LocID);
	const STLocationInfo*	GetRegionInfo(int LocID);

	//INetworkSink
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult);
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead);
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode);
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode);
	STDMETHOD(OnCreate)(ServerEntity, int nErrorCode);
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode);

	//IPingSink
	STDMETHOD(NotifyPingResult)(int nIndex, int nTTL, float fLossRate, int nAverageime);

	BEGIN_COM_MAP(CLoginHelper)
		COM_INTERFACE_ENTRY(INetworkSink)
		COM_INTERFACE_ENTRY(IPingSink)
	END_COM_MAP()

protected:
	void StartTestRegions();

	IGameApp*		m_pApp;
	INetwork*		m_pNetwork;
	IPingMgr*		m_pPingMgr;
	DWORD			m_dwCookie;
	DWORD			m_dwCookiePing;
	CScriptState*	m_pScriptState;

public:
	RegionContainer	m_RegionContainer;
	IDMap			m_PingIndex2RegionIDMap;
	IDMap			m_RegionID2PingValueMap;
};

#endif