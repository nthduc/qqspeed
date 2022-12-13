UI.children.SysLoginMsgbox=
{
	type="MESSAGEBOX",
	rect = { 0, 0, 340, 270},	
	bkimage="res/uires/common/msgbox.gsa",
	textsize=12,
	textfont=1,
	textstyle=dt_center,
	initlayer=msgbox_layer,
	style=wgtstyle_topmost;
	visible=0,
	
	OnSelfShow=function()
		Win_SetCapture("UI.SysLoginMsgbox");
		Win_SetFocus("UI.SysLoginMsgbox");
	end;
		
	OnCancel=function()
		CancelLogin();
		Win_ShowWidget("UI.SysLoginMsgbox", false);
	end;	
		 
	OnSocketConnect=function(entity, code)
		if entity == eQQServer then
			if code == 0 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "连接QQ签名服务器成功");
			else
				Win_SetText("UI.SysLoginMsgbox.sentence", "连接QQ签名服务器失败");
				Win_ShowWidget("UI.SysLoginMsgbox", true);
			end;
		end;
		if entity == eDirServer then
			if code == 0 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "连接目录服务器成功");
			else
				Win_SetText("UI.SysLoginMsgbox.sentence", "连接目录服务器失败");
				Win_ShowWidget("UI.SysLoginMsgbox", true);
			end;
		end;
		if entity == eGameServer then
			if code == 0 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "连接游戏服务器成功");
			else
				Win_SetText("UI.SysLoginMsgbox.sentence", "连接游戏服务器失败");
				Win_ShowWidget("UI.SysLoginMsgbox", true);
			end;
		end;
	end;
	
	OnSocketClose=function(entity, code)
		if entity == eGameServer then
			Win_SetText("UI.SysLoginMsgbox.sentence", "与游戏服务器的连接被关闭");
			Win_ShowWidget("UI.SysLoginMsgbox", true);
		end;
	end;
	

	OnStartLogin=function(entity,code)
		if entity == eQQServer then
			Win_SetText("UI.SysLoginMsgbox.sentence", "正在连接服务器..");
		end;
		if entity == eDirServer then
			Win_SetText("UI.SysLoginMsgbox.sentence", "正在连接服务器...");
		end;
		if entity == eGameServer then
			Win_SetText("UI.SysLoginMsgbox.sentence", "正在连接服务器....");
		end;
	end;
	
	OnResponseLogin=function(entity, result)
		if entity == eQQServer then
			if result == 0 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "正在连接服务器..");
			elseif result == 1 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "验证失败，请确认用户名密码是否输入正确");
				Win_ShowWidget("UI.SysLoginMsgbox", true);
			elseif result == 2 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "登陆超时");
				Win_ShowWidget("UI.SysLoginMsgbox", true);			
			else
				Win_SetText("UI.SysLoginMsgbox.sentence", "登陆失败");
				Win_ShowWidget("UI.SysLoginMsgbox", true);			
			end;
		end;
		if entity == eDirServer then
			if result == 0 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "正在连接服务器...");
			else
				--Win_SetText("UI.SysLoginMsgbox.sentence", "登陆目录服务器失败");
				local loginresult=GetDirInfo();
				if loginresult~=nil and loginresult.reason ~= nil and loginresult.reason ~= "" then
					Win_SetText("UI.SysLoginMsgbox.sentence", loginresult.reason);
				else
					Win_SetText("UI.SysLoginMsgbox.sentence", "无法连接服务器");
				end;
				Win_ShowWidget("UI.SysLoginMsgbox", true);
			end;
		end;
		if entity == eGameServer then
			if result == 0 then
				Win_SetText("UI.SysLoginMsgbox.sentence", "登陆成功.");
				Win_ShowWidget("UI.SysLoginMsgbox", false);
				Win_ShowWidget("UI.SysRegionDlg",false);
				
				--cmd_gohome();				
				UI.children.splashWgt._logintype = 1;
				UI.children.splashWgt._canStartNextStep = true;
				return 0;
			end;
		
			if result == eLoginRequireReg then
				Win_ShowWidget("UI.SysLoginMsgbox", false);
				Win_ShowWidget("UI.SysRegionDlg",false);

				--cmd_regist();
				UI.children.splashWgt._logintype = 0; 
				UI.children.splashWgt._canStartNextStep = true;
				return 0;
			end;
			
			if result == eLoginTimeout then 
				Win_SetText("UI.SysLoginMsgbox.sentence", "登陆超时");
			else
			end;
			Win_ShowWidget("UI.SysLoginMsgbox", true);
		end;
	end;
	
	OnResponseLogin2=function()
		if Param.reasonlen > 0 then
			Win_SetText("UI.SysLoginMsgbox.sentence", Param.reason);
			--Win_ShowWidget("UI.SysLoginMsgbox", true);
		end;			
		--[[if (not IsFinishNewHandTask(1)) then
			UI.children.startmenu.SetTutorial(true);
		else
			UI.children.startmenu.SetTutorial(false);
		end;]]
	end;
	children=
	{									
		sentence=
		{
			type="LABEL",
			rect = { 27,57,280,125 },
			textsize=12,
			caption="正在连接...",
			textstyle=dt_left+dt_vcenter,
			drawcolor={255,255,255,255},
			richmode=ui_richmode_color+ui_richmode_hlink,
			enable=1,		
			initlayer=0;
			pluslayer=0;
		},
		
		cancelbtn=
		{
			type="BUTTON",
			rect={130,210,90,40},
			bkimage="res/uires/common/cancelbtn.gsa",
			defaultcancel=1,
			
			OnClick=function()
				CancelLogin();
				Win_ShowWidget("UI.SysLoginMsgbox", false);
				ExitApp();
			end;
		},
		miniclosebtn=
		{
			type="BUTTON",
			rect={308,-12,25,25},
			bkimage="res/uires/common/miniclose.gsa",

			OnClick=function()
				CancelLogin();
				Win_ShowWidget("UI.SysLoginMsgbox", false);
			end;
		}
	},					 
};
			
