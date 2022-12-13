UI.children.taskbar=
{
	type="DIALOG",
	rect={0,768-55,800,30},
	drawflag=drawflag_win_fill,
	bkcolor={0,50,200,200},
	initlayer=msgbox_layer,
	visible=0,
	
	children=
	{
		MenuBtn=
		{
			type="BUTTON",
			rect={15,0,75,30},
			bkimage="res/uires/taskbar/menubtn.gsa",
			bkimagepos={0,2},
			caption="�˵�",
			textsize=12,
			
			OnClick=function()
				cmd_showmenu();
			end;
		};
		
		BackBtn=
		{
			type="BUTTON",
			rect={90,0,32,30},
			bkimage="res/uires/taskbar/backbtn.gsa",
			textsize=12,
			
			OnClick=function()
				cmd_goback();
			end;
		};
		
		GarageBtn=
		{
			type="BUTTON",
			rect={128,0,110,30},
			bkimage="res/uires/taskbar/garagebtn.gsa",
			bkimagepos={0,5},
			caption="����",
			textsize=12,
			
			OnClick=function()
				cmd_gotogarage();
			end;
		};

		ShopBtn=
		{
			type="BUTTON",
			rect={239,0,110,30},
			bkimage="res/uires/taskbar/shopbtn.gsa",
			bkimagepos={0,5},
			caption="�̵�",
			textsize=12,

			OnClick=function()
				cmd_gotoshop();
			end;
		};

		TinBtn=
		{
			type="BUTTON",
			rect={349,0,110,30},
			bkimage="res/uires/taskbar/tinbtn.gsa",
			bkimagepos={0,5},
			caption="��Ʒ��",
			textsize=12,

			OnClick=function()
				cmd_opentin();
			end;
		};
		
		SysTray=
		{
			type="WIDGET",
			rect={660,0,120,30},
			
			children=
			{
				SetBtn=
				{
					type="BUTTON",
					rect={0,0,30,30},
					bkimage="res/uires/taskbar/setbtn.gsa",
					
					OnClick=function()
						cmd_set();
					end;
				};
				FriendsBtn=
				{
					type="BUTTON",
					rect={30,0,30,30},
					bkimage="res/uires/taskbar/friendsbtn.gsa",
					
					OnClick=function()
						cmd_showfriends();
					end;
				};
				InfoBtn=
				{
					type="BUTTON",
					rect={60,0,30,30},
					bkimage="res/uires/taskbar/infobtn.gsa",
					
					OnClick=function()
						cmd_showmyinfo();
					end;
				};
				ExitBtn=
				{
					type="BUTTON",
					rect={90,0,30,30},
					bkimage="res/uires/taskbar/exitbtn.gsa",
					
					OnClick=function()
						cmd_exit();
					end;
				};
			};
		};
	};
};

