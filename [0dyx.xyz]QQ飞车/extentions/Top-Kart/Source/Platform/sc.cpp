////////////////////////////////////////////////
//     FileName:    script.cpp
//     Author:      Fish
//     Date:        2006-10-11 10:46:26
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "sc.h"
#include "sc_err.h"
#include "framework/GameApp_i.h"
#include "framework/gui_i.h"
#include "Network/Network_i.h"
#include "Network/consts.h"
#include "network/userdata.h"
#include "QQTppp_i.h"
#include "GameUI.h"
#include "common.h"
#include "algorithm"
#include "GlobalInterface.h"
#include "MsgToLua.h"
#include "UIMiscMgr.h"
#include "MsgDataCache.h"
#include "datadef.h"
#include "scriptutils.h"
#include "../nifalloc.h"
#include "../palyerlevel.h"
#include "../Avatar/AvatarMgr.h"
#include "../Avatar/AvatarNet.h"
#include "../Avatar/ShopMgr.h"
#include "../PhysKart/kart.h"
#include "../PhysKart/TencentCar.h"
#include "../scene.h"
#include "../Task/TaskMgr.h"
#include "../Task/TaskSys.h"
#include "../Task/TaskNet.h"
#include "../NifLoader.h"
#include "../IDisplayD3D9/FlashScreen.h"
#include "../Win32SoundManager.h"
#include "../Task/AwardMgr.h"

//用户登陆相关函数
// args		: (none)
// retvalue	: user, pwd, 为空表示不存在
int SC_GetLastUserName(lua_State *ls)
{
	char szuser[40] = "";
	char szpwd[40] = "";
	CUserDataProfile aProfile;
	int count = aProfile.GetUserCount();
	if ( count > 0 )
	{
		UINT Uin = aProfile.GetUserData(count - 1).Uin;
		_snprintf(szuser, 40, "%d", Uin);
		strncpy(szpwd, aProfile.GetUserData(count - 1).Pwd, 40);
	}
	lua_pushstring(ls, szuser);
	lua_pushstring(ls, szpwd);
	return 2;
}

//args	: none
//retvalue	: dirinfo
int SC_GetDirInfo( lua_State *ls )
{
	DirResponseInfo* pDirInfo = GlobalInterface::m_pNetwork->GetDirInfo();
	if ( pDirInfo )
	{
		lua_newtable(ls);
		DEFINE_MSGTOLUA(ls, pDirInfo);
		return 1;
	}
	return 0;
}

