////////////////////////////////////////////////
//     FileName:    NetworkImpl.cpp
//     Author:      Fish
//     Date:        2006-9-18 15:56:00
//     Description: []
////////////////////////////////////////////////

#include "stdafx.h"
#include "NetworkImpl.h"
#include "Network/consts.h"
#include "Network/msg.h"
#include "Network/userdata.h"
#include "PingMgr_i.h"
#include "PingMgr_i.c"
#include "fileop.h"
#include "algorithm"
#include "utils.h"
#include "mycom.h"
#include "TimerMgr.h"
#include "client.h"
#include "CmdParse.h"
#include "CmdParse_acceptinvote.h"
#include "CmdParse_playgame.h"
#include "CmdParse_start.h"
#include "loaderdef.h"

CNetworkImpl::CNetworkImpl()
{
	m_pApp = NULL;
	m_eLoginType = eLoginNormal;
	m_pPingMgr = NULL;
	m_hWnd = NULL;
	m_dwWorldID = 1;
	m_dwCountry = 1;
	m_dwChannel = 1;
	m_hWndLoader = 0;
	memset(&m_AppInfo, 0, sizeof(m_AppInfo));
}

CNetworkImpl::~CNetworkImpl()
{
}

STDMETHODIMP CNetworkImpl::AppInit(IGameApp* App)
{
	ASSERT(App);

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		XError(FormatErrorMessage(err).c_str());
		return E_FAIL;
	}

	m_pApp = App;
	m_pApp->GetMainWnd(&m_AppInfo.hWnd);
	m_pApp->GetVersion(&m_AppInfo.Version);
	//m_pApp->GetLUVersion(&m_AppInfo.luVersion, &m_AppInfo.luoptVersion);
	GetLUVersion(m_AppInfo.luVersion, m_AppInfo.luoptVersion);

	CQQLoginSocket::SetSinkProxy((ISocketSinkProxy*)this);
	CDirSocket::SetSinkProxy((ISocketSinkProxy*)this);
	CGameSocket::SetSinkProxy((ISocketSinkProxy*)this);
	CShopSocket::SetSinkProxy((ISocketSinkProxy*)this);

	//初始化失败也不返回E_FAIL，否则其他模块拿不到Network,后果就严重了
	CTimerMgr::Create();
	InitLoginQQ();
	InitLoginDir();
	InitLoginGame(); 
	InitLoginShop();

	CreateObjectFromFile(PINGMGR_MODULE, CLSID_CPingMgr, IID_IPingMgr, (void**)&m_pPingMgr);
	ASSERT(m_pPingMgr);
	m_pPingMgr->Initialize(this);

	CComPtr<IGameSettings> spSettings;
	m_pApp->QueryInterface(IID_IGameSettings, (void**)&spSettings);
	if ( spSettings )
	{
		DWORD dwWorldID = 0;
		spSettings->GetIntValue("worldid", (INT*)&dwWorldID);
		if ( dwWorldID )
		{
			m_dwWorldID = dwWorldID;
		}
	}
	SetActiveWorld(m_dwWorldID);
	CheckLoginFromLoader();

	if ( m_spVersionUpdater )
	{
		m_spVersionUpdater->AddEventHandler(
			static_cast<IVersionUpdaterEvent*>(this), NULL);
	}

	return S_OK;
}

STDMETHODIMP CNetworkImpl::AppUnInit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( m_spVersionUpdater )
	{
		m_spVersionUpdater->RemoveEventHandler(
			static_cast<IVersionUpdaterEvent*>(this));
	}
	RELEASE(m_pPingMgr);
	CGameSocket::Close();
	CShopSocket::Close();
	CDirSocket::Close();
	CQQLoginSocket::Close();
	CTimerMgr::Destroy();
	WSACleanup();
	return S_OK;
}

#define MAX_WORLD_COUNT	10

STDMETHODIMP CNetworkImpl::SetActiveWorld(DWORD dwWorldID)
{
	char strProfile[MAX_PATH] = "";
	_fullpath(strProfile, "WorldDef.ini", MAX_PATH);
	char strApp[MAX_PATH] = "";
	sprintf(strApp, "World%d", dwWorldID);

	SOCKET_CREATE_INFO_LIST listSocketInfo;

	char strSvrList[MAX_PATH] = "";
	char strSvrListFullPath[MAX_PATH] = "";
	GetPrivateProfileString(strApp, "svrlist", "", strSvrList, MAX_PATH, strProfile);
	if ( strlen(strSvrList) > 0 )
	{
		_fullpath(strSvrListFullPath, strSvrList, MAX_PATH);
		LoadServerCFGFromFile(strSvrListFullPath, listSocketInfo);
	}
	for ( int i = 1; i < MAX_WORLD_COUNT; i++ )
	{
		if ( i == dwWorldID )
			continue;

		sprintf(strApp, "World%d", i);
		GetPrivateProfileString(strApp, "svrlist", "", strSvrList, MAX_PATH, strProfile);
		if ( strlen(strSvrList) > 0 )
		{
			_fullpath(strSvrListFullPath, strSvrList, MAX_PATH);
			LoadServerCFGFromFile(strSvrListFullPath, listSocketInfo);
		}	
	}
	CDirSocket::AddSocketInfoList(listSocketInfo);
	CDirSocket::SetActiveWorld(dwWorldID);
	return S_OK;
}

STDMETHODIMP CNetworkImpl::SetLoginType(UINT nLoginType)
{
	CGameSocket::SetLoginType(nLoginType);
	return S_OK;
}

STDMETHODIMP CNetworkImpl::StartLogin(UINT Uin, PBYTE Pwd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( Uin != 0 ) 
	{
		m_eLoginType = eLoginNormal;
		return LoginNormal(Uin, Pwd) == TRUE ? S_OK : E_FAIL;
	}
	else if ( LoginFromQQGame() )
	{
		m_eLoginType = eLoginFromQQGame;
		return S_OK;
	}
	else if ( LoginFromQQ() )
	{
		m_eLoginType = eLoginFromQQ;
		return S_OK;
	}
	else
	{
		ASSERT(0); //Unknown login type
		return E_FAIL;
	}
}

STDMETHODIMP CNetworkImpl::StartReLogin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	const QQUSERDATA* pUserData = GetUserData();
	return LoginNormal(pUserData->Uin, pUserData->byPwdHash2, 2);
}

STDMETHODIMP CNetworkImpl::CancelLogin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CQQLoginSocket::Close();
	CDirSocket::Close();
	CGameSocket::Close();
	Fire_OnResponseLogin(eGameServer, eLoginCanceled);
	return S_OK;
}

QQUSERDATA* CNetworkImpl::GetQQUserData()
{
	return CQQLoginSocket::GetUserData();
}

DirResponseInfo* CNetworkImpl::GetDirInfo()
{
	return CDirSocket::GetResponseInfo();
}

GameResponseInfo* CNetworkImpl::GetGameInfo()
{
	return CGameSocket::GetResponseInfo();
}

