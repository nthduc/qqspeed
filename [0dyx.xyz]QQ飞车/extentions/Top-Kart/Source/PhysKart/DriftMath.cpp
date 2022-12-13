#include "Top-KartPCH.h"
#include ".\driftmath.h"
#include "DriftCenter.h"


						/*-----------------------------------------------------------------------

											Create by HoverZhao 2007.3.27

						-------------------------------------------------------------------------*/


PDRIFTBASEPARAM		 	CDriftMath::ms_pDriftBaseParam	= NULL; 
CTencentCar::KEYINFO *	CDriftMath::ms_pKeyInfo			= NULL;
CDriftCenter		 *  CDriftMath::ms_pCenter			= NULL;
NxVec3					CDriftMath::ms_vSlidForce		= NxVec3(0.0f,0.0f,0.0f);
NxVec3					CDriftMath::ms_vRollForce		= NxVec3(0.0f,0.0f,0.0f);

NxVec3					CDriftMath::ms_vUpKeyForce		= NxVec3(0.0f,0.0f,0.0f);
NxVec3					CDriftMath::ms_vDirKeyForce		= NxVec3(0.0f,0.0f,0.0f);
NxVec3					CDriftMath::ms_vBannerHeadForce = NxVec3(0.0f,0.0f,0.0f);
NxReal					CDriftMath::ms_fBannerVecForce	= 0.0f;
NxReal					CDriftMath::ms_fDirKeyTwist		= 0.0f;
NxReal					CDriftMath::ms_fMaxWec			= 0.0f;
NxReal					CDriftMath::ms_fStartWec		= 0.0f;
NxReal					CDriftMath::ms_fEndVecFist		= 0.0f;
NxReal					CDriftMath::ms_fEndVecSecon		= 0.0f;
NxReal					CDriftMath::ms_fStartVec		= 0.0f;
NxReal					CDriftMath::ms_fBannerAngle		= 0.0f;
NxReal					CDriftMath::ms_fBannerTwist		= 0.0f;
NxReal					CDriftMath::ms_fClockWec		= 0.0f;

#define SAFECHECKER \
	if ( !ms_pCenter || !ms_pDriftBaseParam || !ms_pKeyInfo)	\
		return ;


//-----------------------------------------------------------------------
CDriftMath::CDriftMath(void)
{
}

CDriftMath::~CDriftMath(void)
{
}

//-----------------------------------------------------------------------
void CDriftMath::DoDampTwist()
{
	SAFECHECKER
}

//-----------------------------------------------------------------------
void CDriftMath::DoDirKeyForce()
{
	SAFECHECKER
	NxReal fAngle		= ms_pCenter->GetHeadAngle();

	NxReal fVecSize = ms_pCenter->CaculatePlaneVecSize();

	NxReal fParamA = ms_pDriftBaseParam->fDirKeyForceParamA;
	NxReal fParamB = ms_pDriftBaseParam->fDirKeyForceParamB;

	NxReal fDirKeyForce = ms_pDriftBaseParam->fDirKeyForce + fParamA * fVecSize / ( fParamB + fVecSize)  ;

	if ( fAngle < NxPi / 2.0f   && fAngle > 0.0f && 
		ms_pKeyInfo->bRight )
	{
		fDirKeyForce = 0;
	}
	if ( fAngle > - NxPi / 2.0f && fAngle < 0.0f && 
		ms_pKeyInfo->bLeft )
	{
		fDirKeyForce = 0;
	}

	ms_vDirKeyForce		= NxVec3(0.0f, fDirKeyForce * NxMath::cos(fAngle), 0.0f);
}

