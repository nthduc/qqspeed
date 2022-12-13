/********************************************************************
作者:	　wincoqin
时间:	  2007/01/30
文件名:   CarFenceCollision.cpp
目的:     处理车-墙碰撞
*********************************************************************/
#pragma once

// libfile

// userfile
#include "CollisionBase.h"

// predefine
class Play;
class CCollisionCenter;
class CTencentCar;

class CCarFenceCollision
	: public CCollisionBase
{
public:
	CCarFenceCollision(CCollisionCenter * pCenter);
	virtual ~CCarFenceCollision();

	virtual void OnTrigger(void * pSelf, void * pOtherInfo, const DWORD dwCollisionType);

protected:
	void ProcessCarFenceCollision(Play * pPlay, CollisionInfo * pCollisionInfo, const DWORD dwCollisionType);
	void applyCounterforce(CTencentCar * pCar, CollisionInfo * pCollisionInfo);

private:
	DWORD m_dwLstTime;

};