SOCKET_CREATE_INFO* CNetworkImpl::GetSocketInfo(ServerEntity Entity)
{
	LPSOCKET_CREATE_INFO pi = NULL;
	switch(Entity)
	{
	case eQQServer:
		pi = CQQLoginSocket::GetSocketInfo();
		break;
	case eDirServer:
		pi = CDirSocket::GetSocketInfo();
		break;
	case eGameServer:
		pi = CGameSocket::GetSocketInfo();
		break;
	case eShopServer:
		pi = CShopSocket::GetSocketInfo();
		break;
	default:
		assert(0);
		break;
	}
	return pi;
}

STDMETHODIMP CNetworkImpl::RequestGameServer(UINT MsgID, PBYTE Data, UINT PlayerID, BYTE DestFE, SHORT DestID, UINT MsgType)
{
	if ( !CGameSocket::GetResponseInfo() )
		return S_FALSE;

	INT nPlayerID = CGameSocket::GetResponseInfo()->m_nPlayerID;
	if ( DestFE == FE_PLAYER && DestID <= 0 )
		DestID = nPlayerID;
	return CGameSocket::RequestGameServer(MsgID, Data, nPlayerID, DestFE, DestID, MsgType) ? S_OK : E_FAIL;
}

STDMETHODIMP CNetworkImpl::RequestShopServer(UINT MsgID, PBYTE Data, UINT MsgType)
{
	return CShopSocket::RequestShop(MsgID, MsgType, Data) ? S_OK : E_FAIL;
}

STDMETHODIMP CNetworkImpl::SendTcpP2pData(SHORT PlayerID, INT PlayerNum, SHORT* PlayerIDs, UINT* PlayerUins, PBYTE Data, INT DataLen)
{
	INT nPlayerID = CGameSocket::GetResponseInfo()->m_nPlayerID;
	return CGameSocket::SendTcpP2pData(nPlayerID, PlayerNum, PlayerIDs, PlayerUins, Data, DataLen) ? S_OK : E_FAIL;
}
// -------------------------------------------------------------------------
STDMETHODIMP CNetworkImpl::OnStartLogin(ServerEntity Entity, int nErrorCode)
{
	Fire_OnStartLogin(Entity, nErrorCode);
	return S_OK;
}

STDMETHODIMP CNetworkImpl::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	if ( Entity == eQQServer )
	{
		if ( Result == eLoginSuccessed )
		{
			if ( FileExists("GameSvrInfo.ini") )
			{
				LoginGameServer(0, 0, 0);
			}
			else
			{
				LoginDir();
			}
		}
	}
	else if ( Entity == eDirServer )
	{
		if ( CheckUpdate() )
		{//在强制更新，不能继续登录

		}
		else if ( Result != eLoginTimeout && Result != eLoginFailed )
		{
			DirResponseInfo* pDirInfo = CDirSocket::GetResponseInfo();
			if ( pDirInfo )
			{
				SaveDirConfig(pDirInfo);
				CShopSocket::Create();		
				for ( int i = 0; i < pDirInfo->m_byShopServerCount; i++ )
				{
					SOCKET_CREATE_INFO kSocketInfo;
					memset(&kSocketInfo, 0, sizeof(kSocketInfo));
					int nSvrIndex = i;
					STServerBaseInfo* pSvrInfo = &pDirInfo->m_astShopServerInfo[nSvrIndex];
					in_addr addr;
					addr.S_un.S_addr = pSvrInfo->m_uiServerIP;
					strncpy(kSocketInfo.szHostAddr, inet_ntoa(addr), 100);
					kSocketInfo.uPorts[0] = pSvrInfo->m_ushServerPort;
					kSocketInfo.uServerID = pSvrInfo->m_iServerID;
					kSocketInfo.nRcvBufSize = RCV_BUF_LEN;
					kSocketInfo.nSndBufSize = SND_BUF_LEN;
					CShopSocket::AddSocketInfo(&kSocketInfo);
				}
				DWORD dwCountry = RandCountry();
				LoginGameServer(dwCountry, RandChannel(dwCountry), 0);
			}
		}
	}
	else if ( Entity == eGameServer )
	{
		GameResponseInfo* pGameInfo = CGameSocket::GetResponseInfo();
		if ( pGameInfo && pGameInfo->m_uiLoginType == ELT_Multi )
		{
			return S_OK;
		}
		m_listPingTask.clear();
		if ( Result == eLoginRequireReg || Result == eLoginSuccessed )
		{
			//替换签名
			QQUSERDATA* pUserInfo = CQQLoginSocket::GetUserData();
			memcpy(pUserInfo->byKey, pGameInfo->m_szKeyGameData, pGameInfo->m_bKeyGameDataLength);

			//初始化商店登陆必须信息
			ShopLoginInfo kLoginInfo;
			kLoginInfo.uin = pUserInfo->Uin;
			memcpy(kLoginInfo.byKey, pUserInfo->byKey, KEY_LEN);
			kLoginInfo.byGameSignatureLen = pUserInfo->byGameSignatureLen;
			memcpy(kLoginInfo.byGameSignature, pUserInfo->byGameSignature, pUserInfo->byGameSignatureLen);
			CShopSocket::Init(&kLoginInfo);

			SaveLastLoginSettings(CGameSocket::GetSocketInfo()->uServerID);
		}
	}
	if ( m_hWndLoader )
	{
		NotifyLoginResult kLoginResult;
		ZeroMemory(&kLoginResult, sizeof(kLoginResult));
		kLoginResult.hWnd = m_AppInfo.hWnd;
		kLoginResult.dwEntity = Entity;
		kLoginResult.nLoginResult = Result;
		if ( Entity == eGameServer )
		{
			strncpy(kLoginResult.strErrorMessage, CGameSocket::GetResponseInfo()->m_szReason, MAX_MESSAGE_LEN - 1);
		}
		else if ( Entity == eDirServer )
		{
			strncpy(kLoginResult.strErrorMessage, CDirSocket::GetResponseInfo()->m_szReason, MAX_MESSAGE_LEN - 1);
		}
		else
		{
			strcpy(kLoginResult.strErrorMessage, "");
		}
		COPYDATASTRUCT kCopyData;
		kCopyData.dwData = EDT_NotifyLoginResult;
		kCopyData.cbData = sizeof(kLoginResult);
		kCopyData.lpData = &kLoginResult;
		::SendMessage(m_hWndLoader, WM_COPYDATA, (WPARAM)m_AppInfo.hWnd, (LPARAM)&kCopyData);
	}
	Fire_OnResponseLogin(Entity, Result);
	return S_OK;
}

STDMETHODIMP CNetworkImpl::OnConnect(ServerEntity Entity, int nErrorCode)
{
	LogText(nErrorCode == 0 ? "Socket Connect!\n" : "Socket Connect Failed!\n");
	Fire_OnConnect(Entity, nErrorCode);
	return S_OK;
}

