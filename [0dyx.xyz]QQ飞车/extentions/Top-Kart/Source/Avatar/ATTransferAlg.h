////////////////////////////////////////////////
//     FileName:    ATTransferAlg.h
//     Author:      Rayhuang
//     Date:        2007-4-20
//     Description: avatar变换算法
////////////////////////////////////////////////
#pragma once 
#include "ATAlgorithmBase.h"


//道具攻击时以一定概率转换道具
class CATTransferAlg : public CATAlgorithmBase
{
public:
	CATTransferAlg(ALGDATA tagALGD);
	virtual ~CATTransferAlg();

	virtual DWORD Calculate(DWORD dwParam);
};



//道具攻击时以一定概率减少道具作用时间
class CATTimeTranAlg : public CATAlgorithmBase
{
public:
	CATTimeTranAlg(ALGDATA tagAlGD);
	virtual ~CATTimeTranAlg();

	virtual DWORD Calculate(DWORD dwParam);
};