//-----------------------------------------------------------------------
void CDriftMath::DoDirKeyTwist()
{
	SAFECHECKER

	CTencentCar::KEYINFO stKeyInfo = ms_pCenter->GetTencentCar()->GetKeyInfo();
	NxReal fVecSize = ms_pCenter->CaculatePlaneVecSize();
	
	NxReal fKeyRowTwist			= ms_pDriftBaseParam->fDirKeyTwist ;

	NxReal fKeyTwistParamA = ms_pDriftBaseParam->fDirKeyTwistParamA;
	NxReal fKeyTwistParamB = ms_pDriftBaseParam->fDirKeyTwistParamB;


	NxReal fKeyRowBannerTwist	= ms_pDriftBaseParam->fBannerKeyTwist;
	NxReal fKeyBannerTwistParamA = ms_pDriftBaseParam->fBannerKeyTwistParamA;
	NxReal fKeyBannerTwistParamB = ms_pDriftBaseParam->fBannerKeyTwistParamB;

	ms_fDirKeyTwist			= fKeyRowTwist;

	NxReal fRuiAngle		= ms_pCenter->GetHeadAngle(true,true);
	NxReal fAngRealAngle	= ms_pCenter->GetHeadAngle();
	NxReal fWecZ			= ms_pCenter->GetLDWZ();
	NxReal fKeyTwist = 0.0f ;


	if ( ( stKeyInfo.bLeft  && fAngRealAngle < 0.0f ) ||
		 ( stKeyInfo.bRight && fAngRealAngle > 0.0f ))
	{
		NxReal FactorA  = 1.0f - NxMath::sin(fRuiAngle * 2 );
		fKeyTwist = ( fKeyRowTwist - fKeyTwistParamA ) * FactorA + fKeyTwistParamB;
	}
	else
	{
		NxReal fKeyTwistMin = fKeyRowBannerTwist - fKeyBannerTwistParamA;
		NxReal fKeyTwistMax = fKeyRowBannerTwist + fKeyBannerTwistParamB;

		fKeyTwist = fKeyTwistMin + (fKeyTwistMax - fKeyTwistMin) * fRuiAngle * 2.0f / NxPi; 
		LogText ( "Drift vec size = %f \n " , fVecSize );
	}

	ms_fDirKeyTwist = fKeyTwist;
}


void CDriftMath::DoUpKeyForce()
{
	SAFECHECKER

	NxReal fVecSize = ms_pCenter->CaculatePlaneVecSize();
	
	//-----------------------------------------------------------------------
	NxReal fUpKeyForce = ms_pDriftBaseParam->fDirUpKeyForce ;
	NxReal fFactorA    = ms_pDriftBaseParam->fDirUpKeyForceParamA;
	NxReal fFactorB    = ms_pDriftBaseParam->fDirUpKeyForceParamB;
	fUpKeyForce = -( fFactorB / (fUpKeyForce * fUpKeyForce) ) * fVecSize * fVecSize + fFactorB + fFactorA;
	//fUpKeyForce = fUpKeyForce + fFactorA * fVecSize + fFactorB * fVecSize * fVecSize;

	ms_vUpKeyForce  = NxVec3 ( fUpKeyForce,0.0f,0.0f);
	
	//-----------------------------------------------------------------------
}

void CDriftMath::DoSlidFrict()
{
	SAFECHECKER
		// 摩擦力
	NxReal fAngle		= ms_pCenter->GetHeadAngle(true,true);
	NxReal fNorAngle    = ms_pCenter->GetHeadAngle();

	NxVec3 vFricDir		= -ms_pCenter->GetLDVDIR();
	NxReal vFircSiz		= 0.0f;
	NxVec3 vFirc		= NxVec3( 0.0f,0.0f,0.0f);
	NxReal vMaxFircSiz  = ms_pDriftBaseParam->fSlidFricForce ;
	vFircSiz = NxMath::sin( fAngle ) * vMaxFircSiz;
	vFirc    = vFircSiz * vFricDir;
	ms_vSlidForce = vFirc;
}

void CDriftMath::DoRollFrict()
{
	SAFECHECKER
		NxReal fAngle		= ms_pCenter->GetHeadAngle(true,true);
	NxReal fNorAngle    = ms_pCenter->GetHeadAngle();

	NxVec3 vFricDir		= -ms_pCenter->GetLDVDIR();
	NxReal vFircSiz		= 0.0f;
	NxVec3 vFirc		= NxVec3( 0.0f,0.0f,0.0f);
	NxReal vMaxFircSiz  = ms_pDriftBaseParam->fRollFricForce ;
	vFircSiz = NxMath::cos( fAngle ) * vMaxFircSiz;
	vFirc    = vFircSiz * vFricDir;
	ms_vRollForce = vFirc;
}