STDMETHODIMP CNetworkImpl::OnReceive(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CNetworkImpl::OnSend(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CNetworkImpl::OnClose(ServerEntity Entity, int nErrorCode)
{
	LogText("Socket Closed!\n");
	Fire_OnClose(Entity, nErrorCode);
	return S_OK;
}

STDMETHODIMP CNetworkImpl::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, 
										   PVOID Data, INT DataLen, PVOID csHead, PVOID msgHead)
{
	if ( Entity == eGameServer && MsgID == ID_SMC_NOTIFYTESTNETSPEED )
	{
		CNotifyTestNetSpeedPara* pData = (CNotifyTestNetSpeedPara*)Data;
		CAckTestNetSpeedPara aPara;
		aPara.m_unStartSec = pData->m_unStartSec;
		aPara.m_unUSec = pData->m_unUSec;
		RequestGameServer(ID_SMC_NOTIFYTESTNETSPEED, (PBYTE)&aPara, 0, FE_PLAYER, 0, Ack);
		return S_OK;
	}

	Fire_OnServerMessage(Entity, MsgID, MsgType, (PBYTE)Data, DataLen, (PBYTE)csHead, (PBYTE)msgHead);
	return S_OK;
}

// -------------------------------------------------------------------------
#define LOGIN_FILE_FROM_QQGAME "myinfo.data"

BOOL CNetworkImpl::InitLoginQQ()
{
	SOCKET_CREATE_INFO_LIST listSocketInfo;
	if ( !LoadServerCFG(eQQServer, listSocketInfo) )
		return FALSE;
	CQQLoginSocket::Create(sizeof(short));
	CQQLoginSocket::SetSocketInfoList(listSocketInfo);
	return TRUE;
}

BOOL CNetworkImpl::InitLoginDir()
{
	SOCKET_CREATE_INFO_LIST listSocketInfo;
	if ( !LoadServerCFG(eDirServer, listSocketInfo) )
		return FALSE;
	CDirSocket::Create(sizeof(UINT));
	CDirSocket::SetSocketInfoList(listSocketInfo);
	return TRUE;
}

BOOL CNetworkImpl::InitLoginGame()
{
	SOCKET_CREATE_INFO_LIST listSocketInfo;
	if ( !LoadServerCFG(eGameServer, listSocketInfo) )
		return FALSE;
	CGameSocket::Create(sizeof(UINT));
	CGameSocket::SetSocketInfoList(listSocketInfo);
	return TRUE;
}

BOOL CNetworkImpl::InitLoginShop()
{
	SOCKET_CREATE_INFO_LIST listSocketInfo;
	if ( !LoadServerCFG(eShopServer, listSocketInfo) )
		return FALSE;
	CShopSocket::Create(sizeof(UINT));
	CShopSocket::SetSocketInfoList(listSocketInfo);
	return TRUE;
}

BOOL CNetworkImpl::LoginDir()
{
	DirLoginInfo kLoginInfo;
	memset(&kLoginInfo, 0, sizeof(kLoginInfo));
	const QQUSERDATA* pUserData = GetUserData();
	kLoginInfo.uin = pUserData->Uin;
	kLoginInfo.byGameSignatureLen = pUserData->byGameSignatureLen;
	memcpy(kLoginInfo.byGameSignature, pUserData->byGameSignature, GAME_SIGNATURE_LEN);
	memcpy(kLoginInfo.byKey, pUserData->byKey, KEY_LEN);
	kLoginInfo.dwWorldID = m_dwWorldID;
	kLoginInfo.dwTime = MyGetTime();
	kLoginInfo.dwVersion = m_AppInfo.Version;
	kLoginInfo.dwLuVersion = m_AppInfo.luVersion;
	kLoginInfo.dwLuOptVersion = m_AppInfo.luoptVersion;
	memcpy(kLoginInfo.szFileHash, g_szHashString, FILE_HASH_LEN);
	CDirSocket::Init(&kLoginInfo);
	CDirSocket::Connect();
	return TRUE;
}

STGameServerInfo* CNetworkImpl::FindSvrInfo(DWORD dwSvrID)
{
	DirResponseInfo* pDirInfo = CDirSocket::GetResponseInfo();
	if ( !pDirInfo )
		return NULL;

	for ( int c = 0; c < pDirInfo->m_byCountryCount; c++ )
	{
		STCountryInfo* pCountryInfo = &pDirInfo->m_astCountryInfo[c];
		for ( int n = 0; n < pCountryInfo->m_byChannelNum; n++ )
		{
			STChannelInfo* pChannelInfo = &pCountryInfo->m_astChannelInfo[n];
			for ( int i = 0; i < pChannelInfo->m_byIDCNum; i++ )
			{
				STIDCServerInfo* pIDCInfo = &pChannelInfo->m_astIDCServerInfo[i];
				for ( int s = 0; s < pIDCInfo->m_byGameServerNum; s++ )
				{
					STGameServerInfo* pSvrInfo = &pIDCInfo->m_astGameServerInfo[s];
					if ( pSvrInfo->m_iServerID == dwSvrID )
					{
						return pSvrInfo;
					}
				}
			}
		}
	}
	XWaring("no svrid found %d", dwSvrID);
	return NULL;
}

STChannelInfo* CNetworkImpl::FindChannelInfo(DWORD dwCountry, DWORD dwChannel)
{
	DirResponseInfo* pDirInfo = CDirSocket::GetResponseInfo();
	if ( !pDirInfo )
		return NULL;

	for ( int c = 0; c < pDirInfo->m_byCountryCount; c++ )
	{
		STCountryInfo* pCountryInfo = &pDirInfo->m_astCountryInfo[c];
		if ( pCountryInfo->m_byCountryID == dwCountry )
		{
			if ( dwChannel == 0 )
			{
				int nChannelIndex = rand() % pCountryInfo->m_byChannelNum;
				dwChannel = pCountryInfo->m_astChannelInfo[nChannelIndex].m_byChannelID;
			}

			for ( int n = 0; n < pCountryInfo->m_byChannelNum; n++ )
			{
				STChannelInfo* pChannelInfo = &pCountryInfo->m_astChannelInfo[n];
				if ( pChannelInfo->m_byChannelID == dwChannel )
				{
					return pChannelInfo;
				}
			}
		}
	}
	XWaring("no channel found with country %d, channel %d", dwCountry, dwChannel);
	return NULL;
}

void CNetworkImpl::InitGameSocket()
{
	const QQUSERDATA* pUserData = CQQLoginSocket::GetUserData();
	GameLoginInfo kLoginInfo;
	kLoginInfo.uin = pUserData->Uin;
	memcpy(kLoginInfo.byKey, pUserData->byKey, KEY_LEN);
	kLoginInfo.byGameSignatureLen = pUserData->byGameSignatureLen;
	memcpy(kLoginInfo.byGameSignature, pUserData->byGameSignature, GAME_SIGNATURE_LEN);
	kLoginInfo.nTime = GetTickCount();
	kLoginInfo.nVersion = m_AppInfo.Version;
	strncpy(kLoginInfo.szNickName, pUserData->szNickName, NICK_NAME_LEN);
	kLoginInfo.byGender = pUserData->byGender;
	kLoginInfo.byIconID = pUserData->nFace;
	kLoginInfo.nIdentity = pUserData->dwUinFlag;
	CGameSocket::Init(&kLoginInfo);
}

STDMETHODIMP CNetworkImpl::LoginGameServer(DWORD dwCountry, DWORD dwChannel, DWORD dwSvrID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ( !CDirSocket::GetResponseInfo() )
	{	//本地有GameSvrInfo.ini时会到这里，作为默认快捷登陆
		InitGameSocket();
		CGameSocket::Reset();
		CGameSocket::Connect();
	}
	else
	{
		if ( dwSvrID )	//追踪玩家的时候直接传入他的SvrID
		{
			STGameServerInfo* pSvrInfo = FindSvrInfo(dwSvrID);
			if ( pSvrInfo )
			{
				SOCKET_CREATE_INFO kCreateInfo;
				memset(&kCreateInfo, 0, sizeof(kCreateInfo));
				in_addr addr;
				addr.S_un.S_addr = pSvrInfo->m_uiServerIP;
				strncpy(kCreateInfo.szHostAddr, inet_ntoa(addr), MAX_HOSTADDR);
				kCreateInfo.uPorts[0] = pSvrInfo->m_ushTcpPort;
				kCreateInfo.uServerID = pSvrInfo->m_iServerID;
				kCreateInfo.uUDPPorts[0] = pSvrInfo->m_ushUdpPort;
				kCreateInfo.nRcvBufSize = RCV_BUF_LEN;
				kCreateInfo.nSndBufSize = SND_BUF_LEN;
				CGameSocket::Reset();
				SOCKET_CREATE_INFO_LIST& listSocketInfo = CGameSocket::GetSocketInfoList();
				listSocketInfo.push_front(kCreateInfo);
				CGameSocket::Connect();
				return S_OK;
			}
		}
		//如果没有指定频道，就随机一个
		if ( dwChannel == 0 )
			dwChannel = RandChannel(dwCountry);

		if ( dwCountry == 0 )
		{	//没有指定国家，就登陆边境服务器
			CGameSocket::ClearSocketInfoList();
			for ( int i = 0; i < CDirSocket::GetResponseInfo()->m_byBorderServerCount; i++ )
			{
				STCommGameServerInfo* pSvrInfo = &CDirSocket::GetResponseInfo()->m_astBorderServerInfo[i];
				SOCKET_CREATE_INFO kCreateInfo;
				memset(&kCreateInfo, 0, sizeof(kCreateInfo));
				in_addr addr;
				addr.S_un.S_addr = pSvrInfo->m_stServerInfo.m_uiServerIP;
				strncpy(kCreateInfo.szHostAddr, inet_ntoa(addr), MAX_HOSTADDR);
				kCreateInfo.uPorts[0] = pSvrInfo->m_stServerInfo.m_ushTcpPort;
				kCreateInfo.uServerID = pSvrInfo->m_stServerInfo.m_iServerID;
				kCreateInfo.uUDPPorts[0] = pSvrInfo->m_stServerInfo.m_ushUdpPort;
				kCreateInfo.nRcvBufSize = RCV_BUF_LEN;
				kCreateInfo.nSndBufSize = SND_BUF_LEN;
				CGameSocket::Create();
				CGameSocket::AddSocketInfo(&kCreateInfo);				
			}
		}
		else
		{
			if ( dwCountry == m_dwCountry && m_listPingTask.size() > 0 )
			{	//如果之前已经Ping测试过了，直接从缓存中找
				DWORD dwLocID = FindBestLocID(dwChannel);
				if ( dwLocID )
				{
					LoginGameServer(dwCountry, dwChannel, dwLocID, true);
					goto KK_EXIT;
				}
			}
			PingChannel(FindChannelInfo(dwCountry, dwChannel));
		}
	}
KK_EXIT:
	m_dwCountry = dwCountry;
	m_dwChannel = dwChannel;
	return S_OK;
}

DWORD CNetworkImpl::RandCountry()
{
	DirResponseInfo* pDirInfo = CDirSocket::GetResponseInfo();
	if ( !pDirInfo )
		return 0;
	if ( pDirInfo->m_byCountryCount == 0 )
	{
		XError("没有国家信息，无法登录!");
		return 0;
	}
	int nIndex = rand() % pDirInfo->m_byCountryCount;
	return pDirInfo->m_astCountryInfo[nIndex].m_byCountryID;
}

DWORD CNetworkImpl::RandChannel(DWORD dwCountry)
{
	DirResponseInfo* pDirInfo = CDirSocket::GetResponseInfo();
	if ( !pDirInfo )
		return 0;
	for ( int i = 0; i < pDirInfo->m_byCountryCount; i++ )
	{
		STCountryInfo* pCountryInfo = &pDirInfo->m_astCountryInfo[i];
		if ( pCountryInfo->m_byCountryID == dwCountry )
		{
			int nIndex = rand() % pCountryInfo->m_byChannelNum;
			return pCountryInfo->m_astChannelInfo[nIndex].m_byChannelID;
		}
	}
	return 0;
}

void CNetworkImpl::PingChannel(STChannelInfo* pChannelInfo)
{
	if ( !pChannelInfo )
		return;
	
	StopPing();
	//m_listPingTask.clear();

	for ( int i = 0; i < pChannelInfo->m_byIDCNum; i++ )
	{
		STIDCServerInfo* pIDCInfo = &pChannelInfo->m_astIDCServerInfo[i];
		srand(GetTickCount());
		if ( pIDCInfo->m_byGameServerNum == 0 )
			continue;

		int nSvrIndex = rand() % pIDCInfo->m_byGameServerNum;
		STGameServerInfo* pSvrInfo = &pIDCInfo->m_astGameServerInfo[nSvrIndex];
		int nPingIndex = 0;
		UINT nSvrIP = ntohl(pSvrInfo->m_uiServerIP);
		USHORT nSvrPort = (pSvrInfo->m_ushTcpPort);
		m_pPingMgr->PingServer(nSvrIP, nSvrPort, &nPingIndex, 3000);

		PingTaskInfo kTaskInfo;
		kTaskInfo.dwChannel = pChannelInfo->m_byChannelID;
		kTaskInfo.dwLocID = pIDCInfo->m_ushLocationID;
		kTaskInfo.nSvrIP = nSvrIP;
		kTaskInfo.nSvrPort = nSvrPort;
		kTaskInfo.nPingIndex = nPingIndex;
		kTaskInfo.nPingCount = 0;
		kTaskInfo.bStoped = false;
		kTaskInfo.nPingValue = 0;
		m_listPingTask.push_back(kTaskInfo);
	}
}

void CNetworkImpl::StopPing()
{
	assert(m_pPingMgr);
	PingTaskInfo_List::iterator it = m_listPingTask.begin();
	for ( ; it != m_listPingTask.end(); ++it )
	{
		PingTaskInfo* pTaskInfo = &*it;
		if ( !pTaskInfo->bStoped )
		{
			m_pPingMgr->CancelPing(pTaskInfo->nPingIndex);
			pTaskInfo->bStoped = true;
		}
	}
}

STDMETHODIMP CNetworkImpl::NotifyPingResult(int nIndex, int nTTL, float fLossRate, int nAverageime)
{
	if ( m_listPingTask.size() == 0 )
		return S_OK;

	int nPingMaxCount = 0;
	PingTaskInfo_List::iterator it = m_listPingTask.begin();
	int nTimeoutCount = 0;
	for ( ; it != m_listPingTask.end(); ++it )
	{
		PingTaskInfo* pTaskInfo = &*it;
		if ( pTaskInfo->nPingIndex == nIndex )
		{
			if ( pTaskInfo->nPingValue == 0 )
			{
				pTaskInfo->nPingValue = (nTTL == 0 ? INT_MAX : nTTL);
			}
			else
			{
				pTaskInfo->nPingValue = (pTaskInfo->nPingValue + nAverageime) / 2;
			}
			pTaskInfo->nPingCount++;
			m_pPingMgr->CancelPing(pTaskInfo->nPingIndex);
			if ( pTaskInfo->nPingCount >= MAX_PINGCOUNT )
			{
			}
			else if ( nTTL != 0 )	//不是超时完成才重新Ping，否则也无法PingServer
			{
				int nPingIndex = 0;
				if ( S_OK == m_pPingMgr->PingServer(pTaskInfo->nSvrIP, pTaskInfo->nSvrPort, &nPingIndex, 3000) )
				{
					pTaskInfo->nPingIndex = nPingIndex;
				}
				else
				{	//没法再Ping了，取权值
					pTaskInfo->nPingCount = MAX_PINGCOUNT;
				}
			}
		}
		if ( pTaskInfo->nPingCount == MAX_PINGCOUNT )
			nPingMaxCount++;
		if ( pTaskInfo->nPingValue == INT_MAX )
			nTimeoutCount++;
	}

	std::sort(m_listPingTask.begin(), m_listPingTask.end());
	DWORD dwLocID = FindBestLocID(m_dwChannel);
	//所有的服务器都Ping了最大次数了，可以开始选服务器登陆(或者全部Ping都超时了，也尝试登录)
	if ( nPingMaxCount == 1 /*m_listPingTask.size()*/ || nTimeoutCount == m_listPingTask.size() )
	{
		LoginGameServer(m_dwCountry, m_dwChannel, dwLocID, true);
	}
	return S_OK;
}

void CNetworkImpl::LoginGameServer(DWORD dwCountry, DWORD dwChannel, DWORD dwLocID, bool bLoc)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	STChannelInfo* pChannelInfo = FindChannelInfo(dwCountry, dwChannel);
	if ( !pChannelInfo )
	{
		XWaring("FindChannelInfo(dwCountry=%d,dwChannel=%d) failed", dwCountry, dwChannel);
		return;
	}
	/*
	登录IDC选择，优先登录速度最快的IDC，其他IDC作为备选加入服务器列表
	*/
	CGameSocket::Create();
	SOCKET_CREATE_INFO_LIST listSocketInfo1;
	SOCKET_CREATE_INFO_LIST listSocketInfo2;
	for ( int i = 0; i < pChannelInfo->m_byIDCNum; i++ )
	{
		STIDCServerInfo* pIDCInfo = &pChannelInfo->m_astIDCServerInfo[i];
		//if ( pIDCInfo->m_ushLocationID == dwLocID )
		{
			for ( int n = 0; n < pIDCInfo->m_byGameServerNum; n++ )
			{
				STGameServerInfo* pSvrInfo = &pIDCInfo->m_astGameServerInfo[n];
				SOCKET_CREATE_INFO kCreateInfo;
				memset(&kCreateInfo, 0, sizeof(kCreateInfo));
				in_addr addr;
				addr.S_un.S_addr = pSvrInfo->m_uiServerIP;
				strncpy(kCreateInfo.szHostAddr, inet_ntoa(addr), MAX_HOSTADDR);
				kCreateInfo.uPorts[0] = pSvrInfo->m_ushTcpPort;
				kCreateInfo.uServerID = pSvrInfo->m_iServerID;
				kCreateInfo.uUDPPorts[0] = pSvrInfo->m_ushUdpPort;
				kCreateInfo.nRcvBufSize = RCV_BUF_LEN;
				kCreateInfo.nSndBufSize = SND_BUF_LEN;
				//CGameSocket::AddSocketInfo(&kCreateInfo);
				if ( pIDCInfo->m_ushLocationID == dwLocID )
				{
					listSocketInfo1.push_back(kCreateInfo);
				}
				else
				{
					listSocketInfo2.push_back(kCreateInfo);
				}
			}
		}
	}
	CGameSocket::SetSocketInfoList(listSocketInfo1);
	CGameSocket::AddSocketInfoList(listSocketInfo2);
	InitGameSocket();
	CGameSocket::Reset();
	CGameSocket::Connect();
}

