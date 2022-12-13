KartPhysParam =
{
	{
		PID = 1;				--普通氮气
		AffectID= 100;						--无用参数
		Param = 6;				--加速度大小
		LifeTime = 4000;		--加速度时间
	};

	{
		PID = 2001;				-- 小喷
		AffectID= 100;						--无用参数
		Param = 3;				--加速度大小
		LifeTime = 500;			--加速度时间
	};
		
	{
		PID = 2002;				-- 组队氮气
		AffectID= 100;						--无用参数
		Param = 7;				--加速度大小
		LifeTime = 5000;		--加速度时间
	};
	
	{
		PID = 99999;			--集气
		AffectID= 99999;					--无用参数
		Param = 80;				--集气系数
		LifeTime = 5000;					--无用参数
	};
	
};


CompensateParam = 
{
	{
		fPercent = 0.05;
		fValue    = 1.0;
	};
	
	{
		fPercent = 0.1;
		fValue    = 1.01;
	};
	
	{
		fPercent = 0.2;
		fValue    = 1.02;
	};
	
	{
		fPercent = 0.3;
		fValue    = 1.03;
	};
	
	{
		fPercent = 0.4;
		fValue    = 1.04;
	};
	
	{
		fPercent = 0.5;
		fValue    = 1.05;
	};
	
	{
		fPercent = 0.6;
		fValue    = 1.06;
	};
};

CollisionBoxParam = 
{
     BoxLength = 0.35;
     BoxWidth  = 0.208;
     BoxHeight = 0.25;
     AdjustOffset = 0.31;
     AdjustPickGap = 0.2;
     AdjustRemoteOffset = 0.31;
     AdjustBevelOffset = 0.28;
};

CarToCarCollisionParam = 
{
     CollisionRadius = 0.39;
     CollisionLength = 0.372;
     CollisionWidth = 0.216;
     CollisionHeight = 0.35;
};

TurnPhysParam = 
{
    BaseTurnRate = 1.05;
    VolatileTurnRate = 1.15;
    MaxTurnSpeed = 16.0;
    MinTurnSpeed = 2.7;
};

GravityParam = 
{
    Gravity = -15.0;
};