//args		: user, pwd
//retvalue	: sc_err
int SC_Login( lua_State *ls )
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 2 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		LPCSTR id = lua_tostring(ls,1);
		char pwd[100] = { 0 };
		
		IWgtData* pWgt = GameUIGetWidget( lua_tostring(ls,2) );
		if( pWgt)
		{
			stUIVar uiVar(VALUE_CHANNEL_CAPTION, 0, 0, NULL);
			pWgt->GetVar(uiVar);
			strncpy( pwd, uiVar.sValue.c_str(), 100);

			int len = uiVar.sValue.length();
			if (len>0) uiVar.sValue = std::string( len, ' ' );
			pWgt->SetVar(uiVar);
		}

		if ( !id || strlen(id) == 0 )
		{
			retval = sc_EmptyUin;
			goto KK_EXIT;
		}

		if ( strlen(pwd) == 0 )
		{
			retval = sc_EmptyPwd;
			goto KK_EXIT;
		}

		GlobalInterface::m_pNetwork->SetLoginType(ELT_Single);
		HRESULT hr = GlobalInterface::m_pNetwork->StartLogin(atoi(id), (PBYTE)pwd);
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retvalue	: sc_err
int SC_CancelLogin(lua_State *ls)
{
	int retval = sc_Unexpected;
	{
		KK_CHECK_POINTER(GlobalInterface::m_pNetwork);
		HRESULT hr = GlobalInterface::m_pNetwork->CancelLogin();
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

int SC_CheckName(lua_State * ls)
{
	int retval = sc_Unexpected;

	{
		KK_CHECK_BOOL(lua_gettop(ls) >= 1);
		const char * nickname = lua_tostring(ls, 1);

		CRequestCheckNamePara kPara;
		strncpy(kPara.m_szNickname, nickname, MAXQQKARTNICKNAMELEN-1);
		kPara.m_szNickname[MAXQQKARTNICKNAMELEN - 1] = 0;
		kPara.m_unTime = GetTickCount();
		LPQQUSERDATA pData = (LPQQUSERDATA)GlobalInterface::m_pNetwork->GetQQUserData();
		kPara.m_unUin = pData->Uin;
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCHECKNAME, (PBYTE)&kPara, 0, FE_PLAYER, 0);
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

int SC_RegistUser(lua_State *ls)
{
	int retval = sc_Unexpected;
	{
		KK_CHECK_BOOL(lua_gettop(ls) >= 7);
		const char * nickname = lua_tostring(ls, 1);
		int gender = lua_tonumber(ls, 2);
		int faceid = lua_tonumber(ls, 3);
		int hairid = lua_tonumber(ls, 4);
		int dressindex = lua_tonumber(ls, 5);
		int colorid = lua_tonumber(ls, 6);
		int countryid = lua_tonumber(ls, 7);
		KK_CHECK_POINTER(GlobalInterface::m_pNetwork);
		LPQQUSERDATA pData = (LPQQUSERDATA)GlobalInterface::m_pNetwork->GetQQUserData();
		/*
		USHORT 	m_unUin;                      		//玩家的Uin
		USHORT 	m_unTime;		       		//玩家注册时间	
		CHAR  m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
		UCHAR 	m_bGender;   		 				//玩家性别，0表示女，1表示男
		SHORT			m_iFaceID;                                  //玩家脸型
		SHORT			m_iHairID;                                   //玩家头发
		UCHAR   m_bDressIndex;                       //套装索引
		SHORT 			m_iColorID;		       		//玩家喷漆颜色ID
		UCHAR  m_bCountry;
		*/
		CRequestRegisterPara kPara;
		kPara.m_unUin = pData->Uin;
		kPara.m_unTime = GetTickCount();
		strncpy(kPara.m_szNickname, nickname, MAXQQKARTNICKNAMELEN-1);
		kPara.m_szNickname[MAXQQKARTNICKNAMELEN-1] = 0;
		kPara.m_bGender = gender;
		kPara.m_nFaceID = faceid;
		kPara.m_nHairID = hairid;
		kPara.m_bDressIndex = dressindex;
		//kPara.m_iRoleID = roleid;
		kPara.m_nColorID = colorid;
		kPara.m_bCountry = countryid;
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTREGISTER, (PBYTE)&kPara, 0, FE_GAMESVRD, 0);
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retvalue	: sc_err
int SC_Logout(lua_State* ls)
{
	int retval = sc_Unexpected;
	{
		KK_CHECK_POINTER(GlobalInterface::m_pNetwork);
		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		CRequestLogoutPara aPara;
		aPara.m_unUin = pPara->Uin;
		aPara.m_unTime = GetTickCount();
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTLOGOUT, (PBYTE)&aPara, 0, FE_GAMESVRD, 0);
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retvalue	: count, users
int SC_GetRecentUsers(lua_State *ls)
{
	char szuser[40] = "\0";
	CUserDataProfile aProfile;
	int count = aProfile.GetUserCount();
	lua_pushnumber(ls, count);
	lua_newtable(ls);
	int index = 1;
	for ( int i = count-1; i >= 0; i-- )
	{
		UINT Uin = aProfile.GetUserData(i).Uin;
		_snprintf(szuser,40, "%d", Uin);
		BeginTableElement(ls, index++);
		SetTableElement(ls, "uin", szuser);
		EndTableElement(ls);			
	}
	return 2;
}

//args	: [LoginType], [Channel], [SvrID]
//retvalue	: 
int SC_LoginGame(lua_State* ls)
{
	UINT nLoginType = ELT_Multi;
	if ( lua_gettop(ls) >= 1 )
	{
		nLoginType = lua_tonumber(ls, 1);
	}
	GameResponseInfo* pGameInfo = GlobalInterface::m_pNetwork->GetGameInfo();
	if ( !pGameInfo )
	{
		XWaring("pGameInfo NULL, cannot logingame");
		return 0;
	}
	DWORD dwCountry = pGameInfo->m_stPlayerDBBaseInfo.m_bCountry;
	
	//DWORD dwCountry = GlobalInterface::m_pNetwork->GetGameInfo()->m_stPlayerDBBaseInfo.m_bCountry;
	DWORD dwChannel = 0;
	if ( lua_gettop(ls) >= 2 )
	{
		dwChannel = lua_tonumber(ls, 2);
	}
	DWORD dwSvrID = 0;
	if ( lua_gettop(ls) >= 3 )
	{
		dwSvrID = lua_tonumber(ls, 3);
	}
	GlobalInterface::m_pNetwork->SetLoginType(nLoginType);
	GlobalInterface::m_pNetwork->LoginGameServer(dwCountry, dwChannel, dwSvrID);
	return 0;
}

// 请求检查名字的有效性
int SC_RequestCheckName(lua_State * ls)
{
	int retval = sc_Unexpected;

	QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	CRequestCheckNamePara aPara;
	if (pPara)
	{
		const char * pName = lua_tostring(ls, 1);		
		strncpy(aPara.m_szNickname, pName, MAXQQKARTNICKNAMELEN - 1);
		aPara.m_szNickname[MAXQQKARTNICKNAMELEN-1] = 0;
		aPara.m_unTime = GetTickCount();
		aPara.m_unUin  = pPara->Uin;

		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCHECKNAME, (PBYTE)&aPara, 0, FE_PLAYER, 0);
		KK_CHECK(hr);
		retval = sc_Successed;
	}

KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}
//请求分属性点
//args : 速度
//		 爆发
//		 集气
//		 喷射
//		 小喷	
//		restpoints = 255,
//retval: responselogin, sc_err

int SC_RequestAssignAttribute(lua_State * ls)
{
	int retval = sc_Unexpected;
	CRequestAssignAttributePara aPara;

	QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	if (pPara)
	{

		KK_CHECK_BOOL(lua_gettop(ls) >= 7);
		int nSpeed		= lua_tonumber(ls, 1);
		int nExplosion	= lua_tonumber(ls, 2); 
		int nGathering	= lua_tonumber(ls, 3);
		int nSpout		= lua_tonumber(ls, 4);;     
		int nMiniSpout	= lua_tonumber(ls, 5); 
	    int nReserve	= lua_tonumber(ls, 6); 
		char bStrategy	= lua_tonumber(ls, 7);  
		


		aPara.m_unUin = pPara->Uin;
		aPara.m_unTime = GetTickCount();
		aPara.m_stPlayerDBAttributeInfo.m_nSpeed		= nSpeed;
		aPara.m_stPlayerDBAttributeInfo.m_nExplosion	= nExplosion;
		aPara.m_stPlayerDBAttributeInfo.m_nGathering	= nGathering;
		aPara.m_stPlayerDBAttributeInfo.m_nSpout		= nSpout;
		aPara.m_stPlayerDBAttributeInfo.m_nMiniSpout	= nMiniSpout;
		aPara.m_stPlayerDBAttributeInfo.m_unReserve		= nReserve;
		aPara.m_stPlayerDBAttributeInfo.m_bStrategy		= bStrategy;

		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTASSIGNATTRIBUTE, (PBYTE)&aPara, 0, FE_PLAYER, 0);
		KK_CHECK(hr);
		retval = sc_Successed;
	}

KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

// 清除配点
int SC_RequestClearAttribute(lua_State * ls)
{
	int retval = sc_Unexpected;
	CRequestClearAttributePara aPara;
	QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	if (pPara)
	{
		aPara.m_unUin = pPara->Uin;
		aPara.m_unTime = GetTickCount();
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCLEARATTRIBUTE, (PBYTE)&aPara, 0, FE_PLAYER, 0);
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 2;
}
bool SetKartParam(int dwPID, float fParam, float fLifeTime)
{
	Play * play = NxPlayMgr::GetHostPlay();
	CTencentCar * pCar = NULL;
	if (play && (play->GetKart())) 
	{
		pCar = play->GetKart()->GetVehicle();
	}
	if (pCar)
	{
		std::list<CTencentCar::PHYSINFO> &PhysList = pCar->GetCarPhysParam();

		std::list<CTencentCar::PHYSINFO>::iterator iter, end;
		iter = PhysList.begin();
		end  = PhysList.end();
		for (; iter != end; ++iter)
		{
			if(iter->dwPID == dwPID)
			{
				iter->fParam = fParam;
				iter->fLifeTime = fLifeTime;
				return true;				
			}
		}	
	}

	return false;
}
// 根据属性点设置车的物理参数
int SC_SetKartAttrib(lua_State * ls)
{
	int retval = sc_Unexpected;
	if (lua_gettop(ls) == 3)
	{
		int dwPID		= lua_tonumber(ls, 1);
		//int dwAffetID	= lua_tonumber(ls, 2); 
		float fParam	= lua_tonumber(ls, 2);
		float fLifeTime	= lua_tonumber(ls, 3);;     
		
		
		if (SetKartParam(dwPID, fParam, fLifeTime))
		{
			retval = sc_Successed;
		}
		else
		{
			retval = sc_InvalidArg;
		}
	}
	else
		retval = sc_InvalidArg;
	lua_pushnumber(ls, retval);
	return 1;
}
//应用程序杂项函数
//args	: name
//retval	: value, sc_err
int SC_GetGameVar(lua_State *ls)
{
	int retval = sc_Unexpected;
	int usedef = 0;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		const char *pname = lua_tostring(ls, 1);
		if ( !pname || strlen(pname) == 0 )
			retval = sc_InvalidArg;
		else	
		{
			assert(GlobalInterface::m_pSettings);
			HRESULT hr = GlobalInterface::m_pSettings->GetIntValue(pname, &usedef);
			KK_CHECK(hr);
		}
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, usedef);
	lua_pushnumber(ls, retval);
	return 2;
}

//args	: name, value
//retval	: sc_err
int SC_SetGameVar(lua_State *ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 2 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		const char *pname = lua_tostring(ls, 1);
		if ( !pname || strlen(pname) == 0 )
		{
			retval = sc_InvalidArg;
		}
		else
		{
			assert(GlobalInterface::m_pSettings);
			int	usedef = int(lua_tonumber(ls,2));
			HRESULT hr = GlobalInterface::m_pSettings->SetIntValue(pname, usedef);
			KK_CHECK(hr);
			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}


//框架逻辑控制相关函数
//args : id
//retval	: sc_err
int SC_ActiveExt(lua_State *ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		assert(GlobalInterface::m_pApp);
		const char* pname = lua_tostring(ls, 1);
		if ( !pname || strlen(pname) == 0 )
			retval = sc_InvalidArg;
		else
		{
			retval = sc_TargetNotFound;
			HRESULT hr = GlobalInterface::m_pApp->ActiveExtension(pname);
			KK_CHECK(hr);
			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

///改变指定玩家所有Avatar的Item状态
//args	: uin,itemid
//retval	: sc_err
int SC_ChangeAvatarStatus(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 2 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		bool bEquip = false;
		if ( lua_gettop(ls) > 2 )
		{
			bEquip = lua_tonumber(ls, 3);
		}
		UINT uin = ((LPQQUSERDATA)GlobalInterface::m_pNetwork->GetQQUserData())->Uin;
		CRequestChangeItemStatus insSta;
		insSta.m_sItemNum=1;
		insSta.m_unUin=lua_tonumber(ls, 1);
		insSta.m_unTime=::GetTickCount();
		insSta.m_stItemStatus[0].m_iItemID=lua_tonumber(ls,2);
		ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(insSta.m_stItemStatus[0].m_iItemID);
		ItemStore*	pStore=CAvatarMgr::Get()->GetItemDataMgr()->GetItemStore(uin,pItem->nId);
		insSta.m_stItemStatus[0].m_bNewStatus=!pStore->insInfo.m_bStatus;
		CAvatarMgr::Get()->ChangeItemStatus(&insSta,bEquip);//不装备，只是查询处理列表
		if (insSta.m_sItemNum && !bEquip)
		{//有需要处理才发送网络消息
			CAvatarNet::RequestChangeItemStatus(&insSta);
		}
	}
	lua_pushnumber(ls, retval);
	return 1;
}

///获取指定玩家所有Avatar的Item列表
//args	: uin
//retval	: items,num,sc_err
int SC_GetAvatarAllList(lua_State* ls)
{ 
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnil(ls);	//items
		lua_pushnumber(ls, 0);	//num
		retval = sc_InvalidArg;
	}
	else
	{
		UINT uin = lua_tonumber(ls, 1);
		QueryPlayerAvatarInfo insGet;
		insGet.m_unUin_In=uin;
		insGet.m_cQueryMode_In=EIQIT_ALL_ITEM;
		CAvatarMgr::Get()->GetPlayerItemList(&insGet);
		/*
		if (insGet.m_nReturnItemNum_Out)
		{
			lua_newtable(ls);
			UINT nCnt = 0;
			for ( ; nCnt < insGet.m_nReturnItemNum_Out; ++nCnt )
			{
				DEFINE_MSGTOLUA_IDX(ls, &insGet.m_stPlayerItemInfo_Out[nCnt], nCnt+1);
				lua_settable(ls, -3);
			}
			retval = sc_Successed;
		}
		else
		{///读取数据失败
			lua_pushnil(ls);	//items
		}
		lua_pushnumber(ls, insGet.m_nReturnItemNum_Out);		
		*/
		lua_newtable(ls);
		DEFINE_MSGTOLUA(ls,&insGet);
		retval=sc_Successed;
	}
	lua_pushnumber(ls, retval);
	return 2;
}

///获取玩家指定Avatar的Item信息
//在该车库分类中顺序排序的索引号，从0开始
int SC_GetAvatarInfoByType(lua_State* ls)
{
	bool bRes=false;
	if ( lua_gettop(ls) < 2 )
	{
		bRes=false;
	}
	else
	{
		UINT nGarageType=lua_tonumber(ls,1);
		UINT nIndexId=lua_tonumber(ls,2); //在该车库分类中顺序排序的索引号，从0开始
		UINT dwUin=NxPlayMgr::GetHostPlay()->GetPlayUin();
		//_ASSERTE(nGarageType && nIndexId<CAvatarMgr::Get()->GetItemDataMgr()->GetGarageTypeObj(dwUin,nGarageType)->maStorePt.size());
		if (nGarageType && nIndexId<CAvatarMgr::Get()->GetItemDataMgr()->GetGarageTypeObj(dwUin,nGarageType)->maStorePt.size())
		{
			
			ITEMPOINTER_MAP::iterator it=CAvatarMgr::Get()->GetItemDataMgr()->GetGarageTypeObj(dwUin,nGarageType)->maStorePt.begin();
			ItemStore* pStore=NULL;
			UINT nCnt=0;
			for(;it!=CAvatarMgr::Get()->GetItemDataMgr()->GetGarageTypeObj(dwUin,nGarageType)->maStorePt.end();++it)
			{
				pStore=it->second;
				if (nCnt==nIndexId)
				{
					break;
				}
				nCnt++;
			}
			if(it!=CAvatarMgr::Get()->GetItemDataMgr()->GetGarageTypeObj(dwUin,nGarageType)->maStorePt.end())
			{
				if (pStore)
				{
					lua_newtable(ls);
					DEFINE_MSGTOLUA(ls,&pStore->insInfo);
					ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(pStore->insInfo.m_iItemID);
					lua_pushstring(ls,pItem->strDesc.c_str());
				}
				else
				{
					_ASSERTE(0);
				}
			}
			bRes=true;
		} 
		else
		{
			bRes=false;
		}
	}
	//
	if (!bRes)
	{
		lua_pushnil(ls);
		lua_pushstring(ls,"no");
	}
	return 2;
}

///获取玩家拥有的按车库分类的avtar数量
int SC_GetAvatarTypeCount(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnumber(ls, 0);	//num
	}
	else
	{
		UINT nGarageType=lua_tonumber(ls,1);
		_ASSERTE(nGarageType);
		UINT dwUin=NxPlayMgr::GetHostPlay()->GetPlayUin();
		UINT nTypeNum=CAvatarMgr::Get()->GetItemDataMgr()->GetGarageTypeObj(dwUin,nGarageType)->maStorePt.size();
		lua_pushnumber(ls, nTypeNum);
	}
	return 1;
}

//房间逻辑控制相关函数
//args	: basemode, submode, from, num, bRequest
//retval	: from, num, rooms, totnum, sc_err
int SC_GetRoomList(lua_State* ls)
{	
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 4 )
	{
		lua_pushnumber(ls, 0);	//from
		lua_pushnumber(ls, 0);	//num
		lua_pushnil(ls);	//rooms
		lua_pushnumber(ls, 0);	//totnum
		retval = sc_InvalidArg;
	}
	else
	{
		int basemode = lua_tonumber(ls, 1);
		int submode = lua_tonumber(ls, 2);
		int from = lua_tonumber(ls, 3);
		int num = lua_tonumber(ls, 4);			
		BOOL request = FALSE;
		if ( lua_gettop(ls) > 4 )
			request = lua_tonumber(ls, 5);

		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		CResponseGetRoomListPara& refRoomList = pCache->m_RoomList;
		if ( refRoomList.m_nResultID == S_OK 
			&& refRoomList.m_sGameModeBase == basemode
			&& refRoomList.m_sGameModeSub == submode )
		{
			//取交集
			int indexfrom = __max(from, refRoomList.m_nStartIndex);
			int indexto = __min(from+num, refRoomList.m_nStartIndex+refRoomList.m_nRoomNum);

			request = false;
			//不完整的情况
			if ( indexto - indexfrom < num && from + num <= refRoomList.m_nCurTotalRoomNum )
				request = true;

			//取了部分或全部取得，填充数据
			if ( indexto > indexfrom )
			{
				lua_pushnumber(ls, indexfrom);
				lua_pushnumber(ls, indexto - indexfrom);

				lua_newtable(ls);
				INT index = 1;
				for ( int i = 0; i < indexto - indexfrom; i++ )
				{
					STRoomInfo& refRoom = refRoomList.m_stRooms[indexfrom+i];
					DEFINE_MSGTOLUA_IDX(ls, &refRoom, index);
					lua_settable(ls, -3);
					index++;
				}
				lua_pushnumber(ls, refRoomList.m_nCurTotalRoomNum);
				retval = sc_Successed;
			}
			else
			{
				lua_pushnumber(ls, 0);	//from
				lua_pushnumber(ls, 0);	//num
				lua_pushnil(ls);	//rooms
				lua_pushnumber(ls, 0);	//totnum
				retval = sc_TargetNotFound;
			}
		}

		static DWORD dwLastRequestTick = GetTickCount();
		DWORD dwCurTick = GetTickCount();
		if ( dwCurTick - dwLastRequestTick > 10000 )
		{
			request = TRUE;
			dwLastRequestTick = dwCurTick;
		}
	
		if ( request )
		{
			QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
			//取60个，前后各扩充
			INT nOffset = (MAXCLIENTGETROOMNUM - num) / 2;
			from -= nOffset;
			if ( from < 0 )
			{
				nOffset -= from;
				from = 0;
			}
			num = MAXCLIENTGETROOMNUM;

			CRequestGetRoomListPara aPara;
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_nStartRoomNum = from;
			aPara.m_nGetRoomNum = num;
			aPara.m_sGameModeBase = basemode;
			aPara.m_sGameModeSub  = submode;
			
			GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTGETROOMLIST, (PBYTE)&aPara, 0, FE_GAMESVRD, 0);
		}
	}
	lua_pushnumber(ls, retval); 
	return 5;
}

// 转人
int SC_SelfRoleShowTurn(lua_State * ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;

	else
	{
		float fDegree = lua_tonumber(ls, 1);

		NiMatrix3 kZDeltaRotate;
		kZDeltaRotate.MakeZRotation(NxPi * fDegree / 180.f);
		
		Play * pSelf = NxPlayMgr::GetHostPlay();

		if (pSelf)
		{
			NiAVObject * pkObj = pSelf->GetCharacterNIFRoot();//->GetObjectByName("Avatar");
			pkObj->SetRotate(pkObj->GetRotate() * kZDeltaRotate);
			retval = sc_Successed;
		}
		else
			retval = sc_Unexpected;
	}

	lua_pushnumber(ls, retval);

	return 1;
}
// 恢复人
int SC_RecoverRoleTurn(lua_State * ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		float fDegree = lua_tonumber(ls, 1);

		Play * pSelf = NxPlayMgr::GetHostPlay();

		if (pSelf)
		{
			NiAVObject * pkObj = pSelf->GetCharacterNIFRoot();//->GetObjectByName("Avatar");
			NiMatrix3 kRotate = pkObj->GetRotate();
			float x = 0, y = 0, z = 0;
			kRotate.ToEulerAnglesXYZ(x, y, z);
			NiMatrix3 kZDeltaRotate;

			float fRotateStepRad = NxPi / 180.f;
			if (fabsf(z) <= fRotateStepRad)
			{
				kRotate = NiMatrix3::IDENTITY;
				retval = 1;
			}
			else	
			{
				kZDeltaRotate.MakeZRotation(-0.2f * fabsf(z) * (fDegree));
				kRotate = kRotate * kZDeltaRotate;
				retval = sc_Successed;
			}
			pkObj->SetRotate(kRotate);

		}
		else
			retval = sc_Unexpected;
	}

	lua_pushnumber(ls, retval);

	return 1;
}
// 转车
int SC_SelfShowTurn(lua_State * ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;

	else
	{
		float fDegree = lua_tonumber(ls, 1);

		NiMatrix3 kZDeltaRotate;
		kZDeltaRotate.MakeZRotation(NxPi * fDegree / 180.f);
		
		Play * pSelf = NxPlayMgr::GetHostPlay();

		if (pSelf)
		{
			NiAVObject * pkObj = pSelf->GetKartNIFRoot()->GetObjectByName("MODE");
			pkObj->SetRotate(pkObj->GetRotate() * kZDeltaRotate);
			retval = sc_Successed;
		}
		else
			retval = sc_Unexpected;
	}

	lua_pushnumber(ls, retval);

	return 1;
}

