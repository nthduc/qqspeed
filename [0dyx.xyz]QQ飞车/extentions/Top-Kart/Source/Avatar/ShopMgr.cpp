#include "Top-KartPCH.h"
#include "ShopMgr.h"
#include "AvatarMgr.h"
#include "AvatarNet.h"
#include "ItemDataMgr.h"
#include "fileop.h"
#include "../network/msgheader.h"

CShopMgr*		CShopMgr::ms_pShopMgr=NULL;

tagShopItem::tagShopItem()
{
	pSelfShowNode=0;
	pAttachNode=0;
}

tagShopItem::~tagShopItem()
{//释放
	pSelfShowNode=0;
	pAttachNode=0;
}

NiNodePtr tagShopItem::GetSelfShowNode(void)
{
	if(pSelfShowNode)
	{//已有资源，无需处理
	}
	else
	{
		if (1==insCmy.m_shItemCount)
		{//直接使用Avatar的模型数据
			ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(insCmy.m_astItemInfo[0].m_iItemID);
			if (pItem)
			{
				CAvatarMgr::Get()->GetItemDataMgr()->GetItemNodeClone(pItem,pSelfShowNode);
				strMovNodeName=pItem->strModelName;
			}
		}
		else if (insCmy.m_shItemCount>0)
		{//要读取该商品特有的
			/*CShopMgr::Get()->GetShopItemNodeClone(this,pSelfShowNode);
			strMovNodeName="";*/
			//todo:使用第一个商品作为演示
			ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(insCmy.m_astItemInfo[0].m_iItemID);
			if (pItem)
			{
				CAvatarMgr::Get()->GetItemDataMgr()->GetItemNodeClone(pItem,pSelfShowNode);
				strMovNodeName=pItem->strModelName;
			}
		}
	}
	return pSelfShowNode;
}

CShopMgr::CShopMgr()
{
}

CShopMgr::~CShopMgr(void)
{
}

CShopMgr* CShopMgr::Get(void)
{
	_ASSERTE(ms_pShopMgr);
	return ms_pShopMgr;
}

void CShopMgr::Create(void)
{
	_ASSERTE(NULL==ms_pShopMgr);
	ms_pShopMgr=NiNew CShopMgr;
}

void CShopMgr::Destroy(void)
{
	SAFE_NIDELETE(ms_pShopMgr);
}

void CShopMgr::Update(float fDeltaTime)
{

}

bool CShopMgr::LoadShopData(void)
{
	//拉商品列表
	CRequestGetCommodityListPara pGetCommList;
	memset(&pGetCommList,0,sizeof(CRequestGetCommodityListPara));
	pGetCommList.m_unTime=::GetTickCount();
	pGetCommList.m_unUin=NxPlayMgr::GetHostPlay()->GetPlayInfo().m_unUin;
	CAvatarNet::RequestGetCommodityListPara(&pGetCommList);
	/*
	CRequestBuyCommodityPara pGetCommList;
	memset(&pGetCommList,0,sizeof(CRequestBuyCommodityPara));
	pGetCommList.m_unTime=::GetTickCount();
	pGetCommList.m_iReqBuySeq=pGetCommList.m_unTime;
	pGetCommList.m_unUin=NxPlayMgr::GetHostPlay()->GetPlayInfo().m_unUin;
	pGetCommList.m_unDstUin=pGetCommList.m_unUin;
	pGetCommList.m_iPayType=EPT_PAY_KB;
	pGetCommList.m_iCommodityID=1;
	CAvatarNet::RequestBuyCommodity(&pGetCommList);
	*/
	return true;
}

ShopItem*	CShopMgr::GetShopItemById(UINT nId)
{
	ShopItem* pShopItem=NULL;
	if(m_maShopItemList.find(nId)!=m_maShopItemList.end())
	{
		pShopItem=&m_maShopItemList[nId];
	}
	return pShopItem;
}

void CShopMgr::GetShopItemNodeClone(ShopItem* pItem,NiNodePtr& pClone)
{
	_ASSERTE(!pClone);
	CStringEx strName,strPath;
	strPath.Format("Shop/%u/%u.nif",pItem->insCmy.m_iID,
		pItem->insCmy.m_iID);
	if(FileExists(strPath.c_str()))
	{//判断文件是否存在，如果未有则需要触发下载
		NiStream kStream;
		if (!kStream.Load(NiApplication::ConvertMediaFilename(strPath.c_str())))
		{
			_ASSERTE(0);
		}
		NiNode* pkNode=NiDynamicCast(NiNode, kStream.GetObjectAt(0));
		pClone = NiDynamicCast(NiNode,pkNode->Clone());
		pClone->UpdateEffects();
		pClone->UpdateProperties();
		pClone->Update(0.0f);
		NiTimeController::StartAnimations(pClone);
	}
	else
	{

	}
}

