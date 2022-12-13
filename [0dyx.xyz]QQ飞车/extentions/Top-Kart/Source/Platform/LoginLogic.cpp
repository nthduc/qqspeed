////////////////////////////////////////////////
//     FileName:    LoginLogic.cpp
//     Author:      Fish
//     Date:        2006-10-12 9:45:33
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include ".\loginlogic.h"
#include "common.h"
#include "network/userdata.h"

CLoginLogic::CLoginLogic(void):
m_pApp(NULL),
m_pNetwork(NULL),
m_dwCookie(0)
{
}

CLoginLogic::~CLoginLogic(void)
{
}

STDMETHODIMP CLoginLogic::Load(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	m_pApp = App;
	{
		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		m_pNetwork = spNetwork;

		Active();
		CheckAutoLogin();
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CLoginLogic::Active()
{
	HRESULT hr = E_FAIL;
	{
		hr = AtlAdvise(m_pNetwork, (INetworkSink*)this, IID_INetworkSink, &m_dwCookie);
		KK_CHECK(hr);
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CLoginLogic::DeActive()
{
	if ( m_dwCookie )
	{
		AtlUnadvise(m_pNetwork, IID_INetworkSink, m_dwCookie);
		m_dwCookie = 0;
	}
	return S_OK;
}

STDMETHODIMP CLoginLogic::UnLoad()
{
	DeActive();
	m_pApp = NULL;
	m_pNetwork = NULL;
	return S_OK;
}

STDMETHODIMP CLoginLogic::GetName(LPCSTR* Name)
{
	static LPCSTR s_myName = "login";
	KK_CHECK_POINTER(Name);
	*Name = s_myName;
KK_EXIT:
	return S_OK;
}

STDMETHODIMP CLoginLogic::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	if ( Entity == eGameServer 
		&& ( Result == eLoginSuccessed || Result == eLoginRequireReg || Result == eLoginVersionRecommend ) )
	{
		BOOL bSaveUin = FALSE;
		BOOL bSavePwd = FALSE;
		CComQIPtr<IGameSettings> spSettings = m_pApp;
		if ( spSettings )
		{
			spSettings->GetIntValue("SaveUin", &bSaveUin);
			spSettings->GetIntValue("SavePwd", &bSavePwd);
		}
		if ( bSaveUin )
		{
			QQUSERDATA* pUser = (QQUSERDATA*)m_pNetwork->GetQQUserData();
			if ( pUser )
			{
				char szPwd[PWD_LEN+1] = "";
				strncpy(szPwd, (char*)pUser->byPwd, PWD_LEN);
				UserData aData(pUser->Uin, bSavePwd ? szPwd : NULL);
				CUserDataProfile aProfile;
				aProfile.SetUserData(&aData);
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CLoginLogic::OnServerMessage(ServerEntity Entity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	return S_OK;
}

STDMETHODIMP CLoginLogic::OnClose(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CLoginLogic::OnConnect(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CLoginLogic::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CLoginLogic::OnStartLogin(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}

void CLoginLogic::CheckAutoLogin()
{
	SHORT nCtrl = GetKeyState(VK_CONTROL);
	if ( nCtrl & 0xff00 )
		return;
	FILE* fp = fopen("c:\\mydata.dat", "r");
	if ( !fp )
		return;
	UINT Uin = 0;
	fscanf(fp, "%d", &Uin);
	char szPwd[20] = { '\0' };
	fscanf(fp, "%s", szPwd);
	m_pNetwork->StartLogin(Uin, (PBYTE)szPwd);
	fclose(fp);
}