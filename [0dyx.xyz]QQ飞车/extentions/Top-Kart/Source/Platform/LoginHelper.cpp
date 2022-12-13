#include "Top-KartPCH.h"
#include ".\loginhelper.h"
#include "PingMgr_i.c"
#include "scriptutils.h"
#include "common.h"
#include "GlobalInterface.h"

CLoginHelper::CLoginHelper():
m_pApp(NULL),
m_pNetwork(NULL),
m_pPingMgr(NULL),
m_dwCookie(0),
m_dwCookiePing(0),
m_pScriptState(NULL)
{
}

CLoginHelper::~CLoginHelper(void)
{
	AppUnInit();
}

HRESULT CLoginHelper::AppInit(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;

		hr = AtlAdvise(m_pNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);

		CComPtr<IPingMgr> spPingMgr;
		hr = m_pNetwork->QueryInterface(IID_IPingMgr, (void**)&spPingMgr);
		KK_CHECK(hr);
		m_pPingMgr = spPingMgr;

		hr = m_pPingMgr->Initialize((IPingSink*)this);
		KK_CHECK(hr);

		m_pScriptState = GlobalInterface::m_pScriptState;
	}
KK_EXIT:
	return hr;
}

HRESULT CLoginHelper::AppUnInit()
{
	if ( m_dwCookie )
	{
		AtlUnadvise(m_pNetwork, IID_INetworkSink, m_dwCookie);
		m_dwCookie = 0;
	}
	if ( m_dwCookiePing )
	{
		AtlUnadvise(m_pPingMgr, IID_IPingSink, m_dwCookiePing);
		m_dwCookiePing = 0;
	}

	m_pApp = NULL;
	m_pNetwork = NULL;
	m_pPingMgr = NULL;
	return S_OK;
}

STDMETHODIMP CLoginHelper::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	HRESULT hr = S_OK;
	if ( Entity == eDirServer )
	{
		CResponseGetHallInfoPara* pPara = (CResponseGetHallInfoPara*)m_pNetwork->GetDirInfo();
		switch(Result)
		{
		case eLoginVersionRecommend:
		case eLoginSuccessed:
			{
				assert(pPara);
				for ( int i = 0; i < pPara->m_bLocationCount; i++ )
				{
					ServerInfoContainer si;
					m_RegionContainer[pPara->m_stLocationInfo[i].m_nLocationID] = si;
				}
				for ( int i = 0; i < pPara->m_bGameServerCount; i++ )
				{
					const STGameServerInfo& gsi = pPara->m_stGameServerInfo[i];
					ServerInfoContainer& si = m_RegionContainer[gsi.m_nLocationID];
					CommonServerInfo csi;
					csi.ID = gsi.m_iServerID;
					csi.IP = gsi.m_unServerIP;
					csi.Port = gsi.m_nServerPort;
					strncpy(csi.Name, gsi.m_szChildLocationName, MAXCHILDLOCATIONNAMELEN);
					si.push_back(csi);
				}
				StartTestRegions();

				CComPtr<IGameSettings> spSettings;
				hr = m_pApp->QueryInterface(IID_IGameSettings, (void**)&spSettings);
				KK_CHECK(hr);
				
				int nLocID = 0;
				spSettings->GetIntValue("LocID", &nLocID);
				RegionContainer::iterator itfind = m_RegionContainer.find(nLocID);
				if ( nLocID > 0 && itfind != m_RegionContainer.end() && itfind->second.size() > 0 )
				{
					m_pNetwork->LoginGameServer(itfind->second.begin()->ID);
				}
				else
				{
					TriggleScriptEventEx(m_pScriptState, "DoShowRegionDlg", "%d", 1);
				}
			}
			break;
		default:
			break;
		}
	}
	else if ( Entity == eGameServer )
	{
		switch(Result)
		{
		case eLoginFailed:
		case eLoginTimeout:
			break;
		default:
			break;
		}
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CLoginHelper::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	return S_OK;
}

STDMETHODIMP CLoginHelper::OnClose(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CLoginHelper::OnConnect(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CLoginHelper::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CLoginHelper::OnStartLogin(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CLoginHelper::NotifyPingResult(int nIndex, int nTTL, float fLossRate, int nAverageime)
{
	int nRegionID = m_PingIndex2RegionIDMap[nIndex];
	m_RegionID2PingValueMap[nRegionID] = nAverageime;
	m_pPingMgr->CancelPing(nIndex);
	TriggleScriptEventEx(m_pScriptState, "OnNotifyPingResult", "%d,%d", nRegionID, nAverageime);
	return S_OK;
}

#define PING_TIMEOUT	5000

void CLoginHelper::StartTestRegions()
{
	m_PingIndex2RegionIDMap.clear();
	for ( RegionContainer::iterator it = m_RegionContainer.begin(); it != m_RegionContainer.end(); it++ )
	{
		ServerInfoContainer& sis = it->second;
		if ( sis.size() > 0 )
		{
			CommonServerInfo& si = sis.back();
			int nPingIndex = 0;
			m_pPingMgr->PingServer(ntohl(si.IP), ntohs(si.Port), &nPingIndex, PING_TIMEOUT);
			m_PingIndex2RegionIDMap[nPingIndex] = it->first;
		}
	}
}

int CLoginHelper::GetRegionPingValue(int LocID)
{
	IDMap::iterator it = m_RegionID2PingValueMap.find(LocID);
	if ( it != m_RegionID2PingValueMap.end() )
	{
		return m_RegionID2PingValueMap[LocID];
	}
	return -1;
}

const STLocationInfo*	CLoginHelper::GetRegionInfo(int LocID)
{
	CResponseGetHallInfoPara* pPara = (CResponseGetHallInfoPara*)m_pNetwork->GetDirInfo();
	for ( int i = 0; i < pPara->m_bLocationCount; i++ )
	{
		const STLocationInfo* pLocInfo = &pPara->m_stLocationInfo[i];
		if ( pLocInfo->m_nLocationID == LocID )
		{
			return pLocInfo;
		}
	}
	return NULL;
}

