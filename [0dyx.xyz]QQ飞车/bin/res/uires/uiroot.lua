UI =
{

AllCursor = 
{	
	{
		name = "EditCursor",
		path = "res/uires/cursor_standard/edit.cur",
		step = 0.8,
		holdframe = 1,
		hotspot={5,8},
	};
	
	{
		name = "LinkCursor",
		path = "res/uires/cursor_standard/link.cur",
		step = 0.8,
		holdframe = 1,
		hotspot={2,2},
	};
	
	{
		name = "Horizon",
		path = "res/uires/cursor_standard/hori.cur",
		step = 0.8,
		hotspot={14,6},
	};
	
	{
		name = "Vertical",
		path = "res/uires/cursor_standard/vert.cur",
		step = 0.8,
		hotspot={6,14},
	};
	
	{
		name = "Click",
		path = "res/uires/cursor_standard/click.cur",
		step = 0.8,
		holdframe = 1,
		hotspot={2,2},
	};	
	
	{
		name = "Forbid",
		path = "res/uires/cursor_standard/forbid.cur",
		step = 0.8,
		hotspot={13,13},
	};
	
	{
		name = "Normal",
		path = "res/uires/cursor_standard/normal.cur",
		step = 0.8,
		holdframe = 1,
		hotspot={2,2},
	};
};--UICursor

AllScreen=
{	
	{
		id="Regist",
		path={"res/uires/regist.lua"},
	};
	
	{
		id = "entry",
		path ={"res/uires/entry.lua"},		
	};

	{
		id = "startmenu",
		path={"res/uires/startmenu.lua"},
	};
	
	{
		id = "roomsel",
		path={"res/uires/roomsel.lua"},
	};

	{
		id = "room",
		path={"res/uires/room.lua"},
	};

	{
		id = "score",
		path={"res/uires/score.lua"},
	};
	
	{
		id="gameui",
		path={"res/uires/gameui.lua"},
	};
	
	{
		id = "shop",
		path={"res/uires/shop.lua",},
	};
	
	{
		id = "taskscore",
		path={"res/uires/taskscore.lua",},
	};
},


AllScript= 
{
};
	
buddy=
{

	{  type=buddy_widget_tipwgt,     style=buddy_style_self,  id="UI.SysTooltip",   };
	{  type=buddy_widget_msgbox,   style=buddy_style_self,  id="UI.SysMsgbox",   };
	{  type=buddy_widget_console,   style=buddy_style_self,  id="UI.SysConsole",  };
};

includes=
{
	"res/uires/login.lua",
	"res/uires/EventHandle.lua",
	"res/uires/chat.lua",
	"res/uires/uimisc.lua",
	"res/uires/taskbar.lua",
	"res/uires/mapdesc.lua",
	"res/uires/uicmd.lua",
	"res/uires/leveldesc.lua",
	"res/uires/mapsel/maps.lua",
	"res/uires/regist/registinfo.lua",
	"res/uires/avatarsel/avatars.lua",
	"res/uires/uiglobal.lua",
	"res/uires/navigation.lua",
	"res/uires/playerinfo.lua",
	"res/uires/garage.lua",
	"res/uires/tiptext.lua",
	"res/uires/taskmsgbox.lua",	
	"res/uires/help.lua",
};

accel=
{

	{"UI.OnAccel_FocusPaste",86,0,1},--ctrl+v粘贴
	{"UI.OnAccel_FocusCut",88,0,1},--ctrl+x剪切
	{"UI.OnAccel_FocusCopy",67,0,1},--ctrl+c拷贝			
	{"UI.OnAccel_ToggleConsole",49,0,1},--ctrl+1
	{"UI.OnAccel_ToggleEditMode",50,0,1},--ctrl+2
	{"UI.OnAccel_ReloadScreen",51,0,1},--ctrl+3
	{"UI.OnAccel_DebugScreen",52,0,1},--ctrl+4
	{"UI.OnAccel_OnEnter",13},--enter 切换交谈栏
	{"UI.OnAccel_HistoryChat",82,0,1},
	{"UI.OnAccel_Esc",27}, --Esc
	{"UI.OnRun", 116},
},
				    
OnAccel_FocusPaste=function()
	local hotwgt=Win_GetHotPath();
	if hotwgt~="" then
	Win_OnPaste(hotwgt);
	end;
	return 1;
	end;
				 
OnAccel_FocusCut=function()
	local hotwgt=Win_GetHotPath();
	if hotwgt~="" then
	Win_OnCut(hotwgt);
	end;
	return 1;
	end;
				 
OnAccel_FocusCopy=function()
	local hotwgt=Win_GetHotPath();
	if hotwgt~="" then
	Win_OnCopy(hotwgt);
	end;
	return 1;
	end;	
					
OnAccel_ToggleConsole=function()
	       Win_ToggleConsole();
	      end;
					 
OnAccel_ToggleEditMode=function()
	       mode=Win_GetEditMode();
		   if mode==0 then
		   Win_SetEditMode(1);
		   else
		   Win_SetEditMode(0);		   
		   end;
	      end;

OnAccel_ReloadScreen=function()
	       Win_ReloadScreen();
	      end;

OnAccel_DebugScreen=function()
	       --Win_DebugScreen();
	       local oldscreen=Win_GetCurScreen();
	       GotoUIScreen("");
	       LoadUI();
	       GotoUIScreen(oldscreen);
	      end;
OnAccel_HistoryChat=function()
	if Win_IsVisible("UI.chatbar") then
		Win_SendMessage("UI.chatbar.chatpanel.inputbar.contentedit",msgtype_keydown,38,0);
	end;
end;
	      
OnAccel_OnEnter=function(wparam,lparam)	
	if Win_GetCurScreen() == "room" then
		Win_SetFocus("UI.room.MainFrame.ChatEdit");
	end;
	return 0;
end;		

OnAccel_Esc=function()
	--LeaveRoom();
end;

OnRun=function()
	if Win_GetCaptureID()~="" then
		return;
	end;
	if Win_GetCurScreen() == "room" then
		UI.children.shortcut_dlg.children.roomAssociated_wgt.children.ReadyBtn.OnClick();
	end;
end;

children=
{
	SysConsole=
	{
		type="CONSOLE",
		rect={0,-250,800,250},
		bkcolor = { 0, 0, 0, 255 },	
		initlayer=console_layer;
		visible=0,
		drawflag=drawflag_win_edge+drawflag_win_fill,
		pathmove=0,
		pathflip=1,
		pathpoints=
		{
		   {0,-250,ui_pathcmd_cycle+ui_pathcmd_alpha,0.0,1.0};
		   {0,0,ui_pathcmd_cycle+ui_pathcmd_alpha,1.0,1.0};
		};
		
		
		OnPathCommand=function(id,x,y)
					  if (id==1) then
				          Win_ShowWidget("UI.SysConsole",true);
					  elseif (id==0) then
				          Win_ShowWidget("UI.SysConsole",false);
					  end;
					  Win_SetValue("UI.SysConsole",1,value_channel_pathmove_pause);					  
					  end;
		
		OnSelfShow=function()
				   Win_SetFocus("UI.SysConsole.downinput");
				   end;
						   
		OnSelfHide=function()
				   Win_SetFocus();
				   end;
				   
		OnEnter=function()
					     
			     local info=Win_GetText("UI.SysConsole.downinput");
			     if info~="" then
			     Win_SetText("UI.SysConsole",info,value_channel_itemtext);
			     Win_SetText("UI.SysConsole.downinput",info,value_channel_edithistory);
			     end;
			     			     
			     Win_SetText("UI.SysConsole.downinput","");
			     
			     end;		   
						   
		children=
		{
		
			downinput=
			{
				type = "EDIT",
				rect = {0,0, 600, 16 },
				drawcolor = { 255, 255, 255, 255 },
				bkcolor = { 0, 0, 0, 128 },			
				tabstop=0,
				textstyle=dt_left+dt_vcenter,
				maxchar=128;
				aligntype=aligntype_winrect+aligntype_father;
				alignstyle=alignstyle_bottom_in+alignstyle_left_in;
				marginh=20;	
			
			};
			
			cmdlabel=
			{
				type="LABEL",
				rect = { 0, 0, 20, 16},								
				textstyle=dt_center+dt_vcenter,
				drawcolor={255,255,255,255},
				bkcolor = { 0, 0, 0, 0 },
				caption=">>",				
				aligntype=aligntype_winrect+aligntype_father;
				alignstyle=alignstyle_bottom_in+alignstyle_left_in;
				
			},	
		
		};
				
	};
	
	SysTooltip=
	{
		type = "LABEL",
		rect={-100,-100,50,30},		
		drawcolor = { 0, 0, 0, 255 },		
		bkcolor={16,16,16,196},
		edgecolor = { 0, 0, 0, 128 },
		captionrect={30,10,10,10},
		textfont=1,
		maxwidth=800,
		minheight=20,
		minwidth=100,
		sizeable=1,	
		visible=0,
		marginh=12,
		marginv=6,
		textsize=12,
		initlayer=systooltip_layer,
		style=wgtstyle_vertical,--+wgtstyle_clipself,		
		richmode=ui_richmode_color,
		alphaspeed=0.1,		
		edgecheck=edge_check_show+edge_check_auto,
		
		patchupleft="res/uires/tooltip/patchupleft.gsa",
		patchupright="res/uires/tooltip/patchupright.gsa",
		patchdownleft="res/uires/tooltip/patchdownleft.gsa",
		patchdownright="res/uires/tooltip/patchdownright.gsa",		
		patchupmiddle="res/uires/tooltip/patchupmiddle.gsa",
		patchdownmiddle="res/uires/tooltip/patchdownmiddle.gsa",
		patchleftmiddle="res/uires/tooltip/patchleftmiddle.gsa",
		patchrightmiddle="res/uires/tooltip/patchrightmiddle.gsa",
		patchmiddle="res/uires/tooltip/patchmiddle.gsa",
		
		children=
		{
		--[[
			leftarrow=
			{
				type="WIDGET",
				rect={-24,25,15,15},
				bkimage="res/uires/tooltip/patchleftarrow.gsa",
			};]]--
		};
	};	
		
	SysMsgbox=
	{
		type="MESSAGEBOX",
		rect = { 0, 0, 339, 230},	
		clientrect = { 0, 0, 339, 230},
		captionrect = { 0, 0, 0, 0 },	
		--bkimagepos = {-343, -36},
		--bkimage="res/uires/common/msgbox_bk.gsa",		
		--coverpic="res/uires/common/msgbox.gsa",
		bkimage="res/uires/common/msgbox.gsa",
		widget_style = wgtstyle_renderitem,
		--imagepos = {-343, -154},
		--bkimage="res/uires/common/msgbox_bk.gsa",
		drawcolor={255,255,255,255},
		textsize=12,
		textfont=1,
		textstyle=dt_center,
		initlayer=msgbox_layer,
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
		
		OnTimer=function()
			local msgid=Win_GetValue("UI.SysMsgbox",value_channel_msgbox_id);
			if (msgid==ui_msgid_conn)or(msgid==ui_msgid_conn_nobtn) then
			
			local curtimer=Win_GetValue("UI.SysMsgbox",value_channel_customindex);
			curtimer=curtimer+1;			
			if curtimer>6 then
			curtimer=1;
			end;
			Win_SetValue("UI.SysMsgbox",curtimer,value_channel_customindex);
			
			local msginfo=Win_GetText("UI.SysMsgbox",value_channel_msgbox_info);
			local ncomma=math.mod(curtimer,6);
			local appendstr=string.rep(".",ncomma);
			
			Win_SetText("UI.SysMsgbox.sentence","    "..msginfo..appendstr);
			
			
			if curtimer<=0 then
			Win_SetValue("UI.SysMsgbox",-1,value_channel_timerspace);			
			else			
			Win_SetValue("UI.SysMsgbox",1,value_channel_timerspace);			
			end;
			
			end;
			end;
		
		OnSelfHide=function()
			Win_SetValue("UI.SysMsgbox",-1,value_channel_timerspace);
			local curScrnId,curScrnPath=Win_GetCurScreen();
			if curScrnId~="" then
			local discarded=Win_GetValue(curScrnPath,value_channel_discarded);	
			if discarded==0 then
			Win_SetFocus(curScrnPath);	
			end;
			end;
			end;
			
		OnSelfShow=function()
			Win_ShowWidget("UI.SysMsgbox.agreebtn",false);
			Win_ShowWidget("UI.SysMsgbox.resistbtn",false);
			Win_ShowWidget("UI.SysMsgbox.okbtn",false);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",false);
			Win_ShowWidget("UI.SysMsgbox.affirmbtn",false);
			Win_ShowWidget("UI.SysMsgbox.abortbtn",false);
			Win_ShowWidget("UI.SysMsgbox.closebtn",false);
			Win_ShowWidget("UI.SysMsgbox.miniclosebtn",true);
						
			Win_SetValue("UI.SysMsgbox",-1,value_channel_timerspace);
			
			local msgid=Win_GetValue("UI.SysMsgbox",value_channel_msgbox_id);
						
			if msgid==ui_msgid_yesno then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_okcancel then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_agreeresist then
			Win_ShowWidget("UI.SysMsgbox.agreebtn",true);
			Win_ShowWidget("UI.SysMsgbox.resistbtn",true);			
			elseif msgid==ui_msgid_close then
			Win_ShowWidget("UI.SysMsgbox.closebtn",true);
			elseif msgid==ui_msgid_miniclose then
			Win_ShowWidget("UI.SysMsgbox.miniclosebtn",true);			
			elseif msgid==ui_msgid_skillrevive then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);			
			elseif msgid==ui_msgid_itemrevive then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_revive then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_endwork then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_quit then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			local bset = GetGameVar("homepage");
			elseif msgid==ui_msgid_bind then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_droplost then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);						
			elseif msgid==ui_msgid_moneyalert then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);						
			elseif msgid==ui_msgid_conn then			
			Win_ShowWidget("UI.SysMsgbox.abortbtn",true);
			Win_SetValue("UI.SysMsgbox",1,value_channel_customindex);
			Win_SetValue("UI.SysMsgbox",1,value_channel_timerspace);
			elseif msgid==ui_msgid_conn_nobtn then			
			Win_SetValue("UI.SysMsgbox",1,value_channel_customindex);
			Win_SetValue("UI.SysMsgbox",1,value_channel_timerspace);
			elseif msgid==ui_msgid_cancel then
			Win_ShowWidget("UI.SysMsgbox.abortbtn",true);	
			elseif msgid==ui_msgid_collect then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);			
			elseif msgid==ui_msgid_update then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_delmailalert then
			Win_ShowWidget("UI.SysMsgbox.okbtn",true);
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid==ui_msgid_cancel then
			Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
			elseif msgid~=ui_msgid_sentence then
			Win_ShowWidget("UI.SysMsgbox.affirmbtn",true);	
			end;
			
			local msginfo=Win_GetText("UI.SysMsgbox",value_channel_msgbox_info);
			if msgid==ui_msgid_quit then
			Win_SetText("UI.SysMsgbox.sentence"," "..msginfo.."\n\n");
			else
			Win_SetText("UI.SysMsgbox.sentence","    "..msginfo);
			end;
			
			Win_SetCapture("UI.SysMsgbox");
			Win_SetFocus("UI.SysMsgbox");
			
			end;
			
		OnOk=function()
			end;
	
		OnCancel=function()
			 end;	
			 
			
		children=
		{			
			sentence=
			{
				type="LABEL",
				rect = { 27,57,280,125 },
				textsize=12,
				rowspace=4,
				textstyle=dt_left+dt_vcenter,
				drawcolor={255,255,255,255},
				richmode=ui_richmode_color+ui_richmode_hlink,
				enable=1,		
				initlayer=0;
				pluslayer=0;
			},
			
			agreebtn=
			{
				type="BUTTON",
				rect={51,220,90,40},
				bkimage="res/uires/common/agreebtn.gsa",
				defaultok=1,
				soundid={1005},				
			},
			
			resistbtn=
			{
				type="BUTTON",
				rect={233,220,90,40},
				bkimage="res/uires/common/resistbtn.gsa",
				defaultcancel=1,
				soundid={1007},
				
			},
			
			okbtn=
			{
				type="BUTTON",
				rect={51,220,90,40},
				bkimage="res/uires/common/okbtn.gsa",
				defaultok=1,
				soundid={1005},				
			},
			
			cancelbtn=
			{
				type="BUTTON",
				rect={233,220,90,40},
				bkimage="res/uires/common/cancelbtn.gsa",
				defaultcancel=1,
				soundid={1007},				
			},
			
			affirmbtn=
			{
				type="BUTTON",
				rect={130,220,90,40};
				bkimage="res/uires/common/affirmbtn.gsa",
				defaultcancel=1,
				soundid={1007},				
			},
			
			abortbtn=
			{
				type="BUTTON",
				rect={144,220,90,40},
				bkimage="res/uires/common/abortbtn.gsa",
				defaultcancel=1,
				soundid={1007},				
			},
			
			closebtn=
			{
				type="BUTTON",
				rect={144,220,90,40};
				bkimage="res/uires/common/closebtn.gsa",
				defaultcancel=1,
				soundid={1007},				
			},
			
			miniclosebtn=
			{
				type="BUTTON",
				rect={306,-11,40,44};
				bkimage="res/uires/common/miniclose.gsa",
				defaultcancel=1,
				soundid={1007},				
			},		
		}, --children					 
	};
	splashWgt=
	{
		type = "WIDGET",
		rect = {0, 0, 1024, 768},
		initlayer = msgbox_layer;
		--alphaspeed=0.25,
		--alphafactor=0.0,
		visible = 1,
		_logintype = 1, --0为需要注册新手， 1为非新手
		_step = 1,
		_canStartNextStep = false;
		
		bkimage="res/uires/entry/bk.gsa",
		
		accel=
		{
			{"UI.splashWgt.OnEscape",27},--ctrl+v粘贴
		};
		
		OnEscape = function()			
			local path = "UI.splashWgt";
			if (UI.children.splashWgt._step == 1) then
				return;
			end;
			if (Win_IsVisible(path)) then
				Win_SetValue(path, -1, value_channel_timerspace);
				Win_ShowWidget(path, false);
				--local curscreen = GetCurUIScreen();
				--GotoUIScreen(curscreen);
				if (UI.children.splashWgt._logintype == 0) then
					cmd_regist();
				else
					cmd_gohome();
				end;
			end;
		end;
		
		OnInit = function()
			local path = "UI.splashWgt";
			Win_SetValue(path, 1000, value_channel_timerspace);
			Win_ConsoleInfo("Splash Show");
			UI.children.splashWgt._step = 1;				
		end;
		
		OnClick = function()
			if (UI.children.splashWgt._step == 1) then
				return;
			end;
			local path = "UI.splashWgt";
			Win_SetValue(path, -1, value_channel_timerspace);
			Win_ShowWidget(path, false);
			--local curscreen = GetCurUIScreen();
			--GotoUIScreen(curscreen);
			if (UI.children.splashWgt._logintype == 0) then
				cmd_regist();
			else
				cmd_gohome();
			end;
		end;
		
		OnTimer = function()
			local path = "UI.splashWgt";
			if (UI.children.splashWgt._canStartNextStep == false) then
				--一百毫秒后重试
				Win_SetValue(path, 100, value_channel_timerspace);
				return;
			end;
			if (UI.children.splashWgt._step == 1) then
				Win_SetValue(path, 3500, value_channel_timerspace);
				UI.children.splashWgt._step = 2;
				return;
			end;
			UI.children.splashWgt._step = 1;
			Win_ConsoleInfo("执行Splash的Timer");
			Win_ShowWidget(path, false);
			--local curscreen = GetCurUIScreen();
			--GotoUIScreen(curscreen);
			if (UI.children.splashWgt._logintype == 0) then
				cmd_regist();
			else
				cmd_gohome();
			end;
		end;
	},
};
};--UI


