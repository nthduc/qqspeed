#include "Top-KartPCH.h"
#include "./avatarmgr.h"
#include "./ItemDataMgr.h"
#include "./ItemUpdateMgr.h"
#include "../nifop.h"
#include "../Platform/GlobalInterface.h"
#include "scriptutils.h"

CAvatarMgr*		CAvatarMgr::ms_pAvatarMgr=NULL;
CItemDataMgr*	CAvatarMgr::m_pItemDataMgr=NULL;
CItemUpdateMgr*	CAvatarMgr::m_pItemUpdateMgr=NULL;

CAvatarMgr::CAvatarMgr(void):
m_nOpId(0)
{
	
	m_pItemDataMgr=NiNew CItemDataMgr;
	m_pItemUpdateMgr=NiNew CItemUpdateMgr;
}

CAvatarMgr::~CAvatarMgr(void)
{
	SAFE_NIDELETE(m_pItemDataMgr);
	SAFE_NIDELETE(m_pItemUpdateMgr);
}

CAvatarMgr* CAvatarMgr::Get(void)
{
	_ASSERTE(ms_pAvatarMgr);
	return ms_pAvatarMgr;
}

void CAvatarMgr::Create(void)
{
	_ASSERTE(NULL==ms_pAvatarMgr);
	ms_pAvatarMgr=NiNew CAvatarMgr;
	///载入Avatar基础数据
	m_pItemDataMgr->LoadData();
}

void CAvatarMgr::Destroy(void)
{
	SAFE_NIDELETE(ms_pAvatarMgr);
}

UINT CAvatarMgr::GetNewOpId()
{
	m_nOpId++;
	return m_nOpId;
}

void CAvatarMgr::Update(float fDeltaTime)
{
	m_pItemUpdateMgr->Update(fDeltaTime);
}

