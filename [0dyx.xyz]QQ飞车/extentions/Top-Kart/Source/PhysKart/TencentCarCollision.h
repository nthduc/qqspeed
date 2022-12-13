/********************************************************************
	����:	��wincoqin
	ʱ��:	 2007/01/26
	Ŀ��:	 �ƶ�ͳһ�Ļص��ӿ�
*********************************************************************/
#pragma once

class ICollisionCallback;
class NxUserControllerHitReport;


class CCollisionCenter : public NxUserControllerHitReport
{
	typedef std::vector<ICollisionCallback*> CollisionPool;
	typedef CollisionPool::iterator			 CollisionIter;

public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit);
	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit);

	bool RegisterCollisionCallback(ICollisionCallback * pCollisionCallback);
	bool UnRegisterCollisionCallBack(ICollisionCallback * pCollisionCallback);

protected:
	CollisionPool m_vecCollisionPool;
};

