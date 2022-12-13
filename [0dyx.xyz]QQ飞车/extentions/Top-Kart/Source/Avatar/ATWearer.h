////////////////////////////////////////////////
//     FileName:    ATWearer.h
//     Author:      Rayhuang
//     Date:        2007-4-26
//     Description: avatar拥有者
////////////////////////////////////////////////

#pragma once 
#include "ItemDefine.h"
#include "AvatarBase.h"
#include <list>
using namespace std;

class CAvatarBase;

class CATWearer
{
public:
	//构造函数
	CATWearer();

	//释构函数
	~CATWearer();

	//更新
	VOID Update(float fTime);

	//是否拥有某avatar
	BOOL HaveAT(DWORD dwID);

	//被道具击中，道具转换或作用时间减少
	VOID OnAttackByGP(GPINFO& tagGPInfo);

	//处理特殊的avatar拥有动作
	DWORD AnimTranform(CONST DWORD& dwID);

	//返回物理属性
	PHYSICPROPERTY GetPhysicPropt();

	//增加avatar
	VOID AddAvatar(CAvatarBase* pAvatar);

	////增加avatar
	VOID AddAvatar(const ItemUnit& tagUnit);

	//删除avatar
	VOID RemoveAvatar(DWORD dwID);

protected:
	list<CAvatarBase*> m_listAvatar;
};