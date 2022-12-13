/*-----------------------------------------------------------------------
Create By HoverZhao 2006.12
------------------------------------------------------------------------*/

#include "Top-KartPCH.h"
#include ".\movepkgmgr.h"
#include <set>
#include <math.h>

#include "DescConstant.h"
#include "./Platform/GlobalInterface.h"
#include "common.h"
#include "network/msg.h"
#include "QQTppp_i.h"
#include "Play.h"
#include "./Track/CheckEditor.h"
#include "Network/Network_i.h"
#include "Network/consts.h"
#include "network/userdata.h"
#include "winbase.h"
#include "network/msgheader.h"

#define MAX_DISTANCE 999999
#define MAX_UDPSPEED 999999
#define MAX_TCPSPEED 200

std::map<int , IQQCarLogicNet* > NxGamePkgMgr::ms_arrLisener;
int NxGamePkgMgr::m_nRegistCookie	 = 0;

//-----------------------------------------------------------------------

NxGamePkgMgr::NxGamePkgMgr()
{
}

NxGamePkgMgr::~NxGamePkgMgr()
{
	_MapUdpReciveTable.clear();
	_MapUdpSendTable.clear();
	_UdpPkgDatabase.clear();
}

bool NxGamePkgMgr::Start()
{
	_MapUdpReciveTable.clear();
	_MapUdpSendTable.clear();
	_UdpPkgDatabase.clear();
	m_bSendLastTcp = false;

	// 初始化SendTable 
	std::set< Play * > _AllRemotePlay = NxPlayMgr::GetAllRemotePlay();
	std::set< Play * >::iterator it = _AllRemotePlay.begin();

	for ( ; it != _AllRemotePlay.end(); it++)
	{
		SenderNotebook Sno;
		Sno._uin = (*it)->GetPlayUin();
		Sno._lastTime = ::GetTickCount();
		Sno._frequence = 0;
		Sno._deltaTime = 0;
		Sno._disable = false;
		Sno._byserver = false;
		Sno._pkgid = 0;
		Sno._udpSpeed = MAX_UDPSPEED;
		_MapUdpSendTable.push_back(Sno);
		
		ReciveNotebook Rno;
		Rno._uin = (*it)->GetPlayUin();
		Rno._index = 0;
		_MapUdpReciveTable.push_back(Rno);

	}
	m_nCurTime = GetTickCount();
	// 初始化TCP 变量
	m_nTcpLastSendTime = GetTickCount();

	m_nRcvUdpPkgStatic = 0;
	m_nStaticTime = GetTickCount();
	m_nIncUdpPkg = 0;
	//m_nTcpDeltaTime = 0;

	return true;
}

//-----------------------------------------------------------------------
void NxGamePkgMgr::ProcessUdpPkg(STUdpPack * pData)
{

	if ( pData == NULL) 
		return;
	

	if ( GetTickCount() - m_nStaticTime < 500 )
	{
		m_nIncUdpPkg++;
	}
	else
	{
		m_nRcvUdpPkgStatic = m_nIncUdpPkg * 2 ;
		m_nStaticTime = GetTickCount();
		m_nIncUdpPkg = 0;
	}

	OnNetMessage(pData->MsgID,(void * )pData);  // bug udp tcp 协议体定义

	if ( pData->MsgID == UDP_MSG_COLLSION )
	{
		TriggerUdpCollisionPkg( (STUDPPlayCollision*)pData );
		return;
	}

	if( pData->MsgID == UDP_MSG_PLAYMOVE)
	{
		STUDPPlayMove * pDate = ( STUDPPlayMove * )pData;
		if( !CheckUdpPkg(pDate->m_unUin, pDate->m_unPackageIndex) )
			return;

		PushUdpPkg(pDate);

		TrigerUdpPkg(pDate->m_unUin, pDate->m_unPackageIndex, pDate);
	}



//	LogText("Recive Udp Pkg from Uin : %d \n", pData->m_unUin); 


}

