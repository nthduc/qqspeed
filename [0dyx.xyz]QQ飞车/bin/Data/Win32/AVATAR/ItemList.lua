--����item���õĴ���ʱ��
EITO_GETPROP=1; --�����ʱ
EITO_USEPROP=2; --ʹ�õ���ʱ
EITO_ATTACKED=4; --�ܵ�����ʱ
EITO_GAMEEND=8;  --��Ϸ����ʱ

--�����㷨����
EIAAT_ADDMONEY=1000; --���Ͻ��, ������3��: ����, �������, �Ƿ���Ҫ�������Կ�(0��1)
EIAAT_ADDEXPERIENCE=1001; --���Ͼ���, ������3��: ����, ��������, �Ƿ���Ҫ�������Կ�(0��1)
EIAAT_MULTIPLYMONEY=1002; --���Խ�ұ���, ������3��: ����, ��ұ���, �Ƿ���Ҫ�������Կ�(0��1)
EIAAT_MULTIPLYEXPERIENCE=1003; --���Ծ��鱶��, ������3��: ����, ���鱶��, �Ƿ���Ҫ�������Կ�(0��1)
EIAAT_PROPTRANSFER=1004; --����ת��, ������1��: Ŀ�ĵ���ID
EIAAT_EFFECTTRANSFER=1005; --Ч��ת��, ������1��:Ŀ��Ч��ID(�˴�Ϊ�����Ч����propID)
EIAAT_ITEMCONSUME=1006; --����item����, ������1��: ����ֵ
EIAAT_ADDMONEYINRACE=1007; --��Ϸ�����ӽ��, ������2��: �������, �Ƿ���Ҫ�������Կ�(0��1)
EIAAT_MINUSTIME=1008;	--����ʱ�䣬������1����ʱ��ֵ(ms)

--�������ֽ�������
EBC_FIRSTORWIN=1; --��������һ��, �����ʤ��
EBC_TOPORWIN=2; --������ǰ����, �����ʤ��
EBC_FIRSTORWININ8=3; --��8�˵ı�����, ��������һ��, �����ʤ��
EBC_OTHERNOFINISHIN8=4; --��8�˵ı����������˾�δ���

--����ID
EPID_SPEEDUP=1; --���ٿ�, ����
EPID_FLY=2; --ˮ��Ӭ, ������ը��
EPID_MISSILE=3; --����
EPID_REDUCER=4; --�ɵ�, ������
EPID_MAGNET=5; --����
EPID_EVIL=6; --��ħ
EPID_WATERBOMB=7; --ˮը��, Ͷ����
EPID_SMEAR=8; --�㽶Ƥ, ����
EPID_CLOUD=9; --����, ����
EPID_DEFEND=10; --������, ������
EPID_ANGLE=11; --��ʹ
EPID_TIMEBOMB=12; --��ʱը��
EPID_RESCINDREDUCER=13; --�������

--avatar���߻������Ͷ���
EAIBT_ROLERELEVANT=256; --0x00000100,��ɫ�������
EAIBT_CARRELEVANT=512; --0x00000200,�����������
EAIBT_NORELEVANT=768;--0x00000300,���ɫ�����޹ص�����
EAIBT_NOENTITY=1024; --0x00000400,��ʵ������
EAIBT_ROLE=1280; --0x00000500,��ɫ
EAIBT_CAR=1536; --0x00000600,����
EAIBT_HAIR=1792;--0X00000700, ͷ��

--avatar�������Ͷ���(��������+������)
EAIT_RR_TOPHEAD=257; --0x00000101,��ɫͷ��
EAIT_RR_GLASS=258; --0x00000102,��ɫ�۾�
EAIT_RR_FACE=259; --0x00000103,��ɫ�沿��ͼ
EAIT_RR_DV=260; --0x00000104,��ɫ�������Ԥ����
EAIT_RR_HEADSHADE=261; --0x00000105,��ɫͷ����״��Ԥ����
EAIT_RR_BODYUP=262; --0x00000106,��ɫ�����壨�����ֱۣ�
EAIT_RR_PALM=263; --0x00000107,��ɫ����
EAIT_RR_TATTOO=264; --0x00000108,��ɫ�ֱ�����
EAIT_RR_BODYDOWN=265; --0x00000109,��ɫ������

