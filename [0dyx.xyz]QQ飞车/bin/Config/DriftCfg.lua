
	DriftBaseParam =
	{	
		fStartVec			= 18.0;		--Ʈ����ʼ�ٶ�
		fEndVecFist			= 50.0;		--��һƮ�ƽ����ٶ� ָ��Ư�ٶȴ���50ʱ,������Ʈ���ٶ�
		fEndVecSecon		= 16.0;		--�ڶ�Ʈ�ƽ����ٶ� ָ��Ư�ٶ�С��50ʱ,������Ʈ���ٶ�
		fClockWec			= 0.5;
		
		
		fDirKeyForce		= 1.5;	
		fDirKeyForceParamA  = 4.0;
		fDirKeyForceParamB  = 10.0;
			
			--[[  
				�����Ҽ�ʱ �Գ���ʩ�����ҷ������ ��Ϊ DirKeyForce .  ע��: ���⳵ͷʱ DirKeyForce ��Ч
				���㹫ʽ :��(1)fDirKeyForce = fDirKeyForce + fDirKeyForceParamA * fVecSize / ( fDirKeyForceParamB + fVecSize) 
							(2)����ʩ�ӵ��� = NxVec3(0.0f, fDirKeyForce * NxMath::cos(fAngle), 0.0f)  
			  ]]
		
		fDirKeyTwist		= 5.9;	    --   ����Ť�ء�
		
		fDirKeyTwistParamA  = 0.5;
		fDirKeyTwistParamB  = 1.5;
			
			  --[[  
					NxReal FactorA  = 1.0f - NxMath::sin(��ͷ�� * 2 );
					fKeyTwist = ( fKeyRowTwist - fKeyTwistParamA ) * FactorA + fKeyTwistParamB;
				]]	
				
					
		fBannerTwist   		= 4.2;		-- ���ذ����Ť�ء����������ٶȷ��򡡸����̲��������෴��ʱ�������á���
		fBannerTwistParamA  = 1.3;		--   fBannerTwist * NxMath::pow(( 2.0f / NxPi * fAngle ) ,fBannerTwistParamA);
		
		
		fBannerKeyTwist     = 6.0;      --�� �ذ�������Ť��
		fBannerKeyTwistParamA = 1.2;
		fBannerKeyTwistParamB = 0.0;
		
			  --[[  
					NxReal fKeyTwistMin = fBannerKeyTwist - fKeyBannerTwistParamA;
					NxReal fKeyTwistMax = fBannerKeyTwist + fKeyBannerTwistParamB;
					fKeyTwist = fKeyTwistMin + (fKeyTwistMax - fKeyTwistMin) * ��ͷ�� * 2.0f / NxPi;
				]]	 
		
		fBannerVecForce		= 6.0;	
		fBannerVecForceParamA = 0.2;
		fBannerVecForceParamB = 0.8;
		fBannerVecForceParamC = 64.0;
		
		  --[[ 
				NxReal fFactor = fBannerVecForceParamA + 
				fBannerVecForceParamB * fVecSize * fVecSize /fBannerVecForceParamC ;

				NxReal fpFac = ( 4.0f / NxPi ) * ��ͷ�� - 1.0f;
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
				��ǰ���ʱ �Գ���ʩ��ǰ����� ��Ϊ DirKeyForce .  ע��: ���⳵ͷʱ DirKeyForce ��Ч
				���㹫ʽ :��(1)fDirKeyForce = fDirKeyForce + fDirKeyForceParamA * fVecSize / ( fDirKeyForceParamB + fVecSize)  ;
							(2)����ʩ�ӵ��� = NxVec3(0.0f, fDirKeyForce * NxMath::cos(fAngle), 0.0f);	
			]]
	}

	 