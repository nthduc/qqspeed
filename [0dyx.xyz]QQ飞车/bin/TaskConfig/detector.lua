
--监测器类型
EDT_Timer = 0;		--倒计时
EDT_BNPress = 1;	--按键检测
EDT_BNPressTime = 2;--按键时间检测
EDT_ExecuteCount = 3;--执行次数检测
EDT_CDTimer = 4;	--倒计时
EDT_Speed = 5;		--检测速度是否达到某一值
EDT_Drift_Speed = 6;	--检测是否漂移并且速度在某一值之上
EDT_TurnCar_Drift = 7;--检测是否回搬车头并且当时处于漂移状态
EDT_SmallNo2 = 8;	--检测小喷是否处于激发状态

--按键值

key_up = 17;
key_left = 30;
key_down = 31;
key_right = 32;
key_shift = 42;
key_ctrl = 29;
--key_a = 30;
--key_s = 31;
--key_d = 32;
--key_w = 17;
--key_rshift = 54;
--key_rctrl = 107;

--异或
OPS_or = 0;     --或者
OPS_and = 1;    --并且



detector =
{
	{
		id = 1;
		type = EDT_Timer;
		params =
		{
			life = 1000;
		};
	};
	
	{
	    id = 2;
	    type = EDT_BNPressTime;
	    params =
	    {
	        life = 3000;
	        keys =
	        {
	            key_up,
	        };
	    };
	};
	
	{
	    id = 3;
     	type = EDT_BNPressTime;
	    params =
	    {
	        life = 3000;
	        keys =
	        {
	            key_down,
	        };
	    };
	};
	
	{
	    id = 4;
     	type = EDT_BNPressTime;
	    params =
	    {
	        life = 3000;
	        keys =
	        {
	            key_up,
	            key_left,
	        };
	    };
	};
	
	{
	    id = 5;
     	type = EDT_BNPressTime;
	    params =
	    {
	        life = 3000;
	        keys =
	        {
	            key_up,
	            key_right,
	        };
	    };
	};
	
	{
	    id = 6;
    	type = EDT_BNPressTime;
	    params =
	    {
	        life = 3000;
	        keys =
	        {
	            key_shift,
	        };
	    };
	};
	
	{
	    id = 7;
    	type = EDT_BNPressTime;
	    params =
	    {
	        life = 3000;
	        keys =
	        {
	            key_ctrl,
	        };
	    };
	};
	
	{
	    id = 8;
    	type = EDT_Timer;
	    params =
	    {
	        life = 1500;
	    };
	};
	
	{
		id = 9;
		type = EDT_ExecuteCount;
		params =
		{
			life = 1;
		};
	};
	
	{
	    id = 10;
    	type = EDT_Timer;
	    params =
	    {
	        life = 500;
	    };
	};
	
	{
		id = 11;
		type = EDT_CDTimer;
		params =
		{
			life = 100000;
		};
	};
	
	{
		id = 12;
		type = EDT_Timer;
		params =
		{
			life = 3000;
		};
	};
	
	
	
--从这开始为新手教学3的检测器	
	{
		id = 13;
		type = EDT_Speed;
		params =
		{
			targetspeed = 100;
		};
	};
	
	{
		id = 14;
		type = EDT_Drift_Speed;
		params =
		{
			targetspeed = 60;
			key1 = key_shift;
			key2 = key_left;
		};
	};
	
	{
		id = 15;
		type = EDT_TurnCar_Drift;
		params =
		{
			key1 = key_up;
			key2 = key_right;
		};
	};
	
	{
		id = 16;
		type = EDT_SmallNo2;
	};
};