EAIT_CR_TEXTURE=513; --0x00000201,�������ƻ�����
EAIT_CR_PAINT=514; --0x00000202,��������
EAIT_CR_BRAND=515; --0x00000203,��������
EAIT_CR_TAIL=516; --0x00000204,����β��
EAIT_CR_LAMPSEPCIAL=517; --0x00000205,����������Ч
EAIT_CR_BRANDSPECIAL=518; --0x00000206,����������Ч
EAIT_CR_BOTTOMLAMP=519; --0x00000207,�����׵�
EAIT_CR_EJECTSEPCIAL=520; --0x00000208,����������Ч
EAIT_NR_TYREMARK=769; --0x00000301,��̥ӡ
EAIT_NR_DRIFTSPECIAL=770; --0x00000302,Ư����Ч
EAIT_NE_ROOMSPECIAL=1025; --0x00000401,������Ч
EAIT_NE_SCENESPECIAL=1026; --0x00000402,����������Ч
EAIT_NE_WINSPECIAL=1027; --0x00000403,ʤ����Ч
EAIT_NE_PROPSPECIAL=1028; --0x00000404,��Ϸ������Ч
EAIT_NE_EXPRESSIONSPECIAL=1029; --0x00000405,������Ч
EAIT_NE_ROLEATTRIBUTECARD=1030; --0x00000406,�������Կ�
EAIT_NE_PROPEXCHANGE=1031; --0x00000407,���߻�λ��
EAIT_ROLE_BASE=1281; --0x00000501,--�����ɫϵ��
EAIT_CAR_BASE=1537; --0x00000601,--��������ϵ��

EAIT_ROLE_NEWTEST=1282; --0x00000502,--�������½�ɫϵ��
EAIT_CAR_NEWTEST=1538; --0x00000602,--������������ϵ��
EAIT_HAIR_NEWTEST=1794;--0X00000702,--������ͷ��

--װ��ʹ����Դ����
EIRES_SIG_NIF=1;		--��ģ��
EIRES_SIG_TEX=2;		--����ͼ
EIRES_MIX_SIG_NIFTEX=3;	--��ϵ�ģ����ͼ
EIRES_MUL_NIF=4;		--��ģ��
EIRES_MUL_TEX=8;		--����ͼ
EIRES_MIX_MUL_NIFTEX=12;--��϶�ģ�Ͷ���ͼ
EIRES_TEX_COLOR=16;		--����ɫ����
EIRES_SIG_COLOR_TEX=18;	--����ɫ���Ե���ͼ
EIRES_SIG_SKIN=32;	--����������
EIRES_MIX_SIG_SKINNIF=33;	--��������ģ��
EIRES_EXT_MODE=512;		--��չģʽ��ʹ�ö������ʹ���


--װ��������Ϊ����
EBI_Animation = 1;	--���Ŷ���
EBI_Effect = 2;		--������Ч
EBI_Sound = 3;		--��������


--ת���㷨����
EAI_GPTansfer = 1;      --��һ������ת�����߹���
EAI_GPReduceTime = 2;   --��һ�����ʼ��ٹ�������ʱ��

--װ��ת���㷨������
EOI_Add = 0;	--��
EOI_Mul = 1;	--��

--boolֵ
BFG_Forbid = 0;	--��ֹ
BFG_Can = 1;	--����

--װ�����࣬���ڳ�����̵���ʾ���࣬uires.lua�и���
ECBT_BASE_KART 		=1;	--����
ECBT_BASE_CHARACTOR	=2;	--��ɫ
ECBT_BASE_KARTEQP	=4;	--����װ��
ECBT_BASE_CHARACTOREQP	=8;	--��ɫװ��
ECBT_BASE_PET		=16;	--����
ECBT_BASE_EFFECT	=32;	--��Ч
ECBT_BASE_OTHER		=64;	--����
ECBT_BASE_HOTRECOMMAND	=128;	--�����Ƽ�����Ʒר�ã�

--�Ա����
EASE_MALE=1;	--����
EASE_FEMALE=2;	--Ů��


--�ҽӵ��б�,name��Ӧģ���еĹҽӵ�����,ʶ���Сд
HitchList=
{
	{
		typeid=EAIBT_ROLE;--��ɫͳһװ��λ
		name="role";
		defaultitemid=10000;	--Ĭ�ϵ�װ��
	};
	{
		typeid=EAIBT_CAR;--����ͳһװ��λ
		name="kart";
		defaultitemid=10003;	--Ĭ�ϵ�װ��
	};
	{
		typeid=EAIBT_HAIR;--ͷ��ͳһװ��λ
		name="hair";
		defaultitemid=10021;	--Ĭ�ϵ�װ��
	};
	{
		typeid=EAIT_ROLE_BASE;--��ͨ��ɫ
		name="role_base";
		defaultitemid=10000;	--Ĭ�ϵ�װ��
	};
	{
		typeid=EAIT_CAR_BASE;--��ͨ��
		name="kart_base";
		defaultitemid=10003;
	};	
	{
		typeid=EAIT_RR_TOPHEAD;--ͷ��
		name="char_head";
	};
	{
		typeid=EAIT_RR_GLASS;--�۾�
		name="char_glass";
	};
	{
		typeid=EAIT_RR_FACE;--������ͼ
		name="char_face";
	};
	{
		typeid=EAIT_RR_BODYUP;--��ɫ�����壨�����ֱۣ�
		name="char_bodyup";
	};
	{
		typeid=EAIT_RR_PALM;--��ɫ����
		name="char_palm";
	};
	{
		typeid=EAIT_RR_TATTOO;--��ɫ�ֱ�����
		name="char_tattoo";
	};
	{
		typeid=EAIT_RR_BODYDOWN;--��ɫ������
		name="char_bodydown";
	};
	------------------------------------------
	{
		typeid=EAIT_CR_BRAND;--����
		name="CP";
	};	
	{
		typeid=EAIT_CR_TAIL;--β��
		name="car_tail";
	};	
	{
		typeid=EAIT_CR_PAINT;--����
		name="car_paint";
		defaultitemid=10014;
	};
	{
		typeid=EAIT_ROLE_NEWTEST;--�½�ɫ
		name="role_newtest";
		defaultitemid=10018;
	};
	{
		typeid=EAIT_CAR_NEWTEST;--�³�
		name="kart_newtest";
		defaultitemid=10017;
	};
	{
		typeid = EAIT_HAIR_NEWTEST;--ͷ���� ������ר��
		name="hair_newtest";
		defaultitemid=10021;
	};
};