void CDriftMath::DoBannerAngle()
{
	SAFECHECKER
	ms_fBannerAngle = ms_pDriftBaseParam->fBannerAngle;
}

void CDriftMath::DoBannerTwist()
{
	NxReal fBannerTwist = ms_pDriftBaseParam->fBannerTwist;
	NxReal fAngle		= ms_pCenter->GetHeadAngle(true,true);
	NxReal fBannerTwistParamA = ms_pDriftBaseParam->fBannerTwistParamA;

	fBannerTwist = fBannerTwist * NxMath::pow(( 2.0f / NxPi * fAngle ) ,fBannerTwistParamA);
	ms_fBannerTwist = fBannerTwist ;

	SAFECHECKER
}

void CDriftMath::DoBannerVecForce()
{
	SAFECHECKER
	NxReal fBannerVecForceParamA = ms_pDriftBaseParam->fBannerVecForceParamA;
	NxReal fBannerVecForceParamB = ms_pDriftBaseParam->fBannerVecForceParamB;
	NxReal fBannerVecForceParamC = ms_pDriftBaseParam->fBannerVecForceParamC;

	NxReal fVecSize = ms_pCenter->CaculatePlaneVecSize();
	
	NxReal fFactor = fBannerVecForceParamA + 
		fBannerVecForceParamB * fVecSize * fVecSize /fBannerVecForceParamC ;
	
	NxReal fAbsAngle = ms_pCenter->GetHeadAngle(true,true);

	NxReal fpFac = ( 4.0f / NxPi ) * fAbsAngle - 1.0f;
	ms_fBannerVecForce = ms_pDriftBaseParam->fBannerVecForce * fFactor * fpFac;
	
}

void CDriftMath::DoBannerHeadForce()
{
	SAFECHECKER
	NxReal fVecSize = ms_pCenter->CaculatePlaneVecSize();
	NxReal fFactor = 0.3 + 0.7 * fVecSize /( 1.0 + fVecSize );
	ms_vBannerHeadForce = NxVec3 ( ms_pDriftBaseParam->fBannerHeadForce * fFactor, 0.0f, 0.0f);
		ms_vBannerHeadForce = NxVec3 ( 0.0f, 0.0f, 0.0f);
}
void CDriftMath::DoStartWec()
{
	SAFECHECKER
	ms_fStartWec = ms_pDriftBaseParam->fStartWec;

	NxReal fAbsRuiAngle = ms_pCenter->GetHeadAngle(true,true);
	//ms_fStartWec = NxMath::sin( fAbsRuiAngle ) * 0.5 + ms_fStartWec;

	// 速度越快起始角速度越快
	//NxReal fVecSize = ms_pCenter->CaculatePlaneVecSize();
	//ms_fStartWec = 2.5 + (fVecSize + 1.0) / 10.0f;
}

void CDriftMath::DoMaxWec()
{
	SAFECHECKER
	NxReal fMaxWec	= 0.0f;
	NxReal fVecSize = ms_pCenter->CaculatePlaneVecSize();
	fMaxWec = ms_pDriftBaseParam->fWecEffect;

	ms_fMaxWec		= fMaxWec;
}

void CDriftMath::DoEndVecSecon()
{
	ms_fEndVecSecon		= ms_pDriftBaseParam->fEndVecSecon;
}

void CDriftMath::DoEndVecFist()
{
	ms_fEndVecFist		= ms_pDriftBaseParam->fEndVecFist;
}

void CDriftMath::DoStartVec()
{
	ms_fStartVec		= ms_pDriftBaseParam->fStartVec;
}

