--------------------------------------------------------------------------------------
-- effcttype  ��ʾЧ������;
-- condition   ���������� 1<<0  �ڿ��У� 1<<1 �����Ӵ���1<<2...
--param0, ����0
--param1, ����1
--param2, ����2
--------------------------------------------------------------------------------------
 -- ��Ч��
-- effcttype = 0;
--param0, no use
--param1, no use
--param2, no use
--------------------------------------------------------------------------------------
-- ������Ч
--effecttype = 1
	--param0, ��Чλ��
		--param0 = 0, ������Ч
		--param1, ��Ч��ַ���ַ���
		--param2, ѭ�����ͣ� 0�������β��ţ�  1����ѭ��
	
		--param0 = 1, ��ǰ��Ч
		--param1, ��Ч��ַ���ַ���
		--param2, ѭ�����ͣ� 0�������β��ţ�  1����ѭ��
--------------------------------------------------------------------------------------
-- ����������Ч
-- effecttype = 2
--param0, ��Ч��ַ
--param1, �����������
--param2, no use

--------------------------------------------------------------------------------------
-- ��������
--effecttype = 3
--param0, ��Ч��ַ�� �ַ���
--param1, ѭ�����ͣ�0�������β��ţ�  1����ѭ��
--param2, no use

-- ����
--effecttype = 4;
--param0, force
--param1, force time, 0 ��ʾ���Σ� ������ʾʱ��
--param2, no use
--

-- ����
--effecttype = 5;
--param0, +xSpeed
--param1, +ySpeed
--param2, +zSpeed

-- ����
--effecttype = 6;
--param0, ����Ŀ�Ľڵ�����
--param1, ��Ч�ڵ�����
--param2, ���������ַ

--condition
cond_inair 		= 1;
cond_contact_terrain =  2;


TerrainProp = 
{
	{
		proptype = 0,--��·������
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
		proptype = 1,--ɳĮ·��
		discript   = "����ɳ������Ч�������л�ɳ�����ĸо�",
		color       = {1.0, 1.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript    = "����ɳ������Ч�������л�ɳ�����ĸо�",
				param0    = 0;
				param1    = "effect/mapeffect/yellowsand.nif",
				param2    = 1;
			},
			{
				effecttype = 4;
				condition = cond_contact_terrain;
				discript   = "�����ٵĶ�̬ģ��",
				param0    = 5.0;
				param1    = 0,
				param2    = 0;
			},
		};
	},
	{
		proptype = 2,--ʯ��·��
		discript   = "ʯ�ӱ��ѵ���Ч��ʯ�ӹ���",
		color       = {128.0/255.0, 128.0/255.0, 128.0/255.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript    = "ʯ�ӱ��ѵ���Ч��ʯ�ӹ���",
				param0    = 0;
				param1    = "effect/mapeffect/stone.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 2,--ľ��·��
		discript   = "ľ�Ų�������Ч",
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
		proptype = 3,--ˮ��
		discript   = "����ˮ������Ч��ˮ��",
		color       = {0.0, 204.0/255.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "����ˮ������Ч��ˮ��";
				param0    = 0;
				param1    = "effect/mapeffect/water.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 4,--�ݵ�
		discript   = "�����Ҷ����Ч����ݵ�������Ч",
		color       = {0.0, 1.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "�����Ҷ����Ч����ݵ�������Ч",
				param0    = 0;
				param1    = "effect/mapeffect/grass.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 5,--ѩ��
		discript   = "ѩ���Ľ�����Ч",
		color       = {1.0, 1.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 4;
				condition = cond_contact_terrain;
				discript   = "ѩ���Ľ�����Ч",
				param0    = 0;
				param1    = "effect/mapeffect/snow.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 6,--ɽ·����·��
		discript   = "�������������Ч",
		color       = {128.0/255.0, 128.0/255.0, 128.0/255.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "�������������Ч",
				param0    = 0;
				param1    = "effect/mapeffect/yellowdust.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 7,--���ٴ�
		discript   = "�����ٵĶ�̬ģ��",
		color       = {204.0/255.0, 153.0/255.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 4;
				condition = cond_contact_terrain;
				discript   = "�����ٵĶ�̬ģ��",
				param0    = 3.0;
				param1    = 0,
				param2    = 0;
			},
		};
	},
	{
		proptype = 8,--����
		discript   = "���汻ѹ�ѵ���Ч�������Ľ�����Ч",
		color       = {0.0, 1.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "���汻ѹ�ѵ���Ч�������Ľ�����Ч",
				param0    = 0;
				param1    = "effect/mapeffect/ice.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 9,--������̶��
		discript   = "�ཬ�ɽ�����Ч",
		color       = {0.0, 0.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain;
				discript   = "�ཬ�ɽ�����Ч",
				param0    = 0;
				param1    = "effect/mapeffect/mire.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 10,--����·��
		discript   = "�����Ĺ��߷�����Ч",
		color       = {0.0, 128.0/255.0, 128.0/255.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain + cond_inair;
				discript   = "�����Ĺ��߷�����Ч",
				param0    = 0;
				param1    = "effect/mapeffect/reflect.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 11,--שͷ·��
		discript   = "nothing",
		color       = {1.0, 153.0/255.0, 204.0/255.0, 1.0,},
		effect = 
		{
		};
	},
	{
		proptype = 12,--����·��
		discript   = "���������������Ч��Զ�������ζ�����Ч��",
		color       = {1.0, 0.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 1;
				condition = cond_contact_terrain + cond_inair;
				discript   = "���������������Ч��Զ�������ζ�����Ч��",
				param0    = 0;
				param1    = "effect/mapeffect/glass.nif",
				param2    = 1;
			},
		};
	},
	{
		proptype = 13,--������
		discript   = "����",
		color       = {153.0/255.0, 204.0/255.0, 0.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 5;
				condition = cond_contact_terrain + cond_inair; 
				discript   = "����",
				param0    = 1.0;
				param1    = 0.0,
				param2    = 10.0;
			},
		};
	},
	{
		proptype = 14,--���͵�
		discript   = "��ͬ���ͷ�ʽ����Ч",
		color       = {1.0, 0.0, 1.0, 1.0,},
		effect     = 
		{
			{
				effecttype = 6;
				condition = cond_contact_terrain + cond_inair;
				discript   = "��ͬ���ͷ�ʽ����Ч",
				param0    =  "A";
				param1    =  "transformanim",
				param2    =  "transformcamera";
			},
		};
	},
	{
		proptype = 15,-- б���ϵļ��ٴ�
		discript   = "��б���ϵĳ�������",
		color       = {0.2, 0.8, 0.8, 1.0,},
		effect     = 
		{
			{
				effecttype = 7;
				condition = cond_contact_terrain + cond_inair;
				discript   = "����",
				param0    =  0.5;
				param1    =  0.0,
				param2    =  0.0;
			},
		};
	},
};