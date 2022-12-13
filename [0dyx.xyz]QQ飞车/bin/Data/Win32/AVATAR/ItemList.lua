--道具item作用的触发时机
EITO_GETPROP=1; --捡道具时
EITO_USEPROP=2; --使用道具时
EITO_ATTACKED=4; --受到攻击时
EITO_GAMEEND=8;  --游戏结束时

--属性算法类型
EIAAT_ADDMONEY=1000; --加上金币, 操作数3个: 条件, 金币增量, 是否需要人物属性卡(0或1)
EIAAT_ADDEXPERIENCE=1001; --加上经验, 操作数3个: 条件, 经验增量, 是否需要人物属性卡(0或1)
EIAAT_MULTIPLYMONEY=1002; --乘以金币倍数, 操作数3个: 条件, 金币倍数, 是否需要人物属性卡(0或1)
EIAAT_MULTIPLYEXPERIENCE=1003; --乘以经验倍数, 操作数3个: 条件, 经验倍数, 是否需要人物属性卡(0或1)
EIAAT_PROPTRANSFER=1004; --道具转换, 操作数1个: 目的道具ID
EIAAT_EFFECTTRANSFER=1005; --效果转换, 操作数1个:目的效果ID(此处为引起该效果的propID)
EIAAT_ITEMCONSUME=1006; --道具item消耗, 操作数1个: 消耗值
EIAAT_ADDMONEYINRACE=1007; --游戏中增加金币, 操作数2个: 金币增量, 是否需要人物属性卡(0或1)
EIAAT_MINUSTIME=1008;	--缩短时间，操作数1个：时间值(ms)

--经验或积分奖励条件
EBC_FIRSTORWIN=1; --个人赛第一名, 组队赛胜利
EBC_TOPORWIN=2; --个人赛前三名, 组队赛胜利
EBC_FIRSTORWININ8=3; --在8人的比赛中, 个人赛第一名, 组队赛胜利
EBC_OTHERNOFINISHIN8=4; --在8人的比赛中其他人均未完成

--道具ID
EPID_SPEEDUP=1; --加速卡, 氮气
EPID_FLY=2; --水苍蝇, 跟踪橡胶炸弹
EPID_MISSILE=3; --导弹
EPID_REDUCER=4; --飞碟, 减速器
EPID_MAGNET=5; --磁铁
EPID_EVIL=6; --恶魔
EPID_WATERBOMB=7; --水炸弹, 投掷网
EPID_SMEAR=8; --香蕉皮, 油污
EPID_CLOUD=9; --乌云, 迷雾
EPID_DEFEND=10; --防护盾, 保护罩
EPID_ANGLE=11; --天使
EPID_TIMEBOMB=12; --定时炸弹
EPID_RESCINDREDUCER=13; --解除减速

--avatar道具基本类型定义
EAIBT_ROLERELEVANT=256; --0x00000100,角色相关类型
EAIBT_CARRELEVANT=512; --0x00000200,赛车相关类型
EAIBT_NORELEVANT=768;--0x00000300,与角色赛车无关的类型
EAIBT_NOENTITY=1024; --0x00000400,非实体类型
EAIBT_ROLE=1280; --0x00000500,角色
EAIBT_CAR=1536; --0x00000600,赛车
EAIBT_HAIR=1792;--0X00000700, 头发

--avatar道具类型定义(基本类型+子类型)
EAIT_RR_TOPHEAD=257; --0x00000101,角色头顶
EAIT_RR_GLASS=258; --0x00000102,角色眼镜
EAIT_RR_FACE=259; --0x00000103,角色面部贴图
EAIT_RR_DV=260; --0x00000104,角色摄像机（预留）
EAIT_RR_HEADSHADE=261; --0x00000105,角色头部形状（预留）
EAIT_RR_BODYUP=262; --0x00000106,角色上躯体（包括手臂）
EAIT_RR_PALM=263; --0x00000107,角色手掌
EAIT_RR_TATTOO=264; --0x00000108,角色手臂纹身
EAIT_RR_BODYDOWN=265; --0x00000109,角色下躯体