//把车子转到对于初始矩阵一个指定角度的位置
int SC_SelfShowFixedTurn(lua_State * ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;

	else
	{
		float fDegree = lua_tonumber(ls, 1);

		Play * pSelf = NxPlayMgr::GetHostPlay();

		if (pSelf)
		{
			NiAVObject * pkObj = pSelf->GetKartNIFRoot();//->GetObjectByName("MODE");
			//NiMatrix3 kRotate = pkObj->GetRotate();
			NiMatrix3 kZDeltaRotate;
			kZDeltaRotate.MakeZRotation(NxPi * fDegree / 180.f);
			//kRotate = kRotate * kZDeltaRotate;
			retval = sc_Successed;
			pkObj->SetRotate(kZDeltaRotate);
		}
		else
		{
			retval = sc_Unexpected;
		}
	}

	lua_pushnumber(ls, retval);

	return 1;
}

int SC_RecoverSelfTurn(lua_State * ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		float fDegree = lua_tonumber(ls, 1);

		Play * pSelf = NxPlayMgr::GetHostPlay();

		if (pSelf)
		{
			bool bForeceRecover = false;
			if (lua_gettop(ls) >= 2)
				bForeceRecover = true;


			NiAVObject * pkObj = pSelf->GetKartNIFRoot()->GetObjectByName("MODE");
			NiMatrix3 kRotate = pkObj->GetRotate();
			if (!bForeceRecover)
			{				
				float x = 0, y = 0, z = 0;
				kRotate.ToEulerAnglesXYZ(x, y, z);
				NiMatrix3 kZDeltaRotate;

				float fRotateStepRad = NxPi / 180.f;
				if (fabsf(z) <= fRotateStepRad)
				{
					kRotate = NiMatrix3::IDENTITY;
					retval = 1;
				}
				else	
				{
					kZDeltaRotate.MakeZRotation(-0.2f * fabsf(z) * (fDegree));
					kRotate = kRotate * kZDeltaRotate;
					retval = sc_Successed;
				}
			}
			else
			{
				kRotate = NiMatrix3::IDENTITY;


			}
			pkObj->SetRotate(kRotate);

		}
		else
			retval = sc_Unexpected;
	}

	lua_pushnumber(ls, retval);

	return 1;
}
//args	: gamemodebase, gamemodesub, roomname, pcount, encrypt, pwd
//retval: sc_error
int SC_CreateRoom(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 4 )
		retval = sc_InvalidArg;
	else
	{
		int gamemodebase = lua_tonumber(ls, 1);
		int gamemodesub = lua_tonumber(ls, 2);
		const char* roomname = lua_tostring(ls, 3);
		int pcount = lua_tonumber(ls, 4);
		BOOL encrypt = FALSE;
		if ( lua_gettop(ls) > 4 )
			encrypt = lua_tonumber(ls, 5);
		const char* pwd = NULL;
		if ( encrypt && lua_gettop(ls) > 5 )
			pwd = lua_tostring(ls, 6);

		assert(GlobalInterface::m_pNetwork);
		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		if ( pPara )
		{
			CRequestCreateRoomPara aPara;
			memset(&aPara, 0, sizeof(aPara));
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_sGameModeBase = gamemodebase;
			aPara.m_sGameModeSub = gamemodesub;
			strncpy(aPara.m_szRoomName, roomname, MAXROOMNAMELEN - 1);
			aPara.m_bSeatNum = pcount;
			aPara.m_bFlag = encrypt;
			if ( pwd )
				strncpy(aPara.m_szPassword, pwd, MAXROOMPASSWORDLEN);
			HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCREATEROOM, (PBYTE)&aPara, 0, FE_GAMESVRD, 0);
			KK_CHECK(hr);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args : mapid
int SC_ChangeMap(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		int mapid = lua_tonumber(ls, 1);
		assert(GlobalInterface::m_pNetwork);
		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();

		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		if ( pPara )
		{
			CRequestChangeMapPara aPara;
			memset(&aPara, 0, sizeof(aPara));
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_iNewMapID = mapid;
			HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(
				ID_CMS_REQUESTCHANGEMAP, (PBYTE)&aPara, 0, FE_ROOM, refRoomDesc.RoomID);
			KK_CHECK(hr);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	return retval;
}


//args : teamid
int SC_ChangeTeam(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		int teamid = lua_tonumber(ls, 1);
		assert(GlobalInterface::m_pNetwork);
		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();

		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		if ( pPara )
		{
			CRequestChangeTeamPara aPara;
			memset(&aPara, 0, sizeof(aPara));
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_bNewTeamID = teamid;
			HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(
				ID_CMS_REQUESTCHANGETEAM, (PBYTE)&aPara, 0, FE_ROOM, refRoomDesc.RoomID);
			KK_CHECK(hr);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	return retval;
}

int SC_GetMyProp(lua_State * ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pNetwork);
	CResponseLoginPara* pPara = GlobalInterface::m_pNetwork->GetGameInfo();
	if (pPara)
	{
		STPlayerDBAttributeInfo pi;
		pi = pPara->m_stPlayerDBAttributeInfo;
		lua_newtable(ls);
		DEFINE_MSGTOLUA(ls, &pi);
		retval = sc_Successed;
	}
	else
	{
		lua_pushnil(ls);	//myinfo
	}
	lua_pushnumber(ls, retval);

	return 2;
}

//args	: (none)
//retval: myinfo, sc_err
int SC_GetMyInfo(lua_State* ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pNetwork);
	QQUSERDATA* pUser = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	CResponseLoginPara* pPara = GlobalInterface::m_pNetwork->GetGameInfo();
	CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
	if ( pUser && pPara && pCache)
	{		
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		STPlayerRoomVisibleInfo pi;
		memset(&pi, 0, sizeof(pi));
		pi.m_unUin = pPara->m_unUin;
		pi.m_unIdentity = pPara->m_unIdentity;
		pi.m_bSeatID = refRoomDesc.SeatID;
		pi.m_bTeamID = refRoomDesc.TeamID;
		pi.m_nPlayerID = pPara->m_nPlayerID;
		/*
		struct STPlayerDBBaseInfo
		{
		CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
		UCHAR	m_bGender;   		 				//玩家性别，0表示女，1表示男
		UCHAR	m_bCountry;	                                             //玩家国家
		USHORT 	m_unExperience;		       //经验
		SHORT 	m_iMoney;			       //金钱
		USHORT 	m_unWinNum;			//胜利次数
		USHORT 	m_unTotalNum;			//比赛总数
		USHORT  m_unHonor;                              //功勋值
		};
		*/
		strncpy(pi.m_stPlayerDBBaseInfo.m_szNickname, pPara->m_stPlayerDBBaseInfo.m_szNickname, MAXQQKARTNICKNAMELEN-1);
		pi.m_stPlayerDBBaseInfo.m_szNickname[MAXQQKARTNICKNAMELEN-1] = 0;
		pi.m_stPlayerDBBaseInfo.m_bCountry = pPara->m_stPlayerDBBaseInfo.m_bCountry;
		pi.m_stPlayerDBBaseInfo.m_bGender = pPara->m_stPlayerDBBaseInfo.m_bGender;
		pi.m_stPlayerDBBaseInfo.m_unExperience = pPara->m_stPlayerDBBaseInfo.m_unExperience;
		pi.m_stPlayerDBBaseInfo.m_iMoney = pPara->m_stPlayerDBBaseInfo.m_iMoney;
		pi.m_stPlayerDBBaseInfo.m_unWinNum = pPara->m_stPlayerDBBaseInfo.m_unWinNum;
		pi.m_stPlayerDBBaseInfo.m_unTotalNum = pPara->m_stPlayerDBBaseInfo.m_unTotalNum;
		pi.m_stPlayerDBBaseInfo.m_unHonor    = pPara->m_stPlayerDBBaseInfo.m_unHonor;
		
		lua_newtable(ls);
		DEFINE_MSGTOLUA(ls, &pi);
		BOOL bIsRoomer = ( refRoomDesc.RoomOwnerID == -1 || refRoomDesc.RoomOwnerID == pi.m_nPlayerID );
		SetTableElement(ls, "isroomer", bIsRoomer);
		retval = sc_Successed;
	}
	else
	{
		lua_pushnil(ls);	//myinfo
	}
	lua_pushnumber(ls, retval);
	return 2;
}

int SC_GetMyLevel(lua_State * ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pNetwork);
	CResponseLoginPara* pPara = GlobalInterface::m_pNetwork->GetGameInfo();

	if (pPara)
	{			
		int level = 0;
		int subexp = 0; 
		int levelexp = 0;
		if (GetPlayerLevel(pPara->m_stPlayerDBBaseInfo.m_unExperience, level, subexp, levelexp))
		{
			lua_pushnumber(ls, level);
		}
		else
		{
			lua_pushnil(ls);
		}
	}
	else
	{
		lua_pushnil(ls);	
	}
	lua_pushnumber(ls, retval);
	return 2;
}

int SC_GetMyAttribute(lua_State * ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pNetwork);
	CResponseLoginPara* pPara = GlobalInterface::m_pNetwork->GetGameInfo();
	if (pPara)
	{
		lua_newtable(ls);
		DEFINE_MSGTOLUA(ls, &pPara->m_stPlayerDBAttributeInfo);
		retval = sc_Successed;
	}
	else
	{
		lua_pushnil(ls);
	}

	lua_pushnumber(ls, retval);
	return 2;
}

int SC_IsSelf(lua_State * ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pNetwork);
	QQUSERDATA* pUser = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	if (pUser)
	{
		if ( lua_gettop(ls) < 1 )
		{
			retval = sc_InvalidArg;
		}
		else
		{
			unsigned int uin = lua_tonumber(ls, 1);
			lua_pushboolean(ls, uin == pUser->Uin ? 1 : 0);
			retval = sc_Successed;
		}
	}
	else
	{
		lua_pushnil(ls);	
	}
	lua_pushnumber(ls, retval);
	return 2;
}
//args	: (none)
//retval: roominfo
int SC_GetMyRoomInfo(lua_State* ls)
{
	CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
	if (!pCache)
		return 0;
	assert(pCache);
	RoomDesc& refRoomDesc = pCache->m_RoomDesc;
	lua_newtable(ls);
	DEFINE_MSGTOLUA(ls, &refRoomDesc);
	return 1;
}
// args: (uin)
// retval: level
int SC_GetPlayerLevelByExp(lua_State * ls)
{
	int retval = sc_Unexpected;
	if (lua_gettop(ls) != 1)
	{
		lua_pushnil(ls);
		retval = sc_InvalidArg;
		lua_pushnumber(ls, retval);
		return 2;
	}
	else
	{
		unsigned int experience = lua_tonumber(ls, 1);

		/*
		int level = MAX_LEVEL;
		for (int i = 0; i < MAX_LEVEL-1; ++i)
		{
			if (experience >= levels[i] && experience < levels[i+1])
			{
				level = i + 1;
				break;
			}

		}
		*/
		int level = 0;
		int subexp = 0; 
		int levelexp = 0;
		if (GetPlayerLevel(experience, level, subexp, levelexp))
		{
			lua_pushnumber(ls, level);
			lua_pushnumber(ls, subexp);
			lua_pushnumber(ls, levelexp);
			retval = sc_Successed;
			lua_pushnumber(ls, retval);
			return 4;
		}
		else
		{
			lua_pushnil(ls);
			retval = sc_Unexpected;
			lua_pushnumber(ls, retval);
			return 2;
		}
	}

	
}
/*
int SC_GetPlayerLevel(lua_State * ls)
{
	int retval = sc_Unexpected;
	if (lua_gettop(ls) != 1)
	{
		lua_pushnil(ls);
		retval = sc_InvalidArg;
	}
	else
	{
		unsigned int uin = lua_tonumber(ls, 1);

		CResponseLoginPara* pPara = (CResponseLoginPara*)GlobalInterface::m_pNetwork->GetGameUserData();
		if (pPara)
		{
			int max_level = MAX_LEVEL;
			unsigned int experience = 0;
			// 自己
			if (pPara->m_unUin == uin)
			{
				experience = pPara->m_stPlayerDBBaseInfo.m_unExperience;
			}
			else
			{
				// TODO: Add Other Player Mathod Here!!
			}

			int level = max_level;
			for (int i = 0; i < max_level-1; ++i)
			{
				if (experience >= levels[i] && experience < levels[i+1])
				{
					level = i + 1;
					break;
				}

			}

			lua_pushnumber(ls, level);
			retval = sc_Successed;
		}
		else
		{
			lua_pushnil(ls);	
			retval = sc_Unexpected;
		}

	}

	lua_pushnumber(ls, retval);
	return 2;
}*/

