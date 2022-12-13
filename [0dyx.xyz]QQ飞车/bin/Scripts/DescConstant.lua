
	DescConstant.KarBaseParam[DescConstant.KarType].vMass = 2000;
	DescConstant.KarBaseParam[DescConstant.KarType].vDigitalSteeringDelta = 0.2;
	DescConstant.KarBaseParam[DescConstant.KarType].vSteeringMaxAngle = 2;
	DescConstant.KarBaseParam[DescConstant.KarType].vMoterForce = 580;
	DescConstant.KarBaseParam[DescConstant.KarType].vMaxVelocity = 400;
	DescConstant.KarBaseParam[DescConstant.KarType].vCameraDistance = 8.0;
	DescConstant.KarBaseParam[DescConstant.KarType].vTransmissionEfficiency = 0.9;
	DescConstant.KarBaseParam[DescConstant.KarType].vDifferentialRatio = 25;
	
	
	DescConstant.KarBaseParam[DescConstant.KarType].motorMinRpmToGearDown = 2500 * 4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].motorMaxRpmToGearUp = 6000 * 4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].motorMinRpm = 1500 * 4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].motorMaxRpm = 7000 * 4.5;

	DescConstant.KarBaseParam[DescConstant.KarType].gearNbForwardGears = 9;
	DescConstant.KarBaseParam[DescConstant.KarType].gearBackwardGearRatio = -2.9;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[0] = 2.66;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[1] = 3.78;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[2] = 4.68;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[3] = 4.40;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[4] = 3.90;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[5] = 3.60;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[6] = 3.40;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[7] = 3.20;
	DescConstant.KarBaseParam[DescConstant.KarType].gearForwardGearRatios[8] = 2.70;

	
--	DescConstant.KarBaseParam[DescConstant.KarType].wheelWidth = 
--	DescConstant.KarBaseParam[DescConstant.KarType].wheelSuspension = 
	DescConstant.KarBaseParam[DescConstant.KarType].wheelApproximation = 10;
	DescConstant.KarBaseParam[DescConstant.KarType].wheelSpringRestitution = 4000;
	DescConstant.KarBaseParam[DescConstant.KarType].wheelSpringDamping = 80;
	DescConstant.KarBaseParam[DescConstant.KarType].wheelSpringBias = 0.001;
	DescConstant.KarBaseParam[DescConstant.KarType].wheelmaxBrakeForce = 1;
	DescConstant.KarBaseParam[DescConstant.KarType].wheelFrictionToFront = 0.1;
	DescConstant.KarBaseParam[DescConstant.KarType].wheelFrictionToSide1 = 0.1;
	DescConstant.KarBaseParam[DescConstant.KarType].wheelFrictionToSide2 = 0.99;

	DescConstant.KarBaseParam[DescConstant.KarType].radiusCoe1 = 1.0;
	DescConstant.KarBaseParam[DescConstant.KarType].radiusCoe2 = 1.8;
	DescConstant.KarBaseParam[DescConstant.KarType].radiusCoe3 = 0.1;
	DescConstant.KarBaseParam[DescConstant.KarType].radiusCoe4 = 0.06;
	DescConstant.KarBaseParam[DescConstant.KarType].radiusCoe5 = 1.0;	

	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[0].x = 1000*4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[0].y = 493*1.54;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[1].x = 2000*4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[1].y = 534*1.54;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[2].x = 4000*4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[2].y = 575*1.54;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[3].x = 5000*4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[3].y = 575*1.54;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[4].x = 6000*4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[4].y = 466*1.54;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[5].x = 7000*4.5;
	DescConstant.KarBaseParam[DescConstant.KarType].torqueCurCoe[5].y = 400*1.54;
	
	
