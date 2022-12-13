--enmUseType(道具使用类型)
EUT_Active		= 0;	--主动使用型
EUT_Passive	= 1;	--被动使用型
--enmAffectType(道具作用类型)
EAT_Attack	= 0;	--攻击型道具
EAT_Protect	= 1;	--防护型道具
EAT_Generator	= 2;	--道具生成型道具
EAT_Cure		= 3; 	--治疗型道具
--enmTargetType(道具作用目标类型)
ETT_Play		= 0;	--所有人
ETT_Enemy		= 1;	--所有敌人
ETT_FirstPlay	= 2;  --第一名
ETT_PrevPlay	= 3;  --前一名
ETT_PrevEnemy	= 4;  --前一名敌人
ETT_Me			= 5;	--自己
--enmCannotUseCondition
ECUC_None		= 0;	--使用不受任何限制
ECUC_OneExists	= 1;	--已经有一个道具正在作用的时候
ECUC_NoAccel	= 2;	--不在加速的时候
--enmSelectTargetType
ESTT_NoSelect	= 0;	--不需要瞄准，直接使用
ESTT_Select	= 1;	--需要瞄准才能使用
--enmProtectType(防护型道具的防护方式)
EPT_AllAttack	= 0;	--对所有攻击免疫
EPT_All		= 1;	--对所有攻击和防护都免疫
EPT_Specified	= 2;	--免疫特定道具
--enmProtectTargetType(防护型道具的作用范围)
EPT_Me		= 100;	--保护自己
EPT_Team		= 101;	--保护整个队伍
--enmMoveType(运动类型)
EMT_None		= 0;	--无运动过程（绝大多数道具的默认行为）
EMT_Missile	= 1;	--导弹
EMT_Flying		= 2; 	--水苍蝇
--enmStaticProp(被动型道具特有的属性)
ESP_None		= 0;
ESP_Shelter	= 1;	--遮挡（迷雾使用）
--enmBlendMode(多道具互斥模式)
EBM_AddNext	= 0;	--本道具作用尚未结束，如果有后一个道具来作用，不停止当前作用，让下一个道具生效，作用叠加
EBM_UseNext	= 1;	--本道具作用尚未结束，如果有后一个道具来作用，马上停止当前作用，让下一个道具生效
EBM_IgnoreNext	= 2;	--本道具作用尚未结束，忽略下一个道具作用
EBM_ForceReplaceOld	= 3;	--前一个道具作用尚未结束时，本道具来到，强制结束上一个道具的作用，即使前一个道具是EBM_IgnoreNext的。
--enmAffectID
EAI_None		= 0;	--无表现
EAI_Lift		= 1;	--被抬起，参数：被抬起的高度
EAI_Magnet	= 2;	--磁铁
EAI_Stop		= 3;	--停止
EAI_InvertLR	= 4;	--左右方向键反置，参数：TRUE（设置），FALSE（取消）
EAI_FreezeGP	= 5;	--不允许使用道具
EAI_FreezeOp	= 6;	--不运行任何操作
EAI_No2		= 7; 	--氮气
EAI_Animation	= 8;	--播放动作，参数：动作ID
EAI_Effect		= 9;	--参数：effectid
EAI_Sound		= 10;
EAI_Speed		= 11; --控制速度（施加力）
EAI_Jet		= 12; --喷火
EAI_Evil	= 13; --魔王道具发光特效
EAI_Generator	= 14;--生成新道具
EAI_No2Emitter	= 15;--小喷触发器

--enmSpecialEffect
ESE_None		= 0;	
ESE_Effect		= 1;	--特效
ESE_Sound		= 2;	--声音
ESE_Animation	= 3;	--动作
--enumAffectOption
EAO_None			= 0;
EAO_AffectMeInstead	= 1;
EAO_AffectPlayInstead	= 2;
EAO_FaceToPeer		= 4;
EAO_AttachToWorld	= 8;
--道具的类别，用于使用互斥
ECAT_Accel	= 1;
ECAT_Protect	= 2;

