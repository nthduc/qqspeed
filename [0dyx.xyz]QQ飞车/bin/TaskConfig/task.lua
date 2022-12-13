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

EAMT_DialoggBox = 0;	--�Ի���
EAMT_ScreenTex = 1;	--��Ļ��ͼ
EAMT_BlinkTex = 2;		--��ҫ����ͼ
EAMT_Effect = 3;		--��Ч
EAMT_Anim = 4;		--����
EAMT_Sound = 5;		--����
EAMT_ScreenEff = 6;		--��Ļ��Ч����CGameEffectMgt������
EAMT_LockKey = 7;		--������
EAMT_CDTimerShow = 8;	--��ʾ����ʱʱ��
EAMT_KeyBlink = 9;		--������˸
EAMT_StartNo2 = 10;	--����С��
EAMT_ScreenFont =11;	--��Ļ����
EAMT_Esc = 12;		--Esc����������������������
EAMT_SwitchTex = 13;	--�����ڿ��ص�ԭ�ӣ�startʱ��ʾĳһtex��stopʱ��ʾ�����tex

--����ִ�е�����
EPT_NeverBack = 0;		--������ͷ��
EPT_Circle = 1;			--ʧ�ܿɻ�ͷ������	

--����״̬
SST_On = 0;	--��
SST_Off = 1;	--��

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
		Id = 1;    			--����id 
  		MapId = 109;        --��ͼid 
		Desc = "������ϰ";  --������� 
		--��ʼ������Ҫ��key 
		PreKey=
		{
			1;
		};
		--���ر�
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
		
		TaskPeriodAtom =		--�������������ڵı���ԭ��
		{
			{
				Type = EAMT_ScreenTex;  --��Ļ����
				Param = 32;              --����
			};

			{
				Type = EAMT_ScreenTex;  --��Ļ����
				Param = 35;              --����
			};

			{
				Type = EAMT_ScreenTex;  --��Ļ����
				Param = 38;              --����
			};

			
			{
				Type = EAMT_ScreenTex;  --��Ļ����
				Param = 41;              --����
			};
		};
		SubTask =           --���������� 
		{
	    
			{--subtask1
				Validation = 110;        --����ʱ���͵���������֤
				AtomLevelList =        --ԭ�Ӳ����� 
				{
					{
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};
						};
					};
			
					{
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};
							
							{
								Type = EAMT_Sound;   --��������
								Param = 125;              --����
							};
							
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 5123;              --����
							};
						};
					};
					
					{
						DetectorId = 2;--�����id
						AtomList =     --ԭ������
						{
							{--��˸�ϼ�
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 31;              --����
								Option = 33;
							};
						};
					};
					
					{
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};
					    	
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 6147;              --����
							};
						};
					};
					
				};
			};
			
			{--subtask2����down��
				Validation = 111;
				AtomLevelList =
				{
					{
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};
							
							{
								Type = EAMT_Sound;   --��������
								Param = 126;              --����
							};
							
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 5379;              --����
							};
						};
					};
					
					{
						DetectorId = 3;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_KeyBlink;    --ԭ������
								Param = 34;               --����
								Option = 36; 
							};
						};
					};
					
					{
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};

							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 6147;              --����
							};
						};
					};
				};
			};
			
			{--subtask3�������ϼ�
				Validation = 112;
				AtomLevelList =
				{
					{
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};
							
							{
								Type = EAMT_Sound;   --��������
								Param = 127;              --����
							};
							
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 5635;              --����
							};
						};
					};

					{
						DetectorId = 4;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 31;              --����
								Option = 33;
							};                          
					    	
							{
								Type = EAMT_KeyBlink;     --ԭ������
								Param = 37;
								Option = 39;
							};
					    };
					};
					
					{
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};

							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 6147;              --����
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
						DetectorId = 8;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};
							
							{
								Type = EAMT_Sound;   --��������
								Param = 128;              --����
							};

							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 5891;              --����
							};
						};
					};
					
					{
						DetectorId = 5;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 31;              --����
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
						DetectorId = 10;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 6147;              --����
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
--����2	
	{
		Id = 2;    		   --����id 
  		MapId = 109;        --��ͼid 
		Desc = "Ư����ϰ";   --������� 
		--��ʼ������Ҫ��key 
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
		{	--��������ͼ
			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 9;              --����
			};

			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 12;              --����
			};

			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 15;              --����
			};

			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 18;              --����
			};

			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 21;              --����
			};

			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 24;              --����
			};

			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 27;              --����
			};
			
			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 30;              --����
			};
			
			--[[{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 43;              --����
			};
			
			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 45;              --����
			};
			
			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 47;              --����
			};
			
			{
				Type = EAMT_ScreenTex;  --ԭ������
				Param = 49;              --����
			};]]
		};
		
		SubTask =           --���������� 
		{
	    
			{--subtask1
				Validation = 30001;        --����ʱ���͵���������֤
				ProcessType = EPT_Circle;
				SubTaskPeriodAtom =		
				{
					{
						Type = EAMT_Esc;	--���esc�����������������
						Param = 30001;
					};
				};
				AtomLevelList =        --ԭ�Ӳ����� 
				{
					{
						DetectorId = 13;--�����id������ٶ��Ƿ�ﵽ100
						AtomList =     --ԭ������
						{
							--������ʾ��ǰ
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 6407;              --����
							};
							--��λ��ʾ��ǰ��
							{
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 23;              --����
								Option = 25;
							};
							
							{
								Type = EAMT_SwitchTex;   --ԭ������
								Param = 43;              --����
								Option = 44;
							};
						};
					};
					
					{
						DetectorId = 14;--�����id������Ƿ�Ư��
						AtomList =     --ԭ������
						{
							--������ʾ����Ư��
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 6663              --����
							};
							--��λ��ʾ��ǰ��
							{
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 23;              --����
								Option = 25;
							};
							--��λ��ʾ�����
							{
								Type = EAMT_KeyBlink;     --ԭ������
								Param = 11;
								Option = 13;
							};
							--��λ��ʾ��shift��
							{
								Type = EAMT_KeyBlink;     --ԭ������
								Param = 20;
								Option = 22;
							};
							
							{
								Type = EAMT_SwitchTex;   --ԭ������
								Param = 45;              --����
								Option = 46;
							};
							
						};
					};
					
					{
						DetectorId = 15;--�����id������Ƿ�ذᳵͷ
						AtomList =     --ԭ������
						{
							--������ʾ�ذᳵͷ
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 6919;              --����
							};
							--��λ��ʾ��ǰ��
							{
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 23;              --����
								Option = 25;
							};
							--��λ��ʾ���Ҽ�
							{
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 17;              --����
								Option = 19;
							};
							
							{
								Type = EAMT_SwitchTex;   --ԭ������
								Param = 47;              --����
								Option = 48;
							};
						};
					};
					
					{
						DetectorId = 16;--�����id������Ƿ���С��
						AtomList =     --ԭ������
						{
							--������ʾС��
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 7175;              --����
							};
							--��λ��ʾ��ǰ��
							{
								Type = EAMT_KeyBlink;   --ԭ������
								Param = 23;              --����
								Option = 25;
							};
							
							{
								Type = EAMT_SwitchTex;   --ԭ������
								Param = 49;              --����
								Option = 50;
							};
						};
					};
					
					{
						DetectorId = 8;--�����id
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

	--����3
	{
		Id = 3;    		   --����id 
  		MapId = 110;        --��ͼid 
		Desc = "��Ȧ��ϰ";  --������� 
		--��ʼ������Ҫ��key 
		PreKey=
		{
			1;
		};

		SubTask =           --���������� 
		{
	    
			{--subtask1
				Validation = 0;        --����ʱ���͵���������֤
				AtomLevelList =        --ԭ�Ӳ����� 
				{
					{
						DetectorId = 1;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_LockKey;   --ԭ������
							};
						};
					};
					
					{
						DetectorId = 12;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_ScreenEff;  --ԭ������
								Param = 2051;              --����
							};
							
							{
								Type = EAMT_LockKey;   --ԭ������
							};
						};
					};
					
					{
						DetectorId = 9;--�����id
						AtomList =     --ԭ������
						{
							{
								Type = EAMT_StartNo2;   --ԭ�����ͣ�����С��
							};
						};
					};
					
					{
						DetectorId = 11;--�����id,����ʱ���
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

	    