//args	: seatid
//retval: pinfo, sc_err
int SC_GetRoomPlayerInfo(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnil(ls);	//pinfo
		retval = sc_InvalidArg;
	}
	else
	{
		int seatid = lua_tonumber(ls, 1);
		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		bool found = false;
		for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
		{
			STPlayerRoomVisibleInfo& refPInfo = refRoomDesc.m_stPlayerInfo[i];
			if ( refPInfo.m_unUin != 0 && refPInfo.m_bSeatID == seatid )
			{
				found = true;
				lua_newtable(ls);
				DEFINE_MSGTOLUA(ls, &refPInfo);
				break;
			}
		}
		if ( !found )
		{
			lua_pushnil(ls);
			retval = sc_TargetNotFound;
		}
	}
	lua_pushnumber(ls, retval);
	return 2;
}


//args	: uin
//retval: sc_err
int SC_FindPlayer(lua_State *ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		UINT uin = lua_tonumber(ls, 1);
		assert(GlobalInterface::m_pNetwork);
		QQUSERDATA* pUser = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		CRequestFindPlayerByQQPara kPara;
		kPara.m_unUin = pUser->Uin;
		kPara.m_unTime = GetTickCount();
		kPara.m_unDstUin = uin;
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTFINDPLAYERBYQQ, (PBYTE)&kPara, 0, FE_GAMESVRD, 0);
		assert(hr == S_OK);
		retval = sc_Successed;
	}
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: uin
//retval: seatid, sc_err
int SC_UinToSeat(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnil(ls);
		retval = sc_InvalidArg;
	}
	else
	{
		UINT Uin = lua_tonumber(ls, 1);

		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		bool found = false;
		for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
		{
			STPlayerRoomVisibleInfo& refPInfo = refRoomDesc.m_stPlayerInfo[i];
			if ( refPInfo.m_unUin == Uin )
			{
				lua_pushnumber(ls, refPInfo.m_bSeatID);
				found = true;
			}
		}
		if ( !found )
		{
			lua_pushnil(ls);
			retval = sc_TargetNotFound;
		}
	}
	lua_pushnumber(ls, retval);
	return 2;
}

//args	: pid
//retval: seatid, sc_err
int SC_PIDToSeat(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnil(ls);
		retval = sc_InvalidArg;
	}
	else
	{
		UINT pid = lua_tonumber(ls, 1);

		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		bool found = false;
		for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
		{
			STPlayerRoomVisibleInfo& refPInfo = refRoomDesc.m_stPlayerInfo[i];
			if ( refPInfo.m_unUin != 0 && refPInfo.m_nPlayerID == pid )
			{
				lua_pushnumber(ls, refPInfo.m_bSeatID);
				found = true;
			}
		}
		if ( !found )
		{
			lua_pushnil(ls);
			retval = sc_TargetNotFound;
		}
	}
	lua_pushnumber(ls, retval);
	return 2;
}

//args	: seatid
//retval: index, sc_err
int SC_SeatToIndex(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnil(ls);	//index
		retval = sc_InvalidArg;
	}
	else
	{
		int seatid = lua_tonumber(ls, 1);
		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		bool found = false;
		if ( refRoomDesc.ValidFlag == TRUE )
		{
			int index = -1;
			assert(seatid != refRoomDesc.SeatID);
			if ( seatid < refRoomDesc.SeatID )
				index = seatid;
			else if ( seatid < MAXSEATNUMINROOM )
				index = seatid - 1;
			if ( index >= 0 )
			{
				found = true;
				lua_pushnumber(ls, index);
				retval = sc_Successed;
			}
		}
		if ( !found )
		{
			lua_pushnil(ls);
			retval = sc_TargetNotFound;
		}
	}
	lua_pushnumber(ls, retval);
	return 2;
}

int IndexToSeat(int index)
{
	CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
	assert(pCache);
	RoomDesc& refRoomDesc = pCache->m_RoomDesc;

	bool found = false;
	if ( refRoomDesc.ValidFlag == TRUE )
	{
		int nSeat = -1;
		if ( index < refRoomDesc.SeatID )
			nSeat = index;
		else if ( index < MAXSEATNUMINROOM )
			nSeat = index + 1;
		if ( nSeat >= 0 )
		{
			return nSeat;
		}
	}
	return -1;
}
//args	: index
//retval: seatid, sc_err
int SC_IndexToSeat(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnil(ls);	//seatid
		retval = sc_InvalidArg;
	}
	else
	{
		int index = lua_tonumber(ls, 1);
		int seatid = IndexToSeat(index);
		if ( seatid >= 0 )
		{
			lua_pushnumber(ls, seatid);
			retval = sc_Successed;
		}
		else
		{
			lua_pushnil(ls); //seatid
			retval = sc_TargetNotFound;
		}
	}
	lua_pushnumber(ls, retval);
	return 2;
}

