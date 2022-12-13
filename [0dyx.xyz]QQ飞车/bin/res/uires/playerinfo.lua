local DD_ACCEL		= 99998;
local DD_JIQI_ID		= 99999;
local DD_NORMAL_NO2= 1;
local DD_XIAOPEN	= 2001;
local DD_TEAM_NO2	= 2002;

local SpeedType 		= 1;
local ExplosionType 	= 2;
local GatherType		= 3;
local SpoutType		= 4;
local MiniSpoutType	= 5;

pointassignguiddesc =
{
	"说明：\n你把你的注意力放在任何一项目能力上,注重平均发展",
	"说明：\n你并不在意转弯的技巧,因为你觉得只要在直道上有优势就能胜券在握.",
	"说明：\n你很享受N20爆发时的加速快感,并把大部分的注意力集中在这方面.",
	"说明：\n你对你的漂移技巧很有信心,并将大部分的注意力放在这方面.",
};

SetPointAssignScheme = function(schemeindex)
	Win_SetText("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg.desc_label", pointassignguiddesc[schemeindex+1]);
	--UI.children.playerinfo_dlg.children.playerinfo_content.children.prop_dlg.children.autoasign_dlg._schemeindex = schemeindex;
	if (prop.strategyprop >= 128) then
		if ((prop.strategyprop - 128) ~= schemeindex) then
			prop.strategyprop = 128 + schemeindex;
			prop.hasChange = true;
		end;
	else
		if ((prop.strategyprop) ~= schemeindex) then
			prop.strategyprop = schemeindex;
			prop.hasChange = true;
		end;
	end;
	
end;


prop=
{
	[1] = {name = "速度", value = 255},
	[2] = {name = "爆发", value = 255},
	[3] = {name = "集气", value = 100},
	[4] = {name = "喷射", value = 100},
	[5] = {name = "小喷", value = 100},	
	maxpoints = 300,
	restpoints = 255,
	strategyprop = 0,
	laststrategy   = 0,
	hasChange = false,
};

function IsUsingStratgy(strategy)
	if (strategy >= 128) then
		return true;
	end;
	return false;
end;

PhysAttribParam = 
{
	dwPID = 0;	
	fParam = 0;
	fLifeTime = 0;
};


function CaculateAndSetKartAttrib( type, value)	 
	if (value == 0 ) then
		return;
	end;
	PhysAttribParam.fParam = 0;
	PhysAttribParam.fLifeTime = 0;
	if (type == SpeedType) then
		PhysAttribParam.dwPID = DD_ACCEL;
		PhysAttribParam.fLifeTime = 0;
		PhysAttribParam.fParam = value * 0.001 + 1.29;
	elseif (type == ExplosionType) then
		PhysAttribParam.dwPID = DD_NORMAL_NO2;
		PhysAttribParam.fParam = value * 0.05 + 6.0;
		PhysAttribParam.fLifeTime = 4000;
		SetKartAttrib(PhysAttribParam.dwPID, PhysAttribParam.fParam, PhysAttribParam.fLifeTime);
		PhysAttribParam.dwPID = DD_TEAM_NO2;
		PhysAttribParam.fParam = value * 0.05 + 6.5;
		PhysAttribParam.fLifeTime = 5000;
		SetKartAttrib(PhysAttribParam.dwPID, PhysAttribParam.fParam, PhysAttribParam.fLifeTime);
		return;		
	elseif (type == GatherType) then
		PhysAttribParam.dwPID = DD_JIQI_ID;
		PhysAttribParam.fParam = value * 0.3 + 77;
	elseif (type == SpoutType) then
		PhysAttribParam.dwPID = DD_NORMAL_NO2;
		PhysAttribParam.fParam = 6.0;
		PhysAttribParam.fLifeTime = value * 20 +3800.0;
		SetKartAttrib(PhysAttribParam.dwPID, PhysAttribParam.fParam, PhysAttribParam.fLifeTime);
		PhysAttribParam.dwPID = DD_TEAM_NO2;
		PhysAttribParam.fParam = 7.0;
		PhysAttribParam.fLifeTime = value * 20 + 4800.0;
		SetKartAttrib(PhysAttribParam.dwPID, PhysAttribParam.fParam, PhysAttribParam.fLifeTime);
		return;		
	elseif (type == MiniSpoutType) then
		PhysAttribParam.dwPID = DD_XIAOPEN;
		PhysAttribParam.fParam = value * 0.02 + 5.8;
		PhysAttribParam.fLifeTime = value * 2 + 780.0;
	else
		Win_ConsoleInfo("Error, No Such Attrib Type!!!");
		return;
	end;
	
	SetKartAttrib(PhysAttribParam.dwPID, PhysAttribParam.fParam, PhysAttribParam.fLifeTime);
