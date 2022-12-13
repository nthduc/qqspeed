#pragma once

/*---------------------------------------------------------------------------------------

Create by Hoverzhao 2006.1.22 

---------------------------------------------------------------------------------------*/
#define INVERSE_MATGLOBAL	\
	NxMat33 InverseMat = m_matGlobal;	\
	if (!InverseMat.getInverse(InverseMat))	\
	{	\
		assert(0);	\
	}

//-----------------------------------------------------------------------
inline void CTencentCar::AddForceLocal( NxVec3 vForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 CarGlobalForce ;
	LocalToGlobal( vForce,CarGlobalForce );
	FORCE CarForce;
	CarForce.vForce = CarGlobalForce;
	CarForce.dwTime  = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}
//-----------------------------------------------------------------------
inline void CTencentCar::AddForceXLocal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( fForce, 0.0f, 0.0f );
	NxVec3 CarGlobalForce;
	LocalToGlobal( vForce ,CarGlobalForce);
	FORCE CarForce;
	CarForce.vForce = CarGlobalForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddForceYLocal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( 0.0f, fForce, 0.0f );
	NxVec3 CarGlobalForce;
	LocalToGlobal( vForce , CarGlobalForce );
	FORCE CarForce;
	CarForce.vForce = CarGlobalForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddForceZLocal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( 0.0f, 0.0f, fForce);
	NxVec3 CarGlobalForce;
	LocalToGlobal( vForce , CarGlobalForce );
	FORCE CarForce;
	CarForce.vForce = CarGlobalForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddForceGlobal( NxVec3 vForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	FORCE CarForce ;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddForceXGlobal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( fForce, 0.0f, 0.0f );
	FORCE CarForce ;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddForceYGlobal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( 0.0f , fForce, 0.0f);
	FORCE CarForce;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddForceZGlobal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( 0.0f, 0.0f, fForce );
	FORCE CarForce;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void CTencentCar::GlobalToLocal( NxVec3 vGlobal, NxVec3 & vLocal )
{
	INVERSE_MATGLOBAL
	vLocal = InverseMat * vGlobal;
}

//-----------------------------------------------------------------------
inline void CTencentCar::LocalToGlobal( NxVec3 vLocal , NxVec3 & vGlobal )
{
	vGlobal = m_matGlobal * vLocal;
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistLocal( NxVec3 vTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwistGlobal;
	LocalToGlobal( vTwist, vTwistGlobal );
	TWIST CarTwist;
	CarTwist.vTwist = vTwistGlobal;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistXLocal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3(fTwist, 0.0f, 0.0f);
	NxVec3 vTwistGlobal;
	LocalToGlobal( vTwist , vTwistGlobal);
	TWIST CarTwist;
	CarTwist.vTwist = vTwistGlobal;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistYLocal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3( 0.0f, fTwist, 0.0f);
	NxVec3 vTwistGlobal;
	LocalToGlobal( vTwist , vTwistGlobal);
	TWIST CarTwist;
	CarTwist.vTwist = vTwistGlobal;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistZLocal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3( 0.0f, 0.0f, fTwist);
	NxVec3 vTwistGlobal;
	LocalToGlobal( vTwist , vTwistGlobal);
	TWIST CarTwist;
	CarTwist.vTwist = vTwistGlobal;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistGlobal( NxVec3 vTwist ,float fTime /* = 0.0f */)
{
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistXGlobal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3(fTwist, 0.0f, 0.0f);
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistYGlobal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3( 0.0f, fTwist, 0.0f );
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void CTencentCar::AddTwistZGlobal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3( 0.0f ,0.0f , fTwist);
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}


//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetForceXLocal()
{
	NxVec3 vForceLocal = NxVec3 ( 0.0f,0.0f,0.0f);
	GlobalToLocal(m_vTotalForce, vForceLocal);
	return vForceLocal.x;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetForceYLocal()
{
	NxVec3 vForceLocal = NxVec3 ( 0.0f, 0.0f, 0.0f );
	GlobalToLocal(m_vTotalForce, vForceLocal);
	return vForceLocal.y;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetForceZLocal()
{
	NxVec3 vForceLocal = NxVec3 ( 0.0f, 0.0f, 0.0f );
	GlobalToLocal(m_vTotalForce, vForceLocal);
	return vForceLocal.z;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetForceXGlobal()
{
	return m_vTotalForce.x;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetForceYGlobal()
{
	return m_vTotalForce.y;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetForceZGlobal()
{
	return m_vTotalForce.z;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetPosGlobal( NxVec3 vPos )
{
	// fix up by matrix
	// 这儿修正被导弹打时掉到地上去的bug
	//	m_pController->setPosition(vExPos);

	NxExtendedVec3  vCurrPos = m_pController->getPosition();
	NxVec3 vStep = NxVec3(vPos.x - vCurrPos.x, vPos.y - vCurrPos.y, vPos.z - vCurrPos.z);

	NxF32	fSharpness		= 0.8f;
	NxU32	unCollisionFlags;
	m_pController->move(
		vStep, 
		PhysXManager::CG_ENVIRONMENT, 
		0.001f, 
		unCollisionFlags, 
		fSharpness);
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetPosGlobal()
{
	NxExtendedVec3  vExPos = m_pController->getPosition();
	return NxVec3( vExPos.x, vExPos.y, vExPos.z);
}

inline NxVec3 CTencentCar::GetBackPosGlobal()
{
	NxExtendedVec3  vExPos = m_pController->getPosition();
	NxVec3 globalPos = NxVec3( vExPos.x, vExPos.y, vExPos.z);
	NxVec3 LocalPos  = NxVec3( 0.0f, 0.0f,0.0f );
	GlobalToLocal(globalPos,LocalPos);
	LocalPos.x -= m_vExtents.y;
	LocalToGlobal(LocalPos,globalPos );
	return globalPos;
}
//-----------------------------------------------------------------------
inline NxMat33 CTencentCar::GetOriGlobal()
{
	return m_matGlobal;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetOriGlobal( NxMat33 matOri)
{
	m_matGlobal = matOri;
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetForwardGlobal()
{
	NxVec3 vForward = m_matGlobal * NxVec3 ( 1.0f, 0.0f, 0.0f ); 
	vForward.normalize();
	return vForward;
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetUpGlobal()
{
	NxVec3 vUp = m_matGlobal * NxVec3 ( 0.0f, 0.0f, 1.0f );
	vUp.normalize();
	return vUp;
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetLeftGlobal()
{
	NxVec3 vLeft = m_matGlobal * NxVec3 ( 0.0f, 1.0f, 0.0f );
	vLeft.normalize();
	return vLeft;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetForwardGlobal( NxVec3 vForward)
{
	m_matGlobal.setColumn(0,vForward);
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetLeftGlobal( NxVec3 vLeft)
{
	m_matGlobal.setColumn(1,vLeft);
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetUpGlobal( NxVec3 vUp )
{
	m_matGlobal.setColumn(2,vUp);
}



//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetVecGlobal()
{
	//if (ECS_FULLCONTACT == GetContactStatus()) 
	//{
	//	NxVec3 kVecLocal = GetVecLocal();
	//	kVecLocal.z = 0;

	//	NxVec3 kVecGlobal;
	//	LocalToGlobal(kVecLocal, kVecGlobal);

	//	return kVecGlobal;
	//}
	//else
        return m_vVecGlobal;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetVecXGlobal()
{
	return m_vVecGlobal.x;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetVecYGlobal() 
{
	return m_vVecGlobal.y;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetVecZGlobal()
{
	return m_vVecGlobal.z;
}
//-----------------------------------------------------------------------
inline void CTencentCar::SetVecGlobal( NxVec3 vVec )
{
	m_vVecGlobal = vVec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetVecXGlobal( NxReal fVec )
{
	m_vVecGlobal.x = fVec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetVecYGlobal( NxReal fVec )
{
	m_vVecGlobal.y = fVec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetVecZGlobal( NxReal fVec )
{
	m_vVecGlobal.z = fVec;
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetVecLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vVecLocal = InverseMat * m_vVecGlobal;
	return vVecLocal;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetVecXLocal()
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal =  InverseMat * m_vVecGlobal;
	return vVecLocal.x;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetVecYLocal()
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal =  InverseMat * m_vVecGlobal;
	return vVecLocal.y;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetVecZLocal()
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal =  InverseMat * m_vVecGlobal;
	return vVecLocal.z;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetVecLocal( NxVec3 vVec)
{
	m_vVecGlobal = m_matGlobal * vVec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetVecXLocal( NxReal fVec )
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal		=  InverseMat * m_vVecGlobal;
	vVecLocal.x		=  fVec;
	m_vVecGlobal	=  m_matGlobal * vVecLocal;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetVecYLocal( NxReal fVec )
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal		=  InverseMat * m_vVecGlobal;
	vVecLocal.y		=  fVec;
	m_vVecGlobal	=  m_matGlobal * vVecLocal;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetVecZLocal( NxReal fVec)
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal		=  InverseMat * m_vVecGlobal;
	vVecLocal.z		=  fVec;
	m_vVecGlobal	=  m_matGlobal * vVecLocal;
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetWecGlobal()
{
	return m_vWecGlobal;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetWecXGlobal()
{
	return m_vWecGlobal.x;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetWecYGlobal()
{
	return m_vWecGlobal.y;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetWecZGlobal()
{
	return m_vWecGlobal.z;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecGlobal( NxVec3 vWec )
{
	m_vWecGlobal = vWec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecXGlobal( NxReal fWec )
{
	m_vWecGlobal.x = fWec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecYGlobal( NxReal fWec )
{
	m_vWecGlobal.y = fWec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecZGlobal( NxReal fWec )
{
	m_vWecGlobal.z = fWec;
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetWecLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetWecXLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal.x;

}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetWecYLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal.y;
}

//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetWecZLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal.z;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecLocal( NxVec3 vWec )
{
	INVERSE_MATGLOBAL
	m_vWecGlobal =  InverseMat * vWec;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecXLocal( NxReal fWec )
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	vWecLocal.x = fWec;
	m_vWecGlobal = m_matGlobal * vWecLocal ;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecYLocal( NxReal fWec )
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	vWecLocal.y = fWec;
	m_vWecGlobal = m_matGlobal * vWecLocal ;
}

//-----------------------------------------------------------------------
inline void CTencentCar::SetWecZLocal( NxReal fWec )
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	vWecLocal.z = fWec;
	m_vWecGlobal = m_matGlobal * vWecLocal ;
}

//-----------------------------------------------------------------------
inline NxVec3 CTencentCar::GetTerrainNormal()
{

	// 这样写很垃圾, 如果第4个轮子不着地又怎么办呢?, 这个值它还能比较对吗?
	//return m_arrCarWheelPool[3]->GetContactNormal();
	// 重写如下

	for (int i = 0; i < 4; ++i)
	{
		if (m_arrCarWheelPool[i]->GetContactStatus() == CCarWheel::ENS_NEAR) 
		{
			return m_arrCarWheelPool[i]->GetContactNormal();
		}
	}

	return NxVec3(0, 0, 1);
	/*
	NxShape * pShape = m_arrCarWheelPool[3]->GetShootShape();
	if (pShape)
	{
		// 这样写很垃圾, 如果第4个轮子不着地又怎么办呢?, 这个值它还能比较对吗?
		//return m_arrCarWheelPool[3]->GetContactNormal();
		// 重写如下

		for (int i = 0; i < 4; ++i)
		{
			if (m_arrCarWheelPool[i]->GetContactStatus() == ENS_NEAR) 
			{
				return m_arrCarWheelPool[i]->GetContactNormal();
			}
		}

		return NxVec3(0, 0, 1);
	}
	else
	{
		return NxVec3(0.0f, 0.0f, 1.0f);
	}
*/
}

//-----------------------------------------------------------------------
inline void   CTencentCar::CheckLock()
{
	if ( ( m_eLockStatus & ELM_MOVE_XAXIS) )
	{
		SetVecXLocal(0.0f);
	}
	if ( ( m_eLockStatus & ELM_MOVE_YAXIS) )
	{
		SetVecYLocal(0.0f);
	}
	if ( ( m_eLockStatus & ELM_MOVE_ZAXIS) )
	{
		SetVecZLocal(0.0f);
	}
	if ( ( m_eLockStatus & ELM_ROTATE_XAXIS) )
	{
		SetWecXLocal(0.0f);
	}
	if ( ( m_eLockStatus & ELM_ROTATE_YAXIS) )
	{
		SetWecYLocal(0.0f);
	}
	if ( ( m_eLockStatus & ELM_ROTATE_ZAXIS) )
	{
		SetWecZLocal(0.0f);
	}
	
}

//-----------------------------------------------------------------------
inline int CTencentCar::GetCarMotionStatus()
{
	int nReturn = 0;
	NxVec3 vGlobalVel	= GetVecGlobal() ; 
	
	// 车身停止
	if ( vGlobalVel.magnitude() < 1.0E-06 )
	{
		return nReturn | ECMD_STOP;
	}

	NxReal fAngle = GetAngleBetweenForwardAndVel();
	
	if ( fAngle > NxPi / 2.0f )
	{
		nReturn = nReturn | ECMD_BACK;
	}
	else
	{
		nReturn = nReturn | ECMD_FORWARD;
	}

/*
	// 普通装态下是这么处理的
	// 漂移的情况为处理　会比较复杂
	NxReal fWec = BornWecFromVec(GetVecGlobal());

	if ( fWec > 0.0f )
	{
		nReturn = nReturn | ECMD_LEFT;
	}
	else if ( fWec < 0.0f )
	{
		nReturn = nReturn | ECMD_RIGHT;
	}
*/

	return nReturn;
}

//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
inline NxReal CTencentCar::GetAngleBetweenForwardAndVel()
{
	NxVec3 vGlobalVel	= GetVecGlobal() ; 
	NxVec3 vForward		= GetForwardGlobal();
	
	// 车身停止
	if ( vGlobalVel.magnitude() < 1.0E-06 )
	{
		return 0.0f;
	}

	vForward.normalize();
	vGlobalVel.normalize();

	NxReal fCita = NxMath::acos( vForward.dot(vGlobalVel));
/*
	vForward.normalize();
	vGlobalVel.normalize();

	NxVec3 vAxis = vForward.cross(vGlobalVel);
	vAxis.normalize();

	NxMat33 rotZ;
	rotZ.rotZ(fCita);
	NxVec3 vTestGlobalVel = vForward * rotZ;


	NxMat33 TestRotate;
	TestRotate.
*/
	return fCita;
}


//-----------------------------------------------------------------------
inline NxReal GetVecAngle ( NxVec3 v1, NxVec3 v2)
{
	v1.normalize();
	v2.normalize();
	NxReal fCita = NxMath::acos( v1.dot(v2));
	return fCita;
}

// 可能有问题的代码  
// 物理旋转逆时针为正 Gamebryo顺时针为正
//-----------------------------------------------------------------------
inline void RotateVecWorld( NxVec3 & v1, NxVec3 vAxis , NxReal fAngle)
{
	NxMat33 matRot;
	NiMatrix3 nimatRot;
	nimatRot.MakeRotation(-fAngle , NiPoint3(vAxis.x, vAxis.y, vAxis.z));
	NiPhysXTypes::NiMatrix3ToNxMat33(nimatRot, matRot);
	v1 = matRot * v1;
}

inline void RotateMatWorld( NxMat33 & mat, NxVec3 vAxis, NxReal fAngle)
{
	NxMat33 matRot;
	NiMatrix3 nimatRot;
	nimatRot.MakeRotation(-fAngle , NiPoint3(vAxis.x, vAxis.y, vAxis.z));
	NiPhysXTypes::NiMatrix3ToNxMat33(nimatRot, matRot);
	mat = mat * matRot;
}


//-----------------------------------------------------------------------
inline NxReal GetAngleBetweenVec3( NxVec3 v1 , NxVec3 v2 )
{
	v1.normalize();
	v2.normalize();

	NxReal fCita = NxMath::acos( v1.dot(v2));

	NxVec3 vAxis = v2.cross(v1);

	if ( vAxis.z > 0)
		return fCita;
	else
		return -fCita;

}


inline NxReal CTencentCar::GetPlaneVecSize()
{
	NxVec3 vLocalVec = GetVecLocal();
	vLocalVec.z = 0.0f;
	return vLocalVec.magnitude();
}


inline NxReal CTencentCar::GetCarHeadAngle (bool bAbs /* = false */, bool bRui /* = fasle */)
{
	NxVec3 vLocalVec	= GetVecLocal();
	vLocalVec.z = 0.0f;
	NxReal fAngle		= GetAngleBetweenVec3 
		( NxVec3( 1.0f, 0.0f , 0.0f) , vLocalVec);

	NxReal fAbsAngle	= NxMath::abs( fAngle );
	NxReal fAbsRuiAngle = 0.0f;

	if ( fAbsAngle > NxPi/2.0f)
		fAbsRuiAngle	= NxPi - fAbsAngle;
	else
		fAbsRuiAngle = fAbsAngle;

	if ( bRui ) 
		return fAbsRuiAngle;

	if ( bAbs )
		return fAbsAngle;

	return fAngle;
}

//-----------------------------------------------------------------------

/// vAxis 旋转轴向   vPoint 旋转起始点的位置 ( 相对 mat 的位置 ) 
inline void RotateMatPointWorld( NxMat34 & Orimat , NxVec3 vAxis , NxVec3 vPoint , NxReal fAngle )
{
	NxMat33 FirstMat;
	vAxis.normalize();

	NxFindRotationMatrix(vAxis, NxVec3(0.0f,0.0f,1.0f) ,FirstMat );

	NxMat34 matTrans ;
	matTrans.M = FirstMat;
	matTrans.t = -vPoint;

	NxMat34 matrTrans;
	matTrans.getInverse(matrTrans);

	Orimat = matTrans * Orimat     ;
         
}

inline void RotateMatWorldZAxis ( NiMatrix3 & mat , NxReal fAngle )
{
	NiMatrix3 matRot;
	matRot.MakeZRotation(fAngle);
	mat = mat * matRot;
}
//-----------------------------------------------------------------------
inline void RotateVecZAxis( NxVec3 &v1, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotZ(fAngle);
	v1 = matRot * v1;
}

//-----------------------------------------------------------------------
inline void RotateVecXAxis( NxVec3 &v1, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotX(fAngle);
	v1 = matRot * v1;
}

//-----------------------------------------------------------------------
inline void RotateVecYAxis( NxVec3 &v1, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotY(fAngle);
	v1 = matRot * v1;
}

//-----------------------------------------------------------------------

inline void RotateMat33ZAxis( NxMat33 & mat, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotZ(fAngle);
	mat = mat * matRot;
}

//-----------------------------------------------------------------------
inline void RotateMat33XAxis ( NxMat33 & mat, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotX(fAngle);
	mat = mat * matRot;
}

//-----------------------------------------------------------------------
inline void RotateMat33YAxis( NxMat33 & mat, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotY(fAngle);
	mat = mat * matRot;
}
//-----------------------------------------------------------------------









