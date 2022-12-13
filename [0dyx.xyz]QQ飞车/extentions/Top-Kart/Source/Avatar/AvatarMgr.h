////////////////////////////////////////////////
//FileName:    AvatarMgr.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatar管理类
////////////////////////////////////////////////

#pragma once
#include "ItemDefine.h"
#include "ItemDataMgr.h"


class CItemDataMgr;
class CItemUpdateMgr;

class CAvatarMgr:
	public NiMemObject
{
public:
	///获取管理器对象
	static CAvatarMgr* Get();
	///初始化管理器
	static void Create();
	///清理管理器，释放所有资源
	static void Destroy();

	CItemDataMgr* GetItemDataMgr()
	{
		return m_pItemDataMgr;
	}
	CItemUpdateMgr* GetItemUpdateMgr()
	{
		return m_pItemUpdateMgr;
	}
protected:
	CAvatarMgr(void);
	virtual ~CAvatarMgr(void);

private:
	///获得最新的操作OpId
	UINT GetNewOpId();
	///Item数据管理
	static CItemDataMgr* m_pItemDataMgr;
	///Item图像、逻辑管理
	static CItemUpdateMgr* m_pItemUpdateMgr;
	///
	static CAvatarMgr*	ms_pAvatarMgr;
	UINT				m_nOpId;		///内部记录UseItem()成功操作自动增量ID
public:
	///更新
	void Update(float fDeltaTime);
	///更换(使用)装备，pQuery是用户QQ号和需要更换新状态的item列表,
		///bEquip是处理模式，true则正式装备并返回处理结果列表，false只查询处理列表并不
		///改变item装备的真实状态，用于网络发送时得到处理列表
		///成功返回true
	bool ChangeItemStatus(CRequestChangeItemStatus* pQueryInOut,bool bEquip);
	///对玩家进行第一次资源装备，包括nif绑定等相关资源，dwUin是用户QQ号
	bool BindRes1st(ULONG dwUin);
	///通过Id获取item资料
	ItemUnit*	GetItemInfoById(UINT nId);
	///查询指定用户的相关Avatar装备信息，输入参数请看FindPlayerAvatarInfo结构
	bool GetPlayerItemList(QueryPlayerAvatarInfo* pList);
	///输入Item Id，获取Item的基本类型，看enmItBaseType
	UINT GetItemBaseType(UINT nItemId);
 };
