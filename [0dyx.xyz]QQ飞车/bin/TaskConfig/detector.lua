
--���������
EDT_Timer = 0;		--����ʱ
EDT_BNPress = 1;	--�������
EDT_BNPressTime = 2;--����ʱ����
EDT_ExecuteCount = 3;--ִ�д������
EDT_CDTimer = 4;	--����ʱ
EDT_Speed = 5;		--����ٶ��Ƿ�ﵽĳһֵ
EDT_Drift_Speed = 6;	--����Ƿ�Ư�Ʋ����ٶ���ĳһֵ֮��
EDT_TurnCar_Drift = 7;--����Ƿ�ذᳵͷ���ҵ�ʱ����Ư��״̬
EDT_SmallNo2 = 8;	--���С���Ƿ��ڼ���״̬

--����ֵ

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

--���
OPS_or = 0;     --����
OPS_and = 1;    --����



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
	
	
	
--���⿪ʼΪ���ֽ�ѧ3�ļ����	
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