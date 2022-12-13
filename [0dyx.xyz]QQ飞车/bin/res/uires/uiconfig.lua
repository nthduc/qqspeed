logintype_single	= 0;
logintype_multi		= 1;

hosttype_play	= 1;
hosttype_avatar	= 2;
hosttype_nif	= 3;
hosttype_scene	= 4;
hosttype_shopitem	= 5;
hosttype_roomself	=6;
hosttype_gameplay	=7;

scorecol_nickname	= 1; --结算面板的栏
scorecol_ftime		= 2;
scorecol_expn		= 4;
scorecol_money		= 8;
scorecol_tp			= 16;

gmodebase_rand	=	0;
gmodebase_race	=	1;
gmodebase_item	=	2;
gmodesub_rand	=	0;
gmodesub_single	=	1;
gmodesub_team	=	2;

seat_close		=	0;
seat_open		=	1;
seat_closed	=	0;
seat_free		=	1;
seat_used		=	2;

state_ready	=	0;
state_unready 	=	1
state_readyed	=	1;
state_unreadyed	=	0;

chat_system	=	-1;
chat_whisper	=	0;
chat_inroom	=	1;
chat_ingame	=	2;
chat_buglet	=	3;

tsound_default_noloop	= 0;
tsound_default_loop	= -1;

--ServerEntity
eQQServer		= 0;
eDirServer 		= 1;
eGameServer 	= 2;
eShopServer 	= 3;

--login result	
eLoginSuccessed 	= 0;
eLoginFailed 		= 1;
eLoginTimeout 		= 2;
eLoginRequireReg 	= 3;
eLoginCanceled 	=4;
eLoginIdentityLimit	=5;
eLoginVersionLimit	=6;
eLoginVersionRecommend	=7;
eLoginFileCracked	=8;

--UIMsgId	
ui_msgid_default	= 0;--带一个ok按钮

ui_msgid_yesno		= 1;--带一个yes按钮，一个no按钮(是，否)
ui_msgid_okcancel	= 2;--带一个ok按钮，一个cancel按钮(确定，取消)
ui_msgid_agreeresist	= 3;--带一个agree按钮，一个resist按钮（同意，拒绝）

ui_msgid_affirm		= 4;--带一个affirm按钮(确定)
ui_msgid_close		= 5;--带一个close按钮(关闭)
ui_msgid_miniclose	= 6;--带一个右上角小close按钮(关闭)
ui_msgid_sentence	= 7;--带一行文本
ui_msgid_cancel		= 110; --带一个cancel按钮
--custombuffer
custom_buffer_none		= -1;
custom_buffer_sketchmap	= 0;

-- dragstyle
ui_dragtype_none=-1;
ui_dragtype_item=0;
ui_drag_type_shop = 1;
ui_drag_type_dressroom_item=2;
ui_drag_type_dressroom_template=3;
ui_drag_type_gamelogic = 4;


cs_pos_weapon=0;
cs_pos_armor=1;

--商品类型
CMDY_ALL = -1
CMDY_FIXSUIT = 0   
CMDY_BODY 	=1
CMDY_HEAD =2
CMDY_LEG  =3
CMDY_SHOE =4
CMDY_TRUNKB =5
CMDY_OBJECT = 6
CMDY_GUIZU  =8 
CMDY_XINPIN  = 16 
CMDY_TEJIA  = 32 
CMDY_LIUXING  = 64


keyboard_chars_dn=
{ 
"0","1","2","3","4","5","6","7","8","9",
"a","b","c","d","e","f","g","h","i","j","k",
"l","m","n","o","p","q","r","s","t","u","v",
"w","x","y","z","`","-","=","\\","[","]",";",
"'",",",".","/"
};

keyboard_chars_up=
{ 
"0","1","2","3","4","5","6","7","8","9",
"A","B","C","D","E","F","G","H","I","J","K",
"L","M","N","O","P","Q","R","S","T","U","V",
"W","X","Y","Z","`","-","=","\\","[","]",";",
"'",",",".","/"
};
	

Root_OnAccel_ReloadScreen=function()
	       Win_ReloadScreen();
	      end;
	      
Root_OnAccel_ToggleMode=function()		
	mode=Win_GetEditMode();
	if mode==0 then
	Win_SetEditMode(1);
	else
	Win_SetEditMode(0);		   
	end;
	return 1;
	end;
			       
Root_OnAccel_ToggleConsole=function()
				    Win_ToggleConsole();
				    return 1;
				    end;
				    
Root_OnAccel_FocusPaste=function()
				 Win_FocusOnPaste();
				 return 1;
				 end;
				 
Root_OnAccel_FocusCut=function()
				 Win_FocusOnCut();
				 return 1;
				 end;
				 
