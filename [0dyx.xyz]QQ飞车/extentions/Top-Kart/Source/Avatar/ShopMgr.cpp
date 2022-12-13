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
{//�ͷ�
	pSelfShowNode=0;
	pAttachNode=0;
}

NiNodePtr tagShopItem::GetSelfShowNode(void)
{
	if(pSelfShowNode)
	{//������Դ�����账��
	}
	else
	{
		if (1==insCmy.m_shItemCount)
		{//ֱ��ʹ��Avatar��ģ������
			ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(insCmy.m_astItemInfo[0].m_iItemID);
			if (pItem)
			{
				CAvatarMgr::Get()->GetItemDataMgr()->GetItemNodeClone(pItem,pSelfShowNode);
				strMovNodeName=pItem->strModelName;
			}
		}
		else if (insCmy.m_shItemCount>0)
		{//Ҫ��ȡ����Ʒ���е�
			/*CShopMgr::Get()->GetShopItemNodeClone(this,pSelfShowNode);
			strMovNodeName="";*/
			//todo:ʹ�õ�һ����Ʒ��Ϊ��ʾ
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
	//����Ʒ�б�
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
	{//�ж��ļ��Ƿ���ڣ����δ������Ҫ��������
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
	//todo:Ŀǰ�������ֻʹ��BaseType��Ϊ��Ʒ����
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
		{//δ���������Ʒ�����ݷ������ָ����
			insShopItem.insCmy=pList[nCnt];
			m_maShopItemList[pList[nCnt].m_iID]=insShopItem;
			//��������ӷ���
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
	{//û�и���Ʒ��job���ڣ����Լ���
		BuyState* pState=&m_maBuyJob[pReq->m_iCommodityID];
		if (0==pState->nState)
		{
			pState->nState=1;
			pState->nLastStaChgTi=nNowTi;
			bRes=true;
		} 
		else
		{//�����ظ����򣬳��ǳ�ʱ
			if ((nNowTi-pState->nLastStaChgTi)>conBuyOvertimeCount)
			{//��ʱ������״̬�����Թ���		
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
	{//�и���Ʒ��job����
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
	{//�и���Ʒ��job����
		pState=&m_maBuyJob[nCommodityId];
	}
	else
	{//���Թ���
		BuyState insSta;
		insSta.nShopId=nCommodityId;
		insSta.nState=0;
		insSta.nLastStaChgTi=::GetTickCount();
		m_maBuyJob[nCommodityId]=insSta;
		pState=&m_maBuyJob[nCommodityId];
	}
	return pState;
}