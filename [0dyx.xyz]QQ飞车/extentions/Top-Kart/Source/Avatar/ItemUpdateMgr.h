////////////////////////////////////////////////
//FileName:    ItemUpdateMgr.h
//Author:      PigHuang
//Date:        2007-03-13
//Description: Avatar图像、逻辑管理类
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
	//更新
	void Update(float fDeltaTime);

	//向wearer增加avatar
	void AddAvatar(DWORD dwUin, DWORD dwATID);

	//向wearer增加avatar
	void AddAvatar(CATWearer* pWearer,ItemUnit tagUnit);

};
