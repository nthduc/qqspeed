#include "Top-KartPCH.h"
#include ".\driftcenter.h"
#include "../NxPlayMgr.h"
#include "../NxSceneMgr.h"
#include "../SceneLevel.h"
#include "../KartCrack.h"
#include "../Play.h"


						/*-----------------------------------------------------------------------

											Create by HoverZhao 2007.3.10

						-------------------------------------------------------------------------*/




#define LTOG_VEC(l, g)	\
	m_pCar->LocalToGlobal(l,g);


//-----------------------------------------------------------------------
#define BANNERANGLE			CDriftMath::BannerAngle()		// 40.0f;
#define STARTVEC			CDriftMath::StartVec()
#define ENDVECFIST			CDriftMath::EndVecFist()	
#define ENDVECSECON			CDriftMath::EndVecSecon()



#define WHEELTURNVEC		NxPi / 4.0f
#define MAXWHEELTURNANG		NxPi / 4.0f
#define MINDRIFTRAIDO		4.0f
#define MAXDRIFTRAIDO		5.0f

//-----------------------------------------------------------------------
void CDriftCenter::DriftStart(CTencentCar::KEYINFO stKeyInfo)
{
	if ( GetDriftStatus() == DRIFT_START )
		return;

	m_arrTotalForce.clear();
	m_arrTotalTwist.clear();
	m_arrVecYForce.clear();

	m_arrTimeTwistPool.clear();
	m_arrTimeForcePool.clear();
	//m_stOldKeyInfo	 = stKeyInfo;
	m_bBanner		 = false;
	m_bShoot		 = false;
	m_bCollision     = false;
	m_bCreateNewTrack = true;
	m_fVecTurnWec  = 0.0f;

	m_pTimeControl->ActiveTimer(0.6f);
	m_fVecDriftStart = CaculatePlaneVecSize();
	m_fOldAngle      = GetHeadAngle(true, true);


	NxReal fStartWec = CDriftMath::StartWec();

	m_fDriftRadio    = MAXDRIFTRAIDO;

	m_vDriftWecLocal = m_pCar->GetWecLocal();

	if ( stKeyInfo.bLeft )
		SetLDWZ( fStartWec);
	if ( stKeyInfo.bRight )
		SetLDWZ(-fStartWec);

	m_nOldDriftContactStatus = m_pCar->GetContactStatus();
	SetDriftStatus( DRIFT_START );

}

//-----------------------------------------------------------------------
void CDriftCenter::DriftEnd( bool bIsNormal /* = true  */,bool bIsCutVec /* = true */)
{
	if ( GetDriftStatus() == DRIFT_NONE )
		return;

	if ( bIsCutVec )
		m_pCar->SetVecLocal(NxVec3( m_vDriftVecLocal.x, 0.0f, m_vDriftVecLocal.z));

	m_pCar->SetWecLocal(NxVec3(0.0f,0.0f,0.0f));

	m_fVecDriftStart = 0.0f;
	SetDriftStatus( DRIFT_NONE );
	m_bCreateNewTrack = true; 


	if ( bIsNormal && !IsCollision())
		m_pCar->SetXiaoPen(true);

	m_bCollision = false;
	m_fVecTurnWec = 0.0f;
}

//-----------------------------------------------------------------------
void CDriftCenter::Update(  DWORD dwTime, CTencentCar::KEYINFO stKeyInfo )
{

	float fTime = dwTime/1000.0f;

	if ( CanDriftStart( stKeyInfo ))
		DriftStart(stKeyInfo);

	if ( GetDriftStatus() == DRIFT_START )
	{
		DriftLoop( fTime, stKeyInfo );
	}

	m_stOldKeyInfo = stKeyInfo;
}

//-----------------------------------------------------------------------
void CDriftCenter::ModifyDriftBaseParam(DRIFTBASEPARAM stDriftBaseParam)
{
	*m_pDriftBaseParam = stDriftBaseParam;
}


void CDriftCenter::AirDrift(float fTime, CTencentCar::KEYINFO stKeyInfo )
{
	m_vDriftVecLocal = m_pCar->GetVecLocal();
	m_vDriftWecLocal = m_pCar->GetWecLocal();
}