ItemList=
{
	{
		itemid=10000;--��10000��ʼ
		name="seaboy";
		itemtype=EAIT_ROLE_BASE;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{--�ö���ӵ�еĹҽӵ��
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;
		};
		modelname="Avatar";	--ģ��ʵ����nif�ļ����ʵ�����ƣ�������ת�����ߴ���ȹ���
		garagetype=ECBT_BASE_PET;	--��������ķ���
		sextype=EASE_MALE;	--�Ա����
 		description="������";
	};
	--��һ������
	{	
		itemid=10001;
		name="xiaoqian";
		itemtype=EAIT_ROLE_BASE;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;	
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_PET;	
		sextype=EASE_FEMALE;		
		description="Сٻ";
	};	
	--��һ������
	{
		itemid=10002;
		name="kart1";
		itemtype=EAIT_CAR_BASE;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"Editable Poly@#0";--����
			"Editable Poly";--����
			"Editable Poly@#1";
			"Editable Poly@#5";
			"Editable Poly@#6";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		description="��ɫ��";
	};
	--��һ������
	{
		itemid=10003;
		name="kart2";
		itemtype=EAIT_CAR_BASE;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"carbody";--����
			"Editable Poly";--����
			"Editable Poly@#1";
			"Editable Poly@#2";
			"Editable Poly@#3";			
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	
		description="�峵";
	};		
	--��һ������
	{
		itemid=10004;--��10000��ʼ
		name="feng_glass";
		itemtype=EAIT_RR_GLASS;--type
		restype=EIRES_SIG_NIF;
		modelname="char_glass";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="�羵";
	};

	--��һ������
	{
		itemid=10005;--��10000��ʼ
		name="sun_glass";
		itemtype=EAIT_RR_GLASS;--type
		restype=EIRES_SIG_NIF;
		modelname="char_glass";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="ī��";
	};

    	--��һ������
	{
		itemid=10006;--��10000��ʼ
		name="cat_ear";
		itemtype=EAIT_RR_TOPHEAD;--type
		restype=EIRES_SIG_NIF;
		modelname="char_head";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="è��";
	};

	--��һ������
	{
		itemid=10007;--��10000��ʼ
		name="rabbit_ear";
		itemtype=EAIT_RR_TOPHEAD;--type
		restype=EIRES_SIG_NIF;
		modelname="char_head";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="�ö�";
	};

	--��һ������
	{
		itemid=10008;--��10000��ʼ
		name="white_balloon";
		itemtype=EAIT_CR_TAIL;--type
		restype=EIRES_SIG_NIF;
		property=
		{
		    switch = BFG_Forbid;
		    nofog = BFG_Forbid;
		    physicPropt=
		    {
				operator = EOI_Add;         --�������������
				speed = 0;            --�ٶ�����
				erupt = 0;            --��������
				accumno2 = 0;         --���۵�������
				bigno2 = 0;          --��������
				minino2 = 0;          --С������
			};

			algorithm=            	--�㷨
			{
				{
			    	type = EAI_GPTansfer;         --����
			    	occasion = EITO_ATTACKED;   	--����ʱ��
			    	operator = EOI_Add;  	--����
			    	condition = EPID_MISSILE;  	--��������
			    	expectation = 2004;	--����ֵ
			    	rate = 50;       	--�õ�����ֵ�ĸ���
			    };
			};
		};
		modelname="objpolySurface6";
		garagetype=ECBT_BASE_KARTEQP;
		description="�ڲ�����";
	};			
	--��һ������
	{
		itemid=10009;--��10000��ʼ
		name="red_balloon";
		itemtype=EAIT_CR_TAIL;--type
		restype=EIRES_SIG_NIF;
		modelname="objpolySurface11";
		garagetype=ECBT_BASE_KARTEQP;
		description="�������";
		
	};				
	--��һ������
	{
		itemid=10010;--��10000��ʼ
		name="qqkart";
		itemtype=EAIT_CR_BRAND;--type
		restype=EIRES_SIG_TEX;
		modelname="Box01";
		garagetype=ECBT_BASE_KARTEQP;
		description="QQKart����";
	};
	--��һ������
	{
		itemid=10011;--��10000��ʼ
		name="love";
		itemtype=EAIT_CR_BRAND;--type
		restype=EIRES_SIG_TEX;
		modelname="Box01";
		garagetype=ECBT_BASE_KARTEQP;
		description="Love����";
	};

	--��һ������
	{
		itemid=10012;
		name="orange";
		itemtype=EAIT_ROLE_BASE;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;			
		};
		modelname="Avatar";	
		garagetype=ECBT_BASE_PET;	--��������ķ���
		sextype=EASE_FEMALE;		
		description="С����";
	};
	--��һ������
	{
		itemid=10013;
		name="kart3";
		itemtype=EAIT_CAR_BASE;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	--��������ķ���	
		description="��ɫ��";
	};
	--��һ������
	{
		itemid=10014;
		name="red_paint";
		itemtype=EAIT_CR_PAINT;--type������
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={0,1.2,3.5};	--ʹ��ɫ�ȱ�ʾ��ɫ
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="��ɫ����";
	};
	--��һ������
	{
		itemid=10015;
		name="yellow_paint";
		itemtype=EAIT_CR_PAINT;--type������
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={40,1.5,3};	--ʹ��ɫ�ȱ�ʾ��ɫ
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="��ɫ����";
	};
	--��һ������
	{
		itemid=10016;
		name="blue_paint";
		itemtype=EAIT_CR_PAINT;--type������
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={200,2,3};
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="��ɫ����";
	};
	--��һ������
	{
		itemid=10017;
		name="NewKart1";
		itemtype=EAIT_CAR_NEWTEST;--type����
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"carbody:0";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	--��������ķ���	
		description="��ɫ�³�";
	};
	--��һ������
	{
		itemid=10018;
		name="NewGirl";
		itemtype=EAIT_ROLE_NEWTEST;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;			
		};
		hidemodellist=
		{--�ö�����Ҫ����Ϸ���������ش����ģ������
			"foot";--��
			"foot_edge";--�ź�ģ		
		};
		modelname="Avatar";	
		garagetype=0;--ECBT_BASE_PET;
		sextype=EASE_FEMALE;		
		description="��Ů����ɫ";
	};
	--��һ������
	{
		itemid=10019;
		name="NewKart2";
		itemtype=EAIT_CAR_NEWTEST;--type����
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	
		description="��ɫ�³�";
	};
	--��һ������
	{
		itemid=10020;
		name="NewKart3";
		itemtype=EAIT_CAR_NEWTEST;--type����
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"Editable Poly";
			--"carbody:1";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		hiderolefoot=0;	--�Ƿ���Ҫ�ڱ�����������ҽ�
		description="HYN[HYN]";
	};

	{
		itemid = 10021;
		name = "GirlLongHair";
		itemtype = EAIT_HAIR_NEWTEST;
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_FEMALE;
		description = "Ʈ�ݳ���(Ů)";
	};
	
	{
		itemid=10022;
		name="NewBoy";
		itemtype=EAIT_ROLE_NEWTEST;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;
		};
		hidemodellist=
		{
			"foot";--��
			"foot_edge";--�ź�ģ		
		};
		modelname="Avatar";
		garagetype=0;--EIGT_EQUIP;ECBT_BASE_PET;
		sextype=EASE_MALE;		
		description="���к���ɫ";
	};

	
	{
		itemid=10023;
		name="kart7";
		itemtype=EAIT_CAR_NEWTEST;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	--��������ķ���	
		description="�³��峵";
	};
	
	{
		itemid=10024;
		name="kart8";
		itemtype=EAIT_CAR_NEWTEST;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		hiderolefoot=1;
		description="��ѻ[Raven]";
	};
	
	{
		itemid = 10025;
		name = "BoyShortHair";
		itemtype = EAIT_HAIR_NEWTEST;--type, ͷ��
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_MALE;	
		description = "��ˬ�̷�(��)";
	};
	
	{
		itemid = 10026;
		name = "GirlShortHair";
		itemtype = EAIT_HAIR_NEWTEST;
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_FEMALE;
		description = "��ˬ�̷�(Ů)";
	};
	
	{
		itemid = 10027;
		name = "BoyCapHair";
		itemtype = EAIT_HAIR_NEWTEST;--type, ͷ��
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_MALE;	
		description = "��ñ�̷�(��)";
	};
	
	{
		itemid=10028;
		name="kart9";
		itemtype=EAIT_CAR_NEWTEST;--type����ɫ
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--�ö�����Ҫ��ɫʱ�����ģ������
			"Editable Poly";
			"RF_W";
			"LF_W";
			"LB_W";
			"RB_W";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		hiderolefoot=1;
		description="����[Hydro]";
	};
	
	{
		itemid=10029;
		name="black_paint";
		itemtype=EAIT_CR_PAINT;--type������
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={0,0,0.3};
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="��ɫ����";
	};	
};


