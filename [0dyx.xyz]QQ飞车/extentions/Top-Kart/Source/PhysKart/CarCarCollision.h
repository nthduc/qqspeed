/********************************************************************
	����:	��wincoqin
	ʱ��:	  2007/01/27
    �ļ���:   CarCarCollision.h
	Ŀ��:     ��������֮�����ײ
*********************************************************************/
#pragma once

// libfile


// userfile
#include "CollisionBase.h"

// predefine
class Play;
class CTencentCar;

class CCarCarCollision
	: public CCollisionBase
{	
public:
	CCarCarCollision(CCollisionCenter * pCollisionCenter);
	virtual ~CCarCarCollision();

	virtual void OnTrigger(void * pSelf, void * pOtherInfo, const DWORD dwCollisionType);	
protected:
	void ProcessCarCarCollision(Play * pSelf, Play * pOther);


protected:
	float m_fLastCollisionTime;
};