DWORD CNetworkImpl::FindBestLocID(DWORD dwChannel)
{
	std::sort(m_listPingTask.begin(), m_listPingTask.end());
	PingTaskInfo_List::iterator it = m_listPingTask.begin();
	for ( ; it != m_listPingTask.end(); ++it )
	{
		PingTaskInfo* pTaskInfo = &*it;
		if ( pTaskInfo->dwChannel == dwChannel )
		{
			return pTaskInfo->dwLocID;
		}
	}
	return 0;
}

BOOL CNetworkImpl::LoginNormal(UINT Uin, const BYTE* Pwd, INT HashType)
{
	QQLoginInfo kLoginInfo;
	kLoginInfo.uin = Uin;
	memcpy(kLoginInfo.byPwd, Pwd, PWD_LEN * sizeof(BYTE));
	kLoginInfo.nHashType = HashType;
	CQQLoginSocket::Init(&kLoginInfo);
	CQQLoginSocket::Connect();
	return TRUE;
}

BOOL CNetworkImpl::LoginFromQQGame()
{
	LPSTR szFileName = LOGIN_FILE_FROM_QQGAME;
	BOOL bRet = FALSE;
	if ( !FileExists(szFileName) )
		return FALSE;

	FILE* fp = fopen(szFileName, "rb");
	if ( !fp )
		goto EXIT;

	fseek(fp, 0, SEEK_END);
	int nFileLen = ftell(fp);
	if ( nFileLen < sizeof(QQGAMELOGINFILEDATA) )
		goto EXIT;
	QQGAMELOGINFILEDATA aLoginData;
	fread(&aLoginData, sizeof(QQGAMELOGINFILEDATA), 1, fp);

	QQUSERDATA* pQQUserData = CQQLoginSocket::GetUserData();
	pQQUserData->Uin = aLoginData.Uin;
	strncpy(pQQUserData->szNickName, aLoginData.szNick, NICK_NAME_LEN);
	pQQUserData->byGender = aLoginData.nGender;
	pQQUserData->byAge = aLoginData.nAge;
	pQQUserData->nFace = aLoginData.nFaceID;
	memcpy(pQQUserData->byKey, aLoginData.byKey, aLoginData.nKeyLen);
	pQQUserData->byGameSignatureLen = aLoginData.nGameSignatureLen;
	memcpy(pQQUserData->byGameSignature, aLoginData.byGameSignature, GAME_SIGNATURE_LEN);
	memcpy(pQQUserData->byUserServiceBitmap, aLoginData.byUserServiceBitmap, BITMAPLEN);
	pQQUserData->dwUinFlag = aLoginData.dwUinFlag;

EXIT:
	if ( fp )
		fclose(fp);
	DeleteFile(szFileName);
	return bRet;
}

