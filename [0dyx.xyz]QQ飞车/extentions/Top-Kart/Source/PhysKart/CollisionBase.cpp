/********************************************************************
	作者:	　wincoqin
	时间:	  2007/01/27
    文件名:   CollisionBase.cpp
	目的:   	
*********************************************************************/
#include "Top-KartPCH.h"
// libfile

// userfile
#include "CollisionBase.h"
#include "TencentCarCollision.h"

CCollisionBase::CCollisionBase(CCollisionCenter * pCenter, const DWORD dwCollisionType) 
	: m_pFather(pCenter)
	, m_dwCollisionType(dwCollisionType)
{
	assert(m_pFather);
	bool bResult = m_pFather->RegisterCollisionCallback(this);
	assert(bResult);
}
CCollisionBase::~CCollisionBase()
{
	assert(m_pFather);
	bool bResult = m_pFather->UnRegisterCollisionCallBack(this);
	assert(bResult);
}
