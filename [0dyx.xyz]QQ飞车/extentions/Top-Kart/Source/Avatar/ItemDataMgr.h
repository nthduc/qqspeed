////////////////////////////////////////////////
//FileName:    ItemDataMgr.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatar数据管理类
////////////////////////////////////////////////

#pragma once
#include "ItemDefine.h"

class Play;

///Avatar数据管理类
class CItemDataMgr:
	public NiMemObject
{
	friend class CAvatarMgr;
	friend class CItemUpdateMgr;

public:
	///载入Avatar固定数据，成功返回true
	bool LoadData();
	///通过Id获取item
	ItemUnit*	GetItemById(UINT nId);
	///获取装备类型命名
	const char* GetTypeName(UINT nType);
	///获取Item模型clone
	void GetItemNodeClone(ItemUnit* pItem,NiNodePtr& pClone);
	///获取Item贴图clone
	void GetItemTexClone(ItemUnit* pItem,NiSourceTexturePtr& pTexClone);
	///备份某个玩家的Avatar信息，在进入商店或车库前备份，以便回复玩家整体装备
	void BackupUserInfo(UINT dwUin);
	///恢复某个玩家的Avatar信息，从商店或车库返回时恢复
	void RestoreUserInfo(UINT dwUin);
	///返回装备存储单元（如果未有相关存储，自动添加），dwUin是用户QQ号，nItemId是Item的统一id
	///成功返回ItemStore指针
	ItemStore* GetItemStore(ULONG dwUin,UINT nItemId);
	///增加ItemStore，返回对象指针，bAddBackup=true，同时处理备用链列表
	ItemStore* AddItemStore(ULONG dwUin,STPlayerDBItemInfo* pItem,bool bAddBackup);
	///返回某个玩家，指定道具类型的当前装备Item实例
	ItemStore* GetCurrEquipStore(ULONG dwUin,UINT nType);
	///清除现有的所有道具绑定
	bool CleanAllBind(void);
	///清除用户备份信息
	bool CleanUserInfo(UINT dwUin);
	///清除某个用户的所有avtar数据，不包括用户备份信息
	void CleanMainAvatarData(UINT dwUin);
	///获取车库分类列表
	GarageTypeObj* GetGarageTypeObj(UINT dwUin,UINT nGarageType);
	///在游戏过程中隐藏或显示角色腿部的脚部位，bShowFlag=true 显示，=false 隐藏
	void ShowRolePart(Play* pPlay,bool bShowFlag);
protected:
	CItemDataMgr(void);
	virtual ~CItemDataMgr(void);
	///获取指定用户号的指定类型的实体，失败返回NULL
	ItTypeObj* GetTypeObj(ULONG dwUin,UINT nHitchObjId);
	///获取挂接实体里面的Item实例
	ItemStore* GetObjItStore(ItTypeObj* pObj,UINT nId);
	///获取用户Avatar管理单元指针
	ItPlayerUint* GetPlayerByUin(ULONG dwUin);
	///获取指定Type挂接点现有挂接的item id，失败返回0
	UINT GetItemOnHitch(ItPlayerUint* pPlayUn,UINT nType);
	///设置指定Type挂接点现有挂接的item id
	void SetItemOnHitch(ItPlayerUint* pPlayUn,UINT nType,UINT nItemId);
	///获取指定Type挂接点的默认挂接item id
	UINT GetDefaultOnHitch(UINT nType);
	///获取制定Type的Nif文件具体路径
	void GetNifFilePath(UINT nType,CStringEx& strName);
	///替换带骨骼数据的模型，调用本函数后pNewNode数据会缺失，不能再使用
	bool ChangeShapeWithSkin(NiNode* pOriNode,NiNode* pNewNode,const char* cpShapeName);
private:
	///载入Item的资源，成功返回true
	bool LoadItemRes(ItemUnit* pItem);
	///根据状态对指定玩家进行装备
	bool BindItem(Play* pPlay,ItemStore* pStore,bool bEquip);
	/////////////////////////////////////
	ITEMHITCH_MAP	m_maHitchList;	///挂接点
	ITEMUNIT_MAP	m_maItemList;	///装备
	ITEMPLAYER_MAP	m_maPlayerList;	///每个玩家所拥有实体列表，以用户号码(QQ号码)为索引
};