EAIT_CR_TEXTURE=513; --0x00000201,赛车花纹或纹理
EAIT_CR_PAINT=514; --0x00000202,赛车喷漆
EAIT_CR_BRAND=515; --0x00000203,赛车车牌
EAIT_CR_TAIL=516; --0x00000204,赛车尾挂
EAIT_CR_LAMPSEPCIAL=517; --0x00000205,赛车车灯特效
EAIT_CR_BRANDSPECIAL=518; --0x00000206,赛车车牌特效
EAIT_CR_BOTTOMLAMP=519; --0x00000207,赛车底灯
EAIT_CR_EJECTSEPCIAL=520; --0x00000208,赛车喷射特效
EAIT_NR_TYREMARK=769; --0x00000301,轮胎印
EAIT_NR_DRIFTSPECIAL=770; --0x00000302,漂移特效
EAIT_NE_ROOMSPECIAL=1025; --0x00000401,房间特效
EAIT_NE_SCENESPECIAL=1026; --0x00000402,赛道场景特效
EAIT_NE_WINSPECIAL=1027; --0x00000403,胜利特效
EAIT_NE_PROPSPECIAL=1028; --0x00000404,游戏道具特效
EAIT_NE_EXPRESSIONSPECIAL=1029; --0x00000405,表情特效
EAIT_NE_ROLEATTRIBUTECARD=1030; --0x00000406,人物属性卡
EAIT_NE_PROPEXCHANGE=1031; --0x00000407,道具换位卡
EAIT_ROLE_BASE=1281; --0x00000501,--常规角色系列
EAIT_CAR_BASE=1537; --0x00000601,--常规赛车系列

EAIT_ROLE_NEWTEST=1282; --0x00000502,--测试用新角色系列
EAIT_CAR_NEWTEST=1538; --0x00000602,--测试用新赛车系列
EAIT_HAIR_NEWTEST=1794;--0X00000702,--新人物头发

--装备使用资源类型
EIRES_SIG_NIF=1;		--单模型
EIRES_SIG_TEX=2;		--单贴图
EIRES_MIX_SIG_NIFTEX=3;	--混合单模型贴图
EIRES_MUL_NIF=4;		--多模型
EIRES_MUL_TEX=8;		--多贴图
EIRES_MIX_MUL_NIFTEX=12;--混合多模型多贴图
EIRES_TEX_COLOR=16;		--带颜色属性
EIRES_SIG_COLOR_TEX=18;	--带颜色属性单贴图
EIRES_SIG_SKIN=32;	--带骨骼数据
EIRES_MIX_SIG_SKINNIF=33;	--带骨骼单模型
EIRES_EXT_MODE=512;		--扩展模式，使用独立类型处理


--装备特殊行为类型
EBI_Animation = 1;	--播放动作
EBI_Effect = 2;		--播放特效
EBI_Sound = 3;		--播放声音


--转换算法类型
EAI_GPTansfer = 1;      --按一定概率转换道具攻击
EAI_GPReduceTime = 2;   --按一定概率减少攻击作用时间

--装备转换算法的算子
EOI_Add = 0;	--加
EOI_Mul = 1;	--乘

--bool值
BFG_Forbid = 0;	--禁止
BFG_Can = 1;	--可以

--装备分类，用于车库和商店显示分类，uires.lua有副本
ECBT_BASE_KART 		=1;	--赛车
ECBT_BASE_CHARACTOR	=2;	--角色
ECBT_BASE_KARTEQP	=4;	--赛车装备
ECBT_BASE_CHARACTOREQP	=8;	--角色装备
ECBT_BASE_PET		=16;	--宠物
ECBT_BASE_EFFECT	=32;	--特效
ECBT_BASE_OTHER		=64;	--其他
ECBT_BASE_HOTRECOMMAND	=128;	--人气推荐（商品专用）

--性别分类
EASE_MALE=1;	--男性
EASE_FEMALE=2;	--女性