void CDriftCenter::DriftGravity( float fTime)
{
	int nContactStatus = m_pCar->GetContactStatus();
	if ( nContactStatus == CTencentCar::ECS_INAIR ||
		 nContactStatus == CTencentCar::ECS_NONECONTACT  )
	{
		NxVec3 vDriftVecGlobal = NxVec3(0.0f,0.0f,0.0f);
		m_pCar->LocalToGlobal(m_vDriftVecLocal,vDriftVecGlobal) ;
		vDriftVecGlobal.z  += m_pCar->GetGravityValue() * fTime;
		m_pCar->GlobalToLocal(vDriftVecGlobal,m_vDriftVecLocal);
	}
	else
	{   
		NxVec3 vDirDown			= -m_pCar->GetUpGlobal();
		NxVec3 vDriftGravityGlobal = NxVec3( 0.0f,0.0f,m_pCar->GetGravityValue() / 3.0f);
		vDriftGravityGlobal = vDriftGravityGlobal - vDriftGravityGlobal.dot(vDirDown) * vDirDown;
		NxVec3 vDriftGravityLocal = NxVec3( 0.0f, 0.0f, 0.0f );
		m_pCar->GlobalToLocal(vDriftGravityGlobal, vDriftGravityLocal);
		m_vDriftVecLocal += vDriftGravityLocal * fTime;
	}
}
//-----------------------------------------------------------------------
void CDriftCenter::DriftLoop( float fTime ,CTencentCar::KEYINFO stKeyInfo )
{
	m_vDriftVecLocal = m_pCar->GetVecLocal();

	m_vOldDriftVecLocal = m_vDriftVecLocal;


//	LogText("Drift m_vDriftWecLocalZ Old = %f \n" , m_vDriftWecLocal.z );


	CDriftMath::CollectData( fTime, &stKeyInfo );

	PreLoop( fTime, stKeyInfo );

	DriftGravity( fTime );

	DriftForceControl	( fTime, stKeyInfo );
	DriftTwistControl	( fTime, stKeyInfo );
	
	UpdateTimeForceTwistPool( fTime );
	TriggerFT( fTime );

	DriftVecControl		( fTime, stKeyInfo );
	DriftWecControl		( fTime, stKeyInfo );

//	LogText("Drift m_vDriftWecLocalZ New = %f \n" , m_vDriftWecLocal.z );

	CaculateVecTurnWec(m_vDriftVecLocal , m_vOldDriftVecLocal , fTime);
	UpdateMarkAndTrack();

	m_nOldDriftContactStatus = m_pCar->GetContactStatus();


}

#define BANNERANGLE_ADD CDriftMath::BannerAngle() + NxPi * 5.0f / 180.0f
#define BANNERANGLE_DEC CDriftMath::BannerAngle() - NxPi * 5.0f / 180.0f

