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
			--Win_SetText("UI.chatbar.chatpanel.chatlist", "/C03[ϵͳ]:�Բ����㲻��˵��̫��", value_channel_itemtext, -1);
			UI.children.chatbar.AddChatInfo(-1,0,0,"�Բ����㲻��˵��̫��");
		else
			Chat(goal,txt);						    						    
			Win_SetValue("UI.chatbar", tick, value_channel_customindex);
			
			local channel=chat_whisper;
			local to=goal;
			if goal == "����" then
				channel=chat_team;
				to=nil;
			end;
			if goal == "��Χ" then
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
			txt="/C06[ϵͳ]:"..content;
		end;
		if channel == chat_whisper then
			if from == nil or from == 0 then
				txt="/C01[����]���/<talkto="..to..">"..tonick.."/</>˵:"..content;
			else
				txt="/C01[����]/<talkto="..from..">"..fromnick.."/</>����˵:"..content;
			end;
		end;
		if channel == chat_team then
			if from == nil or from == 0 then
				txt="/C03[���]��˵:"..content;
			else
				txt="/C03[���]/<talkto="..from..">"..fromnick.."/</>˵:"..content;
			end;
		end;
		if channel == chat_square then
			if from == nil or from == 0 then
				txt="/C05[��Χ]��˵:"..content;
			else
				txt="/C05[��Χ]/<talkto="..from..">"..fromnick.."/</>˵:"..content;
			end;
		end;
		if channel == chat_buglet then
			if from == nil or from == 0 then
				txt="/C00[С����]��˵:"..content;
			else
				txt="/C00[С����]/<talkto="..from..">"..fromnick.."/</>˵"..content;
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
					caption="��Χ",
					
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
				--��ҳ��ť
				prepagebtn = 
				{
					type = "BUTTON",
					rect = {32, 127, 28, 16 },
					bkimage="res/uires/common/prepage.gsa",					
					extendstyle=ui_btn_style_popupmsg,
					tiptext="��ҳ",	
					
					OnClick=function()
						    local page=Win_GetValue("UI.chatbar.facepanel.facelist",value_channel_page_v);
						    Win_SetValue("UI.chatbar.facepanel.facelist",page-1,value_channel_page_v);						    
						    end;	
				};
				
				--��ҳ��ť
				nextpagebtn = 
				{
					type = "BUTTON",
					rect = { 96, 127, 28, 16 },
					bkimage="res/uires/common/nextpage.gsa",
					extendstyle=ui_btn_style_popupmsg,
					tiptext="��ҳ",	
					
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
						"���� /jy /C04/1","Ʋ�� /pz /C04/2","ɫ /se /C04/3","���� /fd /C04/4","���� /dy /C04/5","���� /ll /C04/6","���� /hx /C04/7","���� /bz /C04/8",
						"˯ /shui /C04/9","��� /dk /C04/10","���� /gg /C04/11","��ŭ /fn /C04/12","��Ƥ /tp /C04/13","���� /cy /C04/14","΢Ц /wx /C04/15","�ѹ� /ng /C04/16",
						"�� /kuk /C04/17","�ǵ� /feid /C04/18","ץ�� /zk /C04/19","�� /tu /C04/20","͵Ц /tx /C04/21","�ɰ� /ka /C04/22","���� /baiy /C04/23","���� /am /C04/24",
						"���� /jie /C04/25","�� /kun /C04/26","���� /jk /C04/27","���� /lh /C04/28","��Ц /hx /C04/29","��� /db /C04/30","�ܶ� /fendou /C04/31","���� /zhm /C04/32",
						"���� /yiw /C04/33","�� /xu /C04/34","�� /yun /C04/35","��ĥ /zhem /C04/36","˥ /shuai /C04/37","���� /kl /C04/38","�ô� /qiao /C04/39","�ټ� /zj /C04/40",
						"���� /shan /C04/41","���� /fd /C04/42","���� /aiq /C04/43","�� /tiao /C04/44","�� /zhao /C04/45","��ü /mm /C04/46","��ͷ /zt /C04/47","è�� /maom /C04/48",
						"С�� /xg /C04/49","ӵ�� /yb /C04/50","Ǯ /qianc /C04/51","���� /dp /C04/52","�Ʊ� /bei /C04/53","���� /dg /C04/54","���� /shd /C04/55","ը�� /zd /C04/56",
						"�� /dao /C04/57","���� /zq /C04/58","���� /yy /C04/59","��� /bb /C04/60","���� /kf /C04/61","�� /fan /C04/62","ҩ�� /yw /C04/63","õ�� /mg /C04/64",
						"��л /dx /C04/65","�� /wen /C04/66","���� /xin /C04/67","���� /xs /C04/68","���� /hy /C04/69","���� /lw /C04/70","�绰 /dh /C04/71","ʱ�� /sj /C04/72",
						"�ʼ� /yj /C04/73","���� /ds /C04/74","̫�� /ty /C04/75","���� /yl /C04/76","ǿ /qiang /C04/77","�� /ruo /C04/78","���� /ws /C04/79","ʤ�� /shl /C04/80",
						"��� /dd /C04/81","��Ů /mn /C04/82","���� /hl /C04/83","ëë /mamao /C04/84","Q�� /qz /C04/85","���� /fw /C04/86","��� /oh /C04/87","�׾� /bj /C04/88",
						"��ˮ /qsh /C04/89","���� /xig /C04/90","���� /xy /C04/91","���� /dy /C04/92","ѩ�� /xr /C04/93","���� /xixing /C04/94","Ů /nv /C04/95","�� /nan /C04/96",
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


