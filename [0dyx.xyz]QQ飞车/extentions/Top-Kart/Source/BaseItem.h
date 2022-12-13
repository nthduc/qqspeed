#ifndef BASEITEM_H
#define BASEITEM_H

#include <queue>
using namespace std;

#define MAX_OWNITEM_NUM	2


class CBaseItem : public NiMemObject
{
public:
	CBaseItem(){}
	virtual ~CBaseItem(){} 
	virtual NiSourceTexturePtr GetItemFigure() = 0;		//得到道具的图标
	virtual void Use() = 0;							//施放道具
	virtual void BeUsed(void * pData) = 0;			//被施放道具
	
};

class CAccelItem : public CBaseItem
{
public:
	CAccelItem();
	virtual ~CAccelItem();
	virtual NiSourceTexturePtr GetItemFigure();
	virtual void Use();
	virtual void BeUsed(void * pData);

private:
	NiSourceTexturePtr m_spItemFigure;
};


class CItemMgr : public NiMemObject
{

public:
	enum enItemType					//道具类型
	{
		enAccel,
		enItemTypeNum,
	};

public:
	CItemMgr();
	~CItemMgr();

	void UseItem();											//使用道具

	void UpdateAffected(float ftime);						//被道具影响，包括其它玩家的道具

	void PushOwnItem(enItemType type, void * pData);		//得到道具，把道具压入拥有道具的队列中

	void PushAffectedItem(enItemType type, void * pData);	//其它玩家对本地玩家施放的道具，把道具压入被使用道具的队列中
	
	void ShowItemsColumn(NiRendererPtr pkRenderer);			//显示道具栏

	void ClearItems();										//清除道具

	void TurnOffColumnShow();								//不显示道具栏
	void TurnOnColumnShow();								//显示道具栏


	static bool Create();									//创建道具管理类单件
	static void Destroy();									//销毁道具管理类单件
	static CItemMgr * Get();								//得到道具管理类单件

protected:
	void PopItemFigure();									//弹出道具的外观图片
	void PusnItemFigure(NiSourceTexturePtr pkTex);			//压入道具的外观图片


private:

	bool m_bIsShowColumn;

	NiScreenElementsPtr m_spItemColumnBase;					//道具栏图片
	NiScreenElementsPtr m_aspItemScnElt[MAX_OWNITEM_NUM];	//道具图片

	unsigned int m_uiCurOwnItemsNum;						//当前拥有道具数
	std::queue<CBaseItem*> m_pOwnItemsQueue;				//自身拥有道具的队列

	std::queue<CBaseItem*> m_pBeUsedItemsQueue;				//被其它玩家使用道具的队列

	static CItemMgr * ms_pItemMgr;

};


#endif