////////////////////////////////////////////////
//FileName:    ShopMgr.h
//Author:      PigHuang
//Date:        2007-05-15
//Description: �̵�ϵͳ��Դ�������ļ��������ĵ�"QQ�������̵�ϵͳ���.doc"
////////////////////////////////////////////////

#pragma once
#include "../network/msgheader.h"
#include "itemdefine.h"
#include <list>
using namespace std;

typedef struct tagShopItem
{
	STCommodity	insCmy;
	NiNodePtr	pSelfShowNode;	///������Ʒ�̵������ʾ��ʵ��
	NiNodePtr	pAttachNode;	///�����ڳ�����������ʾ��ʵ��
	CStringEx	strMovNodeName;	///ģ���ڲ�����������ת�ľ���ڵ����֣��������һ��ת
	tagShopItem();
	~tagShopItem();
	///��ȡ��Ʒ�̵������ʾNode���Զ����������Դ���ɹ�����true
	NiNodePtr GetSelfShowNode(void);
}ShopItem;

typedef map<UINT,ShopItem>	SHOPITEMUNIT_MAP;	///����ƷIdΪ����
typedef map<UINT,ShopItem*>	SHOPPTLIST_MAP;	///����ƷId���ౣ��������Ʒָ��
	///�����ָ��ֱ��ָ����Ʒ���б�m_maShopItemList�ľ���ʵ�������ڷ����ѯ����

///��Ʒ�����ͽ��з���ṹ
typedef struct tagShopTypeObj
{
	UINT			nType;			///ʵ������(���ݷ���������)
	SHOPPTLIST_MAP	maPtList;	///Ŀǰʵ�����е�װ���б�
}ShopTypeObj;

typedef map<USHORT,ShopTypeObj>	SHOPTYPEOBJ_MAP;	///����Ʒ����(��������)���ౣ��������Ʒָ��

typedef struct tagBuyState
{
	UINT nShopId;	///��Ʒid
	UINT nState;	///����ֵ������״̬,0=������ɣ�1=������
	UINT nLastStaChgTi;	///���һ��״̬�л���ʱ�䣬���������̳���30�룬�Զ�����
	CStringEx strDoc;///�ɹ���ʧ��ԭ��˵��
}BuyState;

const UINT conBuyOvertimeCount=30000; ///����ʱ����ֵ����λ��ms

class CShopMgr:
	public NiMemObject
{
public:

	///��ȡ����������
	static CShopMgr* Get();
	///��ʼ��������
	static void Create();
	///������������ͷ�������Դ
	static void Destroy();
	///��ȡShop Itemģ��clone�������������pClone=NULL����Ҫ��������
	static void GetShopItemNodeClone(ShopItem* pItem,NiNodePtr& pClone);
	///ͨ��Id��ȡshop item
	ShopItem*	GetShopItemById(UINT nId);
	///ͨ�����ͻ�ȡ�̵������
	ShopTypeObj*	GetShopTypeObj(UINT nType);
	///����Shop���ݣ�������Ʒ�б�
	bool LoadShopData(void);
	///������Ʒ�б���Ϣ��Ŀǰ�Ǵӷ����������������б����潫��Ϊ�ļ���ʽ
	bool StoreList(USHORT nNum,STCommodity* pList);
	///���빺���¼�б���ֹ�ظ�����ʧ�ܷ���false�����ܴ����������
	bool AddBuyJob(CRequestBuyCommodityPara* pReq);
	///ɾȥ�����б��¼�����Ź����ܣ�pInDoc˵���ɹ���ʧ�ܵ�ԭ��ֱ�ӷ��ؽ����ã�����ʧ�ܷ���false
	bool SetBuyJobState(UINT nCommodityId,UINT nState,char* pInDoc);
	///��ѯ��ǰ�����б��¼״̬����BuyState
	BuyState* GetBuyJobState(UINT nCommodityId);
	
	///����
	void Update(float fDeltaTime);
protected:
	CShopMgr();
	virtual ~CShopMgr();

	SHOPITEMUNIT_MAP	m_maShopItemList;	///��Ʒ�����б�
	SHOPTYPEOBJ_MAP		m_maShopType;	///������Ʒ������б�

private:
	static CShopMgr*	ms_pShopMgr;
	std::map<UINT,BuyState>m_maBuyJob;	///ʹ����Ʒid��Ϊ����
};