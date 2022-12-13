

--[[
	--[勿删]
	--商品模板
	{
		Id = 332;					--商品ID
		Name = "幸运之星";			--商品名称
		BaseType =  2;				--商品大类型（角色，卡丁车，装备，装饰等）
		SubType = 1;				--商品子类型（卡丁车 大类中的： 爆裂）
		GeneralDescription = "从前有个庙.";	--商品总体描述
		AttributeDescription = "金币+5%" ;	--属性描述
		Item_Count = 1;				--该商品包含的道具个数
		{							
			ItemID = 0;				--道具ID
			ItemNum = 12;			--道具个数 
			ItemAvailPeriod = 10;		--道具有效期
		};
		Price_count = 1;				--价钱种类
		{
			QQPrice = 10;			--Q币价格
			QQGamePrice = -1;		--QQGame游戏币价格
			QQKartPrice = -1;			--酷币价格
			QQPointPrice = 0;		--Q 点价格
			PriceDesc = " 365天 20QB";	--价钱描述
		};
		DateSaleLimite = 0;			--每天销量限制
		MemeberRebate = 90;			--会员折扣
		CommonRebate = 100;			--普通人折扣
		ClientVersion = 1010611;			--版本号
		SaleTag = 0x0001;			--商品标记（新品，推荐，热卖，优惠，关闭等）
		LevelLimit = 20;				--等级限制
		HonorLimit = 12;				--荣誉限制
		OtherLimit1 = 0;				--其他限制1
		OtherLimit2 = 0;				--其他限制2
		Priority = 0;				--商品优先级（决定在货架上显示的次序）
	}
]]--

--enum ENMCommoditySaleTag
--{
	ECST_TAG_NEW			=1;			--新品
	ECST_TAG_HOT			=2;			--热卖
	ECST_TAG_RECOMMEND 		=4;			--推荐
	ECST_TAG_REBATE			=8;			--折扣
	ECST_TAG_CLOSE			=16;		--停售
--};


--enum ENMCommodityBaseType
--{
	ECBT_BASE_KART 			= 1;		--赛车
	ECBT_BASE_CHARACTOR 	= 2;		--角色
	ECBT_BASE_KARTEQP		= 4;		--赛车装备
	ECBT_BASE_CHARACTOREQP	= 8;		--角色装备
	ECBT_BASE_PET			= 16;		--宠物
	ECBT_BASE_EFFECT		= 32;		--特效
	ECBT_BASE_OTHER			= 64;		--其他
	ECBT_BASE_HOTRECOMMAND	= 128;		--人气推荐
	
--};


--enum ENMCommoditySubType
--{
	--赛车
	ECST_SUB_MORNINGWIND		= 0;	--晨风
	ECST_SUB_THUNDERBOLT		= 1;	--雷霆
	ECST_SUB_MARS				= 2;	--战神
	ECST_SUB_ROSEBUSH			= 3;	--蔷薇
	ECST_SUB_BUSHBUCK			= 4;	--羚羊
	
	--赛车装备
	ECST_SUB_PAINT				= 10;	--喷漆
	ECST_SUB_FLOWERPATTERN		= 11;	--花纹
	ECST_SUB_KARTPLATE			= 12;	--车牌
	ECST_SUB_KARTLIGHT			= 13;	--车灯
	ECST_SUB_KARTCHASISS		= 14;	--车底盘
	
	--角色装备
	ECST_SUB_HEAD				= 20;	--头饰
	ECST_SUB_FACE				= 21;	--脸饰
	ECST_SUB_GLASS				= 22;	--眼镜
	
	--特效
	ECST_SUB_TYREMARK			= 30;	--轮胎印
	ECST_SUB_DIFTEFFECT			= 31;	--漂移特效
	ECST_SUB_ROOMEFFECT			= 32;	--房间特效
	ECST_SUB_ARENAEFFECT		= 33;	--赛场特效
	ECST_SUB_WINEFFECT			= 34;	--胜利特效
	
	ECST_SUB_DEFAULT			= 100;	--默认子类
	
--};


Version = 104;