BOOL CNetworkImpl::LoginFromQQ()
{
	CString strCmdLine = GetCommandLine();
	CString strQQCmd = CCmdParse::GetCmdStr(strCmdLine);
	if (0 == strQQCmd.Compare("/START"))
	{
		CCmdParse_start CmdPara;
		CmdPara.SetCmdLine(strCmdLine);
		BYTE byHash[16];
		CmdPara.GetHash(byHash);

		// 设置本地玩家属性
		SetLoginPlayer(CmdPara.GetUIN(), byHash, CmdPara.GetPWHashTime());
		return TRUE;
	}
	else if (0 == strQQCmd.Compare("/PLAYGAME"))
	{
		CCmdParse_playgame CmdPara;
		CmdPara.SetCmdLine(strCmdLine);

		BYTE byHash[16];
		CmdPara.GetHash(byHash);

		// 设置本地玩家属性
		SetLoginPlayer(CmdPara.GetUIN(), byHash, CmdPara.GetPWHashTime());

		// to keep things uniform we send WM_COPYDATA message as well as QQ
		QQ_REQUEST_PLAY_GAME qrpg;
		CmdPara.GetKey((BYTE*)qrpg.m_chKey);
		qrpg.m_shGameID = CmdPara.GetGameID();

		HWND hQQWnd = (HWND)CmdPara.GetWndHandle();

		COPYDATASTRUCT cds;
		cds.dwData = WM_QQ_REQUEST_PLAY_GAME;
		cds.cbData = sizeof(QQ_REQUEST_PLAY_GAME);
		cds.lpData = (PVOID)&qrpg;

		HWND hWnd = ::FindWindow(NULL, "QQTIPCWnd");
		if (!IsWindow(hWnd))
		{
			ASSERT(FALSE);
			return FALSE;
		}

		::SendMessage(hWnd, WM_COPYDATA, (WPARAM)hQQWnd, (LPARAM)(LPVOID)&cds);
		return TRUE;
	}
	else if (0 == strQQCmd.Compare("/ACCEPTINVOTE"))
	{
		CCmdParse_acceptinvote CmdPara;
		CmdPara.SetCmdLine(strCmdLine);
		BYTE byHash[16];
		CmdPara.GetHash(byHash);

		// 设置玩家登陆信息
		SetLoginPlayer(CmdPara.GetUIN(), byHash, CmdPara.GetPWHashTime());

		// to keep things uniform we send WM_COPYDATA message as well as QQ
		QQ_REQUEST_ACCEPT_INVITE qrai;
		ZeroMemory(&qrai, sizeof(qrai));
		qrai.m_shServerID = CmdPara.GetServerID();
		qrai.m_shSectionID = CmdPara.GetSectionID();
		qrai.m_shRoomID = CmdPara.GetRoomID();
		qrai.m_shGameID = 100;

		HWND hQQWnd = (HWND)CmdPara.GetWndHandle();

		COPYDATASTRUCT cds;
		cds.dwData = WM_QQ_REQUEST_ACCEPT_INVITE;
		cds.cbData = sizeof(QQ_REQUEST_ACCEPT_INVITE);
		cds.lpData = (PVOID)&qrai;

		HWND hWnd = ::FindWindow(NULL, "QQTIPCWnd");
		if (!IsWindow(hWnd))
		{
			ASSERT(FALSE);
			return FALSE;
		}

		::SendMessage(hWnd, WM_COPYDATA, (WPARAM)hQQWnd, (LPARAM)(LPVOID)&cds);
		return TRUE;
	}
	return FALSE;
}

