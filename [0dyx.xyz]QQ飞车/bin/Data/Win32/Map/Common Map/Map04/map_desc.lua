map_desc = 
{
	map_name = "ÖÐ¹ú³Ç",
	map_hardness = 1,
	map_checkpoint = "checkpoint.nif",
	map_camplayback = "CamPlayBack.nif",
	map_eaglemap = "map.nif",
	map_pickmap = "pick.nif",
	map_scene    = "sc.nif",
	map_icon_small = "",
	map_icon_big = "",
	awardname = "Map/jiangtai_zgc.nif",
	teamawardname = "Map/jt_team_zgc.nif",
	ambientsoundname = "Data/Win32/SOUNDS/ChinaCity.mp3",
	usingAlphaSorterToSky = 1,
 	awardtransform=
	{
		pos =
		{
			0.0, 0.0, 0.0,
		};
		rotate =
		{
			0.0, 0.0, 0.0,
		};
	};
	
	awardcamtran=
	{
		pos=
		{
			-138.549, 7.829, -1.609,
		};
		rotate=
		{
			96.756, -0.972, 6.493,
		};
	};

	
	map_has_mirror = 0; -- if has no reflection, the value should be 0
	mirrors = 
	{
		{
			mirror_name = "mirror",
			node_name = "glass",
			trishape_name = "glass:2",--"road";--
			
			centpoint = 
			{
				111.44727 ,105.90320, -7.1070347,
			};
			
			right_dir = 
			{
				0, 1, 0,
			};
			
			up_dir = 
			{
				0, 0, 1,
			};
		};		
	};
	
	map_static_colliders = 
	{		
	},
	map_anim_colliders = 
	{
	},
	map_active_colliders = 
	{	
	},
	
	camera = 
	{
		intro_camera = "cam2.nif",		
	},
	
	propbox=37;
	
	disp_strategy = 
	{
		allvisibledist     = 100.0,
		minvisibleangle = 0.1,
		maxprocessradius = 0.5,
	},
	physx_susPos=
	{
		pos=
		{
			100000.0, 100000.0, 0.0,
		};
	};
}


