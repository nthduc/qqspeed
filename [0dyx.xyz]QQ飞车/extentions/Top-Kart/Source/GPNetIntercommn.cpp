#include "Top-KartPCH.h"
#include "GPNetInterCommn.h"
#include "./network/msgheader.h"
#include "./Platform/GlobalInterface.h"

#include "network/msg.h"
#include "common.h"
#include "zlib.h"
#include "./Platform/GlobalInterface.h"

#include "common.h"
#include "PlayerRank.h"
#include "gpdefine.h"
#include "GPPoint.h"
//#include "BaseItem.h"
#include "gpcontrol.h"
#include "gpsettings.h"
#include "singlet.h"
#include "NxSceneMgr.h"
#include "task.h"
#include "PowerController.h"

class CPlayGetGPTask : public Task
{
public:
	CPlayGetGPTask(IGPTarget* pTarget, GP_ID id)
	{
		SetType(Task::TYPE_TIME);
		float fTime = Scheduler::Get()->GetClock().GetTime();
		SetTimeParams(fTime, GPSettings::gottendelay, GPSettings::gottendelay);

		m_pTarget = pTarget;
		m_id = id;
	}
	~CPlayGetGPTask()
	{
		if ( m_pTarget )
		{
			m_pTarget->AddGP(m_id.dwID, m_id.dwIndex);
			m_pTarget = 0;
		}
	}

	virtual void Execute(unsigned int uiFrameCount, float fTime)
	{
	}
private:
	IGPTarget*	m_pTarget;
	GP_ID m_id;
};


CGPNetIntercommn::CGPNetIntercommn(void):
m_pApp(NULL),
m_pNetwork(NULL),
m_dwCookie(0),
m_dwP2PCookie(0)
{
}

CGPNetIntercommn::~CGPNetIntercommn(void)
{
	AppUnInit();
}


HRESULT CGPNetIntercommn::AppInit(IGameApp* App)
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

HRESULT CGPNetIntercommn::AppUnInit()
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
		KK_CHECK(hr);

		hr = AtlUnadvise(spService, IID_IQQTpppSink, m_dwP2PCookie);
		m_dwP2PCookie = 0;
	}
	m_pNetwork = NULL;
	m_pQQTppp = NULL;
	m_pApp = NULL;
	return S_OK;

KK_EXIT:
	return hr;
}


STDMETHODIMP CGPNetIntercommn::OnResponseLogin(ServerEntity Entity, LoginResult Result)
{
	return S_OK;
}


