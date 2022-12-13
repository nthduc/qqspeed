UI.children.chatbar=
{
	type = "WIDGET",
	rect = { 0, 450, 550,250 },				
	style=wgtstyle_areawake+wgtstyle_cleanwin,
	initlayer=msgbox_layer - 10,
	visible=0,
	moveable=1,
	
	OnEnter=function()
		local txt=Win_GetText("UI.chatbar.chatpanel.inputbar.contentedit");
		if txt == "" then
			Win_ShowWidget("UI.chatbar",false);
			return 1;
		end;
		local goal=Win_GetText("UI.chatbar.chatpanel.chateredit");		
		local oldtick=Win_GetValue("UI.chatbar", value_channel_customindex);
		local tick = GetTickCount();
		if tick - oldtick < 1000 then
			--Win_SetText("UI.chatbar.chatpanel.chatlist", "/C03[系统]:对不起，你不能说话太快", value_channel_itemtext, -1);
			UI.children.chatbar.AddChatInfo(-1,0,0,"对不起，你不能说话太快");
		else
			Chat(goal,txt);						    						    
			Win_SetValue("UI.chatbar", tick, value_channel_customindex);
			
			local channel=chat_whisper;
			local to=goal;
			if goal == "队伍" then
				channel=chat_team;
				to=nil;
			end;
			if goal == "周围" then
				channel=chat_square;
				to=nil;
			end;
			UI.children.chatbar.AddChatInfo(channel,0,to,txt);
		end;
		Win_SetText("UI.chatbar.chatpanel.inputbar.contentedit",txt,value_channel_edithistory);
		Win_SetText("UI.chatbar.chatpanel.inputbar.contentedit","");
		return 1;
	end;		   
		
	OnWhisperTo=function(who)
		Win_SetText("UI.chatbar.chatpanel.chateredit", who);
	end;
     
	AddChatInfo=function(channel,from,to,content)
		local txt = channel;
		local fromnick=GetPlayerCard(from).NickName;
		local tonick=GetPlayerCard(to).NickName;
		if fromnick == nil or fromnick == "" then
			fromnick = from;
		end;
		if tonick == nil or tonick == "" then
			tonick = to;
		end;
		if channel == chat_system then
			txt="/C06[系统]:"..content;
		end;
		if channel == chat_whisper then
			if from == nil or from == 0 then
				txt="/C01[密语]你对/<talkto="..to..">"..tonick.."/</>说:"..content;
			else
				txt="/C01[密语]/<talkto="..from..">"..fromnick.."/</>对你说:"..content;
			end;
		end;
		if channel == chat_team then
			if from == nil or from == 0 then
				txt="/C03[组队]你说:"..content;
			else
				txt="/C03[组队]/<talkto="..from..">"..fromnick.."/</>说:"..content;
			end;
		end;
		if channel == chat_square then
			if from == nil or from == 0 then
				txt="/C05[周围]你说:"..content;
			else
				txt="/C05[周围]/<talkto="..from..">"..fromnick.."/</>说:"..content;
			end;
		end;
		if channel == chat_buglet then
			if from == nil or from == 0 then
				txt="/C00[小喇叭]你说:"..content;
			else
				txt="/C00[小喇叭]/<talkto="..from..">"..fromnick.."/</>说"..content;
			end;
		end;
		Win_SetText("UI.chatbar.chatpanel.chatlist", txt, value_channel_itemtext, -1);
	end;
	children=
	{
		bg=
		{
			type="WIDGET",
			rect={0,0,550,250},
			enable=0,
			style=wgtstyle_cleanwin,
			bkimage="res/uires/chatbar/panel_chat.gsa",
			--[[
			children=
			{
				leftpanel=
				{
					type="WIDGET",
					rect={0,0,30,250},
					style=wgtstyle_cleanwin,
					bkimage="res/uires/chatbar/chatbg_left.gsa",
				};
				middlepanel=
				{
					type="WIDGET",
					rect={84,0,450,250},
					style=wgtstyle_cleanwin,
					patchmiddle="res/uires/chatbar/chatbg_middle.gsa",
				};
				rigthpanel=
				{
					type="WIDGET",
					rect={530,0,30,250},
					style=wgtstyle_cleanwin,
					bkimage="res/uires/chatbar/chatbg_right.gsa",
				};
			};]]
		};
		chatpanel=
		{
			type="WIDGET",
			rect = { 30, 30, 500, 220 },
			aligntype=aligntype_father+aligntype_winrect,
			style=wgtstyle_areawake+wgtstyle_cleanwin,

			children=
			{
				minibtn=
				{
					type="BUTTON",
					rect={443,-13,44,20},
					bkimage="res/uires/common/miniclose.gsa",
					OnClick=function()
						Win_ShowWidget("UI.chatbar", false);
					end;
				};
				
				chatlist=
				{
					type = "TEXTLIST",
					rect = { 22, 13, 450, 155 },
					style=wgtstyle_clipself+wgtstyle_ajustfollow+wgtstyle_cleanwin,
					extendstyle=textlist_style_blockinmiddle + textlist_style_delayscroll,
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
				inputbar=
				{
					type="WIDGET",
					rect = { 22, 169, 480, 20 },
					
					children=
					{
						contentedit=
						{
							type = "RICHEDIT",
							rect = { 0,4,420,12 },							
							drawcolor={255,255,255,255},	
							focuscolor={255,255,255,255},	
							textsize=12,
							textstyle=dt_left+dt_vcenter,
							richmode=0,							
							drawflag=drawflag_focuscaption,
							extendstyle=edit_style_flashcursor,
							maxchar=100,
							
							OnTab=function()
								Win_SetFocus("UI.chatbar.chatpanel.chateredit");
								end;
						};
						
						sendbtn = 
						{
							type = "BUTTON",
							rect={430,0,50,20},
							bkimage="res/uires/chatbar/sendbtn.gsa",
							
							OnClick=function()
							     UI.children.chatbar.OnEnter();					     
							end;
						};	
					};
				};
				

				chateredit=
				{
					type="EDIT",	
					rect = { 240,192,60,12 },
					textsize=12,
					textstyle=dt_left,
					drawflag=drawflag_win_fill,
					drawcolor={0,0,0,255},
					bkcolor={150,150,150,255},
					caption="周围",
					
					OnTab=function()
						Win_SetFocus("UI.chatbar.chatpanel.inputbar.contentedit");
					end;
				},
				
				
				chaterbtn = 
				{
					type = "BUTTON",
					rect = { 300,190,50,16 },
					bkimage = "res/uires/chatbar/areaselbtn.gsa",
					
					OnClick=function()
						if Win_IsVisible("UI.chatbar.chaterlist") then
							Win_ShowWidget("UI.chatbar.chaterlist",false);
						else
							Win_ShowWidget("UI.chatbar.chaterlist",true);
						end;
					end;						
				},	
				
				actionbtn=
				{
					type="BUTTON",
					rect ={430, 190, 43, 16},
					bkimage="res/uires/chatbar/actionbtn.gsa",
					style=wgtstyle_clipself+wgtstyle_ajustfollow,
					
					OnClick=function()
						Win_ShowWidget("UI.chatbar.actionpanel", true);
						Win_AlignWidget("UI.chatbar.actionpanel","UI.chatbar.chatpanel.actionbtn",aligntype_winrect,alignstyle_top_out+alignstyle_left_in);
					end;
				};
				
				facebtn= 
				{
					type = "BUTTON",
					rect={370,190,43,16},
					bkimage="res/uires/chatbar/iconsbtn.gsa",
					
					OnClick=function()
						if Win_IsVisible("UI.chatbar.facepanel") then
							Win_ShowWidget("UI.chatbar.facepanel",false);
						else
						     Win_SetText("UI.chatbar.facepanel","UI.chatbar.chatpanel",value_channel_customname);
						     Win_AlignWidget("UI.chatbar.facepanel","UI.chatbar.chatpanel.facebtn",aligntype_winrect,alignstyle_top_out+alignstyle_left_in);
						     Win_ShowWidget("UI.chatbar.facepanel",true);
						end;						     
					end;
				};								
			};
		};
		
		chaterlist=
		{
			type="LISTBOX",
			rect={64,-15,80,60},
			clientrect={10,10,60,40},
			style=wgtstyle_topmost+wgtstyle_popup+wgtstyle_vertical+wgtstyle_self_ysize+wgtstyle_self_xsize,
			extendstyle=listbox_style_auto_comfort+listbox_style_itemheight_fix+listbox_style_wheel+listbox_style_richtext,
			richmode=ui_richmode_color,		
			textsize=12,
			itemfocusback="res/uires/common/itemsel.gsa",
			visible=0,
			itemdrawcolor={0,0,0,255},
			itemselcolor={0,0,0,255},
			itemfocuscolor={0,0,0,255},
			
			patchupleft="res/uires/bb/patchupleft.gsa",
			patchupright="res/uires/bb/patchupright.gsa",
			patchdownleft="res/uires/bb/patchdownleft.gsa",
			patchdownright="res/uires/bb/patchdownright.gsa",		
			patchupmiddle="res/uires/bb/patchupmiddle.gsa",
			patchdownmiddle="res/uires/bb/patchdownmiddle.gsa",
			patchleftmiddle="res/uires/bb/patchleftmiddle.gsa",
			patchrightmiddle="res/uires/bb/patchrightmiddle.gsa",
			patchmiddle="res/uires/bb/patchmiddle.gsa",
			
			OnSelfShow=function()						
				local seltext=Win_GetText("UI.chatbar.chatpanel.chateredit");
				local num,tgts,descs,kwds1,kwds2=GetChatGoal();
				local oldsel=0;				
				
				Win_SetValue("UI.chatbar.chaterlist",num,value_channel_item_num);
				for i=1,num do				
					Win_SetText("UI.chatbar.chaterlist",descs[i],value_channel_itemtext,i-1);
					if seltext==tgts[i] then
						oldsel=i-1;
					end;
				end;				
				Win_UpdateSelf("UI.chatbar.chaterlist");
				Win_AlignWidget("UI.chatbar.chaterlist","UI.chatbar.chatpanel.chaterbtn",aligntype_winrect,alignstyle_top_out+alignstyle_left_in, -15, 0);
				Win_SetValue("UI.chatbar.chaterlist",oldsel,value_channel_sel);
				Win_SetFocus("UI.chatbar.chaterlist");
			end;		
			
			OnOk=function()
				local seltxt="";
				local cursel=Win_GetValue("UI.chatbar.chaterlist",value_channel_sel)+1;				
				local num,tgts,descs,kwds1,kwds2=GetChatGoal();
				for i=1,num do									  						
					if cursel==i then
						seltxt=tgts[i];						
					end;
				end;								
										
				if seltxt~="" then									
					Win_SetText("UI.chatbar.chatpanel.chateredit",seltxt);
				end;						
				Win_SetFocus("UI.chatbar.chatpanel.inputbar.contentedit");
				end;	
					
			OnClick=function()
				Win_EndWidget("UI.chatbar.chaterlist",true);
			end;
		};
		
		facepanel=
		{
			type="DIALOG",
			rect={0,20,129, 148},	
			style=wgtstyle_popup+wgtstyle_topmost,
			visible=0,
			initlayer=msgbox_layer - 1,
			bkcolor= { 26, 27, 22, 196 },
			patchupleft="res/uires/bb/patchupleft.gsa",
			patchupright="res/uires/bb/patchupright.gsa",
			patchdownleft="res/uires/bb/patchdownleft.gsa",
			patchdownright="res/uires/bb/patchdownright.gsa",		
			patchupmiddle="res/uires/bb/patchupmiddle.gsa",
			patchdownmiddle="res/uires/bb/patchdownmiddle.gsa",
			patchleftmiddle="res/uires/bb/patchleftmiddle.gsa",
			patchrightmiddle="res/uires/bb/patchrightmiddle.gsa",
			patchmiddle="res/uires/bb/patchmiddle.gsa",
		
			OnOk=function()
				Win_ShowWidget("UI.chatbar.facepanel",false);
				txt=Win_GetText("UI.chatbar.facepanel.facelist",value_channel_seltext);
				bdyname=Win_GetText("UI.chatbar.facepanel",value_channel_customname);
				if (bdyname~="") then				
				editname=bdyname..".inputbar.contentedit";
				Win_OnInsert(editname,txt);
				Win_SetFocus(editname);
				end;
			end;
			
			children=
			{
				--上页按钮
				prepagebtn = 
				{
					type = "BUTTON",
					rect = {32, 127, 28, 16 },
					bkimage="res/uires/common/prepage.gsa",					
					extendstyle=ui_btn_style_popupmsg,
					tiptext="上页",	
					
					OnClick=function()
						    local page=Win_GetValue("UI.chatbar.facepanel.facelist",value_channel_page_v);
						    Win_SetValue("UI.chatbar.facepanel.facelist",page-1,value_channel_page_v);						    
						    end;	
				};
				
				--下页按钮
				nextpagebtn = 
				{
					type = "BUTTON",
					rect = { 96, 127, 28, 16 },
					bkimage="res/uires/common/nextpage.gsa",
					extendstyle=ui_btn_style_popupmsg,
					tiptext="下页",	
					
					OnClick=function()
						    local page=Win_GetValue("UI.chatbar.facepanel.facelist",value_channel_page_v);
						    Win_SetValue("UI.chatbar.facepanel.facelist",page+1,value_channel_page_v);						    
						    end;				
				};
		
				pageedit=
				{								
					type="EDIT",
					rect={64,127,30,14},
					captionrect={1,1,28,12},
					drawcolor={255,255,255,255},
					editmethod=ui_editmethod_number+ui_editmethod_value,
					style=wgtstyle_advance,
					editable=0,									
					maxvalue=6,
					minvalue=1,
					value=1,
					textsize=12,
				};
				
				facelist=
				{
					type="LISTBOX",
					rect={4,6,120, 120},	
					style=wgtstyle_none,							
					extendstyle=listbox_style_itemheight_fix+listbox_style_richtext,
					richmode=ui_richmode_icon+ui_richmode_color+ui_richmode_hlink,
					richflag=1,
					itemwidth=24,
					itemheight=24,
					itemmarginh=3,
					itemmarginv=3,
					itemnum=96,
					itemfocuscolor={0,0,0,0},
					itemseledgecolor={255,255,255,255},
					itemedgecolor={0,0,0,0},
					
					mornitor={value_channel_page_v};
					OnUpdate=function()
						local page=Win_GetValue("UI.chatbar.facepanel.facelist",value_channel_page_v);
						Win_SetValue("UI.chatbar.facepanel.pageedit",page+1);
						end;
					
					itemtext=
					{
						"/1","/2","/3","/4","/5","/6","/7","/8",
						"/9","/10","/11","/12","/13","/14","/15","/16",
						"/17","/18","/19","/20","/21","/22","/23","/24",
						"/25","/26","/27","/28","/29","/30","/31","/32",
						"/33","/34","/35","/36","/37","/38","/39","/40",
						"/41","/42","/43","/44","/45","/46","/47","/48",
						"/49","/50","/51","/52","/53","/54","/55","/56",
						"/57","/58","/59","/60","/61","/62","/63","/64",
						"/65","/66","/67","/68","/69","/70","/71","/72",
						"/73","/74","/75","/76","/77","/78","/79","/80",
						"/81","/82","/83","/84","/85","/86","/87","/88",
						"/89","/90","/91","/92","/93","/94","/95","/96",
					},
					
					itemtip=
					{
						"惊讶 /jy /C04/1","撇嘴 /pz /C04/2","色 /se /C04/3","发呆 /fd /C04/4","得意 /dy /C04/5","流泪 /ll /C04/6","害羞 /hx /C04/7","闭嘴 /bz /C04/8",
						"睡 /shui /C04/9","大哭 /dk /C04/10","尴尬 /gg /C04/11","发怒 /fn /C04/12","调皮 /tp /C04/13","呲牙 /cy /C04/14","微笑 /wx /C04/15","难过 /ng /C04/16",
						"酷 /kuk /C04/17","非典 /feid /C04/18","抓狂 /zk /C04/19","吐 /tu /C04/20","偷笑 /tx /C04/21","可爱 /ka /C04/22","白眼 /baiy /C04/23","傲慢 /am /C04/24",
						"饥饿 /jie /C04/25","困 /kun /C04/26","惊恐 /jk /C04/27","流汗 /lh /C04/28","憨笑 /hx /C04/29","大兵 /db /C04/30","奋斗 /fendou /C04/31","咒骂 /zhm /C04/32",
						"疑问 /yiw /C04/33","嘘 /xu /C04/34","晕 /yun /C04/35","折磨 /zhem /C04/36","衰 /shuai /C04/37","骷髅 /kl /C04/38","敲打 /qiao /C04/39","再见 /zj /C04/40",
						"闪人 /shan /C04/41","发抖 /fd /C04/42","爱情 /aiq /C04/43","跳 /tiao /C04/44","找 /zhao /C04/45","美眉 /mm /C04/46","猪头 /zt /C04/47","猫咪 /maom /C04/48",
						"小狗 /xg /C04/49","拥抱 /yb /C04/50","钱 /qianc /C04/51","灯泡 /dp /C04/52","酒杯 /bei /C04/53","蛋糕 /dg /C04/54","闪电 /shd /C04/55","炸弹 /zd /C04/56",
						"刀 /dao /C04/57","足球 /zq /C04/58","音乐 /yy /C04/59","便便 /bb /C04/60","咖啡 /kf /C04/61","饭 /fan /C04/62","药丸 /yw /C04/63","玫瑰 /mg /C04/64",
						"凋谢 /dx /C04/65","吻 /wen /C04/66","爱心 /xin /C04/67","心碎 /xs /C04/68","会议 /hy /C04/69","礼物 /lw /C04/70","电话 /dh /C04/71","时间 /sj /C04/72",
						"邮件 /yj /C04/73","电视 /ds /C04/74","太阳 /ty /C04/75","月亮 /yl /C04/76","强 /qiang /C04/77","弱 /ruo /C04/78","握手 /ws /C04/79","胜利 /shl /C04/80",
						"多多 /dd /C04/81","美女 /mn /C04/82","汉良 /hl /C04/83","毛毛 /mamao /C04/84","Q仔 /qz /C04/85","飞吻 /fw /C04/86","怄火 /oh /C04/87","白酒 /bj /C04/88",
						"汽水 /qsh /C04/89","西瓜 /xig /C04/90","下雨 /xy /C04/91","多云 /dy /C04/92","雪人 /xr /C04/93","星星 /xixing /C04/94","女 /nv /C04/95","男 /nan /C04/96",
					},
					
					OnRClick=function()
					       Win_ShowWidget("UI.chatbar.facepanel",false);
					       bdyname=Win_GetText("UI.chatbar.facepanel",value_channel_customname);
					       if (bdyname~="") then
					       editname=bdyname..".inputbar.contentedit";
					       Win_SetFocus(editname);
					       end;
					end;
						       
					OnClick=function()
						Win_EndWidget("UI.chatbar.facepanel",true);
					end;
				};
			};
		};
		actionpanel=
		{
			type="LISTBOX",
			rect={0,200,100,100},
			clientrect={10,10,80,80},
			style=wgtstyle_topmost+wgtstyle_popup+wgtstyle_vertical+wgtstyle_self_ysize+wgtstyle_self_xsize,
			extendstyle=listbox_style_auto_comfort+listbox_style_itemheight_fix+listbox_style_wheel+listbox_style_richtext,
			visible=0,
			initlayer=msgbox_layer,
			bkcolor= { 26, 27, 22, 196 },
			alignstyle=alignstyle_left_in,
			itemfocuspos={0,0,0,-1},
			textsize=12,
			itemdrawcolor={0,0,0,255},
			itemselcolor={0,0,0,255},
			itemfocuscolor={0,0,0,255},
			patchupleft="res/uires/bb/patchupleft.gsa",
			patchupright="res/uires/bb/patchupright.gsa",
			patchdownleft="res/uires/bb/patchdownleft.gsa",
			patchdownright="res/uires/bb/patchdownright.gsa",		
			patchupmiddle="res/uires/bb/patchupmiddle.gsa",
			patchdownmiddle="res/uires/bb/patchdownmiddle.gsa",
			patchleftmiddle="res/uires/bb/patchleftmiddle.gsa",
			patchrightmiddle="res/uires/bb/patchrightmiddle.gsa",
			patchmiddle="res/uires/bb/patchmiddle.gsa",

			OnInit=function()
				local actioncount, actions = GetChatActionsDesc();
				if actioncount > 0 then
					Win_SetValue("UI.chatbar.actionpanel", actioncount, value_channel_item_num);
					local index = 0;
					for i = 1, actioncount,1 do
						if actions[i].Name ~= nil and actions[i].Name ~= "" then
							Win_SetText("UI.chatbar.actionpanel",actions[i].Name,value_channel_itemtext,index);	
							index=index+1;
						end;
					end;
				end;
			end;
			OnClick=function()
				local sel = Win_GetValue("UI.chatbar.actionpanel",value_channel_hot);
				local seltext = Win_GetText("UI.chatbar.actionpanel", value_channel_itemtext, sel);
				Win_ShowWidget("UI.chatbar.actionpanel",false);
				DoChatAction(seltext);
				end;
		};
		
	};			
};		


