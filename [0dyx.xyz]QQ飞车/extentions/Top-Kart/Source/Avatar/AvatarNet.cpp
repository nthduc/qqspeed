#include "Top-KartPCH.h"
#include "./avatarnet.h"
#include "./avatarmgr.h"
#include "./ShopMgr.h"
#include "../NxPlayMgr.h"
#include "../network/msgheader.h"
#include "../Platform/GlobalInterface.h"
#include "network/msg.h"
#include "common.h"
#include "zlib.h"

CAvatarNet::CAvatarNet(void):
m_pApp(NULL),
m_pNetwork(NULL),
m_dwCookie(0),
m_dwP2PCookie(0)
{
}

CAvatarNet::~CAvatarNet(void)
{
	AppUnInit();
}

HRESULT CAvatarNet::AppInit(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;

		hr = AtlAdvise(spNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);

		CComPtr<IQQTppp> spQQTppp;
		hr = m_pApp->GetService(IID_IQQTppp, (void**)&spQQTppp);
		KK_CHECK(hr);
		m_pQQTppp = spQQTppp;

		CComPtr<IGameService> spService;
		hr = m_pApp->GetService(IID_IQQTppp, &spService);
		KK_CHECK(hr);

		hr = AtlAdvise(spService, (IQQTpppSink*)this, IID_IQQTpppSink, &m_dwP2PCookie);
		KK_CHECK(hr);


	}
KK_EXIT:
	return hr;
}

HRESULT CAvatarNet::AppUnInit()
{
	HRESULT hr = S_OK;
	if ( m_dwCookie )
	{
		AtlUnadvise(m_pNetwork, IID_INetworkSink, m_dwCookie);
		m_dwCookie = 0;
	}
	if ( m_dwP2PCookie )
	{
		CComPtr<IGameService> spService;
		hr = m_pApp->GetService(IID_IQQTppp, &spService);
		if(S_OK==hr)
		{
			hr = AtlUnadvise(spService, IID_IQQTpppSink, m_dwP2PCookie);
		}
		m_dwP2PCookie = 0;
	}
	m_pNetwork = NULL;
	m_pQQTppp = NULL;
	m_pApp = NULL;
	return hr;
}

STDMETHODIMP CAvatarNet::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	return S_OK;
}


STDMETHODIMP CAvatarNet::OnServerMessage(ServerEntity Entity,
											   UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	//if ( Entity != eGameServer )
	//	return S_OK;

	DISPATCH_MESSAGE_QQKART( MsgID, Data)

		return S_OK;
}

STDMETHODIMP CAvatarNet::OnClose(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}


STDMETHODIMP CAvatarNet::OnConnect(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::OnStartLogin(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::OnUdpLoginOk()
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::OnRecvP2PGameDataFromClient
(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CAvatarNet::SendTcpP2pData(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData)
{
	return S_OK;
}

//������Ϣӳ�䵽��Ӧ����

BEGIN_MESSAGE_MAP_QQKART(CAvatarNet)
ON_COMMAND_QQKART( ID_CMS_REQUESTCHANGEITEMSTATUS,	CAvatarNet::OnResponseChangeItemStatus)
ON_COMMAND_QQKART( ID_CMS_REQUESTBUYCOMMODITY,	CAvatarNet::OnResponseBuyCommodity)
ON_COMMAND_QQKART( ID_CMS_REQUESTGETCOMMODITYLIST,	CAvatarNet::OnResponseGetCommodityListPara)
END_MESSAGE_MAP_QQKART

void CAvatarNet::RequestChangeItemStatus(CRequestChangeItemStatus* pReq)
{
	_ASSERTE(pReq->m_unUin && pReq->m_unTime && pReq->m_sItemNum);
	XTrace("��װ����Ϣ��װ������=%u",pReq->m_sItemNum);
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCHANGEITEMSTATUS, 
		(PBYTE) pReq, 0, FE_PLAYER, NxSceneMgr::Get()->GetGameSvrID());
}

void CAvatarNet::OnResponseChangeItemStatus(void* pData)
{
	CResponseChangeItemStatus* pGetData = (CResponseChangeItemStatus*) pData;
	if( S_OK==pGetData->m_nResultID && pGetData->m_sItemNum)
	{//����װ��״̬
		CRequestChangeItemStatus	insQu;
		insQu.m_unUin=NxPlayMgr::GetHostPlay()->GetPlayInfo().m_unUin;
		insQu.m_sItemNum=pGetData->m_sItemNum;
		insQu.m_unTime=::GetTickCount();
		memcpy(insQu.m_stItemStatus,pGetData->m_stItemStatus,
			pGetData->m_sItemNum*sizeof(STItemStatus));
		CAvatarMgr::Get()->ChangeItemStatus(&insQu,true);
		CALL(NxPlayMgr::GetHostPlay(), ChangeTeam());
		XTrace("�ɹ�װ��");
	}
	else
	{
		XTrace("װ��ʧ�ܴ�����Ϣ=%s",pGetData->m_szReason);
	}

	return;
}

void CAvatarNet::RequestBuyCommodity(CRequestBuyCommodityPara* pReq)
{
	if(CShopMgr::Get()->AddBuyJob(pReq))
	{
		GlobalInterface::m_pNetwork->RequestShopServer(ID_CMS_REQUESTBUYCOMMODITY, 
			(PBYTE) pReq, 0);
		XTrace("����������ƷID=%u",pReq->m_iCommodityID);
	}
	else
	{//�����ظ�����
		_ASSERTE(0);
	}
}

void CAvatarNet::OnResponseBuyCommodity(void* pData)
{
	CResponseBuyCommodityPara* pGetData = (CResponseBuyCommodityPara*) pData;
	if( S_OK==pGetData->m_nResultID)
	{//
		XTrace("�ɹ�������ƷID=%u",pGetData->m_iCommodityID);
		CShopMgr::Get()->SetBuyJobState(pGetData->m_iCommodityID,0,pGetData->m_szReason);
	}
	else
	{
		CShopMgr::Get()->SetBuyJobState(pGetData->m_iCommodityID,0,pGetData->m_szReason);
		XTrace("������Ʒ������Ϣ=%s",pGetData->m_szReason);
	}
	
	return;
}

void CAvatarNet::RequestGetCommodityListPara(CRequestGetCommodityListPara* pReq)
{
	GlobalInterface::m_pNetwork->RequestShopServer(ID_CMS_REQUESTGETCOMMODITYLIST, 
		(PBYTE) pReq, 0);
	XTrace("������ȡ��Ʒ�б�");
}

void CAvatarNet::OnResponseGetCommodityListPara(void* pData)
{
	CResponseGetCommodityListPara* pGetData = (CResponseGetCommodityListPara*) pData;
	if( S_OK==pGetData->m_shResultID)
	{//
		XTrace("�����Ʒ�б���%u����Ʒ",pGetData->m_shCommodityNum);
		CShopMgr::Get()->StoreList(pGetData->m_shCommodityNum,pGetData->m_astCommodity);
	}
	else
	{
		XTrace("��ȡ��Ʒ�б������Ϣ=%s",pGetData->m_szReason);
	}
	return;
}