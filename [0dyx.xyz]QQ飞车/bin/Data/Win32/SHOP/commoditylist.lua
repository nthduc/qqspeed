

--[[
	--[��ɾ]
	--��Ʒģ��
	{
		Id = 332;					--��ƷID
		Name = "����֮��";			--��Ʒ����
		BaseType =  2;				--��Ʒ�����ͣ���ɫ����������װ����װ�εȣ�
		SubType = 1;				--��Ʒ�����ͣ������� �����еģ� ���ѣ�
		GeneralDescription = "��ǰ�и���.";	--��Ʒ��������
		AttributeDescription = "���+5%" ;	--��������
		Item_Count = 1;				--����Ʒ�����ĵ��߸���
		{							
			ItemID = 0;				--����ID
			ItemNum = 12;			--���߸��� 
			ItemAvailPeriod = 10;		--������Ч��
		};
		Price_count = 1;				--��Ǯ����
		{
			QQPrice = 10;			--Q�Ҽ۸�
			QQGamePrice = -1;		--QQGame��Ϸ�Ҽ۸�
			QQKartPrice = -1;			--��Ҽ۸�
			QQPointPrice = 0;		--Q ��۸�
			PriceDesc = " 365�� 20QB";	--��Ǯ����
		};
		DateSaleLimite = 0;			--ÿ����������
		MemeberRebate = 90;			--��Ա�ۿ�
		CommonRebate = 100;			--��ͨ���ۿ�
		ClientVersion = 1010611;			--�汾��
		SaleTag = 0x0001;			--��Ʒ��ǣ���Ʒ���Ƽ����������Żݣ��رյȣ�
		LevelLimit = 20;				--�ȼ�����
		HonorLimit = 12;				--��������
		OtherLimit1 = 0;				--��������1
		OtherLimit2 = 0;				--��������2
		Priority = 0;				--��Ʒ���ȼ��������ڻ�������ʾ�Ĵ���
	}
]]--

--enum ENMCommoditySaleTag
--{
	ECST_TAG_NEW			=1;			--��Ʒ
	ECST_TAG_HOT			=2;			--����
	ECST_TAG_RECOMMEND 		=4;			--�Ƽ�
	ECST_TAG_REBATE			=8;			--�ۿ�
	ECST_TAG_CLOSE			=16;		--ͣ��
--};


--enum ENMCommodityBaseType
--{
	ECBT_BASE_KART 			= 1;		--����
	ECBT_BASE_CHARACTOR 	= 2;		--��ɫ
	ECBT_BASE_KARTEQP		= 4;		--����װ��
	ECBT_BASE_CHARACTOREQP	= 8;		--��ɫװ��
	ECBT_BASE_PET			= 16;		--����
	ECBT_BASE_EFFECT		= 32;		--��Ч
	ECBT_BASE_OTHER			= 64;		--����
	ECBT_BASE_HOTRECOMMAND	= 128;		--�����Ƽ�
	
--};


--enum ENMCommoditySubType
--{
	--����
	ECST_SUB_MORNINGWIND		= 0;	--����
	ECST_SUB_THUNDERBOLT		= 1;	--����
	ECST_SUB_MARS				= 2;	--ս��
	ECST_SUB_ROSEBUSH			= 3;	--Ǿޱ
	ECST_SUB_BUSHBUCK			= 4;	--����
	
	--����װ��
	ECST_SUB_PAINT				= 10;	--����
	ECST_SUB_FLOWERPATTERN		= 11;	--����
	ECST_SUB_KARTPLATE			= 12;	--����
	ECST_SUB_KARTLIGHT			= 13;	--����
	ECST_SUB_KARTCHASISS		= 14;	--������
	
	--��ɫװ��
	ECST_SUB_HEAD				= 20;	--ͷ��
	ECST_SUB_FACE				= 21;	--����
	ECST_SUB_GLASS				= 22;	--�۾�
	
	--��Ч
	ECST_SUB_TYREMARK			= 30;	--��̥ӡ
	ECST_SUB_DIFTEFFECT			= 31;	--Ư����Ч
	ECST_SUB_ROOMEFFECT			= 32;	--������Ч
	ECST_SUB_ARENAEFFECT		= 33;	--������Ч
	ECST_SUB_WINEFFECT			= 34;	--ʤ����Ч
	
	ECST_SUB_DEFAULT			= 100;	--Ĭ������
	
--};


Version = 104;


