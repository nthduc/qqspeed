local country_desc = 
{
	[0] = {name = "海拉尔王国", flag = "",};
	[1] = {name = "翡翠王国", flag = "",};
};

local UIScreen_NavitationBtn = 
{
	prefix = "UI.navigation.",
	[0] = 
	{
		btnname = "multiplay_btn",
		uiscreen  = "roomsel",
	},
	[1] = 
	{
		btnname = "shop_btn",
		uiscreen  = "shop",
	},
	[2] =
	{
		btnname = "carbarn_btn",
		uiscreen  = "startmenu",
	},
	[3] =
	{
		btnname = "task_btn",
		uiscreen  ="",
	},
	[4] =
	{
		btnname = "practice_btn",
		uiscreen  = "",
	},
	[5] =
	{
		btnname = "citycenter_btn",
		uiscreen  = "",
	},
	[6] =
	{
		btnname = "challenge_btn",
		uiscreen  = "",
	},
	[7] =
	{
		btnname = "honor_btn",
		uiscreen  = "",
	},
};

function UpdateNavitationBtnState()
	local curscreen = GetCurUIScreen();
	
	Win_ConsoleInfo("UpdateNavitationBtnState, screen = "..curscreen);
	if (curscreen ~= "startmenu") then
		Win_EnableWidget("UI.shortcut_dlg.carbarnAssociated_label.multiplay_btn2", true);
	end;
	local bFind = false;
	if (curscreen ~= nil ) then
		for i = 0, 7 do
			Win_ConsoleInfo("screen"..i.."="..(UIScreen_NavitationBtn[i].uiscreen));
			if (UIScreen_NavitationBtn[i].uiscreen == curscreen) then				
				UI.children.navigation.UpdateSelect((UIScreen_NavitationBtn.prefix)..(UIScreen_NavitationBtn[i].btnname));
				--Win_ConsoleInfo(UIScreen_NavitationBtn[i].btnname);
				bFind = true;
				break;
			end;
		end;
	end;
	
	if (bFind ~= true) then
		for i = 0, 7 do
			if (UIScreen_NavitationBtn[i].uiscreen ~= "") then
				local path = (UIScreen_NavitationBtn.prefix)..(UIScreen_NavitationBtn[i].btnname);
				Win_EnableWidget(path, true);			
			end;
		end;
		
		UI.children.navigation._curSelectBtnPath = nil;
	end;
	

	local path = "UI.navigation.miscellaneous_dlg.roominfo_label";
	if (curscreen == "room") then
		Win_ShowWidget(path, true);
		local roominfo = GetMyRoomInfo();
		if (roominfo.roomid < 10) then
			Win_SetText(path..".roomid", "房间000"..roominfo.roomid);
		elseif (roominfo.roomid < 100) then
			Win_SetText(path..".roomid", "房间00"..roominfo.roomid);
		elseif (roominfo.roomid < 1000) then
			Win_SetText(path..".roomid", "房间0"..roominfo.roomid);
		else
			Win_SetText(path..".roomid", "房间"..roominfo.roomid);
		end;
		Win_SetText(path..".roomname", roominfo.roomname);
		local roomtype = "";
		local gamemodebase = roominfo.gamemodebase;
		local gamemodesub  = roominfo.gamemodesub;
		
		if (gamemodesub == gmodesub_single) then
			roomtype = "个人";
		else
			roomtype = "组队";
		end;
		
		if (gamemodebase == gmodebase_item) then
			roomtype = roomtype.."道具赛";
		else
			roomtype = roomtype.."竞速赛";
		end;
			
		
		Win_SetText(path..".roomtype",roomtype);
	else
		Win_ShowWidget(path, false);
	end;
end;