//args	: gamemodebase,gamemodesub
//retval: sc_err
int SC_QuickEnterRoom(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 2 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		int gamemodebase = lua_tonumber(ls, 1);
		int gamemodesub = lua_tonumber(ls, 2);
		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		if ( pPara )
		{
			CRequestQuickEnterRoomPara aPara;
			memset(&aPara, 0, sizeof(aPara));
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_sGameModeBase = gamemodebase;
			aPara.m_sGameModeSub = gamemodesub;
			HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTQUICKENTERROOM, (PBYTE)&aPara, 0, FE_GAMESVRD, 0);
			KK_CHECK(hr);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retval: sc_err
int SC_LeaveRoom(lua_State* ls)
{
	int retval = sc_Unexpected;
	CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
	assert(pCache);
	RoomDesc& refRoomDesc = pCache->m_RoomDesc;

	QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	if ( pPara )
	{
		CRequestLeaveRoomPara aPara;
		aPara.m_unUin = pPara->Uin;
		aPara.m_unTime = GetTickCount();
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTLEAVEROOM, (PBYTE)&aPara, 0, FE_ROOM, refRoomDesc.RoomID);
		KK_CHECK(hr);

		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}
int SC_GetTickCount(lua_State * ls)
{
	unsigned int unTickCount = GetTickCount();
	lua_pushnumber(ls, unTickCount);
	return 1;
}
//args	: roomid,[pwd],[svrid]
//retval: sc_err
int SC_EnterRoom(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		int roomid = lua_tonumber(ls, 1);
		LPCSTR pwd = NULL;
		if ( lua_gettop(ls) > 1 )
		{
			pwd = lua_tostring(ls, 2);
		}
		int svrid = 0;
		if ( lua_gettop(ls) > 2 )
		{
			svrid = lua_tonumber(ls, 3);
		}

		GameResponseInfo* pGameInfo = GlobalInterface::m_pNetwork->GetGameInfo();
		SOCKET_CREATE_INFO* pSocketInfo = GlobalInterface::m_pNetwork->GetSocketInfo(eGameServer);
		if ( svrid != 0 && pGameInfo && pSocketInfo && pSocketInfo->uServerID != svrid )//需要切换服务器的情况
		{
			DWORD dwCountry = pGameInfo->m_stPlayerDBBaseInfo.m_bCountry;
			GlobalInterface::m_pNetwork->SetLoginType(ELT_Multi);
			GlobalInterface::m_pNetwork->LoginGameServer(dwCountry, 0, svrid);
			GlobalInterface::m_pDataCache->AddEnterRoomTask(svrid, roomid);
			retval =sc_Successed;
		}
		else
		{
			QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
			if ( pPara )
			{
				CRequestEnterRoomPara aPara;
				memset(&aPara, 0, sizeof(aPara));
				aPara.m_unUin = pPara->Uin;
				aPara.m_unTime = GetTickCount();
				aPara.m_nRoomID = roomid;
				if ( pwd )
					strncpy(aPara.m_szPassword, pwd, MAXROOMPASSWORDLEN);
				HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTENTERROOM, (PBYTE)&aPara, 0, FE_ROOM, roomid);
				KK_CHECK(hr);

				retval = sc_Successed;
			}
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retval: sc_er
int SC_ToggleReady(lua_State* ls)
{
	int retval = sc_Unexpected;
	CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
	assert(pCache);
	RoomDesc& refRoomDesc = pCache->m_RoomDesc;

	QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	if ( pPara )
	{
		UCHAR nNewReady = State_Ready;
		if ( refRoomDesc.Ready == State_Readied )
			nNewReady = State_UnReady;
		CRequestChangeReadyStatePara aPara;
		aPara.m_unUin = pPara->Uin;
		aPara.m_unTime = GetTickCount();
		aPara.m_bReadyState = nNewReady;
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCHANGEREADYSTATE, (PBYTE)&aPara, 0, FE_ROOM, refRoomDesc.RoomID);
		KK_CHECK(hr);

		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: seatid
//retval: sc_err
int SC_ToggleSeatState(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		int seatid = lua_tonumber(ls, 1);
		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		if ( pPara )
		{
			CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
			assert(pCache);
			RoomDesc& refRoomDesc = pCache->m_RoomDesc;

			UCHAR nNewState = Seat_Close;
			if ( refRoomDesc.SeatStatus[seatid] == Seat_Closed )
				nNewState = Seat_Open;
			CRequestChangeSeatStatePara aPara;
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_nRoomID = refRoomDesc.RoomID;
			aPara.m_bSeatID = seatid;
			aPara.m_bSeatState = nNewState;	//0表示关闭，1表示打开
			HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTCHANGESEATSTATE, (PBYTE)&aPara, 0, FE_ROOM, refRoomDesc.RoomID);
			KK_CHECK(hr);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: uin
//retval: sc_err
int SC_Kick(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		UINT Uin = lua_tonumber(ls, 1);
		if ( Uin == 0 )
			goto KK_EXIT;
		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;

		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		if ( pPara )
		{
			CRequestKickFromRoomPara aPara;
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_unDstUin = Uin;
			HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTKICKFROMROOM, (PBYTE)&aPara, 0, FE_ROOM, refRoomDesc.RoomID);
			KK_CHECK(hr);
			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//游戏逻辑相关函数
//args	: (none)
//retval: sc_err
int SC_StartGame(lua_State* ls)
{
	int retval = sc_Unexpected;
	QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
	if ( pPara )
	{
		CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
		assert(pCache);
		RoomDesc& refRoomDesc = pCache->m_RoomDesc;
		CRequestStartGamePara aPara;
		aPara.m_unUin = pPara->Uin;
		aPara.m_unTime = GetTickCount();
		HRESULT hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTSTARTGAME, (PBYTE)&aPara, 0, FE_ROOM, refRoomDesc.RoomID);
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}




//聊天相关函数
//args	: (none)
//retval : num, desc
int SC_GetChatActionsDesc(lua_State *ls)
{
	StrKeyToActionMap& ActionMap = GlobalInterface::m_pUIMiscMgr->m_ChatActionMapLocal;
	lua_pushnumber(ls, ActionMap.size());
	lua_newtable(ls);
	int index = 1;
	for ( StrKeyToActionMap::const_iterator it = ActionMap.begin(); it != ActionMap.end(); it++ )
	{
		BeginTableElement(ls, index++);
		SetTableElement(ls, "name", it->first.c_str());
		EndTableElement(ls);
	}
	return 2;
}

//args	: Uin,Type,Content,UseP2P
//retval: sc_err
int SC_Talk(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 3 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		BOOL usep2p = FALSE;
		UINT Uin = lua_tonumber(ls, 1);
		UCHAR type = lua_tonumber(ls, 2);
		LPCSTR content = lua_tostring(ls, 3);
		if ( lua_gettop(ls) > 3 )
			usep2p = lua_tonumber(ls, 4);

		QQUSERDATA* pPara = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		if ( pPara )
		{
			CRequestTalkPara aPara;
			aPara.m_unUin = pPara->Uin;
			aPara.m_unTime = GetTickCount();
			aPara.m_bType = type;
			aPara.m_unDestPlayerUin = Uin;
			aPara.m_nContentLength = __min(MAXCHATCONTENTLEN, strlen(content));
			strncpy(aPara.m_szContent, content, MAXCHATCONTENTLEN);

			HRESULT hr = E_FAIL;
			if ( !usep2p )
			{
				hr = GlobalInterface::m_pNetwork->RequestGameServer(ID_CMS_REQUESTTALK, (PBYTE)&aPara, 0, FE_PLAYER, 0);
			}
			else
			{
				BYTE byBuffer[NORMAL_DATA_LEN];
				INT nSize = 0;
				*(INT*)byBuffer = ID_CMS_REQUESTTALK;
				aPara.Encode((void*)(byBuffer + sizeof(INT)), &nSize);
				hr = GlobalInterface::m_pQQTppp->BroadCast(nSize+sizeof(INT), byBuffer, 0);
			}	
			KK_CHECK(hr);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: content
//retval: sc_err
int SC_TalkFilter(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		const char* str = lua_tostring(ls, 1);
		unsigned int uin = -1;
		if ( lua_gettop(ls) > 1 )
			uin = lua_tonumber(ls, 2);

		if ( GlobalInterface::m_pUIMiscMgr->DoChatAction(uin, str) )
		{
			retval = sc_Successed;
		}
//		else
//		{
//			NxPlayMgr::GetHostPlay()->SetCharaterAnimation(1216);
//			retval = sc_Successed;
//		}
	}
	lua_pushnumber(ls, retval);
	return 1;
}

//UI界面相关函数
//args	: rootpath
//retvalue	: sc_err
int SC_LoadUI(lua_State* ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pGUI);
	HRESULT hr = GlobalInterface::m_pGUI->LoadUI("res/uires/uires.lua");
	KK_CHECK(hr);
	retval = sc_Successed;
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: idpath
//retval : sc_err
int SC_GotoUIScreen( lua_State *ls )
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		const char *pname = lua_tostring(ls, 1);
		if ( !pname || strlen(pname) == 0 )
			retval = sc_InvalidArg;
		else
		{
			assert(GlobalInterface::m_pGUI);
			HRESULT hr = GlobalInterface::m_pGUI->EnterUILayout(pname);
			KK_CHECK(hr);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args : none
// retval: curuiscreen
int SC_GetCurUIScreen(lua_State * ls)
{
	int retval = sc_Unexpected;

	const char * pname = GlobalInterface::m_pGUI->GetCurLayout();
	lua_pushstring(ls, pname);

	return 1;
}

int SC_GetLastUIScreen(lua_State * ls)
{
	const char * pname = GlobalInterface::m_pGUI->GetLastLayout();

	if (pname)
	{
		lua_pushstring(ls, pname);
		return 1;
	}

	return 0;
}

//args	: layoutname
//retval: sc_err
int SC_EnterUILayout(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		KK_CHECK_POINTER(GlobalInterface::m_pGUI);
		const char* name = lua_tostring(ls, 1);
		HRESULT hr = GlobalInterface::m_pGUI->EnterUILayout(name);
		KK_CHECK(hr);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retval : attrb, sc_err
int SC_GetDisplayAttr(lua_State *ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pApp);
	HWND hWnd;
	GlobalInterface::m_pApp->GetMainWnd(&hWnd);
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
	int w = rcClient.right - rcClient.left;
	int h = rcClient.bottom - rcClient.top;
	BOOL bWindow = (GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE);

	lua_newtable(ls);
	SetTableElement(ls, "windowed", bWindow);	
	SetTableElement(ls, "width", w);	
	SetTableElement(ls, "height", h);	
	retval = sc_Successed;

	lua_pushnumber(ls, retval);
	return 2;
}

//args	: id, txt, x, y, life, txtsize, space, color, width, height, margin
//retval: sc_err
int SC_AddBubble(lua_State* ls)
{
	int retval = sc_Unexpected;
	if (lua_gettop(ls) < 4 )
		retval = sc_InvalidArg;
	else
	{
		UINT id = lua_tonumber(ls, 1);
		LPCSTR txt=lua_tostring(ls, 2);
		INT x = lua_tonumber(ls, 3);
		INT y = lua_tonumber(ls, 4);
		INT life = -1;
		INT txtsize=12;
		INT space = 0;
		UINT color = 0;
		INT width = -1, height = -1, margin = 10;
		if ( lua_gettop(ls) > 4 )
			life = lua_tonumber(ls, 5);
		if ( lua_gettop(ls) > 5 )
			txtsize = lua_tonumber(ls, 6);
		if ( lua_gettop(ls) > 6 )
			space = lua_tonumber(ls, 7);
		if ( lua_gettop(ls) > 7 )
			color = lua_tonumber(ls, 8);
		if ( lua_gettop(ls) > 8 )
			width = lua_tonumber(ls, 9);
		if ( lua_gettop(ls) > 9 )
			height = lua_tonumber(ls, 10);
		if ( lua_gettop(ls) > 10 )
			margin = lua_tonumber(ls, 11);
		IBubbleMgr* pBubbleMgr = GlobalInterface::m_pGUI->GetBubbleMgr();
		if ( pBubbleMgr )
		{
			BOOL bAdd = pBubbleMgr->AddBubble(id, txt, Point2D(x, y), life, txtsize, space, color, width, height, margin);
			KK_CHECK_BOOL(bAdd);

			retval = sc_Successed;
		}
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retval: sc_err
int SC_ClearBubble(lua_State* ls)
{
	int retval = sc_Unexpected;
	IBubbleMgr* pBubbleMgr = GlobalInterface::m_pGUI->GetBubbleMgr();
	if ( pBubbleMgr )
	{
		pBubbleMgr->Clear();
		retval = sc_Successed;
	}
	lua_pushnumber(ls, retval);
	return 1;
}

//args	: (none)
//retval:sc_err
int SC_ExitApp(lua_State* ls)
{
	int retval = sc_Unexpected;
	assert(GlobalInterface::m_pApp);
	HWND hWnd = NULL;
	HRESULT hr = GlobalInterface::m_pApp->GetMainWnd(&hWnd);
	KK_CHECK(hr);
	PostMessage(hWnd, WM_CLOSE, 0, 0);
	retval = sc_Successed;
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//script aux
int SC_DoString(lua_State *ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		KK_CHECK_POINTER(GlobalInterface::m_pScriptState);
		const char* str = lua_tostring(ls, 1);
		retval = GlobalInterface::m_pScriptState->DoString(str);
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

int SC_LogText(lua_State *ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		const char* str = lua_tostring(ls, 1);
		LogText(str);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

int SC_Test(lua_State* ls)
{
	CNotifyRaceOverPara aPara;
	memset(&aPara, 0, sizeof(aPara));
	aPara.m_bCurrentPlayerNum = 1;
	aPara.m_astRaceScore[0].m_unUin = 48071210;
	aPara.m_astRaceScore[0].m_unFinTime = -1;
	aPara.m_astRaceScore[0].m_nExpInc = 100;
	aPara.m_astRaceScore[0].m_nMoneyInc = 10000;
	MsgToLua(ls, &aPara);
	TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnNotifyRaceOver");
	return 0;
}

//sound
int SC_PlaySound(lua_State *ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		int id = lua_tonumber(ls, 1);
		//SoundManager::SoundType st = (SoundManager::SoundType)id;
		//NxSceneMgr::Get()->GetSoundManager()->PlaySound(st);
		CSoundMgr::Get()->PlaySound(id);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

int SC_StopSound(lua_State *ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
		retval = sc_InvalidArg;
	else
	{
		int id = lua_tonumber(ls, 1);
		//SoundManager::SoundType st = (SoundManager::SoundType)id;
		//NxSceneMgr::Get()->GetSoundManager()->StopSound(st);
		CSoundMgr::Get()->StopSound(id);
		retval = sc_Successed;
	}
KK_EXIT:
	lua_pushnumber(ls, retval);
	return 1;
}

//args: a, b
//ret: a & b
int SC_BitAnd(lua_State *ls)
{
	if ( lua_gettop(ls) < 2 )
	{
		assert(0);
		return 0;
	}
	int a = lua_tonumber(ls, 1);
	int b = lua_tonumber(ls, 2);
	lua_pushnumber(ls, a & b);
	return 1;
}

//args: a, b
//ret: a | b
int SC_BitOr(lua_State *ls)
{
	if ( lua_gettop(ls) < 2 )
	{
		assert(0);
		return 0;
	}
	int a = lua_tonumber(ls, 1);
	int b = lua_tonumber(ls, 2);
	lua_pushnumber(ls, a | b);
	return 1;
}

int SC_AllocNif(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnumber(ls, 0);
	}
	else
	{
		const char* pstrNifFileName = lua_tostring(ls, 1);
		DWORD dwHandle = CNifAlloc::Get()->Alloc(pstrNifFileName);
		lua_pushnumber(ls, dwHandle);
	}
	return 1;
}

int SC_FreeNif(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
	}
	else
	{
		DWORD dwHandle = lua_tonumber(ls, 1);
		CNifAlloc::Get()->Free(dwHandle);
	}
	return 0;
}

int SC_NifAddRef(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnumber(ls, 0);
	}
	else
	{
		const char* pstrNifFileName = lua_tostring(ls, 1);
		NiNode* pkNode = CNifLoader::Get()->Load(pstrNifFileName);
		pkNode->IncRefCount();
		lua_pushnumber(ls, DWORD(pkNode));
	}
	return 1;
}

int SC_NifRelease(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
	}
	else
	{
		DWORD dwHandle = lua_tonumber(ls, 1);
		NiNode* pkNode = (NiNode*)dwHandle;
		if ( pkNode )
			pkNode->DecRefCount();
	}
	return 0;
}

int SC_TexAddRef(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnumber(ls, 0);
	}
	else
	{
		const char* pstrTexFileName = lua_tostring(ls, 1);
		NiNode* pkNode = CNifLoader::Get()->LoadTexture(pstrTexFileName);
		pkNode->IncRefCount();
		lua_pushnumber(ls, DWORD(pkNode));
	}
	return 1;

}

int SC_TexRelease(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
	}
	else
	{
		DWORD dwHandle = lua_tonumber(ls, 1);
		NiNode* pkNode = (NiNode*)dwHandle;
		if ( pkNode )
			pkNode->DecRefCount();
	}
	return 0;
}

int SC_BindSelfKartToScene(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		assert(0);		
	}
	else
	{
		const char * pcBindName = lua_tostring(ls, 1);

		Play * pSelf = NxPlayMgr::GetHostPlay();
		
		if (pSelf && pcBindName && CKartScene::Get())
		{
			//pSelf->GetKartNIFRoot();
			CKartScene::Get()->AddKartToPoint(pcBindName, pSelf);
		}
	}

	return 0;
}

int SC_BindSelfRoleToScene(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		assert(0);		
	}
	else
	{
		const char * pcBindName = lua_tostring(ls, 1);

		Play * pSelf = NxPlayMgr::GetHostPlay();
		
		if (pSelf && pcBindName && CKartScene::Get())
		{
			//pSelf->GetCharacterNIFRoot();
			CKartScene::Get()->AddRoleToPoint(pcBindName, pSelf);
		}
	}

	return 0;
}

int SC_SetSceneCamera(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		assert(0);
		return 0;
	}
	const char * pcCameraName = lua_tostring(ls, 1);
	NiCamera * pkCamera = CKartScene::Get()->GetSceneCamera(pcCameraName);

	if(pkCamera)
	{
		NiAVObject * pkObj = CKartScene::Get()->GetScene();
		NiTimeController::StartAnimations(pkCamera->GetParent(), GetMainApp()->GetAccumTime());
		pkCamera->GetParent()->UpdateNodeBound();
		pkCamera->GetParent()->UpdateProperties();
		pkCamera->GetParent()->UpdateControllers(GetMainApp()->GetAccumTime());
		pkCamera->GetParent()->Update(GetMainApp()->GetAccumTime());

		CKartScene::Get()->SetCamera(pkCamera);
	}

	return 0;
}

int SC_RemoveKartFromUIScene(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		assert(0);
		return 0;
	}
	const char * pcCameraName = lua_tostring(ls, 1);

	Play * pSelf = NxPlayMgr::GetHostPlay();
	if (pSelf)
	{
		CKartScene::Get()->RemoveKartFromPoint(pcCameraName, pSelf);
	}
	return 0;
}

int SC_RemoveRoleFromUIScene(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		assert(0);
		return 0;
	}
	const char * pcCameraName = lua_tostring(ls, 1);

	Play * pSelf = NxPlayMgr::GetHostPlay();
	if (pSelf)
	{
		CKartScene::Get()->RemoveRoleFromPoint(pcCameraName, pSelf);
	}
	return 0;
}

int SC_ReBindSelfPlayer(lua_State * ls)
{
	Play * pSelf = NxPlayMgr::GetHostPlay();
	if (pSelf)
	{
		pSelf->ReBindAll();
	}

	return 0;
}



int SC_SelfPlayAnimation(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		assert(0);
		return 0;
	}

	int elemid = lua_tonumber(ls, 1);
	Play * pSelf = NxPlayMgr::GetHostPlay();
	if (pSelf)
	{
		pSelf->PlayAnimation(elemid, true);
	}
	return 0;
}

int SC_GetShopCatItemNum(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		lua_pushnumber(ls, 0);	//totnum
	}
	else
	{
		UINT nTypeId = lua_tonumber(ls, 1);
		ShopTypeObj* pObj=CShopMgr::Get()->GetShopTypeObj(nTypeId);
		lua_pushnumber(ls,(int)pObj->maPtList.size());
	}
	return 1;
}

