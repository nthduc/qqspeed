--------------------------------------------------------------------------------------
-- effcttype  表示效果类型;
-- condition   触发条件， 1<<0  在空中， 1<<1 与地面接触，1<<2...
--param0, 参数0
--param1, 参数1
--param2, 参数2
--------------------------------------------------------------------------------------
 -- 无效果
-- effcttype = 0;
--param0, no use
--param1, no use
--param2, no use
--------------------------------------------------------------------------------------
-- 播放特效
--effecttype = 1
	--param0, 特效位置
		--param0 = 0, 车后特效
		--param1, 特效地址，字符串
		--param2, 循环类型， 0——单次播放，  1——循环
	
		--param0 = 1, 车前特效
		--param1, 特效地址，字符串
		--param2, 循环类型， 0——单次播放，  1——循环
--------------------------------------------------------------------------------------
-- 其他对象特效
-- effecttype = 2
--param0, 特效地址
--param1, 动画相机名称
--param2, no use

--------------------------------------------------------------------------------------
-- 播放声音
--effecttype = 3
--param0, 音效地址， 字符串
--param1, 循环类型，0——单次播放，  1——循环
--param2, no use

-- 加速
--effecttype = 4;
--param0, force
--param1, force time, 0 表示单次， 其他表示时间
--param2, no use
--

-- 弹跳
--effecttype = 5;
--param0, +xSpeed
--param1, +ySpeed
--param2, +zSpeed

-- 传送
--effecttype = 6;
--param0, 传送目的节点名称
--param1, 特效节点名称
--param2, 动画相机地址

--condition
cond_inair 		= 1;
cond_contact_terrain =  2;


TerrainProp = 
{
	{
		proptype = 0,--公路（城镇）
		discript   = "nothing",
		color 	     = {204.0/255, 153.0/255,  1.0, 1.0,},	
		effect     = 
		{
			{
				effecttype = 5;
				condition = cond_contact_terrain;
				discript   = "asdfsasdfas";
				param0    = 0;
				param1    = "effect/mapeffect/water.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 1,--沙漠路面
		discript   = "扬起沙尘的特效及场景中黄沙漫漫的感觉",
		color       = {1.0, 1.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript    = "扬起沙尘的特效及场景中黄沙漫漫的感觉",
				param0    = 0;
				param1    = "effect/mapeffect/yellowsand.nif",
				param2    = 1;
			},
			{
				effecttype = 4;
				condition = cond_contact_terrain;
				discript   = "被加速的动态模糊",
				param0    = 5.0;
				param1    = 0,
				param2    = 0;
			},
		};
	},
	{
		proptype = 2,--石子路面
		discript   = "石子崩裂的特效及石子滚动",
		color       = {128.0/255.0, 128.0/255.0, 128.0/255.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript    = "石子崩裂的特效及石子滚动",
				param0    = 0;
				param1    = "effect/mapeffect/stone.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 2,--木桥路面
		discript   = "木桥颤动的特效",
		color       = {153.0/255.0, 51.0/255.0, 0.0, 1.0,},
		effect     = 
		{
			--[[{
				effecttype = 2;
				param0    = 0;
				param1    = "effect/mapeffect/huangshao.nif",
				param2    = 1;
			},]]
		};
	},
	{
		proptype = 3,--水面
		discript   = "划出水花的特效及水面",
		color       = {0.0, 204.0/255.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "划出水花的特效及水面";
				param0    = 0;
				param1    = "effect/mapeffect/water.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 4,--草地
		discript   = "卷起草叶的特效或深草倒伏的特效",
		color       = {0.0, 1.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "卷起草叶的特效或深草倒伏的特效",
				param0    = 0;
				param1    = "effect/mapeffect/grass.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 5,--雪地
		discript   = "雪花四溅的特效",
		color       = {1.0, 1.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 4;
				condition = cond_contact_terrain;
				discript   = "雪花四溅的特效",
				param0    = 0;
				param1    = "effect/mapeffect/snow.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 6,--山路（土路）
		discript   = "黄土被卷起的特效",
		color       = {128.0/255.0, 128.0/255.0, 128.0/255.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "黄土被卷起的特效",
				param0    = 0;
				param1    = "effect/mapeffect/yellowdust.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 7,--加速带
		discript   = "被加速的动态模糊",
		color       = {204.0/255.0, 153.0/255.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 4;
				condition = cond_contact_terrain;
				discript   = "被加速的动态模糊",
				param0    = 3.0;
				param1    = 0,
				param2    = 0;
			},
		};
	},
	{
		proptype = 8,--冰面
		discript   = "冰面被压裂的特效，飞渣四溅的特效",
		color       = {0.0, 1.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "冰面被压裂的特效，飞渣四溅的特效",
				param0    = 0;
				param1    = "effect/mapeffect/ice.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 9,--沼泽（泥潭）
		discript   = "泥浆飞溅的特效",
		color       = {0.0, 0.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "泥浆飞溅的特效",
				param0    = 0;
				param1    = "effect/mapeffect/mire.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 10,--金属路面
		discript   = "金属的光线反射特效",
		color       = {0.0, 128.0/255.0, 128.0/255.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain + cond_inair;
				discript   = "金属的光线反射特效",
				param0    = 0;
				param1    = "effect/mapeffect/reflect.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 11,--砖头路面
		discript   = "nothing",
		color       = {1.0, 153.0/255.0, 204.0/255.0, 1.0,},
		effect = 
		{
		};
	},
	{
		proptype = 12,--玻璃路面
		discript   = "玻璃光线折射的特效及远处玻璃晃动的特效。",
		color       = {1.0, 0.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain + cond_inair;
				discript   = "玻璃光线折射的特效及远处玻璃晃动的特效。",
				param0    = 0;
				param1    = "effect/mapeffect/glass.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 13,--弹跳点
		discript   = "弹跳",
		color       = {153.0/255.0, 204.0/255.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 5;
				condition = cond_contact_terrain + cond_inair; 
				discript   = "弹跳",
				param0    = 1.0;
				param1    = 0.0,
				param2    = 10.0;
			},
		};
	},
	{
		proptype = 14,--传送点
		discript   = "不同传送方式的特效",
		color       = {1.0, 0.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 6;
				condition = cond_contact_terrain + cond_inair;
				discript   = "不同传送方式的特效",
				param0    =  "A";
				param1    =  "transformanim",
				param2    =  "transformcamera";
			},
		};
	},
	{
		proptype = 15,-- 斜面上的加速带
		discript   = "给斜面上的车辆加速",
		color       = {0.2, 0.8, 0.8, 1.0,},
		effect     = 
		{
			{
				effecttype = 7;
				condition = cond_contact_terrain + cond_inair;
				discript   = "加速",
				param0    =  0.5;
				param1    =  0.0,
				param2    =  0.0;
			},
		};
	},
};