
local x = 6.8;
local y = -0.2;
MotorSetting = 
{
	AccelStatus = 
	{
		1.20, 1.2, 0.8, 0.3, 2.0, 1.9,
	},
	ForwardAccel = 
	{
		{0 	       , x+y,},
		{1 	       , x+y,},
		{2 	       , x+y,},
		{3 	       , x+y,},
		{4 	       , x+y,},
		{5 	       , x+y,},
		{6 	       , x+y,},
		{7 	       , x+y,},
		{8 	       , x+y,},
		{9 	       , x+y,},
		{10 	       , x,},
		{11 	       , x+0.1,},
		{12 	       , x+0.2,},
		{13 	       , 0.32172 + x,},
		{14 	       , 0.14535 + x,},
		{15 	       , -0.50089 + x,},
		{16 	       , -0.68565 + x,},
		{17 	       , -1.29676 + x,},
		{18 	       , -10,},
		{19 	       , -20,},
		{20 	       , -100,},
	--[[	
		{0 	         ,5.0	       ,},
		{0.44737 	 ,5.514772 ,  },
		{1.52105 	 ,6.32671 ,  },
		{2.59474 	 ,7.54275 ,  },
		{3.66842 	 ,8.46868 ,  },
		{4.74211 	 ,5.9606  , },
		{5.81579 	 ,4.05795 ,  },
		{6.88947 	 ,3.28342 ,  },
		{9.03684	 ,2.42617 , },
		{14.18421	 ,2.19066    ,  },
		{15.47895	 ,0   , },
		{16.62632	 ,-2   , },
		{17.0	, -5	, },
		{18.0	, -10	, },
		{19.0	, -100.0, },		
		]]
		--[[
		{9.03684	 ,12.12617 , },
		{10.11053	 ,9.70538   ,},
		{11.18421	 ,7.3366     ,  },
		{12.25789	 ,5.15907   ,},
		{13.33158	 ,3.2953     ,  },
		{14.40526	 ,1.85112   , },
		{15.47895	 ,0.91564   , },
		{16.55263	 ,0.5613     ,  },
		{17.62632	 ,0.84382   , },
		{18.7	 ,1.80222   , },
		]]
	},                                
	ForwardDecel =   
	{
		{
			0, -8,
		},
		{
			2, -10,
		},		
		{
			6, -15,
		},						
		{
			18.7, -20,
		},			
	},
	BackwardAccel = 
	{
		{
			0, -5.5,
		},
		{
			1.2, -5.5,
		},
		{
			2.2, -5.5,
		},
		{
			3.6, -5.5,
		},
		{
			4.6, -5.5,
		},
		{
			5.6, -5.5,
		},
		{
			10, -4.2,
		},
		{
			12, 0,
		},
	},
	BackwardDecel = 
	{
		{
			0, 6,
		},
		{
			1, 8,
		},
		{
			2, 9,
		},
		{
			4, 14,
		},		
		{
			6, 18,
		},
	},
};