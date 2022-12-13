#pragma once


						/*-----------------------------------------------------------------------

										Create by HoverZhao 2007.3.27

						-------------------------------------------------------------------------*/


typedef struct tagDriftBaseParam
{

public:
	tagDriftBaseParam()
	{
		fStartVec		= 0.0f;
		fEndVecFist		= 0.0f;
		fEndVecSecon	= 0.0f;

		fDirKeyForce	= 0.0f;
		fDirKeyForceParamA = 0.0f;
		fDirKeyForceParamB = 0.0f;


		fDirKeyTwist	= 0.0f;
		fDirKeyTwistParamA = 0.0f;
		fDirKeyTwistParamB = 0.0f;

		fDirUpKeyForce	= 0.0f;
		fDirUpKeyForceParamA = 0.0f ;
		fDirUpKeyForceParamB = 0.0f ; 


		fBannerTwist    = 0.0f;
		fBannerTwistParamA  = 0.0f;


		fBannerKeyTwist = 0.0f;
		fBannerKeyTwistParamA = 0.0f;
		fBannerKeyTwistParamB = 0.0f;
		
		
		fBannerVecForce = 0.0f;
		fBannerVecForceParamA = 0.0f;
		fBannerVecForceParamB = 0.0f;
		fBannerVecForceParamC = 0.0f;

		fBannerHeadForce= 0.0f;
		fSlidFricForce	= 0.0f;
		fRollFricForce	= 0.0f;
		fStartWec		= 0.0f;
		fMaxWec			= 0.0f;
		fBannerAngle    = 0.0f;
		fSuaiJianTwist  = 0.0f;
		fVecEffect		= 0.0f;
		fWecEffect		= 0.0f;
		fClockWec		= 0.0f;

	}

	void InitBaseParam()
	{
		fStartVec		= 18.0f;
		fEndVecFist		= 50.0f;
		fEndVecSecon	= 20.0f;

		fDirKeyForce	= 2.0f;
		fDirKeyForceParamA = 4.0f;
		fDirKeyForceParamB = 10.0f;

		fDirKeyTwist	= 6.0f;
		fDirKeyTwistParamA = 0.5f;
		fDirKeyTwistParamB = 1.5f;

		fDirUpKeyForce	= 2.0f;
		fDirUpKeyForceParamA =  0.0f;
		fDirUpKeyForceParamB =  0.0f;

		fBannerTwist    = 1.0f;
		fBannerTwistParamA = 1.3f;

		fBannerKeyTwist = 5.0f;
		fBannerKeyTwistParamA = 1.2f;
		fBannerKeyTwistParamB  = 0.0f;
		
		fBannerVecForce	= 3.0f;
		fBannerVecForceParamA = 0.2f;
		fBannerVecForceParamB = 0.8f;
		fBannerVecForceParamC = 64.0f;


		fBannerHeadForce= 3.5f;
		fSlidFricForce	= 2.0f;
		fRollFricForce	= 3.0f;
		fStartWec		= 2.5f;
		fMaxWec			= 3.5f;
		fBannerAngle    = 40.0f;
		fSuaiJianTwist  = 3.0f;
		fVecEffect		= 1.0f;
		fWecEffect		= 3.0f;
		fClockWec		= 1.0f;


	}

	tagDriftBaseParam( const tagDriftBaseParam & stParam)
	{
		fStartVec		= stParam.fStartVec;
		fEndVecFist		= stParam.fEndVecFist;
		fEndVecSecon	= stParam.fEndVecSecon;

		fDirKeyForce	= stParam.fDirKeyForce;
		fDirKeyForceParamA = stParam.fDirKeyForceParamA;
		fDirKeyForceParamB = stParam.fDirKeyForceParamB;

		fDirKeyTwist	= stParam.fDirKeyTwist;
		fDirKeyTwistParamA = stParam.fDirKeyTwistParamA;
		fDirKeyTwistParamB = stParam.fDirKeyTwistParamB;

		fDirUpKeyForce	= stParam.fDirUpKeyForce;
		fDirUpKeyForceParamA =  stParam.fDirUpKeyForceParamA;
		fDirUpKeyForceParamB =  stParam.fDirUpKeyForceParamB;


		fBannerKeyTwist= stParam.fBannerKeyTwist;
		fBannerKeyTwistParamA = stParam.fBannerKeyTwistParamA;
		fBannerKeyTwistParamB = stParam.fBannerKeyTwistParamB;

		fBannerTwist	= stParam.fBannerTwist;
		fBannerTwistParamA = stParam.fBannerTwistParamA;


		fBannerVecForce	= stParam.fBannerVecForce;
		fBannerVecForceParamA = stParam.fBannerVecForceParamA;
		fBannerVecForceParamB = stParam.fBannerVecForceParamB;
		fBannerVecForceParamC = stParam.fBannerVecForceParamC;


		fBannerHeadForce= stParam.fBannerHeadForce;
		fSlidFricForce	= stParam.fSlidFricForce;
		fRollFricForce  = stParam.fRollFricForce;
		fStartWec		= stParam.fStartWec;
		fMaxWec			= stParam.fMaxWec;
		fBannerAngle	= stParam.fBannerAngle;
		fSuaiJianTwist  = stParam.fSuaiJianTwist;
		fVecEffect		= stParam.fVecEffect;
		fWecEffect		= stParam.fWecEffect;
		fClockWec		= stParam.fClockWec;

	}

	tagDriftBaseParam & operator= ( const tagDriftBaseParam & stParam)
	{
		fStartVec		= stParam.fStartVec;
		fEndVecFist		= stParam.fEndVecFist;
		fEndVecSecon	= stParam.fEndVecSecon;
		fDirKeyForce	= stParam.fDirKeyForce;
		fDirKeyForceParamA = stParam.fDirKeyForceParamA;
		fDirKeyForceParamB = stParam.fDirKeyForceParamB;

		fDirKeyTwist	= stParam.fDirKeyTwist;
		fDirKeyTwistParamA = stParam.fDirKeyTwistParamA;
		fDirKeyTwistParamB = stParam.fDirKeyTwistParamB;

		fDirUpKeyForce	= stParam.fDirUpKeyForce;
		fDirUpKeyForceParamA =  stParam.fDirUpKeyForceParamA;
		fDirUpKeyForceParamB =  stParam.fDirUpKeyForceParamB;

		fBannerKeyTwist = stParam.fBannerKeyTwist;
		fBannerKeyTwistParamA = stParam.fBannerKeyTwistParamA;
		fBannerKeyTwistParamB = stParam.fBannerKeyTwistParamB;

		fBannerTwist			= stParam.fBannerTwist;
		fBannerTwistParamA		= stParam.fBannerTwistParamA;

		fBannerVecForce	= stParam.fBannerVecForce;
		fBannerVecForceParamA = stParam.fBannerVecForceParamA;
		fBannerVecForceParamB = stParam.fBannerVecForceParamB;
		fBannerVecForceParamC = stParam.fBannerVecForceParamC;

		fBannerHeadForce= stParam.fBannerHeadForce;
		fSlidFricForce	= stParam.fSlidFricForce;
		fRollFricForce  = stParam.fRollFricForce;
		fStartWec		= stParam.fStartWec;
		fMaxWec			= stParam.fMaxWec;
		fBannerAngle	= stParam.fBannerAngle;
		fSuaiJianTwist  = stParam.fSuaiJianTwist;
		fVecEffect		= stParam.fVecEffect;
		fWecEffect		= stParam.fWecEffect;
		fClockWec		= stParam.fClockWec;

		return (*this);
	}

	NxReal fStartVec;
	NxReal fEndVecFist;
	NxReal fEndVecSecon;

	NxReal fDirKeyForce;
	NxReal fDirKeyForceParamA;
	NxReal fDirKeyForceParamB;	
	
	
	NxReal fDirKeyTwist;
	NxReal fDirKeyTwistParamA;
	NxReal fDirKeyTwistParamB;

	NxReal fDirUpKeyForce;
	NxReal fDirUpKeyForceParamA;
	NxReal fDirUpKeyForceParamB; 


	NxReal fBannerKeyTwist;
	NxReal fBannerKeyTwistParamA;
	NxReal fBannerKeyTwistParamB;


	NxReal fBannerTwist;
	NxReal fBannerTwistParamA;


	NxReal fBannerVecForce;
	NxReal fBannerVecForceParamA;
	NxReal fBannerVecForceParamB;
	NxReal fBannerVecForceParamC;

	NxReal fBannerHeadForce;
	NxReal fSlidFricForce;
	NxReal fRollFricForce;
	NxReal fStartWec;
	NxReal fMaxWec;
	NxReal fBannerAngle;
	NxReal fSuaiJianTwist;
	NxReal fVecEffect;
	NxReal fWecEffect;
	NxReal fClockWec;


}DRIFTBASEPARAM , * PDRIFTBASEPARAM;


