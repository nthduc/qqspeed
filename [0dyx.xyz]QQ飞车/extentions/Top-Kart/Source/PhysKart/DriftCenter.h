#pragma once



								/*-----------------------------------------------------------------------

													Create by HoverZhao 2007.3.10

								-------------------------------------------------------------------------*/



#define VECSCALE(x)  x / 14.0f  


#include "TencentCar.h"
#include "variant.h"
#include "DriftMath.h"


class CDriftCallBack
{
public:
	virtual void OnTimerCallBack() = 0;
	virtual void OnShiftPress() = 0;
};

class CTimerControl
{
public:
	CTimerControl(CDriftCallBack * pCallBack );
	~CTimerControl();

public:
	void UpdateTimer();
	void ActiveTimer( float fLastTime );
	void DeActiveTimer();
	void OnTimer();
private:
	bool				m_bIsActive;
	
	float				m_dwLastTime;
	DWORD				m_dwStartTime;

	CDriftCallBack	*	m_pCallBack;

};

class CKeyEventControl
{
	typedef enum enmKeyEvent
	{
		DRIFT_KEYEVENT_SHIFTPRESS,
		DRIFT_KEYEVENT_SHIFTRELEASE,
		DRIFT_KEYEVENT_RIGHTPRESS,
		DRIFT_KEYEVENT_LEFTPRESS
	}ENUMKEYEVENT;

public:
	CKeyEventControl(CDriftCallBack * pCallBack);
	~CKeyEventControl();
public:

	void UpdateKeyEventControl( CTencentCar::KEYINFO stKeyInfo );
	void OnEventKey(ENUMKEYEVENT eEvent );

protected:
	CTencentCar::KEYINFO m_stOldKeyInfo;
	CDriftCallBack	*	 m_pCallBack;

};



//-----------------------------------------------------------------------
class CDriftCenter : public CDriftCallBack
{
	friend class CDriftMath;
public: 
	typedef enum eDriftingStatus
	{
		DRIFT_NONE = 1,
		DRIFT_START

	}ENMDRIFTSTATUS;
	
	typedef enum eDriftForceType
	{
		DF_NORMAL,
		DR_VECY,
		DR_VECX
	}ENMDRIFTTYPE;

	typedef struct tagTimeForce
	{
		NxVec3 fForce;
		float fTime;
	}TIMEFORCE,*PTIMEFORCE;

	typedef struct tagTimeTwist
	{
		NxVec3 fTwist;
		float fTime;
	}TIMETWIST,*PTIMETWIST;

public :
	CDriftCenter(CTencentCar * pCar );
	~CDriftCenter();

public : 
	void ModifyDriftBaseParam(DRIFTBASEPARAM stDriftBaseParam);

public:
	bool LoadConfig();

private:
	DRIFTBASEPARAM * m_pDriftBaseParam;


public:
	bool InitDriftCenter();

	void Update( DWORD dwTime , CTencentCar::KEYINFO stKeyInfo );
	void Run(  DWORD dwTime , CTencentCar::KEYINFO stKeyInfo );

	void DriftStart(CTencentCar::KEYINFO stKeyInfo);
	void DriftEnd( bool bIsNormal = true ,bool bIsCutVec = true);
	void DriftLoop( float fTime ,CTencentCar::KEYINFO stKeyInfo );


protected:
	void UpdateTurnMovement( float fTime ,CTencentCar::KEYINFO stKeyInfo );
	void UpdateLineMovement( float fTime );
	void UpdateTimeForceTwistPool( float fTime);


protected:
	void DriftVecControl	( float fTime, CTencentCar::KEYINFO &stKeyInfo);
	void DriftWecControl	( float fTime, CTencentCar::KEYINFO &stKeyInfo);
	void DriftForceControl	( float fTime, CTencentCar::KEYINFO &stKeyInfo);
	void DriftTwistControl	( float fTime, CTencentCar::KEYINFO &stKeyInfo);
	void PreLoop			( float fTime, CTencentCar::KEYINFO &stKeyInfo);

	void DriftGravity		( float fTime);

	bool CanDriftStart(CTencentCar::KEYINFO &stKeyInfo );

	void ProcessWec( CTencentCar::KEYINFO & stKeyInfo );

protected:
	void SetBanner(bool bBanner)	{ m_bBanner = bBanner;	}
	bool GetBanner()				{ return m_bBanner;		}

	void SetShoot (bool bShoot )	{ m_bShoot = bShoot;	}
	bool GetShoot ()				{ return m_bShoot;		}

private:
	bool m_bBanner;
	bool m_bShoot;

public:
	void AddLDF	( NxVec3 vForce	, float fTime = 0.0f ) { m_arrTotalForce.push_back(vForce ); } 

	void AddLDT ( NxVec3 vTwist	) { m_arrTotalTwist.push_back(vTwist ); }
	void AddLDTZ( NxReal fTwistZ) { NxVec3 vTwist = NxVec3 ( 0.0f, 0.0f, fTwistZ ); 
									m_arrTotalTwist.push_back(vTwist ); }

