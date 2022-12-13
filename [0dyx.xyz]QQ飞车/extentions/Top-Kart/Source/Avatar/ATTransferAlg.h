////////////////////////////////////////////////
//     FileName:    ATTransferAlg.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar�任�㷨
////////////////////////////////////////////////
#pragma once 
#include "ATAlgorithmBase.h"


//���߹���ʱ��һ������ת������
class CATTransferAlg : public CATAlgorithmBase
{
public:
	CATTransferAlg(ALGDATA tagALGD);
	virtual ~CATTransferAlg();

	virtual DWORD Calculate(DWORD dwParam);
};



//���߹���ʱ��һ�����ʼ��ٵ�������ʱ��
class CATTimeTranAlg : public CATAlgorithmBase
{
public:
	CATTimeTranAlg(ALGDATA tagAlGD);
	virtual ~CATTimeTranAlg();

	virtual DWORD Calculate(DWORD dwParam);
};