STDMETHODIMP CGPNetIntercommn::OnServerMessage(ServerEntity Entity,
											UINT MsgID, UINT MsgType, PBYTE Data, INT DataLen, PBYTE csHead, PBYTE msgHead)
{
	if ( Entity != eGameServer )
		return S_OK;

	DISPATCH_MESSAGE_QQKART( MsgID, Data)

	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnClose(ServerEntity Entity, int nErrorCode)
{
	return S_OK;
}


STDMETHODIMP CGPNetIntercommn::OnConnect(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnCreate(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnStartLogin(ServerEntity, int nErrorCode)
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnUdpLoginOk()
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnRecvP2PDataFromSvr(short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnGetPlayerInfoFromSvr(short shPlayerID, int nUin)
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnRecvClientTry2ConnectData(short shPlayerID, int nUin, ULONG ulTestMyIP, USHORT usTestMyPort)
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::OnRecvP2PGameDataFromClient
(short shSocketPlayerID, int nSocketPlayerUin, short shPlayerID, int nBodyLen, BYTE *pBody)
{
	return S_OK;
}

STDMETHODIMP CGPNetIntercommn::SendTcpP2pData(int nPlayerCount, short *asPlayerID, int *aiUin, int iLen, BYTE *pData)
{
	return S_OK;
}


//根据消息映射到相应函数

BEGIN_MESSAGE_MAP_QQKART(CGPNetIntercommn)
ON_COMMAND_QQKART( ID_CMS_REQUESTDIRFT,				CGPNetIntercommn::OnResponseDrift )
ON_COMMAND_QQKART( ID_SMC_NOTIFYDRIFT,				CGPNetIntercommn::OnNotifyReportDrift)
ON_COMMAND_QQKART( ID_CMS_REQUESTGETPROP,			CGPNetIntercommn::OnResponseGetProp )
ON_COMMAND_QQKART( ID_SMC_NOTIFYGETPROP,			CGPNetIntercommn::OnNotifyGetProp )
ON_COMMAND_QQKART( ID_CMS_REQUESTUSEPROP,			CGPNetIntercommn::OnResponseUseProp )
ON_COMMAND_QQKART( ID_SMC_NOTIFYUSEPROP,			CGPNetIntercommn::OnNotifyUseProp )
ON_COMMAND_QQKART( ID_SMC_NOTIFYPROPEFFECTRESULT,	CGPNetIntercommn::OnNotifyPropEffectResult )
ON_COMMAND_QQKART( ID_SMC_NOTIFYCHANGEAIMSTATE,		CGPNetIntercommn::OnNotifyChangeAimState )
ON_COMMAND_QQKART( ID_SMC_NOTIFYREPLACEPROP,		CGPNetIntercommn::OnNotifyReplaceProp)
END_MESSAGE_MAP_QQKART


//三维坐标点转换为三维数组
void CGPNetIntercommn::Poin3DToArray3(NiPoint3 point, long* arr)
{
	arr[0] = point.x;
	arr[1] = point.y;
	arr[2] = point.z;
}


//上报服务器此次漂移累加的氮气量
void CGPNetIntercommn::RequestDrift(unsigned int unInc)
{
	CRequestReportDriftPara data;
	data.m_unUin				= NxPlayMgr::GetHostPlay()->GetPlayUin();
	data.m_unTime				= GetTickCount();
	data.m_iAccelerateFuelInc	= unInc;		//赋予此次漂移的增量

	//上报服务器
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTDIRFT, (PBYTE) &data, 0, FE_GAMELOGIC, NxSceneMgr::Get()->GetGameSvrID());
}



//请求服务器要获取位置为shPropPositionNO的道具, shPropPositionNO为位置索引
void CGPNetIntercommn::RequsetGetProp(short shPropPositionNO)
{
	CRequestGetPropPara data;
	data.m_unUin				= NxPlayMgr::GetHostPlay()->GetPlayUin();
	data.m_unTime				= GetTickCount();
	data.m_bCurrentRank			= CPlayerRankMgr::Get()->GetHostCurRank();		//得到本地玩家当前排名
	data.m_iPreviousInterval	= CPlayerRankMgr::Get()->GetPreviousInterval();	//得到本地玩家和前一玩家的距离
	data.m_sPropPositionNO		= shPropPositionNO;		//道具点的索引
	//玩家当前的位置
	Poin3DToArray3(NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetWorldTranslate(), data.m_aiCurrentPosition);

	//上报服务器
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTGETPROP, (PBYTE) &data, 0, FE_GAMELOGIC, NxSceneMgr::Get()->GetGameSvrID());
}


//请求服务器使用道具， shProp为道具ID， iPropIndex为服务器生成的道具索引， unDstUin为目标的Uin
void CGPNetIntercommn::RequestUseProp(short shPropID, int iPropIndex, unsigned int unDstUin, int nNewID)
{
	CRequestUsePropPara data;
	data.m_unUin				= NxPlayMgr::GetHostPlay()->GetPlayUin();
	data.m_unTime				= GetTickCount();
	data.m_unDstUin				= unDstUin;
	data.m_sPropID				= shPropID;		//道具ID
	data.m_iPropIndex			= iPropIndex;	//服务器关于本道具的index
	data.m_sNewID				= nNewID;
	//玩家当前的位置
	Poin3DToArray3(NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetWorldTranslate(), data.m_aiCurrentPosition);

	//上报服务器
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTUSEPROP, (PBYTE) &data, 0, FE_GAMELOGIC, NxSceneMgr::Get()->GetGameSvrID());
	
}


//上报服务器道具作用结果, iPropIndex为服务器生成的道具索引
//iResult为作用结果，0表示被击中，1表示没被击中
void CGPNetIntercommn::RequestPropEffectResult(short shPropID, int iPropIndex, int iResult, short sItemID)
{
	CRequestPropEffectResultPara data;
	data.m_unUin			= NxPlayMgr::GetHostPlay()->GetPlayUin();
	data.m_nEffectResult	= iResult;
	data.m_unTime			= GetTickCount();
	data.m_sPropID			= shPropID;
	data.m_iPropIndex		= iPropIndex;	//服务器关于本道具的index
	data.m_iItemID			= sItemID;
	
	//上报服务器
	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTPROPEFFECTRESULT, 
														(PBYTE) &data, 0, FE_GAMELOGIC, NxSceneMgr::Get()->GetGameSvrID());
}


