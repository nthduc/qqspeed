map_desc = 
{
	map_name = "¸çÌØ",
	map_hardness = 1,
	map_checkpoint = "checkpoint.nif",
	map_camplayback = "CamPlayBack.nif",
	map_eaglemap = "map.nif",
	map_pickmap = "pick.nif",
	map_scene    = "sc.nif",
	map_icon_small = "",
	map_icon_big = "",
	awardname = "Map/jiangtai_gete.nif",
	teamawardname = "Map/jt_team_gete.nif",
	ambientsoundname = "Data/Win32/SOUNDS/sleepingforest.mp3",
	
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
			-76.539, 34.551, 0.074,
		};
		rotate=
		{
			97.317, 0.116, -77.367,
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
	
	propbox=0;
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