//-----------------------------------------------------------------------
void CDriftCenter::PreLoop( float fTime, CTencentCar::KEYINFO &stKeyInfo)
{
	NxReal fAngle = GetHeadAngle( true, true );


	NxReal fLeftBannerTwist  =	CDriftMath::BannerTwist();
	NxReal fRightBannerTwist = -CDriftMath::BannerTwist();
	NxReal fDampTwist		 =  CDriftMath::DampTwist();
	NxVec3 fRollFrict		 =  CDriftMath::RollFrict();

	ProcessWec(stKeyInfo);

	if ( fAngle >= BANNERANGLE_ADD  && m_fOldAngle < BANNERANGLE_ADD )	// bug 可能是动态策略
	{
		m_pTimeControl->DeActiveTimer();
		SetBanner(true);
	}

	if ( fAngle < BANNERANGLE_ADD && m_fOldAngle >= BANNERANGLE_ADD)
	{
		m_pTimeControl->ActiveTimer(0.8);
		SetShoot(true);
	}

	if ( fAngle < BANNERANGLE_ADD && m_fOldAngle >= BANNERANGLE_ADD && 
		GetBanner() && !IsCollision())
	{
		m_pCar->SetXiaoPen(true);
		SetBanner(false);
	}

	if ( fAngle < BANNERANGLE_DEC && 
		m_fOldAngle >= BANNERANGLE_DEC )
	{
		if ( !stKeyInfo.bShift || NxMath::abs( GetLDWZ() ) < CDriftMath::ClockWec() )
			DriftEnd(true,false);
	}

	m_fOldAngle= fAngle;

	m_pTimeControl->UpdateTimer();
	m_pKeyEventControl->UpdateKeyEventControl(stKeyInfo);


	if ( stKeyInfo.bLeft  && GetLDWZ() <= 0.0f )
		AddLDTZ( fLeftBannerTwist );

	if ( stKeyInfo.bRight && GetLDWZ() >= 0.0f )
		AddLDTZ( fRightBannerTwist );


	if ( !stKeyInfo.bLeft && !stKeyInfo.bRight )
	{
		if ( NxMath::abs( GetLDWZ() ) >  1.0E-03 )
			AddLDTZ ( -NxMath::sign( GetLDWZ() ) * fDampTwist);
		else
			SetLDWZ(0.0f);
	}


	if ( !stKeyInfo.bLeft && !stKeyInfo.bRight && !stKeyInfo.bUp)
	{
		NxVec3 vVecDir = GetLDV();
		vVecDir.normalize();
		AddLDF( -vVecDir * 3.0f);
	}


	if ( CaculatePlaneVecSize() < VECSCALE( ENDVECFIST ) ) 
		if ( !stKeyInfo.bShift || (!stKeyInfo.bLeft && !stKeyInfo.bRight) )
			if ( m_fVecDriftStart > VECSCALE( ENDVECFIST ) )
				DriftEnd(true);

	if ( CaculatePlaneVecSize() < VECSCALE( ENDVECSECON ) )
		if ( m_fVecDriftStart > VECSCALE( ENDVECFIST ) )
			DriftEnd(true);
		else
			DriftEnd(false);


	if ( GetHeadAngle(true, false) > DGAngle(135.0f) ) 
		DriftEnd();



	// 更新车轮角 应该放到TencentCar 中统一实现
	// 是更新速度方向的重要因素之一
	if ( stKeyInfo.bLeft && 
		m_fWheelTurnAngle < m_fWheelMaxTurnAngle) 
	{
		m_fWheelTurnAngle += m_fWheelTurnVec * fTime;
		m_fWheelTurnAngle  = NxMath::min(m_fWheelTurnAngle,m_fWheelMaxTurnAngle); 
	}

	if ( stKeyInfo.bRight && 
		m_fWheelTurnAngle > -m_fWheelMaxTurnAngle)
	{
		m_fWheelTurnAngle -= m_fWheelTurnVec * fTime;
		m_fWheelTurnAngle  = NxMath::max(m_fWheelTurnAngle, -m_fWheelMaxTurnAngle);
	}
		
			
}

//-----------------------------------------------------------------------
 void CDriftCenter::OnTimerCallBack()
{
	int nContactStatus = m_pCar->GetContactStatus();
	NxReal fAngle = GetHeadAngle( true, true );
	if ( fAngle <= BANNERANGLE && !GetBanner() && nContactStatus != CTencentCar::ECS_INAIR)
	{
		if ( !GetShoot())
			DriftEnd( false ,false);
		else
			DriftEnd( true, false );
	}
}

//-----------------------------------------------------------------------
void CDriftCenter::OnShiftPress()
{
	NxReal fRealAngle = GetHeadAngle();
	
	if ( fRealAngle >= BANNERANGLE + NxPi * 5.0f / 180.0f   && fRealAngle < NxPi/2.0f && m_stOldKeyInfo.bRight )
		  AddTimeTwist(NxVec3( 0.0f ,0.0f ,-15.0f),0.10f);

	if ( fRealAngle <= - (BANNERANGLE + NxPi * 5.0f / 180.0f  )  && fRealAngle > -NxPi/2.0f  && m_stOldKeyInfo.bLeft  )
		  AddTimeTwist(NxVec3( 0.0f ,0.0f , 15.0f),0.10f);
}
//-----------------------------------------------------------------------
void CDriftCenter::Run(  DWORD dwTime,CTencentCar::KEYINFO stKeyInfo )
{
	float fTime = dwTime/1000.0f;

	if ( GetDriftStatus() == DRIFT_NONE ) 
		return ;


	/*if ( m_pCar->GetContactStatus() != CTencentCar::ECS_INAIR )
			m_vDriftVecLocal.z = -4.0f;*/


	m_pCar->SetVecLocal( m_vDriftVecLocal );
	m_pCar->SetWecLocal( m_vDriftWecLocal );

	UpdateTurnMovement(fTime , stKeyInfo);
	UpdateLineMovement(fTime );

}