--挂接点列表,name对应模型中的挂接点名称,识别大小写
HitchList=
{
	{
		typeid=EAIBT_ROLE;--角色统一装备位
		name="role";
		defaultitemid=10000;	--默认的装备
	};
	{
		typeid=EAIBT_CAR;--车辆统一装备位
		name="kart";
		defaultitemid=10003;	--默认的装备
	};
	{
		typeid=EAIBT_HAIR;--头发统一装备位
		name="hair";
		defaultitemid=10021;	--默认的装备
	};
	{
		typeid=EAIT_ROLE_BASE;--普通角色
		name="role_base";
		defaultitemid=10000;	--默认的装备
	};
	{
		typeid=EAIT_CAR_BASE;--普通车
		name="kart_base";
		defaultitemid=10003;
	};	
	{
		typeid=EAIT_RR_TOPHEAD;--头顶
		name="char_head";
	};
	{
		typeid=EAIT_RR_GLASS;--眼镜
		name="char_glass";
	};
	{
		typeid=EAIT_RR_FACE;--脸部贴图
		name="char_face";
	};
	{
		typeid=EAIT_RR_BODYUP;--角色上躯体（包括手臂）
		name="char_bodyup";
	};
	{
		typeid=EAIT_RR_PALM;--角色手掌
		name="char_palm";
	};
	{
		typeid=EAIT_RR_TATTOO;--角色手臂纹身
		name="char_tattoo";
	};
	{
		typeid=EAIT_RR_BODYDOWN;--角色下躯体
		name="char_bodydown";
	};
	------------------------------------------
	{
		typeid=EAIT_CR_BRAND;--车牌
		name="CP";
	};	
	{
		typeid=EAIT_CR_TAIL;--尾挂
		name="car_tail";
	};	
	{
		typeid=EAIT_CR_PAINT;--喷漆
		name="car_paint";
		defaultitemid=10014;
	};
	{
		typeid=EAIT_ROLE_NEWTEST;--新角色
		name="role_newtest";
		defaultitemid=10018;
	};
	{
		typeid=EAIT_CAR_NEWTEST;--新车
		name="kart_newtest";
		defaultitemid=10017;
	};
	{
		typeid = EAIT_HAIR_NEWTEST;--头发， 新人物专用
		name="hair_newtest";
		defaultitemid=10021;
	};
};

