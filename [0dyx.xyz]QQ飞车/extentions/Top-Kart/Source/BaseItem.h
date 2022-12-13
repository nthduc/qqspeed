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
	virtual NiSourceTexturePtr GetItemFigure() = 0;		//�õ����ߵ�ͼ��
	virtual void Use() = 0;							//ʩ�ŵ���
	virtual void BeUsed(void * pData) = 0;			//��ʩ�ŵ���
	
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
	enum enItemType					//��������
	{
		enAccel,
		enItemTypeNum,
	};

public:
	CItemMgr();
	~CItemMgr();

	void UseItem();											//ʹ�õ���

	void UpdateAffected(float ftime);						//������Ӱ�죬����������ҵĵ���

	void PushOwnItem(enItemType type, void * pData);		//�õ����ߣ��ѵ���ѹ��ӵ�е��ߵĶ�����

	void PushAffectedItem(enItemType type, void * pData);	//������ҶԱ������ʩ�ŵĵ��ߣ��ѵ���ѹ�뱻ʹ�õ��ߵĶ�����
	
	void ShowItemsColumn(NiRendererPtr pkRenderer);			//��ʾ������

	void ClearItems();										//�������

	void TurnOffColumnShow();								//����ʾ������
	void TurnOnColumnShow();								//��ʾ������


	static bool Create();									//�������߹����൥��
	static void Destroy();									//���ٵ��߹����൥��
	static CItemMgr * Get();								//�õ����߹����൥��

protected:
	void PopItemFigure();									//�������ߵ����ͼƬ
	void PusnItemFigure(NiSourceTexturePtr pkTex);			//ѹ����ߵ����ͼƬ


private:

	bool m_bIsShowColumn;

	NiScreenElementsPtr m_spItemColumnBase;					//������ͼƬ
	NiScreenElementsPtr m_aspItemScnElt[MAX_OWNITEM_NUM];	//����ͼƬ

	unsigned int m_uiCurOwnItemsNum;						//��ǰӵ�е�����
	std::queue<CBaseItem*> m_pOwnItemsQueue;				//����ӵ�е��ߵĶ���

	std::queue<CBaseItem*> m_pBeUsedItemsQueue;				//���������ʹ�õ��ߵĶ���

	static CItemMgr * ms_pItemMgr;

};


#endif