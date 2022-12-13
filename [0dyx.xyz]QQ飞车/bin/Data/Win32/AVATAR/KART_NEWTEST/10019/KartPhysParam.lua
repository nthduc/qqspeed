KartPhysParam =
{
	{
		PID = 1;				--��ͨ����
		AffectID= 100;						--���ò���
		Param = 6;				--���ٶȴ�С
		LifeTime = 4000;		--���ٶ�ʱ��
	};

	{
		PID = 2001;				-- С��
		AffectID= 100;						--���ò���
		Param = 3;				--���ٶȴ�С
		LifeTime = 500;			--���ٶ�ʱ��
	};
		
	{
		PID = 2002;				-- ��ӵ���
		AffectID= 100;						--���ò���
		Param = 7;				--���ٶȴ�С
		LifeTime = 5000;		--���ٶ�ʱ��
	};
	
	{
		PID = 99999;			--����
		AffectID= 99999;					--���ò���
		Param = 80;				--����ϵ��
		LifeTime = 5000;					--���ò���
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
     BoxLength = 0.4;
     BoxWidth  = 0.2;
     BoxHeight = 0.25;
     AdjustOffset = 0.1;
     AdjustPickGap = 0.2;
     AdjustRemoteOffset = 0.33;
     AdjustBevelOffset = 0.38; 
};

CarToCarCollisionParam = 
{
     CollisionRadius = 0.48;
     CollisionLength = 0.462;
     CollisionWidth = 0.208;
     CollisionHeight = 0.35;
};

TurnPhysParam = 
{
    BaseTurnRate = 1.05;
    VolatileTurnRate = 1.25;
    MaxTurnSpeed = 11.0;
    MinTurnSpeed = 2.7;
};

GravityParam = 
{
    Gravity = -20.0;
};