ShopTypeObj*	CShopMgr::GetShopTypeObj(UINT nType)
{
	ShopTypeObj* pShopType=NULL;
	//todo:目前这里的链只使用BaseType作为商品分类
	nType=nType & 0xffff;
	if(m_maShopType.find(nType)!=m_maShopType.end())
	{
		pShopType=&m_maShopType[nType];
	}
	else
	{
		ShopTypeObj insObj;
		insObj.nType=nType;
		m_maShopType[nType]=insObj;
		pShopType=&m_maShopType[nType];
	}
	_ASSERTE(pShopType);
	return pShopType;
}

bool CShopMgr::StoreList(USHORT nNum,STCommodity* pList)
{
	ShopItem* pShopItem=NULL;
	ShopTypeObj* pType=NULL;
	ShopItem insShopItem;
	UINT nCnt=0;
	for (nCnt=0;nCnt<nNum;++nCnt)
	{
		pShopItem=GetShopItemById(pList[nCnt].m_iID);
		if (!pShopItem)
		{//未加入添加商品并根据分类加入指针链
			insShopItem.insCmy=pList[nCnt];
			m_maShopItemList[pList[nCnt].m_iID]=insShopItem;
			//加入各个子分类
			USHORT nAndUse=1,nTypeTmp=0;
			do 
			{
				nTypeTmp=insShopItem.insCmy.m_stType.m_shBase & nAndUse;
				if (nTypeTmp)
				{
					pType=GetShopTypeObj(nTypeTmp);
					pType->maPtList[insShopItem.insCmy.m_iID]=&m_maShopItemList[pList[nCnt].m_iID];
				}
				nAndUse=nAndUse<<1;
			} while(nAndUse!=0x8000);
		}
	}
	return true;
}

bool CShopMgr::AddBuyJob(CRequestBuyCommodityPara* pReq)
{
	bool bRes=false;
	UINT nNowTi=::GetTickCount();
	if(m_maBuyJob.find(pReq->m_iCommodityID)!=m_maBuyJob.end())
	{//没有该商品的job存在，可以加入
		BuyState* pState=&m_maBuyJob[pReq->m_iCommodityID];
		if (0==pState->nState)
		{
			pState->nState=1;
			pState->nLastStaChgTi=nNowTi;
			bRes=true;
		} 
		else
		{//不能重复购买，除非超时
			if ((nNowTi-pState->nLastStaChgTi)>conBuyOvertimeCount)
			{//超时，重置状态，可以购买		
				pState->nState=1;
				pState->nLastStaChgTi=nNowTi;
				bRes=true;
			}
			else
			{
				bRes=false;
			}
		}
		
	}
	else
	{
		BuyState insBuySta;
		insBuySta.nShopId=pReq->m_iCommodityID;
		insBuySta.nState=1;
		insBuySta.nLastStaChgTi=nNowTi;
		m_maBuyJob[pReq->m_iCommodityID]=insBuySta;
		bRes=true;
	}
	return bRes;
}

bool CShopMgr::SetBuyJobState(UINT nCommodityId,UINT nState,char* pInDoc)
{
	bool bRes=false;
	if(m_maBuyJob.find(nCommodityId)!=m_maBuyJob.end())
	{//有该商品的job存在
		m_maBuyJob[nCommodityId].nState=nState;
		m_maBuyJob[nCommodityId].nLastStaChgTi=::GetTickCount();
		m_maBuyJob[nCommodityId].strDoc=pInDoc;
		bRes=true;
	}
	return bRes;
}

BuyState* CShopMgr::GetBuyJobState(UINT nCommodityId)
{
	BuyState* pState=NULL;
	if(m_maBuyJob.find(nCommodityId)!=m_maBuyJob.end())
	{//有该商品的job存在
		pState=&m_maBuyJob[nCommodityId];
	}
	else
	{//可以购买
		BuyState insSta;
		insSta.nShopId=nCommodityId;
		insSta.nState=0;
		insSta.nLastStaChgTi=::GetTickCount();
		m_maBuyJob[nCommodityId]=insSta;
		pState=&m_maBuyJob[nCommodityId];
	}
	return pState;
}