////////////////////////////////////////////////
//FileName:    ItemDataMgr.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatar���ݹ�����
////////////////////////////////////////////////

#pragma once
#include "ItemDefine.h"

class Play;

///Avatar���ݹ�����
class CItemDataMgr:
	public NiMemObject
{
	friend class CAvatarMgr;
	friend class CItemUpdateMgr;

public:
	///����Avatar�̶����ݣ��ɹ�����true
	bool LoadData();
	///ͨ��Id��ȡitem
	ItemUnit*	GetItemById(UINT nId);
	///��ȡװ����������
	const char* GetTypeName(UINT nType);
	///��ȡItemģ��clone
	void GetItemNodeClone(ItemUnit* pItem,NiNodePtr& pClone);
	///��ȡItem��ͼclone
	void GetItemTexClone(ItemUnit* pItem,NiSourceTexturePtr& pTexClone);
	///����ĳ����ҵ�Avatar��Ϣ���ڽ����̵�򳵿�ǰ���ݣ��Ա�ظ��������װ��
	void BackupUserInfo(UINT dwUin);
	///�ָ�ĳ����ҵ�Avatar��Ϣ�����̵�򳵿ⷵ��ʱ�ָ�
	void RestoreUserInfo(UINT dwUin);
	///����װ���洢��Ԫ�����δ����ش洢���Զ���ӣ���dwUin���û�QQ�ţ�nItemId��Item��ͳһid
	///�ɹ�����ItemStoreָ��
	ItemStore* GetItemStore(ULONG dwUin,UINT nItemId);
	///����ItemStore�����ض���ָ�룬bAddBackup=true��ͬʱ���������б�
	ItemStore* AddItemStore(ULONG dwUin,STPlayerDBItemInfo* pItem,bool bAddBackup);
	///����ĳ����ң�ָ���������͵ĵ�ǰװ��Itemʵ��
	ItemStore* GetCurrEquipStore(ULONG dwUin,UINT nType);
	///������е����е��߰�
	bool CleanAllBind(void);
	///����û�������Ϣ
	bool CleanUserInfo(UINT dwUin);
	///���ĳ���û�������avtar���ݣ��������û�������Ϣ
	void CleanMainAvatarData(UINT dwUin);
	///��ȡ��������б�
	GarageTypeObj* GetGarageTypeObj(UINT dwUin,UINT nGarageType);
	///����Ϸ���������ػ���ʾ��ɫ�Ȳ��ĽŲ�λ��bShowFlag=true ��ʾ��=false ����
	void ShowRolePart(Play* pPlay,bool bShowFlag);
protected:
	CItemDataMgr(void);
	virtual ~CItemDataMgr(void);
	///��ȡָ���û��ŵ�ָ�����͵�ʵ�壬ʧ�ܷ���NULL
	ItTypeObj* GetTypeObj(ULONG dwUin,UINT nHitchObjId);
	///��ȡ�ҽ�ʵ�������Itemʵ��
	ItemStore* GetObjItStore(ItTypeObj* pObj,UINT nId);
	///��ȡ�û�Avatar����Ԫָ��
	ItPlayerUint* GetPlayerByUin(ULONG dwUin);
	///��ȡָ��Type�ҽӵ����йҽӵ�item id��ʧ�ܷ���0
	UINT GetItemOnHitch(ItPlayerUint* pPlayUn,UINT nType);
	///����ָ��Type�ҽӵ����йҽӵ�item id
	void SetItemOnHitch(ItPlayerUint* pPlayUn,UINT nType,UINT nItemId);
	///��ȡָ��Type�ҽӵ��Ĭ�Ϲҽ�item id
	UINT GetDefaultOnHitch(UINT nType);
	///��ȡ�ƶ�Type��Nif�ļ�����·��
	void GetNifFilePath(UINT nType,CStringEx& strName);
	///�滻���������ݵ�ģ�ͣ����ñ�������pNewNode���ݻ�ȱʧ��������ʹ��
	bool ChangeShapeWithSkin(NiNode* pOriNode,NiNode* pNewNode,const char* cpShapeName);
private:
	///����Item����Դ���ɹ�����true
	bool LoadItemRes(ItemUnit* pItem);
	///����״̬��ָ����ҽ���װ��
	bool BindItem(Play* pPlay,ItemStore* pStore,bool bEquip);
	/////////////////////////////////////
	ITEMHITCH_MAP	m_maHitchList;	///�ҽӵ�
	ITEMUNIT_MAP	m_maItemList;	///װ��
	ITEMPLAYER_MAP	m_maPlayerList;	///ÿ�������ӵ��ʵ���б����û�����(QQ����)Ϊ����
};
