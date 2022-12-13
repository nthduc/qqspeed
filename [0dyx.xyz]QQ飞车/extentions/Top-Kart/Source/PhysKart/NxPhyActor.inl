#pragma once

				/*---------------------------------------------------------------------------------------

										Create by Hoverzhao 2006.4.25

				---------------------------------------------------------------------------------------*/

#define INVERSE_MATGLOBAL	\
	NxMat33 InverseMat = m_matGlobal;	\
	if (!InverseMat.getInverse(InverseMat))	\
	{	\
		assert(0);	\
	}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddForceLocal( NxVec3 vForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
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
inline void NxPhyActor::AddForceXLocal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
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
inline void NxPhyActor::AddForceYLocal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
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
inline void NxPhyActor::AddForceZLocal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
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
inline void NxPhyActor::AddForceGlobal( NxVec3 vForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	FORCE CarForce ;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddForceXGlobal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( fForce, 0.0f, 0.0f );
	FORCE CarForce ;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddForceYGlobal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( 0.0f , fForce, 0.0f);
	FORCE CarForce;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddForceZGlobal( NxReal fForce ,int eType /* = 0 */ ,float fTime /* = 0.0f */)
{
	NxVec3 vForce = NxVec3 ( 0.0f, 0.0f, fForce );
	FORCE CarForce;
	CarForce.vForce = vForce;
	CarForce.dwTime = fTime * 1000.0f;
	CarForce.eType  = eType;
	m_arrForcePool.push_back( CarForce );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::GlobalToLocal( NxVec3 vGlobal, NxVec3 & vLocal )
{
	INVERSE_MATGLOBAL
	vLocal = InverseMat * vGlobal;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::LocalToGlobal( NxVec3 vLocal , NxVec3 & vGlobal )
{
	vGlobal = m_matGlobal * vLocal;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddTwistLocal( NxVec3 vTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwistGlobal;
	LocalToGlobal( vTwist, vTwistGlobal );
	TWIST CarTwist;
	CarTwist.vTwist = vTwistGlobal;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddTwistXLocal( NxReal fTwist ,float fTime /* = 0.0f */)
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
inline void NxPhyActor::AddTwistYLocal( NxReal fTwist ,float fTime /* = 0.0f */)
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
inline void NxPhyActor::AddTwistZLocal( NxReal fTwist ,float fTime /* = 0.0f */)
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
inline void NxPhyActor::AddTwistGlobal( NxVec3 vTwist ,float fTime /* = 0.0f */)
{
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddTwistXGlobal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3(fTwist, 0.0f, 0.0f);
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddTwistYGlobal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3( 0.0f, fTwist, 0.0f );
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::AddTwistZGlobal( NxReal fTwist ,float fTime /* = 0.0f */)
{
	NxVec3 vTwist = NxVec3( 0.0f ,0.0f , fTwist);
	TWIST CarTwist;
	CarTwist.vTwist = vTwist;
	CarTwist.dwTime = fTime * 1000.0f;
	m_arrTwistPool.push_back( CarTwist );
}


//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetForceXLocal()
{
	NxVec3 vForceLocal = NxVec3 ( 0.0f,0.0f,0.0f);
	GlobalToLocal(m_vTotalForce, vForceLocal);
	return vForceLocal.x;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetForceYLocal()
{
	NxVec3 vForceLocal = NxVec3 ( 0.0f, 0.0f, 0.0f );
	GlobalToLocal(m_vTotalForce, vForceLocal);
	return vForceLocal.y;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetForceZLocal()
{
	NxVec3 vForceLocal = NxVec3 ( 0.0f, 0.0f, 0.0f );
	GlobalToLocal(m_vTotalForce, vForceLocal);
	return vForceLocal.z;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetForceXGlobal()
{
	return m_vTotalForce.x;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetForceYGlobal()
{
	return m_vTotalForce.y;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetForceZGlobal()
{
	return m_vTotalForce.z;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetPosGlobal( NiPoint3 vPos )
{
	NxExtendedVec3 vExPos(vPos.x, vPos.y, vPos.z);
	m_pController->setPosition(vExPos);
	//m_pModel->SetTranslate(vPos.x,vPos.y,vPos.z);
}

//-----------------------------------------------------------------------
inline NiPoint3 NxPhyActor::GetPosGlobal()
{
	NxExtendedVec3  vExPos = m_pController->getPosition();
	return NiPoint3( vExPos.x, vExPos.y, vExPos.z);
	
	//return m_pModel->GetTranslate();
}

//-----------------------------------------------------------------------
inline NxMat33 NxPhyActor::GetOriGlobal()
{
	return m_matGlobal;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetOriGlobal( NxMat33 matOri)
{
	m_matGlobal = matOri;
}

//-----------------------------------------------------------------------
inline NiPoint3 NxPhyActor::GetForwardGlobal()
{
	NxVec3 vForward = m_matGlobal * NxVec3 ( 1.0f, 0.0f, 0.0f ); 
	vForward.normalize();
	NiPoint3 vBForward = NiPoint3 ( vForward.x, vForward.y, vForward.z);
	return   vBForward;
}

//-----------------------------------------------------------------------
inline NiPoint3 NxPhyActor::GetUpGlobal()
{
	NxVec3 vUp = m_matGlobal * NxVec3 ( 0.0f, 0.0f, 1.0f );
	vUp.normalize();
	NiPoint3 vBUp = NiPoint3( vUp.x,vUp.y,vUp.z);
	return vBUp;
}

//-----------------------------------------------------------------------
inline NiPoint3 NxPhyActor::GetLeftGlobal()
{
	NxVec3 vLeft = m_matGlobal * NxVec3 ( 0.0f, 1.0f, 0.0f );
	vLeft.normalize();
	NiPoint3 vBLeft = NiPoint3( vLeft.x, vLeft.y, vLeft.z );
	return vBLeft;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetForwardGlobal( NiPoint3 vForward)
{
	NxVec3 vXForward = NxVec3( vForward.x, vForward.y, vForward.z );
	m_matGlobal.setColumn(0,vXForward);
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetLeftGlobal( NiPoint3 vLeft)
{
	NxVec3 vXLeft = NxVec3 ( vLeft.x , vLeft.y, vLeft.z );
	m_matGlobal.setColumn(1,vXLeft);
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetUpGlobal( NiPoint3 vUp )
{
	NxVec3 vXUp = NxVec3 ( vUp.x, vUp.y, vUp.z);
	m_matGlobal.setColumn(2,vXUp);
}



//-----------------------------------------------------------------------
inline NxVec3 NxPhyActor::GetVecGlobal()
{
        return m_vVecGlobal;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetVecXGlobal()
{
	return m_vVecGlobal.x;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetVecYGlobal() 
{
	return m_vVecGlobal.y;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetVecZGlobal()
{
	return m_vVecGlobal.z;
}
//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecGlobal( NxVec3 vVec )
{
	m_vVecGlobal = vVec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecXGlobal( NxReal fVec )
{
	m_vVecGlobal.x = fVec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecYGlobal( NxReal fVec )
{
	m_vVecGlobal.y = fVec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecZGlobal( NxReal fVec )
{
	m_vVecGlobal.z = fVec;
}

//-----------------------------------------------------------------------
inline NxVec3 NxPhyActor::GetVecLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vVecLocal = InverseMat * m_vVecGlobal;
	return vVecLocal;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetVecXLocal()
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal =  InverseMat * m_vVecGlobal;
	return vVecLocal.x;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetVecYLocal()
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal =  InverseMat * m_vVecGlobal;
	return vVecLocal.y;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetVecZLocal()
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal =  InverseMat * m_vVecGlobal;
	return vVecLocal.z;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecLocal( NxVec3 vVec)
{
	m_vVecGlobal = m_matGlobal * vVec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecXLocal( NxReal fVec )
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal		=  InverseMat * m_vVecGlobal;
	vVecLocal.x		=  fVec;
	m_vVecGlobal	=  m_matGlobal * vVecLocal;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecYLocal( NxReal fVec )
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal		=  InverseMat * m_vVecGlobal;
	vVecLocal.y		=  fVec;
	m_vVecGlobal	=  m_matGlobal * vVecLocal;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetVecZLocal( NxReal fVec)
{
	NxVec3 vVecLocal;
	INVERSE_MATGLOBAL
	vVecLocal		=  InverseMat * m_vVecGlobal;
	vVecLocal.z		=  fVec;
	m_vVecGlobal	=  m_matGlobal * vVecLocal;
}

//-----------------------------------------------------------------------
inline NxVec3 NxPhyActor::GetWecGlobal()
{
	return m_vWecGlobal;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetWecXGlobal()
{
	return m_vWecGlobal.x;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetWecYGlobal()
{
	return m_vWecGlobal.y;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetWecZGlobal()
{
	return m_vWecGlobal.z;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecGlobal( NxVec3 vWec )
{
	m_vWecGlobal = vWec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecXGlobal( NxReal fWec )
{
	m_vWecGlobal.x = fWec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecYGlobal( NxReal fWec )
{
	m_vWecGlobal.y = fWec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecZGlobal( NxReal fWec )
{
	m_vWecGlobal.z = fWec;
}

//-----------------------------------------------------------------------
inline NxVec3 NxPhyActor::GetWecLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetWecXLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal.x;

}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetWecYLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal.y;
}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetWecZLocal()
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	return vWecLocal.z;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecLocal( NxVec3 vWec )
{
	INVERSE_MATGLOBAL
	m_vWecGlobal =  InverseMat * vWec;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecXLocal( NxReal fWec )
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	vWecLocal.x = fWec;
	m_vWecGlobal = m_matGlobal * vWecLocal ;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecYLocal( NxReal fWec )
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	vWecLocal.y = fWec;
	m_vWecGlobal = m_matGlobal * vWecLocal ;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::SetWecZLocal( NxReal fWec )
{
	INVERSE_MATGLOBAL
	NxVec3 vWecLocal = InverseMat * m_vWecGlobal;
	vWecLocal.z = fWec;
	m_vWecGlobal = m_matGlobal * vWecLocal ;
}


//-----------------------------------------------------------------------
inline void NxPhyActor::UpdateVec(float fTime)
{
	float fDeltaTime = fTime;
	NxVec3 vTotalForce = GetTotalForce();
	NxVec3 vVecGlobal  = GetVecGlobal();
	vVecGlobal += fDeltaTime * vTotalForce / m_fMass ;
	SetVecGlobal( vVecGlobal );
}

//-----------------------------------------------------------------------
inline void NxPhyActor::UpdateWec(float fTime)
{

	float	fDeltaTime			= fTime ;
	NxVec3	vTotalTwist			= GetTotalTwist();
	NxVec3	vLocalTotalTwist	= NxVec3( 0.0f, 0.0f, 0.0f );
	GlobalToLocal( vTotalTwist, vLocalTotalTwist);

	NxVec3 vWecLocal			= GetWecLocal();

	// 这里使用 柱状刚体 角加速度 的计算方式
	vWecLocal += vLocalTotalTwist * fDeltaTime;

	if ( vWecLocal.magnitude() < 1.0e-03)
	{
		vWecLocal = NxVec3( 0.0f, 0.0f, 0.0f );
	}

	SetWecLocal ( vWecLocal );

}
//-----------------------------------------------------------------------
inline void NxPhyActor::TriggerForceTwistPool()
{
	m_vTotalForce = NxVec3 ( 0.0f, 0.0f, 0.0f);
	m_vTotalTwist = NxVec3 ( 0.0f, 0.0f, 0.0f);

	bool bOnlyOne = true;
	for ( int i = 0; i < m_arrForcePool.size(); ++i)
	{
		if ( m_arrForcePool[i].eType == EFT_ONLYONE )
		{
			if ( bOnlyOne )
			{
				m_vTotalForce += m_arrForcePool[i].vForce;
				bOnlyOne = false;
			}
		}
		else
		{
			m_vTotalForce += m_arrForcePool[i].vForce;
		}
	}

	for ( int j = 0; j < m_arrTwistPool.size(); ++j)
	{
		m_vTotalTwist += m_arrTwistPool[j].vTwist;
	}


	ClearForcePool();
	ClearTwistPool();
}

//-----------------------------------------------------------------------
inline void NxPhyActor::ClearForcePool()
{
	bool bIsEnd = false;

	std::vector<FORCE>::iterator iter = m_arrForcePool.begin();
	while( !bIsEnd )
	{
		bIsEnd = true;
		for(; iter != m_arrForcePool.end(); ++iter)
		{
			if ( (*iter).dwTime < GetDeltaTime()) 
			{
				m_arrForcePool.erase(iter);
				bIsEnd = false;
				break;
			}
			else 
			{
				(*iter).dwTime -= GetDeltaTime();
			}
		}
	}
}

//-----------------------------------------------------------------------
inline void NxPhyActor::ClearTwistPool()
{
	bool bIsEnd = false;

	std::vector<TWIST>::iterator iter = m_arrTwistPool.begin();
	while( !bIsEnd )
	{
		bIsEnd = true;
		for(; iter != m_arrTwistPool.end(); ++iter)
		{
			if ( (*iter).dwTime < GetDeltaTime() ) 
			{
				m_arrTwistPool.erase(iter);
				bIsEnd = false;
				break;
			}
			else 
			{
				(*iter).dwTime -= GetDeltaTime();
			}
		}
	}
}

//-----------------------------------------------------------------------
inline void NxPhyActor::RotateMatWorldZAxis ( NiMatrix3 & mat , NxReal fAngle )
{
	NiMatrix3 matRot;
	matRot.MakeZRotation(fAngle);
	mat = mat * matRot;
}

//-----------------------------------------------------------------------
inline  void NxPhyActor::UpdateLineMovement(float fTime)
{
	NxVec3	vVecGlobal		= GetVecGlobal();

	NxVec3	vStep(0, 0, 0);
	vStep = vVecGlobal * fTime;

	NiPoint3 vPos = GetPosGlobal();

	NxF32	fSharpness		= 1.0f;
	NxU32	unCollisionFlags;

	m_pController->move(
		vStep, 
		PhysXManager::CG_ENVIRONMENT, 
		0.000001f, 
		unCollisionFlags, 
		fSharpness);

	NiPoint3 vNewPos = GetPosGlobal();
	m_pModel->SetTranslate( vNewPos.x, vNewPos.y, vNewPos.z );

}


//-----------------------------------------------------------------------
inline void NxPhyActor::RotateMat33XAxis ( NxMat33 & mat, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotX(fAngle);
	mat = mat * matRot;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::RotateMat33YAxis ( NxMat33 & mat, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotY(fAngle);
	mat = mat * matRot;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::RotateMat33ZAxis ( NxMat33 & mat, NxReal fAngle)
{
	NxMat33 matRot;
	matRot.rotZ(fAngle);
	mat = mat * matRot;
}

//-----------------------------------------------------------------------
inline void NxPhyActor::UpdateTurnMovement(float fTime)
{

	NxVec3	vWecLocal	= GetWecLocal();
	NxMat33	matGlobal   = GetOriGlobal();

	RotateMat33XAxis( matGlobal , vWecLocal.x * fTime);
	RotateMat33YAxis( matGlobal , vWecLocal.y * fTime);
	RotateMat33ZAxis( matGlobal , vWecLocal.z * fTime);
	
	SetOriGlobal(matGlobal);
	NiMatrix3 matModelGlobal;
	NiPhysXTypes::NxMat33ToNiMatrix3( matGlobal,matModelGlobal );
	RotateMatWorldZAxis(matModelGlobal,-NxPi/2.0f);
	m_pModel->SetRotate(matModelGlobal);


	//RotateMatWorldZAxis(matModelGlobal,-NxPi/2.0f);

}

//-----------------------------------------------------------------------
inline void NxPhyActor::Update(float fTime )
{
	if (!m_pModel)
		return;

	AddForceGlobal( NxVec3 ( 0.0f, 0.0f, -9.8f));

	TriggerForceTwistPool();
	
	UpdateVec(fTime);
	UpdateWec(fTime);

	UpdateTurnMovement(fTime);
	UpdateLineMovement(fTime);


	return;
}

////-----------------------------------------------------------------------
//inline NxReal GetAngleBetweenVec3( NxVec3 v1 , NxVec3 v2 )
//{
//	v1.normalize();
//	v2.normalize();
//
//	NxReal fCita = NxMath::acos( v1.dot(v2));
//
//	NxVec3 vAxis = v2.cross(v1);
//
//	if ( vAxis.z > 0)
//		return fCita;
//	else
//		return -fCita;
//
//}

//-----------------------------------------------------------------------
inline NxReal NxPhyActor::GetPlaneVecSize()
{
	NxVec3 vLocalVec = GetVecLocal();
	vLocalVec.z = 0.0f;
	return vLocalVec.magnitude();
}


inline NxReal NxPhyActor::GetHeadAngle (
										bool bAbs /* = false */,
										bool bRui /* = fasle */)

{
	NxVec3 vLocalVec	= GetVecLocal();
	vLocalVec.z = 0.0f;
	NxReal fAngle		= GetAngleBetweenVec3 
		( NxVec3( 1.0f, 0.0f , 0.0f) , 
		  vLocalVec);

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

//
//inline void RotateMatWorldZAxis ( NiMatrix3 & mat , NxReal fAngle )
//{
//	NiMatrix3 matRot;
//	matRot.MakeZRotation(fAngle);
//	mat = mat * matRot;
//}