int SC_GetShopCatItemId(lua_State * ls)
{
	bool bRes=false;
	int retval = sc_Unexpected;
	if (lua_gettop(ls) >= 2)
	{
		UINT nTypeId = lua_tonumber(ls, 1);
		UINT nNumId = lua_tonumber(ls,2);
		ShopTypeObj* pObj=CShopMgr::Get()->GetShopTypeObj(nTypeId);
		UINT nCnt=0;
		SHOPPTLIST_MAP::iterator it=pObj->maPtList.begin();
		if (!pObj->maPtList.empty())
		{//枚举返回指定商品id
			for (nCnt=0;nCnt<nNumId;++nCnt)
			{
				++it;
				if(it==pObj->maPtList.end())
				{
					break;
				}
			}
			//
			if(it!=pObj->maPtList.end())
			{
				ShopItem* pItem=it->second;
				lua_newtable(ls);
				DEFINE_MSGTOLUA(ls,&pItem->insCmy);
				retval = sc_Successed;
				bRes=true;
			}
		}
	}
	//
	if (!bRes)
	{
		lua_pushnil(ls);
	}
	lua_pushnumber(ls,retval);
	return 2;
}

int SC_ShopSelfShowTurn(lua_State * ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 2 )
		retval = sc_InvalidArg;

	else
	{
		float fDegree = lua_tonumber(ls, 1);
		UINT  nItemId=lua_tonumber(ls, 2);
		NiMatrix3 kZDeltaRotate;
		kZDeltaRotate.MakeZRotation(NxPi * fDegree / 180.f);
		//NiPoint3 kPt;


		Play * pSelf = NxPlayMgr::GetHostPlay();

		ShopItem* pShopItem= CShopMgr::Get()->GetShopItemById(nItemId);
		if (pSelf && pShopItem && pShopItem->GetSelfShowNode())
		{
			//ShopItem* pShopItem= CShopMgr::Get()->GetShopItemById(7);
			//NiAVObject * pkObj = pSelf->GetKartNIFRoot()->GetObjectByName("MODE");
			NiAVObject* pkObj=pShopItem->GetSelfShowNode()->GetObjectByName(pShopItem->strMovNodeName.c_str());
			if (pkObj)
			{
				pkObj->SetRotate(/*pkObj->GetRotate() * */ kZDeltaRotate);
				pShopItem->GetSelfShowNode()->Update(0.0f);
			}
			else
			{
				_ASSERTE(0);
			}
			retval = sc_Successed;
		}
		else
		{
			retval = sc_Unexpected;
		}
		/*
		ShopItem* pShopItem= CShopMgr::Get()->GetShopItemById(nItemId);
		if(pShopItem && pShopItem->GetSelfShowNode() && pShopItem->GetSelfShowNode()->GetObjectByName("Avatar"))
		{
		NiAVObject * pkObj =NiDynamicCast(NiNode,pShopItem->GetSelfShowNode()->GetObjectByName("Avatar"));
		//NiAVObject * pkObj = pShopItem->GetSelfShowNode();
		pkObj->SetRotate(pkObj->GetRotate() * kZDeltaRotate);
		//kPt=pkObj->GetTranslate();
		//pkObj->SetTranslate(kPt.x+(fDegree/100.0f),kPt.y,kPt.z);
		retval = sc_Successed;
		}
		else
		{
		retval = sc_Unexpected;
		}
		*/
	}

	lua_pushnumber(ls, retval);

	return 1;
}

// 改变自己的车、玩家、头发 
int SC_SelfChangeSelf(lua_State * ls)
{
	if (lua_gettop(ls) <= 0) 
	{
		return 0;
	}
	else
	{
		Play * pPlay = NxPlayMgr::GetHostPlay();
		unsigned int unID = lua_tonumber(ls, 1);
		if (pPlay)
		{
			pPlay->ChangeItem(unID);
			lua_pushnumber(ls, 1);
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

// 改变dummy点的朝向
int SC_RotateUISceneDummy(lua_State * ls)
{
	if (lua_gettop(ls) <= 1)
	{
		return 0;
	}
	else
	{
		const char * strDummyName = lua_tostring(ls, 1);
		float	     fDegree	  = lua_tonumber(ls, 2);
		NiNode * pkScene = CKartScene::Get()->GetScene();
		NiAVObject * pkDummy = NULL;
		if (pkScene)
		{
			pkDummy = pkScene->GetObjectByName(strDummyName);
		}

		if (pkDummy)
		{
			NiMatrix3 kRotateZ;
			kRotateZ.MakeZRotation(NxMath::degToRad(fDegree));

			pkDummy->SetRotate(pkDummy->GetRotate() * kRotateZ);
			lua_pushboolean(ls, 1);
			return 1;
		}
		return 0;
	}
}

int SC_ShopBuyItem(lua_State* ls)

{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 3 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		UINT	nItemId=lua_tonumber(ls, 1);
		UINT	nPayType=lua_tonumber(ls,2);
		UINT	nBuyNum=lua_tonumber(ls,3);	///购买数量
		_ASSERTE(1==nBuyNum);
		ShopItem* pShItem=CShopMgr::Get()->GetShopItemById(nItemId);
		if (pShItem)
		{
			CRequestBuyCommodityPara pGetCommList;
			memset(&pGetCommList,0,sizeof(CRequestBuyCommodityPara));
			pGetCommList.m_unTime=::GetTickCount();
			pGetCommList.m_iReqBuySeq=pGetCommList.m_unTime;
			pGetCommList.m_unUin=NxPlayMgr::GetHostPlay()->GetPlayInfo().m_unUin;
			pGetCommList.m_unDstUin=pGetCommList.m_unUin;
			pGetCommList.m_iPayType=EPT_PAY_KB;	//todo:暂时是酷币购买
			pGetCommList.m_iCommodityID=pShItem->insCmy.m_iID;
			CAvatarNet::RequestBuyCommodity(&pGetCommList);
			retval = sc_Successed;
		}
		else

		{
			retval = sc_Unexpected;
		}
	}

	lua_pushnumber(ls, retval);

	return 1;
}

int SC_GetShopBuyState(lua_State* ls)
{
	if ( lua_gettop(ls) < 1 )
	{
		lua_pushnumber(ls,0);
		lua_pushstring(ls,"Error");
	}
	else
	{
		UINT	nItemId=lua_tonumber(ls, 1);
		ShopItem* pShItem=CShopMgr::Get()->GetShopItemById(nItemId);
		if (pShItem)
		{
			BuyState*	pState=CShopMgr::Get()->GetBuyJobState(nItemId);
			lua_pushnumber(ls,pState->nState);
			lua_pushstring(ls,pState->strDoc.c_str());
		}
		else
		{
			lua_pushnumber(ls,0);
			lua_pushstring(ls,"Error");
		}

	}

	return 2;
}

int SC_EquipShopItem(lua_State* ls)
{
	int retval = sc_Unexpected;
	if ( lua_gettop(ls) < 1 )
	{
		retval = sc_InvalidArg;
	}
	else
	{
		UINT	nItemId=lua_tonumber(ls, 1);
		UINT nCnt=0;
		ShopItem* pShItem=CShopMgr::Get()->GetShopItemById(nItemId);
		if (pShItem)
		{
			UINT dwUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
			CRequestChangeItemStatus insSta;
			ItemStore* pStore=NULL;
			insSta.m_sItemNum=pShItem->insCmy.m_shItemCount;
			insSta.m_unUin=dwUin;
			insSta.m_unTime=::GetTickCount();
			for (nCnt=0;nCnt<pShItem->insCmy.m_shItemCount;++nCnt)
			{
				insSta.m_stItemStatus[nCnt].m_iItemID=pShItem->insCmy.m_astItemInfo[nCnt].m_iItemID;
				insSta.m_stItemStatus[nCnt].m_bNewStatus=1;
				//如果目前没有这个avatar则要添加
				pStore=CAvatarMgr::Get()->GetItemDataMgr()->GetItemStore(dwUin,insSta.m_stItemStatus[nCnt].m_iItemID);
				if (!pStore)
				{
					STPlayerDBItemInfo insDbInfo;
					insDbInfo.m_bStatus=0;
					insDbInfo.m_iItemNum=1;
					insDbInfo.m_iItemID=insSta.m_stItemStatus[nCnt].m_iItemID;
					pStore=CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(dwUin,&insDbInfo,false);//不更新副本
				}
			}
			CAvatarMgr::Get()->ChangeItemStatus(&insSta,true);//直接装备
			retval = sc_Successed;
		}
		else
		{
			retval = sc_Unexpected;
		}

	}

	lua_pushnumber(ls, retval);
	return 1;
}

///备份原有装备状态
int SC_BackupShopEquip(lua_State* ls)
{
	UINT dwUin=NxPlayMgr::GetHostPlay()->GetPlayUin();
	CAvatarMgr::Get()->GetItemDataMgr()->BackupUserInfo(dwUin);
	return 0;
}

///恢复原有装备状态
int SC_RestoreShopEquip(lua_State* ls)
{
	if(CAvatarMgr::Get()->GetItemDataMgr()->CleanAllBind())
	{
		UINT dwUin=NxPlayMgr::GetHostPlay()->GetPlayUin();
		CAvatarMgr::Get()->GetItemDataMgr()->RestoreUserInfo(dwUin);
		CAvatarMgr::Get()->BindRes1st(dwUin);
	}
	return 0;
}
///恢复原有装备状态
int SC_CleanShopEquipInfo(lua_State* ls)
{
	UINT dwUin=NxPlayMgr::GetHostPlay()->GetPlayUin();
	if(CAvatarMgr::Get()->GetItemDataMgr()->CleanUserInfo(dwUin))
	{
	}
	return 0;
}
///立即装备某物品，传入参数为ItemID， uin
int SC_EquipItemImmediate(lua_State * ls)
{
	if ( lua_gettop(ls) < 2 )
	{
		assert(0);
		return 0;
	}
	unsigned int uin = lua_tonumber(ls, 1);
	unsigned int unId= lua_tonumber(ls, 2);
	CRequestChangeItemStatus insSta;
	insSta.m_sItemNum=1;
	insSta.m_unUin=uin;
	insSta.m_unTime=::GetTickCount();
	insSta.m_stItemStatus[0].m_bNewStatus=1;
	insSta.m_stItemStatus[0].m_iItemID=unId;
	ItemStore* pStore=CAvatarMgr::Get()->GetItemDataMgr()->GetItemStore(uin,unId);
	if (!pStore)
	{//没有则加一下这个头发
		STPlayerDBItemInfo insDbInfo;
		insDbInfo.m_bStatus=0;
		insDbInfo.m_iItemNum=1;
		insDbInfo.m_iItemID=unId;
		pStore=CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(uin,&insDbInfo,false);//不更新副本
	}
	CAvatarMgr::Get()->ChangeItemStatus(&insSta,true);//直接装备

	return 0;
}

// 显示/隐藏鼠标
int SC_ShowPointer(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		return 0;
	}

	bool bShow = lua_toboolean(ls, 1);

	if (bShow)
		GetMainApp()->ShowPointer();
	else
		GetMainApp()->HidePointer();

	lua_pushboolean(ls, 1);

	return 1;
}

// 未完成
int SC_SetDevice(lua_State * ls)
{
	if (lua_gettop(ls) < 3)
	{
		return 0;
	}

	int nWindowWidth = lua_tonumber(ls, 1);
	int nWindowHeight= lua_tonumber(ls, 2);
	bool bFullScreen = lua_toboolean(ls, 3);

	lua_pushboolean(ls, 1);

	return 1;
}

int SC_LeaveTask(lua_State * ls)
{
	CTaskMgr::Get()->LeaveTask();
	return 0;
}

int SC_RequestStartTaskSingleTask(lua_State* ls)
{
	if (lua_gettop(ls) < 2)
	{
		return 0;
	}
	DWORD dwUin = lua_tonumber(ls, 1);
	DWORD dwTaskId = lua_tonumber(ls, 2);
	DWORD dwMapId = CTaskMgr::Get()->FindTaskCfg(dwTaskId)->dwMapID;
	CTaskNet::RequestStartSingleTask(dwUin, dwTaskId, dwMapId);
	return 0;
}


int SC_IsFinishNewHandTask(lua_State* ls)
{
	if (lua_gettop(ls) < 1)
	{
		return 0;
	}
	DWORD dwID = lua_tonumber(ls, 1);
	bool bFinish = CTaskMgr::Get()->IsFinishNewHandTask(dwID);
	lua_pushboolean(ls, bFinish ? 1 : 0);
	return 1;
}

int SC_IsCurTaskFinish(lua_State* ls)
{
	if(CTaskMgr::Get()->GetCurTask()->GetState() == EDS_Success)
	{
		lua_pushboolean(ls, 1);		
	}
	else
	{
		lua_pushboolean(ls, 0);		
	}
	return 1;;
}


int SC_RequestSkipTask(lua_State* ls)
{
	if(lua_gettop(ls) < 2)
	{
		return 0;
	}
	DWORD dwUin = lua_tonumber(ls, 1);
	DWORD dwOpt = lua_tonumber(ls, 2);
	CTaskNet::RequestSkipTask(dwUin, dwOpt);
	return 0;
}

int SC_RequestReportCurrentOpt(lua_State* ls)
{
	if(lua_gettop(ls) < 2)
	{
		return 0;
	}
	DWORD dwUin = lua_tonumber(ls, 1);
	DWORD dwOpt = lua_tonumber(ls, 2);
	CTaskNet::RequestReportCurrentOpt(dwUin, dwOpt);
	return 0;
}

int SC_IsAward(lua_State* ls)
{
	if(CAwardMgr::Get()->IsAward())
	{
		lua_pushboolean(ls, 1);		
	}
	else
	{
		lua_pushboolean(ls, 0);		
	}
	return 1;;
}

int SC_FinishAward(lua_State* ls)
{
	CALL(CAwardMgr::Get(), FinishFirstAward());
	return 0;
}

int SC_PlaceRemotePlayerInRoom(lua_State * ls)
{
	unsigned int uin = lua_tonumber(ls, 1);

	Play * pPlay = NxPlayMgr::GetPlay(uin);
	NiAVObject * pkScene = CKartScene::Get()->GetScene();

	if (pPlay)
	{
		NiAVObject * pkKartDummy = pkScene->GetObjectByName("ROOT");
		NiAVObject * pkRoleDummy = pkScene->GetObjectByName("playermode");

		assert(pkKartDummy && pkRoleDummy);
		if (pkKartDummy && pkRoleDummy)
		{
			const NiTransform & kKartTransform = pkKartDummy->GetWorldTransform();
			const NiTransform & kRoleTransform = pkRoleDummy->GetWorldTransform();

			NiNode * pkKart = pPlay->GetKartNIFRoot();
			NiNode * pkRole = pPlay->GetCharacterNIFRoot();

			pPlay->ShowName(false);

			//	((NiNode*)(pkKart->GetObjectByName("playermode")))->DetachChild(pkRole);
			//if (pkRole->GetParent())
			//	pkRole->GetParent()->DetachChild(pkRole);
			NiPoint3 kOffset = NiPoint3(0, 0, 0.1);
			pkKart->SetTranslate(kKartTransform.m_Translate + kOffset);
			pkKart->SetRotate(kKartTransform.m_Rotate);
			pkKart->Update(0.0f);			
			pkRole->SetTranslate(kRoleTransform.m_Translate - kOffset);			
			NiMatrix3 kRoleRotate = kRoleTransform.m_Rotate;
			if (lua_gettop(ls) >= 2)
			{
				float fDelta = lua_tonumber(ls, 2);

				NiMatrix3 kZRotate;
				kZRotate.MakeZRotation(fDelta / 180.0f * NI_PI);
				kRoleRotate = kRoleRotate * kZRotate;
			}
			pkRole->SetRotate(kRoleRotate);
			pPlay->PlayAnimation(1104, true);
		}
	}

	return 0;
}

int SC_UnPlaceRemotePlayerInRoom(lua_State * ls)
{
	unsigned int uin = lua_tonumber(ls, 1);

	Play * pPlay = NxPlayMgr::GetPlay(uin);
	
	if (pPlay)
	{		
		NiNode * pkKart = pPlay->GetKartNIFRoot();
		NiNode * pkRole = pPlay->GetCharacterNIFRoot();
		pkKart->SetTranslate(NiPoint3(0, 0, 0));
		pkKart->SetRotate(NiMatrix3::IDENTITY);
		pkRole->SetTranslate(NiPoint3(0, 0, 0));		
		pkRole->SetRotate(NiMatrix3::IDENTITY);
		//pPlay->ReBindAll();
	}

	return 0;
}

#include "../PlayerRank.h"
int SC_GetGameRankInfo(lua_State * ls)
{
	if ( CPlayerRankMgr::Get() )
	{
		GAME_RANKINFO* ri = CPlayerRankMgr::Get()->GetGameRankInfo();
		if ( ri )
		{
			lua_newtable(ls);
			DEFINE_MSGTOLUA(ls, ri);
			return 1;
		}
	}
	return 0;
}

int SC_GetRaceTime(lua_State * ls)
{
	if ( CLevelManager::Get() )
	{
		float fLevelTime = CLevelManager::Get()->GetLevelTime();
		float fBestTime = CLevelManager::Get()->GetLinks()->GetBestLapTime();
		int fLevelTimeInt = fLevelTime * 100;
		int fBestTimeInt = fBestTime * 100;
		lua_pushnumber(ls, fLevelTimeInt);
		lua_pushnumber(ls, fBestTimeInt);
		return 2;
	}
	return 0;
}

int SC_GetLaps(lua_State * ls)
{
	if ( CLevelManager::Get() )
	{
		int nLaps = CLevelManager::Get()->GetLinks()->GetCurLaps();
		int nTotalLaps = CLevelManager::Get()->GetLinks()->GetTotalLaps();
		lua_pushnumber(ls, nLaps);
		lua_pushnumber(ls, nTotalLaps);
		return 2;
	}
	return 0;
}

int SC_GetSpeed(lua_State * ls)
{
	Play* pHostPlay = NxPlayMgr::GetHostPlay();
	if ( pHostPlay )
	{
		CKart* pKart = pHostPlay->GetKart();
		if ( pKart )
		{
			int nSpeed = pKart->GetVelocity() * GameSetting::SpeedDispCoef;
			lua_pushnumber(ls, nSpeed);
			return 1;
		}
	}
	lua_pushnumber(ls, 0);
	return 1;
}

int SC_PlayFlash(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		pkFlash->Play();
		lua_pushboolean(ls, 1);

	}

	return 1;
}

