UI.children.room = 
{
	type = "SCREEN",
	rect = {0,0,1024,768},	
	bkimage="res/uires/startmenu/carbarn_bk.gsa",
	visible=0,	
	autoscale = 1,
	updatemute = 1,
	
	OnInit = function()	
		Win_ShowWidget("UI.room",true);			
		--PlaySound(2);
		--cmd_shownavigation(0);
		return 1;			   
	end;	
	
	OnDenit = function()	
		Win_ShowWidget("UI.room",false);
		--StopSound(2);
		ClearBubble();
		--cmd_shownavigation(1);
		return 1;			   
	end;
	
	OnSelfShow = function()
		ShowPointer(true);
		cmd_shownavigation();
		cmd_setcamera();
		--播放背景音乐
		PlayMusic("Data/Win32/SOUNDS/ui_circle.mp3",1,1);
		Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.ReadyBtn",true);
		local chatpath="UI.room.MainFrame.ChatList";
		--local num = Win_GetValue(chatpath, value_channel_item_num);
		Win_SetValue(chatpath, 0, value_channel_item_num);
	end;
	
	OnSelfHide = function()
		--cmd_shownavigation();
	end;

	OnResponseChangeSeatState=function()
		if Param.resultid == 0 then
			local seatid=Param.seatid;
			local seatstate=Param.seatstate;
			local index=SeatToIndex(seatid);
			UI.children.room.children.MainFrame.children.PlayerListPanel.SetState(index, seatstate);
		end;
	end;
	
	OnResponseKickFromRoom=function()
		if Param.resultid == 0 then
			local uin=Param.uin;
			if uin == 0 then
				return;
			end;
			Win_ConsoleInfo("NotifyKick");
			for i=0,6,1 do
				local thisid="UI.room.MainFrame.PlayerListPanel.PlayerList"..i;
				local thisUin=Win_GetValue(thisid,value_channel_customindex);
				if thisUin == uin then
					Win_ConsoleInfo("Reset "..thisid);
					UI.children.room.children.MainFrame.children.PlayerListPanel.Reset(i);
				end;
			end;
		end;
	end;
	
	OnResponseChangeReadyState=function()
		if Param.resultid == 0 then
			local readystate=Param.readystate;
			
			local ready = state_unreadyed;
			if readystate==state_ready then
				ready=state_readyed;
			end;
			if readystate==state_unready then
				ready=state_unreadyed;
			end;

			UI.children.shortcut_dlg.children.roomAssociated_wgt.SetReady(ready);
			UI.children.room.children.MainFrame.children.PlayerListPanel.children.MySeat.SetReady(ready);
		end;
	end;
	
	OnNotifyEnterRoom=function()
		local seatid=Param.pinfo.seatid;
		local index=SeatToIndex(seatid);
		UI.children.room.children.MainFrame.children.PlayerListPanel.UpdateSeat(index);
	end;
	
	OnNotifyLeaveRoom=function()
		if Param.uin==GetMyInfo().uin then
			GotoUIScreen("roomsel");
		else
			UI.children.room.children.MainFrame.children.PlayerListPanel.ResetByUin(Param.uin);
			local changeowner=Param.changeroomer;
			if changeowner ~= 0 then
				UI.children.shortcut_dlg.children.roomAssociated_wgt.children.ReadyBtn.Refresh();
				UI.children.room.children.MainFrame.children.PlayerListPanel.children.MySeat.Refresh();
				local newownerseatid=PIDToSeat(Param.newroomerid);
				if newownerseatid ~= nil then
					local index=SeatToIndex(newownerseatid);
					UI.children.room.children.MainFrame.children.PlayerListPanel.SetOwner(index,1);
				end;
			end;
		end;
	end;
	
	OnNotifyChangeSeatState=function()
		local seatid=Param.seatid;
		local seatstate=Param.seatstate;
		local index=SeatToIndex(seatid);
		UI.children.room.children.MainFrame.children.PlayerListPanel.SetState(index, seatstate);
	end;
	
	OnNotifyKickFromRoom=function()
		if Param.uin == GetMyInfo().uin then
			GotoUIScreen("roomsel");
		else			
			for i=0,6,1 do
				local thisid="UI.room.MainFrame.PlayerListPanel.PlayerList"..i;
				local thisUin=Win_GetValue(thisid,value_channel_customindex);
				if thisUin==Param.uin then					
					UI.children.room.children.MainFrame.children.PlayerListPanel.Reset(i);
				end;
			end;
		end;
	end;
	
	OnNotifyChangeReadyState=function()
		local seatid=UinToSeat(Param.uin);
		local readystate=Param.readystate;
		local index=SeatToIndex(seatid);
		
		local ready = state_unreadyed;
		if readystate==state_ready then
			ready=state_readyed;
		end;
		if readystate==state_unready then
			ready=state_unreadyed;
		end;
		UI.children.room.children.MainFrame.children.PlayerListPanel.SetReady(index, ready);
	end;
	
	OnNotifyChangeMap=function()
		UIGlobal._MapIDSelected = Param.mapid;
		UI.children.room.children.MainFrame.children.PlayerListPanel.children.MapPanel.Update();
		UI.children.room.children.MapSelDlg.Update();
	end;
	
	OnResponseChangeMap=function()
		UIGlobal._MapIDSelected = Param.mapid;
		UI.children.room.children.MainFrame.children.PlayerListPanel.children.MapPanel.Update();
		UI.children.room.children.MapSelDlg.Update();
	end;

	OnNotifyPlayerChangeTeam=function()
		local seatid=UinToSeat(Param.uin);
		local index=SeatToIndex(seatid);
		UI.children.room.children.MainFrame.children.PlayerListPanel.UpdateSeat(index);
	end;
	
	OnResponseChangeTeam=function()
		UI.children.room.children.MainFrame.children.PlayerListPanel.children.MySeat.Refresh();
	end;
	
	OnNotifyChangeRoomOwner=function()
		UI.children.shortcut_dlg.children.roomAssociated_wgt.Init();
		UI.children.room.children.MainFrame.children.PlayerListPanel.children.MySeat.Refresh();
		local newownerseatid=PIDToSeat(Param.roomerid);
		if newownerseatid ~= nil then
			local index=SeatToIndex(newownerseatid);
			UI.children.room.children.MainFrame.children.PlayerListPanel.SetOwner(index,1);
		end;
	end;
	
	OnNotifyGameBegin=function()
		RemoveSelfKartFromUIScene("ROOT");
		RemoveSelfRoleFromUIScene("playermode");
		ReBindSelfPlayer();
		--SelfPlayAnimation(1102);
	end;
	
	OnNotifyChangeItemStatus=function(uin)
		PlaceRemotePlayerInRoom(uin, -30.0);
	end;
	
	OnTimer=function()
		SelfPlayAnimation(1104);
		BindSelfRoleToScene("playermode");
		BindSelfKartToScene("ROOT");	
	end;

	children = 
	{
		MainFrame=
		{
			type="MESSAGEBOX",
			rect={10,10,1000,740},
			bkimagepos={-5,-10},
			moveable=0,
			
			children=
			{			
				PlayerListPanel=
				{
					type="WIDGET",
					rect={87,117,820,460},
					template={"res/uires/playerlisttempl.lua"},
					
					children=
					{
						MySeat=
						{
							type="WIDGET",
							rect={750,0,160,260},
							hosttype=6,
							hostindex=-1,
							initlayer = 0,
							style=wgtstyle_renderitem,
							drawflag=drawflag_clientprior,
							enable = 0,

							OnInit=function()
								UI.children.room.children.MainFrame.children.PlayerListPanel.children.MySeat.Refresh();
							end;
							
							Refresh=function()
								--Win_ConsoleInfo("刷新我的信息");
								local myinfo=GetMyInfo();
								local myid="UI.room.MainFrame.PlayerListPanel.MySeat";
								local myidfull="UI.children.room.children.MainFrame.children.PlayerListPanel.children.MySeat";
								
								if myinfo.isroomer==1 then
									Win_SetBackPic(myid..".OwnerLabel","res/uires/room/owner_l.gsa");
								end;
								if myinfo.isroomer == 0 then
									Win_SetBackPic(myid..".OwnerLabel","");
								end;
								
								--Win_ConsoleInfo("我的等级是:"..myinfo.level);
								--暂时屏蔽等级
								--[[
								local level = GetPlayerLevelByExp(myinfo.playerdbinfo.exp);
								if (level ) then
									Win_SetBackPic(myid..".LevelLabel", leveldesc[level-1].icon);
								else
									Win_ConsoleInfo("等级为空");
								end;
								]]
								
								local ready=GetMyRoomInfo().readystate;
								
								--Win_ConsoleInfo("我的ReadyState:"..ready);
								cmd_dostring(myidfull..".SetReady("..ready..")");
							end;
							
							SetReady=function(ready)
								--Win_ConsoleInfo("设置我的准备状态");
								--Win_ConsoleInfo("ReadyState:"..ready);
								
								local tmap=	{	
											[state_readyed]="res/uires/room/selfprepare.gsa",
											[state_unreadyed]= "",
										};
								Win_SetBackPic("UI.room.MainFrame.PlayerListPanel.MySeat.ReadyLabel",tmap[ready]);
								
							end;
							
							children=
							{
								LevelLabel=
								{
									type="LABEL",
									rect={12,2,15,15},
									visible = 0,
								};
								OwnerLabel=
								{
									type="LABEL",
									rect={80,2,41,71},
								};
								ReadyLabel=
								{
									type="LABEL",
									rect={-150,470,135,42},
									--visible = 0,
								};
							};
						};
						
						MapPanel=
						{
							type="WIDGET",
							rect={482,2,157,256},
							bkimage="res/uires/room/mapiconbk2.gsa",
							--enable=0,
							
							OnInit=function()
								local t=GetMyRoomInfo();
								UIGlobal._MapIDSelected = t.mapid;
								UI.children.room.children.MainFrame.children.PlayerListPanel.children.MapPanel.Update();
								Win_EnableWidget("UI.room.MainFrame.PlayerListPanel.MapPanel.MapThumb", false);
							end;
							
							Update=function()
								local m=FindMap(UIGlobal._MapIDSelected);
								Win_SetText("UI.room.MainFrame.PlayerListPanel.MapPanel.NameLabel",m.id);
								Win_SetBackPic("UI.room.MainFrame.PlayerListPanel.MapPanel.MapThumb",m.thumb2);
								
								Win_ShowWidget("UI.room.MainFrame.PlayerListPanel.MapPanel.MapThumb.lv",false);
								if m.lv~=nil and m.lv~=0 then
									Win_ShowWidget("UI.room.MainFrame.PlayerListPanel.MapPanel.MapThumb.lv",true);
									
									local id="UI.room.MainFrame.PlayerListPanel.MapPanel.MapThumb.lv.dot";
									local rc=Win_GetRect(id);
									Win_SetRectAll(id,rc.x,rc.y,0+11.4*m.lv,10);
									--Win_SetRect(id, rc.x, rc.y, 141, 13);
								end;

							end;
							
							children=
							{
								NameLabel=
								{
									type="LABEL",
									rect={13,8,130,18},
									textsize=12,
								};
								MapThumb=
								{
									type="BUTTON",
									rect={4,30,149,221},
									autoscale = 1,
									enable = 0,
									
									OnClick = function()
										cmd_mapsel();
									end;
									
									children=
									{
										lv=
										{
											type="LABEL";
											rect={4,0,141,13},
											bkimage="res/uires/mapsel/maplv2.gsa";
											--autoscale = 1,
											
											
											children=
											{
												dot=
												{
													type="LABEL";
													rect={28,2,114,10},
													bkimage="res/uires/mapsel/maplvdot2.gsa";
													style=wgtstyle_clipself;
													--autoscale = 1,
												};
											};
										};

									};
								};
							};
						};
					};				
					
					OnInit=function()
						UI.children.room.children.MainFrame.children.PlayerListPanel.FillList();
					end;
					
					FillList=function()
						Win_ClearTemplates("UI.room.MainFrame.PlayerListPanel");
						Win_AddTemplate("UI.room.MainFrame.PlayerListPanel",0,0,6);
						for i=0,6,1 do
							UI.children.room.children.MainFrame.children.PlayerListPanel.UpdateSeat(i);
						end;
					end;
					
					SetState=function(index, state)
						local myid="UI.room.MainFrame.PlayerListPanel.PlayerList"..index;
						local myidfull="UI.children.room.children.MainFrame.children.PlayerListPanel.children.PlayerList"..index;
						cmd_dostring(myidfull..".SetState("..state..")");
					end;
					
					SetReady=function(index, ready)
						local myid="UI.room.MainFrame.PlayerListPanel.PlayerList"..index;
						local myidfull="UI.children.room.children.MainFrame.children.PlayerListPanel.children.PlayerList"..index;
						cmd_dostring(myidfull..".SetReady("..ready..")");
					end;
					
					Reset=function(index)
						local myid="UI.room.MainFrame.PlayerListPanel.PlayerList"..index;
						local myidfull="UI.children.room.children.MainFrame.children.PlayerListPanel.children.PlayerList"..index;
						cmd_dostring(myidfull..".Reset()");
					end;
					
					ResetByUin=function(Uin)
						--此时不能用UinToSeat了，因为内核信息被删除了
						for i=0,6,1 do
							local myid="UI.room.MainFrame.PlayerListPanel.PlayerList"..i;
							local thisUin=Win_GetValue(myid,value_channel_customindex);
							if thisUin == Uin then
								UI.children.room.children.MainFrame.children.PlayerListPanel.Reset(i);
								return;
							end;
						end;
					end;
					
					UpdateSeat=function(index)
						local myid="UI.room.MainFrame.PlayerListPanel.PlayerList"..index;
						local myidfull="UI.children.room.children.MainFrame.children.PlayerListPanel.children.PlayerList"..index;
						cmd_dostring(myidfull..".UpdateSeat()");
					end;
					
					SetOwner=function(index,set)
						local myid="UI.room.MainFrame.PlayerListPanel.PlayerList"..index;
						local myidfull="UI.children.room.children.MainFrame.children.PlayerListPanel.children.PlayerList"..index;
						cmd_dostring(myidfull..".SetOwner("..set..")");
					end;
				};
				ChatEdit=
				{
					type="EDIT",
					rect={143,710,700,20},
					bkcolor={100,100,100,255},
					drawflag=drawflag_win_fill,					
					maxchar=150,
					
					OnEnter=function()
						UI.children.room.children.MainFrame.children.ChatEdit.children.SendBtn.OnClick();
					end;
					
					children=
					{
						SendBtn=
						{
							type="BUTTON",
							rect={700,0,40,20},
							caption="Send",
							textfont=1,
							
							OnClick=function()
								local text=Win_GetText("UI.room.MainFrame.ChatEdit");
								if text=="" then
									return 0;
								end;
								Win_SetText("UI.room.MainFrame.ChatEdit","");
								Talk(0,1,text);
								TalkFilter(text);--聊天表情
								--气泡
								local id="UI.room.MainFrame.PlayerListPanel.MySeat";
								local rc = Win_GetRect(id);
								AddBubble(GetMyInfo().uin,text,rc.x+10,rc.y+5,10000,12,0,4278190080,150);
								Win_SetText("UI.room.MainFrame.ChatList", GetMyInfo().playerdbinfo.nickname..":"..text, value_channel_itemtext, -1);
							end;
						},
					};
				};
				
				ChatList=
				{
					type = "TEXTLIST",
					rect = { 143, 643, 700, 60},
					style=wgtstyle_ajustfollow+wgtstyle_cleanwin,--wgtstyle_clipself+
					extendstyle=textlist_style_autoscroll,
					scrollspeed=100000,
					rowspace=6,
					textsize=12,
					textstyle=dt_left+dt_magicedge,
					ajustflag=rect_flag_window+rect_flag_client;
					magiccolor={0,0,0,128},
					drawcolor={0,0,0,255},
					linkcolor={100,100,255,255},
					mousedelta=18,
					richmode=ui_richmode_all,
					richflag=1,
					--initlayer=msgbox_layer-2,

					children=
					{
						vscroll=
						{
					
							type="SCROLLBAR",
							rect={0,0,17,50},
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
							
							OnEdgeAjust=function(x,y,w,h)
								Win_ReAlignChild("UI.chatbar.chatpanel.chatlist.vscroll");
							end;
							
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
									style=wgtstyle_primary,
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
				
			};
		};
		

		MapSelDlg=
		{
			type="MESSAGEBOX",
			rect={0,0,693,453},
			bkimage="res/uires/mapsel/bk.gsa",
			initlayer=msgbox_layer-1,			
			visible = 0,
			style=wgtstyle_topmost+wgtstyle_renderbuffer;
			drawflag = drawflag_backprior,
			custombuffer = 1,
			buffervalue =
			{
				banimation = 1,
				cycletype = 2, 
				restype = 1, 
				resname="scene/splash.nif",			
			},
			_mapcat=0;
			_mapindex=0;
			
			OnInit=function()
				Win_ShowWidget("UI.room.MapSelDlg", false);
				--UI.children.room.children.MapSelDlg.Update();
			end;
			OnSelfShow=function()
				Win_SetCapture("UI.room.MapSelDlg");
				UI.children.room.children.MapSelDlg.Update();
			end;
			
			OnSelfHide=function()
				Win_SetCapture();
			end;
			
			Update=function(innerupdate)
				local mapid=UIGlobal._MapIDSelected;
				if innerupdate ~= nil then
					local mapcat=UI.children.room.children.MapSelDlg._mapcat;
					local mapindex=UI.children.room.children.MapSelDlg._mapindex;
					local t=FindMapByCatIndex(mapcat, mapindex);
					mapid=t.mapid;
				end;
				local m=FindMap(mapid);
				Win_SetText("UI.room.MapSelDlg.GModeLabel",m.id);
				Win_SetBackPic("UI.room.MapSelDlg.ThumbView",m.thumb);
				Win_ShowWidget("UI.room.MapSelDlg.ThumbView.lv",false);
				if m.lv~=nil and m.lv~=0 then
					Win_ShowWidget("UI.room.MapSelDlg.ThumbView.lv",true);
					local id="UI.room.MapSelDlg.ThumbView.lv.dot";
					local rc=Win_GetRect(id);
					Win_SetRectAll(id,rc.x,rc.y,50+22.3*m.lv,30);

				end;
				
				if innerupdate == nil then
					local catid="UI.room.MapSelDlg.TabBar.MapSelCat"..m.cat;
					local bCheck = Win_IsChecked(catid);
					if bCheck ~= true then
						Win_SelectSelf(catid);
					end;
				end;
			end;
			
			OnOk=function()
				local mapcat=UI.children.room.children.MapSelDlg._mapcat;
				local mapindex=UI.children.room.children.MapSelDlg._mapindex;
				local t=FindMapByCatIndex(mapcat, mapindex);
				local mapid=t.mapid;
				ChangeMap(mapid);
			end;
			
			children=
			{
				miniclosebtn=
				{
					type="BUTTON",
					rect={659,-3,40,44};
					bkimage="res/uires/common/miniclose.gsa",
					defaultcancel=1,
					soundid={1007},				
				},	
				TabBar=
				{
					type="WIDGETLIST",
					rect={86,40,530,50},
					style=wgtstyle_clipchild_window,
					extendstyle=wgtlist_style_enum_item;
					template={"res/uires/mapselcattempl.lua"},
					
					OnInit=function()
						local count=table.getn(mapscat);
						Win_ClearTemplates("UI.room.MapSelDlg.TabBar");
						Win_AddTemplate("UI.room.MapSelDlg.TabBar", 0, 0, count-1);
					end;
					children=
					{
					};
				};
				PrevBtn=
				{
					type="BUTTON",
					rect={35,48,50,30},
					bkimage="res/uires/roomsel/prevbtn.gsa",
					
					OnClick=function()
						local myid="UI.room.MapSelDlg.TabBar";
						local sel=Win_GetValue(myid, value_channel_sel);
						Win_SetValue(myid, sel-1,value_channel_sel);
					end;
				};
				NextBtn=
				{
					type="BUTTON",
					rect={615,48,50,30},
					bkimage="res/uires/roomsel/nextbtn.gsa",
					
					OnClick=function()
						local myid="UI.room.MapSelDlg.TabBar";
						local sel=Win_GetValue(myid, value_channel_sel);
						Win_SetValue(myid, sel+1,value_channel_sel);
					end;
				};
				GModeLabel=
				{
					type="LABEL",
					rect={58,116,332,30},
					caption="游戏模式",
					textfont=1,
				};
				ThumbView=
				{
					type="LABEL",
					rect={71,162,310,175},
					autoscale=1,
					
					children=
					{
						lv=
						{
							type="LABEL";
							rect={5,0,300,30},
							bkimage="res/uires/mapsel/maplv.gsa";
							
							
							children=
							{
								dot=
								{
									type="LABEL";
									rect={0,0,10,10},
									bkimage="res/uires/mapsel/maplvdot.gsa";
									style=wgtstyle_clipself,
								};
							};
						};
					};
				};
				MapListPanel=
				{
					type="WIDGET",
					rect={429,97,220,310},
					
					OnInit=function()
						UI.children.room.children.MapSelDlg.children.MapListPanel.Update();
					end;
					
					Update=function()
						local mapcat=UI.children.room.children.MapSelDlg._mapcat;
						local mapcount=QueryMapCount(mapcat);
						Win_ClearTemplates("UI.room.MapSelDlg.MapListPanel.list");
						--Win_AddTemplate("UI.room.MapSelDlg.MapListPanel.list",0,0,mapcount-1);
						Win_SetValue("UI.room.MapSelDlg.MapListPanel.list", mapcount, value_channel_item_num);
					end;
					
					children=
					{
						list=
						{
							type="WIDGETLIST",							
							rect={5,8,190,305},
							clientrect={10,10,200,290};
							style=wgtstyle_clipchild_window,							
							itemmarginh=1,
							itemmarginv=1,
							template={"res/uires/maplisttemp.lua"},							
							primaryscroll="UI.room.MapSelDlg.MapListPanel.vscroll",
							
							children=
							{
							};
						};
						vscroll=
						{
							type="SCROLLBAR",
							rect={0,0,25,280},
							pattern=wgtpattern_rawvisible+wgtpattern_alphavisible,
							style=wgtstyle_vertical+wgtstyle_ajustfollow,					
							ajustflag=rect_flag_window+rect_flag_client+rect_flag_caption;							
							bkcolor={0,0,0,128},
							edgecolor={128,128,128,255},								
							pagesize=8,
							marginh=3,
							marginv=0,
							aligntype=aligntype_father+aligntype_winrect,
							alignstyle=alignstyle_right_in+alignstyle_vcenter,	
							--extendstyle=scrollbar_auto_hide,
							buddywidget="UI.room.MapSelDlg.MapListPanel.list",
							enable = 1,
							
							children=
							{
								blockbtn=
								{
									type="BUTTON",
									rect={0,0,25,25},
									framescheme={{1,0,0,0}},
									bkimage="res/uires/common/scroll_block.gsa",
									bkimagepos={0,0},
									moveable=1,
									aligntype=aligntype_father+aligntype_clientrect,
									alignstyle=alignstyle_top_in+alignstyle_hcenter,	
									
								};--blockbtn
								
			
								spinup=
								{
									type="SPIN",
									rect={0,0,25,20},
									bkimage="res/uires/common/scroll_up.gsa",
									style=wgtstyle_primary,
									textstyle=1,		
									aligntype=aligntype_father+aligntype_winrect,
									alignstyle=alignstyle_top_out+alignstyle_hcenter,	
									marginh=0,
									marginv=0,							
									
								};--spinup
								
								spindown=
								{
									type="SPIN",
									rect={0,0,25,20},
									bkimage="res/uires/common/scroll_down.gsa",	
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
				OkBtn=
				{
					type="BUTTON",
					rect={200,450,100,40},
					bkimage="res/uires/common/okbtn.gsa",
					
					OnClick=function()
						Win_EndWidget("UI.room.MapSelDlg", true);
					end;
				};
				CancelBtn=
				{
					type="BUTTON",
					rect={370,450,100,40},
					bkimage="res/uires/common/cancelbtn.gsa",
					
					OnClick=function()
						Win_EndWidget("UI.room.MapSelDlg", false);
					end;
				};
			};
		};
	};
};
