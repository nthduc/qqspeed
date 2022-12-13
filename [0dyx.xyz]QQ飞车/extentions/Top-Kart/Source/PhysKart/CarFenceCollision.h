/********************************************************************
����:	��wincoqin
ʱ��:	  2007/01/30
�ļ���:   CarFenceCollision.cpp
Ŀ��:     ����-ǽ��ײ
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