void CDriftCenter::UpdateTimeForceTwistPool( float fTime)
{
	std::vector<TIMEFORCE>::iterator iterforce = m_arrTimeForcePool.begin();
	std::vector<TIMEFORCE> ArrTimeForcePool;
	ArrTimeForcePool.clear();
	
	for ( ; iterforce != m_arrTimeForcePool.end() ; iterforce++ )
	{
		(*iterforce).fTime -= fTime; 
		if ( (*iterforce).fTime >= 0.0f )
			ArrTimeForcePool.push_back(*iterforce);
	}
	m_arrTimeForcePool.clear(); 
	m_arrTimeForcePool = ArrTimeForcePool;
	iterforce = m_arrTimeForcePool.begin();
	for ( ; iterforce != m_arrTimeForcePool.end() ; iterforce++ )
	{
		AddLDF((*iterforce).fForce);
	}
	std::vector<TIMETWIST>::iterator itertwist = m_arrTimeTwistPool.begin();
	std::vector<TIMETWIST> ArrTimeTwistPool;
	ArrTimeTwistPool.clear();

	for ( ; itertwist != m_arrTimeTwistPool.end(); itertwist++ )
	{
		(*itertwist).fTime -= fTime;
		if ( (*itertwist).fTime >= 0.0f )
			ArrTimeTwistPool.push_back(*itertwist);
	}
	m_arrTimeTwistPool.clear();
	m_arrTimeTwistPool = ArrTimeTwistPool;
	itertwist = m_arrTimeTwistPool.begin();
	for ( ; itertwist != m_arrTimeTwistPool.end(); itertwist++ )
	{
		AddLDT((*itertwist).fTwist);
	}
}
//-----------------------------------------------------------------------
void CDriftCenter::UpdateTurnMovement( float fTime , CTencentCar::KEYINFO stKeyInfo)
{

	if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR )
	{
		return;
	}

	NxReal fAngle			= GetLDWZ() * fTime;
	
	NxMat33	matGround		= m_pCar->GetMatGround();
	NxVec3 vUp				= m_pCar->GetUpGlobal();

	//LogText ( "Drift vUp x = %f, y = %f, z = %f  \n " , vUp.x, vUp.y, vUp.z );

	RotateMatWorld ( matGround, vUp, fAngle);

	m_pCar->SetMatGround ( matGround );			
	m_pCar->SetTurnAngle( m_pCar->GetTurnAngle() + fAngle );
	return;

}

//-----------------------------------------------------------------------
void CDriftCenter::UpdateLineMovement( float fTime )
{
	NxVec3	vVecGlobal		= NxVec3 ( 0.0f, 0.0f, 0.0f );

	m_pCar->LocalToGlobal( m_vDriftVecLocal, vVecGlobal );

	NxVec3	vStep			= vVecGlobal * 1.0f * fTime;
	NxF32	fSharpness		= 0.8f;
	NxU32	unCollisionFlags;

	if ( m_pCar->IsBoxController() )
		m_pCar->getBoxController()->getActor()->moveGlobalOrientation(m_pCar->GetOriGlobal());

		LogText ( "vDriftStep = %f \n ", vStep );

	m_pCar->getBoxController()->move(
		vStep, 
		PhysXManager::CG_ENVIRONMENT, 
		0.001f, 
		unCollisionFlags, 
		fSharpness);
	return;
}