UI.children.SysRegionDlg=
{
	type="MESSAGEBOX",
	rect = { 0, 0, 336, 199 },	
	captionrect = { 0, 0, 0, 0 },	
	bkimage="res/uires/sysdlg/msgbox.gsa",
	drawcolor={255,255,255,255},
	textsize=12,
	textfont=1,
	textstyle=dt_center,
	initlayer=msgbox_layer,
	visible=0,
	
	OnSelfShow=function()
		UI.children.SysRegionDlg.Refresh();
		Win_SetCapture("UI.SysRegionDlg");
	end;
	
	OnSelfHide=function()
		Win_SetCapture();
	end;
	
	Refresh=function()
		local loc_count,locs = GetLocationInfo();
		Win_SetValue("UI.SysRegionDlg.regionlist", loc_count,value_channel_item_num);
		
		for i = 1, loc_count do
			local text = locs[i].name;
			if locs[i].ping <= 0 then
				text = text.."(测试中...)";
			end;
			if locs[i].ping > 0 and locs[i].ping <= 100 then
				text = text.."(良好)";
			end;
			if locs[i].ping > 100 and locs[i].ping <= 500 then
				text = text.."(正常)";
			end;
			if locs[i].ping > 500 then
				text = text.."(繁忙)";
			end;
			Win_SetText("UI.SysRegionDlg.regionlist", text, value_channel_itemtext,i-1);
		end;
		
		if loc_count > 0 then
			Win_SetValue("UI.SysRegionDlg.regionlist", 0, value_channel_sel);
		end;
	end;
	
	children=
	{
		regionlist=
		{
			type="LISTBOX",
			rect={20,40,280,80},
			clientrect={0,0,280,80},
			itemheight=19,
			maxheight=80,
			style=wgtstyle_vertical+wgtstyle_self_xsize+wgtstyle_self_ysize,	
			extendstyle=listbox_style_auto_comfort+listbox_style_itemheight_fix+listbox_style_richtext,
			richmode=ui_richmode_all,
			itemtextstyle=dt_left+dt_vcenter+dt_magicedge;
			itemselbkcolor={200,0,0,100},
			itemfocuspos={0,0,0,0},
			itemselpos={0,0,0,0},
			itemdrawcolor={0,0,255,255},
			itemfocuscolor={0,0,200,255},
			itemselcolor={0,0,255,255},
			textsize=12,
			textfont=1,
			children=
			{
				vscroll=
				{
					type="SCROLLBAR",
					rect={0,0,17,115},
					captionrect={4,0,9,115},
					pattern=wgtpattern_rawvisible+wgtpattern_alphavisible,
					style=wgtstyle_vertical+wgtstyle_ajustfollow,					
					ajustflag=rect_flag_window+rect_flag_client+rect_flag_caption;
					bkcolor={0,0,0,128},
					edgecolor={128,128,128,255},						
					pagesize=8,
					marginh=3,
					marginv=0,
					aligntype=aligntype_father+aligntype_winrect,
					alignstyle=alignstyle_right_out+alignstyle_vcenter,	
					extendstyle=scrollbar_auto_hide,
					drawflag=drawflag_caption_fill,

					children=
					{
						blockbtn=
						{
							type="BUTTON",
							rect={0,0,17,50},
							bkimage="res/uires/public/BTN_scrollbar.gsa",
							bkimagepos={0,0},
							moveable=1,
							aligntype=aligntype_father+aligntype_clientrect,
							alignstyle=alignstyle_top_in+alignstyle_hcenter,	
							
						};--blockbtn
						
						spinup=
						{
							type="SPIN",
							rect={0,0,17,20},
							bkimage="res/uires/public/BTN_scrollup.gsa",
							style=wgtstyle_primary+wgtstyle_alignfollow,
							textstyle=1,		
							aligntype=aligntype_father+aligntype_winrect,
							alignstyle=alignstyle_top_out+alignstyle_hcenter,	
							marginh=0,
							marginv=0,							
							
						};--spinup
						
						spindown=
						{
							type="SPIN",
							rect={0,0,17,20},
							bkimage="res/uires/public/BTN_scrolldown.gsa",	
							style=wgtstyle_alignfollow,
							aligntype=aligntype_father+aligntype_winrect,
							alignstyle=alignstyle_bottom_out+alignstyle_hcenter,	
							marginh=0,
							marginv=0,
													
						};--spindown
						
					};--children
			
				};--scroll
			};
		};
		quicksel=
		{
			type="CHECK",
			rect={50,144,100,20},
			caption="自动分配区域",
			textsize=12,
			bkimage = "res/uires/common/checklabel.gsa",
			
			OnCheck=function(bcheck)
				if bcheck ~= 0 then
					Win_EnableWidget("UI.SysRegionDlg.regionlist", false);
				else
					Win_EnableWidget("UI.SysRegionDlg.regionlist", true);
				end;
			end;
		};
		OkBtn=
		{
			type="BUTTON",
			rect={265,171,40,20},
			bkimage="res/uires/common/affirmbtn.gsa",
			
			OnClick=function()
				local sel = Win_GetValue("UI.SysRegionDlg.regionlist", value_channel_sel);
				if Win_IsChecked("UI.SysRegionDlg.regionlist") then
					sel = 0;
				end;
				if sel < 0 then
					return;
				end;
				local loc_count,locs = GetLocationInfo();
				local svrcount, svrs=GetLocationServers(locs[sel+1].id);
				SelectRegion(locs[sel+1].id, svrs[1].id);
			end;
		};
	};
};
