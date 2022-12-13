	--比赛中跟随相机 
	RaceCam =
	{	
		NearPlane   	= 0.1;  		--远剪裁面
		FarPlane 		= 2000.0;    		--近剪裁面
		AspectRatio     = 1.333;  		--高宽比率
		Fov				= 1.08; 			--广角
		
	
	    --初始静止时参数 
		MinDis 			= 1.5;	     		--相机离车的最小距离
		MaxDis          = 3.0;              --相机离车的最大距离
		Height 			= 0.9;	            --相机离地面的高度
		RotAngle		= 0.315;				--绕右方向旋转角度(相机的倾角）
		
		--运动中的插值参数 
		SpeedCoef		= 0.12;	            --速度参数(相机远近和速度成正比） 
		DistCoef		= 1;	            --距离参数（相机位置变化的快慢和距离成正比） 
		HeightCoef      = 0.64;              --高度参数（相机高度的变化和此参数相关，线性）
		RotCoef			= 0.05;	        	--绕右方向旋转角度参数（相机的倾斜角度和距离成正比，距离乘于此参数） 
		AngleScope		= 1.2;				--相机和模型保持在此角度内
		DriftCoef		= 0.004;            --漂移时相机会更近

        ForwardCoef1    = 0.42;              --前进方向角度插值系数1（趋势，unit）（相机和模型夹角越大时相机变化越大）
        ForwardCoef2    = 4.0;              --前进方向角度插值系数2（scale）（相机和模型夹角越大时相机变化越大）
        ForwardCoef3    = 0.32;             --角速度拟和植（目的是当角速度越小的时候，相机变化趋势越大）
		UpCoef1			= 0.4;				--上方向角度插值系数1（趋势，unit）
		UpCoef2			= 0.8;             	--上方向角度插值系数2（scale）
		
		HeightRef		= 0.05;
	};
	
	
	
	--鸟瞰相机 
	AirScapeCam =
	{
		NearPlane   	= 0.5;  		--远剪裁面
		FarPlane 		= 2200.0;    		--近剪裁面
		AspectRatio     = 1.333;  		--高宽比率
		Fov				= 0.78; 			--广角
		
		Height			= 12.0;			--相机高度
	}; 
		
		
    	--小地图相机
	EagleMapCam =
	{
		NearPlane   	= 0.5;  		--远剪裁面
		FarPlane 		= 2200.0;    		--近剪裁面
		AspectRatio     = 1.333;  		--高宽比率
		Fov				= 0.78; 			--广角

		Height			= 16;			--相机高度
	};

     	--模型相机
	ModelCam =
	{
		NearPlane   	= 0.5;  		--远剪裁面
		FarPlane 		= 2200.0;    		--近剪裁面
		AspectRatio     = 1.333;  		--高宽比率
		Fov				= 0.78; 			--广角

	};
	 