-- ���ٶ����߲�������ͼ hover 
--										A
--										|
--										| m_fRadioUp_NormalAccel
--										|    |
--				m_fAcel_Vfu_NormalAccel	|    | * 
--				*		*	  *  	  *	|     *   *  
--			    	    				|    *       *
--				*	    	  		 m_fAcel_OriForward * --- m_fRadioDown_NormalAccel       
--			m_fAcel_Vfu_NonePress	 	*  *               *
--				*   				  	|                    *   m_fVMax_NormalAccel
--			____ *______________________|_______________________*___________________ V
--	  									|��m_fVTopAccel_NormalAccel                  
--				* 	*	*	*	  *  	*	   *                 *  *
--	  							m_fAcel_OriBack  *              *    *
--	  		     						|            *           *      *	m_fAcel_NonePress
--	  		  							|               *        *        *
--	  	  								|                  *	 *				 
--						m_fRadio_DeAccel *   *
--	   			     											*  m_fDeAcelMax = m_fRadio_DeAccel * m_fVMax_NormalAccel + m_fAcel_OriBack
--
--																m_fAcel_AcelToNormal
--	
	DescConstant.KarBaseParam[DescConstant.KarType].m_fVMax_NormalAccel			= 14.0; --����ٶ�
	DescConstant.KarBaseParam[DescConstant.KarType].m_fVMax_UseItemAccel		= 17.0; --�������ٶ�

	--DescConstant.KarBaseParam[DescConstant.KarType].m_fVMax_NormalAccel			= 18.0;
	--DescConstant.KarBaseParam[DescConstant.KarType].m_fVMax_UseItemAccel		= 30.0;

	DescConstant.KarBaseParam[DescConstant.KarType].m_fVTopAccel_NormalAccel	= 3.0;
	DescConstant.KarBaseParam[DescConstant.KarType].m_fVTopAccel_UseItemAccel	= 3.0 ;

	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_OriForward			= 3.0 ;	
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_OriBack				=-3.0 ;		
--[[
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcelMax_NormalAccel		= 8.0;	
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcelMax_UseItemAccel		= 12.0;	
	DescConstant.KarBaseParam[DescConstant.KarType].m_fDeAcelMax				= -16.0; 

	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_Vfu_NormalAccel     = 3.0;
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_Vfu_UseItemAccel	= 6.0;

	DescConstant.KarBaseParam[DescConstant.KarType].m_fVMin						= -20.0; 

	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_NonePress			= -2;		 
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_Vfu_NonePress		= 2;

	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_AcelToNormal		= -9.0;
	]]
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcelMax_NormalAccel		= 8.0;	
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcelMax_UseItemAccel		= 12.0;	
	DescConstant.KarBaseParam[DescConstant.KarType].m_fDeAcelMax				= -36.0; --�޸ĺ������

	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_Vfu_NormalAccel     = 3.0;
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_Vfu_UseItemAccel	= 6.0;

	DescConstant.KarBaseParam[DescConstant.KarType].m_fVMin						= -20.0; 

	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_NonePress			= -0.5;		--�Զ����� 
	DescConstant.KarBaseParam[DescConstant.KarType].m_fAcel_Vfu_NonePress		= 36.0;  --������
	
	
		--����Ư��
		
	DescConstant.KarBaseParam[DescConstant.KarType].bDreamDrifting				= 0;
	
	DescConstant.KartRotate   = 0.2;  -- ת��
	DescConstant.KartRotate1 = 0.6; -- ��ͷ��
	
	
	
	DescSendPkg.SendPkg[0]._begin_dis = 0;
	DescSendPkg.SendPkg[0]._end_dis = 20;
	DescSendPkg.SendPkg[0]._frequence = 15;
	
	DescSendPkg.SendPkg[1]._begin_dis = 20;
	DescSendPkg.SendPkg[1]._end_dis = 30;
	DescSendPkg.SendPkg[1]._frequence = 10;
	
	DescSendPkg.SendPkg[2]._begin_dis = 30;
	DescSendPkg.SendPkg[2]._end_dis = 200;
	DescSendPkg.SendPkg[2]._frequence = 5;
	
	DescSendPkg.SendPkg[3]._begin_dis = 200;
	DescSendPkg.SendPkg[3]._end_dis = 500;
	DescSendPkg.SendPkg[3]._frequence = 2;	
	
	DescSendPkg.SendPkg[4]._begin_dis = 500;
	DescSendPkg.SendPkg[4]._end_dis = 20000;
	DescSendPkg.SendPkg[4]._frequence = 1;		

--���������
	--��ͨ���
	DescCamera.cameraNormalHeight = 0.80;				--����߶�
	DescCamera.cameraNormalMaxDis = 2.50;				--�����룬����ֵ��
	DescCamera.cameraNormalMinDis = 2.18;				--��С���룬����ֵ��
	DescCamera.cameraNormalFocusDis = 2.00;			--�ӵ����
    DescCamera.cameraNormalCoef = 0.05;        		--��������ٶȱ仯�Ĳ�����ֵԽ�󣬺��ٶ�֮��Խ���� 
    DescCamera.cameraNormalInterp = 0.02;            --�����ֵ�仯������ֵԽ��仯Խ�������ӦԽѸ�� 
	
	--����ʱ���
	DescCamera.cameraSpeedUpHeight = 0.80;			--����߶�	
	DescCamera.cameraSpeedUpMaxDis = 3.2;--2.6;				--�����룬����ֵ��
	DescCamera.cameraSpeedUpMinDis = 2.4;				--��С���룬����ֵ��
	DescCamera.cameraSpeedUpFocusDis = 5.6;			--�ӵ����
	DescCamera.cameraSpeedUpCoef = 0.5;     		--����ʱ��������ٶȱ仯�Ĳ�����ֵԽ�󣬺��ٶ�֮��Խ���� 
	DescCamera.cameraSpeedUpInterp = 2;             --����ʱ�����ֵ�仯������ֵԽ��仯Խ�������ӦԽѸ�� 
	
	
	DescCamera.cameraMaxSpeed = 45;                 -- ���˶�������ٶȣ����ڲ�ֵ��ƽ�������λ�ñ仯 
	DescCamera.cameraHeightIterp = 0.02;			--����ĸ߶Ȳ�ֵ�����Ա仯 
	DescCamera.cameraFocusIterp = 0.01;             --������ӵ��ֵ�����Ա仯 
	
