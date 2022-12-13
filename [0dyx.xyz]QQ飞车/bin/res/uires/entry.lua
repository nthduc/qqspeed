UI.children.entry = 
{
	type = "SCREEN",
	rect = { 0, 0, 1024, 768},
	visible=0,
	bkimage="res/uires/entry/bk.gsa",
	autoscale = 1,
	updatemute = 1,
	
	OnInit = function()	
		Win_ShowWidget("UI.entry",true);			
		return 1;			   
	end;
	 
	OnDenit = function()	
		Win_ShowWidget("UI.entry",false);
		return 1;			   
	end;
	
	OnSelfShow = function()
		cmd_hideselfInfo();
		cmd_hidenavigation();
		RemoveSelfKartFromUIScene("ROOT");
		RemoveSelfRoleFromUIScene("playermode");
		ReBindSelfPlayer();
	end;
		
	OnEnter=function()
		UI.children.entry.children.logindlg.OnEnter();
	end;
	
	children = 
	{	
		logindlg=
		{
			type="WIDGET",
			rect={392,138,562,383},
			--bkimage="res/uires/entry/logindlg.gsa",
			visible=0;
			
			OnInit=function()
			--[[
				Win_SetText( "UI.entry.logindlg.pwdedit","");		
				local idtxt, pwdtxt=GetLastUserName();
				if idtxt ~= nil and idtxt ~= "" then
					--Win_ShowWidget("UI.entry.logindlg.idedit.accountlabel",false);
					Win_SetText( "UI.entry.logindlg.idedit",idtxt);
				end;
				if pwdtxt ~= nil and pwdtxt ~= "" then
					--Win_ShowWidget("UI.entry.logindlg.pwdedit.PasswordLabel",false);
					Win_SetText( "UI.entry.logindlg.pwdedit", pwdtxt);
				end;
				
				local brecord=GetGameVar("saveuin");
				Win_SetCheck("UI.entry.logindlg.recordcheck",(brecord==1));]]
			end;
			
			OnEnter=function()
				UI.children.entry.children.logindlg.children.loginbtn.OnClick();
			end;
			
			children=
			{
				idedit = 
				{			
					type = "EDIT",
					rect = { 204,164,200,23 },		
					focuscolor = { 0, 0, 0, 255 },
					captionrect={5,4,190,16},
					textsize=16,
					textstyle=dt_left+dt_top,
					editmethod = ui_editmethod_number,	
					textfont=1,
					maxchar=20;
					--tiptext="«Î ‰»ÎQQ∫≈",
					--bkimage="res/uires/entry/inputbg.gsa",
					mornitor={value_channel_caption},
					
					OnUpdate=function()
						--[[if Win_GetText("UI.entry.logindlg.idedit") == "" then
							Win_ShowWidget("UI.entry.logindlg.idedit.accountlabel",true);
						else
							Win_ShowWidget("UI.entry.logindlg.idedit.accountlabel",false);
						end;]]
					end;
					
					OnMouseIn=function()
						--Win_SetBackPic("UI.entry.logindlg.idedit","res/uires/entry/inputbg_hover.gsa");
					end;
					
					OnMouseOut=function()
						--Win_SetBackPic("UI.entry.logindlg.idedit","res/uires/entry/inputbg.gsa");
					end;
					
					OnLostFocus=function()
						--[[local txt = Win_GetText("UI.entry.logindlg.idedit");
						if txt == "" then
							Win_ShowWidget("UI.entry.logindlg.idedit.accountlabel",true);
						end;]]
					end;
					
					OnGainFocus=function()
						--Win_ShowWidget("UI.entry.logindlg.idedit.accountlabel",false);
					end;
					
					OnTab=function()
						Win_SetFocus("UI.entry.logindlg.pwdedit");
					end;
					
					children=
					{
					--[[
						accountlabel=
						{
							type="LABEL",
							rect={0,0,170,22},
							captionrect={5,3,160,22},
							caption="QQ∫≈¬Î",
							alignstyle=alignstyle_left_in,
							textstyle=dt_left,
							textfont=1,
							tiptext="«Î ‰»ÎQQ∫≈",
					
							OnClick=function()
								--Win_ShowWidget("UI.entry.logindlg.idedit.accountlabel",false);
								--Win_SetFocus("UI.entry.logindlg.idedit");
							end;							
							
							OnMouseIn=function()
							end;
						};
						idlistbtn=
						{
							type="BUTTON",
							rect={153,0,18,22},
							textsize=16,
							bkimage="res/uires/entry/iddropbtn.gsa",
							bkimagepos={0,-1},
							
							OnClick=function()
								if Win_IsVisible("UI.entry.logindlg.idlist") then
									Win_ShowWidget("UI.entry.logindlg.idlist",false);
								else
									Win_ShowWidget("UI.entry.logindlg.idlist",true);
								end;
							end;
							
							OnMouseIn=function()
							end;
						};]]
					};
				};
				pwdedit= 
				{
					type = "EDIT",
					rect = { 204,220,200,23 },
					textsize=16,
					captionrect={5,4,190,16},
					textstyle=dt_left+dt_top,
					editmethod = ui_editmethod_english+ui_editmethod_chinese+ui_editmethod_number+ui_editmethod_symbol+ui_editmethod_coder,	
					textfont=1,
					maxchar=16;
					--tiptext="«Î ‰»Î√‹¬Î",
					--bkimage="res/uires/entry/inputbg.gsa",
					
					OnMouseIn=function()
						--Win_SetBackPic("UI.entry.logindlg.pwdedit","res/uires/entry/inputbg_hover.gsa");
					end;
					
					OnMouseOut=function()
						--Win_SetBackPic("UI.entry.logindlg.pwdedit","res/uires/entry/inputbg.gsa");
					end;
					
					OnLostFocus=function()
						--[[local txt = Win_GetText("UI.entry.logindlg.pwdedit");
						if txt == "" then
							Win_ShowWidget("UI.entry.logindlg.pwdedit.PasswordLabel",true);
						end;]]
					end;
					
					OnGainFocus=function()
						--Win_ShowWidget("UI.entry.logindlg.pwdedit.PasswordLabel",false);
					end;
					
					OnTab=function()
						Win_SetFocus("UI.entry.logindlg.idedit");
					end;
					
					children=
					{
					--[[
						PasswordLabel=
						{
							type="LABEL",
							rect={0,0,170,22},
							captionrect={5,3,160,22},
							caption="√‹¬Î",
							textstyle=dt_left,
							textfont=1,
							alignstyle=alignstyle_left_in,
							tiptext="«Î ‰»Î√‹¬Î",
							
							OnClick=function()
								--Win_ShowWidget("UI.entry.logindlg.pwdedit.PasswordLabel",false);
								--Win_SetFocus("UI.entry.logindlg.pwdedit");
								
							end;							
							
							OnMouseIn=function()
							end;
						};	]]					
					};
				};
							
				idlist =
				{
					type="LISTBOX",
					rect={123,67,170,120},
					clientrect={5,0,160,120},
					style=wgtstyle_popup+wgtstyle_topmost+wgtstyle_vertical + wgtstyle_clipself,
					visible=0,
					initlayer=msgbox_layer,
					bkcolor= { 100,100,100, 255},
					itemfocusback="res/uires/entry/inputbg_hover.gsa",
					itemfocuspos={0,0,0,-1},
					textsize=16,
					itemtextstyle=dt_left+dt_vcenter+dt_magicedge;
					textfont=1, 
					patchupmiddle="res/uires/entry/drop_top.gsa",
					patchmiddle="res/uires/entry/drop_mid.gsa",
					patchdownmiddle="res/uires/entry/drop_bottom.gsa",
					visible=0,
					
					OnInit=function()
					--[[
						local usercount, users = GetRecentUsers();
						if usercount > 0 then
							Win_SetValue("UI.entry.logindlg.idlist", usercount, value_channel_item_num);
							for i = 1, usercount, 1 do
							Win_SetText("UI.entry.logindlg.idlist",users[i].uin,value_channel_itemtext,i-1);	
							end;
						end;]]
					end;
					
					OnClick=function()
						local sel = Win_GetValue("UI.entry.logindlg.idlist",value_channel_hot);
						local seltext = Win_GetText("UI.entry.logindlg.idlist", value_channel_itemtext, sel);
						Win_SetText("UI.entry.logindlg.idedit", seltext);
						Win_ShowWidget("UI.entry.logindlg.idlist",false);
					end;
				};
				
				recordcheck= 
				{
					type = "CHECK",
					rect={121,116,120,16};
					textsize=12,
					textstyle=dt_left,
					drawcolor={0,0,0,255},
					bkimage = "res/uires/common/checklabel.gsa",
					visible=0,
					
					OnCheck=function(bcheck)									   
						SetGameVar("saveuin",bcheck);
					end;						
						
					OnMouseIn=function()
						--Win_SetText("UI.entry.infolabel","œ¬¥Œµ«¬º «∑Ò◊‘∂ØÃÓ»Î’ ∫≈?");
					end;					
				};
				
				keyboardbtn = 
				{
					type = "CHECK",
					rect = { 300,86,22,20 },
					bkimage = "res/uires/entry/keyboardbtn.gsa",
					extendstyle=ui_btn_style_none,
					tiptext="»Ìº¸≈Ã",
					soundid={1006},
					visible=0,
					
					OnMouseIn=function()
					end;
								
					OnClick=function()
					     if  Win_IsChecked("UI.entry.logindlg.keyboardbtn")  then
						Win_ShowWidget("UI.entry.keyboarddlg",true);
					     else 
						Win_ShowWidget("UI.entry.keyboarddlg",false);
					     end;						     
					end;					    								    
						    
				};
				loginbtn = 
				{
					type = "BUTTON",
					rect = { 90,305,160,60},
					--bkimage = "res/uires/entry/loginbtn.gsa",
					soundid={1006},
					
					OnMouseIn=function()
					end;
								
					OnClick = function()	
						local account = Win_GetText( "UI.entry.logindlg.idedit" );		
						local pwd = Win_GetText( "UI.entry.logindlg.pwdedit" );		
						if account =="" then
							--Win_ShowMsgBox("«Î ‰»Î”√ªß√˚");
							local rc = Win_GetRect("UI.entry.logindlg.idedit");
							--SetCursorPos(rc.x+rc.w-20, rc.y+rc.h/2);
							return;
						end;
						if pwd == "" then
							--Win_ShowMsgBox("«Î ‰»Î√‹¬Î");
							local rc = Win_GetRect("UI.entry.logindlg.pwdedit");
							--SetCursorPos(rc.x+rc.w-20, rc.y+rc.h/2);
							--Win_ShowTooltip("UI.entry.logindlg.pwdedit.PasswordLabel");
							return;
						end;
						Login(account, "UI.entry.logindlg.pwdedit");
						Win_ShowWidget("UI.SysLoginMsgbox", true);
					end;					    								    
						    
				};
				leavebtn=
				{
					type="BUTTON",
					rect={300,305,160,60},
					--bkimage = "res/uires/entry/leavebtn.gsa",
					
					OnClick=function()
						ExitApp();
					end;
				};
				
			};
		
		};
		--[[
		words=
		{
			type="LABEL",
			rect={3,0,1024,20},
			bkimage="res/uires/entry/16words.gsa",
		};
		copyrightlabel=
		{
			type="LABEL",
			rect={300,700,429,21},
			bkimage="res/uires/entry/copyrights.gsa",
		};
	
		]]
		keyboarddlg=
		{
			type="DIALOG",
			rect={242,150,313,168},
			--bkimage="res/uires/entry/keyboard.gsa",
			style=wgtstyle_popup+wgtstyle_topmost,
			visible=0,			
			mornitor={value_channel_visible},
			
			OnUpdate=function()
				if not Win_IsVisible("UI.entry.keyboarddlg")  then											
					if Win_IsChecked("UI.entry.logindlg.keyboardbtn") then
						Win_SetCheck("UI.entry.logindlg.keyboardbtn",false);									
					end;						
				end;
			end;	
			
			OnInit=function()
				Win_ClearTemplates("UI.entry.keyboarddlg.keypanelup");
				Win_ClearTemplates("UI.entry.keyboarddlg.keypaneldn");
				Win_AddTemplate("UI.entry.keyboarddlg.keypanelup",0,0,46);
				Win_AddTemplate("UI.entry.keyboarddlg.keypaneldn",0,0,46);
			end;
				
			children=
			{
				btn_space=
				{
					type="PICLABEL",
					rect={17,113,78,22},
					captionrect={20,3,60,16},
					caption="Space",
					fontpic = "res/uires/common/fontpic.gsa",
					bkimage = "res/uires/entry/keyspace.gsa",
					drawcolor={0,0,0,255},
					captionoffset={{1,1}},
					framescheme={{1,0,0,0}},
					extendstyle=ui_btn_style_popupmsg,
					
					OnClick=function()						
						Win_OnInsert("UI.entry.logindlg.pwdedit"," ");
						Win_SetFocus("UI.entry.logindlg.pwdedit");
					end;
				};
				
				btn_shift=
				{
					type="PICLABEL",
					rect={95,113,68,22},
					captionrect={16,3,48,16},
					caption="Shift",
					fontpic = "res/uires/common/fontpic.gsa",
					bkimage = "res/uires/entry/keyshift.gsa",
					drawcolor={0,0,0,255},
					captionoffset={{1,1}},
					framescheme={{1,0,0,0}},
					extendstyle=ui_btn_style_popupmsg,
					
					OnClick=function()												
						Win_ToggleWidget("UI.entry.keyboarddlg.keypanelup",0);
						Win_ToggleWidget("UI.entry.keyboarddlg.keypaneldn",0);
						Win_SetFocus("UI.entry.logindlg.pwdedit");
					end;
				};
				
				btn_back=
				{
					type="PICLABEL",
					rect={164,113,76,22},
					captionrect={8,3,70,16},
					caption="BackSpace",
					fontpic = "res/uires/common/fontpic.gsa",
					bkimage = "res/uires/entry/keyback.gsa",
					drawcolor={0,0,0,255},
					captionoffset={{1,1}},
					framescheme={{1,0,0,0}},
					extendstyle=ui_btn_style_spin,
										
					OnClick=function()												
						Win_SendMessage("UI.entry.logindlg.pwdedit",msgtype_keydown,8,0);
					end;
				};
				
				btn_enter=
				{
					type="PICLABEL",
					rect={242,88,52,48},
					captionrect={8,27,40,16},
					caption="Enter",
					fontpic = "res/uires/common/fontpic.gsa",
					bkimage = "res/uires/entry/keyenter.gsa",
					drawcolor={0,0,0,255},
					winshape={{ shape_type_rect,1,0,26,52,22 },{ shape_type_rect,1,30,0,22,26 }},
					captionoffset={{1,1}},
					framescheme={{1,0,0,0}},
					extendstyle=ui_btn_style_popupmsg,
					
					OnClick=function()						
						Win_ShowWidget("UI.entry.keyboarddlg",false);
						Win_SetFocus("UI.entry.logindlg.pwdedit");
					end;
				};
				
				keypanelup=
				{
					type="DIALOG",
					rect={0,0,313,10},					
					template={"res/uires/keyitemup.lua"};
					pluslayer=0,					
					visible=0,
					children={};
				};
				
				keypaneldn=
				{
					type="DIALOG",
					rect={0,0,313,10},					
					template={"res/uires/keyitemdn.lua"};				
					pluslayer=0,
					children={};
				};
				
			};		
		};
		
	}--children
};--entry
	
	