//-----------------------------------------------------------------------
void   CDriftMath::CollectData(float fTime, CTencentCar::KEYINFO * pData)
{
	if ( !ms_pKeyInfo )
	{
		ms_pKeyInfo = new  CTencentCar::KEYINFO;
	}

	if ( !ms_pDriftBaseParam )
	{
		ms_pDriftBaseParam = new DRIFTBASEPARAM;
		LoadDriftConfig();
	}

	memcpy(ms_pKeyInfo,pData,sizeof(CTencentCar::KEYINFO));
	DoBannerAngle();
	DoBannerTwist();
	DoBannerVecForce();
	DoBannerHeadForce();
	DoStartWec();
	DoRollFrict();
	DoSlidFrict();
	DoUpKeyForce();
	DoDirKeyTwist();
	DoDirKeyForce();
	DoDampTwist();
	DoMaxWec();
	DoEndVecSecon();
	DoEndVecFist();
	DoStartVec();

}

//-----------------------------------------------------------------------
bool CDriftMath::InitDriftMath(CDriftCenter * pDriftCenter)
{
	bool bReturn = true;
	SAFE_DELETE( ms_pDriftBaseParam );
	SAFE_DELETE( ms_pKeyInfo );
	ms_pDriftBaseParam	= new DRIFTBASEPARAM;
	ms_pKeyInfo			= new CTencentCar::KEYINFO;
	ms_pCenter			= pDriftCenter;
	
	if ( !LoadDriftConfig() )
		bReturn = false;

	return bReturn;
}

//-----------------------------------------------------------------------
bool CDriftMath::LoadDriftConfig()
{
	CScriptState ls;
	if(ls.DoFile("config/DriftCfg.lua") != 0)
		return false;

	CVarTable vtDrift("DriftBaseParam");
	vtDrift.GetFromScript(&ls);

	ms_pDriftBaseParam->fStartVec			= vtDrift.Element("fStartVec")->Float();
	ms_pDriftBaseParam->fEndVecFist			= vtDrift.Element("fEndVecFist")->Float();
	ms_pDriftBaseParam->fEndVecSecon		= vtDrift.Element("fEndVecSecon")->Float();

	ms_pDriftBaseParam->fDirKeyForce		= vtDrift.Element("fDirKeyForce")->Float();
	ms_pDriftBaseParam->fDirKeyForceParamA  = vtDrift.Element("fDirKeyForceParamA")->Float();
	ms_pDriftBaseParam->fDirKeyForceParamB  = vtDrift.Element("fDirKeyForceParamB")->Float();


	ms_pDriftBaseParam->fDirKeyTwist		= vtDrift.Element("fDirKeyTwist")->Float();
	ms_pDriftBaseParam->fDirKeyTwistParamA  = vtDrift.Element("fDirKeyTwistParamA")->Float();
	ms_pDriftBaseParam->fDirKeyTwistParamB  = vtDrift.Element("fDirKeyTwistParamB")->Float();


	ms_pDriftBaseParam->fDirUpKeyForce		 = vtDrift.Element("fDirUpKeyForce")->Float();
	ms_pDriftBaseParam->fDirUpKeyForceParamA = vtDrift.Element("fDirUpKeyForceParamA")->Float();
	ms_pDriftBaseParam->fDirUpKeyForceParamB = vtDrift.Element("fDirUpKeyForceParamB")->Float();


	ms_pDriftBaseParam->fBannerKeyTwist		 = vtDrift.Element("fBannerKeyTwist")->Float();
	ms_pDriftBaseParam->fBannerKeyTwistParamA= vtDrift.Element("fBannerKeyTwistParamA")->Float();
	ms_pDriftBaseParam->fBannerKeyTwistParamB= vtDrift.Element("fBannerKeyTwistParamB")->Float();

	ms_pDriftBaseParam->fBannerTwist		= vtDrift.Element("fBannerTwist")->Float();
	ms_pDriftBaseParam->fBannerTwistParamA  = vtDrift.Element("fBannerTwistParamA")->Float();

	ms_pDriftBaseParam->fBannerVecForce		  = vtDrift.Element("fBannerVecForce")->Float();
	ms_pDriftBaseParam->fBannerVecForceParamA = vtDrift.Element("fBannerVecForceParamA")->Float();
	ms_pDriftBaseParam->fBannerVecForceParamB = vtDrift.Element("fBannerVecForceParamB")->Float();
	ms_pDriftBaseParam->fBannerVecForceParamC = vtDrift.Element("fBannerVecForceParamC")->Float();

	ms_pDriftBaseParam->fBannerHeadForce	= vtDrift.Element("fBannerHeadForce")->Float();
	ms_pDriftBaseParam->fRollFricForce		= vtDrift.Element("fRollFricForce")->Float();
	ms_pDriftBaseParam->fSlidFricForce		= vtDrift.Element("fSlidFricForce")->Float();
	ms_pDriftBaseParam->fStartWec			= vtDrift.Element("fStartWec")->Float();
	ms_pDriftBaseParam->fMaxWec				= vtDrift.Element("fMaxWec")->Float();
	ms_pDriftBaseParam->fBannerAngle		= vtDrift.Element("fBannerAngle")->Float();
	ms_pDriftBaseParam->fSuaiJianTwist		= vtDrift.Element("fSuaiJianTwist")->Float();
	ms_pDriftBaseParam->fVecEffect			= vtDrift.Element("fVecEffect")->Float();
	ms_pDriftBaseParam->fWecEffect			= vtDrift.Element("fWecEffect")->Float();
	ms_pDriftBaseParam->fClockWec			= vtDrift.Element("fClockWec")->Float();



	ms_fStartWec		= ms_pDriftBaseParam->fStartWec;
	ms_fMaxWec			= ms_pDriftBaseParam->fMaxWec;
	ms_fEndVecFist		= ms_pDriftBaseParam->fEndVecFist;
	ms_fEndVecSecon		= ms_pDriftBaseParam->fEndVecSecon;
	ms_fStartVec		= ms_pDriftBaseParam->fStartVec;
	ms_fBannerAngle		= ms_pDriftBaseParam->fBannerAngle;
	ms_fBannerTwist     = ms_pDriftBaseParam->fBannerTwist;

	
	return true;
}