bool CAvatarMgr::ChangeItemStatus(CRequestChangeItemStatus* pQueryInOut,
										bool bEquip)
{
	//pQuery->m_cQueryMode_In这里不使用
	_ASSERTE(pQueryInOut->m_unUin && pQueryInOut->m_sItemNum);
	bool bRes=true;
	bool bCarIn=false;	///判断是否涉及车子装备
	bool bRoleIn=false;	///判断是否涉及角色装备
	bool bHairIn=false;	///判断是否涉及角色头发
	UINT nCurrRoleId=0;	///本次装备的角色
	bool bCurrRoleState=0;	///装备的角色状态
	UINT nCurrHairId=0;	///本次装备的头发
	bool bCurrHairState=0;	///装备的头发状态
	///步骤
	///1.查找新Item所用到的挂接点，包括屏蔽的挂接点
	///2.把这些挂接点现有物品卸载
	///3.装备新Item
	ItemUnit* pItem=NULL,*pItem2nd=NULL;
	ItemStore* pStore=NULL,*pStore2nd=NULL;
	UINT nCntMain=0,nCnt=0,nTmp=0,nItemId=0,nDefTmp=0;
	UCHAR cStatus=0;
	//排序，把车辆和角色都放到前面
	std::list<STItemStatus>liCarRole;
	std::list<STItemStatus>liNormal;
	std::set<UINT>seAddId;	///防止多次加入
	Play* pPlay=NxPlayMgr::GetPlay(pQueryInOut->m_unUin);
	ItemUnit* pPlayRole=m_pItemDataMgr->GetItemById(pPlay->GetPlayInfo().m_nRoleID);
	ItPlayerUint* pPlayUn=m_pItemDataMgr->GetPlayerByUin(pQueryInOut->m_unUin);
	for (nCnt=0;nCnt<pQueryInOut->m_sItemNum;++nCnt)
	{
		pItem=GetItemInfoById(pQueryInOut->m_stItemStatus[nCnt].m_iItemID);
		if (seAddId.find(pItem->nId)!=seAddId.end())
		{
			continue;
		}
		UINT unBaseType = pItem->nType & 0xff00;
		//把车和角色放在最前
		if(unBaseType== EAIBT_CAR)
		{
			liCarRole.push_front(pQueryInOut->m_stItemStatus[nCnt]);
			bCarIn=true;
		}
		else if(unBaseType== EAIBT_ROLE)
		{
			liCarRole.push_back(pQueryInOut->m_stItemStatus[nCnt]);
			///如果是装备人物则更换为这个id
			if(pQueryInOut->m_stItemStatus[nCnt].m_bNewStatus)
			{
				pPlayRole=m_pItemDataMgr->GetItemById(pQueryInOut->m_stItemStatus[nCnt].m_iItemID);
			}
			nCurrRoleId=pQueryInOut->m_stItemStatus[nCnt].m_iItemID;
			bCurrRoleState=pQueryInOut->m_stItemStatus[nCnt].m_bNewStatus;
			bRoleIn=true;
		}
		else if (unBaseType== EAIBT_HAIR)
		{
			liNormal.push_back(pQueryInOut->m_stItemStatus[nCnt]);
			nCurrHairId=pQueryInOut->m_stItemStatus[nCnt].m_iItemID;
			bCurrHairState=pQueryInOut->m_stItemStatus[nCnt].m_bNewStatus;
			bHairIn=true;
		}
		else
		{
			liNormal.push_back(pQueryInOut->m_stItemStatus[nCnt]);
		}
		seAddId.insert(pItem->nId);
	}
	//todo:临时代码,判断如果有角色涉及装备，就把对应的头发也卸载或装备
	if(nCurrRoleId && !nCurrHairId)
	{
		STItemStatus insSTSta;
		if (10018==nCurrRoleId)
		{
			insSTSta.m_bNewStatus=bCurrRoleState;
			insSTSta.m_iItemID=10021;
			liNormal.push_back(insSTSta);
		}
		else if (10022==nCurrRoleId)
		{
			insSTSta.m_bNewStatus=bCurrRoleState;
			insSTSta.m_iItemID=10025;
			liNormal.push_back(insSTSta);
		}
	}
	//有车辆或角色的装备，就要重新绑定所有装备道具
	if (bCarIn || bRoleIn)
	{
		ITEMSTORE_MAP::iterator itStore;
		ItTypeObj* pObj=NULL;
		STItemStatus insStatus;
		ITEMOBJ_MAP::iterator itPlay=pPlayUn->maHObjList.begin();
		//历遍处理全部类型对象链
		for (;itPlay!=pPlayUn->maHObjList.end();++itPlay)
		{
			pObj=&(itPlay->second);

			//历遍处理全部使用中的装备
			itStore=pObj->maItemStore.begin();
			for (;itStore!=pObj->maItemStore.end();++itStore)
			{
				pStore=&(itStore->second);
				pItem=GetItemInfoById(pStore->insInfo.m_iItemID);
				UINT unBaseType = pItem->nType & 0xff00;
				if (seAddId.find(pItem->nId)!=seAddId.end() || pStore->insInfo.m_bStatus!=EIS_EQUIPPED)
				{
					continue;
				}
				insStatus.m_iItemID=pItem->nId;
				insStatus.m_bNewStatus=EIS_EQUIPPED;
				//把车和角色放在最前
				if(unBaseType== EAIBT_CAR)
				{
					if (!bCarIn)
					{
						liCarRole.push_front(insStatus);
					}
				}
				else if(unBaseType== EAIBT_ROLE)
				{
					if (!bRoleIn)
					{
						liCarRole.push_back(insStatus);
					}
				}
				else
				{
					liNormal.push_front(insStatus);
				}
				seAddId.insert(pItem->nId);
			}
		}

		
	}
	//解除绑定
	if (bEquip && (bCarIn || bRoleIn || bHairIn))
	{
		TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnBeforeBind");
	}
	////////////
	CRequestChangeItemStatus insQu;
	memcpy(&insQu,pQueryInOut,sizeof(CRequestChangeItemStatus));
	insQu.m_sItemNum=0;
	if (!liCarRole.empty())
	{
		std::list<STItemStatus>::iterator it=liCarRole.begin();
		for (;it!=liCarRole.end();++it)
		{
			insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=(*it).m_iItemID;
			insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=(*it).m_bNewStatus;
			insQu.m_sItemNum++;
		}
	}
	//
	if (!liNormal.empty())
	{
		std::list<STItemStatus>::iterator it=liNormal.begin();
		for (;it!=liNormal.end();++it)
		{
			insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=(*it).m_iItemID;
			insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=(*it).m_bNewStatus;
			insQu.m_sItemNum++;
		}
	}
	//开始总装备
	//memcpy(&insQu,pQueryInOut,sizeof(CRequestChangeItemStatus));
	memcpy(pQueryInOut,&insQu,sizeof(CRequestChangeItemStatus));
	insQu.m_sItemNum=0;
	for (nCntMain=0;nCntMain<pQueryInOut->m_sItemNum;++nCntMain)
	{
		nItemId=pQueryInOut->m_stItemStatus[nCntMain].m_iItemID;
		cStatus=pQueryInOut->m_stItemStatus[nCntMain].m_bNewStatus;
		pItem=m_pItemDataMgr->GetItemById(nItemId);
		pStore=m_pItemDataMgr->GetItemStore(pQueryInOut->m_unUin,nItemId);
		if (!pStore)
		{
			continue;
		}
		nTmp=m_pItemDataMgr->GetItemOnHitch(pPlayUn,pItem->nType);
		//todo:加入一个判断新人物不转配人物装备的功能
		if (EAIBT_ROLERELEVANT==(pItem->nType & 0xff00) && EAIT_ROLE_NEWTEST==pPlayRole->nType)
		{
			continue;
		}
		//根据性别判断装备是否可以使用
		if (pItem->nSexType>0 && pItem->nSexType!=pPlayRole->nSexType)
		{
			continue;
		}


		///////////////////
		if (EIS_UNEQUIPPED==cStatus && EIS_EQUIPPED==pStore->insInfo.m_bStatus)
		{//卸载，判断该挂接点是否有默认装备，有则不能直接卸载
			nDefTmp=m_pItemDataMgr->GetDefaultOnHitch(pItem->nType);
			if(nDefTmp)
			{
				continue;
			}
			//
			if (nTmp==nItemId)
			{//卸载主挂接点原有装备
				insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=nItemId;
				insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=cStatus;
				insQu.m_sItemNum++;
				if (bEquip)
				{//需要真正装备
					m_pItemDataMgr->SetItemOnHitch(pPlayUn,pItem->nType,0);
					m_pItemDataMgr->BindItem(pPlay,pStore,false);
					pStore->insInfo.m_bStatus=EIS_UNEQUIPPED;
				}
				//卸载其他挂接点
				for (nCnt=0;nCnt<pItem->nBlockSlotNum;++nCnt)
				{
					nTmp=m_pItemDataMgr->GetItemOnHitch(pPlayUn,pItem->arrBlockId[nCnt]);
					if (nTmp==nItemId)
					{//需要卸载各挂接点原有装备
						//todo:这里还需要考虑自动装载该挂接点的默认装备
						if (bEquip)
						{
							m_pItemDataMgr->SetItemOnHitch(pPlayUn,
								pItem->arrBlockId[nCnt],0);
						}
					}
				}
			}
		}
		else if (EIS_EQUIPPED==cStatus/* && EIS_UNEQUIPPED==pStore->insInfo.m_bStatus*/)
		{//装备
			insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=nItemId;
			insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=EIS_EQUIPPED;
			insQu.m_sItemNum++;
			if (nTmp && nTmp!=nItemId)
			{//需要卸载主挂接点原有装备
				insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=nTmp;
				insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=EIS_UNEQUIPPED;
				insQu.m_sItemNum++;
				if (bEquip)
				{
					pStore2nd=m_pItemDataMgr->GetItemStore(pQueryInOut->m_unUin,nTmp);
					pStore2nd->insInfo.m_bStatus=EIS_UNEQUIPPED;
					m_pItemDataMgr->BindItem(pPlay,pStore2nd,false);
				}
			}
			//
			//卸载其他挂接点
			for (nCnt=0;nCnt<pItem->nBlockSlotNum;++nCnt)
			{
				nTmp=m_pItemDataMgr->GetItemOnHitch(pPlayUn,pItem->arrBlockId[nCnt]);
				if (nTmp && nTmp!=nItemId)
				{
					insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=nItemId;
					insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=EIS_UNEQUIPPED;
					insQu.m_sItemNum++;
					if (bEquip)
					{//需要卸载各挂接点原有装备
						pStore2nd=m_pItemDataMgr->GetItemStore(pQueryInOut->m_unUin,nTmp);
						pStore2nd->insInfo.m_bStatus=EIS_UNEQUIPPED;
						m_pItemDataMgr->SetItemOnHitch(pPlayUn,
							pItem->arrBlockId[nCnt],nItemId);
						m_pItemDataMgr->BindItem(pPlay,pStore2nd,false);
					}
				}
			}
			//
			if (bEquip)
			{
				//正式装备
				m_pItemDataMgr->SetItemOnHitch(pPlayUn,pItem->nType,nItemId);
				pStore->insInfo.m_bStatus=EIS_EQUIPPED;
				m_pItemDataMgr->BindItem(pPlay,pStore,true);
			}
		}
	}
	//重新绑定
	if (bEquip && (bCarIn || bRoleIn || bHairIn))
	{
		TriggleScriptEvent(GlobalInterface::m_pScriptState, "OnAfterBind");
	}
	//
	memcpy(pQueryInOut,&insQu,sizeof(CRequestChangeItemStatus));
	return bRes;	
}

