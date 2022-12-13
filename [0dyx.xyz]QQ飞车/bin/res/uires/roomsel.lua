channellist = 
{
	[0] = {id = 0, name = "练习频道", levellow = 0, levelhigh = 0,tabcover = "res/uires/roomsel/channel/channel_0.gsa"};
	[1] = {id = 1, name = "新手频道", levellow = 1, levelhigh = 20,tabcover = "res/uires/roomsel/channel/channel_1.gsa"};
	[2] = {id = 2, name = "初级频道", levellow = 21, levelhigh = 40,tabcover = "res/uires/roomsel/channel/channel_2.gsa"};
	[3] = {id = 3, name = "中级频道", levellow = 41, levelhigh = 60,tabcover = "res/uires/roomsel/channel/channel_3.gsa"};
	[4] = {id = 4, name = "高级频道", levellow = 61, levelhigh = 80,tabcover = "res/uires/roomsel/channel/channel_4.gsa"};
	[5] = {id = 5, name = "专业频道", levellow = 81, levelhigh = 100,tabcover = "res/uires/roomsel/channel/channel_5.gsa"};
	[6] = {id = 6, name = "职业频道", levellow = 101, levelhigh = 160,tabcover = "res/uires/roomsel/channel/channel_6.gsa"};
	[7] = {id = 7, name = "自由频道", levellow = 1, levelhigh = 160,tabcover = "res/uires/roomsel/channel/channel_7.gsa"};	
	--[0] = {name = "初级频道", mark_low = 0, mark_high = 1000,};
	curselectedchannel = nil;
};

myproperchannellist = nil;

GetProperChannelList = function(level)
	local properchannellist = {};
	local mostpropchannel = nil;
	local i = -1;
	for ele in channellist do
		i=ele;
		if (level >= channellist[ele].levellow and level <= channellist[ele].levelhigh) then
			mostpropchannel = channellist[ele];
			break;
		end;
	end;
	
	local num = 0;
	if (i-1>=1) then		
		properchannellist[num] = channellist[i-1];
		num = num+1;
	end;
	properchannellist[num] = channellist[i];
	num = num+1;
	if (i+1 <= 6) then
		properchannellist[num] = channellist[i+1];
		num = num+1;
	end;
	
	if (level >= 1) then
		properchannellist[num] = channellist[7];
		num = num+1;		
	end;
	
	return properchannellist, num;
end;

