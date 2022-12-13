////////////////////////////////////////////////
//FileName:    AvatarMgr.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatar������
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
	///��ȡ����������
	static CAvatarMgr* Get();
	///��ʼ��������
	static void Create();
	///������������ͷ�������Դ
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
	///������µĲ���OpId
	UINT GetNewOpId();
	///Item���ݹ���
	static CItemDataMgr* m_pItemDataMgr;
	///Itemͼ���߼�����
	static CItemUpdateMgr* m_pItemUpdateMgr;
	///
	static CAvatarMgr*	ms_pAvatarMgr;
	UINT				m_nOpId;		///�ڲ���¼UseItem()�ɹ������Զ�����ID
public:
	///����
	void Update(float fDeltaTime);
	///����(ʹ��)װ����pQuery���û�QQ�ź���Ҫ������״̬��item�б�,
		///bEquip�Ǵ���ģʽ��true����ʽװ�������ش������б�falseֻ��ѯ�����б���
		///�ı�itemװ������ʵ״̬���������緢��ʱ�õ������б�
		///�ɹ�����true
	bool ChangeItemStatus(CRequestChangeItemStatus* pQueryInOut,bool bEquip);
	///����ҽ��е�һ����Դװ��������nif�󶨵������Դ��dwUin���û�QQ��
	bool BindRes1st(ULONG dwUin);
	///ͨ��Id��ȡitem����
	ItemUnit*	GetItemInfoById(UINT nId);
	///��ѯָ���û������Avatarװ����Ϣ����������뿴FindPlayerAvatarInfo�ṹ
	bool GetPlayerItemList(QueryPlayerAvatarInfo* pList);
	///����Item Id����ȡItem�Ļ������ͣ���enmItBaseType
	UINT GetItemBaseType(UINT nItemId);
 };
