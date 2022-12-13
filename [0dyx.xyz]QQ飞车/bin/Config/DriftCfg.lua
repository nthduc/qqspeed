
	DriftBaseParam =
	{	
		fStartVec			= 18.0;		--飘移起始速度
		fEndVecFist			= 50.0;		--第一飘移结束速度 指起漂速度大于50时,结束的飘移速度
		fEndVecSecon		= 16.0;		--第二飘移结束速度 指起漂速度小于50时,结束的飘移速度
		fClockWec			= 0.5;
		
		
		fDirKeyForce		= 1.5;	
		fDirKeyForceParamA  = 4.0;
		fDirKeyForceParamB  = 10.0;
			
			--[[  
				按左右键时 对车会施加左右方向的力 称为 DirKeyForce .  注意: 反扳车头时 DirKeyForce 无效
				计算公式 :　(1)fDirKeyForce = fDirKeyForce + fDirKeyForceParamA * fVecSize / ( fDirKeyForceParamB + fVecSize) 
							(2)最终施加的力 = NxVec3(0.0f, fDirKeyForce * NxMath::cos(fAngle), 0.0f)  
			  ]]
		
		fDirKeyTwist		= 5.9;	    --   正搬扭矩　
		
		fDirKeyTwistParamA  = 0.5;
		fDirKeyTwistParamB  = 1.5;
			
			  --[[  
					NxReal FactorA  = 1.0f - NxMath::sin(车头角 * 2 );
					fKeyTwist = ( fKeyRowTwist - fKeyTwistParamA ) * FactorA + fKeyTwistParamB;
				]]	
				
					
		fBannerTwist   		= 4.2;		-- 　回搬减速扭矩　当车　角速度方向　跟键盘操作方向相反　时会起作用　　
		fBannerTwistParamA  = 1.3;		--   fBannerTwist * NxMath::pow(( 2.0f / NxPi * fAngle ) ,fBannerTwistParamA);
		
		
		fBannerKeyTwist     = 6.0;      --　 回搬增加速扭矩
		fBannerKeyTwistParamA = 1.2;
		fBannerKeyTwistParamB = 0.0;
		
			  --[[  
					NxReal fKeyTwistMin = fBannerKeyTwist - fKeyBannerTwistParamA;
					NxReal fKeyTwistMax = fBannerKeyTwist + fKeyBannerTwistParamB;
					fKeyTwist = fKeyTwistMin + (fKeyTwistMax - fKeyTwistMin) * 车头角 * 2.0f / NxPi;
				]]	 
		
		fBannerVecForce		= 6.0;	
		fBannerVecForceParamA = 0.2;
		fBannerVecForceParamB = 0.8;
		fBannerVecForceParamC = 64.0;
		
		  --[[ 
				NxReal fFactor = fBannerVecForceParamA + 
				fBannerVecForceParamB * fVecSize * fVecSize /fBannerVecForceParamC ;

				NxReal fpFac = ( 4.0f / NxPi ) * 车头角 - 1.0f;
				ms_fBannerVecForce = ms_pDriftBaseParam->fBannerVecForce * fFactor * fpFac;
			]]	
		
		fBannerHeadForce	= 5.5;
		fSlidFricForce		= 1.2;	   --   NxMath::sin( fAngle ) * vMaxFircSiz
		fRollFricForce		= 1.0;	   --   NxMath::cos( fAngle ) * vMaxFircSiz
		fStartWec			= 0.8;	
		fMaxWec				= 3.5;	
		fBannerAngle    	= 45.0;	
		fSuaiJianTwist  	= 5.0;	
		fVecEffect			= 1.0;
		fWecEffect			= 3.7;
		
		
		fDirUpKeyForce		= 10.0;	--     ( fFactorB / (fUpKeyForce * fUpKeyForce) ) * fVecSize * fVecSize + fFactorB + fFactorA;
		fDirUpKeyForceParamA  = 2.0;
		fDirUpKeyForceParamB  = 14.0;
		
			--[[  
				按前向键时 对车会施加前向的力 称为 DirKeyForce .  注意: 反扳车头时 DirKeyForce 无效
				计算公式 :　(1)fDirKeyForce = fDirKeyForce + fDirKeyForceParamA * fVecSize / ( fDirKeyForceParamB + fVecSize)  ;
							(2)最终施加的力 = NxVec3(0.0f, fDirKeyForce * NxMath::cos(fAngle), 0.0f);	
			]]
	}

	 