CommodityList = 
{
--[[
	{
		Id = 1;					
		Name = "风镜";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "看穿迷雾";	
		AttributeDescription = "金币+5%";
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
		Name = "墨镜";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "从前有个庙.";	
		AttributeDescription = "金币+5%";
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
		Name = "猫耳";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "防飞碟攻击";	
		AttributeDescription = "金币+5%";
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
		Name = "兔耳";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription ="防飞碟攻击";	
		AttributeDescription = "金币+5%";
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
		Name = "腾彩气球";			
		BaseType = ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "50个 / 1 Q币\n50个 / 300 酷币\n你不知道这个图像代表的确切含义，你只知道，它是一种神秘的符号，一种精神的标志，一种图腾的象征。";	
		AttributeDescription = "挂在车后可减小飞弹对你的伤害。";
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
		Name = "企鹅气球";			
		BaseType = ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "50个 / 1 Q币\n50个 / 500 酷币\n如果你认识它，你就一定知道，它不是一只普通的企鹅。";	
		AttributeDescription = "挂在车后可减小飞弹对你的伤害。";
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
		Name = "Love车牌";			
		BaseType = ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 10 酷币\n爱是love，爱是amour，爱是rak，爱是爱心，爱是love，爱是正大无私的奉献…… ";	
		AttributeDescription = "装饰爱车的车牌。";
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
		Name = "红色喷漆";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 10 酷币\n红色，热烈、冲动、强有力的色彩；传达着活力，积极，热诚，温暖，让你勇往直前。";	
		AttributeDescription = "把赛车涂装成眩目的红色。";
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
		Name = "黄色喷漆";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 10 酷币\n高贵的黄色，神圣、庄严，让你以一种骄傲的姿态高高在上，轻易的粉碎敌人的愚蠢和懦弱。";	
		AttributeDescription = "把赛车涂装成高贵的黄色。";
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
		Name = "蓝色喷漆";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 10 酷币\n蓝色是博大的色彩，天空和大海这辽阔的景色都呈蔚蓝色。蓝色是永恒的象征。";	
		AttributeDescription = "把赛车涂装成优雅的蓝色。";
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
		Name = "黑色喷漆";			
		BaseType = ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 50 酷币\n黑色是冷、静的结合，让你在沉默中爆发与众不同的酷炫。";	
		AttributeDescription = "把赛车涂装成冷炫的黑色。";
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
		Name = "美少女";			
		BaseType = ECBT_BASE_CHARACTOR;
		SubType = 1;
		GeneralDescription = "可爱的美少女";	
		AttributeDescription = "人气飙升";
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
		GeneralDescription = "60天 / 1 Q币\n60天 / 1 酷币\n强劲的马力配合灵活的操控，让人置身于如行云流水般的驾驶境界！";	
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
		Name = "流体[Hydro]";			
		BaseType = ECBT_BASE_KART;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 500 酷币\n据说这是最符合流体力学理论的赛车。从其外型上就能读出安布雷公司的设计理念！";	
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
		Name = "女孩头发";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "飘啊飘。";	
		AttributeDescription = "金币+5%";
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
		Name = "新男孩";			
		BaseType = ECBT_BASE_CHARACTOR;
		SubType = 1;
		GeneralDescription = "有了我赛车世界才算完整。";	
		AttributeDescription = "金币+5%";
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
		Name = "渡鸦[Raven]";			
		BaseType = ECBT_BASE_KART;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 50 酷币\n在车身设计上采用了“后掠翼”的设计，轻盈车身弥补了推动力匮乏的弱点。";	
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
		Name = "QQKart车牌";			
		BaseType =ECBT_BASE_HOTRECOMMAND+ECBT_BASE_KARTEQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 10 酷币\n打上了QQ标志的车牌让您身价倍增，高贵的价值是身份的象征。";	
		AttributeDescription = "装饰爱车的车牌。";
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
		Name = "清爽短发(男)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 20 酷币\nJay式的偏分造型，非常适合充满活力的年轻赛车手。";	
		AttributeDescription = "改变男孩的头部造型。";
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
		Name = "飘逸长发(女)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 20 酷币\n运动感十足的披肩直发，谁说赛车只是男孩子的天下！";	
		AttributeDescription = "改变女孩的头部造型。";
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
		Name = "戴帽短发(男)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 20 酷币\n我的赛道我做主，戴上专业的赛车帽，我就是纵横赛场的舒马赫。";	
		AttributeDescription = "改变男孩的头部造型。";
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
		Name = "时尚短发(女)";			
		BaseType = ECBT_BASE_CHARACTOREQP;
		SubType = 1;
		GeneralDescription = "60天 / 1 Q币\n60天 / 20 酷币\n速度是我的时尚，赛道是我的舞台。想追我，就猛踩油门吧！";	
		AttributeDescription = "改变女孩的头部造型。";
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



