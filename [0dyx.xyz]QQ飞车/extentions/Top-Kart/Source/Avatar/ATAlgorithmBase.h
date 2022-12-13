////////////////////////////////////////////////
//     FileName:    ATBehaveBase.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar�任�㷨����
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
	DWORD m_dwType;		//����
	DWORD m_dwOccasion;	//����ʱ��
	SHORT m_shOpertor;	//����
	DWORD m_dwCondition;	//��������
	DWORD m_dwExpectation;//����ֵ	
	DWORD m_dwRate;		//�õ�����ֵ�ĸ���
};