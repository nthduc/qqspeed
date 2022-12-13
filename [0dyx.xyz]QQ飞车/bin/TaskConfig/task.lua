TaskPack =
{
	{
	    Id = 1000;
	    TaskList=
	    {
	        1,
	        2,
	    };
	    Desc = "pack";
	    
	};
}

EAMT_DialoggBox = 0;	--对话框
EAMT_ScreenTex = 1;	--屏幕贴图
EAMT_BlinkTex = 2;		--闪耀的贴图
EAMT_Effect = 3;		--特效
EAMT_Anim = 4;		--动画
EAMT_Sound = 5;		--声音
EAMT_ScreenEff = 6;		--屏幕特效（由CGameEffectMgt激发）
EAMT_LockKey = 7;		--锁键盘
EAMT_CDTimerShow = 8;	--显示倒计时时间
EAMT_KeyBlink = 9;		--键盘闪烁
EAMT_StartNo2 = 10;	--激活小喷
EAMT_ScreenFont =11;	--屏幕字体
EAMT_Esc = 12;		--Esc按键，请求服务器完成任务
EAMT_SwitchTex = 13;	--类似于开关的原子，start时显示某一tex，stop时显示另外的tex

--任务执行的类型
EPT_NeverBack = 0;		--永不回头的
EPT_Circle = 1;			--失败可回头重做的	

--开关状态
SST_On = 0;	--开
SST_Off = 1;	--关

--[[
GetTabVolum = function(tab)
	local volum = 0;
	for ele in tab() do
	    volum += volum;
	end;
end;
]]--

Task =
{
	{
		Id = 1;    			--任务id 
  		MapId = 109;        --地图id 
		Desc = "操作练习";  --任务介绍 
		--开始任务需要的key 
		PreKey=
		{
			1;
		};
		--开关表
		SwitchTable =
		{
			acceplane		= SST_On;
			countdowntime	= SST_Off;
                        eaglemap		= SST_Off;
		        elapsetime	= SST_Off;
		        itemcolumn	= SST_Off;
		        playerrank		= SST_Off;
		        singlepowerctrl	= SST_Off;
		        teampowerctrl	= SST_Off;
		        reset		       	= SST_Off;
		};  
		
		TaskPeriodAtom =		--整个任务周期内的表现原子
		{
			{
				Type = EAMT_ScreenTex;  --屏幕纹理
				Param = 32;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --屏幕纹理
				Param = 35;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --屏幕纹理
				Param = 38;              --参数
			};

			
			{
				Type = EAMT_ScreenTex;  --屏幕纹理
				Param = 41;              --参数
			};
		};
		SubTask =           --子任务链表 
		{
	    
			{--subtask1
				Validation = 110;        --非零时发送到服务器验证
				AtomLevelList =        --原子层链表 
				{
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};
						};
					};
			
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};
							
							{
								Type = EAMT_Sound;   --播放声音
								Param = 125;              --参数
							};
							
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 5123;              --参数
							};
						};
					};
					
					{
						DetectorId = 2;--检测器id
						AtomList =     --原子链表
						{
							{--闪烁上键
								Type = EAMT_KeyBlink;   --原子类型
								Param = 31;              --参数
								Option = 33;
							};
						};
					};
					
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};
					    	
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 6147;              --参数
							};
						};
					};
					
				};
			};
			
			{--subtask2，按down键
				Validation = 111;
				AtomLevelList =
				{
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};
							
							{
								Type = EAMT_Sound;   --播放声音
								Param = 126;              --参数
							};
							
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 5379;              --参数
							};
						};
					};
					
					{
						DetectorId = 3;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_KeyBlink;    --原子类型
								Param = 34;               --参数
								Option = 36; 
							};
						};
					};
					
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};

							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 6147;              --参数
							};
						};
					};
				};
			};
			
			{--subtask3，按左上键
				Validation = 112;
				AtomLevelList =
				{
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};
							
							{
								Type = EAMT_Sound;   --播放声音
								Param = 127;              --参数
							};
							
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 5635;              --参数
							};
						};
					};

					{
						DetectorId = 4;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_KeyBlink;   --原子类型
								Param = 31;              --参数
								Option = 33;
							};                          
					    	
							{
								Type = EAMT_KeyBlink;     --原子类型
								Param = 37;
								Option = 39;
							};
					    };
					};
					
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};

							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 6147;              --参数
							};
						};
					};
				};
			};

			{--subtask4
				Validation = 113;
				AtomLevelList =
				{
					{
						DetectorId = 8;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};
							
							{
								Type = EAMT_Sound;   --播放声音
								Param = 128;              --参数
							};

							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 5891;              --参数
							};
						};
					};
					
					{
						DetectorId = 5;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_KeyBlink;   --原子类型
								Param = 31;              --参数
								Option = 33;
							};
					    	
							{
								Type = EAMT_KeyBlink;
								Param = 40;
								Option = 42;
							};
						};
					};
					
					{
						DetectorId = 10;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 6147;              --参数
							};
						};
					};
				};
			};
		};
		
		CanStart = function()
		end;

		OnStart = function()
		end;
		
		OnEnd = function()
		end;
	};