//请求改变瞄准状态,unDstUin为目标的Uin, 
//uchState为瞄准状态，0取消瞄准，1瞄准
void CGPNetIntercommn::RequestChangeAimState(unsigned int unDstUin, unsigned char uchState)
{
	CRequestChangeAimStatePara data;
	data.m_unUin			= NxPlayMgr::GetHostPlay()->GetPlayUin();
	data.m_unTime			= GetTickCount();
	data.m_unDstUin			= unDstUin;
	data.m_bAimState		= uchState;		//瞄准状态

	GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCHANGEAIMSTATE, 
														(PBYTE) &data, 0, FE_GAMELOGIC, NxSceneMgr::Get()->GetGameSvrID());
}


//服务器回复因漂移取得加速卡的结果
void CGPNetIntercommn::OnResponseDrift(void* pData)
{
	CResponseReportDriftPara* pGetData = (CResponseReportDriftPara*) pData;

	if( pGetData->m_sResultID == S_OK )
	{
		IGPTarget* pTarget = CGPControl::Get()->GetTargetIterator()->GetHost();
		KK_CHECK_POINTER(pTarget);

		pTarget->AddGP(pGetData->m_sPropID, pGetData->m_iPropIndex);//加入道具
	}
KK_EXIT:	
	return;
}

//服务器通知组队赛中队伍氮气的积累量
void CGPNetIntercommn::OnNotifyReportDrift(void* pData)
{
	CNotifyReportDriftPara* pGetData = (CNotifyReportDriftPara*) pData;
	
	if(NxSceneMgr::Get())
	{
		CALL(CPowerController::Get(), GetTeamPower()->SetPower(pGetData->m_iTeamAccelerateFule));
	}
}

//服务器答复获取位置为shPropPositionNO的道具
void CGPNetIntercommn::OnResponseGetProp(void* pData)
{
	CResponseGetPropPara* pGetData = (CResponseGetPropPara*) pData;	
	if( pGetData->m_nResultID == S_OK )
	{
		XTrace("自己得到道具了 ID=%d",pGetData->m_sPropID);
		IGPTarget* pTarget = CGPControl::Get()->GetTargetByID(pGetData->m_unUin);
		KK_CHECK_POINTER(pTarget);

		//这里不再做限制，由服务器控制
		//因为在调试的时候客户端运行手工添加道具，不然服务器过来的就加不进去
		//pTarget->AddGP(pGetData->m_sPropID, pGetData->m_iPropIndex);//加入道具
		GP_ID id(pGetData->m_sPropID, pGetData->m_iPropIndex, 0);
		CPlayGetGPTask* pTask = NiNew CPlayGetGPTask(pTarget, id);
		Scheduler::Get()->Schedule(pTask);
		CGPControl::Get()->OnGotten(pGetData->m_sPropID, pGetData->m_unUin);//播放特效
	}
KK_EXIT:	
	return;
}


//服务器通知道具的获取情况
void CGPNetIntercommn::OnNotifyGetProp(void* pData)
{
	CNotifyGetPropPara* pGetData = (CNotifyGetPropPara*) pData;
			
	CGPPointMgr::GetPoint(pGetData->m_sPropPositionNO)->SetActive(false);//道具点设为不可见（即被吃掉）
	//CGPPointMgr::SetReCreateTime(3000);	//设道具点恢复时间
	
	IGPTarget* pTarget = CGPControl::Get()->GetTargetByID(pGetData->m_unUin);
	IGPTarget* pHost = CGPControl::Get()->GetTargetIterator()->GetHost();
	if ( pTarget == pHost )
	{	//自己的道具在Response里面加
		return;
	}
	if ( pGetData->m_iPropIndex == 0 )
	{
		return;
	}
	if ( pTarget )
	{
		XTrace("%d得到了道具id=%d,index=%d", pGetData->m_unUin, pGetData->m_sPropID, pGetData->m_iPropIndex);
		pTarget->AddGP(pGetData->m_sPropID, pGetData->m_iPropIndex);//加入道具
	}
	else
	{
		XWaring("%d not exists, cannot AddGP", pGetData->m_unUin);
	}

}