//-----------------------------------------------------------------------
void CDriftCenter::DriftForceControl( float fTime, CTencentCar::KEYINFO &stKeyInfo)
{

	if ( NxMath::abs ( GetLDV().y) < 1.0e-03) 
		return ;

	// 腾空重力
	//if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR )
	//{
	//	NxVec3 vGravity = NxVec3( 0.0f, 0.0f, -1.0f );
	//	NxVec3 vGravityLocal = NxVec3( 0.0f, 0.0f, 0.0f );
	//	m_pCar->GlobalToLocal( vGravity, vGravityLocal);
	//	AddLDF( vGravityLocal );
	//	//return;
	//}

	// 摩擦力
	NxReal fAngle		= GetHeadAngle(true,true);
	NxReal fNorAngle    = GetHeadAngle();

/*  浑然破碎
	NxVec3 vFricDir		= -GetLDVDIR();
	NxReal vFircSiz		= 0.0f;
	NxVec3 vFirc		= NxVec3( 0.0f,0.0f,0.0f);
	NxReal vMaxFircSiz  = SLIDFRICFORCE ;

	vFircSiz = NxMath::sin( fAngle ) * vMaxFircSiz;
	vFirc    = vFircSiz * vFricDir;
	AddLDF( vFirc );
*/

	NxVec3 vSlidFirc				= CDriftMath::SlidFrict();
	NxVec3 vRollFirc			= CDriftMath::RollFrict();

	NxVec3 vUpKeyForce			= CDriftMath::UpKeyForce();
	NxVec3 vDirLeftKeyForce		= CDriftMath::DirKeyForce();
	NxVec3 vDirRightKeyForce	= NxVec3( vDirLeftKeyForce.x, -vDirLeftKeyForce.y,vDirLeftKeyForce.z);

	NxVec3 vBannerHeadForce		= CDriftMath::BannerHeadForce();
	NxReal fLeftBannerVecForce  = CDriftMath::BannerVecForce();
	NxReal fRightBannerVecForce =-CDriftMath::BannerVecForce();

	if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR )
	{
		vSlidFirc = NxVec3(0.0f,0.0f,0.0f);
		vRollFirc = NxVec3(0.0f,0.0f,0.0f);
		vDirLeftKeyForce	= NxVec3( 0.0f, 0.0f,0.0f);
		vDirRightKeyForce	= NxVec3( 0.0f, 0.0f,0.0f);
		vBannerHeadForce    = NxVec3( 0.0f, 0.0f,0.0f);
		vUpKeyForce.x = __max ( vUpKeyForce.x, 2.0f);
	}
	AddLDF(vSlidFirc );
	AddLDF(vRollFirc);

	//if ( (stKeyInfo.bUp && fAngle >=  BANNERANGLE) ||
	//if( (stKeyInfo.bUp  && CaculatePlaneVecSize() < VECSCALE( ENDVECFIST ))) 
	if ( stKeyInfo.bUp )
	{
		AddLDF( vUpKeyForce );						// 考虑 get 引擎输出动力
	}

	if ( fNorAngle > 0.0f)		// fNorAngle  ：判断可以防止过速
	{
		AddLDF( vDirLeftKeyForce  );
	}

	if ( fNorAngle < 0.0f)
	{
		AddLDF( vDirRightKeyForce );
	}


	if ( stKeyInfo.bRight && fNorAngle > 0.0f )
	{
		AddLDFVecY	( fLeftBannerVecForce );
		AddLDF		( vBannerHeadForce );
	}

	if ( stKeyInfo.bLeft  && fNorAngle < 0.0f )
	{
		AddLDFVecY	( fRightBannerVecForce);
		AddLDF		( vBannerHeadForce );
	}

}


//-----------------------------------------------------------------------
void CDriftCenter::DriftTwistControl( float fTime, CTencentCar::KEYINFO &stKeyInfo)
{

	int nDir = 1.0f;

	NxReal fAngle		= GetHeadAngle(true,false);
	if ( fAngle > NxPi / 2.0f )
		nDir = -1.0f  * 2.0f ;

	NxReal fWecZ = GetLDWZ();

	NxReal fLeftKeyTwist	=  CDriftMath::DirKeyTwist();
	NxReal fRightKeyTwist	= -CDriftMath::DirKeyTwist();
	
	if ( stKeyInfo.bUp)
	{
		if ( stKeyInfo.bLeft )
		{
			AddLDTZ( fLeftKeyTwist  );
		}
		if ( stKeyInfo.bRight)
		{
			AddLDTZ( fRightKeyTwist );
		}
	}
	else
	{
		if ( stKeyInfo.bLeft)
		{
			AddLDTZ( fLeftKeyTwist  * nDir );
		}

		if ( stKeyInfo.bRight )
		{
			AddLDTZ( fRightKeyTwist * nDir );
		}
	}
}

//-----------------------------------------------------------------------
void CDriftCenter::TriggerFT(float fTime)
{


	NxVec3 vTotalForce = NxVec3 ( 0.0f, 0.0f, 0.0f );
	std::vector < NxVec3 >::iterator it = m_arrTotalForce.begin();
	for ( ; it != m_arrTotalForce.end(); it++ ) {
		vTotalForce += (*it);
	}

	std::vector< TIMEFORCE >::iterator iterforce = m_arrTimeForcePool.begin();
	for ( ; iterforce != m_arrTimeForcePool.end(); iterforce++ ) {
		vTotalForce += (*iterforce).fForce;
	}
	NxVec3 vTotalTwist = NxVec3 ( 0.0f, 0.0f, 0.0f );
	std::vector < NxVec3 >::iterator pt = m_arrTotalTwist.begin();
	for ( ; pt != m_arrTotalTwist.end(); pt++ ) {
		vTotalTwist += (*pt);
	}

	std::vector < TIMETWIST>::iterator itertwist = m_arrTimeTwistPool.begin();
	for ( ; itertwist != m_arrTimeTwistPool.end(); itertwist++ ) {
		vTotalTwist + (*itertwist).fTwist;
	}

//	LogText("Totalforce: xForce = %f, yForce = %f, zForce = %f \n ", 
//		vTotalForce.x, vTotalForce.y, vTotalForce.z);

	m_vDriftVecLocal += vTotalForce * fTime;

	m_vDriftWecLocal += vTotalTwist * fTime;




	std::vector< NxReal >::iterator bt = m_arrVecYForce.begin();
	for ( ; bt != m_arrVecYForce.end(); bt++ ) {
		m_vDriftVecLocal.y += (*bt) * fTime;
	}

	m_arrTotalTwist.clear();
	m_arrTotalForce.clear();
	m_arrVecYForce.clear();

}

