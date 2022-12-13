#pragma once

#include "framework/GameApp_i.h"
#include "Network/Network_i.h"
#include "antisdk/AntiBotImport.h"
NS_USING_ANTIBOT

struct NOTIFY_ANTI_BOT;
class CAntibotImpl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAntibotImpl>,
	public INetworkSink,
	public INotifySink
{
public:
	CAntibotImpl(void);
	~CAntibotImpl(void);

	HRESULT AppInit(IGameApp*);
	HRESULT AppUnInit();

	//INetworkSink
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult);
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead);
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode);
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode);
	STDMETHOD(OnCreate)(ServerEntity, int nErrorCode);
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode);

	//INotifySink
	virtual int OnCheckResult(BYTE *pbyResult, DWORD dwSize);

	BEGIN_COM_MAP(CAntibotImpl)
		COM_INTERFACE_ENTRY(INetworkSink)
	END_COM_MAP()

protected:
	void NotifyGameServerInitAnti(AB_INIT_INFO*);
	void NotifyGameServerCheckResultAnti(AB_VERIFY_RESULT*);
	void OnNotifyAntiBot(NOTIFY_ANTI_BOT*);

private:
	IGameApp*	m_pApp;
	INetwork*	m_pNetwork;
	IAntiBot*	m_pAntiBot;
	CABDataMsg	m_AntiBotEncoder;
	DWORD		m_dwCookie;
};