void CNetworkImpl::SetLoginPlayer(int nUin, const BYTE* byPwd, int iHashType)
{
	QQUSERDATA* pQQUserData = CQQLoginSocket::GetUserData();
	BYTE byHash1[MD5_DIGEST_LENGTH];
	BYTE byHash2[MD5_DIGEST_LENGTH];
	if( 0 == iHashType )
	{		
		::Md5HashBuffer(byHash1, byPwd, byteslen(byPwd, MD5_DIGEST_LENGTH));
		::Md5HashBuffer(byHash2, byHash1, MD5_DIGEST_LENGTH);
	}
	else if( 1 == iHashType )
	{
		::memcpy(byHash1, byPwd, 16);
		::Md5HashBuffer(byHash2, byHash1, MD5_DIGEST_LENGTH);
	}
	else if ( 2 == iHashType )
	{
		::memcpy(byHash2, byPwd, MD5_DIGEST_LENGTH);
	}		
	else
	{
		ASSERT(0);
		return;
	}
	memcpy(pQQUserData->byKey, byHash2, MD5_DIGEST_LENGTH);
}

void CNetworkImpl::SaveLastLoginSettings(UINT ServerID)
{
	CComQIPtr<IGameSettings> spSettings = m_pApp;
	if ( spSettings == NULL )
		return;

	INT bSaveUin = FALSE;
	spSettings->GetIntValue("SaveUin", &bSaveUin);
	if ( bSaveUin )
	{
		CUserDataProfile aProfile;
		aProfile.SetUserData(&UserData(CQQLoginSocket::GetUserData()->Uin, NULL));
	}
}

void RunLU(void)
{
	ShellExecute(NULL, "open", "lu.exe", NULL, NULL, SW_SHOW);
}