GPConfig=
{
	{
		id=10;
		name="保护罩";
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
			{	--保护罩保护特效
				type=ESE_Effect;
				param=6; 
			};
			{	--保护的音效
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
		name="酷比炸弹"; --水苍蝇
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
			{	--播放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--播放的音效
				type=ESE_Sound;
				param=105;
			};
			{	--飞行的音效
				type=ESE_Sound;
				param=113;
			};
		};
		
		--从听到被攻击的声音到被炸的时间间隔
		triggertime=2500;
		triggereffect=
		{
			{	--击中的音效
				type=ESE_Sound;
				param=113;
			};
		};
		
		affects=
		{
			{	--水苍蝇击中音效
				id=EAI_Sound;
				param=114;
				lifetime=1000;
			};
			{	--地上的水珠
				id=EAI_Effect;
				param=3;
				starttime=0;
				lifetime=2000;
				option=EAO_AttachToWorld;
			};
			{	--包在库比中
				id=EAI_Effect;
				param=41;
				starttime=0;
				lifetime=2000;
				--option=EAO_FaceToPeer;
			};
			{	--抬起
				id=EAI_Lift;
				param=1.25;
				lifetime=2000;
				option=100;	--在这段时间内抬升到param规定的高度
			};
			{	--摇晃
				id=EAI_Animation;
				param=2306;
				starttime=500;
				lifetime=2500;
			};
			--[[
			{	--被糖泡包围
				id=EAI_Effect;
				param=4;
				lifetime=2000;
			};]]
		};
	};	
	{
		id=9;
		name="迷雾";
		usetype=EUT_Active;
		affectype=EAT_Generator;
		
		icontex="textures/fog1.tga";
		newid=1001;	--新生成的id从1000开始
		
		throweffect=
		{
			{	--释放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--释放的音效
				type=ESE_Sound;
				param=104;
			};
			{	--释放的特效
				type=ESE_Effect;
				param=20;
			};
		};
	};
	{
		id=1001;
		name="迷雾";
		usetype=EUT_Passive;
		affectype=EAT_Attack;
		
		staticprop=ESP_Shelter;
		staynode=5;
		staytime=6600;
		
		position=
		{
			x=-0.5; --迷雾在赛道前方的距离
		};
		bound=
		{
			x=0.1; --迷雾包围球的半径
		};			
	};
	
	{
		id=3;
		name="导弹";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		blendmode=EBM_IgnoreNext;
		readytime=1000;
		
		movectrl=
		{
			type=EMT_Missile;
			starttime=1000;
			lifetime=1500; --飞行的时间
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
			{	--释放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--释放音效
				type=ESE_Sound;
				param=110;
			};
			{	--飞行音效
				type=ESE_Sound;
				param=115;
			};
			{	--伸出发射架
				type=ESE_Effect;
				param=42;
			};
		};
		
		triggertime=1500; --参见水苍蝇
		triggereffect=
		{
			{	--导弹跟踪而来的音效
				type=ESE_Sound;
				param=115;
			};
		};
		affects=
		{
			{	--击中爆炸
				id=EAI_Effect;
				param=15;
				lifetime=2000;
				option=EAO_AttachToWorld;
			};
			{	--导弹爆炸的音效
				id=EAI_Sound;
				param=116;
				lifetime=2000;
			};
			{	--显示被击中动作
				id=EAI_Animation;
				param=1314;
				lifetime=2000;
				option=EAO_AffectPlayInstead;
			};
			{	--腾空
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
			{	--转动三圈的动画
				id=EAI_Animation;
				param=2303;	--转动三圈的动画，等待资源
				lifetime=1000;
			};
			{	--监视镜头
				id=EAI_Effect;
				param=29;
				lifetime=2500;
			};
			{	--触发小喷
				id=EAI_No2Emitter;
				param=500;
				starttime=2000;
				lifetime=500;
			};
		};
	};
	{
		id=1;
		name="氮气";
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
			{	--喷气音效
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
		name="大糖泡";
		usetype=EUT_Active;
		affectype=EAT_Generator;
		
		birthtime=0;
		newid=1002;
		
		icontex="textures/waterbomb1.tga";
		
		throweffect=
		{
			{	--释放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--释放特效
				type=ESE_Effect;
				param=22;
			};
			{	--释放音效
				type=ESE_Sound;
				param=105;
			};
		};
	};
	{
		id=1002;
		name="大糖泡";
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
			{	--被水泡困住的音效
				id=EAI_Sound;
				param=112;
				lifetime=2000;
			};
			{	--被风裹住
				id=EAI_Effect;
				param=40;
				lifetime=2000;
			};
			{ 	--被抬起
				id=EAI_Lift;
				param=1;
				lifetime=2000;
				option=1000;
			};
			{	--摇晃
				id=EAI_Animation;
				param=2306;
				starttime=1000;
				lifetime=1000;
			};
			{	--监视镜头
				id=EAI_Effect;
				param=28;
				lifetime=2000;
			};
		};
	};
	{
		id=4;
		name="乌龟";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		targettype=ETT_FirstPlay;
		
		icontex="textures/tortoise1.tga";
		throweffect=
		{
			{	--释放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--释放特效
				type=ESE_Effect;
				param=18;
			};
			{	--释放飞碟的音效
				type=ESE_Sound;
				param=101;
			};
		};
		affects=
		{
			{	--被飞碟击中的音效
				id=EAI_Sound;
				param=102;
				lifetime=3000;
			};
			{	--减速
				id=EAI_Speed;
				param=0.1;
				lifetime=3000;
			};
			{	--3只小乌龟的动画
				id=EAI_Effect;
				param=9;
				starttime=0;
				lifetime=3000;
			};
			{	--监视镜头
				id=EAI_Effect;
				param=25;
				lifetime=3000;
			};
			{	--飞碟结束的音效
				id=EAI_Sound;
				param=103;
				starttime=3000;
				lifetime=1000;
			};
		};
	};
	{
		id=6;
		name="魔王";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		targettype=ETT_Enemy;
		
		throweffect=
		{
			{	--释放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--释放音效
				type=ESE_Sound;
				param=100;
			};
			{	--释放特效
				type=ESE_Effect;
				param=21;
			};
		};
		
		icontex="textures/devil1.tga";
		affects=
		{
			{	--恶魔作用时奸笑声
				id=EAI_Sound;
				param=118;
				lifetime=3000;
			};
			{	--左右键操作相反
				id=EAI_InvertLR;
				lifetime=6000;
			};
			{	--魔王动画
				id=EAI_Effect;
				param=10;
				lifetime=6000;
			};
			{	--身上发光
				id=EAI_Evil;
				lifetime=6000;
			};
		};
	};
	{
		id=5;
		name="磁铁";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		
		--affectself=1;
		targettype=ETT_Play;
		selecttargettype=ESTT_Select;
		
		icontex="textures/magnet1.tga";
		
		throweffect=
		{
			{	--释放音效
				type=ESE_Sound;
				param=109;
			};
		};
		affects=
		{
			{	--磁铁作用的音效
				id=EAI_Sound;
				param=119;
				lifetime=3000;
			};
			{	--磁铁效果
				id=EAI_Magnet;
				lifetime=3000;
			};
			{	--磁铁特效
				id=EAI_Effect;
				param=13;
				lifetime=3000;
				option=EAO_AffectMeInstead+EAO_FaceToPeer;
			};
			--[[{	--磁铁对方身上效果
				id=EAI_Effect;
				param=39;
				lifetime=3000;
			};]]
			{	--磁铁监视镜头
				id=EAI_Effect;
				param=27;
				lifetime=3000;
				option=1;
			};
		};
	};		
	{
		id=8;
		name="香蕉皮";
		usetype=EUT_Active;
		affectype=EAT_Generator;
		
		newid=1003;
		icontex="textures/banana1.tga";
		
		throweffect=
		{
			{	--释放动作
				type=ESE_Animation;
				param=1313;
			};
			{	--释放音效
				type=ESE_Sound;
				param=105;
			};
		};
	};
	{
		id=1003;
		name="香蕉皮";
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
			{	--香蕉皮作用时音效
				id=EAI_Sound;
				param=120;
				lifetime=1000;
			};
			{	--减速
				id=EAI_Speed;
				param=0.1;
				lifetime=200;
			};
			{	--显示被击中动作
				id=EAI_Animation;
				param=1314;
				lifetime=2000;
				option=EAO_AffectPlayInstead;
			};
			{	--打滑
				id=EAI_Animation;
				param=2304; --车子打滑的动画，等待资源中
				lifetime=2500;
			};
			{	--锁定操作
				id=EAI_FreezeOp;
				lifetime=2500;
			};
			{	--监视镜头
				id=EAI_Effect;
				param=26;
				lifetime=2500;
			};
		};
	};
	
	{
		id=2001;
		name="小喷";
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
			{	--喷气音效
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
		name="组队氮气";
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
			{	--喷气音效
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
		name="蘑菇";
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

	--组队道具
	{
		id=11;
		name="自爆";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		targettype=ETT_Me;
		
		icontex="textures/waterbomb1.tga";
		
		throweffect=
		{
			{	--释放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--释放音效
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
		name="自爆";
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
			{	--被水泡困住的音效
				id=EAI_Sound;
				param=112;
				lifetime=2000;
			};
			{	--被糖泡包围住
				id=EAI_Effect;
				param=4;
				lifetime=2000;
			};
			{ 	--被抬起
				id=EAI_Lift;
				param=1;
				lifetime=2000;
				option=1000;
			};
			{	--摇晃
				id=EAI_Animation;
				param=2306;
				starttime=1000;
				lifetime=1000;
			};
			{	--监视镜头
				id=EAI_Effect;
				param=28;
				lifetime=2000;
			};
		};
	};
	{
		id=12;
		name="团队无敌";
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
			{	--保护罩保护特效
				type=ESE_Effect;
				param=6; 
			};
			{	--保护的音效
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
		name="电磁波";
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
			{	--保护罩保护特效
				type=ESE_Effect;
				param=6; 
			};
			{	--保护的音效
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
		name="导弹-香蕉";
		usetype=EUT_Active;
		affectype=EAT_Attack;
		blendmode=EBM_IgnoreNext;
		
		movectrl=
		{
			type=EMT_Missile;
			starttime=1000;
			lifetime=1500; --飞行的时间
			movenode=14;
		};
		
		targettype=ETT_Play;
		selecttargettype=ESTT_Select;
		
		icontex="textures/missile1.tga";
		
		throweffect=
		{
			{	--释放动作
				type=ESE_Animation;
				param=1310;
			};
			{	--释放音效
				type=ESE_Sound;
				param=110;
			};
			{	--飞行音效
				type=ESE_Sound;
				param=115;
			};
			{	--伸出发射架
				type=ESE_Effect;
				param=42;
			};
		};

		triggertime=1500; --参见水苍蝇
		triggereffect=
		{
			{	--导弹跟踪而来的音效
				type=ESE_Sound;
				param=115;
			};
		};
		affects=
		{
			{	--香蕉皮作用时音效
				id=EAI_Sound;
				param=120;
				lifetime=2000;
			};
			{	--减速
				id=EAI_Speed;
				param=0.1;
				lifetime=200;
			};
			{	--显示被击中动作
				id=EAI_Animation;
				param=1314;
				lifetime=2000;
				option=EAO_AffectPlayInstead;
			};
			{	--打滑
				id=EAI_Animation;
				param=2304; --车子打滑的动画，等待资源中
				lifetime=2500;
			};
			{	--锁定操作
				id=EAI_FreezeOp;
				lifetime=2500;
			};
			{	--监视镜头
				id=EAI_Effect;
				param=26;
				lifetime=2500;
			};
		};
	};
};