UI.children.roomsel = 
{
	type = "SCREEN",
	rect = { 0, 0, 1024, 768},	
	bkimage="res/uires/startmenu/carbarn_bk.gsa",
	autoscale=1,
	visible=0,	
	updatemute = 1,
	
	OnInit = function()	
		Win_ShowWidget("UI.roomsel",true);
		
		--Win_ShowWidget("UI.taskbar",true);
		--cmd_shownavigation(0);
		return 1;			   
	end;	

	OnDenit = function()	
		Win_ShowWidget("UI.roomsel",false);
		--cmd_shownavigation(1);
		--Win_ShowWidget("UI.taskbar",false);
		return 1;			   
	end;
	
	OnSelfShow = function()
		cmd_shownavigation();
		cmd_setcamera();
	end;
	
	OnSelfHide = function()
		--cmd_shownavigation();
	end;
	
	OnResponseGetRoomList=function()
		if Param.resultid==0 then
			UI.children.roomsel.children.MainFrame.children.RoomListPanel.FillList(0);
		end;
	end;
	
	OnResponseLogin2=function()
		if Param.logintype==logintype_multi then
			Win_ConsoleInfo("OnResponseLogin2");
			local page = UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno;
			local npp=UI.children.roomsel.children.MainFrame.children.RoomListPanel._npp;
			GetRoomList(gmodebase_rand, gmodesub_rand, page*npp,npp,1);
		end;
	end;
	
	children=
	{
		testbtn=
		{
			type="BUTTON",
			rect={0,0,100,50},
			
			OnClick=function()
				
			end;
		};
		SelfShow_Wgt=
		{
			type = "WIDGET",
			rect = {0, 0, 0, 0},
			hosttype=6,
			hostindex=-1,
			initlayer = 0,
			style=wgtstyle_renderitem,
		},
		MainFrame=
		{
			type="WIDGET",
			rect={26,76,1000,700},
			initlayer = 2000,
			
			children=
			{
				--[[Toolbar=
				{
					type="WIDGET",
					rect={-15,50,100,660},
					
					children=
					{
						returnbtn=
						{
							type="BUTTON",
							rect={0,522,100,100},
							bkimage="res/uires/startmenu/returnbtn.gsa",
							
							OnClick=function()
								cmd_goback();
							end;
						};
					};
				};]]
				

				
				FindPlayerDlg=
				{
					type="MESSAGEBOX",
					rect={0,0,369,284},
					bkimage="res/uires/roomsel/find/findbk.gsa",
					initlayer=msgbox_layer-1,
					visible=0,
					
					style=wgtstyle_topmost+wgtstyle_renderbuffer;
					--drawflag = drawflag_clientprior,
					drawflag = drawflag_backprior,
					visible=0,
					defaultendok=0,
					alphaspeed=0.25,
					alphafactor=0.0,
		
					custombuffer = 1,
					buffervalue =
					{
						banimation = 1,
						cycletype = 2, 
						restype = 1, 
						resname="scene/splash.nif",			
					},
					
					OnSelfShow=function()
						Win_SetCapture("UI.roomsel.MainFrame.FindPlayerDlg");
						Win_SetText("UI.roomsel.MainFrame.FindPlayerDlg.idedit", "");
					end;
					
					OnSelfHide=function()
						Win_SetCapture();
					end;
					
					children=
					{
						miniclosebtn=
						{
							type="BUTTON",
							rect={336, -8, 40,44};
							bkimage="res/uires/common/miniclose.gsa",
							defaultcancel=1,
							soundid={1007},				
						},	
						ideditname = 
						{
							type="EDIT",
							rect={72,71,223,20},
							textstyle=dt_left,
							editmethod = ui_editmethod_english+ui_editmethod_chinese+ui_editmethod_number,	
							textfont=1,
							maxchar=20;
							tiptext="请输入玩家昵称",	
						},
						idedit=
						{
							type="EDIT",
							rect={72,126,223,20},
							textstyle=dt_left,
							editmethod = ui_editmethod_number,	
							textfont=1,
							maxchar=20;
							tiptext="请输入QQ号",		
						};
						
						okbtn=
						{
							type="BUTTON",
							rect={143,240,100,40},
							bkimage="res/uires/common/okbtn.gsa",
							
							OnClick=function()
								local uin=Win_GetText("UI.roomsel.MainFrame.FindPlayerDlg.idedit");
								if uin~="" then
									FindPlayer(tonumber(uin));
								end;
								Win_EndWidget("UI.roomsel.MainFrame.FindPlayerDlg",true);
							end;
						};
					};
				};
				InputPwdDlg=
				{
					type="MESSAGEBOX";
					rect={0,0,339,146},
					bkimage="res/uires/roomsel/inputpwdbk.gsa",
					initlayer=msgbox_layer-1,
					visible=0,
					
					_roomid=0;
					
					OnInit=function()
					end;
					
					Show=function(roomid)
						UI.children.roomsel.children.MainFrame.children.InputPwdDlg._roomid=roomid;
						Win_ShowWidget("UI.roomsel.MainFrame.InputPwdDlg",true);
					end;
					
					OnSelfShow=function()
						Win_SetCapture("UI.roomsel.MainFrame.InputPwdDlg");
						Win_SetFocus("UI.roomsel.MainFrame.InputPwdDlg.pwdedit");
						Win_SetText("UI.roomsel.MainFrame.InputPwdDlg.pwdedit","");
					end;
					
					OnSelfHide=function()
						Win_SetCapture();
					end;
					
					children=
					{
						pwdedit=
						{
							type="EDIT";
							rect={55,65,225,20},
							editmethod = ui_editmethod_english+ui_editmethod_chinese+ui_editmethod_number+ui_editmethod_symbol+ui_editmethod_coder,	
						};
						okbtn=
						{
							type="BUTTON";
							rect={130,120,100,40},
							bkimage="res/uires/common/okbtn.gsa",
							
							OnClick=function()
								local roomid=UI.children.roomsel.children.MainFrame.children.InputPwdDlg._roomid;
								local pwd=Win_GetText("UI.roomsel.MainFrame.InputPwdDlg.pwdedit");
								Win_EndWidget("UI.roomsel.MainFrame.InputPwdDlg");
								EnterRoom(roomid,pwd);
							end;
						};
					};
				};
				RoomListPanel=
				{
					type="WIDGET",
					rect={445,43,425,464},
					bkimage = "res/uires/roomsel/roomlistpanel_bk.gsa",
					customstring="scheme/allrooms/",	--设置默认Scheme
					template={"res/uires/roomlisttempl.lua"},
					--initlayer = msgbox_layer+1,
					_playcatlog=0,
					_pageno=0,
					_npp=5,
					
					children=
					{
						--[[singleplaybtn=
						{
							type="BUTTON",
							rect={40,10,140,60},
							
							OnClick=function()
								local tmap={[0]={["basemode"]=gmodebase_rand, ["submode"]=gmodesub_single},
									[1]={["basemode"]=gmodebase_item, ["submode"]=gmodesub_single},
									[2]={["basemode"]=gmodebase_race, ["submode"]=gmodesub_single},
									[3]={["basemode"]=gmodebase_item, ["submode"]=gmodesub_single},
								};
								local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
								QuickEnterRoom(tmap[playcatlog].basemode, tmap[playcatlog].submode);
							end;
						};
						teamplaybtn=
						{
							type="BUTTON",
							rect={190,10,140,60},
							--enable=0,
							
							OnClick=function()
								local tmap={[0]={["basemode"]=gmodebase_rand, ["submode"]=gmodesub_team},
									[1]={["basemode"]=gmodebase_item, ["submode"]=gmodesub_team},
									[2]={["basemode"]=gmodebase_race, ["submode"]=gmodesub_team},
									[3]={["basemode"]=gmodebase_item, ["submode"]=gmodesub_team},
								};
								local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
								QuickEnterRoom(tmap[playcatlog].basemode, tmap[playcatlog].submode);
							end;
						};]]
						PrevBtn=
						{
							type="BUTTON",
							rect={-52,211,50,35},
							bkimage="res/uires/common/leftarrow.gsa";
							
							OnClick=function()
								local page = UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno;
								local npp=UI.children.roomsel.children.MainFrame.children.RoomListPanel._npp;
								local indexfrom=page*npp;
								
								if page > 0 then
									UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno = page-1;
									UI.children.roomsel.children.MainFrame.children.RoomListPanel.FillList();
								end;
							end;
						};
						NextBtn=
						{
							type="BUTTON",
							rect={431,211,50,35},
							bkimage="res/uires/common/rightarrow.gsa",
		
							OnClick=function()
								local page = UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno;
								local npp=UI.children.roomsel.children.MainFrame.children.RoomListPanel._npp;
								local indexfrom=page*npp;
								
								local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
								local tmap={	[0] = gmodebase_rand, 
											[1] = gmodebase_item,
											[2] = gmodebase_race,
											[3] = gmodebase_race,
										};
								local from,num,rooms,totcount=GetRoomList(tmap[playcatlog], gmodesub_rand, indexfrom,npp);
								if totcount > (page + 1)* npp then
									UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno = page+1;
									UI.children.roomsel.children.MainFrame.children.RoomListPanel.FillList();
								end;
							end;
						};
						Tabbar=
						{
							type="DIALOG",
							rect={337,23,82,300},
							moveable = 1,
							
							children=
							{
								itemmode_label = 
								{
									type = "LABEL",
									--caption = "道具模式",
									rect = {0, 0, 82, 142},
									captionrect = {0, 1, 82, 16},
									textsize = 14,
									
									children = 
									{
										signle_item=
										{
											type="BUTTON",
											rect={1,23,82,61},
											bkimage="res/uires/roomsel/singitem_btn.gsa",
											tiptext = "快速加入道具赛房间",
											--caption = "个人赛",
											--tabrect={0,0,100,40},
											--groupid=1,
											--groupstop=1,
											
											OnClick=function()
												QuickEnterRoom(gmodebase_item, gmodesub_single);
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 2;
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
											end;
					
										};
										team_item=
										{
											type="BUTTON",
											rect={1,80,82,61},
											bkimage="res/uires/roomsel/teamitem_btn.gsa",
											tiptext = "快速加入组队道具赛房间",
											--caption = "组队赛",
											--tabrect={0,0,100,40},
											--groupid=1,
											--groupstop=2,
											
											OnClick=function()
												QuickEnterRoom(gmodebase_item, gmodesub_team);
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 2;
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
											end;
					
										};
									},
								},
								speedmode_label = 
								{
									type = "LABEL",
									--caption = "竞速模式",
									rect = {1, 173, 82, 142},
									captionrect = {0, 1, 82, 16},
									textsize = 12,
									
									children = 
									{
										single_race=
										{
											type="BUTTON",
											rect={0,20,82,61},
											bkimage="res/uires/roomsel/singlerace_btn.gsa",
											tiptext = "快速加入竞速赛房间",
											--caption = "个人赛",
											--tabrect={0,0,100,40},
											--groupid=1,
											--groupstop=3,
											
											OnClick=function()								
												QuickEnterRoom(gmodebase_race, gmodesub_single);
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 2;
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
											end;
					
										};
										team_race=
										{
											type="BUTTON",
											rect={1,77,82,61},
											bkimage="res/uires/roomsel/teamrace_btn.gsa",
											tiptext = "快速加入组队竞速赛房间",
											--tabrect={0,0,100,40},
											--caption = "组队赛",
											--groupid=1,
											--groupstop=4,
											
											OnClick=function()								
												QuickEnterRoom(gmodebase_race, gmodesub_team);
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 2;
												--UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
											end;
					
										};
									},
								},
								
								
								
								--[[allroomsbtn=
								{
									type="TABWIN",
									rect={0,0,150,40},
									tabcover="res/uires/roomsel/allroomsbtn.gsa",
									tabrect={0,0,100,40},
									groupid=1,
									groupstop=1,
									checked=1,
									
									--[[
									0-所有房间
									1-道具赛
									2-竞速赛
									3-对抗赛
									]]
									OnGainFocus=function()
										UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 0;
										UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
									end;
								};
								itemplaybtn=
								{
									type="TABWIN",
									rect={100,0,100,40},
									tabcover="res/uires/roomsel/itemplaybtn.gsa",
									tabrect={0,0,100,40},
									groupid=1,
									groupstop=2,
									
									OnGainFocus=function()
										UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 1;
										UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
									end;
			
								};
								raceplaybtn=
								{
									type="TABWIN",
									rect={200,0,100,40},
									tabcover="res/uires/roomsel/raceplaybtn.gsa",
									tabrect={0,0,100,40},
									groupid=1,
									groupstop=3,
									
									OnGainFocus=function()
										UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 2;
										UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
									end;
			
								};
								combatplaybtn=
								{
									type="TABWIN",
									rect={300,0,100,40},
									tabcover="res/uires/roomsel/combatplaybtn.gsa",
									tabrect={0,0,100,40},
									groupid=1,
									groupstop=4,
									
									OnGainFocus=function()
										UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog = 3;
										UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
									end;
		
								};]]
							};
						};
						createroombtn=
						{
							type="BUTTON",
							rect={342,354,62,44},
							bkimage = "res/uires/roomsel/createroom_btn.gsa",
							tiptext = "创建一个新的游戏房间",
							--caption = "创建",
							
							OnClick=function()
								cmd_createroom();
							end;
						};
						
						findbtn=
						{
							type="BUTTON",
							rect={342,400,62,44},
							bkimage = "res/uires/roomsel/find_btn.gsa",
							tiptext = "精确查找玩家",
							--caption = "查找",
							
							OnClick=function()
								cmd_findplayer();
							end;
						};
						channel = 
						{
							type="CONTAINER",
							rect={20,456,480,30},
							--drawflag=drawflag_win_fill,
							--bkcolor={100,100,100,200},
							--bkimage="res/uires/roomsel/channelseledit.gsa",
							--bkimagepos={-3,-3},
							template = {"res/uires/channeltempl.lua"},
														
							OnInit = function()	
								UI.children.roomsel.children.MainFrame.children.RoomListPanel.children.channel.OnFillChannel(channellist.curselectedchannel);																
							end;
							OnFillChannel = function(startchannel)						
								local mylevel = GetMyLevel();
								myproperchannellist, num = GetProperChannelList(mylevel);
								if (num == nil or num == 0) then
									return;
								end;
								Win_ClearTemplates("UI.roomsel.MainFrame.RoomListPanel.channel");
								Win_AddTemplate("UI.roomsel.MainFrame.RoomListPanel.channel", 0, 0, num-1);
								if (startchannel == nil) then
									if (num == 1 or num == 3) then									
										Win_SetCheck("UI.roomsel.MainFrame.RoomListPanel.channel.channel0", 1);
									else
										Win_SetCheck("UI.roomsel.MainFrame.RoomListPanel.channel.channel1", 1);
									end;
								else
									Win_SetCheck("UI.roomsel.MainFrame.RoomListPanel.channel.channel"..startchannel,startchannel);
								end;
							end;
							children = 
							{--[[
								channel0 = 
								{
									type = "TABWIN",
									--caption = channellist[0].name;
									rect={0,0,77,35},
									tabrect = {0,0,77,35},
									tabcover = channellist[0].tabcover;
									groupid = 30,
									groupstop = 1,
								},
								
								channel1 = 
								{
									type = "TABWIN",
									--caption = channellist[1].name;
									rect={77,0,77,35},
									tabrect = {0,0,77,35},
									tabcover = channellist[1].tabcover;
									groupid = 30,
									groupstop = 2,
								},
								
								channel2 = 
								{
									type = "TABWIN",
									--caption = channellist[2].name;
									rect={77*2,0,77,35},
									tabrect = {0,0,77,35},
									tabcover = channellist[2].tabcover;
									groupid = 30,
									groupstop = 3,
								},
								
								channel3 = 
								{
									type = "TABWIN",
									--caption = channellist[3].name;
									rect={77*3,0,77,35},
									tabrect = {0,0,77,35},
									tabcover = channellist[2].tabcover;
									groupid = 30,
									groupstop = 4,
								},]]
							},
							
						};
						--[[
						channelsel=
						{
							type="CONTAINER",
							rect={20,550,150,30},
							--drawflag=drawflag_win_fill,
							--bkcolor={100,100,100,200},
							bkimage="res/uires/roomsel/channelseledit.gsa",
							bkimagepos={-3,-3},
							
							children=
							{
								channelsellabel=
								{
									type="LABEL",
									rect={0,0,120,30},
									bkimage="res/uires/roomsel/channellist.gsa",
									framescheme={{0,0,0,0}, {1,1,1,1}, {2,2,2,2}, {3,3,3,3}},
									bkimagepos={5,3},
								};

								channelselbtn=
								{
									type="BUTTON",
									rect={120,0,30,30},
									bkimage="res/uires/roomsel/channelselbtn.gsa",
									
									OnClick=function()
										Win_ShowWidget("UI.roomsel.MainFrame.RoomListPanel.channelsel.channellist",true);
										Win_AlignWidget("UI.roomsel.MainFrame.RoomListPanel.channelsel.channellist", "UI.roomsel.MainFrame.RoomListPanel.channelsel.channelsellabel",aligntype_winrect,alignstyle_left_in+alignstyle_bottom_out);
									end;
									
								};
								
								channellist=
								{
									type="LISTBOX",
									rect={0,0,120,0},
									iconlist="res/uires/roomsel/channellist.gsa",
									style=wgtstyle_clipself+wgtstyle_vertical+wgtstyle_self_ysize+wgtstyle_popup,
									extendstyle=listbox_style_auto_comfort+listbox_style_itemheight_fix,
									visible=0,
									itemheight=35,
									itemfocusback="res/uires/roomsel/channelitemselbk.gsa",
									
									patchupleft="res/uires/patch/patchupleft.gsa",
									patchupmiddle="res/uires/patch/patchupmiddle.gsa",
									patchupright="res/uires/patch/patchupright.gsa",
									patchdownleft="res/uires/patch/patchdownleft.gsa",
									patchdownmiddle="res/uires/patch/patchdownmiddle.gsa",
									patchdownright="res/uires/patch/patchdownright.gsa",
									patchleftmiddle="res/uires/patch/patchleftmiddle.gsa",
									patchmiddle="res/uires/patch/patchmiddle.gsa",
									patchrightmiddle="res/uires/patch/patchrightmiddle.gsa",
									
									OnInit=function()
										Win_SetValue("UI.roomsel.MainFrame.RoomListPanel.channelsel.channellist",4,value_channel_item_num);
									end;
									
									OnClick=function()
										local sel=Win_GetValue("UI.roomsel.MainFrame.RoomListPanel.channelsel.channellist",value_channel_sel);
										Win_SetValue("UI.roomsel.MainFrame.RoomListPanel.channelsel.channelsellabel", sel,value_channel_framescheme);
										Win_ShowWidget("UI.roomsel.MainFrame.RoomListPanel.channelsel.channellist",false);
									end;
								};
							};
						};]]
					};
					
					OnInit=function()
						--UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
						--Win_SetValue("UI.roomsel.MainFrame.RoomListPanel",5000,value_channel_timerspace);
						local page = UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno;
						local npp=UI.children.roomsel.children.MainFrame.children.RoomListPanel._npp;
						local playcatlog = 0;
						local tmap={	[0] = gmodebase_rand, 
									[1] = gmodebase_item,
									[2] = gmodebase_race,
									[3] = gmodebase_race,
								};
						GetRoomList(tmap[playcatlog], gmodesub_rand, page*npp,npp,1);
					end;
					
					OnTimer=function()
						UI.children.roomsel.children.MainFrame.children.RoomListPanel.Update();
						Win_SetValue("UI.roomsel.MainFrame.RoomListPanel",5000,value_channel_timerspace);
					end;
					
					Update=function()
						UI.children.roomsel.children.MainFrame.children.RoomListPanel.FillList();
						--[[local tmap={	[0] = "scheme/allrooms/",
									[1] = "scheme/itemplay/",
									[2] = "scheme/raceplay/",
									[3] = "scheme/combatplay/",
								};
						--local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
						local schemeid=tmap[0];--tmap[playcatlog];
						
						if schemeid ~= nil and schemeid ~= "" then
							Win_SetBackPic("UI.roomsel.MainFrame.RoomListPanel","res/uires/roomsel/"..schemeid.."bk.gsa");
							--Win_SetBackPic("UI.roomsel.MainFrame.RoomListPanel.singleplaybtn","res/uires/roomsel/"..schemeid.."singleplaybtn.gsa");
							--Win_SetBackPic("UI.roomsel.MainFrame.RoomListPanel.teamplaybtn","res/uires/roomsel/"..schemeid.."teamplaybtn.gsa");
							--Win_SetBackPic("UI.roomsel.MainFrame.RoomListPanel.createroombtn","res/uires/roomsel/"..schemeid.."createroombtn.gsa");
							--Win_SetBackPic("UI.roomsel.MainFrame.RoomListPanel.findbtn","res/uires/roomsel/"..schemeid.."findbtn.gsa");
						end;]]
					end;
					
					FillList=function(bNoRequest)
						local page = UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno;
						local npp=UI.children.roomsel.children.MainFrame.children.RoomListPanel._npp;
						
						Win_ClearTemplates("UI.roomsel.MainFrame.RoomListPanel");
						Win_AddTemplate("UI.roomsel.MainFrame.RoomListPanel",0,0,npp-1);
						
						--local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
						local playcatlog = 0;
						local tmap={	[0] = gmodebase_rand, 
									[1] = gmodebase_item,
									[2] = gmodebase_race,
									[3] = gmodebase_race,
								};
						local bUpdate=1;
						if bNoRequest~=nil then
							bUpdate=0;
						end;
						--GetRoomList(tmap[playcatlog], gmodesub_rand, page*npp,npp,bUpdate);
					end;
					
				};
				
			};
		};
		
		CreateRoomDlg=
		{
			type="MESSAGEBOX",
			rect={0,0,442,255},
			initlayer=msgbox_layer-1,
			bkimage="res/uires/roomsel/createroomdlg.gsa",
			style=wgtstyle_topmost+wgtstyle_renderbuffer;
			--drawflag = drawflag_clientprior,
			drawflag = drawflag_backprior,
			visible=0,
			defaultendok=0,
			alphaspeed=0.25,
			alphafactor=0.0,
		
			custombuffer = 1,
			buffervalue =
			{
				banimation = 1,
				cycletype = 2, 
				restype = 1, 
				resname="scene/splash.nif",			
			},
			
			_gmode=0,
			_pnum=8,
			
			OnSelfShow=function()
				Win_SetCapture("UI.roomsel.CreateRoomDlg");
				local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
				local tmap={ [0] = 0, [1] = 0, [2] = 2, [3] = 0 };
				--Win_ConsoleInfo("playcatlog:"..playcatlog);
				--Win_ConsoleInfo("gmode:"..tmap[playcatlog]);
				UI.children.roomsel.children.CreateRoomDlg._gmode = tmap[playcatlog];
				local id = "UI.roomsel.CreateRoomDlg.SelGameEdit";
				Win_SetValue(id, UI.children.roomsel.children.CreateRoomDlg._gmode,value_channel_framescheme);
			end;
			
			OnSelfHide=function()
				Win_SetCapture();
			end;
			
			children=
			{
				miniclosebtn=
				{
					type="BUTTON",
					rect={409,-11,40,44};
					bkimage="res/uires/common/miniclose.gsa",
					defaultcancel=1,
					soundid={1007},				
				},		
				SelGameEdit=
				{
					type="LABEL",
					rect={133,55,195,30},
					--caption="个人道具赛",
					bkimage="res/uires/roomsel/playcatlog.gsa",
					editable=0,
					framescheme={{0,0,0,0},{1,1,1,1},{2,2,2,2},{3,3,3,3}},
					bkimagepos={20,2},
					
					OnInit=function()
					end;
					
					
					children=
					{
						DragDownBtn=
						{
							type="BUTTON",
							rect={164,-2,30,30},
							bkimage="res/uires/roomsel/channelselbtn.gsa",
							
							OnClick=function()
								Win_ShowWidget("UI.roomsel.CreateRoomDlg.GameList",true);
								Win_AlignWidget("UI.roomsel.CreateRoomDlg.GameList","UI.roomsel.CreateRoomDlg.SelGameEdit",aligntype_winrect,alignstyle_bottom_out+alignstyle_left_in);
							end;
						};
					};
				};
				GameList=
				{
					type="LISTBOX",
					rect={2,0,163,95},
					iconlist="res/uires/roomsel/playcatlog.gsa",
					style=wgtstyle_vertical+wgtstyle_self_ysize+wgtstyle_popup,--wgtstyle_clipself+
					extendstyle=listbox_style_auto_comfort+listbox_style_itemheight_fix,
					visible=0,
					itemheight=20,
					itemfocusback="res/uires/room/itemsel_focus.gsa",
					--itemselback="res/uires/room/itemsel_focus.gsa",
					--itemdrawback   = "res/uires/room/itemselback.gsa",
					initlayer=msgbox_layer-1,
					--patchupleft="res/uires/patch/patchupleft.gsa",
					--patchupmiddle="res/uires/patch/patchupmiddle.gsa",
					--patchupright="res/uires/patch/patchupright.gsa",
					--patchdownleft="res/uires/patch/patchdownleft.gsa",
					--patchdownmiddle="res/uires/patch/patchdownmiddle.gsa",
					--patchdownright="res/uires/patch/patchdownright.gsa",
					--patchleftmiddle="res/uires/patch/patchleftmiddle.gsa",
					--patchmiddle="res/uires/patch/patchmiddle.gsa",
					--patchrightmiddle="res/uires/patch/patchrightmiddle.gsa",
					
					
					OnInit=function()
						Win_SetValue("UI.roomsel.CreateRoomDlg.GameList",4,value_channel_item_num);
					end;


					OnClick=function()
						local sel = Win_GetValue("UI.roomsel.CreateRoomDlg.GameList",value_channel_sel);
						Win_SetValue("UI.roomsel.CreateRoomDlg.SelGameEdit",sel,value_channel_framescheme);
						UI.children.roomsel.children.CreateRoomDlg._gmode = sel;
						Win_ShowWidget("UI.roomsel.CreateRoomDlg.GameList",false);
					end;
				};
				
				--房间名称
				RoomNameEdit=
				{
					type="EDIT",
					rect={142,100,220,20},
					caption="大家一起玩~!",
					textsize=12,
					tabstop=1,	
					font=1,
					tiptext = "给房间起名字",
				};
				
				--房间人数
				PCountEdit=
				{
					type="LABEL",
					rect={133,134,195,30},
					textsize=12,
					--editmethod = ui_editmethod_number,	
					tabstop=1,	
					editable=0,
					bkimage="res/uires/roomsel/pnumlist.gsa",
					framescheme={{0,0,0,0},{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4},{5,5,5,5},{6,6,6,6},{7,7,7,7}},
					bkimagepos={20,2},
							
					OnInit=function()
						local myid = "UI.roomsel.CreateRoomDlg.PCountEdit";
						Win_SetValue(myid, UI.children.roomsel.children.CreateRoomDlg._pnum - 2,value_channel_framescheme);
					end;
					
					children=
					{
						DragDownBtn=
						{
							type="BUTTON",
							rect={164,-2,30,30},
							bkimage="res/uires/roomsel/channelselbtn.gsa",
							
							OnClick=function()
								Win_ShowWidget("UI.roomsel.CreateRoomDlg.PCountList", true);
								Win_AlignWidget("UI.roomsel.CreateRoomDlg.PCountList","UI.roomsel.CreateRoomDlg.PCountEdit",aligntype_winrect,alignstyle_bottom_out+alignstyle_left_in);
							end;
						};
						
					};
				};
				PCountList=
				{
					type="LISTBOX",
					rect={4,0,163,171},
					style=wgtstyle_vertical+wgtstyle_self_ysize+wgtstyle_popup,--wgtstyle_clipself
					extendstyle=listbox_style_auto_comfort+listbox_style_itemheight_fix,
					--bkimage="res/uires/roomsel/channelselbk.gsa",
					iconlist="res/uires/roomsel/pnumlist.gsa",
					itemdrawback   = "res/uires/room/itemsel_focus.gsa",
					itemfocusback  = "res/uires/room/itemselback.gsa",
					visible=0,
					itemheight=20,
					initlayer=msgbox_layer-1,
					
					--patchupleft="res/uires/patch/patchupleft.gsa",
					--patchupmiddle="res/uires/patch/patchupmiddle.gsa",
					--patchupright="res/uires/patch/patchupright.gsa",
					--patchdownleft="res/uires/patch/patchdownleft.gsa",
					--patchdownmiddle="res/uires/patch/patchdownmiddle.gsa",
					--patchdownright="res/uires/patch/patchdownright.gsa",
					--patchleftmiddle="res/uires/patch/patchleftmiddle.gsa",
					--patchmiddle="res/uires/patch/patchmiddle.gsa",
					--patchrightmiddle="res/uires/patch/patchrightmiddle.gsa",
					
					OnInit=function()
						Win_SetValue("UI.roomsel.CreateRoomDlg.PCountList",7,value_channel_item_num);
					end;
					
					OnClick=function()
						local sel = Win_GetValue("UI.roomsel.CreateRoomDlg.PCountList",value_channel_sel);
						Win_SetValue("UI.roomsel.CreateRoomDlg.PCountEdit",sel,value_channel_framescheme);
						UI.children.roomsel.children.CreateRoomDlg._pnum=sel+2;
						Win_ShowWidget("UI.roomsel.CreateRoomDlg.PCountList",false);
					end;
				};

				--房间加密
				
				EncryptCheck=
				{
					type = "CHECK",
					rect={130,175,33,30};
					bkimage = "res/uires/common/checklabel.gsa",
					tiptext = "为房间设置一个密码",
				
					OnCheck=function(bCheck)
						if bCheck == 1 then							
							Win_EnableWidget("UI.roomsel.CreateRoomDlg.PwdEdit",true);
							Win_SetFocus("UI.roomsel.CreateRoomDlg.PwdEdit");
						end;
						if bCheck == 0 then
							Win_EnableWidget("UI.roomsel.CreateRoomDlg.PwdEdit",false);
						end;
					end;
				};
				PwdEdit=
				{
					type="EDIT",
					rect={223,183,140,20},
					textstyle=dt_left+dt_top,
					editmethod = ui_editmethod_english+ui_editmethod_chinese+ui_editmethod_number+ui_editmethod_symbol+ui_editmethod_coder,	
					tabstop=1,	
					maxchar=8;
					enable=0;
				};
				
				--确定
				OkBtn=
				{
					type="BUTTON",
					rect={100,230,100,40},
					--caption="确定",
					bkimage="res/uires/common/okbtn.gsa",
					textsize=12,
					tabstop=1,	
					
					OnClick=function()
						--local gamemode=Win_GetValue("UI.roomsel.CreateRoomDlg.SelGameEdit", value_channel_customindex);
						local gmode=UI.children.roomsel.children.CreateRoomDlg._gmode;
						
						if gmode > 3 then
							gmode = 3;
						end;
						
						if gmode < 0 then 
							gmode = 0;
						end;
						
						local tmap={ [0]={["basemode"]=gmodebase_item,["submode"]=gmodesub_single},
								[1]={["basemode"]=gmodebase_item,["submode"]=gmodesub_team},
								[2]={["basemode"]=gmodebase_race,["submode"]=gmodesub_single},
								[3]={["basemode"]=gmodebase_race,["submode"]=gmodesub_team},
						};
						
						local roomname=Win_GetText("UI.roomsel.CreateRoomDlg.RoomNameEdit");
						--local pcount=Win_GetText("UI.roomsel.CreateRoomDlg.PCountEdit");
						local pcount=UI.children.roomsel.children.CreateRoomDlg._pnum;
						local encrypt;
						if Win_IsChecked("UI.roomsel.CreateRoomDlg.EncryptCheck") then
							encrypt=1;
						end;
						local pwd=Win_GetText("UI.roomsel.CreateRoomDlg.PwdEdit");
						CreateRoom(tmap[gmode].basemode,tmap[gmode].submode,roomname,pcount,encrypt,pwd);
						UIGlobal._MapIDSelected = 0;
						Win_EndWidget("UI.roomsel.CreateRoomDlg",true);
					end;
				};
				
				--取消
				CancelBtn=
				{
					type="BUTTON",
					rect={250,230,100,40},
					--caption="取消",
					bkimage="res/uires/common/cancelbtn.gsa",
					textsize=12,
					tabstop=1,	
					
					OnClick=function()
						Win_EndWidget("UI.roomsel.CreateRoomDlg",false);
					end;
				};
			};
		};
	};
};