//-----------------------------------------------------------------------
bool NxGamePkgMgr::CheckUdpPkg(ULONG dwUin, ULONG index)
{
	if ( NxPlayMgr::GetHostPlay()->GetPlayUin() == dwUin ) 
		return false;
	
	std::vector<ReciveNotebook>::iterator it = _MapUdpReciveTable.begin();
	for( ; it != _MapUdpReciveTable.end(); it++)
	{
		if( (*it)._uin == dwUin) 
		{
			if ((*it)._index < index)
			{
				(*it)._index = index;
				return true;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------
void NxGamePkgMgr::PushUdpPkg(STUDPPlayMove * pData)
{
	if ( pData == NULL)
		return;

	STUDPPlayMove rData ;
	memcpy( &rData, pData , sizeof( STUDPPlayMove));
	_UdpPkgDatabase.push_back(rData);

}

float NxGamePkgMgr::GetSwooshFactor( ULONG dwUin )
{
	std::vector<SenderNotebook>::iterator it = _MapUdpSendTable.begin();

	INT nSpeed = MAX_UDPSPEED;
	for( ; it != _MapUdpSendTable.end(); it++)
	{
		if ( (*it)._uin == dwUin )  {
			nSpeed = (*it)._udpSpeed;
			break;
		}
	}
	
	if ( nSpeed < 50 )
		nSpeed = 0;

	float fSwooshFactor = __min( 1.0f + nSpeed / 100.0f , 5.0f);
	return fSwooshFactor;
}
//-----------------------------------------------------------------------
void NxGamePkgMgr::TrigerUdpPkg(ULONG dwUin, ULONG index,STUDPPlayMove * pData)
{
	Play * pPlay = NxPlayMgr::GetPlay(dwUin);
	if ( pPlay && NxPlayMgr::GetHostPlay()->GetPlayUin() != dwUin)  {
		GetSwooshFactor(dwUin);
		pPlay->SetKartParam( pData, GetSwooshFactor(dwUin));
	}
	return;
}

void NxGamePkgMgr::UpdateUdpConnect()
{
	std::vector<SenderNotebook>::iterator it = _MapUdpSendTable.begin();

	for( ; it != _MapUdpSendTable.end(); it++)
	{
		(*it)._byserver =  GlobalInterface::m_pQQTppp->CanPass((*it)._uin);
		//(*it)._byserver  = true;
		if ( (*it)._byserver ) 
			(*it)._udpSpeed = MAX_TCPSPEED;
		else
			GlobalInterface::m_pQQTppp->GetP2pNetSpeed((*it)._uin,&((*it)._udpSpeed));

		//LogText("Uin: %d , udp Speed: %d \n",(*it)._uin,(*it)._udpSpeed);
	}


}
//-----------------------------------------------------------------------

void NxGamePkgMgr::Update(float fTime)
{
	UpdateUdpConnect();
	SendUdp(fTime);
	SendTcp(fTime);
}
//-----------------------------------------------------------------------
// 这里没有必要对玩家是否完成比赛做判断 发包又怎么样，你打我呀！
void NxGamePkgMgr::SendUdp(float fTime)
{
	Analaysis(ANA_DISTANCE);
	m_nCurTime = GetTickCount();

//	bool bIsExistUdpPlayer = false;
//	bool bIsExistTcpPlayer = false;
	std::vector<SenderNotebook*> vUdpPlayers;
	std::vector<SenderNotebook*> vTcpPlayser;
	vUdpPlayers.clear();
	vTcpPlayser.clear();

	std::vector<SenderNotebook>::iterator it = _MapUdpSendTable.begin();

	for( ; it != _MapUdpSendTable.end(); it++)
	{
		if ( (*it)._byserver == true) {
			vTcpPlayser.push_back(&(*it));
			continue;
		}
		else{
			vUdpPlayers.push_back(&(*it));
		}
	}

	STUDPPlayMove UdpPk;
	memset( &UdpPk, 0, sizeof(STUDPPlayMove));
	UdpPk.MsgID = UDP_MSG_PLAYMOVE;
	NxPlayMgr::GetHostPlay()->GetKartParam(&UdpPk);	
/*
	if ( vTcpPlayser.size() > 0 || vUdpPlayers.size() > 0)	{
		UdpPk.m_unPackageIndex = (*it)._pkgid++;
	}
*/
	for ( int i = 0; i < vUdpPlayers.size(); i++)	{
		if ( vUdpPlayers[i]->_frequence == 0)
			continue;
		else if ((  m_nCurTime - vUdpPlayers[i]->_lastTime ) < 
			1000 / vUdpPlayers[i]->_frequence )
			continue;
		else {

			// 玩家可能会中途离开
			if ( !NxPlayMgr::GetPlay(vUdpPlayers[i]->_uin))
				continue;

			UdpPk.m_unPackageIndex = vUdpPlayers[i]->_pkgid++ ;
			
			GlobalInterface::m_pQQTppp->SendP2pData2Player(
			NxPlayMgr::GetPlay(vUdpPlayers[i]->_uin)->GetPlayID()	,
			NxPlayMgr::GetPlay(vUdpPlayers[i]->_uin)->GetPlayUin()	,
			sizeof(STUDPPlayMove)	,
			(unsigned char*)&UdpPk	,
			true);
			vUdpPlayers[i]->_lastTime = GetTickCount();
			vUdpPlayers[i]->_deltaTime = 0;
//			LogText("Send Udp Pkg to Uin : %d \n", vUdpPlayers[i]->_uin); 
		}
	}

	if ( vTcpPlayser.empty()) 
		return;


	INT				PlayCount = 0;
	SHORT			PlayIDs[MAX_PLAYER_NUM] =	{0,0,0,0,0,0,0,0};
	unsigned int	PlayUins[MAX_PLAYER_NUM] =	{0,0,0,0,0,0,0,0};


	for ( int k = 0; k < vTcpPlayser.size(); k++) {
		if (NxPlayMgr::GetPlay(vTcpPlayser[k]->_uin))
		{
			if (( m_nCurTime - vTcpPlayser[k]->_lastTime ) 
				< 1000 / vTcpPlayser[k]->_frequence) 
				continue;
			else {
				PlayIDs[PlayCount]	= NxPlayMgr::GetPlay(vTcpPlayser[k]->_uin)->GetPlayID();
				PlayUins[PlayCount] = NxPlayMgr::GetPlay(vTcpPlayser[k]->_uin)->GetPlayUin();  
				// 冗余的调用，感觉格式漂亮，嘿嘿
				PlayCount++;
				vTcpPlayser[k]->_pkgid++ ;
				vTcpPlayser[k]->_lastTime = GetTickCount();
			}
		}
	}
	
	if ( PlayCount <= 0 ) 
		return;

	if (PlayCount > MAX_PLAYER_NUM)
	{
		LogText( "NxGamePkgMgr::Update(); UDP 转发 的 Play数量越界 请切歌 ！\n " );
			assert(0);
		return;
	}
	
	//if ( IsDebuggerPresent() )
	{
		UdpPk.m_unPackageIndex = vTcpPlayser[0]->_pkgid;
		GlobalInterface::m_pQQTppp->BroadCastDataViaServer
			(PlayCount,PlayIDs,PlayUins,sizeof(STUDPPlayMove),(unsigned char*)&UdpPk);
	}


}


//-----------------------------------------------------------------------
/*-----------------------------------------------------------------------
分析逻辑： 
1。玩家中途离开  不发包, bug 缺
2。远程玩家已经到达终点 上报服务器 不再发UDP包 
3。本地玩家到达终点 上报服务器 不再发 UDP包
-----------------------------------------------------------------------*/
void NxGamePkgMgr::Analaysis(eAnalaysis AnaType)
{

	Play * pHostPlay = NULL;
	Play * pRemotePlay = NULL;

	switch (AnaType)
	{
	case ANA_DISTANCE:
		{
			pHostPlay = NxPlayMgr::GetHostPlay();
			std::vector< SenderNotebook >::iterator it = _MapUdpSendTable.begin();
			for( ; it != _MapUdpSendTable.end(); it++)
			{
				if ( (*it)._byserver ==  true ) {
					(*it)._frequence = 9;
				}
				else {

					if (!NxPlayMgr::GetPlay((*it)._uin))
						continue;

					pRemotePlay = NxPlayMgr::GetPlay((*it)._uin);
					ULONG nDis = ComputeDistance(pHostPlay,pRemotePlay);
					ULONG nFre = GetFrequence(nDis);
					(*it)._frequence = nFre;
				}
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------
// 要根据 玩家是否完成比赛 处理
void NxGamePkgMgr::SendTcp(float fTime)
{
	if ( m_bSendLastTcp )
		return;

	if (NxPlayMgr::GetHostPlay()->IsFinish())
	{
		m_bSendLastTcp = true;
		SendTcpPkg(fTime);
		m_nTcpLastSendTime = ::GetTickCount();
		return;
	}

	if (  GetTickCount() - m_nTcpLastSendTime < SEND_TCPPACK_TIME )
	{
		return;
	}

	SendTcpPkg(fTime);
	m_nTcpLastSendTime = ::GetTickCount();
}

void NxGamePkgMgr::SendTcpPkg(float fTime)
{

	STUDPPlayMove uData;
	CRequestReportCurrentInfoPara aPara;
	NxPlayMgr::GetHostPlay()->GetKartParam(&uData);


	aPara.m_unUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
	aPara.m_unTime = GetTickCount();
	aPara.m_iCurrentStatus = 0 ; // bug 道具加速，漂移加速的状态功能为实现

	aPara.m_iCurrenRate[0] = uData.m_unLinearVelocity[0];
	aPara.m_iCurrenRate[1] = uData.m_unLinearVelocity[1];
	aPara.m_iCurrenRate[2] = uData.m_unLinearVelocity[2];


	aPara.m_iCurrentPosition[0] = uData.m_unArrPosition[0];
	aPara.m_iCurrentPosition[1] = uData.m_unArrPosition[1];
	aPara.m_iCurrentPosition[2] = uData.m_unArrPosition[2];

	std::vector<CCheckPoint*> vPath;
	vPath = CLevelManager::Get()->GetLinks()->GetLinkGoPath();
	aPara.m_bPassedCheckPointNum = min__( vPath.size(), MAXCHECKPOINTNUM );

	for ( int i = 0 ; i < aPara.m_bPassedCheckPointNum; i++ )
	{
		aPara.m_nPassedCheckPointIDs[i] = vPath[i]->nIndex;
		LogText("NxGamePkgMgr::SendTcpPkg();  Point ID = %d \n",vPath[i]->nIndex);
	}

	LogText("----------------------------------------------------\n");

	GlobalInterface::m_pNetwork->RequestGameServer(
		ID_CMS_REQUESTREPORTCURRENTINFO,
		(PBYTE)&aPara, 
		0,FE_GAMELOGIC,
		NxSceneMgr::Get()->GetGameSvrID());

	CLevelManager::Get()->GetLinks()->ResetGoPath();
}
//-----------------------------------------------------------------------

ULONG NxGamePkgMgr::ComputeDistance(Play * pPlay1, Play * pPlay2)
{
	if( pPlay1 == NULL || pPlay2 == NULL)
		return MAX_DISTANCE;
	
	NiPoint3 p1 = pPlay1->GetPos();
	NiPoint3 p2 = pPlay2->GetPos();

	return sqrt(
	(p1.x - p2.x) * (p1.x - p2.x) +
	(p1.y - p2.y) * (p1.y - p2.y) +
	(p1.z - p2.z) * (p1.z - p2.z) );

}

ULONG NxGamePkgMgr::GetFrequence(ULONG nDistance)
{
	// modify by hoverzhao 
	// 车辆模型缩小一半
	// 车体间隔最小单位由1变为0.5
	// end modify     2007.2.5
	for ( int i = 0; i < 5 ; i ++ )
	{
		if ( nDistance >= DescSendPkg::SendPkg[i]._begin_dis
		&&	 nDistance < DescSendPkg::SendPkg[i]._end_dis )
			return DescSendPkg::SendPkg[i]._frequence;
	}

	return 0;
}

//-----------------------------------------------------------------------
void NxGamePkgMgr::SendUdpPkgImmt( void * pData,ENMPACKTYPE eType /* = EPT_NORMAL */)
{

	if ( !pData)
		return;

	if ( EPT_PLAYCOLLISON == eType )
	{
		STUDPPlayCollision * pPkg = ( STUDPPlayCollision * ) pData;

		pPkg->MsgID = UDP_MSG_COLLSION;

		GlobalInterface::m_pQQTppp->SendP2pData2Player(
			NxPlayMgr::GetPlay(pPkg->m_unOtherUin)->GetPlayID()	,
			NxPlayMgr::GetPlay(pPkg->m_unOtherUin)->GetPlayUin()	,
			sizeof(STUDPPlayCollision)	,
			(unsigned char*)pPkg	,
			true);
	}

	return;
}

//-----------------------------------------------------------------------
void NxGamePkgMgr::TriggerUdpCollisionPkg( STUDPPlayCollision * pData)
{
	if ( !pData ) 
		return;


	//TODO:
	if (NxPlayMgr::GetHostPlay() && NxPlayMgr::GetHostPlay()->GetKart() && NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle())
		NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->RecvCollisionPacket(pData);	

	return;
}

//-----------------------------------------------------------------------
void NxGamePkgMgr::UnRegistLisener( int iCookie )
{
	std::map<int , IQQCarLogicNet* >::iterator iter = ms_arrLisener.find(iCookie);
	if( iter != ms_arrLisener.end())
	{
		ms_arrLisener.erase(iter);
	}
}

//-----------------------------------------------------------------------
int NxGamePkgMgr::RegistLisener( IQQCarLogicNet * pLisener )
{
	assert(pLisener);
	ms_arrLisener[m_nRegistCookie] = pLisener;
	return 	++m_nRegistCookie;
}


//-----------------------------------------------------------------------
void NxGamePkgMgr::OnNetMessage(UINT MsgID, void * pData, int MsgType /* = 0 */)
{
	std::map<int, IQQCarLogicNet* >::iterator iter = ms_arrLisener.begin();
	for ( ; iter != ms_arrLisener.end(); iter++ )
	{
		iter->second->OnNetMessage(MsgID,pData );
	}
	return;
}