end;

function AddPoints(choice)
	if (prop.restpoints <= 0 or prop[choice+1].value >= prop.maxpoints) then
		return;
	end;
	prop.hasChange = true;
	local fatherpath = "UI.playerinfo_dlg.playerinfo_content.prop_dlg";
	local path = fatherpath..".playerprop"..choice;
	local uin = UI.children.playerinfo_dlg._uin;
	--if () then
	local progresspath = path..".prop_progressbar";
	
	
	Win_ConsoleInfo(prop[choice+1].name);
	Win_ConsoleInfo(progresspath);
	
	prop.restpoints = prop.restpoints - 1;
	prop[choice+1].value = prop[choice+1].value + 1;
	
	--text = prop[choice+1].name..": "..prop[choice+1].value.."/"..prop.maxpoints;
	
	if(choice==1) then text = "增加车辆的最大速度"; end;
	if(choice==2) then text = "增加氮气喷射的威力"; end;
	if(choice==3) then text = "增加氮气收集的速度"; end;
	if(choice==4) then text = "增加氮气喷射的时间"; end;
	if(choice==5) then text = "增加小喷的时间";	 end;
	
	Win_SetText(progresspath, text, value_channel_tiptext);	
	Win_SetValue(progresspath, prop.maxpoints, value_channel_maxvalue);
	Win_SetValue(progresspath, prop[choice+1].value,value_channel_pos);	
	Win_SetText(path..".cur_proppoint", prop[choice+1].value);
	Win_SetText(fatherpath..".restpoints_label", ""..prop.restpoints);
	if Win_IsVisible("UI.SysTooltip") then
		wgtpath=Win_GetText("UI.SysTooltip",value_channel_host);
		if (wgtpath==progresspath) then
			Win_SetText("UI.SysTooltip",text);
		end;
	end;		
	CaculateAndSetKartAttrib(choice + 1, prop[choice+1].value);
	--end;
end;

function SendPointsToSever()
	if (prop.hasChange) then		
		RequestAssignAttribute(prop[1].value,prop[2].value,prop[3].value,prop[4].value,prop[5].value, prop.restpoints,prop.strategyprop);
	end;
	prop.hasChange = false;
end;

function ResetPoints(playeruin)
	UI.children.playerinfo_dlg.OnFillPlayerInfo(playeruin);
