--enmUseType(����ʹ������)
EUT_Active		= 0;	--����ʹ����
EUT_Passive	= 1;	--����ʹ����
--enmAffectType(������������)
EAT_Attack	= 0;	--�����͵���
EAT_Protect	= 1;	--�����͵���
EAT_Generator	= 2;	--���������͵���
EAT_Cure		= 3; 	--�����͵���
--enmTargetType(��������Ŀ������)
ETT_Play		= 0;	--������
ETT_Enemy		= 1;	--���е���
ETT_FirstPlay	= 2;  --��һ��
ETT_PrevPlay	= 3;  --ǰһ��
ETT_PrevEnemy	= 4;  --ǰһ������
ETT_Me			= 5;	--�Լ�
--enmCannotUseCondition
ECUC_None		= 0;	--ʹ�ò����κ�����
ECUC_OneExists	= 1;	--�Ѿ���һ�������������õ�ʱ��
ECUC_NoAccel	= 2;	--���ڼ��ٵ�ʱ��
--enmSelectTargetType
ESTT_NoSelect	= 0;	--����Ҫ��׼��ֱ��ʹ��
ESTT_Select	= 1;	--��Ҫ��׼����ʹ��
--enmProtectType(�����͵��ߵķ�����ʽ)
EPT_AllAttack	= 0;	--�����й�������
EPT_All		= 1;	--�����й����ͷ���������
EPT_Specified	= 2;	--�����ض�����
--enmProtectTargetType(�����͵��ߵ����÷�Χ)
EPT_Me		= 100;	--�����Լ�
EPT_Team		= 101;	--������������
--enmMoveType(�˶�����)
EMT_None		= 0;	--���˶����̣�����������ߵ�Ĭ����Ϊ��
EMT_Missile	= 1;	--����
EMT_Flying		= 2; 	--ˮ��Ӭ
--enmStaticProp(�����͵������е�����)
ESP_None		= 0;
ESP_Shelter	= 1;	--�ڵ�������ʹ�ã�
--enmBlendMode(����߻���ģʽ)
EBM_AddNext	= 0;	--������������δ����������к�һ�����������ã���ֹͣ��ǰ���ã�����һ��������Ч�����õ���
EBM_UseNext	= 1;	--������������δ����������к�һ�����������ã�����ֹͣ��ǰ���ã�����һ��������Ч
EBM_IgnoreNext	= 2;	--������������δ������������һ����������
EBM_ForceReplaceOld	= 3;	--ǰһ������������δ����ʱ��������������ǿ�ƽ�����һ�����ߵ����ã���ʹǰһ��������EBM_IgnoreNext�ġ�
--enmAffectID
EAI_None		= 0;	--�ޱ���
EAI_Lift		= 1;	--��̧�𣬲�������̧��ĸ߶�
EAI_Magnet	= 2;	--����
EAI_Stop		= 3;	--ֹͣ
EAI_InvertLR	= 4;	--���ҷ�������ã�������TRUE�����ã���FALSE��ȡ����
EAI_FreezeGP	= 5;	--������ʹ�õ���
EAI_FreezeOp	= 6;	--�������κβ���
EAI_No2		= 7; 	--����
EAI_Animation	= 8;	--���Ŷ���������������ID
EAI_Effect		= 9;	--������effectid
EAI_Sound		= 10;
EAI_Speed		= 11; --�����ٶȣ�ʩ������
EAI_Jet		= 12; --���
EAI_Evil	= 13; --ħ�����߷�����Ч
EAI_Generator	= 14;--�����µ���
EAI_No2Emitter	= 15;--С�紥����

--enmSpecialEffect
ESE_None		= 0;	
ESE_Effect		= 1;	--��Ч
ESE_Sound		= 2;	--����
ESE_Animation	= 3;	--����
--enumAffectOption
EAO_None			= 0;
EAO_AffectMeInstead	= 1;
EAO_AffectPlayInstead	= 2;
EAO_FaceToPeer		= 4;
EAO_AttachToWorld	= 8;
--���ߵ��������ʹ�û���
ECAT_Accel	= 1;
ECAT_Protect	= 2;

