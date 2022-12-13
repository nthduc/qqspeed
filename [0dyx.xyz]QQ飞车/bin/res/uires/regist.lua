UI.children.Regist =
{
	type = "SCREEN",
	rect = {0,0,1024,768},	
	bkimage="res/uires/regist/bg.gsa",
	visible=0,		
	updatemute = 1,
	
	OnInit = function()	
		Win_ShowWidget("UI.Regist",true);
		return 1;			   
	end;	
	
	OnDenit = function()	
		Win_ShowWidget("UI.Regist",false);
		return 1;			   
	end;
	
	OnSelfShow = function()
		cmd_setcamera();
		PlayMusic("Data/Win32/SOUNDS/ui_circle.mp3",1,1);
	end;

	
	children=
	{
	
		titlebar=
		{
			type="WIDGET";
			rect={200,0,580,100},
			bkimage="res/uires/regist/title.gsa";
			alignstyle=alignstyle_right_in+alignstyle_top_in;
			aligntype=aligntype_father+aligntype_winrect;
		};
		
		RegistWgt=
		{
			type = "WIDGET",
			rect = {75, 768/768 * 180, 1024*400/1024, 768 * 400/768},
			moveable = 1,
			
			_sex=0;
			
			children = 
			{
				namebar =
				{
					type = "LABEL",
					rect = {0, 0, 1024*400/1024, 40},
					--caption = "姓名",
					--text = "姓名",
					bkimagepos={5,5};
					bkimage="res/uires/regist/姓名.gsa";
					captionrect={8,3,1024/1024 * 70,16},					
					--tiptext = "玩家名字只能由汉字、大小写字母以及数字组成",
				
					children = 
					{
						nameinput = 
						{
							type = "EDIT",
							rect  = {1024/1024 * 60, 0, 1024/1024 * 200, 40},
							textsize=16,
							textstyle=dt_left+dt_top,
							editmethod = ui_editmethod_english+ui_editmethod_chinese+ui_editmethod_number, --+ui_editmethod_symbol+ui_editmethod_coder,
							textfont=1,
							maxchar=16;
							--mornitor={value_channel_caption},
							captionrect={5,10,190,20},
							tiptext = "请输入要创建的角色名称",
							bkimage="res/uires/regist/inputbg.gsa";
							
						},
						checkbtn = 
						{
							type = "BUTTON",
							rect = {1024/1024 * 240, 0, 1024/1024 * 80, 40},
							textsize = 16,
							textfont=0,
							textstyle=dt_center+dt_top,
							--caption = "检测",
							bkimage="res/uires/regist/checkname.gsa";
							bkimagepos={0,-5};
							
							OnClick = function()
								local name = Win_GetText("UI.Regist.RegistWgt.namebar.nameinput");
								if (name == "") then
									Win_ShowMsgBox("名字不能为空");
									return;
								end;
								CheckName(name);
							end;
						},
					},
				},
				RegistSelPanel=
				{
					type="WIDGET";
					template={"res/uires/RegistSelItem.lua"};
					
					OnInit=function()
						Win_ClearTemplates("UI.Regist.RegistWgt.RegistSelPanel");
						Win_AddTemplate("UI.Regist.RegistWgt.RegistSelPanel",0,0,5);						
						--SelfChangeSelf(10021);				
						Win_SetValue("UI.Regist.RegistWgt.RegistSelPanel", 400, value_channel_timerspace);
					end;
					
					OnTimer = function()
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.UpdateData(0);
						--RemoveSelfKartFromUIScene("ROOT");
						--RemoveSelfRoleFromUIScene("playermode");
						--ReBindSelfPlayer();
						--SelfChangeSelf(RegistSelInfo[1][2][1].id);
						--SelfChangeSelf(RegistSelInfo[1][3][1].id);
						--SelfPlayAnimation(1104);
						--BindSelfKartToScene("ROOT");		
						--BindSelfRoleToScene("playermode");							
						Win_ShowWidget("UI.Regist.PlayerViewDlg", true);
					end;
					
					UpdateData=function(sex)
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem1._selindex = 0;
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem2._selindex = 0;
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem3._selindex = 0;
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem4._selindex = 0;
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem5._selindex = 0;
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem0.UpdateData(sex,0);
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem1.UpdateData(sex,1);
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem2.UpdateData(sex,2);
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem3.UpdateData(sex,3);
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem4.UpdateData(sex,4);					
						UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem5.UpdateData(sex,5);					
					end;
					
					children=
					{
					};
				};
				confirm_wgt =
				{
					type = "WIDGET",
					rect = {0, 768/768 * 340, 1024/1024 * 400,40},
					
					children = 
					{
						ok_btn = 
						{
							type = "BUTTON",
							rect  = {1024/1024 * 20, 0, 1024/1024 * 100, 40},
							--caption = "创建",
							bkimage="res/uires/common/okbtn.gsa";
							
							OnClick = function()
								local nickname=Win_GetText("UI.Regist.RegistWgt.namebar.nameinput");
								if nickname==nil or nickname == "" then
									Win_ShowMsgBox("名字不能为空");
									return;
								end;
								
								local sex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem0.GetSelID();
								local face=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem1.GetSelID();
								local hair=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem2.GetSelID();
								local suite=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem3.GetSelID();
								local color=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem4.GetSelID();
								local country=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem5.GetSelID();
								RegistUser(nickname, sex, face, hair, suite, color, country);
							end;
						},
						exit_btn =
						{
							type = "BUTTON",
							rect  = {1024/1024 * 180, 0, 1024/1024 * 100, 40},
							--caption = "退出",
							bkimage="res/uires/common/cancelbtn.gsa";
							
							OnClick = function()
								ExitApp();
								--GotoUIScreen("entry");
							end;
						},
					},
				},
			},
		},
		PlayerViewDlg=
		{
			type = "DIALOG",
			rect = {1024/1024 * 500, 768/768 * 145, 1024/1024 * 500, 768/768 * 500},
			captionrect = {1024/1024 * 250, 3, 1024/1024 * 100, 16 },
			--caption = "个人形象展示",
			--moveable = 1,
			visible = 0,
			_turnstate = 0,
			
			OnInit = function()
				Win_SetValue("UI.Regist.PlayerViewDlg",30,value_channel_timerspace);
			end;
			
			OnTimer = function()
				local turnstate = UI.children.Regist.children.PlayerViewDlg._turnstate;
				
				if (turnstate ~= 0) then
					if (UI.children.Regist.children.PlayerViewDlg.IsTurnStop()) then
						local ret = RecoverSelfTurn(turnstate);
						RecoverRoleTurn(turnstate);
						if (ret == 1) then
							UI.children.Regist.children.PlayerViewDlg._turnstate = 0;
						end;
					else
						SelfShowTurn(turnstate);
						SelfRoleShowTurn(turnstate);
					end;
					Win_SetValue("UI.Regist.PlayerViewDlg",30,value_channel_timerspace);
				end;					
			end;
			
			IsTurnStop = function()
				if (UI.children.Regist.children.PlayerViewDlg.children.left_btn._leftturn == 0 and UI.children.Regist.children.PlayerViewDlg.children.right_btn._rightturn == 0) then
					return true;
				end;
				return false;
			end;
			
			children = 
			{
				selfshow_wgt = 
				{
					type = "WIDGET",
					rect = {1024/1024 * 0, 20, 1024/1024 * 500, 768/768 * 460},
					style=wgtstyle_static+wgtstyle_renderitem,
					hosttype=6,
					hostindex=-1,
				},
				left_btn = 
				{
					type = "WIDGET",
					rect = {1024/1024 * 0, 400, 1024/1024 * 100, 768/768 * 100},
					--caption = "turn left",
					bkimage="res/uires/regist/leftrot.gsa";
					initlayer=msgbox_layer,
					_leftturn = 0,
										
					OnMouseIn = function()
						UI.children.Regist.children.PlayerViewDlg._turnstate = 1;
						UI.children.Regist.children.PlayerViewDlg.children.left_btn._leftturn = 1;							
						Win_SetValue("UI.Regist.PlayerViewDlg",30,value_channel_timerspace);
					end;
					
					OnMouseOut = function()
						UI.children.Regist.children.PlayerViewDlg.children.left_btn._leftturn = 0;
						
					end;
					
					
				},
				right_btn = 
				{
					type = "WIDGET",
					rect = {1024/1024 * 400, 400,1024/1024 * 100, 768/768 * 100},
					--caption = "turn right",
					bkimage="res/uires/regist/rightrot.gsa";
					initlayer=msgbox_layer,
					_rightturn = 0,
					
					
					OnMouseIn = function()
						UI.children.Regist.children.PlayerViewDlg._turnstate = -1;
						UI.children.Regist.children.PlayerViewDlg.children.right_btn._rightturn = 1;	
						Win_SetValue("UI.Regist.PlayerViewDlg",30,value_channel_timerspace);						
					end;
					
					OnMouseOut = function()
						UI.children.Regist.children.PlayerViewDlg.children.right_btn._rightturn = 0;
					end;
					
				},
			},
		};
		--[[
		RegistDlg=
		{
			type="MESSAGEBOX";
			rect={0,0,460,500},
			bkimage="res/uires/regist/bk.gsa",
			moveable=1,
			
			_roleid=10000,
			_colorid=10011,
			
			OnInit=function()
				UI.children.Regist.children.RegistDlg._roleid = 10000;
				UI.children.Regist.children.RegistDlg._colorid=10011;
				UI.children.Regist.children.RegistDlg.children.ShowLabel.Update();
			end;
			
			children=
			{
				name=
				{
					type="LABEL",
					rect={128,51,310,30},
					
					OnInit=function()
						Win_SetText("UI.Regist.RegistDlg.name",GetMyInfo().playerdbinfo.nickname);
					end;
				};
				rolebtn=
				{
					type="BUTTON";
					rect={20,100,100,100},
					bkimage="res/uires/regist/rolebtn.gsa",
					
					OnClick=function()
						UI.children.Regist.children.AvatarSelDlg.ShowRegist(1);
					end;
				};
				colorbtn=
				{
					type="BUTTON";
					rect={20,200,100,100};
					bkimage="res/uires/regist/colorbtn.gsa",
					
					OnClick=function()
						UI.children.Regist.children.AvatarSelDlg.ShowRegist(2);
					end;
				};
				
				ShowLabel=
				{
					type="WIDGET";
					rect={125,100,315,355};
					
					style=wgtstyle_renderitem,
					hosttype=2,
					hostindex=10000,
					drawflag=drawflag_clientprior,
					
					Update=function()
						local roleid=UI.children.Regist.children.RegistDlg._roleid;
						Win_SetValue("UI.Regist.RegistDlg.ShowLabel",roleid,value_channel_hostindex);
					end;
				};
				
				okbtn=
				{
					type="BUTTON";
					rect={22,415,100,40};
					bkimage="res/uires/common/okbtn.gsa";
					
					OnClick=function()
						local roleid=UI.children.Regist.children.RegistDlg._roleid;
						local colorid=UI.children.Regist.children.RegistDlg._colorid;
						RegistUser(roleid,colorid);
					end;
				};
			};
		};
		
		
		AvatarSelDlg=
		{
			type="MESSAGEBOX",
			rect={0,0,701,519},
			bkimage="res/uires/avatarsel/bk.gsa",
			initlayer=msgbox_layer,
			visible=0,
			
			_avatarcat=1;
			_avatarindex=0;
			
			ShowRegist=function(cat)
				UI.children.Regist.children.AvatarSelDlg._avatarcat=cat;
				Win_ShowWidget("UI.Regist.AvatarSelDlg",true);
				UI.children.Regist.children.AvatarSelDlg.Update();
			end;
			
			Update=function()
				UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel.Update();
			end;
			
			OnSelfShow=function()
				Win_SetCapture("UI.Regist.AvatarSelDlg");
			end;
			
			OnSelfHide=function()
				Win_SetCapture();
			end;
			
			children=
			{
				TabBar=
				{
					type="WIDGETLIST",
					rect={86,40,530,50},
					style=wgtstyle_clipchild_window,
					extendstyle=wgtlist_style_enum_item;
					template={"res/uires/avatarselcattempl.lua"},
					
					children=
					{
						btn1=
						{
							type="BUTTON",
							rect={0,0,80,40},
							bkimage="res/uires/regist/all.gsa",
						};
					};
				};
				AvatarListPanel=
				{
					type="WIDGET",
					rect={33,89,632,350},
					_page=0,
					_npp=4;
					
					OnInit=function()
						UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel.Update();
					end;
					
					Update=function()
						local avatarcat=UI.children.Regist.children.AvatarSelDlg._avatarcat;
						local avatarcount=QueryRegistInfoCount(avatarcat);
						local page=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page;
						local npp=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._npp;
						local showcount=avatarcount-page*npp;
						if showcount<0 then
							showcount=0;
						end;
						if showcount>npp then
							showcount=npp;
						end;
						Win_ClearTemplates("UI.Regist.AvatarSelDlg.AvatarListPanel.list");
						Win_AddTemplate("UI.Regist.AvatarSelDlg.AvatarListPanel.list",0,0,showcount-1);
					end;
					
					children=
					{
						list=
						{
							type="WIDGETLIST",							
							rect={0,0,632,300},
							style=wgtstyle_clipchild_window,
							template={"res/uires/registlisttemp.lua"},
							
							children=
							{
							};
						};
						PrevBtn=
						{
							type="BUTTON",
							rect={0,310,50,30},
							bkimage="res/uires/Registsel/prevbtn.gsa",
							
							OnClick=function()
								local page=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page;
								if page>0 then
									UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page=page-1;
									UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel.Update();
								end;
							end;
						};
						NextBtn=
						{
							type="BUTTON",
							rect={582,310,50,30},
							bkimage="res/uires/Registsel/nextbtn.gsa",
							
							OnClick=function()
								local page=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page;
								local npp=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._npp;
								local avatarcat=UI.children.Regist.children.AvatarSelDlg._avatarcat;
								local avatarcount=QueryRegistInfoCount(avatarcat);
								if (page+1)*npp < avatarcount then
									UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page=page+1;
									UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel.Update();
								end;
							end;
						};
					};
				};
				ConfirmBtn=
				{
					type="BUTTON",
					rect={200,450,100,40},
					bkimage="res/uires/common/okbtn.gsa",
					
					OnClick=function()
						local page=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page;
						local npp=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._npp;
						local avatarcat=UI.children.Regist.children.AvatarSelDlg._avatarcat;
						local avatarindex=UI.children.Regist.children.AvatarSelDlg._avatarindex;
						local t=FindRegistInfo(avatarcat, avatarindex);
						if t~= nil then
							ChangeAvatarStatus(GetMyInfo().uin,t.itemid,1);
						end;
						Win_EndWidget("UI.Regist.AvatarSelDlg", true);
					end;
				};
				CloseBtn=
				{
					type="BUTTON",
					rect={365,450,100,40},
					bkimage="res/uires/common/cancelbtn.gsa",
					
					OnClick=function()
						Win_EndWidget("UI.Regist.AvatarSelDlg", false);
					end;
				};
			};
		};
		]]
	};
};