bool CNetworkImpl::CheckUpdate()
{
	DirResponseInfo* pDirInfo = CDirSocket::GetResponseInfo();
	if ( !pDirInfo )
		return false;

	/*
	pDirInfo->m_ushResultID = EDLR_Err_VersionLimit;
	pDirInfo->m_uiLastMasterVersion = 2;
	pDirInfo->m_uiLastSlaveVersion = 2;
	pDirInfo->m_uiMasterFullVersion = 0;
	pDirInfo->m_uiSlaveFullVersion = 0;
	*/

	BOOL bForceUpdate = FALSE;	//是否强制
	BOOL bUpdate = FALSE;
	switch( pDirInfo->m_ushResultID )
	{
	case EDLR_Succeed:	
		break;
	case EDLR_Err_IdentityLimit:
		break;
	case EDLR_Err_VersionLimit:
		bForceUpdate = TRUE;
		bUpdate = TRUE;
		break;
	case EDLR_Err_VersionRecommend:
		bUpdate = TRUE;
		break;
	case EDLR_Err_FileCracked:
		::MessageBox(m_AppInfo.hWnd, pDirInfo->m_szReason, NULL, MB_OK | MB_ICONEXCLAMATION );
		bForceUpdate = TRUE;
		bUpdate = TRUE;
		break;
	}
	if ( !bUpdate )
		return false;

	if ( !pDirInfo->m_byDownloadServerNum )
	{
		XWaring("无下载服务器信息，无法更新！");
		return false;
	}

	// 外部升级服务器模式：
	PACKAGECOREINFOGROUP stPackageGroup = {0};

	int nPackageIndex = 0;
	TCHAR szPackName[MAX_PATH] = "";
	//服务器最新完整包版本号不为空而且大于本地前台更新版本号的时候需要下载完整包
	if ( pDirInfo->m_uiMasterFullVersion && pDirInfo->m_uiMasterFullVersion > m_AppInfo.luVersion )
	{
		_stprintf(szPackName, "setup_%d.EXE", pDirInfo->m_uiMasterFullVersion);

		PACKAGECOREINFO& stPacakgeInfo = stPackageGroup.stPackageCoreInfo[nPackageIndex++];
		stPacakgeInfo.dwPackageVersion = pDirInfo->m_uiMasterFullVersion;
		_tcscpy(stPacakgeInfo.szPackageName, szPackName);	// 升级包的名称
		_tcscpy(stPacakgeInfo.szPackageFileName, szPackName);	// 本地保存文件名
		stPacakgeInfo.dwUpdateStrategy = bForceUpdate ? EUS_FORCE_TIPDOWNLOAD : EUS_ADVISE_TIPDOWNLOAD;	
	}
	if ( pDirInfo->m_uiLastMasterVersion > max(m_AppInfo.luVersion, pDirInfo->m_uiMasterFullVersion) )
	{
		for ( int i = max(m_AppInfo.luVersion, pDirInfo->m_uiMasterFullVersion); 
			i < pDirInfo->m_uiLastMasterVersion; i++ )
		{
			_stprintf(szPackName, "patch_%d_%d.zip", i, i + 1);

			PACKAGECOREINFO& stPacakgeInfo = stPackageGroup.stPackageCoreInfo[nPackageIndex++];
			stPacakgeInfo.dwPackageVersion = i;
			_tcscpy(stPacakgeInfo.szPackageName, szPackName);	// 升级包的名称
			_tcscpy(stPacakgeInfo.szPackageFileName, szPackName);	// 本地保存文件名
			stPacakgeInfo.dwUpdateStrategy = bForceUpdate ? EUS_FORCE_TIPDOWNLOAD : EUS_ADVISE_TIPDOWNLOAD;	
		}
	}
	//同样的后台下载逻辑（先前台、后后台）
	if ( pDirInfo->m_uiSlaveFullVersion && pDirInfo->m_uiSlaveFullVersion > m_AppInfo.luoptVersion )
	{
		_stprintf(szPackName, "opt_setup_%d.EXE", pDirInfo->m_uiSlaveFullVersion);

		PACKAGECOREINFO& stPacakgeInfo = stPackageGroup.stPackageCoreInfo[nPackageIndex++];
		stPacakgeInfo.dwPackageVersion = pDirInfo->m_uiMasterFullVersion;
		_tcscpy(stPacakgeInfo.szPackageName, szPackName);	// 升级包的名称
		_tcscpy(stPacakgeInfo.szPackageFileName, szPackName);	// 本地保存文件名
		stPacakgeInfo.dwUpdateStrategy = EUS_FORCE_TIPNOTHING;	
	}
	if ( pDirInfo->m_uiLastSlaveVersion > max(m_AppInfo.luoptVersion, pDirInfo->m_uiSlaveFullVersion) )
	{
		for ( int i = max(m_AppInfo.luoptVersion, pDirInfo->m_uiSlaveFullVersion); 
			i < pDirInfo->m_uiLastSlaveVersion; i++ )
		{
			_stprintf(szPackName, "opt_patch_%d_%d.zip", i, i + 1);

			PACKAGECOREINFO& stPacakgeInfo = stPackageGroup.stPackageCoreInfo[nPackageIndex++];
			stPacakgeInfo.dwPackageVersion = i;
			_tcscpy(stPacakgeInfo.szPackageName, szPackName);	// 升级包的名称
			_tcscpy(stPacakgeInfo.szPackageFileName, szPackName);	// 本地保存文件名
			stPacakgeInfo.dwUpdateStrategy = EUS_FORCE_TIPNOTHING;	
		}
	}

	stPackageGroup.nPackageNum = nPackageIndex;
	for ( int i = 0; i < nPackageIndex; i++ )
	{
		// 常规包1
		PACKAGECOREINFO& stPacakgeInfo = stPackageGroup.stPackageCoreInfo[i];
		// 未压缩升级包校验值;
		ZeroMemory(stPacakgeInfo.szPackageFileHash, 33 * sizeof(TCHAR));
		stPacakgeInfo.nDownloadType = 3; // 下载方式，1为HTTP方式，2为P2P方式，3为两者混合

		_tcscpy(stPacakgeInfo.szPackageDir, _T("liveupdate"));	// 下载Dir;		
		stPacakgeInfo.dwPackageFileSize = 0;		// 安装包zip文件的大小（单位：字节）

		int nDownloadSvrIndex = rand() % pDirInfo->m_byDownloadServerNum;
		in_addr addr;
		addr.S_un.S_addr = pDirInfo->m_astDownloadServerInfo[nDownloadSvrIndex].m_uiIPAddr;

		_tcscpy(stPacakgeInfo.szHttpIP, inet_ntoa(addr));	//Http相关参数 ↓<<
		stPacakgeInfo.ushHttpPort = pDirInfo->m_astDownloadServerInfo[nDownloadSvrIndex].m_ushPort;		//Http相关参数 ↑>>
	}

	if ( m_spVersionUpdater && stPackageGroup.nPackageNum > 0 )
	{
		m_spVersionUpdater->UpdateClient((LPVOID) &stPackageGroup, FALSE, TRUE);
		return bForceUpdate;
	}
	return false;
}

/*老版本的升级方式，注释掉
void CNetworkImpl::CheckUpdate()
{
	DirResponseInfo* pDirInfo = CDirSocket::GetResponseInfo();
	if ( pDirInfo )
	{
		BOOL bUpdate = FALSE, bUpdateBg = TRUE;
		switch( pDirInfo->m_ushResultID )
		{
		case EDLR_Succeed:	
			break;
		case EDLR_Err_IdentityLimit:
			break;
		case EDLR_Err_VersionLimit:
			::MessageBox(m_AppInfo.hWnd, pDirInfo->m_szReason, NULL, MB_OK | MB_ICONEXCLAMATION );
			bUpdate = TRUE;
			break;
		case EDLR_Err_VersionRecommend:
			if ( IDYES == ::MessageBox(m_AppInfo.hWnd, pDirInfo->m_szReason, NULL, MB_YESNO | MB_ICONQUESTION) )
			{
				bUpdate = TRUE;
			}
			break;
		case EDLR_Err_FileCracked:
			{
				::MessageBox(m_AppInfo.hWnd, pDirInfo->m_szReason, NULL, MB_OK | MB_ICONEXCLAMATION );

				char szProfile[MAX_PATH] = "\0";
				_fullpath(szProfile, "client.ini", MAX_PATH);
				char szHome[MAX_PATH] = "\0";
				GetPrivateProfileString("app", "home", "", szHome, MAX_PATH, szProfile);
				if ( szHome )
					ShellExecute(NULL, "open", szHome, NULL, NULL, SW_SHOW);
				::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			}
			break;
		}
		char szUpdateProfile[MAX_PATH] = "\0";
		_fullpath(szUpdateProfile, "lu.cfg", MAX_PATH);
		int nVersion = pDirInfo->m_uiLastMasterVersion;
		char szVersion[40] = "\0";
		_snprintf(szVersion, 40, "%d", nVersion);
		int nOptVersion = pDirInfo->m_uiLastSlaveVersion;
		char szOptVersion[40] = "\0";
		_snprintf(szOptVersion, 40, "%d", nOptVersion);
		LPCSTR infile = pDirInfo->m_szMasterFileName;
		LPCSTR optinfile = pDirInfo->m_szSlaveFileName;
		WritePrivateProfileString("update", "version", szVersion, szUpdateProfile);
		WritePrivateProfileString("update", "optversion", szOptVersion, szUpdateProfile);

		DWORD nVersion_Current = m_AppInfo.luVersion, nOptVersion_Current = m_AppInfo.luoptVersion;
		if ( bUpdate && nVersion_Current < nVersion )
		{
			char szInFile[MAX_PATH] = "\0";
			if ( pDirInfo->m_byMasterFileNameLen == 0 )
			{
				_snprintf(szInFile, MAX_PATH, "patch_%d_%d.in", nVersion_Current, nVersion);
				infile = szInFile;
			}
			WritePrivateProfileString("update", "infile", infile, szUpdateProfile);
		}
		if ( bUpdateBg && nOptVersion_Current < nOptVersion )
		{
			char szOptInFile[MAX_PATH] = "\0";
			if ( pDirInfo->m_bySlaveFileNameLen == 0 )
			{
				_snprintf(szOptInFile, MAX_PATH, "optpatch_%d_%d.in", nOptVersion_Current, nOptVersion);
				optinfile = szOptInFile;
			}
			WritePrivateProfileString("update", "optinfile", optinfile, szUpdateProfile);
		}
		char szServerCount[40] = "\0";
		_snprintf(szServerCount, 40, "%d", pDirInfo->m_byDownloadServerNum);
		WritePrivateProfileString("update", "servercount", szServerCount, szUpdateProfile);

		for ( int i = 0; i < pDirInfo->m_byDownloadServerNum; i++ )
		{
			const STDownloadServerInfo& si = pDirInfo->m_astDownloadServerInfo[i];
			char szApp[40] = "\0";
			_snprintf(szApp, 40, "server%d", i+1);
			in_addr addr;
			addr.S_un.S_addr = si.m_uiIPAddr;
			char szURL[MAX_PATH] = "\0";
			_snprintf(szURL, MAX_PATH, "http://%s:%d/%s", inet_ntoa(addr), si.m_ushPort, 
				pDirInfo->m_byRootDirectionLen > 0 ? pDirInfo->m_szRootDirection : "");
			WritePrivateProfileString(szApp, "addr", szURL, szUpdateProfile);
		}
		if ( bUpdate )
		{
			atexit(RunLU);
			::PostMessage(m_AppInfo.hWnd, WM_CLOSE, 0, 0);
		}
		else
		{
			RunLU();
		}
	}
}
*/