UI.children.navigation = 
{
	type = "DIALOG",
	rect = {0,-768/5,1024,100},	
	--bkcolor={0,50,200,200},
	initlayer=msgbox_layer,
	bkimagepos = {3, 83},
	bkimage = "res/uires/navigation/friend_bk.gsa",	
	visible = 0,
	_curSelectBtnPath = nil,
	_IsInRoomPressBtn = false,	
	
	--drawflag=drawflag_win_edge+drawflag_win_fill,
	pathmove=0,
	pathflip=0,
	pathpoints=
	{
		{0,-768/5,ui_pathcmd_cycle+ui_pathcmd_alpha,0.0,1.0};
		{0,0,ui_pathcmd_cycle+ui_pathcmd_alpha,1.0,1.0};
	};
		
		
	OnPathCommand=function(id,x,y)
		if (id==1) then
			Win_ShowWidget("UI.navigation",true);			
			Win_SetValue("UI.navigation",1,value_channel_pathmove_pause);	
		elseif (id==0) then
			Win_ShowWidget("UI.navigation",false);
		end;						  
	end;
	
	OnSelfShow = function()
		local myinfo = GetMyInfo();
		local path = "UI.navigation.miscellaneous_dlg";
		if (myinfo) then
			local dbinfo = myinfo.playerdbinfo;
			
			--Win_SetText(path..".country_label", country_desc[dbinfo.country].name);
				
			local level = GetPlayerLevelByExp(myinfo.playerdbinfo.exp);
			if (level) then
				Win_SetText(path..".level_label", " lv"..level);
			end;
			
			Win_SetText(path..".exploit_label", " "..dbinfo.honor);
			Win_SetText(path..".kubi_label", " "..dbinfo.money);
		end;	

		UpdateNavitationBtnState();		
	end;
			
	OnSelfHide = function()
		Win_RecoverPathPoint("UI.navigation");
		cmd_hideselfInfo();
	end;
	
	UpdateSelect = function(path)
		--if (UI.children.navigation._curSelectBtnPath ~= nil and path == UI.children.navigation._curSelectBtnPath) then
		--	return false;
		--end;
		
		Win_SelectSelf(path);
		Win_EnableWidget(path, false);
		UI.children.navigation._curSelectBtnPath = path;
		Win_ConsoleInfo("path = "..path.." screen = "..GetCurUIScreen() );
		if (GetCurUIScreen() == "room") then
			Win_ConsoleInfo("LeaveRoom");
			UI.children.navigation._IsInRoomPressBtn = true;
			LeaveRoom();
		else
			UI.children.navigation._IsInRoomPressBtn = false;
		end;			
		return true;
	end;

	children = 
	{
		bk_wgt = 
		{
			type = "WIDGET",
			rect  = {0, 0, 0, 0},
			--bkimagepos = {-3, -83,},
			bkimage="res/uires/navigation/navigation_bk.gsa",
			initlayer = 0,
		},
		friend_wgt = 
		{
			type = "WIDGET",
			rect  = {3, 83, 120, 120},
			--bkimagepos = {-3, -83,},
			--bkimage="res/uires/navigation/navigation_bk.gsa",
			initlayer = 1,
			
			children = 
			{
				friend_btn = 
				{
					type = "BUTTON",
					rect = {10, 38, 55, 55},
					--caption = "好友",
					bkimage = "res/uires/navigation/friend_btn.gsa",
					--groupid = 999,
					--groupstop = 1,
				},
				help_btn = 
				{
					type = "BUTTON",
					rect = {85, -3, 32, 32},
					bkimage = "res/uires/navigation/help_btn.gsa",
					
					OnClick = function()
						Win_ShowWidget("UI.help", true);
					end;
				},
			},			
			
		},
		
		
		
		
		multiplay_btn = 
		{
			type = "BUTTON",
			rect = {0, -10, 126, 78},
			--caption = "多人游戏",
			bkimage = "res/uires/navigation/multiplay_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 1,
			initlayer = 1,
			tiptext = "开始多人游戏",
			
			OnClick = function()
				--if (bCheck == 1) then
					if (UI.children.navigation.UpdateSelect("UI.navigation.multiplay_btn") ~= true) then
						return;
					end;
					--Win_SelectSelf("UI.navigation.multiplay_btn");
					--Win_EnableWidget("UI.navigation.multiplay_btn", false);
					--UI.children.navigation._curSelectBtnPath = "UI.navigation.multiplay_btn";
					--Win_ConsoleInfo("NotInGameUI");
					cmd_mutiplay();					
					--LoginGame(logintype_multi);
				--end;
			end;
			
			OnCheck = function(bCheck)			
				if (bCheck ~= 1) then
					Win_ConsoleInfo("Multiplay Be UnChecked");
					Win_EnableWidget("UI.navigation.multiplay_btn", true);
				end;
			end;
		},
		
		shop_btn = 
		{
			type = "BUTTON",
			rect  = {110, -1, 126, 78},
			--caption = "商店",
			bkimage = "res/uires/navigation/shop_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 2,
			initlayer = 1,
			tiptext = "购买新车和衣服",
			
			OnClick = function()
				if (UI.children.navigation.UpdateSelect("UI.navigation.shop_btn") ~= true) then
					return ;
				end;
				--Win_EnableWidget("UI.navigation.shop_btn", false);
				--Win_SelectSelf("UI.navigation.shop_btn");
				--UI.children.navigation._curSelectBtnPath = UI.children.navigation.children.shop_btn;
				GotoUIScreen("shop");
			end;
			
			OnCheck = function(bCheck)
				if (bCheck~=1) then					
					Win_ConsoleInfo("shop_btn Be UnChecked");
					Win_EnableWidget("UI.navigation.shop_btn", true);
				end;
			end;
		},
		
		carbarn_btn = 
		{
			type = "BUTTON",
			rect  = {240, -1, 126, 78},
			--caption = "车库",
			bkimage = "res/uires/navigation/carbarn_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 3,
			initlayer = 1,
			tiptext = "在车库装扮自己",
			
			OnClick = function()
				if (UI.children.navigation.UpdateSelect("UI.navigation.carbarn_btn") ~= true) then
					return ;
				end;
				GotoUIScreen("startmenu");
				--Win_EnableWidget("UI.navigation.carbarn_btn", false);
				--Win_SelectSelf("UI.navigation.carbarn_btn");
				--UI.children.navigation._curSelectBtnPath = UI.children.navigation.children.carbarn_btn;
			end;
			
			OnCheck = function(bCheck)
				if (bCheck ~= 1) then
					Win_EnableWidget("UI.navigation.carbarn_btn", true);
				end;
			end;
		},
		
		task_btn = 
		{
			type = "BUTTON",
			rect  = {370, -1, 126, 78},
			--caption = "任务",
			bkimage = "res/uires/navigation/task_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 4,
			initlayer = 1,
			tiptext = "游戏任务列表",
			
			OnClick = function()
				return;
				
				--if (UI.children.navigation.UpdateSelect("UI.navigation.task_btn") ~= true) then
				--	return;
				--end;
				--Win_EnableWidget("UI.navigation.task_btn", false);
				--Win_SelectSelf("UI.navigation.task_btn");
				--UI.children.navigation._curSelectBtnPath = UI.children.navigation.children.task_btn;
			end;
			
			OnCheck = function(bCheck)
				if (bCheck ~= 1) then
					Win_EnableWidget("UI.navigation.task_btn", true);
				end;
			end;
		},
		practice_btn = 
		{
			type = "BUTTON",
			rect  = {500, -1, 126, 78},
			--caption = "联系",
			bkimage ="res/uires/navigation/practice_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 5,
			initlayer = 1,
			tiptext = "个人训练/比赛回放",
			
			OnClick = function()
				return;
				--if (UI.children.navigation.UpdateSelect("UI.navigation.practice_btn") ~= true) then
				--	return;
				--end;
				--Win_EnableWidget("UI.navigation.practice_btn", false);
				--Win_SelectSelf("UI.navigation.practice_btn");
				--UI.children.navigation._curSelectBtnPath = UI.children.navigation.children.practice_btn;
			end;
			
			OnCheck = function(bCheck)
				if (bCheck ~= 1) then
					Win_EnableWidget("UI.navigation.practice_btn", true);
				end;
			end;
		},
		citycenter_btn = 
		{
			type = "BUTTON",
			rect  = {630, -1, 126, 78},
			--caption = "车库",
			bkimage = "res/uires/navigation/citycenter_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 6,
			initlayer = 1,
			tiptext = "聊天交友休闲升级",
			
			OnClick = function()
				return;
				--if (UI.children.navigation.UpdateSelect("UI.navigation.citycenter_btn") ~= true) then
				--	return;
				--end;
				--Win_EnableWidget("UI.navigation.citycenter_btn", false);
				--Win_SelectSelf("UI.navigation.citycenter_btn");
				--UI.children.navigation._curSelectBtnPath = UI.children.navigation.children.citycenter_btn;
			end;
			
			OnCheck = function(bCheck)
				if (bCheck ~= 1) then
					Win_EnableWidget("UI.navigation.citycenter_btn", true);
				end;
			end;
		},
		challenge_btn = 
		{
			type = "BUTTON",
			rect  = {760, -1, 126, 78},
			--caption = "地域争霸",
			bkimage = "res/uires/navigation/challenge_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 7,
			initlayer = 1,
			tiptext = "车队据点争霸赛",
			
			OnClick = function()
				return;
				--if (UI.children.navigation.UpdateSelect("UI.navigation.challenge_btn") ~= true) then
				--	return;
				--end;
				--Win_EnableWidget("UI.navigation.challenge_btn", false);
				--Win_SelectSelf("UI.navigation.challenge_btn");
				--UI.children.navigation._curSelectBtnPath = UI.children.navigation.children.challenge_btn;
			end;
			
			OnCheck = function(bCheck)
				if (bCheck ~= 1) then
					Win_EnableWidget("UI.navigation.challenge_btn", true);
				end;
			end;
		},
		honor_btn = 
		{
			type = "BUTTON",
			rect  = {890, -1, 126, 78},
			--caption = "荣誉",
			bkimage = "res/uires/navigation/honor_btn.gsa",
			framescheme={{0,1,2,0}},
			groupid = 999,
			groupstop = 8,
			initlayer = 1,
			tiptext = "国际比赛为荣誉而战！",
			
			OnClick = function()
				return;
				--if (UI.children.navigation.UpdateSelect("UI.navigation.honor_btn") ~= true) then
				--	return;
				--end;
				--Win_EnableWidget("UI.navigation.honor_btn", false);
				--Win_SelectSelf("UI.navigation.honor_btn");
				--UI.children.navigation._curSelectBtnPath = UI.children.navigation.children.honor_btn;
			end;
			
			OnCheck = function(bCheck)
				if (bCheck ~= 1) then
					Win_EnableWidget("UI.navigation.honor_btn", true);
				end;
			end;
		},
		
		return_btn2 = 
		{
			type = "BUTTON",
			rect = {982, 66, 42, 43},
			bkimage = "res/uires/navigation/return_btn2.gsa",
			tiptext = "上一步",
			
			OnClick = function()
				UI.children.shortcut_dlg.children.return_btn.OnClick();
			end;
		},
		
		miscellaneous_dlg = 
		{
			type = "DIALOG",
			rect = {338, 77, 588, 21},
			moveable = 0,
			bkimage = "res/uires/navigation/miscellaneous_bk.gsa",
			textsize = 12,			
		
			
			children = 
			{
				--[[country_label = 
				{
					type = "LABEL",
					rect = {10, 5, 100, 20},
					caption = "海拉尔王国",
				},
				
				country_status_label=
				{
					type = "PICLABEL",
					fontpic = "",
					rect = {160, 0, 30, 30},
				},]]
				level_label = 
				{
					type = "LABEL",
					rect = {66, 0, 100, 16},
					caption = "lv100",--等级: 
				},
				exploit_label = 
				{
					type = "LABEL",
					rect = {184, 0, 100, 16},
					caption = "1000",--功勋: 
				},
				kubi_label = 
				{
					type = "LABEL",
					rect = {319, 0, 100, 16},
					caption = "1000",--酷币: 
				},
				
				encharge = 
				{
					type = "BUTTON",
					rect = {457, 2, 61, 14},
					--caption = "Q币冲值",
					bkimage = "res/uires/navigation/QB_btn.gsa",
					
					OnClick = function()
						Win_ShowMsgBox("请购买Q币卡充值！");
					end;
				},
				roominfo_label=
				{
					type = "LABEL",
					rect = {88, 21, 429, 26},
					bkimage = "res/uires/room/roominfo_label.gsa",
					visible = 0,		
					
					
					
					children = 
					{
						roomid=
						{
							type = "LABEL",
							rect = {24, 3, 80, 16},
							caption = "房间0",
							textsize=12,
							textstyle=dt_bottom+dt_center,
						},
						
						roomname=
						{
							type = "LABEL",
							rect = {136, 3, 184,16},
							caption = "大家一起来",
							textsize=12,
							textstyle=dt_bottom+dt_center,
						},
						
						roomtype=
						{
							type = "LABEL",
							rect = {320, 3, 82, 16},
							caption = "道具赛",
							textsize=12,
							textstyle=dt_bottom+dt_center,
						},
					},
				},
			},
		},
		bugreport_btn=
		{
			type = "BUTTON",
			rect = {0, 187, 78, 134},
			bkimage = "res/uires/navigation/bugReport.gsa",
			
			OnClick = function()
				WinExecute("http://www.npc6.com/qqspeed_book");
			end;
		},
	},
};