GPConfig=
{
	{
		id=10;
		name="������";
		category=ECAT_Protect;
		usetype=EUT_Active;
		affectype=EAT_Protect;
		cannotusecondition=ECUC_OneExists;
		
		protecttype=EPT_AllAttack;
		protectcount=1;
		protecttarget=EPT_Me;
		
		icontex="textures/shield1.tga";
		throweffect=
		{
			{
				type=ESE_Sound;
				param=105;
			};
		};
		protecteffect=
		{
			{	--�����ֱ�����Ч
				type=ESE_Effect;
				param=6; 
			};
			{	--��������Ч
				type=ESE_Sound;
				param=111;
			};
		};
		affects=
		{
			{
				id=EAI_Effect;
				param=1;
				lifetime=2500;
			};
		};
	};
	{
		id=2;
		name="���ը��"; --ˮ��Ӭ
		usetype=EUT_Active;
		affectype=EAT_Attack;
		blendmode=EBM_IgnoreNext;
	
		targettype=ETT_PrevEnemy; 

		icontex="textures/kubi1.tga";
		
		movectrl=
		{
			type=EMT_Flying;
			starttime=0;
			lifetime=2500;
			movenode=12;
		};
		
		throweffect=
		{
			{	--���Ŷ���
				type=ESE_Animation;
				param=1310;
			};
			{	--���ŵ���Ч
				type=ESE_Sound;
				param=105;
			};
			{	--���е���Ч
				type=ESE_Sound;
				param=113;
			};
		};
		
		--����������������������ը��ʱ����
		triggertime=2500;
		triggereffect=
		{
			{	--���е���Ч
				type=ESE_Sound;
				param=113;
			};
		};
		
		affects=
		{
			{	--ˮ��Ӭ������Ч
				id=EAI_Sound;
				param=114;
				lifetime=1000;
			};
			{	--���ϵ�ˮ��
				id=EAI_Effect;
				param=3;
				starttime=0;
				lifetime=2000;
				option=EAO_AttachToWorld;
			};
			{	--���ڿ����
				id=EAI_Effect;
				param=41;
				starttime=0;
				lifetime=2000;
				--option=EAO_FaceToPeer;
			};
			{	--̧��
				id=EAI_Lift;
				param=1.25;
				lifetime=2000;
				option=100;	--�����ʱ����̧����param�涨�ĸ߶�
			};
			{	--ҡ��
				id=EAI_Animation;
				param=2306;
				starttime=500;
				lifetime=2500;
			};
			--[[
			{	--�����ݰ�Χ
				id=EAI_Effect;
				param=4;
				lifetime=2000;
			};]]
		};
	};	
	{
		id=9;
		name="����";
		usetype=EUT_Active;
		affectype=EAT_Generator;
		
		icontex="textures/fog1.tga";
		newid=1001;	--�����ɵ�id��1000��ʼ
		
		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1310;
			};
			{	--�ͷŵ���Ч
				type=ESE_Sound;
				param=104;
			};
			{	--�ͷŵ���Ч
				type=ESE_Effect;
				param=20;
			};
		};
	};
	{
		id=1001;
		name="����";
		usetype=EUT_Passive;
		affectype=EAT_Attack;
		
		staticprop=ESP_Shelter;
		staynode=5;
		staytime=6600;
		
		position=
		{
			x=-0.5; --����������ǰ���ľ���
		};
		bound=
		{
			x=0.1; --�����Χ��İ뾶
		};			
	};
	
	{
		id=3;
		name="����";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		blendmode=EBM_IgnoreNext;
		readytime=1000;
		
		movectrl=
		{
			type=EMT_Missile;
			starttime=1000;
			lifetime=1500; --���е�ʱ��
			movenode=14;
			srcpos=
			{
				x=-0.4;
				y=0.4;
				z=0.4;
			};
		};
		
		targettype=ETT_Play;
		selecttargettype=ESTT_Select;
		
		icontex="textures/missile1.tga";

		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1310;
			};
			{	--�ͷ���Ч
				type=ESE_Sound;
				param=110;
			};
			{	--������Ч
				type=ESE_Sound;
				param=115;
			};
			{	--��������
				type=ESE_Effect;
				param=42;
			};
		};
		
		triggertime=1500; --�μ�ˮ��Ӭ
		triggereffect=
		{
			{	--�������ٶ�������Ч
				type=ESE_Sound;
				param=115;
			};
		};
		affects=
		{
			{	--���б�ը
				id=EAI_Effect;
				param=15;
				lifetime=2000;
				option=EAO_AttachToWorld;
			};
			{	--������ը����Ч
				id=EAI_Sound;
				param=116;
				lifetime=2000;
			};
			{	--��ʾ�����ж���
				id=EAI_Animation;
				param=1314;
				lifetime=2000;
				option=EAO_AffectPlayInstead;
			};
			{	--�ڿ�
				id=EAI_Lift;
				param=1.75;
				lifetime=700;
				option=350;
			};
			{
				id=EAI_Lift;
				param=-0.75;
				starttime=800;
				lifetime=700;
				option=700;
			};
			{	--ת����Ȧ�Ķ���
				id=EAI_Animation;
				param=2303;	--ת����Ȧ�Ķ������ȴ���Դ
				lifetime=1000;
			};
			{	--���Ӿ�ͷ
				id=EAI_Effect;
				param=29;
				lifetime=2500;
			};
			{	--����С��
				id=EAI_No2Emitter;
				param=500;
				starttime=2000;
				lifetime=500;
			};
		};
	};
	{
		id=1;
		name="����";
		category=ECAT_Accel;
		usetype=EUT_Active;
		affectype=EAT_Protect;
		cannotusecondition=ECUC_OneExists+ECUC_NoAccel;
		stopcondition=ECUC_NoAccel;
		
		protecttype=EPT_Specified;
		protectcount=0;
		protecttarget=EPT_Me;
		protectindex=-1;
		
		icontex="textures/n2o1.tga";
		affects=
		{
			{	--������Ч
				id=EAI_Sound;
				param=124;
				lifetime=3500;
			};
		
			{	--No2
				id=EAI_No2;
				param=1;
				lifetime=10000;
			};
		};
	};
	{
		id=7;
		name="������";
		usetype=EUT_Active;
		affectype=EAT_Generator;
		
		birthtime=0;
		newid=1002;
		
		icontex="textures/waterbomb1.tga";
		
		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1310;
			};
			{	--�ͷ���Ч
				type=ESE_Effect;
				param=22;
			};
			{	--�ͷ���Ч
				type=ESE_Sound;
				param=105;
			};
		};
	};
	{
		id=1002;
		name="������";
		usetype=EUT_Passive;
		afftectype=EAT_Attack;
		blendmode=EBM_IgnoreNext;
		
		staynode=8;
		staytime=4000;
		camera=28;
		
		position=
		{
			x=60;
		};
		bound=
		{
			x=1.8;
		};
	
		affects=
		{
			{	--��ˮ����ס����Ч
				id=EAI_Sound;
				param=112;
				lifetime=2000;
			};
			{	--�����ס
				id=EAI_Effect;
				param=40;
				lifetime=2000;
			};
			{ 	--��̧��
				id=EAI_Lift;
				param=1;
				lifetime=2000;
				option=1000;
			};
			{	--ҡ��
				id=EAI_Animation;
				param=2306;
				starttime=1000;
				lifetime=1000;
			};
			{	--���Ӿ�ͷ
				id=EAI_Effect;
				param=28;
				lifetime=2000;
			};
		};
	};
	{
		id=4;
		name="�ڹ�";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		targettype=ETT_FirstPlay;
		
		icontex="textures/tortoise1.tga";
		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1310;
			};
			{	--�ͷ���Ч
				type=ESE_Effect;
				param=18;
			};
			{	--�ͷŷɵ�����Ч
				type=ESE_Sound;
				param=101;
			};
		};
		affects=
		{
			{	--���ɵ����е���Ч
				id=EAI_Sound;
				param=102;
				lifetime=3000;
			};
			{	--����
				id=EAI_Speed;
				param=0.1;
				lifetime=3000;
			};
			{	--3ֻС�ڹ�Ķ���
				id=EAI_Effect;
				param=9;
				starttime=0;
				lifetime=3000;
			};
			{	--���Ӿ�ͷ
				id=EAI_Effect;
				param=25;
				lifetime=3000;
			};
			{	--�ɵ���������Ч
				id=EAI_Sound;
				param=103;
				starttime=3000;
				lifetime=1000;
			};
		};
	};
	{
		id=6;
		name="ħ��";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		targettype=ETT_Enemy;
		
		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1310;
			};
			{	--�ͷ���Ч
				type=ESE_Sound;
				param=100;
			};
			{	--�ͷ���Ч
				type=ESE_Effect;
				param=21;
			};
		};
		
		icontex="textures/devil1.tga";
		affects=
		{
			{	--��ħ����ʱ��Ц��
				id=EAI_Sound;
				param=118;
				lifetime=3000;
			};
			{	--���Ҽ������෴
				id=EAI_InvertLR;
				lifetime=6000;
			};
			{	--ħ������
				id=EAI_Effect;
				param=10;
				lifetime=6000;
			};
			{	--���Ϸ���
				id=EAI_Evil;
				lifetime=6000;
			};
		};
	};
	{
		id=5;
		name="����";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		
		--affectself=1;
		targettype=ETT_Play;
		selecttargettype=ESTT_Select;
		
		icontex="textures/magnet1.tga";
		
		throweffect=
		{
			{	--�ͷ���Ч
				type=ESE_Sound;
				param=109;
			};
		};
		affects=
		{
			{	--�������õ���Ч
				id=EAI_Sound;
				param=119;
				lifetime=3000;
			};
			{	--����Ч��
				id=EAI_Magnet;
				lifetime=3000;
			};
			{	--������Ч
				id=EAI_Effect;
				param=13;
				lifetime=3000;
				option=EAO_AffectMeInstead+EAO_FaceToPeer;
			};
			--[[{	--�����Է�����Ч��
				id=EAI_Effect;
				param=39;
				lifetime=3000;
			};]]
			{	--�������Ӿ�ͷ
				id=EAI_Effect;
				param=27;
				lifetime=3000;
				option=1;
			};
		};
	};		
	{
		id=8;
		name="�㽶Ƥ";
		usetype=EUT_Active;
		affectype=EAT_Generator;
		
		newid=1003;
		icontex="textures/banana1.tga";
		
		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1313;
			};
			{	--�ͷ���Ч
				type=ESE_Sound;
				param=105;
			};
		};
	};
	{
		id=1003;
		name="�㽶Ƥ";
		usetype=EUT_Passive;
		affecttype=EAT_Attack;
		birthtime=1000;		
		
		staynode=11;
		staytime=30000;
		passcount=1;
		--camera=26;
		
		position=
		{
			x=0;
		};
		bound=
		{
			x=0.5;
		};
		
		affects=
		{
			{	--�㽶Ƥ����ʱ��Ч
				id=EAI_Sound;
				param=120;
				lifetime=1000;
			};
			{	--����
				id=EAI_Speed;
				param=0.1;
				lifetime=200;
			};
			{	--��ʾ�����ж���
				id=EAI_Animation;
				param=1314;
				lifetime=2000;
				option=EAO_AffectPlayInstead;
			};
			{	--��
				id=EAI_Animation;
				param=2304; --���Ӵ򻬵Ķ������ȴ���Դ��
				lifetime=2500;
			};
			{	--��������
				id=EAI_FreezeOp;
				lifetime=2500;
			};
			{	--���Ӿ�ͷ
				id=EAI_Effect;
				param=26;
				lifetime=2500;
			};
		};
	};
	
	{
		id=2001;
		name="С��";
		category=ECAT_Accel;
		usetype=EUT_Active;
		affectype=EAT_Protect;
		cannotusecondition=ECUC_OneExists+ECUC_NoAccel;
		stopcondition=ECUC_NoAccel;
		
		protecttype=EPT_Specified;
		protectcount=0;
		protecttarget=EPT_Me;
		protectindex=-1;
		
		icontex="textures/n2o1.tga";
		affects=
		{
			{	--������Ч
				id=EAI_Sound;
				param=124;
				lifetime=500;
			};
		
   			{	--No2
				id=EAI_No2;
				param=2001;
				lifetime=10000;
			};
		};
	};
	{
		id=2002;
		name="��ӵ���";
		category=ECAT_Accel;
		usetype=EUT_Active;
		affectype=EAT_Protect;
		cannotusecondition=ECUC_OneExists+ECUC_NoAccel;
		stopcondition=ECUC_NoAccel;
		
		protecttype=EPT_Specified;
		protectcount=0;
		protecttarget=EPT_Me;
		protectindex=-1;
		
		icontex="textures/n2o1.tga";
		affects=
		{
			{	--������Ч
				id=EAI_Sound;
				param=124;
				lifetime=4000;
			};
   			{	--No2
				id=EAI_No2;
				param=2002;
				lifetime=10000;
			};
		};
	};
	{
		id=8888;
		name="Ģ��";
		category=ECAT_Protect;
		usetype=EUT_Active;
		affectype=EAT_Protect;
		cannotusecondition=ECUC_OneExists;
		
		protecttype=EPT_AllAttack;
		protectcount=1;
		protecttarget=EPT_Me;
		
		icontex="textures/shield1.tga";
		
		affects=
		{
			{
				id=EAI_Effect;
				param=38;
				lifetime=2500;
			};
		};
	};

	--��ӵ���
	{
		id=11;
		name="�Ա�";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		targettype=ETT_Me;
		
		icontex="textures/waterbomb1.tga";
		
		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1310;
			};
			{	--�ͷ���Ч
				type=ESE_Sound;
				param=105;
			};
		};
		affects=
		{
			{
				id=EAI_Generator;
				param=1004;
				starttime=3000;
				lifetime=1000;
			};
		};
	};
	{
		id=1004;
		name="�Ա�";
		usetype=EUT_Passive;
		afftectype=EAT_Attack;
		blendmode=EBM_IgnoreNext;
		
		staynode=8;
		staytime=1000;
		camera=28;
		
		position=
		{
			x=0;
		};
		bound=
		{
			x=1.8;
		};
	
		affects=
		{
			{	--��ˮ����ס����Ч
				id=EAI_Sound;
				param=112;
				lifetime=2000;
			};
			{	--�����ݰ�Χס
				id=EAI_Effect;
				param=4;
				lifetime=2000;
			};
			{ 	--��̧��
				id=EAI_Lift;
				param=1;
				lifetime=2000;
				option=1000;
			};
			{	--ҡ��
				id=EAI_Animation;
				param=2306;
				starttime=1000;
				lifetime=1000;
			};
			{	--���Ӿ�ͷ
				id=EAI_Effect;
				param=28;
				lifetime=2000;
			};
		};
	};
	{
		id=12;
		name="�Ŷ��޵�";
		category=ECAT_Protect;
		usetype=EUT_Active;
		affectype=EAT_Protect;
		
		protecttype=EPT_AllAttack;
		protectcount=1;
		protecttarget=EPT_Team;
		
		icontex="textures/shield1.tga";
		throweffect=
		{
			{
				type=ESE_Sound;
				param=105;
			};
		};
		protecteffect=
		{
			{	--�����ֱ�����Ч
				type=ESE_Effect;
				param=6; 
			};
			{	--��������Ч
				type=ESE_Sound;
				param=111;
			};
		};
		affects=
		{
			{
				id=EAI_Effect;
				param=1;
				lifetime=4000;
			};
		};
	};
	{
		id=13;
		name="��Ų�";
		usetype=EUT_Active;
		affectype=EAT_Cure;
		targettype=ETT_FirstPlay;
		
		cureid=4;
		icontex="textures/shield1.tga";
		throweffect=
		{
			{
				type=ESE_Sound;
				param=105;
			};
		};
		cureeffect=
		{
			{	--�����ֱ�����Ч
				type=ESE_Effect;
				param=6; 
			};
			{	--��������Ч
				type=ESE_Sound;
				param=111;
			};
		};
		affects=
		{	
			{
				id=EAI_None;
				lifetime=1000;
			};
		};
	};
	{
		id=2004;
		name="����-�㽶";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		blendmode=EBM_IgnoreNext;
		
		movectrl=
		{
			type=EMT_Missile;
			starttime=1000;
			lifetime=1500; --���е�ʱ��
			movenode=14;
		};
		
		targettype=ETT_Play;
		selecttargettype=ESTT_Select;
		
		icontex="textures/missile1.tga";
		
		throweffect=
		{
			{	--�ͷŶ���
				type=ESE_Animation;
				param=1310;
			};
			{	--�ͷ���Ч
				type=ESE_Sound;
				param=110;
			};
			{	--������Ч
				type=ESE_Sound;
				param=115;
			};
			{	--��������
				type=ESE_Effect;
				param=42;
			};
		};

		triggertime=1500; --�μ�ˮ��Ӭ
		triggereffect=
		{
			{	--�������ٶ�������Ч
				type=ESE_Sound;
				param=115;
			};
		};
		affects=
		{
			{	--�㽶Ƥ����ʱ��Ч
				id=EAI_Sound;
				param=120;
				lifetime=2000;
			};
			{	--����
				id=EAI_Speed;
				param=0.1;
				lifetime=200;
			};
			{	--��ʾ�����ж���
				id=EAI_Animation;
				param=1314;
				lifetime=2000;
				option=EAO_AffectPlayInstead;
			};
			{	--��
				id=EAI_Animation;
				param=2304; --���Ӵ򻬵Ķ������ȴ���Դ��
				lifetime=2500;
			};
			{	--��������
				id=EAI_FreezeOp;
				lifetime=2500;
			};
			{	--���Ӿ�ͷ
				id=EAI_Effect;
				param=26;
				lifetime=2500;
			};
		};
	};
};