int SC_StopFlash(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}
		FlashRenderer * pkFlash = NULL;
		if (pkTexture)
		{
			FlashRenderer * pkFlash = pkTexture->GetFlashRender();
		}
		if (pkFlash)
			pkFlash->Stop();
		lua_pushboolean(ls, 1);

	}

	return 1;
}

int SC_FlashRewind(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		pkFlash->Rewind();
		lua_pushboolean(ls, 1);

	}
	
	return 1;
}

int SC_FlashForward(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		pkFlash->Forward();
		lua_pushboolean(ls, 1);

	}

	return 1;
}

int SC_FlashPause(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		pkFlash->Pause();
		lua_pushboolean(ls, 1);

	}

	return 1;
}

int SC_FlashGotoFrame(lua_State * ls)
{
	if (lua_gettop(ls) < 2)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);
		unsigned int unFrame = lua_tonumber(ls, 2);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		pkFlash->GotoFrame(unFrame);
		lua_pushboolean(ls, 1);

	}

	return 1;
}
int SC_FlashBack(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		pkFlash->Back();
		lua_pushboolean(ls, 1);

	}

	return 1;
}
int SC_FlashGetFrameNum(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		return 0;
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		unsigned int unFrameNum = pkFlash->GetFrameNum();
		lua_pushnumber(ls, unFrameNum);

	}

	return 1;
}
int SC_FlashIsPlaying(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		lua_pushboolean(ls, pkFlash->IsPlaying());

	}

	return 1;
}

int SC_SetLoopFlash(lua_State * ls)
{
	if (lua_gettop(ls) < 2)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		bool bLoop		 = lua_toboolean(ls, 2);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}

		FlashRenderer * pkFlash = pkTexture->GetFlashRender();

		pkFlash->SetLoop(bLoop);
		lua_pushboolean(ls, 1);
	}

	return 1;
}

int SC_StopPlayFlash(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenTexture * pkTexture = FlashScreenManager::Get()->GetFlashScreen(unID);
		if (!pkTexture)
		{
			lua_pushboolean(ls, 0);
			return 1;
		}
		FlashRenderer * pkFlash = NULL;
		if (pkTexture)
		{
			FlashRenderer * pkFlash = pkTexture->GetFlashRender();
		}
		if (pkFlash)
			pkFlash->StopPlay();
		lua_pushboolean(ls, 1);
	}

	return 1;
}

int SC_GetLoopFlash(lua_State * ls)
{
	return 0;
}

int SC_CreateFlash(lua_State * ls)
{
	return 0;
}

int SC_RemoveFlash(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		lua_pushboolean(ls, 0);
	else
	{
		unsigned int unID = lua_tonumber(ls, 1);

		FlashScreenManager::Get()->ReleaseFlashScreen(unID);
		
		lua_pushboolean(ls, 1);
	}

	return 1;
}

int SC_RemoveAllFlashes(lua_State *ls)
{
	FlashScreenManager * pkFlashMgr = FlashScreenManager::Get();
	if (pkFlashMgr)
		pkFlashMgr->ReleaseAllFlash();

	return 0;
}

int SC_GetCharSign(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		return 0;

	unsigned int id = lua_tonumber(ls, 1);

	char x[2]={0};
	x[0] = id + ' ';
	x[1] = 0;
	lua_pushstring(ls, x);

	return 1;
}

int SC_GetCountDownTime(lua_State* ls)
{
	if(CLevelManager::Get())
	{
		int nCDTime = CLevelManager::Get()->GetCountDownTime();
		lua_pushnumber(ls, nCDTime);
		return 1;
	}
	lua_pushnumber(ls, 0);
	return 1;
}

int SC_GetCurTaskID(lua_State* ls)
{
	if(CTaskMgr::Get())
	{
		if(CTaskMgr::Get()->GetCurTask())
		{
			PTASKCONFIG pTaskCfg =  CTaskMgr::Get()->GetCurTask()->GetTaskCfg();
			/*DWORD dwID = CTaskMgr::Get()->GetCurTask()->GetTaskCfg()->dwID;*/
			lua_pushnumber(ls, pTaskCfg->dwID);
			return 1;
		}
	}
	lua_pushnumber(ls, 0);
	return 1;
}

///播放mp3音乐，输入3个参数:
int SC_PlayMusic(lua_State * ls)
{
	if (lua_gettop(ls) < 3)
	{

	}
	else
	{
		CStringEx strTmp=lua_tostring(ls,1);
		UINT nPlayStatus=lua_tonumber(ls,2);
		int iLoopFlag=lua_tonumber(ls,3);
		if(1==iLoopFlag)
		{//转换一下
			iLoopFlag=-1;
		}

		if (nPlayStatus)
		{
			CSoundMgr::Get()->SetSource(CSoundMgr::ST_AMBIENT, strTmp.c_str(),0.4, iLoopFlag, nPlayStatus);
			CSoundMgr::Get()->PlaySound(CSoundMgr::ST_AMBIENT);
		} 
		else
		{
			CSoundMgr::Get()->StopSound(CSoundMgr::ST_AMBIENT);
		}
	}

	return 0;
}

int SC_WinExecute(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
	{
		return 0;
	}

	const char * pcstr = lua_tostring(ls, 1);

	::ShellExecute(NULL, "open", pcstr, NULL, NULL, SW_SHOW);
	return 0;
}


