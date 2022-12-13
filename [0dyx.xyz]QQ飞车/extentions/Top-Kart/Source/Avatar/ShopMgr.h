////////////////////////////////////////////////
//FileName:    ShopMgr.h
//Author:      PigHuang
//Date:        2007-05-15
//Description: 商店系统资源管理定义文件，配套文档"QQ卡丁车商店系统设计.doc"
////////////////////////////////////////////////

#pragma once
#include "../network/msgheader.h"
#include "itemdefine.h"
#include <list>
using namespace std;

typedef struct tagShopItem
{
	STCommodity	insCmy;
	NiNodePtr	pSelfShowNode;	///用作物品商店橱窗演示的实例
	NiNodePtr	pAttachNode;	///用于在车或人身上演示的实例
	CStringEx	strMovNodeName;	///模型内部可以用于旋转的具体节点名字，避免相机一起转
	tagShopItem();
	~tagShopItem();
	///获取物品商店橱窗演示Node，自动载入相关资源，成功返回true
	NiNodePtr GetSelfShowNode(void);
}ShopItem;

typedef map<UINT,ShopItem>	SHOPITEMUNIT_MAP;	///以商品Id为索引
typedef map<UINT,ShopItem*>	SHOPPTLIST_MAP;	///以商品Id分类保存具体的商品指针
	///这里的指针直接指向商品总列表m_maShopItemList的具体实例，用于分类查询而已

///商品按类型进行分类结构
typedef struct tagShopTypeObj
{
	UINT			nType;			///实体类型(根据分类来索引)
	SHOPPTLIST_MAP	maPtList;	///目前实体所有的装备列表
}ShopTypeObj;

typedef map<USHORT,ShopTypeObj>	SHOPTYPEOBJ_MAP;	///以商品类型(基本类型)分类保存具体的商品指针

typedef struct tagBuyState
{
	UINT nShopId;	///商品id
	UINT nState;	///具体值代表购买状态,0=购买完成，1=购买中
	UINT nLastStaChgTi;	///最后一次状态切换的时间，如果购买过程超过30秒，自动解锁
	CStringEx strDoc;///成功或失败原因说明
}BuyState;

const UINT conBuyOvertimeCount=30000; ///购买超时限制值，单位是ms

class CShopMgr:
	public NiMemObject
{
public:

	///获取管理器对象
	static CShopMgr* Get();
	///初始化管理器
	static void Create();
	///清理管理器，释放所有资源
	static void Destroy();
	///获取Shop Item模型clone，如果不存在则pClone=NULL，需要处理下载
	static void GetShopItemNodeClone(ShopItem* pItem,NiNodePtr& pClone);
	///通过Id获取shop item
	ShopItem*	GetShopItemById(UINT nId);
	///通过类型获取商店分类链
	ShopTypeObj*	GetShopTypeObj(UINT nType);
	///载入Shop数据，例如商品列表
	bool LoadShopData(void);
	///保存商品列表信息，目前是从服务器拉回来数据列表，后面将改为文件方式
	bool StoreList(USHORT nNum,STCommodity* pList);
	///加入购买记录列表，防止重复购买，失败返回false，不能处理这个购买
	bool AddBuyJob(CRequestBuyCommodityPara* pReq);
	///删去购买列表记录，开放购买功能，pInDoc说明成功或失败的原因，直接返回界面用，函数失败返回false
	bool SetBuyJobState(UINT nCommodityId,UINT nState,char* pInDoc);
	///查询当前购买列表记录状态，看BuyState
	BuyState* GetBuyJobState(UINT nCommodityId);
	
	///更新
	void Update(float fDeltaTime);
protected:
	CShopMgr();
	virtual ~CShopMgr();

	SHOPITEMUNIT_MAP	m_maShopItemList;	///商品的总列表
	SHOPTYPEOBJ_MAP		m_maShopType;	///根据商品分类的列表

private:
	static CShopMgr*	ms_pShopMgr;
	std::map<UINT,BuyState>m_maBuyJob;	///使用商品id作为索引
};