ItemList=
{
	{
		itemid=10000;--从10000开始
		name="seaboy";
		itemtype=EAIT_ROLE_BASE;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{--该对象拥有的挂接点表
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;
		};
		modelname="Avatar";	--模型实体在nif文件里的实际名称，用于旋转，或者处理等功能
		garagetype=ECBT_BASE_PET;	--车库里面的分类
		sextype=EASE_MALE;	--性别分类
 		description="海王子";
	};
	--下一个道具
	{	
		itemid=10001;
		name="xiaoqian";
		itemtype=EAIT_ROLE_BASE;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;	
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_PET;	
		sextype=EASE_FEMALE;		
		description="小倩";
	};	
	--下一个道具
	{
		itemid=10002;
		name="kart1";
		itemtype=EAIT_CAR_BASE;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"Editable Poly@#0";--车体
			"Editable Poly";--车轮
			"Editable Poly@#1";
			"Editable Poly@#5";
			"Editable Poly@#6";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		description="红色车";
	};
	--下一个道具
	{
		itemid=10003;
		name="kart2";
		itemtype=EAIT_CAR_BASE;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"carbody";--车体
			"Editable Poly";--车轮
			"Editable Poly@#1";
			"Editable Poly@#2";
			"Editable Poly@#3";			
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	
		description="板车";
	};		
	--下一个道具
	{
		itemid=10004;--从10000开始
		name="feng_glass";
		itemtype=EAIT_RR_GLASS;--type
		restype=EIRES_SIG_NIF;
		modelname="char_glass";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="风镜";
	};

	--下一个道具
	{
		itemid=10005;--从10000开始
		name="sun_glass";
		itemtype=EAIT_RR_GLASS;--type
		restype=EIRES_SIG_NIF;
		modelname="char_glass";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="墨镜";
	};

    	--下一个道具
	{
		itemid=10006;--从10000开始
		name="cat_ear";
		itemtype=EAIT_RR_TOPHEAD;--type
		restype=EIRES_SIG_NIF;
		modelname="char_head";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="猫耳";
	};

	--下一个道具
	{
		itemid=10007;--从10000开始
		name="rabbit_ear";
		itemtype=EAIT_RR_TOPHEAD;--type
		restype=EIRES_SIG_NIF;
		modelname="char_head";
		garagetype=ECBT_BASE_CHARACTOREQP;
		description="兔耳";
	};

	--下一个道具
	{
		itemid=10008;--从10000开始
		name="white_balloon";
		itemtype=EAIT_CR_TAIL;--type
		restype=EIRES_SIG_NIF;
		property=
		{
		    switch = BFG_Forbid;
		    nofog = BFG_Forbid;
		    physicPropt=
		    {
				operator = EOI_Add;         --给予操作的算子
				speed = 0;            --速度属性
				erupt = 0;            --爆发属性
				accumno2 = 0;         --积累氮气属性
				bigno2 = 0;          --大喷属性
				minino2 = 0;          --小喷属性
			};

			algorithm=            	--算法
			{
				{
			    	type = EAI_GPTansfer;         --类型
			    	occasion = EITO_ATTACKED;   	--触发时机
			    	operator = EOI_Add;  	--算子
			    	condition = EPID_MISSILE;  	--条件参数
			    	expectation = 2004;	--期望值
			    	rate = 50;       	--得到期望值的概率
			    };
			};
		};
		modelname="objpolySurface6";
		garagetype=ECBT_BASE_KARTEQP;
		description="腾彩气球";
	};			
	--下一个道具
	{
		itemid=10009;--从10000开始
		name="red_balloon";
		itemtype=EAIT_CR_TAIL;--type
		restype=EIRES_SIG_NIF;
		modelname="objpolySurface11";
		garagetype=ECBT_BASE_KARTEQP;
		description="企鹅气球";
		
	};				
	--下一个道具
	{
		itemid=10010;--从10000开始
		name="qqkart";
		itemtype=EAIT_CR_BRAND;--type
		restype=EIRES_SIG_TEX;
		modelname="Box01";
		garagetype=ECBT_BASE_KARTEQP;
		description="QQKart车牌";
	};
	--下一个道具
	{
		itemid=10011;--从10000开始
		name="love";
		itemtype=EAIT_CR_BRAND;--type
		restype=EIRES_SIG_TEX;
		modelname="Box01";
		garagetype=ECBT_BASE_KARTEQP;
		description="Love车牌";
	};

	--下一个道具
	{
		itemid=10012;
		name="orange";
		itemtype=EAIT_ROLE_BASE;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;			
		};
		modelname="Avatar";	
		garagetype=ECBT_BASE_PET;	--车库里面的分类
		sextype=EASE_FEMALE;		
		description="小橘子";
	};
	--下一个道具
	{
		itemid=10013;
		name="kart3";
		itemtype=EAIT_CAR_BASE;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	--车库里面的分类	
		description="橙色车";
	};
	--下一个道具
	{
		itemid=10014;
		name="red_paint";
		itemtype=EAIT_CR_PAINT;--type，喷漆
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={0,1.2,3.5};	--使用色度表示颜色
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="红色喷漆";
	};
	--下一个道具
	{
		itemid=10015;
		name="yellow_paint";
		itemtype=EAIT_CR_PAINT;--type，喷漆
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={40,1.5,3};	--使用色度表示颜色
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="黄色喷漆";
	};
	--下一个道具
	{
		itemid=10016;
		name="blue_paint";
		itemtype=EAIT_CR_PAINT;--type，喷漆
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={200,2,3};
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="蓝色喷漆";
	};
	--下一个道具
	{
		itemid=10017;
		name="NewKart1";
		itemtype=EAIT_CAR_NEWTEST;--type，车
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"carbody:0";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	--车库里面的分类	
		description="蓝色新车";
	};
	--下一个道具
	{
		itemid=10018;
		name="NewGirl";
		itemtype=EAIT_ROLE_NEWTEST;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;			
		};
		hidemodellist=
		{--该对象需要在游戏过程中隐藏处理的模型名称
			"foot";--脚
			"foot_edge";--脚黑模		
		};
		modelname="Avatar";	
		garagetype=0;--ECBT_BASE_PET;
		sextype=EASE_FEMALE;		
		description="新女孩角色";
	};
	--下一个道具
	{
		itemid=10019;
		name="NewKart2";
		itemtype=EAIT_CAR_NEWTEST;--type，车
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	
		description="红色新车";
	};
	--下一个道具
	{
		itemid=10020;
		name="NewKart3";
		itemtype=EAIT_CAR_NEWTEST;--type，车
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"Editable Poly";
			--"carbody:1";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		hiderolefoot=0;	--是否需要在比赛中隐藏玩家脚
		description="HYN[HYN]";
	};

	{
		itemid = 10021;
		name = "GirlLongHair";
		itemtype = EAIT_HAIR_NEWTEST;
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_FEMALE;
		description = "飘逸长发(女)";
	};
	
	{
		itemid=10022;
		name="NewBoy";
		itemtype=EAIT_ROLE_NEWTEST;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_RR_TOPHEAD;
			EAIT_RR_GLASS;
		};
		hidemodellist=
		{
			"foot";--脚
			"foot_edge";--脚黑模		
		};
		modelname="Avatar";
		garagetype=0;--EIGT_EQUIP;ECBT_BASE_PET;
		sextype=EASE_MALE;		
		description="新男孩角色";
	};

	
	{
		itemid=10023;
		name="kart7";
		itemtype=EAIT_CAR_NEWTEST;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;	--车库里面的分类	
		description="新长板车";
	};
	
	{
		itemid=10024;
		name="kart8";
		itemtype=EAIT_CAR_NEWTEST;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"carbody";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		hiderolefoot=1;
		description="渡鸦[Raven]";
	};
	
	{
		itemid = 10025;
		name = "BoyShortHair";
		itemtype = EAIT_HAIR_NEWTEST;--type, 头发
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_MALE;	
		description = "清爽短发(男)";
	};
	
	{
		itemid = 10026;
		name = "GirlShortHair";
		itemtype = EAIT_HAIR_NEWTEST;
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_FEMALE;
		description = "清爽短发(女)";
	};
	
	{
		itemid = 10027;
		name = "BoyCapHair";
		itemtype = EAIT_HAIR_NEWTEST;--type, 头发
		restype= EIRES_MIX_SIG_SKINNIF;
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"hair";
			"hair_edge";
		};
		modelname="Avatar";
		garagetype=ECBT_BASE_CHARACTOREQP;
		sextype=EASE_MALE;	
		description = "戴帽短发(男)";
	};
	
	{
		itemid=10028;
		name="kart9";
		itemtype=EAIT_CAR_NEWTEST;--type，角色
		restype=EIRES_SIG_NIF;
		hitchlist=
		{
			EAIT_CR_BRAND;
			EAIT_CR_TAIL;
			EAIT_CR_PAINT;
		};
		changecolorlist=
		{--该对象需要换色时处理的模型名称
			"Editable Poly";
			"RF_W";
			"LF_W";
			"LB_W";
			"RB_W";
		};
		modelname="MODE";
		garagetype=ECBT_BASE_KART;
		hiderolefoot=1;
		description="流体[Hydro]";
	};
	
	{
		itemid=10029;
		name="black_paint";
		itemtype=EAIT_CR_PAINT;--type，喷漆
		restype=EIRES_SIG_COLOR_TEX;
		texcolor={0,0,0.3};
		modelname="objpCylinder1";
		garagetype=ECBT_BASE_KARTEQP;
		description="黑色喷漆";
	};	
};


