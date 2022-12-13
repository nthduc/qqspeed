map_desc = 
{
	map_name = "新手教学",
	map_hardness = 1,
	map_checkpoint = "checkpoint.nif",
	map_camplayback = "CamPlayBack.nif",
	map_eaglemap = "map.nif",
	map_pickmap = "pick.nif",
	map_scene    = "sc.nif",
	map_icon_small = "",
	map_icon_big = "",
	awardname = "Map/jiangtai.nif",
	--teamawardname = "Map/jt_team.nif",
	enablecheckpoint = 0,
	
	awardtransform=
	{
		pos = 
		{
			85.989,57.069,-19.23,
		};
		rotate =
		{
			0.0, 0.0, -180.0,
		};
	};
	
	awardcamtran=
	{
		pos=
		{
			0, 0, 0,
		};
		rotate=
		{
			0, 0, 0,
		};
	};
	
	map_has_mirror = 0; -- if has no reflection, the value should be 0
	
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
			60.9, 9.40, 0.0,
		};
	};
}


