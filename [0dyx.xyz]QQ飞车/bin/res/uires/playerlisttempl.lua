PlayerList_UIID_=
{
	type="BUTTON",
	rect={0,0,155,255},
	clientrect={0,0,155,255},
	groupstop=_UIID_,
	style=wgtstyle_template,
	bkimage="res/uires/room/seatbk.gsa",
	widget_layer = normal_layer_init + focus_layer_plus + 1,
	
	OnInit=function()
		local rcParent=Win_GetRect("UI.room.MainFrame.PlayerListPanel");
		local row=math.floor(_UIID_/3);
		local col=math.floor(_UIID_-row*3);
		if row == 2 then
			row = 1;
			col=3;
		end;
		
		local x=rcParent.x+0+col*160;
		local y=rcParent.y+4+row*260;
		local myid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		Win_SetRect(myid,x,y,155,255);		
	end;
	
	OnDeinit = function()
		Win_ConsoleInfo("Hide RemotePlayerss");
		local seatid=IndexToSeat(_UIID_);
		if seatid == nil then
			Win_ConsoleInfo("玩家没有了1");
			return;
		end;
		local pi=GetRoomPlayerInfo(seatid);
		Win_ConsoleInfo("玩家1");
		if pi ~= nil then --玩家还在
			Win_ConsoleInfo("玩家没有了2");
			UnPlaceRemotePlayerInRoom(pi.uin);
		end;
	end;
	
	SetName=function(name)
		local thisid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		Win_SetText(thisid..".NameLabel",name);
	end;
	--暂且屏蔽SetLevel功能
	SetLevel=function(level)
		local thisid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		--Win_SetBackPic(thisid..".LevelLabel",leveldesc[level].Icon);
		Win_SetText(thisid..".LevelLabel", "lv."..level);
	end;
	
	SetReady=function(ready)
		--Win_ConsoleInfo("设置位置的准备状态");
		--Win_ConsoleInfo("index:".._UIID_);
		--Win_ConsoleInfo("ReadyState:"..ready);
		
		local tmap=	{	
					[state_readyed]="res/uires/room/ready.gsa",
					[state_unreadyed]= "",
				};
		local thisid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		Win_SetBackPic(thisid..".MainLabel.playerview_wgt.ReadyLabel",tmap[ready]);	
	end;

	SetState=function(state)
		--Win_ConsoleInfo("设置位置的开关状态");
		--Win_ConsoleInfo("index:".._UIID_);
		--Win_ConsoleInfo("SeatState:"..state);
		
		local tmap=	{	
					[seat_closed]="res/uires/room/closedbk.gsa",
					[seat_free]="",
					[seat_used]="",
				};
		local thisid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		Win_SetBackPic(thisid..".MainLabel",tmap[state]);
	end;

	SetOwner=function(set)
		local myid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		if set == 1 then
			--Win_ConsoleInfo("设置房主");
			--Win_ConsoleInfo("index:".._UIID_);
			Win_SetBackPic(myid..".OwnerLabel","res/uires/room/owner.gsa");
			Win_SetBackPic(myid..".MainLabel.playerview_wgt.ReadyLabel","");
		end;
		if set == 0 then
			--Win_ConsoleInfo("设置非房主");
			--Win_ConsoleInfo("index:".._UIID_);
			Win_SetBackPic(myid..".OwnerLabel","");
		end;
	end;
	
	Reset=function()
		local myid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		Win_SetValue(myid,0,value_channel_customindex);
		Win_SetValue(myid..".MainLabel.playerview_wgt",0,value_channel_hostindex);--准备画头像
		Win_SetText(myid..".NameLabel", "");
		--Win_SetBackPic(myid..".LevelLabel","");
		Win_SetText(myid..".LevelLabel","");
		Win_SetBackPic(myid..".MainLabel","");
		Win_SetBackPic(myid..".MainLabel.playerview_wgt.ReadyLabel","");
		Win_SetBackPic(myid..".OwnerLabel","");
	end;
	
	UpdateSeat=function()
		local myid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
		local myidfull="UI.children.room.children.MainFrame.children.PlayerListPanel.children.PlayerList".._UIID_;
		local seatid=IndexToSeat(_UIID_);
		if seatid == nil then
			return;
		end;
		
		local pi=GetRoomPlayerInfo(seatid);
		if pi == nil then --玩家离开了
			cmd_dostring(myidfull..".Reset()");
			--是否关闭
			local seatstatus=GetMyRoomInfo().seatstatus[seatid+1];
			cmd_dostring(myidfull..".SetState("..seatstatus..")");
			
		else
			--Win_ConsoleInfo(pi.nickname);
			Win_SetValue(myid,pi.uin,value_channel_customindex);
			Win_SetValue(myid..".MainLabel.playerview_wgt",pi.uin,value_channel_hostindex);--准备画头像
			PlaceRemotePlayerInRoom(pi.uin, -30.0);
			cmd_dostring(myidfull..".SetName('"..pi.playerdbinfo.nickname.."')");
			local level = GetPlayerLevelByExp(pi.playerdbinfo.exp);
			cmd_dostring(myidfull..".SetLevel("..level..")");
			
			if GetMyRoomInfo().roomerid==pi.pid then
				cmd_dostring(myidfull..".SetOwner(1)");--标志队长
			else
				cmd_dostring(myidfull..".SetOwner(0)");
			end;
			
			cmd_dostring(myidfull..".SetReady("..pi.status..")");
		end;
		
	end;
	
	children=
	{
		--等级图标
		LevelLabel=
		{
			type="LABEL",
			rect={0, 238,40,16},
			--captionrect = {0, 3, 60, 16},
			textsize=12,
			textstyle=dt_center+dt_vcenter,
		};	
		--昵称栏
		NameLabel=
		{
			type="BUTTON",
			rect={50,238,85,16},
			--captionrect = {0, 3, 85, 16},
			textsize=12,
			textstyle=dt_center+dt_vcenter,
		};
		--个人信息按钮
		InfoBtn=
		{
			type="BUTTON",
			rect={135,233,20,20},
			bkimage="res/uires/room/infobtn.gsa",
			
			OnClick=function()				
				local myid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;				
				local thisUin=Win_GetValue(myid, value_channel_customindex);
				
				if thisUin ~= 0 then
					cmd_lookat(thisUin);
				end;
			end;
		};
		--关闭按钮
		CloseBtn=
		{
			type="BUTTON",
			rect={135,0,25,25},
			bkimage="res/uires/room/closebtn.gsa",
			
			OnClick=function()
				local myid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
				local Uin=Win_GetValue(myid,value_channel_customindex);
				if GetMyInfo().isroomer == 1 then
					Kick(Uin);
				end;
			end;
		};		
		--队长标志
		OwnerLabel=
		{
			type="LABEL",
			rect={113,-3,20,35},
			initlayer = msgbox_layer-1,
		};
		
		--主要用于座位关闭时的状态
		MainLabel=
		{
			type="LABEL",
			rect={0,46,155,180},
			bkimagepos = {-10, 0},
			drawflag=drawflag_clientprior,
			
			OnClick=function()
				local myid="UI.room.MainFrame.PlayerListPanel.PlayerList".._UIID_;
				local Uin=Win_GetValue(myid,value_channel_customindex);
				local seatid=IndexToSeat(_UIID_);
				--[[if GetMyInfo().isroomer == 1 and uin ~= 0 then
					Kick(Uin);
				end;	]]			
				if GetMyInfo().isroomer == 1 and Uin==0 then
					ToggleSeatState(seatid);
				end;								
			end;
			
			children=
			{
				playerview_wgt=
				{
					type = "LABEL",
					rect = {-25, -40, 180, 240},
					style=wgtstyle_renderitem,
					hosttype=1,
					hostindex=0,					
					enable = 0,
					--visible = 0,
					
					children = 
					{
						ReadyLabel=
						{
							type="LABEL",
							rect={43,197,110,20},
						};
					},
				},
				
			};
		};
	};
};
