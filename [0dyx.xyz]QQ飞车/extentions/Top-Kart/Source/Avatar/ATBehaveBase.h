////////////////////////////////////////////////
//     FileName:    ATBehaveBase.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar特有表现
////////////////////////////////////////////////

#pragma once 
#include "ItemDefine.h"
#include "ATWearer.h"


//装备特有的表形基类
class CATBehaveBase
{
public:
	CATBehaveBase(BEHAVEDATA tagBD);
	virtual ~CATBehaveBase();
	virtual void Start();				//开始表现
	virtual void Stop();				//停止表现
	virtual void Update(float fTime);	//更新
	void SetWearer(CATWearer* pTarget);	//设置佩戴者（目标对象）
	void SetLifeTime(DWORD dwLift);		//设置生命期
	void SetOccasion(DWORD dwOccasion);	//设置触发时机
	void SetCondition(DWORD dwCondt);	//设置触发条件
	void SetParam(DWORD dwParam);		//设置额外参数

	DWORD GetType();
	DWORD GetOccasion();				//返回触发时机
	DWORD GetConditioin();				//返回触发条件
	bool IsActive();					//是否激活

protected:
	CATWearer* m_pWearer;
	DWORD m_dwType;			//类型
	DWORD m_dwOccasion;		//触发时机
	DWORD m_dwCondition;	//触发条件
	DWORD m_dwLife;			//生命长度
	DWORD m_dwParam;		//参数
	DWORD m_dwOption;		//自由参数
	DWORD m_dwStartTime;	//开始时间
	bool  m_bIsAcive;		//是否处于激活状态	
};