Root_OnAccel_FocusCopy=function()
				 Win_FocusOnCopy();
				 return 1;
				 end;	
					 



Root_SysConsole_OnPathCommand=function(id,x,y)
			  if (id==1) then
			  Win_ShowWidget("UI.SysConsole",true);
			  elseif (id==0) then
			  Win_ShowWidget("UI.SysConsole",false);
			  end;
			  Win_SetValue("UI.SysConsole",1,value_channel_pathmove_pause);					  
			  end;

Root_SysConsole_OnSelfShow=function()
		   Win_SetFocus("UI.SysConsole.downinput");
		   end;
				   
Root_SysConsole_OnSelfHide=function()
		   Win_SetFocus();
		   end;
		   
Root_SysConsole_OnEnter=function()
			     
	     info=Win_GetText("UI.SysConsole.downinput");
	     if info~="" then
	     Win_SetText("UI.SysConsole",info,value_channel_itemtext);
	     Win_SetText("UI.SysConsole.downinput",info,value_channel_edithistory);
	     end;
				     
	     Win_SetText("UI.SysConsole.downinput","");
	     
	     end;		   
				   

Root_SysMsgbox_OnSelfShow=function()
	Win_ShowWidget("UI.SysMsgbox.agreebtn",false);
	Win_ShowWidget("UI.SysMsgbox.resistbtn",false);
	Win_ShowWidget("UI.SysMsgbox.okbtn",false);
	Win_ShowWidget("UI.SysMsgbox.cancelbtn",false);
	Win_ShowWidget("UI.SysMsgbox.affirmbtn",false);
	Win_ShowWidget("UI.SysMsgbox.closebtn",false);
	Win_ShowWidget("UI.SysMsgbox.miniclosebtn",false);
	
	msgid=Win_GetValue("UI.SysMsgbox",value_channel_msgbox_id);
				
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
	elseif msgid==ui_msgid_revive then
	Win_ShowWidget("UI.SysMsgbox.okbtn",true);
	Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);
	elseif msgid==ui_msgid_endwork then
	Win_ShowWidget("UI.SysMsgbox.okbtn",true);
	Win_ShowWidget("UI.SysMsgbox.cancelbtn",true);			
	elseif msgid~=ui_msgid_sentence then
	Win_ShowWidget("UI.SysMsgbox.affirmbtn",true);		
	end;
	
	msginfo=Win_GetText("UI.SysMsgbox",value_channel_msgbox_info);
	Win_SetText("UI.SysMsgbox.sentence","    "..msginfo);
	
	Win_SetCapture("UI.SysMsgbox");
	Win_SetFocus("UI.SysMsgbox");
	
	end;
	
Root_SysMsgbox_OnOk=function()
	 MsgBoxReturn(1);
	 end;

Root_SysMsgbox_OnCancel=function()
	 MsgBoxReturn(0);
	 end;	
	 
	

Entry_OnDenit = function()	
		Win_ShowWidget("UI.entry",false);
		return 1;			   
		end;
			
Entry_OnInit = function()	
		Win_SetValue("UI.entry",0,value_channel_alphafactor);
		Win_ShowWidget("UI.entry",true);
		
		Win_SetText( "UI.entry.btnpanel.pwdedit","");				
		Win_SetFocus( "UI.entry.btnpanel.idedit" ); 		
				
		return 1;			   
		end;
	 
Entry_OnAccel_OnTab=function()
			  
			  if not Win_IsVisible("UI.SysConsole") then
			  
				  if Win_IsFocused("UI.entry.btnpanel.idedit") then
				  Win_SetFocus("UI.entry.btnpanel.pwdedit");
				  else 
				  Win_SetFocus("UI.entry.btnpanel.idedit");
				  end;
			  
			  end;
			  
			  return 1;		  
			  end;
			  
Entry_OnAccel_OnEnter=function()
			  result=0;
			  if not Win_IsVisible("UI.SysConsole") then
			  
				  if Win_IsVisible("UI.SysMsgbox") then
				  Win_EndWidget("UI.SysMsgbox",true);				  
				  else 
				  Entry_btnpanel_loginbtn_OnClick();
				  end;
				  
				  result=1;
			  
			  end;
			  
			  return result;		  
			  end;

Entry_OnAccel_OnEscape=function()
	  if Win_IsVisible("UI.SysMsgbox") then
	  Win_EndWidget("UI.SysMsgbox",false);					  
	  else 
	  Entry_btnpanel_quitbtn_OnClick();
	  end;
	  
	 return 1;
	 end;
					 
Room_OnDenit = function()	
		Win_ShowWidget("UI.room",false);
		return 1;			   
		end;
			