//-----------------------------------------------------------------------
void CDriftCenter::DriftVecControl( float fTime, CTencentCar::KEYINFO &stKeyInfo)
{
	if ( stKeyInfo.bUp )
	{
		NxReal fDeltaAngle = GetHeadAngle(true) - NxPi/2.0f;
		if ( fDeltaAngle  >= 0 )
			if ( m_vDriftVecLocal.x < 0 )   
				m_vDriftVecLocal.x = 0.0f;
	}
}

//-----------------------------------------------------------------------
void CDriftCenter::DriftWecControl( float fTime, CTencentCar::KEYINFO &stKeyInfo)
{

	if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR && 
		m_nOldDriftContactStatus    != CTencentCar::ECS_INAIR )
	{
		m_fDriftInAirSaveWecZ = GetLDWZ();
	}

	if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR )
	{
		SetLDWZ(0.0f);
	}

	if ( m_pCar->GetContactStatus() != CTencentCar::ECS_INAIR &&
		m_nOldDriftContactStatus    == CTencentCar::ECS_INAIR )
	{
		m_fDriftInAirSaveWecZ = __min ( m_fDriftInAirSaveWecZ,CDriftMath::StartWec());
		SetLDWZ( m_fDriftInAirSaveWecZ );
	}

	NxReal fWecZ = GetLDWZ();
	//LogText ( "Drift WecZ : = %f \n " , fWecZ );

	if( NxMath::abs(fWecZ) > CDriftMath::MaxWec() ) 
		fWecZ = CDriftMath::MaxWec() * NxMath::sign( fWecZ );

	SetLDWZ(fWecZ);
}

//-----------------------------------------------------------------------
bool CDriftCenter::CanDriftStart(CTencentCar::KEYINFO &stKeyInfo )
{
	if ( GetDriftStatus() == DRIFT_START ) 
		return false;
	if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR)
		return false;

	m_vDriftVecLocal = m_pCar->GetVecLocal();
	//m_vDriftWecLocal = m_pCar->GetWecLocal();

	if ( CaculatePlaneVecSize() > VECSCALE( STARTVEC) && 
		stKeyInfo.bShift && 
		(stKeyInfo.bLeft || stKeyInfo.bRight) )
	{
		if ( !m_stOldKeyInfo.bShift )
			return true;
		if ( stKeyInfo.bLeft  && !m_stOldKeyInfo.bLeft )
			return true;
		if ( stKeyInfo.bRight && !m_stOldKeyInfo.bRight )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------
NxReal CDriftCenter::CaculatePlaneVecSize()
{
	NxReal LocalVecX = m_vDriftVecLocal.x;
	NxReal LocalVecY = m_vDriftVecLocal.y;
	NxReal fReturn = NxMath::pow 
		( LocalVecX * LocalVecX + LocalVecY * LocalVecY , 0.5f);

	return fReturn ;
}

//-----------------------------------------------------------------------
bool CDriftCenter::IsAccumulate()
{
	bool bReturn = true;
	if ( GetDriftStatus() == DRIFT_NONE )
		bReturn = false;

	if ( GetDriftStatus() == DRIFT_START)
	{
		if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR )
		{
			bReturn = false;
		}
		if ( CaculatePlaneVecSize() < VECSCALE( 50.0f) )
		{
			bReturn = false;
		}
	}
	return bReturn;
}