//-----------------------------------------------------------------------
void CDriftMath::UnInitDriftMath()
{
	ms_pCenter = NULL;
	SAFE_DELETE(ms_pDriftBaseParam);
	SAFE_DELETE(ms_pKeyInfo );
}


//-----------------------------------------------------------------------
NxReal CDriftMath::BannerAngle()
{
	return ms_pDriftBaseParam->fBannerAngle * NxPi / 180.0f;
}

NxReal CDriftMath::BannerTwist()
{
	return ms_fBannerTwist;
}

NxReal CDriftMath::BannerVecForce()
{
	return ms_fBannerVecForce;
}

NxVec3 CDriftMath::BannerHeadForce()
{
	return ms_vBannerHeadForce;
}

NxReal CDriftMath::DampTwist()
{
	return ms_pDriftBaseParam->fSuaiJianTwist;
}

NxVec3 CDriftMath::DirKeyForce()
{
	return ms_vDirKeyForce;
}

NxReal CDriftMath::DirKeyTwist()
{
	return ms_fDirKeyTwist;
}

NxVec3 CDriftMath::RollFrict()
{

	return ms_vRollForce;
}

NxVec3 CDriftMath::SlidFrict()
{
	return ms_vSlidForce;
}

NxVec3 CDriftMath::UpKeyForce()
{
	return ms_vUpKeyForce;
}

NxReal CDriftMath::MaxWec()
{
	return ms_fMaxWec;
}

NxReal CDriftMath::StartWec()
{
	return ms_fStartWec;
}

NxReal CDriftMath::EndVecFist()
{
	return ms_fEndVecFist;
}

NxReal CDriftMath::EndVecSecon()
{
	return ms_fEndVecSecon;
}

NxReal CDriftMath::StartVec()
{
	return ms_fStartVec;
}

NxReal CDriftMath::ClockWec()
{
	return ms_fClockWec;
}