ItemUnit* CAvatarMgr::GetItemInfoById(UINT nId)
{//内部已有校验
	return m_pItemDataMgr->GetItemById(nId);
}

bool CAvatarMgr::GetPlayerItemList(QueryPlayerAvatarInfo* pList)
{
	_ASSERTE(pList->m_unUin_In>0 && pList->m_cQueryMode_In>0 &&
		pList->m_cQueryMode_In<EIQIT_MAX);
	bool bRes=false;
	UINT nNumber=0;
	ItPlayerUint* pPlayer=m_pItemDataMgr->GetPlayerByUin(pList->m_unUin_In);
	if(pPlayer)
	{
		ItemStore* pStore=NULL;
		ItTypeObj* pObj=NULL;
		ItemUnit* pItem=NULL;
		ITEMSTORE_MAP::iterator itStore;
		ITEMOBJ_MAP::iterator itPlay=pPlayer->maHObjList.begin();
		//历遍处理全部类型对象链
		for (;itPlay!=pPlayer->maHObjList.end();++itPlay)
		{
			pObj=&(itPlay->second);
			//历遍处理全部使用中的装备
			itStore=pObj->maItemStore.begin();
			for (;itStore!=pObj->maItemStore.end();++itStore)
			{
				pStore=&(itStore->second);
				switch(pList->m_cQueryMode_In)
				{
				case	EIQIT_ALL_ITEM:
					{
						break;
					}
				case	EIQIT_EQUIPPED_ITEM:
					{
						if (pStore->insInfo.m_bStatus!=EIS_EQUIPPED)
						{//未装备的无需记录
							continue;
						}
						break;
					}
				case	EIQIT_UNEQUIPPED_ITEM:
					{
						if (EIS_EQUIPPED==pStore->insInfo.m_bStatus)
						{//装备的无需记录
							continue;
						}
						break;
					}
				default:
					_ASSERTE(0);
				}
				memcpy(&pList->m_stItemInfo_Out[nNumber],&pStore->insInfo,
					sizeof(STPlayerDBItemInfo));
				nNumber++;
			}
		}
	}
	pList->m_nItemNum_InOut=nNumber;
	return bRes;
}