void CNetworkImpl::GetLUVersion(DWORD& dwVersion, DWORD& dwOptVersion)
{
	TCHAR szProfile[MAX_PATH] = "";
	_fullpath(szProfile, "ver.ini", MAX_PATH);
	dwVersion = GetPrivateProfileInt("info", "version", 0, szProfile);
	_fullpath(szProfile, "ver_opt.ini", MAX_PATH);
	dwOptVersion = GetPrivateProfileInt("info", "version", 0, szProfile);
}

void CNetworkImpl::SaveDirConfig(DirResponseInfo* pDirInfo)
{
	char strWorldProfile[MAX_PATH] = "";
	char strApp[MAX_PATH] = "";
	char strDir[MAX_PATH] = "";
	char strDirProfile[MAX_PATH] = "";
	char strDirApp[MAX_PATH] = "";
	char strPorts[MAX_PATH] = "";
	char strSvrID[MAX_PATH] = "";
	_fullpath(strWorldProfile, "WorldDef.ini", MAX_PATH);
	for ( int i = 0; i < pDirInfo->m_byWorldCount; i++ )
	{
		STWorldBaseInfo* pWorldInfo = &pDirInfo->m_astWorldBaseInfo[i];
		sprintf(strApp, "World%d", pWorldInfo->m_byWorldID);
		sprintf(strDir, "DirSvrInfo%d.ini", pWorldInfo->m_byWorldID);
		char strDesc[MAX_PATH] = "";
		strncpy(strDesc, pWorldInfo->m_szName, pWorldInfo->m_byNameLen);
		WritePrivateProfileString(strApp, "desc", strDesc, strWorldProfile);
		WritePrivateProfileString(strApp, "svrlist", strDir, strWorldProfile);

		_fullpath(strDirProfile, strDir, MAX_PATH);
		for ( int d = 0; d < pWorldInfo->m_byDirNum; d++ )
		{
			STDirServerInfo* pSvrInfo = &pWorldInfo->m_astDirServerInfo[d];
			sprintf(strDirApp, "ServerInfo%d", d + 1);
			in_addr addr;
			addr.S_un.S_addr = pSvrInfo->m_uiServerIP;
			WritePrivateProfileString(strDirApp, "IP", inet_ntoa(addr), strDirProfile);
			sprintf(strPorts, "%d,%d,%d", pSvrInfo->m_aushServerPort[0], pSvrInfo->m_aushServerPort[1], pSvrInfo->m_aushServerPort[2]);
			WritePrivateProfileString(strDirApp, "Ports", strPorts, strDirProfile);
			sprintf(strSvrID, "%d", pSvrInfo->m_iServerID);
			WritePrivateProfileString(strDirApp, "ServerID", strSvrID, strDirProfile);
		}
	}
}

void CNetworkImpl::CheckLoginFromLoader()
{
	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL, PAGE_READONLY, 0, PAGE_SIZE, KART_MAP);
	if ( !hFileMap || GetLastError() != ERROR_ALREADY_EXISTS )
	{
		if ( hFileMap )
		{
			CloseHandle(hFileMap);
		}
		MessageBox(NULL, "无法打开共享内存", NULL, MB_OK);
		return;
	}
	LPVOID pMapData = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	if ( !pMapData )
	{
		CloseHandle(hFileMap);
		MessageBox(NULL, "无法映射共享内存", NULL, MB_OK);
		return;
	}
	SharedData* pSharedData = (SharedData*)pMapData;
	if ( pSharedData->dwUin == 0 || typelen(pSharedData->byPwd) == 0 )
	{
		UnmapViewOfFile(pMapData);
		CloseHandle(hFileMap);
		MessageBox(NULL, "共享内存数据为空", NULL, MB_OK);
		return;
	}
	m_hWndLoader = pSharedData->hWnd;
	SetActiveWorld(pSharedData->dwWorldID);
	LoginNormal(pSharedData->dwUin, pSharedData->byPwd, 2);
	UnmapViewOfFile(pMapData);
	CloseHandle(hFileMap);
}

void CNetworkImpl::OnUpdateSuccess()
{

}

void CNetworkImpl::OnUpdateFail()
{

}

void CNetworkImpl::OnIgnoreUpdate()
{

}

void CNetworkImpl::OnAlreadyNewest()
{

}

void CNetworkImpl::OnNeedUpdate()
{

}


int CNetworkImpl::oi_symmetry_encrypt2_len(int nInBufLen)
{
	return ::oi_symmetry_encrypt2_len(nInBufLen);
}

void CNetworkImpl::oi_symmetry_encrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{
	return ::oi_symmetry_encrypt2(pInBuf, nInBufLen, pKey, pOutBuf, pOutBufLen);
}

BOOL CNetworkImpl::oi_symmetry_decrypt2(const BYTE* pInBuf, int nInBufLen, const BYTE* pKey, BYTE* pOutBuf, int *pOutBufLen)
{
	return ::oi_symmetry_decrypt2(pInBuf, nInBufLen, pKey, pOutBuf, pOutBufLen);
}