//-----------------------------------------------------------------------
void CDriftCenter::UpdateMarkAndTrack()
{
	if ( m_pCar->GetContactStatus() == CTencentCar::ECS_INAIR )
	{
		// 解决在空中也有划痕的情况
		// start - fix up by matrixzhang
		if (this->IsDrifting())
			m_bCreateNewTrack = true;
		// end - fix up by matrixzhang
		return ;
	}

	if(NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle() != m_pCar)
	{
		return;
	}

	if(m_bCreateNewTrack)
	{
		CLevelManager::Get()->GetKartCrack()->AddNewTrack();
		m_bCreateNewTrack = false;
	}

	for(int i = 2; i < 4; i++)
	{
		if(m_pCar->GetWheel(i)->GetContactStatus() == CCarWheel::ENS_NEAR)
		{
			NiPoint3 pUp,pDir,pPos;

			NxVec3 nv3 = m_pCar->GetWheel(i)->GetContactPos();
			pPos.x = nv3.x;
			pPos.y = nv3.y;
			pPos.z = nv3.z;


			NiMatrix3 pR;

			pR = NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate();

			pR.GetCol(1,pDir);
			pR.GetCol(2,pUp);
			pDir = -pDir;
			pDir.Unitize();

			NiPoint3 p;
			NxPlayMgr::GetHostPlay()->GetKartNIFRoot()->GetRotate().GetCol(0,p);

			p.Unitize();

			//	修改人:	wincoqin
			//  时间:	2007/02/01
			//  目的:   调整车痕离车的距离			
			pPos += pDir * 0.08f;
			float fDel  = -0.07f;
			// 结束

			if(i == 2)
			{
				pPos += p * fDel;
			}
			else
			{
				pPos -= p * fDel;
			}

			NxReal fCrackAngle = 0.0f;
			NxReal fRuiHeadAngle = GetHeadAngle( true, false );
			if ( fRuiHeadAngle >= NxPi / 2.0f )
				fCrackAngle = 0.0f;
			else
			{
				if ( i == 2 )
				//	fCrackAngle = (NxPi/2.0f - GetHeadAngle(true, true)) / 1.5f;
					fCrackAngle = (NxPi * 50.0f / 180.0f) / 1.5f;
					if ( i== 3 )
				//	fCrackAngle = (GetHeadAngle(true, true ) - NxPi/ 2.0f) / 1.5f;
					//fCrackAngle = NxPi * 75.0f / 180.0f;
					fCrackAngle = -(NxPi * 50.0f / 180.0f)/ 1.5f;
			}

			if ( CaculatePlaneVecSize () > VECSCALE(ENDVECFIST))
				CLevelManager::Get()->GetKartCrack()->AddPoints(i - 2,pPos,pUp,pDir, fCrackAngle);
			else
				CLevelManager::Get()->GetKartCrack()->AddPoints(i - 2,pPos,pUp,pDir, fCrackAngle,CKartCrack::ENM_SHOWGUIJI);
		}
	}
}


//-----------------------------------------------------------------------
NxReal CDriftCenter::CaculateVecTurnWec(NxVec3 NewVec, NxVec3 OldVec, float fTime)
{
	NxReal fAngle =  GetAngleBetweenVec3(NewVec, OldVec);
	
	if ( fAngle < 0.0f ) 
		fAngle = -fAngle;

	m_fVecTurnWec = fAngle / fTime ;
	return m_fVecTurnWec;
}