CommodityList = 
{
--[[
	{
		Id = 1;					
		Name = "�羵";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "��������";	
		AttributeDescription = "���+5%";
		Items = 			
		{		
			{
				ItemID = 10004;			
				ItemNum = 2;			
				ItemAvailPeriod = 10000;
			};
		};
		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};
	
	{
		Id = 2;					
		Name = "ī��";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "��ǰ�и���.";	
		AttributeDescription = "���+5%";
		Items = 			
		{		
			{
				ItemID = 10005;			
				ItemNum = 1;			
				ItemAvailPeriod = 00010;
			};
		};
		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};

	{
		Id = 3;					
		Name = "è��";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "���ɵ�����";	
		AttributeDescription = "���+5%";
		Items = 			
		{		
			{
				ItemID = 10006;			
				ItemNum = 1;			
				ItemAvailPeriod = 10000;
			};
		};
		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 3;		
	};
	
	{
		Id = 4;					
		Name = "�ö�";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription ="���ɵ�����";	
		AttributeDescription = "���+5%";
		Items = 			
		{		
			{
				ItemID = 10007;			
				ItemNum = 1;			
				ItemAvailPeriod = 100000;
			};
		};
		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 6;		
	};
]]
	{
		Id = 1;					
		Name = "�ڲ�����";			
		BaseType = ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "50�� / 1 Q��\n50�� / 300 ���\n�㲻֪�����ͼ������ȷ�к��壬��ֻ֪��������һ�����صķ��ţ�һ�־���ı�־��һ��ͼ�ڵ�������";	
		AttributeDescription = "���ڳ���ɼ�С�ɵ�������˺���";
		Items = 			
		{		
			{
				ItemID = 10008;			
				ItemNum = 50;			
				ItemAvailPeriod = 0;
			};
		};
		QQPrice = 1;
		QQKartPrice = 300;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 3;		
	};
	
	{
		Id = 2;					
		Name = "�������";			
		BaseType = ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "50�� / 1 Q��\n50�� / 500 ���\n�������ʶ�������һ��֪����������һֻ��ͨ����졣";	
		AttributeDescription = "���ڳ���ɼ�С�ɵ�������˺���";
		Items = 			
		{		
			{
				ItemID = 10009;			
				ItemNum = 50;			
				ItemAvailPeriod = 0;
			};
		};
		QQPrice = 1;
		QQKartPrice = 500;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 2;		
	};

	{
		Id = 3;					
		Name = "Love����";			
		BaseType = ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 10 ���\n����love������amour������rak�����ǰ��ģ�����love������������˽�ķ��ס��� ";	
		AttributeDescription = "װ�ΰ����ĳ��ơ�";
		Items = 			
		{		
			{
				ItemID = 10011;			
				ItemNum = 2;			
				ItemAvailPeriod = 10000;
			};
		};
		QQPrice = 1;
		QQKartPrice = 10;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};

	
	{
		Id = 4;
		Name = "��ɫ����";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 10 ���\n��ɫ�����ҡ��嶯��ǿ������ɫ�ʣ������Ż������������ȳϣ���ů����������ֱǰ��";	
		AttributeDescription = "������Ϳװ��ѣĿ�ĺ�ɫ��";
		Items = 
		{
			{
				ItemID = 10014;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 10;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;		
		CommonRebate = 100;			
		ClientVersion = 1010611;			
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
	
	{
		Id = 5;
		Name = "��ɫ����";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 10 ���\n�߹�Ļ�ɫ����ʥ��ׯ�ϣ�������һ�ֽ�������̬�߸����ϣ����׵ķ�����˵��޴���ų����";	
		AttributeDescription = "������Ϳװ�ɸ߹�Ļ�ɫ��";
		Items = 
		{
			{
				ItemID = 10015;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 10;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;		
		CommonRebate = 100;				
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
					
	{
		Id = 6;
		Name = "��ɫ����";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 10 ���\n��ɫ�ǲ����ɫ�ʣ���պʹ��������ľ�ɫ����ε��ɫ����ɫ�������������";	
		AttributeDescription = "������Ϳװ�����ŵ���ɫ��";
		Items = 
		{
			{
				ItemID = 10016;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 10;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;		
		CommonRebate = 100;				
		ClientVersion = 1010611;		
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
	
	{
		Id = 7;
		Name = "��ɫ����";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 50 ���\n��ɫ���䡢���Ľ�ϣ������ڳ�Ĭ�б������ڲ�ͬ�Ŀ��š�";	
		AttributeDescription = "������Ϳװ�����ŵĺ�ɫ��";
		Items = 
		{
			{
				ItemID = 10029;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 50;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;		
		CommonRebate = 100;				
		ClientVersion = 1010611;		
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};		
--[[	
	{
		Id = 7;					
		Name = "����Ů";			
		BaseType = ECBT_BASE_CHARACTOR;
		SubType = 1;
		GeneralDescription = "�ɰ�������Ů";	
		AttributeDescription = "�������";
		Items = 			
		{		
			{
				ItemID = 10018;			
				ItemNum = 1;			
				ItemAvailPeriod = 10000;
			};
			{
				ItemID = 10021;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};
		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 1;		
	};
]]
	
	{
		Id = 8;
		Name = "HYN[HYN]";			
		BaseType = ECBT_BASE_KART;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 1 ���\nǿ��������������Ĳٿأ�������������������ˮ��ļ�ʻ���磡";	
		AttributeDescription = " ";
		Items = 
		{
			{
				ItemID = 10020;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;
		CommonRebate = 100;				
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
	{
		Id = 9;
		Name = "����[Hydro]";			
		BaseType = ECBT_BASE_KART;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 500 ���\n��˵���������������ѧ���۵����������������Ͼ��ܶ��������׹�˾��������";	
		AttributeDescription = " ";
		Items = 
		{
			{
				ItemID = 10028;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 500;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;
		CommonRebate = 100;				
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
--[[
	{
		Id = 14;
		Name = "Ů��ͷ��";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "Ʈ��Ʈ��";	
		AttributeDescription = "���+5%";
		Items = 
		{
			{
				ItemID = 10021;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;				
		ClientVersion = 1010611;
		CommonRebate = 100;				
		SaleTag = ECST_TAG_HOT;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
	
	{
		Id = 9;
		Name = "���к�";			
		BaseType = ECBT_BASE_CHARACTOR;
		SubType = 1;
		GeneralDescription = "�����������������������";	
		AttributeDescription = "���+5%";
		Items = 
		{
			{
				ItemID = 10022;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
			{
				ItemID = 10025;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 1;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;				
		ClientVersion = 1010611;
		CommonRebate = 100;				
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
		
]]
	{
		Id = 10;
		Name = "��ѻ[Raven]";			
		BaseType = ECBT_BASE_KART;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 50 ���\n�ڳ�������ϲ����ˡ�����������ƣ���ӯ�����ֲ����ƶ����ѷ������㡣";	
		AttributeDescription = "";
		Items = 
		{
			{
				ItemID = 10024;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 50;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;		
		CommonRebate = 100;			
		ClientVersion = 1010611;		
		SaleTag = ECST_TAG_RECOMMEND;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};
	{
		Id = 11;					
		Name = "QQKart����";			
		BaseType =ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 10 ���\n������QQ��־�ĳ���������۱������߹�ļ�ֵ����ݵ�������";	
		AttributeDescription = "װ�ΰ����ĳ��ơ�";
		Items = 			
		{		
			{
				ItemID = 10010;			
				ItemNum = 1;			
				ItemAvailPeriod = 100000;
			};
		};
		QQPrice = 1;
		QQKartPrice = 10;
		QQPointPrice = 1;	
		DateSaleLimite = 0;			
		MemeberRebate = 90;			
		CommonRebate = 100;					
		ClientVersion = 1010611;	
		SaleTag = ECST_TAG_NEW ;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 2;		
	};
	{
		Id = 12;
		Name = "��ˬ�̷�(��)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 20 ���\nJayʽ��ƫ�����ͣ��ǳ��ʺϳ������������������֡�";	
		AttributeDescription = "�ı��к���ͷ�����͡�";
		Items = 
		{
			{
				ItemID = 10025;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 20;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;				
		ClientVersion = 1010611;
		CommonRebate = 100;				
		SaleTag = ECST_TAG_HOT;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};
	{
		Id = 13;
		Name = "Ʈ�ݳ���(Ů)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 20 ���\n�˶���ʮ�������ֱ����˭˵����ֻ���к��ӵ����£�";	
		AttributeDescription = "�ı�Ů����ͷ�����͡�";
		Items = 
		{
			{
				ItemID = 10021;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 20;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;				
		ClientVersion = 1010611;
		CommonRebate = 100;				
		SaleTag = ECST_TAG_HOT;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};		
	{
		Id = 14;
		Name = "��ñ�̷�(��)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 20 ���\n�ҵ�����������������רҵ������ñ���Ҿ����ݺ�����������ա�";	
		AttributeDescription = "�ı��к���ͷ�����͡�";
		Items = 
		{
			{
				ItemID = 10027;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 20;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;				
		ClientVersion = 1010611;
		CommonRebate = 100;				
		SaleTag = ECST_TAG_HOT;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};
	
	{
		Id = 15;
		Name = "ʱ�ж̷�(Ů)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60�� / 1 Q��\n60�� / 20 ���\n�ٶ����ҵ�ʱ�У��������ҵ���̨����׷�ң����Ͳ����Űɣ�";	
		AttributeDescription = "�ı�Ů����ͷ�����͡�";
		Items = 
		{
			{
				ItemID = 10026;
				ItemAvailPeriod = 10000;
				ItemNum = 1;
			};
		};

		QQPrice = 1;
		QQKartPrice = 20;
		QQPointPrice = 1;
	
		DateSaleLimite = 60;
		MemeberRebate = 90;				
		ClientVersion = 1010611;
		CommonRebate = 100;				
		SaleTag = ECST_TAG_HOT;			
		LevelLimit = 20;			
		HonorLimit = 12;		
		OtherLimit = 0;			
		Priority = 0;		
	};	
};



