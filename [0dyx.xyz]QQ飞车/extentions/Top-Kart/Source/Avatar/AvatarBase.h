//     FileName:    AvatarBase.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar逻辑基类
////////////////////////////////////////////////
#pragma once 
#include "ItemDefine.h"
#include <list>
using namespace std;

class CATWearer;
class CATBehaveBase;
class CATAlgorithmBase;
//CAvatarBase主要用于表现装备的逻辑属性，
//和装备的挂接显示分离开，通过装备的ID联系起来
class CAvatarBase
{
public:
	CAvatarBase();

	virtual ~CAvatarBase();

	//根据配置生成装备对象
	VOID Create(DWORD ID, AVATARPROPERTY tagPropt);

	//更新
	VOID Update(float fTime);	

	//返回道具ID
	DWORD GetID();

	//转换道具
	DWORD GPTransform(DWORD dwSource);

	//改变道具作用时间
	DWORD GPLifeTransfrom(DWORD dwCondition, DWORD dwLife);

	//物理属性接口
	PHYSICPROPERTY GetPhysicPropt();

	//改变动作位道具特有动作
	DWORD AnimTransform(DWORD dwSource);

	//是否可以交换道具位置
	BOOL CanSwitch();	

	//是否可以看穿云雾
	BOOL CanThroughFog();

	//触发道具特有表现（行为）
	VOID TriggerBehave(DWORD dwType, DWORD dwOccasion);

	//设置avatar的拥有者
	VOID SetWearer(CATWearer* pWearer);

protected:
	//将behave从活跃队列中转移到非活跃队列中
	VOID DeactivateBehave(CATBehaveBase* pBehave); 

protected:
	CATWearer* m_pWearer;
	DWORD m_dwID;
	BOOL m_bCanSwitch;
	BOOL m_bNoFog;
	PHYSICPROPERTY m_tagPhyPropt;
	std::list<ANIMDATA> m_listAnim;
	std::list<CATBehaveBase*> m_listInactiveBehave;
	std::list<CATBehaveBase*> m_listActiveBehave;
	std::list<CATAlgorithmBase*> m_listGPTranAlgthm;
	std::list<CATAlgorithmBase*> m_listGPLifeAlgthm;
};