--任务2	
	{
		Id = 2;    		   --任务id 
  		MapId = 109;        --地图id 
		Desc = "漂移练习";   --任务介绍 
		--开始任务需要的key 
		PreKey=
		{
			1;
		};
		
		SwitchTable =
		{
			acceplane		= SST_On;
			countdowntime	= SST_Off;
                        eaglemap		= SST_Off;
		        elapsetime	= SST_Off;
		        itemcolumn	= SST_Off;
		        playerrank		= SST_Off;
		        singlepowerctrl	= SST_Off;
		        teampowerctrl	= SST_Off;
		        reset		       	= SST_Off;
		};  
		
		TaskPeriodAtom =
		{	--按键背景图
			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 9;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 12;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 15;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 18;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 21;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 24;              --参数
			};

			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 27;              --参数
			};
			
			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 30;              --参数
			};
			
			--[[{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 43;              --参数
			};
			
			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 45;              --参数
			};
			
			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 47;              --参数
			};
			
			{
				Type = EAMT_ScreenTex;  --原子类型
				Param = 49;              --参数
			};]]
		};
		
		SubTask =           --子任务链表 
		{
	    
			{--subtask1
				Validation = 30001;        --非零时发送到服务器验证
				ProcessType = EPT_Circle;
				SubTaskPeriodAtom =		
				{
					{
						Type = EAMT_Esc;	--检测esc按键，请求完成任务
						Param = 30001;
					};
				};
				AtomLevelList =        --原子层链表 
				{
					{
						DetectorId = 13;--检测器id，检测速度是否达到100
						AtomList =     --原子链表
						{
							--文字提示向前
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 6407;              --参数
							};
							--键位提示向前键
							{
								Type = EAMT_KeyBlink;   --原子类型
								Param = 23;              --参数
								Option = 25;
							};
							
							{
								Type = EAMT_SwitchTex;   --原子类型
								Param = 43;              --参数
								Option = 44;
							};
						};
					};
					
					{
						DetectorId = 14;--检测器id，检测是否漂移
						AtomList =     --原子链表
						{
							--文字提示按键漂移
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 6663              --参数
							};
							--键位提示向前键
							{
								Type = EAMT_KeyBlink;   --原子类型
								Param = 23;              --参数
								Option = 25;
							};
							--键位提示按左键
							{
								Type = EAMT_KeyBlink;     --原子类型
								Param = 11;
								Option = 13;
							};
							--键位提示按shift键
							{
								Type = EAMT_KeyBlink;     --原子类型
								Param = 20;
								Option = 22;
							};
							
							{
								Type = EAMT_SwitchTex;   --原子类型
								Param = 45;              --参数
								Option = 46;
							};
							
						};
					};
					
					{
						DetectorId = 15;--检测器id，检测是否回搬车头
						AtomList =     --原子链表
						{
							--文字提示回搬车头
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 6919;              --参数
							};
							--键位提示向前键
							{
								Type = EAMT_KeyBlink;   --原子类型
								Param = 23;              --参数
								Option = 25;
							};
							--键位提示按右键
							{
								Type = EAMT_KeyBlink;   --原子类型
								Param = 17;              --参数
								Option = 19;
							};
							
							{
								Type = EAMT_SwitchTex;   --原子类型
								Param = 47;              --参数
								Option = 48;
							};
						};
					};
					
					{
						DetectorId = 16;--检测器id，检测是否有小喷
						AtomList =     --原子链表
						{
							--文字提示小喷
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 7175;              --参数
							};
							--键位提示向前键
							{
								Type = EAMT_KeyBlink;   --原子类型
								Param = 23;              --参数
								Option = 25;
							};
							
							{
								Type = EAMT_SwitchTex;   --原子类型
								Param = 49;              --参数
								Option = 50;
							};
						};
					};
					
					{
						DetectorId = 8;--检测器id
					};
				};				
			};
		};
		
		CanStart = function()
		end;

		OnStart = function()
		end;
		
		OnEnd = function()
		end;
	};

	--任务3
	{
		Id = 3;    		   --任务id 
  		MapId = 110;        --地图id 
		Desc = "单圈练习";  --任务介绍 
		--开始任务需要的key 
		PreKey=
		{
			1;
		};

		SubTask =           --子任务链表 
		{
	    
			{--subtask1
				Validation = 0;        --非零时发送到服务器验证
				AtomLevelList =        --原子层链表 
				{
					{
						DetectorId = 1;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_LockKey;   --原子类型
							};
						};
					};
					
					{
						DetectorId = 12;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_ScreenEff;  --原子类型
								Param = 2051;              --参数
							};
							
							{
								Type = EAMT_LockKey;   --原子类型
							};
						};
					};
					
					{
						DetectorId = 9;--检测器id
						AtomList =     --原子链表
						{
							{
								Type = EAMT_StartNo2;   --原子类型，激发小喷
							};
						};
					};
					
					{
						DetectorId = 11;--检测器id,倒计时检测
					};
				};				
			};
		};
		
		CanStart = function()
		end;

		OnStart = function()
		end;
		
		OnEnd = function()
		end;
	};	
 };

	    