UI.children.shortcut_dlg = 
{
	type = "DIALOG",
	rect = {0, 687, 100, 81},
	bkimage = "res/uires/common/common_bk.gsa",
	initlayer=msgbox_layer,
	visible = 0,
	
	pathmove=0,
	pathflip=0,
	pathpoints=
	{
		{0,687 + 768/5,ui_pathcmd_cycle+ui_pathcmd_alpha,0.0,1.0};
		{0,687,ui_pathcmd_cycle+ui_pathcmd_alpha,1.0,1.0};
	};
	
	OnPathCommand=function(id,x,y)
		if (id==1) then
			Win_ShowWidget("UI.shortcut_dlg",true);			
			Win_SetValue("UI.shortcut_dlg",1,value_channel_pathmove_pause);	
		elseif (id==0) then
			Win_ShowWidget("UI.shortcut_dlg",false);
		end;						  
	end;
	
	OnSelfShow = function()
		CheckUpgrade();
		local curscreen = GetCurUIScreen();
		if (curscreen == "shop") then
			Win_ShowWidget("UI.shortcut_dlg.shopAssociated_label", true);			
		else
			Win_ShowWidget("UI.shortcut_dlg.shopAssociated_label", false);
		end;
		
		if (curscreen == "roomsel") then
			Win_ShowWidget("UI.shortcut_dlg.hallAssociated_label", true);
		else
			Win_ShowWidget("UI.shortcut_dlg.hallAssociated_label", false);
		end;
		
		if (curscreen == "startmenu") then
			Win_ShowWidget("UI.shortcut_dlg.carbarnAssociated_label", true);
		else
			Win_ShowWidget("UI.shortcut_dlg.carbarnAssociated_label", false);
		end;
		
		if (curscreen == "room") then
			Win_ShowWidget("UI.shortcut_dlg.roomAssociated_wgt", true);
		else
			Win_ShowWidget("UI.shortcut_dlg.roomAssociated_wgt", false);
		end;
	end;
	
	children = 
	{
		return_btn = 
		{
			type = "BUTTON",
			rect  = {0, 18, 65, 62},
			--caption = "返回",
			bkimage = "res/uires/common/return_btn.gsa",
			tiptext = "上一步",
			
			OnClick = function()
				if (UI.children.navigation._curSelectBtnPath ~= nil and UI.children.navigation._curSelectBtnPath ~= "") then
					Win_EnableWidget(UI.children.navigation._curSelectBtnPath, true);
				end;
				cmd_goback();
			end;
		},
		setting_btn = 
		{
			type = "BUTTON",
			rect = {67, 43, 24, 36},
			--caption = "设置",
			bkimage = "res/uires/common/setting_btn.gsa",	
			tiptext = "系统设置",
			
			OnClick = function()
				--RequestClearAttribute();
			end;
		},
		
		upgrade_btn=
		{
			type = "PICLABEL",
			rect = {90, -25, 42, 43},
			captionoffset = {{15, 15,},{15, 15,},{15, 15,}},
			--caption = "信息",
			fontpic = "res/uires/common/upgrade_btn.gsa",
			visible = 0,
			_curframe = 1,
			_bStop = false,
			updatemute = 1,
			
			OnSelfShow = function()
				UI.children.shortcut_dlg.children.upgrade_btn._bStop = false;
				UI.children.shortcut_dlg.children.upgrade_btn._curframe = 1;
				local curframe = UI.children.shortcut_dlg.children.upgrade_btn._curframe;
				--Win_ConsoleInfo(tostring(curframe));
				Win_SetText("UI.shortcut_dlg.upgrade_btn", tostring(curframe));
				Win_SetValue("UI.shortcut_dlg.upgrade_btn", 100, value_channel_timerspace);
				curframe = math.mod(curframe+1, 3)+1;
				UI.children.shortcut_dlg.children.upgrade_btn._curframe = curframe;
			end;
			
			OnMouseIn = function()
				Win_SetValue("UI.shortcut_dlg.upgrade_btn", -1, value_channel_timerspace);
			end;
			
			OnMouseOut=function()
				if (UI.children.shortcut_dlg.children.upgrade_btn._bStop ~= true) then
					Win_SetValue("UI.shortcut_dlg.upgrade_btn", 50, value_channel_timerspace);				
				end;
			end;
			
			OnTimer = function()
				local curframe = UI.children.shortcut_dlg.children.upgrade_btn._curframe;
				Win_SetText("UI.shortcut_dlg.upgrade_btn", tostring(curframe));
				--Win_ConsoleInfo(tostring(curframe));
				curframe = math.mod(curframe+1, 3)+1;
				UI.children.shortcut_dlg.children.upgrade_btn._curframe = curframe;
				if (curframe == 3) then
					Win_SetValue("UI.shortcut_dlg.upgrade_btn", 500, value_channel_timerspace);
				else				
					Win_SetValue("UI.shortcut_dlg.upgrade_btn", 100, value_channel_timerspace);
				end;
				
			end;
			
			OnClick = function()
				local myinfo = GetMyInfo();
				cmd_showplayerinfo(myinfo.uin, true);
				Win_ShowWidget("UI.shortcut_dlg.upgrade_btn", false);
				Win_SelectSelf("UI.playerinfo_dlg.tabbar.playerprop");
				Win_SetValue("UI.shortcut_dlg.upgrade_btn", -1, value_channel_timerspace);
				UI.children.shortcut_dlg.children.upgrade_btn._bStop = true;
			end;
		};
		
		selfinfo_btn = 
		{
			type = "BUTTON",
			rect = {95, 43, 24, 36},
			--caption = "信息",
			bkimage = "res/uires/common/selfinfo_btn.gsa",
			tiptext = "我的信息/属性加点",
			
			OnClick = function()
				local myinfo = GetMyInfo();
				if (cmd_isselfinfovisible()) then							
					if (myinfo) then								
						cmd_showplayerinfo(myinfo.uin, false);
					else
						Win_ConsoleInfo("个人资料为空？");
					end;
				else
					Win_ConsoleInfo("Show PlayerInfo");
					cmd_showplayerinfo(myinfo.uin, true);
				end;
			end;
		},
		shopAssociated_label = 
		{
			type = "LABEL",
			rect = {655, -2, 369, 83},
			bkimage = "res/uires/navigation/shopassociate_bk.gsa",
			visible = 0,
			
			children = 
			{
				charge_btn = 
				{
					type = "BUTTON",
					rect = {29, 28, 162, 54},
					bkimage = "res/uires/navigation/charge_btn.gsa",
					tiptext = "使用Q币卡充值酷币",
					
					
					OnClick = function()
						Win_ShowMsgBox("请购买Q币卡充值！");
					end;
				},
				vip_btn = 
				{
					type = "BUTTON",
					rect  = {194, 28, 162, 54},
					--caption = "成为会员",
					bkimage = "res/uires/navigation/vip_btn.gsa",
					tiptext = "开启紫钻成为VIP会员",
					
					OnClick = function()
						Win_ShowMsgBox("本游戏紫钻业务尚未开通！");
					end;
				},
			},
		},
		hallAssociated_label = 
		{
			type = "LABEL",
			rect = {655, -59, 369, 140},
			bkimage = "res/uires/navigation/frontierchannel_bk.gsa",
			visible = 0,
			
			children = 
			{
				frontierchannel_btn = 
				{
					type = "BUTTON",
					rect = {34, 71, 317, 64},
					bkimage = "res/uires/navigation/frontierchannel_btn.gsa",
					tiptext = "国际比赛为荣誉而战!",
				
					OnClick = function()
					end;
				},
				
				specialarrow_label = 
				{
					type = "LABEL",
					rect = {263, 25, 105, 87},
					bkimage = "res/uires/navigation/specialarrow.gsa",
				},
			},
		},
		carbarnAssociated_label = 
		{
			type = "LABEL",
			rect = {655, -114, 369, 100},
			bkimagepos = {207, -9},
			bkimage = "res/uires/navigation/multiplay2_bk.gsa",			
			moveable = 0,
			visible = 0,
			
			children = 
			{
				multiplay_btn2 =
				{
					type = "BUTTON",
					rect = {204, -55, 165, 150},	
					bkimage = "res/uires/navigation/multiplay_btn2.gsa",
					tiptext = "这里进入多人游戏",
					
					OnClick = function()
						Win_EnableWidget("UI.shortcut_dlg.carbarnAssociated_label.multiplay_btn2", false);
						UI.children.navigation.children.multiplay_btn.OnClick();
					end;
				},
				multibtn_label = 
				{
					type = "LABEL",
					rect = {0, 96, 83*4, 55},
					bkimage = "res/uires/navigation/carbarnAssociated_label.gsa",
					moveable = 0,
					visible = 1,
					
					children = 
					{
						equip_btn=
						{
							type = "BUTTON",
							rect = {29,28, 110, 55},
							bkimage = "res/uires/navigation/equip_btn.gsa",
							tiptext = "装扮出最酷的自己",
							
							OnClick = function()
								cmd_avatarsel(ECBT_BASE_KARTEQP);
							end;
						},
						
						car_btn = 
						{
							type = "BUTTON",
							rect = {110+29, 28, 110, 55},
							bkimage = "res/uires/navigation/car_btn.gsa",
							tiptext = "更换个性赛车",
							
							OnClick = function()
								cmd_avatarsel(ECBT_BASE_KART);
							end;
						},
						pet_btn = 
						{
							type = "BUTTON",
							rect = {110 * 2+29,28, 110, 55},
							bkimage = "res/uires/navigation/pet_btn.gsa",
							tiptext = "可爱的替身宝宝",
							
							OnClick = function()
								cmd_avatarsel(ECBT_BASE_PET);
							end;
						},

						--[[
						adorn_btn=
						{
							type = "BUTTON",
							rect = {82 * 3+29, 28, 83, 55},
							bkimage = "res/uires/navigation/adorn_btn.gsa",
							iptext = "装扮出最酷的自己",
							
							OnClick = function()
								cmd_avatarsel(EIGT_ADORN);
							end;
						},
						]]
					},
				},
			},
		},
		roomAssociated_wgt = 
		{		
			type="WIDGET",
			rect={655,-18,369, 99},--109*3,54},
			bkimage = "res/uires/navigation/multiplay2_bk.gsa",			
			bkimagepos = {207, -107},
			--bkimage = "res/uires/navigation/roomAssociated_wgt.gsa",
			rectid=0;--GetMyRoomInfo().gamemodesub;
			--autoscale = 1,
			visible = 0;
			
			Init=function()				
			end;
			
			SetReady=function(ready)
				UI.children.shortcut_dlg.children.roomAssociated_wgt.children.ReadyBtn.Refresh();
			end;
			
			OnSelfShow = function()
				UI.children.shortcut_dlg.children.roomAssociated_wgt.rectid = GetMyRoomInfo().gamemodesub;
				UI.children.shortcut_dlg.children.roomAssociated_wgt.children.ReadyBtn.Refresh();
			end;
			
			children=
			{
				bklabel = 
				{
					type = "LABEL",
					bkimage = "res/uires/navigation/roomAssociated_wgt.gsa",
					rect = {0, 0, 369, 99},
					enable = 0,
					initlayer = 0,
					
				};
				ReadyBtn=
				{
					type="BUTTON",
					rect={246,-117,163,150},
					textsize=12,
					textstyle=dt_bottom+dt_center,
					bkimage="res/uires/room/readybtn.gsa",
					bkimagepos={-37,-20},
					initlayer = 1,
					
					OnInit=function()
						UI.children.shortcut_dlg.children.roomAssociated_wgt.children.ReadyBtn.Refresh();
					end;
					
					Refresh=function()
						Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelBtn",false);
						local roominfo = GetMyRoomInfo();
						if (roominfo == nil) then
							return;
						end;
						if roominfo.readystate==state_readyed then
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.AvatarSelBtn",false);
						end;
						if roominfo.readystate==state_unreadyed then
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.AvatarSelBtn",true);
						end;
						
						if GetMyInfo().isroomer == 1 then	--是队长，将按钮设置为开始游戏
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.AvatarSelBtn",true);
							Win_SetBackPic("UI.shortcut_dlg.roomAssociated_wgt.ReadyBtn","res/uires/room/startbtn.gsa");
							--Win_SetText("UI.shortcut_dlg.roomAssociated_wgt.ReadyBtn","开始");
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.MapSelBtn",true);
							Win_EnableWidget("UI.room.MainFrame.PlayerListPanel.MapPanel.MapThumb",true);
							local ri=GetMyRoomInfo();
							if ri.gamemodesub==gmodesub_team then
								Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelBtn",true);
							end;
						end;
						if GetMyInfo().isroomer == 0 then	
							local ready=GetMyRoomInfo().readystate;
							local tmap=	{	
										[state_readyed] = { ["image"]="res/uires/room/unreadybtn.gsa",["caption"]="取消准备" },
										[state_unreadyed] = { ["image"]="res/uires/room/readybtn.gsa",["caption"]="准备" },
									};
							Win_SetBackPic("UI.shortcut_dlg.roomAssociated_wgt.ReadyBtn",tmap[ready].image);
							--Win_SetText("UI.shortcut_dlg.roomAssociated_wgt.ReadyBtn",tmap[ready].caption);
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.MapSelBtn",false);
							Win_EnableWidget("UI.room.MainFrame.PlayerListPanel.MapPanel.MapThumb",false);
							local ri=GetMyRoomInfo();
							Win_ConsoleInfo("gmode="..ri.gamemodesub);
							Win_ConsoleInfo("ready="..ready);
							if ri.gamemodesub==gmodesub_team then
								if ready==state_readyed then
									Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelBtn",false);
								end;
								if ready==state_unreadyed then
									Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelBtn",true);
								end;
							end;
						end;
					end;
					
					OnClick=function()
						if GetMyInfo().isroomer == 1 then --是队长，开始游戏
							--RemoveSelfKartFromUIScene("ROOT");
							--RemoveSelfRoleFromUIScene("playermode");
							--ReBindSelfPlayer();
							StartGame();
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.ReadyBtn",false);
						else
							ToggleReady();	--非队长，切换准备状态
						end;
					end;						
				};

				MapSelBtn=
				{
					type="BUTTON",
					rect={29,27,109,54},
					textsize=12,
					textstyle=dt_bottom+dt_center,
					bkimage="res/uires/room/mapselbtn.gsa",
					--bkimagepos={-7,-5},
					initlayer = 1,
					enable = 0,
					
					OnClick=function()
						cmd_mapsel();
					end;
				};
				
				AvatarSelBtn=
				{
					type="BUTTON",
					rect={109*2+29,27,109,54},
					textsize=12,
					textstyle=dt_bottom+dt_center,
					bkimage="res/uires/room/avatarselbtn.gsa",
					--bkimagepos={-7,-5},
					initlayer = 1,
					
					OnClick=function()
						cmd_avatarsel(ECBT_BASE_KART);
					end;
				};
				
				TeamSelBtn=
				{
					type="BUTTON",
					rect={109+29,27,109,54},
					textsize=12,
					textstyle=dt_bottom+dt_center,
					bkimage="res/uires/room/teamselbtn.gsa",
					--bkimagepos={-7,-5},
					enable=0,
					initlayer = 1,
					
					OnInit=function()
						
						local ri=GetMyRoomInfo();
						if (ri == nil) then
							return;
						end;
						
						if ri.gamemodesub==gmodesub_team then
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelBtn",true);
						end;
						if ri.gamemodesub~=gmodesub_team then
							Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelBtn",false);
						end;
					end;
					OnClick=function()
						Win_ShowWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",true);
						--Win_AlignWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg","UI.shortcut_dlg.roomAssociated_wgt.TeamSelBtn",aligntype_winrect,alignstyle_top_in+alignstyle_right_out,-5);
					end;
					OnMouseOut=function()
						Win_SetValue("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",100,value_channel_timerspace);
					end;
					
				};
				
				TeamSelDlg=
				{
					type="WIDGET",
					rect={4,-216,323,241},
					visible=0,
					initlayer=msgbox_layer-1;
					bkimage="res/uires/room/teamselbk.gsa",
					--align_type = aligntype_winrect,
					--align_style = alignstyle_hcenter + alignstyle_vcenter,
					
					OnMouseOut=function()
						Win_SetValue("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",100,value_channel_timerspace);
					end;
					OnMouseIn=function()
						Win_SetValue("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",-1,value_channel_timerspace);
					end;
					OnTimer=function()
						Win_ConsoleInfo("timer,false");
						Win_ShowWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",false);
					end;
					children=
					{
						team_b=
						{
							type="BUTTON",
							rect={50,52,124,158},
							bkimage = "res/uires/room/blueteam_btn.gsa",
							
							OnMouseIn=function()
								Win_SetValue("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",-1,value_channel_timerspace);
							end;
							
							OnClick=function()
								ChangeTeam(1);
								Win_ShowWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",false);
							end;
						};
						team_r=
						{
							type="BUTTON",
							rect={193,52,124,158},
							bkimage = "res/uires/room/redteam_btn.gsa",
							
							OnMouseIn=function()
								Win_SetValue("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",-1,value_channel_timerspace);
							end;
							
							OnClick=function()
								ChangeTeam(0);
								Win_ShowWidget("UI.shortcut_dlg.roomAssociated_wgt.TeamSelDlg",false);
							end;
						};
					};
				};
				--[[returnbtn=
				{
					type="BUTTON",
					rect={0,603,90,90},
					bkimage="res/uires/startmenu/returnbtn.gsa",
					bkimagepos={-7,-5},
					OnClick=function()
						cmd_goback();
					end;
				};	]]					
			};
				
		},
	},			
};