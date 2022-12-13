#include "Top-KartPCH.h"
#include ".\antibotimpl.h"
#include "network/msg.h"
#include "common.h"
#include "zlib.h"

#pragma comment(lib, "zdll")

CAntibotImpl::CAntibotImpl(void):
m_pAntiBot(NULL),
m_dwCookie(0)
{
}

CAntibotImpl::~CAntibotImpl(void)
{
	AppUnInit();
}

HRESULT CAntibotImpl::AppInit(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		hr = AtlAdvise(spNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;
	}
KK_EXIT:
	return hr;
}

HRESULT CAntibotImpl::AppUnInit()
{
	if ( m_dwCookie )
	{
		AtlUnadvise(m_pNetwork, IID_INetworkSink, m_dwCookie);
		m_dwCookie = 0;
	}
	if ( m_pAntiBot )
	{
		m_pAntiBot->Release();
		m_pAntiBot = NULL;
	}

	m_pApp = NULL;
	m_pNetwork = NULL;
	return S_OK;
}

STDMETHODIMP CAntibotImpl::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
	if ( IsDebuggerPresent() )
		return S_OK;
#else if defined(_DEBUG) || defined(DEBUG)
	return S_OK;
#endif

	if ( !m_pAntiBot )
	{
		m_pAntiBot = CreateAntiBot();
		if ( !m_pAntiBot )
		{
			LogText("CreateAntiBot Failed\n");
			return S_OK;
		}
		m_pAntiBot->AddNotifySink(this);

		DWORD dwVersion = 0;
		m_pApp->GetVersion(&dwVersion);
		m_pAntiBot->SetGameVer(dwVersion);
		SetSvrId(0);
	}
	if( !m_pAntiBot )
	{
		LogText("AntiBot not avalid! Ignore Check\n");
		return S_OK;
	}

	AB_INIT_INFO abInitInfo;
	abInitInfo.shProtoId = CS_AB_INIT_INFO;
	m_pAntiBot->GetInitInfo(&abInitInfo.stPrevData);

	NotifyGameServerInitAnti(&abInitInfo);
	return S_OK;
}

STDMETHODIMP CAntibotImpl::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity != eGameServer )
		return S_OK;
	switch(MsgID)
	{
	case ID_CMS_NOTIFY_ANTI_BOT:
		{
			NOTIFY_ANTI_BOT* pParam = (NOTIFY_ANTI_BOT*)Data;
			OnNotifyAntiBot(pParam);
		}
		break;
	}
	return S_OK;
}

STDMETHODIMP CAntibotImpl::OnClose(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CAntibotImpl::OnConnect(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CAntibotImpl::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CAntibotImpl::OnStartLogin(ServerEntity, int nErrorCode)
{
	return S_OK;
}

int CAntibotImpl::OnCheckResult(BYTE *pbyResult, DWORD dwSize)
{
	QQUSERDATA* pUserData = (QQUSERDATA*)m_pNetwork->GetQQUserData();
	if ( pUserData )
	{
		AB_VERIFY_RESULT avr;
		avr.dwUin = pUserData->Uin;
		avr.shProtoId = CS_AB_VERIFY_RESULT;
		memcpy(&avr.stPacket, pbyResult, dwSize);
		NotifyGameServerCheckResultAnti(&avr);
	}
	return 0;
}

void CAntibotImpl::NotifyGameServerInitAnti(AB_INIT_INFO* pInitInfo)
{
	QQUSERDATA* pUserData = (QQUSERDATA*)m_pNetwork->GetQQUserData();
	if ( pUserData )
	{
		NOTIFY_ANTI_BOT aPara;
		aPara.m_dwUin = pUserData->Uin;
		aPara.m_nBufferLen = sizeof(AB_INIT_INFO);
		m_AntiBotEncoder.EncodeInitInfo(pInitInfo, (BYTE*)aPara.m_szBuffer);
		m_pNetwork->RequestGameServer(ID_CMS_NOTIFY_ANTI_BOT, (PBYTE)&aPara, 0, FE_GAMESVRD, 0);
	}
}

void CAntibotImpl::NotifyGameServerCheckResultAnti(AB_VERIFY_RESULT* pVerifyResult)
{
	QQUSERDATA* pUserData = (QQUSERDATA*)m_pNetwork->GetQQUserData();
	if ( pUserData )
	{
		NOTIFY_ANTI_BOT aPara;
		aPara.m_dwUin = pUserData->Uin;
		DWORD dwResult = m_AntiBotEncoder.EncodeVerifyResult(pVerifyResult, (BYTE*)aPara.m_szBuffer);
		aPara.m_nBufferLen = dwResult;
		m_pNetwork->RequestGameServer(ID_CMS_NOTIFY_ANTI_BOT, (PBYTE)&aPara, 0, FE_GAMESVRD, 0);
	}
}

void CAntibotImpl::OnNotifyAntiBot(NOTIFY_ANTI_BOT* pParam)
{
	AB_DATA ABData;
	AB_RAND_PARAM ABRandParam;
	short shProtoId = m_AntiBotEncoder.DecodeProtolId((BYTE*)pParam->m_szBuffer);
	switch (shProtoId)
	{
	case SC_AB_SEND_ANTIBOT:
		{
			m_AntiBotEncoder.DecodeDataMsg((BYTE*)pParam->m_szBuffer, &ABData);
			if (m_pAntiBot)
			{
				if (ABData.nHasData && ABData.nHasCPData)
				{
					LogText("Get data from server (receive CP data)\n");
					m_pAntiBot->OnRcvCPFinish(ABData.pbyData, ABData.shDataSize, ABData.byVer, ABData.nRCID);
				}
				if (ABData.nHasData && ABData.nHasRCData)
				{
					#define MAX_RC_LEN 65000
					BYTE *prcdata = new BYTE[MAX_RC_LEN];
					unsigned long destlen = MAX_RC_LEN;
					int iret = uncompress( prcdata, &destlen, ABData.pbyData, ABData.shDataSize);
					ASSERT( iret == Z_OK );

					LogText("Get data from server (receive RC data)\n");
					m_pAntiBot->OnRcvRCFinish(prcdata, destlen, ABData.byVer, ABData.nRCID);
					delete []prcdata;
				}
			}
		}
		break;

	case SC_AB_RAND_CHECK:
		{
			LogText("Get data from server (receive rand check)\n");
			m_AntiBotEncoder.DecodeRandParam((BYTE*)pParam->m_szBuffer, &ABRandParam);
			if (m_pAntiBot)
			{
				m_pAntiBot->OnRandCheck(0, (BYTE*)ABRandParam.byRand);
			}
		}
		break;
	}
}