UI.children.MainMenu=
{
	type="WIDGET",
	rect={5,178,195,382},
	style=wgtstyle_popup,
	drawflag=drawflag_win_fill,
	bkcolor={200,200,200,220},
	initlayer=msgbox_layer-1,
	visible=0,
			
	Init=function()
		local myinfo=GetMyInfo();
		Win_SetText("UI.MainMenu.NameLabel",myinfo.playerdbinfo.nickname);
	end;
	
	children=
	{
		NameLabel=
		{
			type="LABEL",
			rect={10,10,175,20},					
		};
		MutiPlayBtn=
		{
			type="BUTTON",
			rect={20,35,158,40},
			bkimage="res/uires/startmenu/mutiplaybtn_s.gsa",
			textsize=12,
			caption="������Ϸ",
			
			OnClick=function()
				cmd_mutiplay();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
		GPBtn=
		{
			type="BUTTON",
			rect={20,75,158,40},
			enable=0,
			bkimage="res/uires/startmenu/gpbtn_s.gsa",
			textsize=12,
			
			OnClick=function()
				cmd_gp();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
		LicenceTestBtn=
		{
			type="BUTTON",
			rect={20,115,158,40},
			bkimage="res/uires/startmenu/licencetestbtn_s.gsa",
			textsize=12,
			caption="���տ���",

			OnClick=function()
				cmd_licencetest();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
		TimeRaceBtn=
		{
			type="BUTTON",
			rect={20,155,158,40},
			bkimage="res/uires/startmenu/timeracebtn_s.gsa",
			textsize=12,
			caption="��ʱ��",

			OnClick=function()
				cmd_timerace();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
		PlayRecordBtn=
		{
			type="BUTTON",
			rect={20,195,158,40},
			bkimage="res/uires/startmenu/playrecordbtn_s.gsa",
			textsize=12,
			caption="�ط�",

			OnClick=function()
				cmd_playrecord();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
		SetBtn=
		{
			type="BUTTON",
			rect={20,235,158,40},
			bkimage="res/uires/startmenu/setbtn_s.gsa",
			textsize=12,
			caption="����",
			
			OnClick=function()
				cmd_set();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
		BackBtn=
		{
			type="BUTTON",
			rect={20,275,158,40},
			bkimage="res/uires/startmenu/backbtn_s.gsa",
			textsize=12,
			caption="����",
			
			OnClick=function()
				cmd_goback();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
		ExitBtn=
		{
			type="BUTTON",
			rect={20,315,158,40},
			bkimage="res/uires/startmenu/exitbtn_s.gsa",
			textsize=12,
			caption="����",
			
			OnClick=function()
				cmd_exit();
				Win_EndWidget("UI.MainMenu",true);
			end;
		};
	};
};

UI.children.SysSetDlg=
{
	type="MESSAGEBOX",
	rect={0,0,200,100},
	drawflag=drawflag_win_fill,
	bkcolor={200,200,200,220},
	textsize=12,
	visible=0,
	initlayer=msgbox_layer-1,
	caption="ϵͳ���öԻ���",
	
	children=
	{
		ConfirmBtn=
		{
			type="BUTTON",
			rect={25,80,50,20},
			caption="ȷ��",
			textsize=12,
			
			OnClick=function()
				Win_EndWidget("UI.SysSetDlg",true);
			end;
		};
		
		CancelBtn=
		{
			type="BUTTON",
			rect={125,80,50,20},
			caption="ȡ��",
			textsize=12,
			
			OnClick=function()
				Win_EndWidget("UI.SysSetDlg",false);
			end;
		};
	};
};

UI.children.SysExitDlg=
{
	type="MESSAGEBOX",
	rect={0,0,185,160},
	drawflag=drawflag_win_fill,
	bkcolor={200,200,200,220},
	initlayer=msgbox_layer,
	visible=0,
	
	children=
	{
		GotoStartMenuBtn=
		{
			type="BUTTON",
			rect={30,45,45,60},
			bkimage="res/uires/sysdlg/gohomebtn.gsa",
			
			OnClick=function()
				GotoUIScreen("startmenu");
				Win_EndWidget("UI.SysExitDlg",true);
			end;
		};
		ExitBtn=
		{
			type="BUTTON",
			rect={110,45,45,60},
			bkimage="res/uires/sysdlg/exitbtn.gsa",
			
			OnClick=function()
				Win_EndWidget("UI.SysExitDlg",true);
				ExitApp();
			end;
		};
	};
};

UI.children.SysMyInfoDlg=
{
	type="MESSAGEBOX",
	rect={225,80,345,425},
	initlayer=msgbox_layer-1,
	style=wgtstyle_popup,
	drawflag=drawflag_win_fill,
	bkcolor={200,200,200,200},
	visible=0,
	
	OnSelfShow=function()
		UI.children.SysMyInfoDlg.Refresh();
	end;
	
	Refresh=function()
		local level=GetMyInfo().level;
		Win_SetBackPic("UI.SysMyInfoDlg.LicencePanel",leveldesc[level].licence);
		
		local name=GetMyInfo().playerdbinfo.nickname;
		Win_SetText("UI.SysMyInfoDlg.LicencePanel.NameLabel","���� :"..name);
		
		Win_SetText("UI.SysMyInfoDlg.LicencePanel.DateLabel","���ִ������� :");
		Win_SetText("UI.SysMyInfoDlg.LicencePanel.LevelLabel","��ʻִ�յȼ� :"..leveldesc[level].licencelevelstring);

		local money=0; --GetMyInfo().money;
		Win_SetText("UI.SysMyInfoDlg.InfoPanel.MoneyEdit",""..money.."���");
	
		local expn=GetMyInfo().raceexp;
		Win_SetText("UI.SysMyInfoDlg.InfoPanel.ExpEdit",expn.."����ֵ");
	
	end;
	
	children=
	{
		LicencePanel=
		{
			type="LABEL",
			rect={10,30,320,220},
			drawflag=drawflag_win_fill,
			bkcolor={100,100,100,100},
			
			children=
			{
				PortraitLabel=
				{
					type="LABEL",
					rect={20,60,86,100},
					--bkimage="res/uires/common/fakerole.gsa",	
					--autoscale=1,
					style=wgtstyle_renderitem,
					hosttype=1,
					hostindex=-1,
					drawflag=drawflag_clientprior,
			
				};
				
				NameLabel=
				{
					type="LABEL",
					rect={120,70,150,20},
					textsize=12,
					textstyle=dt_left,
				};
				DateLabel=
				{
					type="LABEL",
					rect={120,100,150,20},
					textsize=12,
					textstyle=dt_left,
				};
				LevelLabel=
				{
					type="LABEL",
					rect={120,130,150,20},
					textsize=12,
					textstyle=dt_left,
				};
			};
		};
		InfoPanel=
		{
			type="WIDGET",
			rect={10,260,320,120},
			
			children=
			{
				LicenceLabel=
				{
					type="LABEL",
					rect={5,5,100,20},
					caption="�ҵļ�ʻִ��",
					textsize=12,
					textstyle=dt_left,
				};
				LicenceEdit=
				{
					type="EDIT",
					rect={135,5,175,20},
					textsize=12,
					textstyle=dt_left,
					editable=0,
				};
				
				MotorcadeLabel=
				{
					type="LABEL",
					rect={5,35,100,20},
					caption="�ҵĳ���",
					textsize=12,
					textstyle=dt_left,
				};
				MotorcadeEdit=
				{
					type="LABEL",
					rect={135,35,175,20},
					textsize=12,
					textstyle=dt_left,
					editable=0,
				};
				
				MoneyLabel=
				{
					type="LABEL",
					rect={5,65,100,20},
					caption="���",
					textstyle=dt_left,
					textsize=12,
				};
				MoneyEdit=
				{
					type="EDIT",
					rect={135,65,175,20},
					textsize=12,
					textstyle=dt_left,
					editable=0,
				};
				
				ExpLabel=
				{
					type="LABEL",
					rect={5,95,100,20},
					caption="����ֵ",
					textsize=12,
					textstyle=dt_left,
				};
				ExpEdit=
				{
					type="EDIT",
					rect={135,95,175,20},
					textsize=12,
					textstyle=dt_left,
					editable=0,
				};
			};
			
		};
		ConfirmBtn=
		{
			type="BUTTON",
			rect={135,385,70,20},
			caption="ȷ��",
			textsize=12,
			
			OnClick=function()
				local myid="UI.SysMyInfoDlg";
				Win_EndWidget(myid,true);
			end;
		};
	};
};


UI.children.SysPInfoDlg=
{
	type="MESSAGEBOX",
	rect={225,80,589,346},
	initlayer=msgbox_layer-1,
	visible=0,
	bkimage="res/uires/roomsel/find/pinfobk.gsa",
	style=wgtstyle_topmost+wgtstyle_renderbuffer;
	--drawflag = drawflag_clientprior,
	drawflag = drawflag_backprior,
	custombuffer = 1,
	buffervalue =
	{
		banimation = 1,
		cycletype = 2, 
		restype = 1, 
		resname="scene/splash.nif",			
	},
	
	_uin=0,
	_nickname="",
	_expn=0,
	_level=0,
	_status=0,
	_roomflag=0,
	_online=0,
	_roleid=10000;
	_svrid=0;
	_roomview=0;
	
	OnSelfShow=function()
		Win_SetCapture("UI.SysPInfoDlg");
		UI.children.SysPInfoDlg.Refresh();
	end;
	OnSelfHide=function()
		Win_SetCapture();
	end;
	
	Refresh=function()
		local level=UI.children.SysPInfoDlg._level;
		local uin=UI.children.SysPInfoDlg._uin;
		local nickname=UI.children.SysPInfoDlg._nickname;
		local expn=UI.children.SysPInfoDlg._expn;
		local status=UI.children.SysPInfoDlg._status;
		local online=UI.children.SysPInfoDlg._online;
		local roleid=UI.children.SysPInfoDlg._roleid;
		local roomview=UI.children.SysPInfoDlg._roomview;
		
		--���εȼ�ͼ��
		--Win_SetBackPic("UI.SysPInfoDlg.LicencePanel",leveldesc[level].licence);
		Win_SetText("UI.SysPInfoDlg.LicencePanel.QQLabel", ""..uin);
		Win_SetText("UI.SysPInfoDlg.LicencePanel.NameLabel",nickname);
		Win_SetText("UI.SysPInfoDlg.LicencePanel.DateLabel","���ִ������� :");
		--Win_SetText("UI.SysPInfoDlg.LicencePanel.LevelLabel","��ʻִ�յȼ� :"..leveldesc[level].licencelevelstring);
		Win_SetText("UI.SysPInfoDlg.LicencePanel.LevelLabel","�ȼ���lv"..level);
		Win_SetText("UI.SysPInfoDlg.InfoPanel.ExpEdit",expn.."����ֵ");
		Win_SetValue("UI.SysPInfoDlg.LicencePanel.PortraitLabel",roleid,value_channel_hostindex);
		
		local desc="";
		local canjoin=0;
		if online~=0 then
			desc="������Ϸ����";
			if status==15 then
				desc="������Ϸ����";
			end;
			if status==16 then
				desc="������Ϸ����";
				canjoin=1;
			end;
			if status==17 then
				desc="������Ϸ��";
			end;
			if status==18 then
				desc="������Ϸ�佱��";
			end;
		end;
		if online==0 then
			desc="������";
		end;
		if roomview==1 then
			desc="-";
			canjoin=0;
		end;
		
		if roomview==1 then
			Win_ShowWidget("UI.SysPInfoDlg.InfoPanel.RefreshBtn",false);
		end;
		if roomview==0 then
			Win_ShowWidget("UI.SysPInfoDlg.InfoPanel.RefreshBtn",true);
		end;
		
		Win_SetText("UI.SysPInfoDlg.InfoPanel.StatusLabel",desc);
		if canjoin==0 then
			Win_EnableWidget("UI.SysPInfoDlg.JoinGameBtn",false);
		end;
		if canjoin~=0 then
			Win_EnableWidget("UI.SysPInfoDlg.JoinGameBtn",true);
		end;
	end;
	
	children=
	{
		LicencePanel=
		{
			type="LABEL",
			rect={22,25,540,180},
			
			children=
			{
				PortraitLabel=
				{
					type="LABEL",
					rect={15,20,192,260},
					--style=wgtstyle_renderitem,
					hosttype=2,
					hostindex=-1,
					drawflag=drawflag_clientprior,
					--enable=0,
				};
				
				NameLabel=
				{
					type="LABEL",
					rect={287,53,237,20},
					textsize=12,
					textstyle=dt_left+dt_vcenter+dt_magicback+dt_magicedge,
					--drawcolor={0,0,0,255},
					--enable=0,
					
				};
				QQLabel = 
				{
					type="LABEL",
					rect={286,89,237,20},
					textsize=12,
					textstyle=dt_left+dt_vcenter+dt_magicback+dt_magicedge,
					--drawcolor={0,0,0,255},
					caption = "QQ",
					--enable=0,
				};
				DateLabel=
				{
					type="LABEL",
					rect={120,120,150,20},
					textsize=12,
					textstyle=dt_left+dt_vcenter+dt_magicback+dt_magicedge,
					--drawcolor={0,0,0,255},
					enable=0,
					visible = 0,
				};
				LevelLabel=
				{
					type="LABEL",
					rect={120,145,150,20},
					textsize=12,
					textstyle=dt_left+dt_vcenter+dt_magicback+dt_magicedge,
					--drawcolor={0,0,0,255},
					enable=0,
					visible = 0,
				};
				
			};
		};
		InfoPanel=
		{
			type="WIDGET",
			rect={234,234,320, 50},
			
			children=
			{
				
				RefreshBtn=
				{
					type="BUTTON",
					rect={250,10,60,30},
					bkimage="res/uires/roomsel/find/refreshbtn.gsa",
					
					OnClick=function()
						local uin=UI.children.SysPInfoDlg._uin;
						FindPlayer(uin);
					end;
				};
				ExpEdit=
				{
					type="EDIT",
					rect={105,7,175,20},
					textsize=12,
					textstyle=dt_left,
					editable=0,
					visible = 0,
				};
				MotorcadeEdit=
				{
					type="LABEL",
					rect={105,44,175,20},
					textsize=12,
					textstyle=dt_left,
					editable=0,
					visible = 0,
				};
				StatusLabel=
				{
					type="EDIT",
					rect={75,11,150,30},
					editable=0,
					textsize=12;
					textstyle=dt_left+dt_top+dt_magicback+dt_magicedge,
				};
			};
			
		};
		JoinGameBtn=
		{
			type="BUTTON",
			rect={30+100,345,100,45},
			bkimage="res/uires/roomsel/find/joingamebtn.gsa",
			enable = 1,
			
			OnClick=function()
				local roomflag=UI.children.SysPInfoDlg._roomflag;
				local roomid=UI.children.SysPInfoDlg._roomid;
				if roomflag==1 then
					UI.children.roomsel.children.MainFrame.children.InputPwdDlg._roomid=roomid;
					Win_ShowWidget("UI.roomsel.MainFrame.InputPwdDlg",true);
				end;
				if roomflag==0 then
					local svrid=UI.children.SysPInfoDlg._svrid;
					--LoginGame(logintype_multi,0,svrid);
					EnterRoom(roomid,0,svrid);
				end;
				local myid="UI.SysPInfoDlg";
				Win_EndWidget(myid,true);
			end;
		};
		
		AddFriendBtn=
		{
			type="BUTTON",
			rect={135+100,345,100,45},
			bkimage="res/uires/roomsel/find/addfriendbtn.gsa",
			enable=1,
			
			OnClick=function()
				local myid="UI.SysPInfoDlg";
				Win_EndWidget(myid,true);
			end;
		};
		ConfirmBtn=
		{
			type="BUTTON",
			rect={240+100,345,100,40},
			bkimage="res/uires/common/okbtn.gsa",
			textsize=12,
			
			OnClick=function()
				local myid="UI.SysPInfoDlg";
				Win_EndWidget(myid,true);
			end;
		};
	};
};