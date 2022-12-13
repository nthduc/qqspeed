////////////////////////////////////////////////
//     FileName:    MsgDataCache.h
//     Author:      Fish
//     Date:        2006-10-18 10:04:09
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "framework/GameApp_i.h"
#include "Network/network_i.h"
#include "Network/msg.h"
#include "datadef.h"

typedef struct tagENTERROOM_INFO
{
	DWORD dwSvrID;
	DWORD dwRoomID;
}ENTERROOM_INFO;

class CMsgDataCache :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMsgDataCache>,
	public IGameService,
	public INetworkSink
{
public:
	CMsgDataCache(void);
	~CMsgDataCache(void);

	//IGameService
	STDMETHOD(AppInit)(IGameApp*);
	STDMETHOD(AppUnInit)();

	//INetworkSink
	STDMETHOD(OnResponseLogin)(ServerEntity, LoginResult);
	STDMETHOD(OnServerMessage)(ServerEntity, UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead);
	STDMETHOD(OnClose)(ServerEntity, int nErrorCode);
	STDMETHOD(OnConnect)(ServerEntity, int nErrorCode);
	STDMETHOD(OnCreate)(ServerEntity, int nErrorCode);
	STDMETHOD(OnStartLogin)(ServerEntity, int nErrorCode);

	STPlayerRoomVisibleInfo* FindPlayerInfo(UINT Uin);
	///设置Avatar信息,nNum是需要处理的Item数，设置pInfo的角色和车子id
	bool SetAvatarInfo(ULONG dwUin,UINT nNum,STPlayerDBItemInfo* pDbItem,KPlayInfo* pInfo);

	bool AddEnterRoomTask(DWORD dwSvrID, DWORD dwRoomID);

	BEGIN_COM_MAP(CMsgDataCache)
		COM_INTERFACE_ENTRY(INetworkSink)
	END_COM_MAP()

private:
	void InitData()
	{
		m_RoomList.m_nResultID = E_FAIL;
		m_RoomDesc.Reset();
	}

public:
	IGameApp*		m_pApp;
	INetwork*		m_pNetwork;
	DWORD			m_dwCookie;
	CResponseGetRoomListPara	m_RoomList;
	RoomDesc		m_RoomDesc;
	ENTERROOM_INFO	m_kEnterRoomTask;
};