#include "../ProilingController.h"
int SC_Profilling(lua_State * ls)
{
	if (lua_gettop(ls) < 1)
		return 0;

	const char * command = lua_tostring(ls, 1);

	ProfilingController * pkCtrller = NxSceneMgr::Get()->GetProfilingController();

	if (pkCtrller && command)
	{
		pkCtrller->Profiling(command);
	}

	return 0;
}

int SC_CheckUpgrade(lua_State * ls)
{
	CResponseLoginPara* pPara = GlobalInterface::m_pNetwork->GetGameInfo();
	
	if (pPara && pPara->m_stPlayerDBAttributeInfo.m_unReserve > 0)
	{	
		GlobalInterface::m_pScriptState->DoString("Win_ShowWidget('UI.shortcut_dlg.upgrade_btn', true);");
	}
	
	return 0;
}

void RegisterScriptFuncs(CScriptState* pState)
{
	pState->RegisterFunc("GetDirInfo", &SC_GetDirInfo);
	pState->RegisterFunc("GetLastUserName", &SC_GetLastUserName);
	pState->RegisterFunc("GetRecentUsers", &SC_GetRecentUsers);
	pState->RegisterFunc("Login", &SC_Login);
	pState->RegisterFunc("CancelLogin", &SC_CancelLogin);
	pState->RegisterFunc("RegistUser", &SC_RegistUser);
	pState->RegisterFunc("LoginGame", &SC_LoginGame);
	pState->RegisterFunc("GetGameVar", &SC_GetGameVar);
	pState->RegisterFunc("SetGameVar", &SC_SetGameVar);
	pState->RegisterFunc("GetChatActionsDesc", &SC_GetChatActionsDesc);
	pState->RegisterFunc("GotoUIScreen", &SC_GotoUIScreen);
	pState->RegisterFunc("GetCurUIScreen", &SC_GetCurUIScreen);
	pState->RegisterFunc("GetLastUIScreen", &SC_GetLastUIScreen);
	pState->RegisterFunc("GetDisplayAttr", &SC_GetDisplayAttr);
	pState->RegisterFunc("ActiveExt", &SC_ActiveExt);
	pState->RegisterFunc("GetRoomList", &SC_GetRoomList);
	pState->RegisterFunc("CreateRoom", &SC_CreateRoom);
	pState->RegisterFunc("ChangeMap", &SC_ChangeMap);
	pState->RegisterFunc("ChangeTeam", &SC_ChangeTeam);
	pState->RegisterFunc("QuickEnterRoom", &SC_QuickEnterRoom);
	pState->RegisterFunc("GetMyRoomInfo", &SC_GetMyRoomInfo);
	pState->RegisterFunc("GetRoomPlayerInfo", &SC_GetRoomPlayerInfo);
	pState->RegisterFunc("FindPlayer", &SC_FindPlayer);
	pState->RegisterFunc("UinToSeat", &SC_UinToSeat);
	pState->RegisterFunc("SeatToIndex", &SC_SeatToIndex);
	pState->RegisterFunc("IndexToSeat", &SC_IndexToSeat);
	pState->RegisterFunc("PIDToSeat", &SC_PIDToSeat);
	pState->RegisterFunc("LeaveRoom", &SC_LeaveRoom);
	pState->RegisterFunc("EnterRoom", &SC_EnterRoom);
	pState->RegisterFunc("GetMyInfo", &SC_GetMyInfo);
	pState->RegisterFunc("ToggleSeatState", &SC_ToggleSeatState);
	pState->RegisterFunc("ToggleReady", &SC_ToggleReady);
	pState->RegisterFunc("StartGame", &SC_StartGame);
	pState->RegisterFunc("Talk", &SC_Talk);
	pState->RegisterFunc("TalkFilter", &SC_TalkFilter);
	pState->RegisterFunc("Kick", &SC_Kick);
	pState->RegisterFunc("AddBubble", &SC_AddBubble);
	pState->RegisterFunc("ClearBubble", &SC_ClearBubble);
	pState->RegisterFunc("EnterUILayout", &SC_EnterUILayout);
	pState->RegisterFunc("LoadUI", &SC_LoadUI);
	pState->RegisterFunc("Logout", &SC_Logout);
	pState->RegisterFunc("ExitApp", &SC_ExitApp);
	pState->RegisterFunc("DoString", &SC_DoString);
	pState->RegisterFunc("LogText", &SC_LogText);
	pState->RegisterFunc("PlaySound", &SC_PlaySound);
	pState->RegisterFunc("StopSound", &SC_StopSound);
	pState->RegisterFunc("PlayMusic", &SC_PlayMusic);
	pState->RegisterFunc("Test", &SC_Test);
	pState->RegisterFunc("GetAvatarAllList", &SC_GetAvatarAllList);
	pState->RegisterFunc("ChangeAvatarStatus", &SC_ChangeAvatarStatus);
	pState->RegisterFunc("ChangeAvatarStatus", &SC_ChangeAvatarStatus);
	pState->RegisterFunc("GetAvatarTypeCount", &SC_GetAvatarTypeCount);
	pState->RegisterFunc("GetAvatarInfoByType", &SC_GetAvatarInfoByType);
	
	pState->RegisterFunc("GetMyLevel", &SC_GetMyLevel);

	pState->RegisterFunc("BitAnd", &SC_BitAnd);
	pState->RegisterFunc("BitOr", &SC_BitOr);

	pState->RegisterFunc("CheckName", &SC_CheckName);
	pState->RegisterFunc("SelfShowTurn", &SC_SelfShowTurn);
	pState->RegisterFunc("RecoverSelfTurn", &SC_RecoverSelfTurn);
	//pState->RegisterFunc("GetPlayerLevel", &SC_GetPlayerLevel);
	pState->RegisterFunc("RequestAssignAttribute", &SC_RequestAssignAttribute);
	pState->RegisterFunc("RequestClearAttribute", &SC_RequestClearAttribute);
	pState->RegisterFunc("IsSelf", &SC_IsSelf);
	pState->RegisterFunc("GetMyAttribute", &SC_GetMyAttribute);
	pState->RegisterFunc("GetPlayerLevelByExp", &SC_GetPlayerLevelByExp);
	pState->RegisterFunc("SetKartAttrib", &SC_SetKartAttrib);
	
	// NifOperation
	pState->RegisterFunc("AllocNif", &SC_AllocNif);
	pState->RegisterFunc("FreeNif", &SC_FreeNif);
	pState->RegisterFunc("NifAddRef", &SC_NifAddRef);
	pState->RegisterFunc("NifRelease", &SC_NifRelease);
	pState->RegisterFunc("TexAddRef", &SC_TexAddRef);
	pState->RegisterFunc("TexRelease", &SC_TexRelease);
	pState->RegisterFunc("GetRaceTime", &SC_GetRaceTime);
	pState->RegisterFunc("GetLaps", &SC_GetLaps);
	pState->RegisterFunc("GetSpeed", &SC_GetSpeed);

	// UI Scene Operator
	pState->RegisterFunc("BindSelfKartToScene", &SC_BindSelfKartToScene);
	pState->RegisterFunc("BindSelfRoleToScene", &SC_BindSelfRoleToScene);
	pState->RegisterFunc("ReBindSelfPlayer", &SC_ReBindSelfPlayer);
	pState->RegisterFunc("SetSceneCamera", &SC_SetSceneCamera);
	pState->RegisterFunc("RemoveSelfKartFromUIScene", &SC_RemoveKartFromUIScene);
	pState->RegisterFunc("RemoveSelfRoleFromUIScene", &SC_RemoveRoleFromUIScene);
	pState->RegisterFunc("RotateUISceneDummy", &SC_RotateUISceneDummy);
	pState->RegisterFunc("SelfRoleShowTurn", &SC_SelfRoleShowTurn);
	pState->RegisterFunc("RecoverRoleTurn", &SC_RecoverRoleTurn);
	pState->RegisterFunc("SelfShowFixedTurn", &SC_SelfShowFixedTurn);

	// 很无聊的东西，一般不要用
	pState->RegisterFunc("GetCharSign", &SC_GetCharSign);
	pState->RegisterFunc("CheckUpgrade", &SC_CheckUpgrade);
	// UI Flash Operators
	pState->RegisterFunc("RemoveAllFlashes", &SC_RemoveAllFlashes);
	pState->RegisterFunc("CreateFlah", &SC_CreateFlash);
	pState->RegisterFunc("RemoveFlash", &SC_RemoveFlash);
	pState->RegisterFunc("PlayFlash", &SC_PlayFlash);
	pState->RegisterFunc("PauseFlash", &SC_FlashPause);
	pState->RegisterFunc("StopFlash", &SC_StopFlash);
	pState->RegisterFunc("RewindFlash", &SC_FlashRewind);
	pState->RegisterFunc("ForwardFlash", &SC_FlashForward);
	pState->RegisterFunc("FlashGotoFrame", &SC_FlashGotoFrame);
	pState->RegisterFunc("BackFlash", &SC_FlashBack);
	pState->RegisterFunc("GetFlashFrameNum", &SC_FlashGetFrameNum);
	pState->RegisterFunc("IsFlashPlaying", &SC_FlashIsPlaying);
	pState->RegisterFunc("SetFlashLoop", &SC_SetLoopFlash);
	pState->RegisterFunc("StopPlayFlash", &SC_StopPlayFlash);


	// Self Player Operator
	pState->RegisterFunc("SelfPlayAnimation", &SC_SelfPlayAnimation);
	pState->RegisterFunc("SelfChangeSelf", &SC_SelfChangeSelf);

	// for remote player in room
	pState->RegisterFunc("PlaceRemotePlayerInRoom", &SC_PlaceRemotePlayerInRoom);
	pState->RegisterFunc("UnPlaceRemotePlayerInRoom", &SC_UnPlaceRemotePlayerInRoom);

	//Shop use
	pState->RegisterFunc("GetShopCatItemNum", &SC_GetShopCatItemNum);
	pState->RegisterFunc("GetShopCatItemId", &SC_GetShopCatItemId);
	pState->RegisterFunc("ShopSelfShowTurn",&SC_ShopSelfShowTurn);
	pState->RegisterFunc("ShopBuyItem",&SC_ShopBuyItem);
	pState->RegisterFunc("GetShopBuyState",&SC_GetShopBuyState);
	pState->RegisterFunc("EquipShopItem",&SC_EquipShopItem);
	pState->RegisterFunc("BackupShopEquip",&SC_BackupShopEquip);
	pState->RegisterFunc("RestoreShopEquip",&SC_RestoreShopEquip);
	pState->RegisterFunc("CleanShopEquipInfo",&SC_CleanShopEquipInfo);
	
	pState->RegisterFunc("EquipItemImmediate", &SC_EquipItemImmediate);
	// Task Associated
	pState->RegisterFunc("LeaveTask", &SC_LeaveTask);
	pState->RegisterFunc("RequestStartTaskSingleTask", &SC_RequestStartTaskSingleTask);
	pState->RegisterFunc("IsFinishNewHandTask", &SC_IsFinishNewHandTask);
	pState->RegisterFunc("IsCurTaskFinish", &SC_IsCurTaskFinish);
	pState->RegisterFunc("GetCurTaskID", &SC_GetCurTaskID);
	pState->RegisterFunc("GetCountDownTime", &SC_GetCountDownTime);
	pState->RegisterFunc("RequestSkipTask", &SC_RequestSkipTask);
	pState->RegisterFunc("RequestReportCurrentOpt", &SC_RequestReportCurrentOpt);
	pState->RegisterFunc("IsAward", &SC_IsAward);
	pState->RegisterFunc("FinishAward", &SC_FinishAward);


	// Device Operator
	pState->RegisterFunc("ShowPointer", &SC_ShowPointer);

	pState->RegisterFunc("GetTickCount", &SC_GetTickCount);
	pState->RegisterFunc("GetGameRankInfo", &SC_GetGameRankInfo);

	// Profilling
	pState->RegisterFunc("Profilling", &SC_Profilling);
	pState->RegisterFunc("WinExecute", &SC_WinExecute);

	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_IsKeyChar,				IsKeyChar );
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_DrawKeyChar,			DrawKeyChar );
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_ValidKeyChar,			ValidKeyChar );
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_HitKeyChar,			HitKeyChar );
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_DrawItem,				DrawItem );
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_Encrypt,				UIBufferEncrypt );
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_Decrypt,				UIBufferDecrypt );
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_ParseCustomBuffer,		UIParseCustomBuffer);
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_DrawCustomBuffer,		UIDrawCustomBuffer);
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_CustomBufferSelfShow,	UICustomBufferSelfShow);
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_CustomBufferDeinit,	UICustomBufferDeinit);
	GlobalInterface::m_pGUI->RegisterUICallBack( UI_Callback_GetCustomBuffer,       UIGetCustomBuffer);
}