--	DescCamera.cameraSpeedUpRatio = 1.7;				--��������ָ�����,ֵԽС�ָ�Խ��

	DescCamera.CAMERA_FAR = 2200.0;				--Զ������
	DescCamera.CAMERA_NEAR = 0.5;					--��������
	DescCamera.CAMERA_AspectRatio = 1.33333;				--�߿����
	DescCamera.CAMERA_FOV = 60;					--���


	GameSetting.ShowShadow = true;			--�Ƿ���ʵӰ��
	GameSetting.SpeedDispCoef = 14.0;		--��ʾ�ٶȱ���
	GameSetting.SpeedLineShowVelo = 120.0;	--��ʾ�ٶ��������ٶȴ�С

	--��ǽ��ײ��Ϊ����
	ContactConfig.smallangle = 35.0;
	ContactConfig.midangle = 90.0;
	ContactConfig.bigangle = 135.0;
	ContactConfig.inversevelocityrate = 0.5;
	
	--������������
	PhysSceneConfig.gravity = -9.8;
	
	
	
	--���복��ײ����
	-- �Ƕ�Ϊ�Ƕ���
	--������ײ�����복��ǰ��ļнǵľ���ֵalpha,������������
	--��������, ����Ƕ�Ϊ��0��m_fAlpha

	DescConstant.KartCollisionParam.m_fBackSpeedCoef = 0.1;-- �ٶ����ϵ��

	-- ��΢��ת��ͷ����ǰ��, �ٶ�û����ʧ, ����Ƕ�Ϊm_fAlpha��m_fAlpha1, �и�ת����ٶ�,��һ�����ٶ���ʧ
	DescConstant.KartCollisionParam.m_fAlpha = 60.0;
	DescConstant.KartCollisionParam.m_fAlpha1= 75.0;
	DescConstant.KartCollisionParam.m_fTurnSpeed = 30;
	DescConstant.KartCollisionParam.m_fLinearSpeedLostRate = 0.2;
	
	-- ��m_fAlpha1��m_fAlphaDrift1����Ư��
	DescConstant.KartCollisionParam.m_fAlphaDrift1 = 105;
	-- ��m_fAlphaDrift1��m_fAlpha2,����ת��
	DescConstant.KartCollisionParam.m_fAlpha2 = 120;
	DescConstant.KartCollisionParam.m_fTurnSpeed1 = 30;
	DescConstant.KartCollisionParam.m_fLinearSpeedLostRate1 = 0.2;

	--��m_fAlpha2��180, ������ǰ�ƶ�
	DescConstant.KartCollisionParam.m_fForwardSpeedCoef = 1.1;	
	
	
	
	-- Ħ�����趨
	DescConstant.KartFriction.m_fFricFactor = 0.5;
	DescConstant.KartFriction.m_fRollFriction = 0.2;
	DescConstant.KartFriction.m_fSlideFriction = 1.2;
	
	
	--��������ϵͳ
	DescSuspension.suspension.sim_vPosition.x = 500.0;
	DescSuspension.suspension.sim_vPosition.y = 500.0;
	DescSuspension.suspension.sim_vPosition.z = -500.0;

	DescSuspension.suspension.sim_vSpecChassis.x = 0.215;
	DescSuspension.suspension.sim_vSpecChassis.y = 0.215;
	DescSuspension.suspension.sim_vSpecChassis.z = 0.01;

	DescSuspension.suspension.sim_vSpecBase.x = 0.2;
	DescSuspension.suspension.sim_vSpecBase.y = 0.2;
	DescSuspension.suspension.sim_vSpecBase.z = 1.5;

	DescSuspension.suspension.sim_fSpecWheelRadius = 0.041;
	DescSuspension.suspension.sim_fMassChassis = 1500;
	DescSuspension.suspension.sim_fMassWheel = 400;

	DescSuspension.suspension.sim_fZBases = 0.0;
	
	DescSuspension.suspension.sim_fZChassis = 0.6;
	DescSuspension.suspension.sim_fAnchorChassis = 0.6;
	DescSuspension.suspension.sim_fZPillar = 0.6;
	DescSuspension.suspension.sim_fAnchorZPillar = 0.6;

	DescSuspension.suspension.sim_fAnchorZSuspensions = 0.5;
	
	DescSuspension.suspension.sim_fZWheels = 0.5;
	
	DescSuspension.suspension.sim_fSusSpring = 125.0;
	DescSuspension.suspension.sim_fSusDamping = 0.8;
	DescSuspension.suspension.sim_fSusValue = 0.05;
	DescSuspension.suspension.sim_fSusRestitution = 0.51;
	DescSuspension.suspension.sim_fSusLimitPlane = 0.107;

	
	--           end ��������ϵͳ