//-----------------------------------------------------------------------
NxReal CDriftCenter::GetHeadAngle
	( bool bAbs /* = false  */,
	  bool bRui /* = false */ )
{
	NxVec3 vLocalVec	= GetLDV();
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

CTimerControl::CTimerControl(CDriftCallBack * pCallBack):
m_bIsActive(false)

{
	m_dwStartTime = ::GetTickCount();
	m_pCallBack   = pCallBack;
}

CTimerControl::~CTimerControl()
{
	m_pCallBack = NULL;
}

void CTimerControl::ActiveTimer( float fLastTime )
{
	m_dwStartTime = ::GetTickCount();
	m_bIsActive   = true;
	m_dwLastTime   = (DWORD)(fLastTime * 1000.0f);
}

void CTimerControl::DeActiveTimer()
{
	m_bIsActive = false;
	
}

void CTimerControl::UpdateTimer()
{
	if ( !m_bIsActive )
		return;

	if ( ::GetTickCount() > m_dwStartTime + m_dwLastTime )
		OnTimer();
}

void CTimerControl::OnTimer()
{
	m_pCallBack->OnTimerCallBack();
	DeActiveTimer();
}

//-----------------------------------------------------------------------

CKeyEventControl::CKeyEventControl(CDriftCallBack * pCallBack)
{
	m_pCallBack = pCallBack;
	memset(&m_stOldKeyInfo,0,sizeof(CTencentCar::KEYINFO));
}

CKeyEventControl::~CKeyEventControl()
{
	m_pCallBack = NULL;
}


void CKeyEventControl::UpdateKeyEventControl( CTencentCar::KEYINFO stKeyInfo )
{
	if ( stKeyInfo.bShift && !m_stOldKeyInfo.bShift) 
		OnEventKey( DRIFT_KEYEVENT_SHIFTPRESS );
	m_stOldKeyInfo = stKeyInfo;
}

void CKeyEventControl::OnEventKey(ENUMKEYEVENT eEvent )
{
	switch( eEvent )
	{
	case DRIFT_KEYEVENT_SHIFTPRESS:
		m_pCallBack->OnShiftPress();
		break;
	default:
		break;
	}
}


//-----------------------------------------------------------------------
CDriftCenter::CDriftCenter(CTencentCar * pCar )
: m_pCar(pCar)
, m_pTimeControl( NULL )
, m_pKeyEventControl( NULL )
, m_pDriftBaseParam ( NULL )
{
	m_nDriftStatus = DRIFT_NONE;
	memset(&m_stOldKeyInfo,0,sizeof(CTencentCar::KEYINFO));
	m_vDriftVecLocal = NxVec3(0.0f,0.0f,0.0f);
	m_vDriftWecLocal = NxVec3(0.0f,0.0f,0.0f);
	m_bBanner		 = false;
	m_bShoot		 = false;
	m_fVecDriftStart = 0.0f;
	m_fWheelTurnAngle= 0.0f;
	m_fOldAngle      = 0.0f;

	m_fWheelMaxTurnAngle = MAXWHEELTURNANG;
	m_fWheelTurnVec		 = WHEELTURNVEC;
	m_bCollision		 = false;
	m_bCreateNewTrack    = false;
	m_arrVecYForce.clear();
	m_arrTimeForcePool.clear();
	m_arrTimeTwistPool.clear();
	m_fDriftInAirSaveWecZ= 0.0f; 
}

//-----------------------------------------------------------------------
CDriftCenter::~CDriftCenter()
{
	if (m_pCar->GetPlay() == NxPlayMgr::GetHostPlay())
	{
		CDriftMath::UnInitDriftMath();
	}
	m_pCar = NULL;
	SAFE_DELETE(m_pTimeControl);
	SAFE_DELETE(m_pKeyEventControl);
	SAFE_DELETE(m_pDriftBaseParam);
	DeleteCriticalSection(&critSec);
}

//-----------------------------------------------------------------------
bool CDriftCenter::InitDriftCenter()
{
	m_pTimeControl		= new CTimerControl(this) ;
	m_pKeyEventControl  = new CKeyEventControl(this);
	m_pDriftBaseParam	= new DRIFTBASEPARAM;
	
	m_pDriftBaseParam->InitBaseParam();


	InitializeCriticalSection(&critSec);
	if (m_pCar->GetPlay() == NxPlayMgr::GetHostPlay())
	{
		if( !CDriftMath::InitDriftMath(this))
			return false;
	}
	return true;
}

//-----------------------------------------------------------------------
bool CDriftCenter::LoadConfig()
{
	CDriftMath::LoadDriftConfig();
	return true;
}




//-----------------------------------------------------------------------
void CDriftCenter::ProcessWec( CTencentCar::KEYINFO & stKeyInfo )
{
	//if ( !m_stOldKeyInfo.bShift && stKeyInfo.bShift ) 
	//	AddLDWZ( 1.5f ,stKeyInfo );
}


//-----------------------------------------------------------------------
void CDriftCenter::AddLDWZ(NxReal fDeltaZ,  CTencentCar::KEYINFO &stKeyInfo)
{
	NxReal fMaxWec		= CDriftMath::MaxWec();
	NxReal fCurWec		= GetLDWZ();
	NxReal fHeadAngle	= GetHeadAngle(false);
	NxReal fAbsAngle	= GetHeadAngle(true,true);
	
	if( fAbsAngle < BANNERANGLE )
		return;

	//fDeltaZ = 5.0f;
	if ( stKeyInfo.bShift )
	{
		if ( stKeyInfo.bRight && fCurWec > 0.0f)
			fCurWec = __min( fCurWec - fDeltaZ ,  fMaxWec );
		if ( stKeyInfo.bLeft  && fCurWec < 0.0f )
			fCurWec = __max( fCurWec + fDeltaZ , -fMaxWec );
	}

	SetLDWZ(fCurWec);

	return;
	
}