#include "TencentCar.h"

class CDriftMath
{
public:
	CDriftMath(void);
	~CDriftMath(void);

public:
	static bool InitDriftMath(CDriftCenter * pDriftCenter);
	static void CollectData(float fTime, CTencentCar::KEYINFO * pData);
	static void UnInitDriftMath();

public:
	static bool LoadDriftConfig();

protected:
	static void DoBannerAngle();
	static void DoBannerTwist();
	static void DoBannerVecForce();
	static void DoBannerHeadForce();
	static void DoUpKeyForce();
	static void DoDirKeyForce();
	static void DoDirKeyTwist();
	static void DoDampTwist();
	static void DoRollFrict();
	static void DoSlidFrict();
	static void DoRotateWec();
	static void DoStartWec();
	static void DoMaxWec();
	static void DoEndVecFist();
	static void DoEndVecSecon();
	static void DoStartVec();


public:
	static NxReal BannerAngle();
	static NxReal BannerTwist();
	static NxReal BannerVecForce();
	static NxVec3 BannerHeadForce();
	static NxVec3 UpKeyForce();
	static NxVec3 DirKeyForce();
	static NxReal DirKeyTwist();
	static NxReal DampTwist();
	static NxVec3 RollFrict();
	static NxVec3 SlidFrict();
	static NxReal StartWec();
	static NxReal MaxWec();
	static NxReal StartVec();
	static NxReal EndVecFist();
	static NxReal EndVecSecon();
	static NxReal ClockWec();

protected:
	static CTencentCar::KEYINFO * ms_pKeyInfo;
	static CDriftCenter			* ms_pCenter;

public:
	static PDRIFTBASEPARAM		  ms_pDriftBaseParam;


protected:
	static NxVec3 ms_vSlidForce;
	static NxVec3 ms_vRollForce;
	static NxVec3 ms_vUpKeyForce;
	static NxVec3 ms_vDirKeyForce;
	static NxReal ms_fBannerVecForce;
	static NxVec3 ms_vBannerHeadForce;
	static NxReal ms_fDirKeyTwist;
	static NxReal ms_fMaxWec;
	static NxReal ms_fStartWec;
	static NxReal ms_fEndVecFist;
	static NxReal ms_fEndVecSecon;
	static NxReal ms_fStartVec;
	static NxReal ms_fBannerAngle;
	static NxReal ms_fBannerTwist;
	static NxReal ms_fClockWec;

	
};