Room_OnInit = function()	
		Win_SetValue("UI.room",0,value_channel_alphafactor);
		Win_ShowWidget("UI.room",true);
		Win_SetFocus("UI.room");
		return 1;			   
		end;
	 
Room_OnAccel_OnTab=function()
			return 1;		  
			  end;

Entry_btnpanel_idedit_OnMouseIn=function()
	Win_SetText("UI.entry.infolabel","请输入您的QQ号码!");
	end;	
	
Entry_btnpanel_pwdedit_OnMouseIn=function()
	Win_SetText("UI.entry.infolabel","请输入您的QQ密码!");
	end;
	
Entry_btnpanel_recordcheck_OnMouseIn=function()
	Win_SetText("UI.entry.infolabel","下次登录是否自动填入帐号?");
	end;		
	
	
Entry_btnpanel_hidecheck_OnMouseIn=function()
	Win_SetText("UI.entry.infolabel","是否隐身登录?");
	end;	

Entry_btnpanel_loginbtn_OnMouseIn=function()
	Win_SetText("UI.entry.infolabel","Enter登录游戏!");
	end;
			
Entry_btnpanel_loginbtn_OnClick = function()	
	account = Win_GetText( "UI.entry.btnpanel.idedit" );
	--if Login(account,"UI.entry.btnpanel.pwdedit")==1 then
	GotoUIScreen("room","RoomPlatForm"); 
	--else
	--Win_ShowMsgBox("帐号密码都必须非空!请任意输入非空的帐号密码！");
	--end;
	end;			

Entry_btnpanel_quitbtn_OnMouseIn=function()
	Win_SetText("UI.entry.infolabel","退出游戏并关闭客户端!");
	end;

Show_ListText=function()
	Win_SetText("UI.entry.textlist","3333",value_channel_itemtext);
	Win_SetText("UI.entry.textlist","4444",value_channel_itemtext);
	Win_SetText("UI.entry.textlist","5555",value_channel_itemtext);
	end;

Entry_btnpanel_quitbtn_OnClick = function() 				      
	Win_ShowMsgBox("确实要退出游戏吗?","Message",ui_msgid_yesno,"UI.entry.quitbtn");	 
	end;	
	      
Entry_btnpanel_quitbtn_OnMsgBoxOk=function() 
	Quit(); 
	end;
	

Gameui_OnAccel_OnEscape=function()
					
										  
	  if Win_IsVisible("UI.SysConsole") then
	  Win_ToggleConsole();
	  elseif Win_IsVisible("UI.SysMsgbox") then
	  Win_ShowWidget("UI.SysMsgbox",false);
	  else 
	  --QuitGame();
	  --GotoUIScreen("entry");
	  end;
	  
	  return 1;
	 end;

--[[	 
Gameui_OnAccel_OnVKLeft = function()
	if Win_IsVisible("UI.gameui.chooseanswerdlg") then
		UI.gameui.chooseanswerdlg.Answer1.OnClick();
	end;
end;

Gameui_OnAccel_OnVKUp = function()
	if Win_IsVisible("UI.gameui.chooseanswerdlg") then
		UI.gameui.chooseanswerdlg.Answer2.OnClick();
	end;
end;

Gameui_OnAccel_OnVKRight = function()
	if Win_IsVisible("UI.gameui.chooseanswerdlg") then
		UI.gameui.chooseanswerdlg.Answer3.OnClick();
	end;
end;

Gameui_OnAccel_OnVKDown = function()
	if Win_IsVisible("UI.gameui.chooseanswerdlg") then
		UI.gameui.chooseanswerdlg.Answer4.OnClick();
	end;
end;
--]]
	 
Gameui_OnDenit = function()	
	   QuitGame();
	   Win_ShowWidget("UI.gameui",false);
	   return 1;			   
	   end;
	
Gameui_OnInit = function()	
	   NewGame("map/starcraft.lua");
	   Win_SetValue("UI.gameui",0,value_channel_alphafactor);
	   Win_ShowWidget("UI.gameui",true);
	   Win_SetFocus( "UI.gameui" ); 
	   return 1;			   
	   end;
	 


Gameui_OnOpenNumInput=function(maxnum,defvalue,name)
Win_SetValue("UI.gameui.numinputdlg",0,value_channel_customtype);
Win_SetValue("UI.gameui.numinputdlg.numslider",maxnum,value_channel_maxvalue);
Win_SetValue("UI.gameui.numinputdlg.numedit",maxnum,value_channel_maxvalue);
Win_SetValue("UI.gameui.numinputdlg.numslider",defvalue);
Win_SetValue("UI.gameui.numinputdlg.numedit",defvalue);						   
Win_ShowWidget("UI.gameui.numinputdlg",true);	
end;
GotoPlatForm = function(platformname)
	OnUIMessage(20000, platformname);
end;