////////////////////////////////////////////////
//FileName:    ItemUpdateMgr.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatarͼ���߼�������
////////////////////////////////////////////////

#pragma once
#include "ItemDefine.h"
#include "ATWearer.h"
#include "AvatarBase.h"

class CItemUpdateMgr:
	public NiMemObject
{
	friend class CAvatarMgr;
	friend class CItemDataMgr;
protected:
	CItemUpdateMgr(void);
	virtual ~CItemUpdateMgr(void);

public:
	//����
	void Update(float fDeltaTime);

	//��wearer����avatar
	void AddAvatar(DWORD dwUin, DWORD dwATID);

	//��wearer����avatar
	void AddAvatar(CATWearer* pWearer,ItemUnit tagUnit);

};
