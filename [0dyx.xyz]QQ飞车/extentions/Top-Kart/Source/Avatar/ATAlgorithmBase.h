////////////////////////////////////////////////
//     FileName:    ATBehaveBase.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar变换算法基类
////////////////////////////////////////////////

#pragma once 
#include "ItemDefine.h"

class CATAlgorithmBase
{
public:
	CATAlgorithmBase(ALGDATA tagALGD);
	virtual ~CATAlgorithmBase();
	virtual DWORD Calculate(DWORD dwParam);
	void SetType(DWORD dwType);
	void SetOccasion(DWORD dwOccasion);
	void SetOperator(SHORT shOp);
	void SetCondition(DWORD dwCondition);
	void SetExpectation(DWORD dwExp);
	void SetRate(DWORD dwRate);

	DWORD GetType();
	DWORD GetOccasion();
protected:
	DWORD m_dwType;		//类型
	DWORD m_dwOccasion;	//触发时机
	SHORT m_shOpertor;	//算子
	DWORD m_dwCondition;	//触发条件
	DWORD m_dwExpectation;//期望值	
	DWORD m_dwRate;		//得到期望值的概率
};