end;
UI.children.playerinfo_dlg = 
{
	type="DIALOG",
	rect = {55, 204, 258, 285},
	--caption = "玩家信息",
	--captionrect = {60, 3, 80, 16},
	--drawcolor = {255, 0, 0, 255},
	textsize=16,
	textstyle = dt_center + dt_vcenter,
	visible = 0,
	moveable = 1,
	initlayer=msgbox_layer-1,
	bkimage = "",
	drawflag=drawflag_win_edge+drawflag_win_fill,
	align_style = alignstyle_hcenter + alignstyle_vcenter;
	
	_uin = 0;
	
	OnInit = function()
		--Win_ConsoleInfo("player_infodlg init");
		--local path = "UI.playerinfo_dlg.playerinfo_content.state_dlg";
		--Win_SetCheck(path, true);
	end;
	
	OnDeinit = function()
		Win_ConsoleInfo("player_infodlg deinit");
	end;
	
	OnSelfShow = function()	
		Win_SetCapture("UI.playerinfo_dlg");
		Win_SetFocus("UI.playerinfo_dlg");
		Win_ConsoleInfo("player_selfshow");
	end;
	
	OnSelfHide = function()
		Win_ConsoleInfo("player_selfhide");
		
	end;

	OnFillPlayerInfo = function(uin)
		UI.children.playerinfo_dlg._uin = uin;
		Win_ConsoleInfo("OnFillPlayerInfo("..uin..")");
		if (IsSelf(uin)) then
			Win_ConsoleInfo("Self");
			local myinfo = GetMyInfo();
			local myattribute = GetMyAttribute();
			if (myinfo and myattribute) then
				local path = "UI.playerinfo_dlg.playerinfo_content";
				--fill player state
				local statepath = path..".state_dlg";
				Win_SetText(path..".name_label", myinfo.playerdbinfo.nickname);
				Win_SetText(statepath..".qq_label", myinfo.uin);
				local level, subexp, levelexp = GetPlayerLevelByExp(myinfo.playerdbinfo.exp);
				
				--升级经验条
				local progresspath = statepath..".experience_label";
				text = ""..subexp.."/"..levelexp;
				Win_SetText(progresspath, text);
				Win_SetText(progresspath, text, value_channel_tiptext);	
				Win_SetValue(progresspath, levelexp, value_channel_maxvalue);
				Win_SetValue(progresspath, subexp,value_channel_pos);	
					
				local scheme = math.floor(levelexp*8/subexp);
				Win_SetValue(progresspath, scheme, value_channel_framescheme);	
				if Win_IsVisible("UI.SysTooltip") then
					wgtpath=Win_GetText("UI.SysTooltip",value_channel_host);
					if (wgtpath==progresspath) then
						Win_SetText("UI.SysTooltip",text);
					end;
				end;	
				
				Win_SetText(statepath..".degree_label", "lv"..level);
				--Win_SetText(statepath..".experience_label", myinfo.playerdbinfo.exp);
				Win_SetText(statepath..".totalmathes_label", myinfo.playerdbinfo.totnum);
				Win_SetText(statepath..".championnums_label",myinfo.playerdbinfo.winnum);
				Win_SetText(statepath..".honor_label",myinfo.playerdbinfo.honor);
				Win_ConsoleInfo("Fill");
				--fill player attrib
				prop[1].value = myattribute.speedprop;
				prop[2].value = myattribute.explosionprop;
				prop[3].value = myattribute.gatherprop;
				prop[4].value = myattribute.spoutprop;
				prop[5].value = myattribute.minispotprop;
				prop.restpoints = myattribute.reserveprop;
				prop.strategyprop = myattribute.strategyprop;	
				prop.laststrategy = prop.strategyprop;
				UI.children.playerinfo_dlg.children.playerinfo_content.children.prop_dlg.FillPropInfo();
				for i = 1,5 do
					CaculateAndSetKartAttrib(i, prop[i].value);
				end
				--fill player team
				local teampath = path..".team_dlg";
				prop.hasChange = false;
			end;
		else
		end;
	end;
	
	children= 
	{
		closebtn=
		{
			type  = "BUTTON",
			rect   = {233, 3, 25, 25},
			bkimage="res/uires/room/closebtn.gsa",
			--align_style = alignstyle_top_in + alignstyle_right_in;
			
			OnClick = function()
				Win_ShowWidget("UI.playerinfo_dlg", false);
			end;
		};
		tabbar = 
		{
			type = "DIALOG",
			rect  = {257, 24, 20, 230},
			
			OnInit = function()
				Win_SelectSelf("UI.playerinfo_dlg.tabbar.playerstate");
			end;
			
			children = 
			{
				playerprop= 
				{
					type = "TABWIN",
					groupid = 9,
					groupstop = 1,
					--caption = "属性", 
					align_style = alignstyle_right_in,
					textstyle = dt_magicback + dt_center + dt_vcenter;
					rect = {0, 67, 27,  55},
					tabcover="res/uires/playerinfo/prop_tabcover.gsa",
					tabrect={6,10,20,  20},
					
					OnCheck=function(bCheck)
						local path = "UI.playerinfo_dlg.playerinfo_content.prop_dlg";
						if bCheck == 1 then
							Win_ConsoleInfo("Select 状态");							
							Win_SetBackPic("UI.playerinfo_dlg", "res/uires/playerinfo/prop_bk.gsa");
							Win_ShowWidget(path, true);
						else
							Win_ShowWidget(path, false);
						end;
					end;
				},
				playerstate= 
				{
					type = "TABWIN",
					groupid = 9,
					groupstop = 0,
					--caption = "状态",
					align_style = alignstyle_right_in,
					textstyle = dt_magicback + dt_center + dt_vcenter;
					rect = {0, 2, 27,  55},
					tabcover="res/uires/playerinfo/state_tabcover.gsa",
					tabrect={6,10,20,  20},
					
					OnCheck=function(bCheck)
						local path = "UI.playerinfo_dlg.playerinfo_content.state_dlg";
						if bCheck == 1 then
							Win_ConsoleInfo("Select 状态");
							Win_ShowWidget(path, true);
							Win_SetBackPic("UI.playerinfo_dlg", "res/uires/playerinfo/state_bk.gsa");
						else
							Win_ShowWidget(path, false);
						end;
					end;
				},
				playerteam= 
				{
					type = "TABWIN",
					groupid = 9,
					groupstop = 2,
					--caption = "车队",
					align_style = alignstyle_right_in,
					textstyle = dt_magicback + dt_center + dt_vcenter;
					rect = {37, 151, 35, 68},
					tabcover="res/uires/playerinfo/team_tabcover.gsa",
					tabrect={0,0,35, 68},
					visible = 0,
					enable = 0,
					
					OnCheck=function(bCheck)
						local path = "UI.playerinfo_dlg.playerinfo_content.team_dlg";
						if bCheck == 1 then
							Win_ConsoleInfo("Select 状态");
							Win_ShowWidget(path, true);
						else
							Win_ShowWidget(path, false);
						end;
					end;
				},			
			},
		},
		
		playerinfo_content = 
		{
			type = "DIALOG",
			rect  = {0, 25, 250, 250},
			
			children = 
			{				
				name_label = 
				{
					type = "LABEL",
					caption = "我是打老虎",
					rect = {20, 20, 210, 16},
					textstyle = dt_magicback + dt_left + dt_vcenter,
					--drawcolor = {0, 0, 0, 255},
				},
				state_dlg = 
				{					
					type = "DIALOG",
					rect  = {15, 51, 240, 200},
					visible = 1,
					textstyle = dt_left + dt_vcenter,
					drawcolor = {0, 0, 0, 255},
					
					children = 
					{						
						qq_label = 
						{
							type = "LABEL",
							caption = "QQ: 1234567890",
							rect = {65, 4, 130, 16},		
							textstyle = dt_magicback + dt_center,--dt_left + dt_vcenter,								
							--drawcolor = {0, 0, 0, 255},
						},
						degree_label=
						{
							type = "LABEL",
							caption = "等级: lv20",
							rect = {65, 35, 130, 16},	
							textstyle = dt_magicback + dt_center,--dt_left + dt_vcenter,								
							--drawcolor = {0, 0, 0, 255},
						},
						experience_label=
						{
							--type = "LABEL",
							--caption = "经验:   60%",
							type="PROGRESSBAR",
							rect = {55, 65, 150, 16},
							captionrect = {0, 0, 150, 16},
							--bkimage="res/uires/playerinfo/prop_progress_bk.gsa",
							progresslayer="res/uires/playerinfo/expprogress.gsa",
							--progressaimer="res/uires/gameui/status/aimer.gsa",
							delaytime=0.05,
							slowtime=0.0,
							step=100,
							textsize=12,
							textfont = 1,
							style=wgtstyle_clipself;
							--rect = {67, 70, 130, 16},
							textstyle = dt_magicback + dt_center,--dt_left + dt_vcenter,								
							--drawcolor = {0, 0, 0, 255},
						},
						totalmathes_label=
						{
							type = "LABEL",
							caption = "参赛场次: 13000",
							rect = {65, 95, 130, 16},								
							textstyle = dt_magicback + dt_center,--dt_left + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
						championnums_label=
						{
							type = "LABEL",
							caption = "冠军场次: 8888",
							rect = {65, 125, 130, 16},
							textstyle = dt_magicback + dt_center,--dt_left + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
						honor_label=
						{
							type = "LABEL",
							caption = "荣誉: 12429402",
							rect = {65, 155, 130, 16},								
							textstyle = dt_magicback + dt_center,--dt_left + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
					},
				},
				
				prop_dlg = 
				{
					type = "DIALOG",
					rect  = {0, 10, 240, 200},
					visible = 0,
					template = {"res/uires/playerproptempl.lua"},
					
					OnInit = function()
						--UI.children.playerinfo_dlg.children.playerinfo_content.children.prop_dlg.FillPropInfo();
						local path = "UI.playerinfo_dlg.playerinfo_content.prop_dlg";
						Win_ConsoleInfo(path.." AddTemplate_front");
						Win_ClearTemplates(path);
						Win_AddTemplate(path,0,0,4);
						Win_ConsoleInfo("AddTemplate_after");
					end;
					
					FillPropInfo = function()
						local path = "UI.playerinfo_dlg.playerinfo_content.prop_dlg";
						
						--[[Win_ConsoleInfo(path.." AddTemplate_front");
						Win_ClearTemplates(path);
						Win_AddTemplate(path,0,0,4);
						Win_ConsoleInfo("AddTemplate_after");]]
						local schemeindex = math.mod(prop.strategyprop, 4);
						if (prop.strategyprop >= 128) then
							Win_SetCheck(path..".autoasign_check", true);																
						else
							Win_SetCheck(path..".autoasign_check", false);
						end;
						local schemepath = path..".autoasign_dlg.checkgroup.propassignchoice"..schemeindex;
						--[[{ 
							[0] = path..".autoasign_dlg.checkgroup.average_check",
							[1] = path..".autoasign_dlg.checkgroup.speed_check",
							[2] = path..".autoasign_dlg.checkgroup.explosion_check",
							[3] = path..".autoasign_dlg.checkgroup.drift_check",
						};]]
						Win_ConsoleInfo(schemepath);
						Win_SelectSelf(schemepath);
						--Win_SetCheck(schemepath, true);
						
						for i = 0, 4 do
							local childpath = path..".playerprop"..i;
							--Win_SetText(childpath..".prop_disc", prop[i+1].name);
							local progresspath = childpath..".prop_progressbar";
							--Win_ConsoleInfo(childpath);
							--Win_ConsoleInfo(prop[i+1].name);
							--Win_ConsoleInfo(progresspath);
							--game_info.REST_ROUND_TIME = game_info.CUR_GAMETIME;
							--game_info.MAX_ROUND_TIME = 1000;
							--text = prop[i+1].name..": "..prop[i+1].value.."/"..prop.maxpoints;
								if(i==0) then text = "增加车辆的最大速度"; end;
								if(i==1) then text = "增加氮气喷射的威力"; end;
								if(i==2) then text = "增加氮气收集的速度"; end;
								if(i==3) then text = "增加氮气喷射的时间"; end;
								if(i==4) then text = "增加小喷的时间";	 end;
							Win_SetText(progresspath, text, value_channel_tiptext);
							Win_SetValue(progresspath, prop.maxpoints, value_channel_maxvalue);
							Win_SetValue(progresspath, prop[i+1].value,value_channel_pos);
							Win_SetText(childpath..".cur_proppoint", prop[i+1].value);
							Win_SetText(path..".restpoints_label", ""..prop.restpoints);
							
							if (prop.restpoints == 0 or  prop[i+1].value >= prop.maxpoints) then
								Win_EnableWidget(childpath..".addpoint_btn", false);
							else
								Win_EnableWidget(childpath..".addpoint_btn", true);
							end;
							
							scheme = math.floor(prop[i+1].value*8/prop.maxpoints);
							Win_SetValue(progresspath, scheme, value_channel_framescheme);
							
							if Win_IsVisible("UI.SysTooltip") then
								wgtpath=Win_GetText("UI.SysTooltip",value_channel_host);
								if (wgtpath==progresspath) then
									Win_SetText("UI.SysTooltip",text);
								end;
							end;	
						end;	
					end;
					
					OnSelfShow = function()

					end;
					
					OnUpdate = function()
						
					end;
					
					children = 
					{						
						restpoints_label = 
						{
							type = "LABEL",
							rect = {101, 172, 140, 20},
							textstyle = dt_magicback + dt_left;
							--caption = prop.restpoints;							
						},
						
						resignpoints_btn=
						{
							type = "BUTTON",
							rect = {16, 205, 65, 35},
							bkimage = "res/uires/playerinfo/resignpoints_btn.gsa",
							tiptext="重新分配全部属性点",	
							
							OnClick = function()	
								RequestClearAttribute();
							end;												
						},
						
						autoasign_check=
						{
							type = "CHECK",
							rect  = {91, 209, 20, 20},
							bkimage = "res/uires/playerinfo/autoasign_check.gsa",
							tiptext="全自动分配属性点",	
							
										
							OnClick = function()		
								Win_ShowWidget("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg", true);
								Win_SetCapture("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg");
								Win_SetFocus("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg");
								local stragetyindex = prop.strategyprop;
								Win_ConsoleInfo("strategy = "..stragetyindex);
								if (stragetyindex < 128) then
									stragetyindex =stragetyindex + 128;																		
									Win_EnableWidget("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_btn", true);	
								else
									stragetyindex = stragetyindex - 128;									
									Win_EnableWidget("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_btn", false);	
								end;	
								prop.hasChange = true;
								Win_ConsoleInfo("strategy2 = "..stragetyindex);
								prop.strategyprop = stragetyindex;																
							end;						
							
						},
						
						autoasign_btn=
						{
							type = "BUTTON",
							rect = {163, 205, 84, 35},
							bkimage = "res/uires/playerinfo/autoasign_btn.gsa",
							tiptext="选择自动分配的方案",	
							
							OnClick = function()
								Win_ShowWidget("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg", true);
								Win_SetCapture("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg");
								Win_SetFocus("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg");
							end;
						},
						
						autoasign_dlg=
						{
							type = "DIALOG",
							rect = {257, -18, 276, 270},
							bkimage = "res/uires/playerinfo/autoasign_dlg.gsa",
							visible = 0,
							moveable = 1,
							_schemeindex = 0,
							
							children = 
							{
								checkgroup = 
								{
									type = "LABEL",
									rect = {45, 56, 178, 87},
									template = {"res/uires/propassignchoicetempl.lua"},
									
									OnInit = function()
										local path = "UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg.checkgroup";
										Win_ClearTemplates(path);
										Win_AddTemplate(path, 0, 0, 3);
									end;
									
									children = 
									{
										--[[average_check = 
										{
											type = "CHECK",
											rect = {57-45, 62-56, 21, 22},
											bkimage = "res/uires/playerinfo/autoasign_check.gsa",
											extendstyle = ui_btn_style_radio,
											groupid = 40,
											groupstop = 1,
											
											OnCheck = function(bCheck)
												if (bCheck) then
													SetPointAssignScheme(0);
												end;
											end;
											
											OnClick = function()
												SetPointAssignScheme(0);
											end;
										},
										
										speed_check=
										{
											type = "CHECK",
											rect = {143-45, 62-56, 21, 22},
											bkimage = "res/uires/playerinfo/autoasign_check.gsa",
											extendstyle = ui_btn_style_radio,
											groupid = 40,
											groupstop = 2,
											
											OnCheck = function(bCheck)
												if (bCheck) then
												
												end;
											end;
											
											OnClick = function()
												SetPointAssignScheme(1);
											end;
										},
										
										explosion_check=
										{
											type = "CHECK",
											rect = {57-45, 95-56, 21, 22},
											bkimage = "res/uires/playerinfo/autoasign_check.gsa",
											extendstyle = ui_btn_style_radio,
											groupid = 40,
											groupstop = 3,
											
											OnCheck = function(bCheck)
												if (bCheck) then
													
												end;
											end;
											
											OnClick = function()
												SetPointAssignScheme(2);
											end;
										},
										
										drift_check=
										{
											type = "CHECK",
											rect = {143-45, 95-56, 21, 22},
											bkimage = "res/uires/playerinfo/autoasign_check.gsa",
											extendstyle = ui_btn_style_radio,
											groupid = 40,
											groupstop = 4,
											
											OnCheck = function(bCheck)
												if (bCheck) then
													
												end;
											end;
											
											OnClick= function()
												SetPointAssignScheme(3);
											end;
										},]]

									},
								},
								desc_label = 
								{
									type = "LABEL",
									rect = {18, 137, 239, 67},
									textsize = 14,
									--textfont = 1,
									
								},
								Confirm_btn=
								{
									type = "BUTTON",
									rect = {64, 235, 65, 35},
									bkimage = "res/uires/playerinfo/confirm_btn.gsa",
									
									OnClick = function()
										Win_ShowWidget("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg", false);
										Win_SetCapture("UI.playerinfo_dlg");
										Win_SetFocus("UI.playerinfo_dlg");
										SendPointsToSever();
									end;
								},
								cancel_btn=
								{
									type = "BUTTON",
									rect = {150, 235, 65, 35},
									bkimage = "res/uires/playerinfo/cancel_btn.gsa",
									
									OnClick = function()
										Win_ShowWidget("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg", false);
										Win_SetCapture("UI.playerinfo_dlg");
										Win_SetFocus("UI.playerinfo_dlg");
										local myattribute = GetMyAttribute();
										if (myattribute.strategyprop == prop.strategyprop) then
											return;
										end;
										
										local selectindex = math.mod(prop.laststrategy, 4);
										local schemepath = "UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg.checkgroup.propassignchoice"..selectindex;
										Win_SelectSelf(schemepath);
										
										prop.hasChange = false;
										
										local checkbtnpath = "UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_check";
										local bPreStrategy = IsUsingStratgy(myattribute.strategyprop);
										local bCurStrategy = IsUsingStratgy(prop.strategyprop);
										if (bPreStrategy == bCurStrategy) then
											return;
										end;
										Win_EnableWidget("UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_btn", bPreStrategy);
										
										prop.strategyprop = myattribute.strategyprop;
										local bCheck = Win_IsChecked(checkbtnpath);
										if (bCheck) then
											Win_SetCheck(checkbtnpath, false);
										else
											Win_SetCheck(checkbtnpath, true);
										end;
									end;									
								},
							},
						},
					};
				},
				
				team_dlg = 
				{
					type = "DIALOG",
					rect  = {0, 0, 220, 200},
					visible = 0,
					
					children=
					{
						team_label = 
						{
							type = "LABEL",
							caption = "车队: 爆走小偷组",
							rect = {5, 20 * 2+ 3, 210, 16},		
							textstyle = dt_magicback + dt_left + dt_vcenter,								
							--drawcolor = {0, 0, 0, 255},
						},
						degree_label=
						{
							type = "LABEL",
							caption = "等级: lv1",
							rect = {5, 20 * 3 + 3, 210, 16},	
							textstyle = dt_magicback + dt_left + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
						career_label=
						{
							type = "LABEL",
							caption = "普通会员",
							rect = {5, 20 * 4 + 3, 210, 16},
							textstyle = dt_magicback + dt_left + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
						contribute_label=
						{
							type = "LABEL",
							caption = "贡献: 13000",
							rect = {5, 20 * 5 + 3, 210, 16},								
							textstyle = dt_magicback + dt_left + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
						experience_label=
						{
							type = "LABEL",
							caption = "经验  80%",
							rect = {5, 20 * 6 + 4, 210, 16},
							textstyle = dt_magicback + dt_left + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
						viewteam_label=
						{
							type = "BUTTON",
							caption = "查看车队",
							rect = {5 + 70, 20 * 7 + 3 + 10, 70, 16},								
							textstyle = dt_magicback + dt_center + dt_vcenter,
							--drawcolor = {0, 0, 0, 255},
						},
					}
				},
			},
		},
		
		
	};
	
};