bool CAvatarMgr::BindRes1st(ULONG dwUin)
{
	bool bRes=true;
	ItPlayerUint* pPlayer=m_pItemDataMgr->GetPlayerByUin(dwUin);
	if(pPlayer)
	{
		ItemStore* pStore=NULL;
		ItemUnit* pItem=NULL;
		ItTypeObj* pObj=NULL;
		ITEMSTORE_MAP::iterator itStore;
		ITEMOBJ_MAP::iterator itPlay=pPlayer->maHObjList.begin();
		CRequestChangeItemStatus insQu;
		std::list<ItemStore*>liStaTmp;
		UINT nHeadType=0;
		insQu.m_unUin=dwUin;
		insQu.m_unTime=::GetTickCount();
		insQu.m_sItemNum=0;
		//历遍处理全部类型对象链
		for (;itPlay!=pPlayer->maHObjList.end();++itPlay)
		{
			pObj=&(itPlay->second);
			
			//历遍处理全部使用中的装备
			itStore=pObj->maItemStore.begin();
			for (;itStore!=pObj->maItemStore.end();++itStore)
			{
				pStore=&(itStore->second);
				pItem=GetItemInfoById(pStore->insInfo.m_iItemID);
				//UINT unBaseType = pItem->nType & 0xff00;
				if (pStore->insInfo.m_bStatus!=EIS_EQUIPPED)
				{
					continue;
				}
				/*
				//把车和角色放在最前
				if(unBaseType== EAIBT_ROLE || unBaseType== EAIBT_CAR)
				{
					if(!liStaTmp.empty() && EAIBT_CAR==nHeadType)
					{//链头是车，插后面
						liStaTmp.insert(liStaTmp.begin(),pStore);
					}
					else
					{
						liStaTmp.push_front(pStore);
					}
					nHeadType=unBaseType;
				}
				else
				{
					liStaTmp.push_back(pStore);
				}
				)*/
				insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=pStore->insInfo.m_iItemID;
				insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=pStore->insInfo.m_bStatus;
				insQu.m_sItemNum++;
			}
		}
		//if (!liStaTmp.empty())
		//{
		//	std::list<ItemStore*>::iterator it=liStaTmp.begin();
		//	for (;it!=liStaTmp.end();++it)
		//	{
		//		insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=(*it)->insInfo.m_iItemID;
		//		insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=(*it)->insInfo.m_bStatus;
		//		insQu.m_sItemNum++;
		//	}
		//	ChangeItemStatus(&insQu,true);
		//}
		ChangeItemStatus(&insQu,true);
	}
	return bRes;
}

UINT CAvatarMgr::GetItemBaseType(UINT nItemId)
{
	ItemUnit* pItem=m_pItemDataMgr->GetItemById(nItemId);
	return (pItem->nType & 0xff00);
}