	void AddTimeLDF( NxVec3 vForce , float fTime = 0.0f ) { 
		TIMEFORCE TimeForce;
		TimeForce.fForce = vForce;
		TimeForce.fTime  = fTime ; 
		m_arrTimeForcePool.push_back(TimeForce);
	}

	void AddTimeTwist ( NxVec3 vTwist , float fTime = 0.0f )  {
		TIMETWIST TimeTwist;
		TimeTwist.fTwist = vTwist;
		TimeTwist.fTime = fTime ;
		m_arrTimeTwistPool.push_back(TimeTwist );
	}

	void AddLDFVecY( NxReal fForce) { m_arrVecYForce.push_back(fForce);}
	
	
	void TriggerFT( float fTime );

	void SetLDV( NxVec3 vVec ) 
	{ 
		EnterCriticalSection( &critSec );
		m_vDriftVecLocal.x  = vVec.x; 
		m_vDriftVecLocal.y = vVec.y;
		m_pCar->SetVecLocal(m_vDriftVecLocal);
		LeaveCriticalSection( & critSec );  
	}

	NxVec3 GetLDV ()	{ return NxVec3( m_vDriftVecLocal.x, m_vDriftVecLocal.y, 0.0f );}
	NxVec3 GetLDW()		{ return m_vDriftWecLocal;	}
	NxReal GetLDWZ()	{ return m_vDriftWecLocal.z; }
	
	
	void   SetLDWZ(NxReal fWecZ) { m_vDriftWecLocal.z = fWecZ;}
	void   AddLDWZ(NxReal fDeltaZ ,  CTencentCar::KEYINFO &stKeyInfo);
	
	
	
	NxVec3 GetLDVDIR()  { NxVec3 vVecLocal = NxVec3( m_vDriftVecLocal.x, m_vDriftVecLocal.y, 0.0f ); 
						 vVecLocal.normalize(); return vVecLocal;}
public:
	NxReal GetHeadAngle(bool bAbs = false ,bool bRui = false) ;


protected:
	NxReal GetKeyTwist ( float fTime, CTencentCar::KEYINFO &stKeyInfo  );

public: 
	int		GetDriftStatus()				{ return m_nDriftStatus ;}
	void	SetDriftStatus( int nStatus )	{ m_nDriftStatus = nStatus; }
	bool	IsDrifting()					{ return m_nDriftStatus == DRIFT_START; }
	bool    IsAccumulate();
	bool    IsCollision()					{ return m_bCollision;}
	void    SetCollision(bool bCollision)	
	{ 
		EnterCriticalSection( &critSec );
		m_bCollision = bCollision ;
		LeaveCriticalSection( & critSec ); 
	} 
	
	void SetTrack( bool bTrack ) { m_bCreateNewTrack = bTrack;}

protected:
	void AirDrift(float fTime, CTencentCar::KEYINFO stKeyInfo );


private:
	int m_nDriftStatus;
	bool m_bCollision;

public:
	void OnTimerCallBack();
	void OnShiftPress();

public:
	void	UpdateMarkAndTrack();
	void    GetWheelTurnAngle();
	NxReal  GetVecTurnAngle() { return m_fVecTurnWec;}

protected:
	//void   UpdateMaxWec();
	//void   SetMaxWec(NxReal fMaxWec)	{ m_fMaxWec = fMaxWec;}
	//NxReal GetMaxWec()					{ return m_fMaxWec;	  }

private:
	NxReal m_fMaxWec;

private:
	bool    m_bCreateNewTrack;

private:
	NxReal  CaculatePlaneVecSize();
	NxReal  CaculateVecTurnWec(NxVec3 NewVec, NxVec3 OldVec, float fTime);

public :
	CTencentCar * GetTencentCar() { return m_pCar;}

private:
	CTencentCar				* m_pCar;
	CTencentCar::KEYINFO	m_stOldKeyInfo ; 
	CTimerControl			* m_pTimeControl;
	CKeyEventControl        * m_pKeyEventControl;

private: 
	int nDriftStatus; 


private:
	NxVec3 m_vDriftVecLocal ;
	NxVec3 m_vDriftWecLocal ;
	NxReal m_fVecDriftStart ;
	NxVec3 m_vOldDriftVecLocal;

private:
	NxReal m_fVecTurnWec;

private:
	std::vector < NxVec3 >	m_arrTotalForce;
	std::vector	< NxVec3 >	m_arrTotalTwist;

	std::vector < NxReal > m_arrVecYForce;
	
	std::vector < TIMETWIST > m_arrTimeTwistPool;
	std::vector < TIMEFORCE > m_arrTimeForcePool;
	


private:
	NxReal m_fWheelTurnAngle;
	NxReal m_fWheelMaxTurnAngle;
	NxReal m_fWheelTurnVec;
	NxReal m_fOldAngle;

	NxReal m_fDriftRadio;

	NxReal m_fDriftInAirSaveWecZ;
	int	   m_nOldDriftContactStatus;


private:
	CRITICAL_SECTION critSec;
	


};