//服务器答复使用请求
void CGPNetIntercommn::OnResponseUseProp(void* pData)
{
	CResponseUsePropPara* pGetData = (CResponseUsePropPara*) pData;

}


//服务器通知道具使用情况
void CGPNetIntercommn::OnNotifyUseProp(void* pData)
{
	CNotifyUsePropPara* pGetData = (CNotifyUsePropPara*) pData;	
	if ( CGPControl::Get() )
	{
		//触发使用道具接口
		GP_ID id(pGetData->m_sPropID, pGetData->m_iPropIndex, 0);
		if(pGetData->m_sNewID)
		{
			id.dwID = pGetData->m_sNewID;
			id.dwOriginID = pGetData->m_sPropID;
		}
		CGPControl::Get()->OnUseGP(id,
			pGetData->m_unUin, pGetData->m_unDstUin);

		//删除玩家的道具
		XTrace("%d使用了道具id=%d,index=%d", pGetData->m_unUin, pGetData->m_sPropID, pGetData->m_iPropIndex);
		IGPTarget* pTarget = CGPControl::Get()->GetTargetByID(pGetData->m_unUin);
		if ( pTarget )
		{
			GP_ID_LIST& refGPs = pTarget->GetGPs();
			if ( refGPs.size() > 0 )
			{
				GP_ID& refID = *refGPs.begin();
				if ( refID.dwIndex == pGetData->m_iPropIndex )
				{
					refGPs.pop_front();
				}
			}
/*			GP_ID_LIST& refGPs = pTarget->GetGPs();			
			if ( refGPs.size() > 0 )
			{
				GP_ID_LIST_Iterator gps_it = refGPs.begin();
				for(; gps_it != refGPs.end(); ++gps_it)
				{
					if( (*gps_it).dwIndex == pGetData->m_iPropIndex)
					{
						pTarget->RemoveGP(pGetData->m_sPropID);
					}
				}
			}
*/		
		}
		else
		{
			XWaring("OnNotifyUseProp:cannot find target");
		}
	}
}

//服务器通知道具作用结果
void CGPNetIntercommn::OnNotifyPropEffectResult(void* pData)
{
	CNotifyPropEffectResultPara* pPara = (CNotifyPropEffectResultPara*)pData;	
	if ( pPara )
	{
		GP_ID id(pPara->m_sPropID, pPara->m_iPropIndex, 0);
		CGPControl::Get()->OnTriggerByGP(id, pPara->m_unUin);
	}

}


//服务器通知被瞄准玩家
void CGPNetIntercommn::OnNotifyChangeAimState(void* pData)
{
	CNotifyChangeAimStatePara* pPara = (CNotifyChangeAimStatePara*)pData;
	if ( CGPControl::Get() )
	{
		CGPControl::Get()->SetAimed(pPara->m_unDstUin, pPara->m_bAimState);
		
		 //开始瞄准,先行删除此道具
		/*弄不懂为什么有这段代码，暂且删除
		if ( pPara->m_unDstUin == 0 )
		{
			IGPTarget* pTarget = CGPControl::Get()->GetTargetByID(pPara->m_unUin);
			if ( pTarget )
			{
				GP_ID_LIST& refGPs = pTarget->GetGPs();
				if ( refGPs.size() > 0 )
				{
					refGPs.pop_front();
				}
			}
			else
			{
				XWaring("OnNotifyChangeAimState:cannot find target");
			}
		}
		*/
	}
}

void CGPNetIntercommn::OnNotifyReplaceProp(void* pData)
{
	CNotifyReplacePropPara* pGetData = (CNotifyReplacePropPara*) pData;
	for(int i = 0; i < pGetData->m_sPlayerNum; ++i)
	{
		IGPTarget* pTarget = CGPControl::Get()->GetTargetByID(pGetData->m_astRacerReplacePropInfo[i].m_unUin);
		STReplacePropInfo* replacePara = pGetData->m_astRacerReplacePropInfo[i].m_astReplaceProp;


		GP_ID_LIST& Id_List = pTarget->GetGPs();
		GP_ID_LIST_Iterator Id_It = Id_List.begin();
		for(int k = 0; k < 2; ++k)
		{
			if(Id_It == Id_List.end())
			{
				break;
			}

			if(replacePara[k].m_iReplaceFlag)
			{
				(*Id_It).dwID = replacePara[k].m_sPropID;
			}

			++Id_It;
		}
		
	}

}
